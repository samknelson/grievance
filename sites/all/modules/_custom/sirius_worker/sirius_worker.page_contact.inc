<?php

function sirius_node_tab_worker_contact_access($worker_node) {
	return Sirius_Worker_Access::getInstance()->access('view', array('worker_node' => $worker_node));
}

function sirius_node_tab_worker_contact($worker_node) {
	return drupal_get_form('sirius_node_tab_worker_contact_form', $worker_node);
}

function sirius_node_tab_worker_contact_form($form, &$form_state, $worker_node) {
  module_load_include('inc', 'field_ui', 'field_ui.admin');
	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/ux.css');
	$is_staff = Sirius_Worker_Access::getInstance()->access('staff');

	$contact_node = node_load($worker_node->field_sirius_contact['und'][0]['target_id']);
	if (!$contact_node) { return sirius_form_error("System error: no contact node associated with the worker."); }

	foreach (array('field_sirius_phone', 'field_sirius_phone_alt', 'field_sirius_email') as $field_name) {
		$renderable = field_view_field('node', $contact_node, $field_name, 'full');
		if ($renderable) { $html .= render($renderable); }
	}

	if ($contact_node->field_sirius_address['und'][0]) {
		$renderable = field_view_field('node', $contact_node, 'field_sirius_address', 'full');
		if ($renderable) { $html .= render($renderable); }

		$accuracy = $contact_node->field_sirius_address_accuracy['und'][0]['value'];
		if ($accuracy) {
			if ($accuracy == 'ROOFTOP') {
				$html .= sirius_fakerender_field('Geocoded?', 'Yes');
			} else {
				$html .= sirius_fakerender_field('Geocoded?', 'No');
			}
			if ($is_staff) {
				$renderable = field_view_field('node', $contact_node, 'field_sirius_address_accuracy', 'full');
				if ($renderable) { $html .= render($renderable); }
			}
		}

		$renderable = field_view_field('node', $contact_node, 'field_sirius_address_geo', 'full');
		if ($renderable) { $html .= render($renderable); }

		$html .= '</div>';

	}
	$form['intro'] = array('#markup' => $html);


  if (user_access('sirius worker admin')) {
  	$contact_nid = $worker_node->field_sirius_contact['und'][0]['target_id'];
  	$html = '<div class="sirius_form_intro">';
  	$html .= t('This worker is associated with: <strong>Contact #@contact_nid</strong>', array('@contact_nid' => $contact_nid));
  	$html .= l('View', 'node/' . $contact_nid, ['attributes' => ['class' => ['tool-button']]]);
  	$html .= '</div>';
  	$form['contact_intro'] = array('#markup' => $html);
  }

	$form_state['worker_nid'] = $worker_node->nid;

	$worker_json = sirius_json_get($worker_node);

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

	$disclaimer_nid = sirius_domain_variable_get('sirius_sms_optin_disclaimer_nid');
	if ($disclaimer_nid) {
		$disclaimer_node = node_load($disclaimer_nid);
		$disclaimer_json = sirius_json_get($disclaimer_node)['disclaimer'];
		// $acceptance_json = $worker_json['disclaimer'][$disclaimer_nid]['acceptances'][0];
		$form['disclaimer'][$disclaimer_nid] = sirius_disclaimer_render_form($disclaimer_json, $acceptance_json);			
	}

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
}

function sirius_node_tab_worker_contact_form_submit($form, &$form_state) {
	drupal_alter('sirius_node_tab_worker_contact_form_submit', $form, $form_state);

	$worker_node = node_load($form_state['worker_nid']);
	global $user;

	$disclaimer_nid = sirius_domain_variable_get('sirius_sms_optin_disclaimer_nid');
	if ($disclaimer_nid) {
		$disclaimer_node = node_load($disclaimer_nid);
		sirius_disclaimer_accept($disclaimer_node, array($disclaimer_node), array());
	}

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

	$val = sirius_email_format($form_state['values']['email']);
	$oldval = $worker_node->field_sirius_email['und'][0]['value'];
	if ($val != $oldval) {
		$worker_node->field_sirius_email['und'][0]['value'] = $val;
		$need_save = TRUE;
		$msg = t('Changed email from @oldval to @val', array('@oldval' => ($oldval?$oldval:"[NULL]"), '@val' => ($val?$val:"[NULL]")));
		sirius_log('worker:contact', 'update', $msg, '', '', array($worker_node->nid), array());
		drupal_set_message($msg);
	}

	if ($need_save) {
		node_save($worker_node);
	}
}
