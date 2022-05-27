<?php

/**
 * Fetch "My" application
 */

function sirius_event_intake_mine($event_nid) {
	return node_load($_SESSION["sirius_event_${event_nid}_intake"]);
}

/**
 * Set "My" application
 */

function sirius_event_intake_mine_set($event_nid, $application_nid) {
	$_SESSION["sirius_event_${event_nid}_intake"] = $application_nid;
}

/**
 * Create "My" new blank application
 */

function sirius_event_intake_mine_create($event_nid, $application_json = NULL) {
	$event_node = node_load($event_nid);
	$application_node = sirius_log('event:intake_application', 'draft', 'New', 'New', NULL, array($event_nid), array(), $application_json);
	$application_node->field_sirius_domain = $event_node->field_sirius_domain;
	sirius_event_intake_set_title($application_node);
	sirius_event_intake_mine_set($event_nid, $application_node->nid);
	return $application_node;
}

function sirius_event_intake_enabled_tid($event_type_tid) { return sirius_event_intake_enabled_type(taxonomy_term_load($event_type_tid)); }

function sirius_event_intake_enabled_type($event_type_term) {
  $json = sirius_jsonfield_util_term_json_get($event_type_term);
  return $json['intake']['enabled'] == 'Yes';
}


function sirius_event_intake_enabled_nid($event_nid) { return sirius_event_intake_enabled(node_load($event_nid)); }

function sirius_event_intake_enabled($event_node) {
	$event_json = sirius_jsonfield_util_node_json_get($event_node);
	return $event_json['intake']['enabled'] == 'Yes';
}

function sirius_event_intake_access_code_anonicalize($code) {
	$code = preg_replace('/[^a-zA-Z0-9_ ]/', '', $code);
	$code = strtoupper($code);
	$code = trim($code);
	return $code;
}

function sirius_event_intake_fetch_by_access_code($access_code) {
	$sql = "select nid ";
	$sql .= "from node ";
	$sql .= "left join field_data_field_sirius_json on field_data_field_sirius_json.entity_type = 'node' and field_data_field_sirius_json.entity_id = node.nid ";
	$sql .= "where node.type = 'sirius_event' ";
	$sql .= "and json_unquote(json_extract(field_sirius_json_value, '\$.intake.anonymous.access_code')) = :access_code ";
	$sql .= "order by nid desc ";
	$stmt = sirius_sql_query($sql, array(':access_code' => $access_code));
	$matches = array();
	while ($hr = $stmt->fetchAssoc()) {
		if (!$fetchall) { return node_load($hr['nid']); }
		$matches[] = $hr['nid'];
	}
	return $matches;
}

function sirius_event_intake_set_title(&$application_node) {
	$title = "Application - ";
	$application_json = sirius_jsonfield_util_term_json_get($application_node);
	if ($application_json['wizard']['intake']['identity']['name_first']) {
		$title .= $application_json['wizard']['intake']['identity']['name_first'] . ' ' . $application_json['wizard']['intake']['identity']['name_last'];
	} else {
		$title .= '(no name)'; 
	}
	$title .= ' - ';

	$event_node = sirius_log_handler($application_node, 'sirius_event');
	if (!$event_node) {
		$title .= '(no event)'; 
	} else {
		$event_json = sirius_jsonfield_util_node_json_get($event_node);
		$title .= $event_json['intake']['anonymous']['title'];
		if (!$title) { $title .= $event_node->title; }
	}

	if ($title != $application_node->title) {
		$application_node->title = substr($title, 0, 255);
		node_save($application_node);
	}
}

function sirius_event_intake_application_status_options() {
	return array(
		'draft' => 'Draft',
		'submitted' => 'Submitted',
		'rejected' => 'Rejected',
		'accepted' => 'Accepted',
		'complete' => 'Complete',
	);
}

