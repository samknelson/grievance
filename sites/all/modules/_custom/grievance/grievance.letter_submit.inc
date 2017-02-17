<?php

function grievance_letter_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
	$form_state['result'] = NULL;
	$grievance_node = $form_state['grievance_node'];
	$output = $form_state['values']['output'];
	if (!$output) {
		drupal_set_message(t('No output specified.'), 'error');
		return;
	}

	// Get the letter nid
	$letter_nids = array();
	foreach ($form_state['values']['letter_nid'] as $ignore => $letter_nid) {
		if ($letter_nid) {
			$letter_nids[] = $letter_nid;
		}
	}
	if (!$letter_nids) {
		drupal_set_message(t('No letter selected.'), 'error');
		return;
	}

	$letter_nodes = node_load_multiple($letter_nids);

	// For PDF output, just go there now.
	if ($output == 'pdf') {
		// Sometimes a "letter" is just an attachment.  If so, we shortcut and just redirect to the attachment.
		foreach ($letter_nodes as $letter_nid => $letter_node) {
			if ($letter_node->field_grievance_attachments['und'][0]['uri']) {
				$url = file_create_url($letter_node->field_grievance_attachments['und'][0]['uri']);
				if ($url) {
					drupal_goto($url);
				}
			}
		}
		// Build the PDF and dump it to the browser.  Processing ends here.
		return grievance_node_tab_letter_pdf($grievance_node, $letter_nids, $form_state['values']);
	}

	// For screen preview, just output it and end processing.
	if ($output == 'preview') {
		drupal_set_message(t('The letter(s) are displayed at the bottom of this page.'));
		$form_state['result'] = grievance_letter_render_html(
			$grievance_node, 
			$letter_nids, 
			array(
				'delivery_tid' => $form_state['values']['delivery_tid'],
				'letter_body' => $form_state['values']['letter_body']['value'],
			)
		);
		return;
	}

	if ($output == 'edit') {
		drupal_set_message(t('The letter(s) can be edited at the bottom of this page.'));
		$form_state['edit_result'] = grievance_letter_render_html(
			$grievance_node, 
			$letter_nids, 
			array(
				'delivery_tid' => $form_state['values']['delivery_tid'],
				'body' => $form_state['values']['letter_body'],
			)
		);
		return;
	}

	// The only other output we know is "Send".  If it's not, we're in trouble.
	if ($output != 'send') {
		drupal_set_message(t('Unknown output: [@output]', array('@output' => $output)), 'error');
		return;
	}

	// Get the company recipients.  @TODO harcoding 100 is lazy.
	$company_recipients = array();
	for ($i=0; $i<100; ++$i) {
		$name = $form_state['values']["company_recipient_{$i}_name"];
		$email = $form_state['values']["company_recipient_{$i}_email"];
		$disposition = $form_state['values']["company_recipient_{$i}_disposition"];
		$company_contact_nid = $form_state['values']["company_recipient_{$i}_nid"];

		// Skip unless we have at least one value
		if (!$name && !$email && !$disposition) { 
			continue;
		}

		// Need email and disposition
		if (!$email || !$disposition) {
			if ($disposition != 'none') {
				drupal_set_message(t('For each company contact, enter the email address and how the contact should be handled (Skip, TO, or CC)'), 'error');
				return;
			}
		}

		$company_recipients[$disposition][] = array(
			'name' => $name, 
			'email' => $email,
			'nid' => $company_contact_nid,
		);
	}
	if (count($company_recipients['primary']) <> 1) {
		drupal_set_message(t('Exactly one company contact must be the primary recipient (TO).'), 'error');
		return;
	}

	// Get the union recipients.  @TODO harcoding 100 is lazy.
	$union_recipients = array();
	for ($i=0; $i<100; ++$i) {
		$name = $form_state['values']["union_recipient_{$i}_name"];
		$email = $form_state['values']["union_recipient_{$i}_email"];
		$disposition = $form_state['values']["union_recipient_{$i}_disposition"];

		// Skip unless we have at least one value
		if (!$name && !$email && !$disposition) { 
			continue;
		}

		// Need email and disposition
		if (!$email || !$disposition) {
			if ($disposition != 'none') {
				drupal_set_message(t('For each union contact, enter the email address and how the contact should be handled (Skip, TO, or CC)'), 'error');
				return;
			}
		}

		$union_recipients[$disposition][] = array('name' => $name, 'email' => $email);
	}
	if (count($union_recipients['primary']) <> 1) {
		drupal_set_message(t('Exactly one union contact must be the sender (FROM).'), 'error');
		return;
	}

	// We've passed the tests, and we're committed to sending the email.
	// First, we want to update the node with the primary recipient.
	$grievance_node = $form_state['grievance_node'];
	$is_changed = FALSE;

	if ($grievance_node->field_grievance_co_name['und'][0]['value'] != $company_recipients['primary'][0]['name']) {
		$grievance_node->field_grievance_co_name['und'][0]['value'] = $company_recipients['primary'][0]['name'];
		$is_changed = TRUE;
	}

	if ($grievance_node->field_grievance_co_email['und'][0]['email'] != $company_recipients['primary'][0]['email']) {
		$grievance_node->field_grievance_co_email['und'][0]['email'] = $company_recipients['primary'][0]['email'];
		$is_changed = TRUE;
	}

	if ($company_recipients['primary'][0]['nid']) {
		$company_recipient_node = node_load($company_recipients['primary'][0]['nid']);
		$fields = array(
			'field_grievance_co_phone',
			'field_grievance_co_address',
			'field_grievance_co_address_2',
			'field_grievance_co_city',
			'field_grievance_co_state',
			'field_grievance_co_zip'
		);
		foreach ($fields as $field) {
			if ($grievance_node->{$field}['und'][0]['value'] != $company_recipient_node->{$field}['und'][0]['value']) {
				$grievance_node->{$field}['und'][0]['value'] = $company_recipient_node->{$field}['und'][0]['value'];
				$is_changed = TRUE;
			}
		}
	}

	if ($is_changed) {
		node_save($grievance_node);
	}

	drupal_set_message(t('Notification FROM: ') . htmlspecialchars($from));
	drupal_set_message(t('Notification TO: ') . htmlspecialchars($to));
	if ($cc) {
		drupal_set_message(t('Notification CC: ') . htmlspecialchars($cc));
	}
	if ($bcc) {
		drupal_set_message(t('Notification BCC: ') . htmlspecialchars($bcc));
	}

}

function grievance_letter_render_html($grievance_node, $letter_nids, $other_tokens) {
	if ($other_tokens['letter_body']) {
		return '<div id="grievance-letter-body">' . $other_tokens['letter_body'] . '</div>';
	}
	$letter_nodes = node_load_multiple($letter_nids);
	$body = '';
	foreach ($letter_nodes as $letter_nid => $letter_node) {
		$letter_part = $letter_node->body['und'][0]['value'];
		$letter_part = token_replace(
			$letter_part, 
			array(
				'node' => $grievance_node,
				'grievance-letter' => $other_tokens,
			),
			array(
				'clear' => TRUE,
			)
		);

		if ($body) {
			$body .= '<div class="grievance-letter-page-break">&nbsp;</div>';
		}
		$body .= '<div class="grievance-letter-part">' . $letter_part . '</div>';
	}

	$body = '<div id="grievance-letter-body">' . $body . '</div>';

	return $body;
}
