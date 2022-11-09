<?php

function sirius_dispatch_node_tab_worker_pref_notifications_form($form, &$form_state, $worker_node) {
	$form_state['worker_nid'] = $worker_node->nid;

	$medium_options = sirius_dispatch_notify_medium_options($worker_node);

	$form['medium'] = array(
		'#title' => t('Send the first dispatch notification by:'),
		'#type' => 'select',
		'#options' => $medium_options,
		'#default_value' => $worker_node->field_sirius_dispatch_medium['und'][0]['value'],
	);

	$form['medium_1'] = array(
		'#title' => t('Send the second dispatch notification by:'),
		'#type' => 'select',
		'#options' => $medium_options,
		'#default_value' => $worker_node->field_sirius_dispatch_medium['und'][1]['value'],
	);

	unset($medium_options['voice']);
	unset($medium_options['voice_alt']);
	$form['medium_2'] = array(
		'#title' => t('Send other notifications (skill expirations, termination notifications, etc.) by:'),
		'#type' => 'select',
		'#options' => $medium_options,
		'#default_value' => $worker_node->field_sirius_dispatch_medium['und'][2]['value'],
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_dispatch_node_tab_worker_pref_notifications_form_submit($form, &$form_state) {
	$worker_node = node_load($form_state['worker_nid']);

	$val = $form_state['values']['medium'];
	$oldval = $worker_node->field_sirius_dispatch_medium['und'][0]['value'];
	if ($val != $oldval) {
		$worker_node->field_sirius_dispatch_medium['und'][0]['value'] = $val;
		$need_save = TRUE;
		$msg = t('Changed primary medium from @oldval to @val', array('@oldval' => ($oldval?$oldval:"[NULL]"), '@val' => ($val?$val:"[NULL]")));
		sirius_log('worker:dispatch', 'update_preferences', $msg, '', '', array($worker_node->nid), array());
		drupal_set_message($msg);
	}

	$val = $form_state['values']['medium_1'];
	$oldval = $worker_node->field_sirius_dispatch_medium['und'][1]['value'];
	if ($val != $oldval) {
		$worker_node->field_sirius_dispatch_medium['und'][1]['value'] = $val;
		$need_save = TRUE;
		$msg = t('Changed secondary medium from @oldval to @val', array('@oldval' => ($oldval?$oldval:"[NULL]"), '@val' => ($val?$val:"[NULL]")));
		sirius_log('worker:dispatch', 'update_preferences', $msg, '', '', array($worker_node->nid), array());
		drupal_set_message($msg);
	}

	$val = $form_state['values']['medium_2'];
	$oldval = $worker_node->field_sirius_dispatch_medium['und'][2]['value'];
	if ($val != $oldval) {
		$worker_node->field_sirius_dispatch_medium['und'][2]['value'] = $val;
		$need_save = TRUE;
		$msg = t('Changed non-dispatch medium from @oldval to @val', array('@oldval' => ($oldval?$oldval:"[NULL]"), '@val' => ($val?$val:"[NULL]")));
		sirius_log('worker:dispatch', 'update_preferences', $msg, '', '', array($worker_node->nid), array());
		drupal_set_message($msg);
	}

	if ($need_save) { node_save($worker_node); }
}