function sirius_event_intake_active($event_node) {
  $result = NULL;

  $json = sirius_jsonfield_util_node_json_get($event_node);

  if ($json['intake_status']['active'] == 'Yes') { 
    $result = array('success' => TRUE);
  } else if ($json['intake_status']['active'] == 'No') { 
    $result = array('success' => FALSE, 'msg' => "The intake form has been closed by an administrator.");
  }
  else if ($json['intake_status']['active'] == 'Auto') { 
    $open_grace = $json['intake_status']['active_grace_open'];
    if ($open_grace > 0) { $open_grace *= 60; } else { $open_grace = 0; }

    $close_grace = $json['intake_status']['active_grace_close'];
    if ($close_grace) { $close_grace *= 60; } else { $close_grace = 0; }

    if ($event_node->field_sirius_daterepeat['und']) {
      $ts_now = time();
      foreach ($event_node->field_sirius_daterepeat['und'] as $tmp) {
        $start_ts = strtotime($tmp['value'] . ' ' . $tmp['timezone_db']);
        $end_ts = strtotime($tmp['value2'] . ' ' . $tmp['timezone_db']);
        if ($start_ts <= 1 || $end_ts <= 1) { continue; }
        $start_ts -= $open_grace;
        $end_ts += $close_grace;
        if ($ts_now >= $start_ts && $ts_now <= $end_ts) {
          $result = array('success' => TRUE);
        }
      }
    }
    if (!$result['success']) {
      $result = array('success' => FALSE, 'msg' => "The intake form is currently closed. " . sirius_event_intake_opens_in($event_node));
    }
  } else { 
    $result = array('success' => FALSE);
  }

  return $result;
}

function sirius_event_intake_opens_in($event_node) {
  if (!$event_node->field_sirius_daterepeat['und']) { return ''; }
  $ts_now = time();
  $opens_in = 999999999;
  foreach ($event_node->field_sirius_daterepeat['und'] as $tmp) {
    $start_ts = strtotime($tmp['value'] . ' ' . $tmp['timezone_db']);
    $end_ts = strtotime($tmp['value2'] . ' ' . $tmp['timezone_db']);
    if ($start_ts <= 1 || $end_ts <= 1) { continue; }
    if ($start_ts >= $ts_now) {
      $range = $start_ts - $ts_now;
      if ($range && $range < $opens_in) { $opens_in = $range; }
    }
  }
  if ($opens_in > 1 && $opens_in < 999999999) {
    return t('Opening in: @interval', array('@interval' => format_interval($opens_in)));
  }
}

function sirius_event_intake_closes_in($event_node) {
  if (!$event_node->field_sirius_daterepeat['und']) { return ''; }
  $ts_now = time();
  $closes_in = 0;
  foreach ($event_node->field_sirius_daterepeat['und'] as $tmp) {
    $start_ts = strtotime($tmp['value'] . ' ' . $tmp['timezone_db']);
    $end_ts = strtotime($tmp['value2'] . ' ' . $tmp['timezone_db']);
    if ($start_ts <= 1 || $end_ts <= 1) { continue; }
    if ($start_ts <= $ts_now && $end_ts >= $ts_now) {
      $range = $end_ts - $ts_now;
      if ($range && $range > $closes_in) { $closes_in = $range; }
    }
  }
  if ($closes_in > 1) {
    return t('Closes in: @interval', array('@interval' => format_interval($closes_in)));
  }
}

function sirius_event_intake_application_render($application_node) {
	return drupal_render(drupal_get_form('sirius_event_intake_application_render_form', $application_node));
}

