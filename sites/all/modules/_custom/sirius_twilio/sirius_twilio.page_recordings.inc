<?php

function sirius_twilio_page_recordings() {
	return drupal_get_form('sirius_twilio_page_recordings_form');
}

function sirius_twilio_page_recordings_form($form, &$form_state) {
	$result = sirius_twilio_recording_search($form_state['values']['search']);
	if (!$result['success']) {
		drupal_set_message($result['msg'], 'error');
	}

	$form['#tree'] = TRUE;

	$form['search']['wrap_open'] = array('#markup' => '<div class="container-inline">');

	$form['search']['dateCreatedAfter'] = array(
		'#title' => t('From:'),
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

	$form['search']['dateCreatedBefore'] = array(
		'#title' => t('To:'),
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

	$form['search']['search'] = array(
		'#type' => 'submit',
		'#value' => t('Search'),
	);

	$form['search']['wrap_close'] = array('#markup' => '</div>');

	if (!$result['records']) {
		$form['rec_sids'] = array('#markup' => t('No recordings found.'));
	} else { 
		foreach ($result['records'] as $record) {
			$rec_sid = $record['sid'];
			$ts = $record['startTime']->getTimestamp();
			$datestr = date('Y-m-d h:i:s a', $ts);
			$datestr .= '<br />';
			$datestr .= '(' . format_interval(time() - $ts) . ' ago' . ')';

			$row = array();
			$row[] = $rec_sid;
			$row[] = $datestr;
			$row[] = format_interval($record['duration']);
			$row[] = theme('audiofield_html5_audio', array('audio_file' => "/sirius/twilio/recording/stream/$rec_sid", 'download_access' => TRUE));
			$rows[$rec_sid] = $row;
		}
		$header = array(
			t('SID'),
			t('Date'),
			t('Duration'),
			t('Play'),
			t('Tools')
		);
    $form['rec_sids'] = array(
	    '#type' => 'tableselect',
	    '#title' => 'Recordings',
	    '#header' => $header,
	    '#options' => $rows,
    );

    $form['action'] = array(
    	'#type' => 'select',
    	'#options' => array(
    		NULL => t('-- Select --'),
    		'delete' => t('Delete selected messages from Twilio'),
    	),
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
		$rec_sids = array_filter(array_values($form_state['values']['rec_sids']));
		$count = $count_succss = $count_failure = 0;
		foreach ($rec_sids as $rec_sid) {
			++$count;
			$result = sirius_twilio_recording_delete($rec_sid);
			if ($result['success']) {
				++$count_success;
			} else {
				++$count_failure;
				drupal_set_message($result['msg'], 'warning');
			}
		}
		drupal_set_message("Attempted to delete $count records. Deleted $count_success with $count_failure failures.");
	}

}
