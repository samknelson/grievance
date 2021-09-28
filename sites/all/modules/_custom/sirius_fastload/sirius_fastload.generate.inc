<?php

function sirius_fastload_generate_cron() {
  $last_ts = variable_get('sirius_fastload_generate_cron_last_ts', 0);
  $now_ts = time();
  if ($now_ts - $last_ts < 60*60) { return; }
  variable_set('sirius_fastload_generate_cron_last_ts', $now_ts);
  sirius_fastload_generate();
}

function sirius_fastload_generate() {
	sirius_minilog('FastLoad: Generator starting');
	if (variable_get('sirius_fastload_enabled', 'No') != 'Yes') {
		$msg = 'FastLoad: Not enabled for this site. Please check the configuration.';
		sirius_minilog($msg, 'warning');
		return array('success' => TRUE, 'msg' => $msg);
	}

	$buffer_size = variable_get('sirius_fastload_buffer_size', 0);
	$batch_size = variable_get('sirius_fastload_batch_size', 0);

	if ($buffer_size < 1) {
		$msg = "FastLoad: Buffer size is less than 1. Please check the configuration.";
		sirius_minilog($msg, 'warning');
		return array('success' => TRUE, 'msg' => $msg);
	}
	if ($batch_size < 1) {
		$msg = "FastLoad: Batch size is less than 1. Please check the configuration.";
		sirius_minilog($msg, 'warning');
		return array('success' => TRUE, 'msg' => $msg);
	}


	$content_types = sirius_fastload_content_types();
	foreach ($content_types as $content_type) {
		$count = sirius_fastload_buffer_count($content_type);
		$to_generate = $buffer_size - $count;
		if ($to_generate < 0) { $to_generate = 0; }
		if ($to_generate > $batch_size) { $to_generate = $batch_size; }

		sirius_minilog("FastLoad $content_type: Buffer size: $buffer_size; Batch size: $batch_size; Current buffer: $count; To generate: $to_generate");

		if (!$to_generate) {
			$msg = "FastLoad $content_type: No new entries need to be generated. Terminating.";
			sirius_minilog($msg);
			return array('success' => TRUE, 'msg' => $msg);
		}

		for ($i=1; $i<=$to_generate; ++$i) {
			$node = sirius_fastload_generate_node($content_type);
			sirius_minilog("FastLoad $content_type: Generated node $i/$to_generate (nid " . $node->nid . ")");
		}
	}
}

function sirius_fastload_generate_node($content_type) {
	global $user;
  $node = new stdClass();
  $node->type = $content_type;
  $node->no_recurse = TRUE;
  $node->language = $user->language;
  $node->uid = $user->uid;
  $node->status = 1;
  $node->promote = 0;
  $node->comment = 0;
  $node->title = 'FastLoad Reserved Node';
  $node->field_sirius_fastload_status['und'][0]['value'] = 'available';
	$node->field_sirius_domain['und'][0]['target_id'] = 1;
	$node->field_sirius_id['und'][0]['value'] = 'fastload_' . rand(1000000000, 9999999999);
  $json = array(
  	'fastload' => array(
  		'generated_ts' => time(),
  	),
  );
	sirius_jsonfield_util_node_json_set($node, $json);


	if ($content_type == 'sirius_log') {
		$node->field_sirius_log_handler['und'][0]['target_id'] = 1;
		$node->field_sirius_category['und'][0]['value'] = 'fastload:available';
		$node->field_sirius_type['und'][0]['value'] = 'pending';
		$ndoe->field_sirius_message['und'][0]['value'] = 'pending';
		$ndoe->field_sirius_summary['und'][0]['value'] = 'pending';
		$ndoe->field_sirius_notes['und'][0]['value'] = 'pending';
	} else if ($content_type == 'sirius_contact') {
		$node->field_sirius_phone['und'][0]['value'] = '2125551212';
		$node->field_sirius_email['und'][0]['value'] = 'fastload@nodomain.com';
		$node->field_sirius_source['und'][0]['value'] = 'fastload';
		$node->field_sirius_name['und'][0]['given'] = 'Generated';
		$node->field_sirius_name['und'][0]['family'] = 'Fastload';
	}

  node_object_prepare($node);
  $node = node_submit($node);
  node_save($node);
  return $node;
}
