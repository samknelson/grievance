<?php

function sirius_twilio_page_recordings_local() {
	return drupal_get_form('sirius_twilio_page_recordings_local_form');
}

function sirius_twilio_page_recordings_local_form($form, &$form_state) {
	/*
	drupal_set_message("Exists");
	dpm(sirius_twilio_recording_fetch_from_call('CA245355f9a79656f242638436087fd3d1'));
	drupal_set_message("Not");
	dpm(sirius_twilio_recording_fetch_from_call('CA245355f9a79656f242638436087fd3d3'));
	dpm(sirius_twilio_recording_fetch_from_call('CA245355f9a79656f242638436087fd3d333'));
	return array();
	*/

	$start_ts = strtotime($form_state['values']['filters']['dateCreatedAfter']);
	$end_ts = strtotime($form_state['values']['filters']['dateCreatedBefore']);

  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_log');
  $query->fieldCondition('field_sirius_category', 'value', 'twilio:recording');
  if ($start_ts > 0) { $query->propertyCondition('created', $start_ts, '>='); }
  if ($end_ts > 0) { $query->propertyCondition('created', $end_ts, '<='); }
  $query->propertyOrderBy('created', 'desc');
  $query->range(0, 100);
  $stmt = $query->execute();
  $records = array();
  if ($stmt['node']) { 
  	$records = node_load_multiple(array_keys($stmt['node']));
  }

	$form['#tree'] = TRUE;

	$form['filters'] = array(
		'#type' => 'fieldset',
		'#title' => 'Filters',
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
	);

	$form['filters']['dateCreatedAfter'] = array(
		'#title' => t('Start Date:'),
    '#type' => 'date_popup',
    '#timepicker' => 'timepicker',
    '#timepicker_options' => array(
			'rows' => 6,
			'minutes' => array(
				'starts' => 0,
				'ends' => 56,
				'interval' => 5,
			),
    ),
    '#attributes' => array(
			'autocomplete' => 'off',
    ),
  );

	$form['filters']['dateCreatedBefore'] = array(
		'#title' => t('End Date:'),
    '#type' => 'date_popup',
    '#timepicker' => 'timepicker',
    '#timepicker_options' => array(
			'rows' => 6,
			'minutes' => array(
				'starts' => 0,
				'ends' => 56,
				'interval' => 5,
			),
    ),
    '#attributes' => array(
			'autocomplete' => 'off',
    ),
  );

	$form['filters']['from'] = array(
		'#type' => 'textfield',
		'#title' => 'From Phone',
	);

	$form['filters']['to'] = array(
		'#type' => 'textfield',
		'#title' => 'To Phone',
	);

	$form['filters']['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Search'),
	);

	$filter_from = sirius_phone_format($form_state['values']['filters']['from']);
	$filter_to = sirius_phone_format($form_state['values']['filters']['to']);

	foreach ($records as $recording_nid => $recording_node) {
		$recording_json = sirius_json_get($recording_node);
		$call = $recording_json['call'];
		$call_sid = $call['sid'];
		$recording = $recording_json['recording'];
		$recording_sid = $recording['sid'];

		$call_node = sirius_twilio_call_fetch_local($call_sid);
		if ($call_node) { $call_nid = $call_node->nid; } else { $call_nid = NULL; }

		$tools = '';
		$tools .= l('Recording', "node/$recording_nid/sirius_twilio_recording", array('attributes' => array('class' => array('tool-button'))));
		if ($call_nid) {
			$tools .= l('Call', "node/$call_nid/sirius_twilio_call", array('attributes' => array('class' => array('tool-button'))));
		} 

		// $recording_nid = sirius_twilio_recording_fetch_local_nid($recording_sid);

		$from = sirius_phone_format($call['from']);
		$to = sirius_phone_format($call['to']);

		if ($filter_from && ($filter_from != $from)) { continue; }
		if ($filter_to && ($filter_to != $to)) { continue; }

		$ts = strtotime($recording['startTime']['date'] . ' GMT');
		$datestr = date('Y-m-d h:i:s a', $ts);
		$datestr .= '<br />';
		$datestr .= '(' . format_interval(time() - $ts) . ' ago' . ')';

		$row = array();
		$row[] = $from;
		$row[] = $to;
		$row[] = $datestr;
		$row[] = sirius_text_to_html($recording_json['comments']);
		$row[] = format_interval($recording['duration']);
		$row[] = theme('audiofield_html5_audio', array('audio_file' => "/sirius/twilio/recording/stream/$recording_sid", 'download_access' => TRUE));
		$row[] = $tools;

		$rows[$recording_nid] = $row;
	}

	if (!$rows) { 
		$form['recording_sid'] = array('#markup' => t('No recordings found.'));
	} else {
		$header = array(
			t('From'),
			t('To'),
			t('Date'),
			t('Comments'),
			t('Duration'),
			t('Play'),
			t('Tools')
		);

    $form['recording_sids'] = array(
	    '#type' => 'tableselect',
	    '#title' => 'Recordings',
	    '#header' => $header,
	    '#options' => $rows,
    );
	}

	/* 
	// When the time comes to do paging, something like this will probably work.
	if ($form_state['values']['op'] == 'Next Page') {
		$result = sirius_twilio_recordings_search($form_state['values']['search'], 3, $form_state['values']['nextPageUrl']);
	} else if ($form_state['values']['op'] == 'Previous Page') {
		$result = sirius_twilio_recordings_search($form_state['values']['search'], 3, $form_state['values']['previousPageUrl']);
	} else {
		$result = sirius_twilio_recordings_search($form_state['values']['search'], 3);
	}
	$form['nextprevwrap_open'] = array('#markup' => '<div class="container-inline">');
  if ($result['previousPageUrl']) {
  	$form['previousPageUrl'] = array('#type' => 'value', '#value' => $result['previousPageUrl']);
  	$form['prev'] = array('#type' => 'submit', '#value' => 'Previous Page');
  }
  if ($result['nextPageUrl']) {
  	$form['nextPageUrl'] = array('#type' => 'value', '#value' => $result['nextPageUrl']);
  	$form['next'] = array('#type' => 'submit', '#value' => 'Next Page');
  }
	$form['nextprevwrap_close'] = array('#markup' => '</div>');
	*/

	return $form;
}


function sirius_twilio_page_recordings_local_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
}