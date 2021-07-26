<?php

use Twilio\Rest\Client;

function sirius_twilio_recording_fetch_local_nid($recording_sid) {
  if (!$recording_sid) { return NULL; }
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_log');
  $query->fieldCondition('field_sirius_category', 'value', 'twilio:recording');
  $query->fieldCondition('field_sirius_notes', 'value', $recording_sid);
  $query->propertyOrderBy('nid', 'desc');
  $query->range(0, 1);
  $stmt = $query->execute();
  if (!$stmt['node']) { return NULL; }
  return array_keys($stmt['node'])[0];
}

function sirius_twilio_recording_fetch_local($recording_sid) {
	return node_load(sirius_twilio_recording_fetch_local_nid($recording_sid));
}

function sirius_twilio_recording_fetch($recording_sid) {
	try {
		$client = sirius_twilio_client_get();
		$record = $client->recordings($recording_sid)->fetch();
		$result = array('success' => TRUE, 'recording' => $record->toArray());
	} catch (Exception $e) {
		$result = array('success' => FALSE, 'msg' => "Failed to fetch recording $recording_sid: " . $e->getCode() . ' - ' . $e->getMessage());
	}

	return $result;
}

function sirius_twilio_recording_fetch_from_call($call_sid) {
	try {
		$client = sirius_twilio_client_get();
		$recordings = $client->recordings->read(["callSid" => $call_sid], 1);

		$found = FALSE;
		$result = array();
		foreach ($recordings as $record) {
			$found = TRUE;
			$result['recording'] = $record->toArray();
			break;
		}
		if ($found) {
			$result['success'] = TRUE;
		} else {
			$result['success'] = FALSE;
			$result['msg'] = "Recording not found for call sid $call_sid.";
		}
	} catch (Exception $e) {
		$result = array('success' => FALSE, 'msg' => "Failed to fetch recording for call $call_sid: " . $e->getCode() . ' - ' . $e->getMessage());
	}

	return $result;
}

function sirius_twilio_recording_search($params = array(), $count = 1000, $page_url = NULL) {
	try {
		$client = sirius_twilio_client_get();

		if (!$params) { $params = array(); }
		$result = array();
		$result['records'] = array();

		if ($page_url) {
			$iterator = $client->recordings->getPage($page_url);
		} else {
			$iterator = $client->recordings->page($params, $count);
		}

		$result['nextPageUrl'] = $iterator->getNextPageUrl();
		$result['previousPageUrl'] = $iterator->getPreviousPageUrl();
		foreach ($iterator as $record) {
			$record_arr = $record->toArray();
			if ($fetch_call) {
				$record_arr['call'] = sirius_twilio_call_fetch($record_arr['callSid']);
			}
			$result['records'][$record_arr['sid']] = $record_arr;
		}
		$result['success'] = TRUE;
		return $result;
	} catch (Exception $e) {
		return array('success' => FALSE, 'msg' => 'Error: ' . $e->getCode() . ' - ' . $e->getMessage());
	}
}

