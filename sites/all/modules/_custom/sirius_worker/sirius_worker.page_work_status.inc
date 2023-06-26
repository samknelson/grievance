<?php

function sirius_node_tab_worker_work_status_access($worker_node) {
	return node_access('view', $worker_node);
}

function sirius_node_tab_worker_work_status_form($form, &$form_state, $worker_node) {
	$form_state['worker_node'] = $worker_node;

	$statuses = sirius_taxonomy_load('sirius_work_status', 'tid', 'name', TRUE);

	$form['work_status'] = array(
		'#type' => 'select',
		'#title' => t('New Work Status'),
		'#options' => $statuses,
		'#required' => TRUE,
	);

	$form['date'] = array(
		'#title' => t('Effective Date'),
		'#type' => 'date_popup',
		'#value' => t('Submit'),
		'#default_value' => date('Y-m-d H:i:s'),
		'#date_format' => 'Y-m-d H:i:s',
    '#attributes' => array(
			'autocomplete' => 'off',
    ),
    '#required' => TRUE,
	);

	$form['reason'] = array(
		'#type' => 'textarea',
		'#title' => t('Reason'),
		'#required' => TRUE,
	);

	for ($delta=0; $delta<3; ++$delta) {
		$form["attach_$delta"] = array(
			'#type' => 'file',
			'#title' => t('Attachment #') . ($delta + 1),
		);
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Submit',
	);

	return $form;
}

function sirius_node_tab_worker_work_status_form_validate($form, &$form_state) {
	$info = field_info_instance('node', 'field_sirius_attachments', 'sirius_log');
	$extensions = $info['settings']['file_extensions'];
	for ($delta = 0; $delta < 3; ++$delta) {
	  $file = file_save_upload("attach_$delta", array(
	    'file_validate_extensions' => array($extensions),
	  ));
	  if ($file) {
	    $name = rand();
	    if ($file = file_move($file, 'private://tmp_' . rand())) {
	      $form_state['storage']["attach_$delta"] = $file;
	    }
	    else {
	      form_set_error("attach_$delta", t("Failed to write the uploaded file to the site's file folder."));
	    }
	  }
	}
}

function sirius_node_tab_worker_work_status_form_submit($form, &$form_state) {
	$files = array();
	for ($delta = 0; $delta < 3; ++$delta) {
	  $file = $form_state['storage']["attach_$delta"];
	  if ($file) {
  		$file = (array) $file;
  		$file['display'] = 1;
			unset($form_state['storage']["attach_$delta"]);
  		unset($file['destination']);
  		unset($file['uri']);
  		$files[] = $file;
  	}
  }

	$worker_node = $form_state['worker_node'];

	sirius_worker_work_status_set(
		$worker_node, 
		$form_state['values']['work_status'], 
		$form_state['values']['reason'],
		$files,
		array(),
		strtotime($form_state['values']['date'])
	);

	drupal_set_message(t("Success! Worker @worker has been assigned the new work status.", array('@worker' => $worker_node->title)));
}