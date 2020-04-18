<?php

function sirius_node_tab_worker_merge($worker_node) {
	return drupal_get_form('sirius_node_tab_worker_merge_form', $worker_node);
}

function sirius_node_tab_worker_merge_form($form, &$form_state, $worker_node) {
	$form_state['worker_nid'] = $worker_node->nid;

	$form['intro'] = array(
		'#markup' => 
			'<div class="sirius_form_intro">' . 
			t('The form below will merge another worker into <strong>@name</strong>.', array('@name' => $worker_node->title)) . 
			'<ul>' .
			'<li>' . t('The old worker\'s record will no longer exist.') . '</li>' .
			'<li>' . t('A new log entry will be created with a copy of the old worker\'s record.') . '</li>' .
			'<li>' . t('Any attachments, log entries, or other nodes which referenced the old worker will reference @name.', array('@name' => $worker_node->title)) . '</li>' .
			'<li>' . t('None of the fields in @name\'s record will be changed.', array('@name' => $worker_node->title)) . '</li>' .
			'</ul>',
	);

	$form['from_worker_nid'] = array(
		'#type' => 'textfield',
		'#size' => 6,
		'#title' => t('OLD Worker Node ID (will be deleted)'),
		'#required' => TRUE,
		'#default_value' => $_REQUEST['from_worker_nid'],
	);

	$form['confirm'] = array(
		'#type' => 'checkbox',
		'#title' => t('Yes, please merge the worker in the box above into @name. This action cannot be undone.', array('@name' => $worker_node->title)),
		'#required' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	if ($form_state['results']) {
		$form['results'] = array(
			'#markup' => '<h2>' . t('Results') . '</h2><hr>' . $form_state['results'],
		);
	}

	return $form;
}

function sirius_node_tab_worker_merge_form_validate($form, &$form_state) {
	$from_worker_nid = $form_state['values']['from_worker_nid'];
	$from_worker_node = node_load($from_worker_nid);

	if (!$from_worker_node) { form_set_error('from_worker_nid', t('@id is not a node id.', array('@id' => $from_worker_nid))); } 
	else if ($from_worker_node->type != 'sirius_worker') { form_set_error('from_worker_nid', t('@id is not a worker node.', array('@id' => $from_worker_nid))); }
}

function sirius_node_tab_worker_merge_form_submit($form, &$form_state) {
	$worker_nid = $form_state['worker_nid'];
	$worker_node = node_load($worker_nid);

	$from_worker_nid = $form_state['values']['from_worker_nid'];
	$from_worker_node = node_load($from_worker_nid);

	$result = sirius_worker_merge($worker_nid, $from_worker_nid);

	if ($result['success']) { drupal_set_message(t('Workers merged: @msg', array('@msg' => $result['msg']))); } 
	else { drupal_set_message(t('Merge failed: @msg', array('@msg' => $result['msg'])), 'error'); }

	$form_state['results'] = sirius_minilog_render_html();
}