function sirius_twilio_recording_pull($recording_sid, $params = array()) {
	// Fetch the recording
	$result = sirius_twilio_recording_fetch($recording_sid);
	if (!$result['success']) { return $result; }
	$recording = $result['recording'];

	// Fetch the call, both local and remote
	$call_sid = $recording['callSid'];
	$result = sirius_twilio_call_fetch($call_sid);
	if ($result['success']) { $call = $result['call']; } else { $call = array(); }
	$call_node = sirius_twilio_call_fetch_local($call_sid);

  $recording_node = sirius_twilio_recording_fetch_local($recording_sid);

	$type = 'pull';
	$msg = 'Pulled recording from Twilio';
	$ts = $recording['startTime']->getTimestamp();
	$datestr = gmdate('Y-m-d h:i:s a e', $ts);
	$summary = 'Pulled recording: ' . format_interval($recording['duration']) . ' recorded at ' . $datestr;
	$handlers = array();
	if ($call_node) {
		$handlers[] = $call_node->nid;
		foreach (sirius_fieldvals($call_node, 'field_sirius_log_handler', 'target_id') as $handler_nid) {
			if ($recording_node && ($handler_nid == $recording_node->nid)) { continue; }
			$handlers[] = $handler_nid;
		}
	}
	$json = array(
		'recording' => $recording,
		'call' => $call,
	);
	if ($params['comments']) { $json['comments'] = $params['comments']; }

	// Pull the file to a temporary location
	$filename = "recording_$recording_sid.mp3";
	$tmp_uri = "temporary://$filename";
	$tmp_path = drupal_realpath($tmp_uri);
	$url = sirius_twilio_recording_stream_url($recording_sid);
	$fp = fopen($tmp_path, 'w+');
	if (!$fp) { return array('success' => FALSE, 'msg' => "Unable to open temporary file: $tmp_path"); }
  $ch = curl_init();
  curl_setopt($ch, CURLOPT_URL, $url);
  curl_setopt($ch, CURLOPT_BINARYTRANSFER, true);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, false);
  curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
  curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 30);
  curl_setopt($ch, CURLOPT_FILE, $fp);
  $result = curl_exec($ch);
  if (!$result) {
  	unlink($tmp_path);
  	return array('success' => FALSE, 'msg' => "Unable to access the remote recording URL: " . curl_error($ch));
  }
  $curl_info = curl_getinfo($ch);
  if ($curl_info['http_code'] != 200) {
  	unlink($tmp_path);
  	return array('success' => FALSE, 'msg' => "Unable to access the remote recording URL; request returned error code " . $curl_info['http_code']);
  }
  curl_close($ch);
  fclose($fp);

  $uri = "private://sirius_attachments/$filename";
	$file = new stdClass;
	$file->filename = $filename;
	$file->uri = $tmp_uri;
	$file->filemime = file_get_mimetype($tmp_path);
	$file->status = FILE_STATUS_PERMANENT;
	$file = (array) file_move($file, $uri, FILE_EXISTS_REPLACE);
	$file['display'] = TRUE;

  if (!$recording_node) {
    $recording_node = sirius_log(
      'twilio:recording', 
      $type,
      $msg, 
      $summary,
      $recording_sid, 
      $handlers,
      array($file),
      $json
    );
  } else {
  	$json = array_merge(sirius_json_get($recording_node), $json);

    $recording_node->field_sirius_type['und'][0]['value'] = $type;
    $recording_node->field_sirius_message['und'][0]['value'] = substr($msg, 0, 254);
    $recording_node->field_sirius_summary['und'][0]['value'] = substr($summary, 0, 254);
    $recording_node->field_sirius_attachments['und'][0] = $file;

    unset($recording_node->field_sirius_log_handler);
    foreach ($handlers as $handler_nid) {
    	$recording_node->field_sirius_log_handler['und'][] = array('target_id' => $handler_nid);
    }

    sirius_json_set($recording_node, $json);
    sirius_jsonfield_log($recording_node, 'pull', "Pulled from Twilio");
    node_save($recording_node);
  }

  // Link the recording node to the call node
  if ($call_node) {
  	sirius_log_handler_set($call_node, $recording_node->nid);
  	node_save($call_node);
  }

  // Since these are log entries, the timestamp should be the time when the call happened, not the time when it was pulled. Or something.
  if ($ts) {
  	$sql = "update node set created = :ts where nid = :nid";
  	$stmt = sirius_sql_query($sql, array(':ts' => $ts, ':nid' => $recording_node->nid));
  }


	return array('success' => TRUE, 'msg' => 'Saved', 'recording_node' => $recording_node);
}

function sirius_twilio_recording_delete($rec_sid) {
	try {
		$client = sirius_twilio_client_get();
		$resource = $client->recordings($rec_sid);
		if (!$resource) { return array('success' => TRUE, 'msg' => 'Record not found.'); }
		$resource->delete();
		return array('success' => TRUE, 'msg' => 'Recording deleted.');
	} catch (Exception $e) {
		return array('success' => FALSE, 'msg' => 'Error: ' . $e->getCode() . ' - ' . $e->getMessage());
	}
}




function sirius_twilio_recording_mode_options() {
	return array(
		'do-not-record' => 'do-not-record',
		'record-from-answer' => 'record-from-answer', 
		'record-from-ringing' => 'record-from-ringing', 
		'record-from-answer-dual' => 'record-from-answer-dual', 
		'record-from-ringing-dual' => 'record-from-ringing-dual'
	);
}

function sirius_twilio_recording_stream_url($recording_sid) {
	// @todo: I don't understand access control at Twilio. They seem to post their recordings publicly, at a hard-to-guess but insecure URL.
	//        Oh well, if they're willing to put them there, I'm willing to proxy them, especially since my page is more secure than theirs.

	$url = 'https://api.twilio.com/2010-04-01/Accounts/';
	$url .= sirius_domain_variable_get('sirius_twilio_account_sid');
	$url .= '/Recordings/'; 
	$url .= $recording_sid;
	$url .= '.mp3';

	return $url;
}

function sirius_twilio_call_fetch_local($call_sid) {
	return sirius_twilio_get_context_node($call_sid);
}

function sirius_twilio_call_fetch($call_sid) {
	try {
		$client = sirius_twilio_client_get();
		$call = $client->calls($call_sid)->fetch();
		$result = array('success' => TRUE, 'call' => $call->toArray());
	} catch (Exception $e) {
		$result = array('success' => FALSE, 'msg' => "Failed to fetch call $call_sid: " . $e->getCode() . ' - ' . $e->getMessage());
	}

	return $result;
}

function sirius_recording_render_player_local($recording_node) {
	$fid = $recording_node->field_sirius_attachments['und'][0]['fid'];
	if (!$fid) { return NULL; }
	$file = file_load($fid);
	if (!$file) { return NULL; }
	$url = file_create_url($file->uri);
	return theme('audiofield_html5_audio', array('audio_file' => $url, 'download_access' => TRUE));
}