<?php

function sirius_training_node_tab_worker_graduate($worker_node, $ms_tid, $level_code) {
	return drupal_get_form('sirius_training_node_tab_worker_graduate_form', $worker_node, $ms_tid, $level_code);
}

function sirius_training_node_tab_worker_graduate_form($form, &$form_state, $worker_node, $ms_tid, $level_code) {
	$form_state['worker_nid'] = $worker_node->nid;
	$form_state['ms_tid'] = $ms_tid;
	$form_state['level_code'] = $level_code;

	$ms_term = taxonomy_term_load($ms_tid);
	$levels = sirius_training_levels_fetch($ms_term);
	$level = $levels[$level_code];
	$form['#tree'] = TRUE;

	$form['intro'] = array(
		'#markup' => '<div class="sirius_form_intro">' . '<strong>' . t('Graduate this worker to @level', array('@level' => $level['title'])) . '</strong>' . '</div>'
	);

	//$notification_emails = sirius_training_level_notification_emails($worker_node, $ms_term, $level);

	/*
	$form['notification_emails'] = array(
		'#title' => t('Notification Emails'),
		'#type' => 'textarea',
		'#default_value' => $notification_emails,
		'#description' => t('Enter email addresses which should be notified when a worker attains this level, one per line.')
	);

	$form['notification_subject'] = array(
		'#title' => t('Subject'),
		'#type' => 'textfield',
		'#default_value' => '',
	);

	$form['notification_body'] = array(
		'#title' => t('Body'),
		'#type' => 'textarea',
		'#default_value' => '',
	);
	*/

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_training_node_tab_worker_graduate_form_submit($form, &$form_state) {
	$worker_nid = $form_state['worker_nid'];
	$worker_node = node_load($worker_nid);
	$ms_tid = $form_state['ms_tid'];
	$ms_term = taxonomy_term_load($ms_tid);
	$level_code = $form_state['level_code'];
	$levels = sirius_training_levels_fetch($ms_term);
	$level = $levels[$level_code];

	$result = sirius_training_level_graduate($worker_node, $ms_term, $level, $form_state['values']);
	if ($result['success']) { 
		drupal_set_message("Graduated " . $worker_node->title . " to $level[title].");
		drupal_goto(current_path());
	} else {
		drupal_set_message("Graduation failed: " . $result['msg']);
		$form_state['rebuild'] = TRUE;
	}
}