function sirius_event_intake_application_render_form($form, &$form_state, $application_node) {
	$application_json = sirius_json_get($application_node);
	$data = $application_json['wizard']['intake'];

	$event_node = sirius_log_handler($application_node, 'sirius_event');
	$event_json = sirius_json_get($event_node);

	$form['#tree'] = TRUE;

	$submitted_ts = $data['submitted_ts'];
	if ($submitted_ts) {
		$form['status'] = array('#markup' => '<p>Application submitted on <strong>' . date('l, F d Y - g:ia', $submitted_ts) . '</strong></p>');
	}

	if ($data['attachments']) {
		$attachments_html = '<h2>' . t('Attachments') . '</h2>';
		foreach ($data['attachments'] as $material_tid => $attachment) {
			$material_term = taxonomy_term_load($material_tid);
			$fid = $attachment['fid'];
			$file = (array) file_load($fid);
			$url = file_create_url($file['uri']);
			$details = "<a href=\"$url\" target=\"_blank\">";
			$details .= $attachment['origname'];
			$details .= ' (';
			$details .= format_size($file['filesize']);
			$details .= ')';
			$details .= '</a>';
			$attachments_html .= sirius_fakerender_field($material_term->name, $details);
		}
		$attachments_html .= '<br /><br />';
		$form['attachments'] = array('#markup' => $attachments_html);
	}

  if ($event_json['intake']['survey']['list']) {
  	foreach ($event_json['intake']['survey']['list'] as $delta => $element) {
  		$title = $element['title'];
  		if (!$title) { 
  			$title = 'Form';
  			if ($delta) { $title .= ' ' . ($delta+1); }
  		}

  		$survey_nid = $element['survey_nid'];
			$survey_node = node_load($survey_nid);
			$survey_json = sirius_json_get($survey_node);
			$survey_form = sirius_jsonfield_util_form_get($survey_json, 'default');

			$application_json = sirius_json_get($application_node);
			$application_results = sirius_jsonfield_util_results_get($application_json, "survey_" . $survey_nid);

			$prep_settings = $event_json;
			$prep_settings['skip_required'] = TRUE;
			$prep_settings['hide_instructions'] = TRUE;
			sirius_event_phonebank_prep_survey($survey_form, $application_results, $prep_settings);
			drupal_add_css(drupal_get_path('module', 'sirius_event') . '/css/survey.css', 'file');

			$form['surveys'][$survey_nid]['title'] = array('#markup' => '<h2>' . $title . '</h2>');
			$form['surveys'][$survey_nid]['open'] = array('#markup' => '<div id="survey_wrap" class="survey_wrap_inpage sirius-form">');
			$form['surveys'][$survey_nid]['form'] = $survey_form;
			$form['surveys'][$survey_nid]['close'] = array('#markup' => '</div>');
  	}
  }

	$form['#disabled'] = TRUE;

	return $form;
}

function sirius_event_intake_application_submit(&$application_node) {
	$application_json = sirius_json_get($application_node);

	// Don't re-submit
	if ($application_node->field_sirius_type['und'][0]['value'] == 'submitted') {
		return array('success' => TRUE, 'msg' => "Status is unchanged.");
	}


	$event_node = sirius_log_handler($application_node, 'sirius_event');
	$worker_node = sirius_log_handler($application_node, 'sirius_worker');
	$event_json = sirius_json_get($event_node);

	// Submit and log
	$application_node->field_sirius_type['und'][0]['value'] = 'submitted';
	sirius_jsonfield_log($application_node, "status_set", "Changed status to submitted");
	sirius_event_intake_set_title($application_node);
	$msg = "Status set to \"submitted\".";

	// Notify
	$email = $application_json['wizard']['intake']['email'];
	$phone = $application_json['wizard']['intake']['phone'];
	$notification_params = $event_json['intake']['notifications']['submit'];
	$notification_params['medium'] = 'both';	
	$result = sirius_event_notify(
		$application_node,
		$email,
		$phone,
		$notification_params, 
		array('application_node' => $application_node, 'event_node' => $event_node, 'worker_node' => $worker_node)
	);
	if ($result['msg']) {
		sirius_jsonfield_log($application_node, "notification", $result['msg']);
	}

	// Assign
	$assignment_options = sirius_event_intake_application_assignment_options($event_node);
	if ($assignment_options) { sirius_event_intake_application_assignment_set($application_node, $assignment_options[0]); }

	node_save($application_node);
	return array('success' => TRUE, 'msg' => $msg);
}
function sirius_event_intake_application_complete(&$application_node) {
	$application_json = sirius_json_get($application_node);

	// Don't re-complete
	if ($application_node->field_sirius_type['und'][0]['value'] == 'complete') {
		return array('success' => TRUE, 'msg' => "Status is unchanged.");
	}

	// Submit and log
	$application_node->field_sirius_type['und'][0]['value'] = 'complete';
	sirius_jsonfield_log($application_node, "status_set", "Changed status to complete");
	sirius_event_intake_set_title($application_node);
	$msg = "Status set to \"complete\".";

	// Un-Assign
	sirius_event_intake_application_assignment_set($application_node, NULL);
	node_save($application_node);
	return array('success' => TRUE, 'msg' => $msg);
}
