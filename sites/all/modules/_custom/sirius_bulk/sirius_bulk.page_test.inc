<?php

function sirius_bulk_node_tab_test_access($bulk_node) {
	if (sirius_domain_nid() && !sirius_domain_component_enabled('bulk')) { return FALSE; }
	return TRUE;
}

function sirius_bulk_node_tab_test($bulk_node) {
	return drupal_get_form('sirius_bulk_node_tab_test_form', $bulk_node);
}

function sirius_bulk_node_tab_test_form($form, &$form_state, $bulk_node) {
	$form_state['bulk_nid'] = $bulk_node->nid;

	$form['#tree'] = TRUE;

	$media = sirius_bulk_media();
	$medium = $bulk_node->field_sirius_bulk_medium['und'][0]['value'];
	$form['into'] = array('#markup' => '<div class="sirius_form_intro">' . t('Message type: ') . '<strong>' . $media[$medium] . '</strong>' . '</div>');

	if ($medium == 'postal') {
		$postal_client = sirius_postal_client_instance();
		$form['to'] = array(
			'#type' => 'fieldset',
			'#title' => t('Address'),
			'#description' => t('Enter the contact NID <strong>OR</strong> a postal address.')
		);

		$form['to']['contact_nid'] = array(
			'#type' => 'textfield',
			'#size' => 10,
			'#title' => 'Contact NID',
		);

		$form['to'] += $postal_client->addr_form();

	} else {
		$form['to'] = array(
			'#type' => 'textfield',
			'#title' => t('Recipient'),
			'#description' => t('Enter the recipient: phone number, email address, postal address, or NID.'),
		);
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Send test message'),
	);

	return $form;
}

function sirius_bulk_node_tab_test_form_submit($form, &$form_state) {
	$bulk_nid = $form_state['bulk_nid'];
	$bulk_node = node_load($bulk_nid);
	$form_state['rebuild'] = TRUE;

	// Figure out the input
	$to_input = $form_state['values']['to'];
	if (is_numeric($to_input)) {
		$recipient_node = node_load($to_input); 
	} else if (is_array($to_input) && is_numeric($to_input['contact_nid'])) {
		$recipient_node = node_load($to_input['contact_nid']);
	}

	if ($recipient_node) {
		if ($recipient_node->type == 'sirius_contact') {
			$contact_node = $recipient_node;
			sirius_worker_get_by_contact_nid($contact_node->nid);
		}
		if ($recipient_node->type == 'sirius_worker') {
			$worker_node = $recipient_node;
			$contact_node = node_load($worker_node->field_sirius_contact['und'][0]['target_id']);
		}
	}

	$medium = $bulk_node->field_sirius_bulk_medium['und'][0]['value'];
	if ($medium == 'choice') {
		$worker_node = sirius_worker_get_by_contact_nid($contact_node->nid);
		if ($worker_node) {
			$medium = $worker_node->field_sirius_dispatch_medium['und'][2]['value'];
		}
	}
	if (!$medium || $medium == 'choice' || $medium == 'none') {
		$medium = 'email';
	}

	if ($medium == 'postal') { $postal_client = sirius_postal_client_instance(); }

	// Get the recipient
	if ($contact_node) {
		if ($medium == 'email') { $to = sirius_email_format($contact_node->field_sirius_email['und'][0]['value']); }
		else if ($medium == 'postal') { $to = $postal_client->addr_from_node($contact_node); }
		else { $to = sirius_phone_format($contact_node->field_sirius_phone['und'][0]['value']); }
	} else {
		if ($medium == 'email') { $to = sirius_email_format($to_input); }
		else if ($medium == 'postal') { $to = $to_input; }
		else { $to = sirius_phone_format($to_input); }
	}

	if ($medium == 'postal') {
		$to_render = $postal_client->render_addr($to, 'oneline_txt');
	} else {
		$to_render = $to;
	}

	drupal_set_message("Sending via $medium to $to_render");
	$result = sirius_bulk_deliver_impl($bulk_node, $medium, $to, array('contact_node' => $contact_node), TRUE);
	if ($result['success']) {
		drupal_set_message($result['msg']);
	} else {
		drupal_set_message($result['msg'], 'warning');
		if ($result['details']) {
			drupal_set_message($result['details'], 'warning');
		}
	}

  if ($result['comm']) {
    $path = 'node/' . $result['comm']->nid . '/sirius_log_comm';
    $msg = "The message can be viewed and tracked <strong>" . l('here', $path) . '</strong>';
    drupal_set_message($msg);
    sirius_minilog($msg);
  }
}