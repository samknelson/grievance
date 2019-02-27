<?php

function sirius_node_tab_worker_dnc_access($worker_node) {
	return node_access('view', $worker_node);
}

function sirius_node_tab_worker_dnc($worker_node) {
	return drupal_get_form('sirius_node_tab_worker_dnc_form', $worker_node);
}

function sirius_node_tab_worker_dnc_form($form, &$form_state, $worker_node) {
	$form_state['worker_node'] = $worker_node;

	drupal_set_title($worker_node->title . ': Employer-Initiated Do Not Call');

	// Show dispatch staff all the DNCs, and let them pick the employer
	$html = '';
	if ($worker_node->field_sirius_dispatch_dncs['und']) {
		foreach ($worker_node->field_sirius_dispatch_dncs['und'] as $tmp) {
			$employer_nid = $tmp['target_id'];
			$employer_node = node_load($employer_nid);
			$html .= '<li>' . $employer_node->title . '</li>';
		}
	}
	if ($html) {
		$html = $worker_node->title . ' is currently on the Do Not Call list for the following employers:<ul>' . $html . '</ul>';
	} else {
		$html = $worker_node->title . ' is not currently on the Do Not Call list for any employers.';
	}
	$form['intro'] = array(
		'#type' => 'markup',
		'#markup' => '<div class="sirius_form_intro">' . $html . '</div>',
	);


	if (user_access('sirius dispatch staff')) {
		$employer_options = array(NULL => t('-- Select --'));
		$employer_options += sirius_employer_list();
		$form['employer_nid'] = array(
			'#type' => 'select',
			'#title' => 'Employer',
			'#options' => $employer_options,
			'#required' => TRUE,
		);

		$form['action'] = array(
			'#type' => 'select',
			'#title' => 'Action',
			'#required' => TRUE,
			'#options' => array(
				NULL => t('-- Select --'),
				TRUE => t('Place this worker on the Do Not Call list'),
				FALSE => t('Remove this worker from the Do Not Call list'),
			),
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


	} else {
		// Show employers just their own DNC 
		/*
		$employer_node = sirius_employer_get();
		if (!$employer_node) {
			drupal_set_message(t('No employer found. This probably means that the user has been misconfigured, and has not been assigned to a shop. Please contact the system administrator.'), 'error');
			return array();
		}

		if (sirius_dispatch_dnc_check($worker_node, $employer_node)) {
			$html = $worker_node->title . ' is currently on the Do Not Call list for ' . $employer_node->title;
		} else {
			$html = $worker_node->title . ' is not currently on the Do Not Call list for ' . $employer_node->title;
		}
		$form['intro'] = array(
			'#type' => 'markup',
			'#markup' => '<div class="sirius_form_intro">' . $html . '</div>',
		);

		$form['employer_nid'] = array(
			'#type' => 'value',
			'#value' => $employer_node->nid,
		);
		*/
	}

	$form += sirius_log_render_form($worker_node->nid, 'dispatch:dnc');

	return $form;
}

function sirius_node_tab_worker_dnc_form_validate($form, &$form_state) {
	$info = field_info_instance('node', 'field_sirius_attachments', 'sirius_log');
	$extensions = $info['settings']['file_extensions'];
	for ($delta = 0; $delta < 3; ++$delta) {
	  $file = file_save_upload("attach_$delta", array(
	    'file_validate_extensions' => array($extensions),
	  ));
	  if ($file) {
	    $name = rand();
	    if ($file = file_move($file, 'private://tmp/' . rand())) {
	      $form_state['storage']["attach_$delta"] = $file;
	    }
	    else {
	      form_set_error("attach_$delta", t("Failed to write the uploaded file to the site's file folder."));
	    }
	  }
	}
}

function sirius_node_tab_worker_dnc_form_submit($form, &$form_state) {
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
	$employer_node = node_load($form_state['values']['employer_nid']);

	sirius_dispatch_dnc_set(
		$worker_node, 
		$employer_node, 
		$form_state['values']['action'], 
		$form_state['values']['reason'],
		$files
	);

	if ($form_state['values']['action']) {
		drupal_set_message(t("Success! Worker @worker has been placed on the Do Not Call list for employer @employer.", array('@worker' => $worker_node->title, '@employer' => $employer_node->title)));
	} else {
		drupal_set_message(t("Success! Worker @worker has been removed from the Do Not Call list for employer @employer.", array('@worker' => $worker_node->title, '@employer' => $employer_node->title)));
	}
}