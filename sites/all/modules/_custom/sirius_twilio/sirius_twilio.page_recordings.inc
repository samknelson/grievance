<?php

function sirius_twilio_page_recordings() {
	return drupal_get_form('sirius_twilio_page_recordings_form');
}

function sirius_twilio_page_recordings_form($form, &$form_state) {
	/*
	drupal_set_message("Exists");
	dpm(sirius_twilio_recording_fetch_from_call('CA245355f9a79656f242638436087fd3d1'));
	drupal_set_message("Not");
	dpm(sirius_twilio_recording_fetch_from_call('CA245355f9a79656f242638436087fd3d3'));
	dpm(sirius_twilio_recording_fetch_from_call('CA245355f9a79656f242638436087fd3d333'));
	return array();
	*/

	$result = sirius_twilio_recording_search($form_state['values']['filters']);
	if (!$result['success']) {
		drupal_set_message($result['msg'], 'error');
	}

	$records = $result['records'];
	if (!$records) { $records = array(); }

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

	foreach ($records as $recording_sid => $recording) {

		$result = sirius_twilio_call_fetch($recording['callSid']);
		if ($result['success']) { $call = $result['call']; } else { $call = array(); }
		$call_sid = $call['sid'];

		$recording_nid = sirius_twilio_recording_fetch_local_nid($recording_sid);
		$call_node = sirius_twilio_call_fetch_local($call_sid);
		if ($call_node) { $call_nid = $call_node->nid; } else { $call_nid = NULL; }

		$tools = '';
		if ($recording_nid) {
			$local = 'Yes';
			$tools .= l('Recording', "node/$recording_nid/sirius_twilio_recording", array('attributes' => array('class' => array('tool-button'))));
		} else {
			$local = '';
		}

		if ($call_nid) {
			$tools .= l('Call', "node/$call_nid/sirius_twilio_call", array('attributes' => array('class' => array('tool-button'))));
		} 

		// $recording_nid = sirius_twilio_recording_fetch_local_nid($recording_sid);

		$from = sirius_phone_format($call['from']);
		$to = sirius_phone_format($call['to']);

		if ($filter_from && ($filter_from != $from)) { continue; }
		if ($filter_to && ($filter_to != $to)) { continue; }

		$ts = $recording['startTime']->getTimestamp();
		$datestr = date('Y-m-d h:i:s a', $ts);
		$datestr .= '<br />';
		$datestr .= '(' . format_interval(time() - $ts) . ' ago' . ')';

		$row = array();
		$row[] = $from;
		$row[] = $to;
		$row[] = $datestr;
		$row[] = format_interval($recording['duration']);
		$row[] = theme('audiofield_html5_audio', array('audio_file' => "/sirius/twilio/recording/stream/$recording_sid", 'download_access' => TRUE));
		$row[] = $local;
		$row[] = $tools;

		$rows[$recording_sid] = $row;
	}

	if (!$rows) { 
		$form['recording_sid'] = array('#markup' => t('No recordings found.'));
	} else {
		$header = array(
			t('From'),
			t('To'),
			t('Date'),
			t('Duration'),
			t('Play'),
			t('Local?'),
			t('Tools')
		);

    $form['recording_sids'] = array(
	    '#type' => 'tableselect',
	    '#title' => 'Recordings',
	    '#header' => $header,
	    '#options' => $rows,
    );

    $form['action'] = array(
    	'#type' => 'select',
    	'#options' => array(
    		NULL => t('-- Select --'),
    		'delete' => t('Delete selected recordings from Twilio'),
    		'pull' => t('Pull selected recordings from Twilio to the local database'),
    	),
    );

		$form['comments'] = array(
			'#type' => 'textarea',
			'#title' => 'Comments',
			'#states' => array('visible' => array(":input[name=\"action\"]" => array('value' => 'pull'))),
		);

    $form['submit'] = array(
    	'#type' => 'submit',
    	'#value' => 'Submit',
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

function sirius_twilio_page_recordings_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
	$action = $form_state['values']['action'];
	unset($form_state['values']['action']);
	unset($form_state['input']['action']);

	if ($action == 'delete') {
		$recording_sids = array_filter(array_values($form_state['values']['recording_sids']));
		$count = $count_success = $count_failure = 0;
		foreach ($recording_sids as $recording_sid) {
			++$count;
			$result = sirius_twilio_recording_delete($recording_sid);
			if ($result['success']) {
				++$count_success;
			} else {
				++$count_failure;
				drupal_set_message($result['msg'], 'warning');
			}
		}
		drupal_set_message("Attempted to delete $count records. Deleted $count_success with $count_failure failures.");
	}


	if ($action == 'pull') {
		$recording_sids = array_filter(array_values($form_state['values']['recording_sids']));
		$count = $count_success = $count_failure = 0;
		foreach ($recording_sids as $recording_sid) {
			++$count;
			$result = sirius_twilio_recording_pull($recording_sid, array('comments' => $form_state['values']['comments']));
			if ($result['success']) {
				++$count_success;
			} else {
				++$count_failure;
				drupal_set_message($result['msg'], 'warning');
			}
		}
		drupal_set_message("Attempted to pull $count records. Pulled $count_success with $count_failure failures.");
	}
}
