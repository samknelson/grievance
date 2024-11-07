<?php

function sirius_node_tab_worker_contact_access($worker_node) {
	// @todo Redundant
	return node_access('view', $worker_node);
}

function sirius_node_tab_worker_contact($worker_node) {
	return drupal_get_form('sirius_node_tab_worker_contact_form', $worker_node);
}

function sirius_node_tab_worker_contact_form($form, &$form_state, $worker_node) {
  module_load_include('inc', 'field_ui', 'field_ui.admin');

  if (user_access('sirius worker admin')) {
  	$contact_nid = $worker_node->field_sirius_contact['und'][0]['target_id'];
  	$html = '<div class="sirius_form_intro">';
  	$html .= t('This worker is associated with: <strong>Contact #@contact_nid</strong>', array('@contact_nid' => $contact_nid));
  	$html .= l('View', 'node/' . $contact_nid, ['attributes' => ['class' => ['tool-button']]]);
  	$html .= '</div>';
  	$form['contact_intro'] = array('#markup' => $html);
  }

	$form_state['worker_node'] = $worker_node;

	$form['email'] = array(
		'#title' => t('Email Address'),
		'#type' => 'textfield',
		'#default_value' => $worker_node->field_sirius_email['und'][0]['value'],
	);

	$form['phone'] = array(
		'#title' => t('Mobile Phone Number'),
		'#type' => 'textfield',
		'#size' => 15,
		'#default_value' => $worker_node->field_sirius_phone['und'][0]['value'],
	);

	$form['phone_alt'] = array(
		'#title' => t('Secondary Phone Number'),
		'#type' => 'textfield',
		'#size' => 15,
		'#default_value' => $worker_node->field_sirius_phone_alt['und'][0]['value'],
	);

	$form['phone_fax'] = array(
		'#title' => t('Fax'),
		'#type' => 'textfield',
		'#size' => 15,
		'#default_value' => $worker_node->field_sirius_phone_fax['und'][0]['value'],
	);

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

function sirius_node_tab_worker_contact_form_validate($form, &$form_state) {
	$email = $form_state['values']['email'];
	if ($email && !sirius_email_validate($email)) {
		form_set_error('email', t('Please enter a valid email address.'));
	}

	$phone = sirius_phone_format($form_state['values']['phone']);
	if ($phone && !sirius_phone_validate($phone)) {
		form_set_error('phone', t('Please enter a valid phone number.'));
	}

	$phone_alt = sirius_phone_format($form_state['values']['phone_alt']);
	if ($phone_alt && !sirius_phone_validate($phone_alt)) {
		form_set_error('phone_alt', t('Please enter a valid phone number.'));
	}

	$phone_fax = sirius_phone_format($form_state['values']['phone_fax']);
	if ($phone_fax && !sirius_phone_validate($phone_fax)) {
		form_set_error('phone_fax', t('Please enter a valid fax number.'));
	}
}

function sirius_node_tab_worker_contact_form_submit($form, &$form_state) {
	drupal_alter('sirius_node_tab_worker_contact_form_submit', $form, $form_state);

	$worker_node = $form_state['worker_node'];
	global $user;

	$need_save = FALSE;
	$val = sirius_phone_format($form_state['values']['phone']);
	$oldval = $worker_node->field_sirius_phone['und'][0]['value'];
	if ($val != $oldval) {
		$worker_node->field_sirius_phone['und'][0]['value'] = $val;
		$need_save = TRUE;
		$msg = t('Changed primary phone number from @oldval to @val', array('@oldval' => ($oldval?$oldval:"[NULL]"), '@val' => ($val?$val:"[NULL]")));
		sirius_log('worker:contact', 'update', $msg, '', '', array($worker_node->nid), array());
		drupal_set_message($msg);
	}

	$val = sirius_phone_format($form_state['values']['phone_alt']);
	$oldval = $worker_node->field_sirius_phone_alt['und'][0]['value'];
	if ($val != $oldval) {
		$worker_node->field_sirius_phone_alt['und'][0]['value'] = $val;
		$need_save = TRUE;
		$msg = t('Changed secondary phone number from @oldval to @val', array('@oldval' => ($oldval?$oldval:"[NULL]"), '@val' => ($val?$val:"[NULL]")));
		sirius_log('worker:contact', 'update', $msg, '', '', array($worker_node->nid), array());
		drupal_set_message($msg);
	}

	$val = sirius_phone_format($form_state['values']['phone_fax']);
	$oldval = $worker_node->field_sirius_phone_fax['und'][0]['value'];
	if ($val != $oldval) {
		$worker_node->field_sirius_phone_fax['und'][0]['value'] = $val;
		$need_save = TRUE;
		$msg = t('Changed fax number from @oldval to @val', array('@oldval' => ($oldval?$oldval:"[NULL]"), '@val' => ($val?$val:"[NULL]")));
		sirius_log('worker:contact', 'update', $msg, '', '', array($worker_node->nid), array());
		drupal_set_message($msg);
	}

	$val = sirius_email_format($form_state['values']['email']);
	$oldval = $worker_node->field_sirius_email['und'][0]['value'];
	if ($val != $oldval) {
		$worker_node->field_sirius_email['und'][0]['value'] = $val;
		$need_save = TRUE;
		$msg = t('Changed email from @oldval to @val', array('@oldval' => ($oldval?$oldval:"[NULL]"), '@val' => ($val?$val:"[NULL]")));
		sirius_log('worker:contact', 'update', $msg, '', '', array($worker_node->nid), array());
		drupal_set_message($msg);
	}

	$addr_changed = FALSE;
	$val = sirius_addr_format_oneline($form_state['values']);
	$oldval = sirius_addr_format_oneline($worker_node->field_sirius_address['und'][0]);
	if ($val != $oldval) {
		$keys = array('thoroughfare', 'premise', 'locality', 'administrative_area', 'postal_code');
		foreach ($keys as $key) {
			$worker_node->field_sirius_address['und'][0][$key] = $form_state['values'][$key];
		}
		$need_save = TRUE;
		$msg = t('Changed address from @oldval to @val', array('@oldval' => ($oldval?$oldval:"[NULL]"), '@val' => ($val?$val:"[NULL]")));
		sirius_log('worker:contact', 'update', $msg, '', '', array($worker_node->nid), array());
		drupal_set_message($msg);
	}

	if ($need_save) {
		node_save($worker_node);
	}
}
