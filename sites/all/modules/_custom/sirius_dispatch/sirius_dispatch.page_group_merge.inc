<?php

function sirius_dispatch_node_tab_group_merge($group_node) {
	return drupal_get_form('sirius_dispatch_node_tab_group_merge_form', $group_node);
}

function sirius_dispatch_node_tab_group_merge_form($form, &$form_state, $group_node) {
  $form_state['group_nid'] = $group_node->nid;

  $form['intro'] = array(
	'#markup' =>
		'<div class="sirius_form_intro">' .
		t('The form below will merge another job group into <strong>@name</strong>.', array('@name' => $group_node->title)) .
		'<ul>' .
		'<li>' . t('The old group\'s record will no longer exist.') . '</li>' .
		'<li>' . t('Any attachments, log entries, or other nodes which referenced the old group will reference @name.', array('@name' => $group_node->title)) . '</li>' .
		'<li>' . t('None of the fields in @name\'s record will be changed.', array('@name' => $group_node->title)) . '</li>' .
		'</ul>',
  );

	$form['from_group_nid'] = array(
		'#type' => 'textfield',
		'#size' => 6,
		'#title' => t('OLD Group Node ID (will be deleted)'),
		'#required' => TRUE,
		'#default_value' => $_REQUEST['from_group_nid'],
	);

	$form['confirm'] = array(
		'#type' => 'checkbox',
		'#title' => t('Yes, please merge the group in the box above into @name. This action cannot be undone.', array('@name' => $group_node->title)),
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

function sirius_dispatch_node_tab_group_merge_form_submit($form, &$form_state) {
	$group_nid = $form_state['group_nid'];
	$group_node = node_load($group_nid);

	$from_group_nid = $form_state['values']['from_group_nid'];
	$from_group_node = node_load($from_group_nid);

	$result = sirius_dispatch_group_merge($group_nid, $from_group_nid);

	if ($result['success']) { drupal_set_message(t('Groups merged: @msg', array('@msg' => $result['msg']))); }
	else { drupal_set_message(t('Merge failed: @msg', array('@msg' => $result['msg'])), 'error'); }

	$form_state['rebuild'] = TRUE;
}
