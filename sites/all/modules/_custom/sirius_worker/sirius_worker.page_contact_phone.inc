<?php

function sirius_node_tab_worker_contact_phone_access($worker_node) {
	return Sirius_Worker_Access::getInstance()->access('update addr', array('worker_node' => $worker_node));
}

function sirius_node_tab_worker_contact_phone_form($form, &$form_state, $worker_node) {
	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/ux.css');

	$is_staff = Sirius_Worker_Access::getInstance()->access('staff');

	$form_state['worker_nid'] = $worker_node->nid;
	$form['#tree'] = TRUE;

	$form['intro'] = array();

	$form['phone'] = array(
		'#type' => 'textfield',
		'#title' => t('Phone'),
		'#default_value' => $worker_node->field_sirius_phone['und'][0]['value'],
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Submit',
	);

	return $form;
}

function sirius_node_tab_worker_contact_phone_form_validate($form, &$form_state) {
	$val = $form_state['values']['phone'];
	if ($val && !sirius_phone_validate($val)) {
		form_set_error('phone', 'Please enter a valid phone number.');
	}
}

function sirius_node_tab_worker_contact_phone_form_submit($form, &$form_state) {
	$worker_node = node_load($form_state['worker_nid']);

	$val = sirius_phone_format($form_state['values']['phone']);
	$oldval = $worker_node->field_sirius_phone['und'][0]['value'];
	if ($val == $oldval) {
		drupal_set_message("Phone number is unchanged.");
		return;
	}

	if ($val) {
		$worker_node->field_sirius_phone['und'][0]['value'] = $val;
	} else {
		unset($worker_node->field_sirius_phone['und'][0]);
	}

	$msg = t('Changed phone from @oldval to @val', array('@oldval' => ($oldval?$oldval:"[NULL]"), '@val' => ($val?$val:"[NULL]")));
	sirius_log('worker:contact', 'update', $msg, '', '', array($worker_node->nid), array());
	drupal_set_message($msg);

	node_save($worker_node);
}
