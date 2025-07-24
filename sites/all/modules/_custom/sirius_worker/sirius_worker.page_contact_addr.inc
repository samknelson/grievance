<?php

function sirius_node_tab_worker_contact_addr_access($worker_node) {
	return Sirius_Worker_Access::getInstance()->access('update addr', array('worker_node' => $worker_node));
}

function sirius_node_tab_worker_contact_addr_form($form, &$form_state, $worker_node) {
	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/ux.css');

	$is_staff = Sirius_Worker_Access::getInstance()->access('staff');

	$form_state['worker_nid'] = $worker_node->nid;
	$form['#tree'] = TRUE;

	$form['intro'] = array();

	$form['thoroughfare'] = array(
		'#type' => 'textfield',
		'#title' => t('Address'),
		'#default_value' => $worker_node->field_sirius_address['und'][0]['thoroughfare'],
	);

	$form['premise'] = array(
		'#type' => 'textfield',
		'#title' => t('Address 2'),
		'#default_value' => $worker_node->field_sirius_address['und'][0]['premise'],
	);

	$form['locality'] = array(
		'#type' => 'textfield',
		'#title' => t('City'),
		'#default_value' => $worker_node->field_sirius_address['und'][0]['locality'],
	);

	$form['administrative_area'] = array(
		'#type' => 'textfield',
		'#title' => t('State'),
		'#default_value' => $worker_node->field_sirius_address['und'][0]['administrative_area'],
		'#size' => 2,
	);

	$form['postal_code'] = array(
		'#type' => 'textfield',
		'#title' => t('Zip'),
		'#default_value' => $worker_node->field_sirius_address['und'][0]['postal_code'],
		'#size' => 5,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Submit',
	);

	return $form;
}

function sirius_node_tab_worker_contact_addr_form_submit($form, &$form_state) {
	$worker_node = node_load($form_state['worker_nid']);

	$addr_changed = FALSE;
	$val = sirius_addr_format_oneline($form_state['values']);
	$oldval = sirius_addr_format_oneline($worker_node->field_sirius_address['und'][0]);
	if ($val == $oldval) {
		drupal_set_message("Address is unchanged.");
		return;
	}

	$keys = array('thoroughfare', 'premise', 'locality', 'administrative_area', 'postal_code');
	foreach ($keys as $key) {
		$worker_node->field_sirius_address['und'][0][$key] = $form_state['values'][$key];
	}
	$need_save = TRUE;
	$msg = t('Changed address from @oldval to @val', array('@oldval' => ($oldval?$oldval:"[NULL]"), '@val' => ($val?$val:"[NULL]")));
	sirius_log('worker:contact', 'update', $msg, '', '', array($worker_node->nid), array());
	drupal_set_message($msg);

	node_save($worker_node);

	$contact_node = node_load($worker_node->field_sirius_contact['und'][0]['target_id']);
	$accuracy = $contact_node->field_sirius_address_accuracy['und'][0]['value'];
	if ($accuracy && ($accuracy != 'ROOFTOP')) {
		drupal_set_message("The geocoding service was not able to find the precise location for this address. An approximate location will be stored.", 'warning');
	}
}
