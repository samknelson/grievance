<?php

function sirius_letter_wizard_info() {
  $items = array();
  $items['letter'] = array(
    'name' => 'letter',
    'title' => 'Letter',
    'description' => 'Send a Letter.',
    'callback_step_list' => 'sirius_lettr_wizard_steps',
  );
  return $items;
}

function sirius_lettr_wizard_steps($data, $entity_node) {
  $steps = array();
  $steps['which'] = array('label' => 'Select Letter', 'callback' => 'sirius_letter_wizard_which');
  if ($data['format'] == 'email') {
  	$steps['recipients'] = array('label' => 'Recipients', 'callback' => 'sirius_letter_wizard_recipients_email');
  } else if ($data['format'] == 'fax' || $data['format'] == 'sms') {
  	$steps['recipients'] = array('label' => 'Recipients', 'callback' => 'sirius_letter_wizard_recipients_phone');
  }
  $steps['body'] = array('label' => 'Subject and Body', 'callback' => 'sirius_letter_wizard_body');
  $steps['review'] = array('label' => 'Review and Send',     'callback' => 'sirius_letter_wizard_send');

  return $steps;
}

/************************************************************************************************
 * Step 1: Which
 ************************************************************************************************/

function sirius_letter_wizard_which($info, $data, $entity_node, $current_step) { return drupal_get_form('sirius_letter_wizard_which_form', $info, $data, $entity_node, $current_step); }

function sirius_letter_wizard_which_form($form, &$form_state, $info, $data, $entity_node, $current_step) {
	$form_state['info'] = $info;
	$form_state['data'] = $data;
	$form_state['entity_node'] = $entity_node;
	$form_state['current_step'] = $current_step;
	$steps = sirius_wizard_steps($info, $data, $entity_node);

	$letters = sirius_letter_list($entity_node);
	if (!$letters) { drupal_set_message("You do not have access to any letter templates for this content type"); }
	$letter_options = array(NULL => t('-- Select --'));
	if ($letters) {
		foreach ($letters as $letter_nid => $letter_node) {
			$letter_options[$letter_nid] = $letter_node->title;
		}
	}

	for ($i=0; $i<5; ++$i) {
		$form["letter_nid_$i"] = array(
			'#type' => 'select',
			'#title' => t('Extra Letter @i', array('@i' => $i)),
			'#default_value' => $data["letter_nid_$i"],
			'#options' => $letter_options,
		);
		if ($i == 0) {
			$form["letter_nid_$i"]['#title'] = t('Main Letter');
			$form["letter_nid_$i"]['#required'] = TRUE;
		} else if ($i == 1) {
			$form["letter_nid_$i"]['#description'] = t('Extra letters are appended to the main letter, but do not affect the recipients or subject.');
		} 
	}

	$format_options = array(
		NULL => t('-- Select --'),
		'email' => t('Email'),
		'pdf' => t('PDF Download'),
	);
	if (sirius_fax_enabled()) { $format_options['fax'] = t('Fax'); }
	if (sirius_sms_enabled()) { $format_options['sms'] = t('SMS Text Message'); }

	$form["format"] = array(
		'#type' => 'select',
		'#title' => t('Format'),
		'#default_value' => $data["format"],
		'#options' => $format_options,
		'#required' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_letter_wizard_which_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$entity_node = $form_state['entity_node'];
	$current_step = $form_state['current_step'];

  //
  // Store our form data
  //
  
  $letter_nodes = array();
  for ($i=0; $i<5; ++$i) {
  	$letter_nid = $form_state['values']["letter_nid_$i"];
  	$data["letter_nid_$i"] = $letter_nid;
		if (!$letter_nid) { continue; }
 		$letter_nodes[$letter_nid] = node_load($letter_nid);
 		if ($i == 0) { $main_letter_node = $letter_nodes[$letter_nid]; }
  }
  $data['format'] = $form_state['values']['format'];

  //
  // Generate default subject, body, and PDF body
  //

	$subject = '';
	$body = '';
	$pdf_body = '';
	foreach ($letter_nodes as $letter_nid => $letter_node) {
		if ($letter_node->field_grievance_notify_subject['und'][0]['value']) {
			if (!$subject) { 
				$subject .= $letter_node->field_grievance_notify_subject['und'][0]['value'];
			}
		}
		if (strlen($letter_node->field_grievance_notify_body['und'][0]['value']) > 10) {
			if ($body) { $body .= '<hr>'; }
			$body .= $letter_node->field_grievance_notify_body['und'][0]['value'];
		}
		if (strlen($letter_node->body['und'][0]['value']) > 10) {
			if ($pdf_body) { $pdf_body .= '<hr>'; }
			$pdf_body .= $letter_node->body['und'][0]['value'];
		}
	}

	if ($data['format'] == 'sms') {
		$body = drupal_html_to_text("$subject\n\n$body");
		$subject = '';
	}

	$subject = token_replace($subject, array('node' => $entity_node), array());
	$body = token_replace($body, array('node' => $entity_node), array());
	$pdf_body = token_replace($pdf_body, array('node' => $entity_node), array());
	$data['subject'] = $subject;
	$data['body'] = $body;
	$data['pdf_body'] = $pdf_body;

  // /////////////////////////////////////////////////////////////////
	// Generate the default list of recipients
  // /////////////////////////////////////////////////////////////////
  //
  // A lot of logic here. At some point this probably could happily become a set of callbacks. But for now,
  // we're just building it all in one place.
  //
  
  if ($data['format'] == 'email') {
		$recipients = array();

   	// Template recipients
		$json = json_decode($main_letter_node->field_sirius_json['und'][0]['value'], TRUE);
		if ($json['recipients']) {
			foreach ($json['recipients'] as $recipient) {
				if ($recipients[$recipient['email']]) { continue; }
				$delta = $recipient['delta'];
				$recipients[$recipient['email']] = $recipient;
			}
			++$delta;
		}

  	// Current user
		global $user;
		if (!$recipients[$user->mail]) {
			$recipients[$user->mail] = array(
				'name' => $user->name,
				'email' => $user->mail,
				'description' => 'Self',
				'disposition' => 'from',
				'delta' => $delta++,
			);
		}

		if ($entity_node->type == 'sirius_worker') {
			$email = $entity_node->field_sirius_email['und'][0]['value'];
			if ($email && !$recipients[$email]) {
				$recipient = array();
				$recipient['description'] = 'Worker';
				$recipient['name'] = $entity_node->title;
				$recipient['email'] = $email;
				$recipient['delta'] = $delta++;
				$recipient['disposition'] = 'skip';
				$recipients[$email] = $recipient;
			}
		}

		if ($json['recipient_options']['level_notification']) {
			list ($ms_tid, $level_code) = explode('|', $json['recipient_options']['level_notification']);
			$ms_term = taxonomy_term_load($ms_tid);
			$levels = sirius_training_levels_fetch($ms_tid);
			$level = $levels[$level_code];
			$candidates = sirius_training_level_notification_recipients($worker_node, $ms_term, $level);
			if ($candidates) {
				foreach ($candidates as $candidate) {
					if (!$candidate['email']) { continue; }
					if ($recipients[$candidate['email']]) { continue; }
					$recipients[$candidate['email']] = array(
						'name' => $candidate['name'],
						'email' => $candidate['email'],
						'description' => $candidate['description'],
						'disposition' => $candidate['disposition'],
						'delta' => $delta++,
					);
				}
			}
		}

		if ($entity_node->type == 'grievance') {

		 	// Grievant
			if ($entity_node->field_grievance_email['und'][0]['email'] && !$recipients[$entity_node->field_grievance_email['und'][0]['email']]) {
				$recipient = array();
				$recipient['description'] = 'Grievant';
				$recipient['name'] = $entity_node->field_grievance_first_name['und'][0]['value'] . ' ' . $entity_node->field_grievance_last_name['und'][0]['value'];
				$recipient['email'] = $entity_node->field_grievance_email['und'][0]['email'];
				$recipient['delta'] = $delta++;
				$recipient['disposition'] = 'skip';
				$recipients[$entity_node->field_grievance_email['und'][0]['email']] = $recipient;
			}

			// Union contacts
			$fields = array(
				'field_grievance_rep_filed',
				'field_grievance_rep_organizer',
				'field_grievance_rep_lead',
				'field_grievance_rep_manager',
				'field_grievance_rep_watching',
				'field_grievance_rep_assignee',
			);

			foreach ($fields as $field) {
				if (isset($entity_node->{$field}['und'])) {
					$label = sirius_field_overrides_label('node', $entity_node->type, $field);

					foreach ($entity_node->{$field}['und'] as $tmp) {
						$rep_user = user_load($tmp['target_id']);
						if (!$rep_user) { continue; }
						if ($recipients[$rep_user->mail]) { continue; }

						$recipient = array();
						$recipient['delta'] = $delta++;
						$recipient['description'] = $label;
						$recipient['uid'] = $rep_user->uid;
						$recipient['name'] = $rep_user->name;
						$recipient['email'] = $rep_user->mail;
						if ($field == 'field_grievance_rep_watching') { $recipient['disposition'] = 'bcc'; } else { $recipient['disposition'] = 'cc'; }
						$recipients[$rep_user->mail] = $recipient;
					}
				}
			}

			// Shop contacts
			$status_tid = $entity_node->field_grievance_status['und'][0]['tid'];
			$shop_nid = $entity_node->field_grievance_shop['und'][0]['target_id'];
			$candidates = grievance_notify_company_contacts(array('shop_nid' => $shop_nid, 'status_nid' => $status_tid));
			foreach ($candidates as $candidate) {
				if (!$candidate['email']) { continue; }
				if ($recipients[$candidate['email']]) { continue; }
				$recipients[$candidate['email']] = array(
					'name' => $candidate['name'],
					'email' => $candidate['email'],
					'description' => $candidate['type'],
					'disposition' => 'cc',
					'delta' => $delta++,
				);
			}
		}
	}

	$data['recipients'] = $recipients;

	sirius_wizard_submit($info, $data, $entity_node, $current_step, "Letter template selected.");
}

/************************************************************************************************
 * Step 2: Body
 ************************************************************************************************/

function sirius_letter_wizard_body($info, $data, $entity_node, $current_step) { return drupal_get_form('sirius_letter_wizard_body_form', $info, $data, $entity_node, $current_step); }

function sirius_letter_wizard_body_form($form, &$form_state, $info, $data, $entity_node, $current_step) {
	$form_state['info'] = $info;
	$form_state['data'] = $data;
	$form_state['entity_node'] = $entity_node;
	$form_state['current_step'] = $current_step;
	$steps = sirius_wizard_steps($info, $data, $entity_node);

	if ($data['format'] == 'email') {
		$form['subject'] = array(
			'#type' => 'textfield',
			'#title' => 'subject',
			'#default_value' => $data['subject'],
		);
	}

	if ($data['format'] == 'email') {
		$form['body'] = array(
			'#type' => 'text_format',
			'#title' => 'Body',
			'#default_value' => $data['body'],
			'#rows' => 20,
		);
	}

	if ($data['format'] == 'sms') {
		$form['body'] = array(
			'#type' => 'textarea',
			'#title' => 'Body',
			'#default_value' => $data['body'],
			'#rows' => 10,
		);
	}

	if ($data['format'] == 'email' || $data['format'] == 'fax' || $data['format'] == 'pdf') {
		$form['pdf_body'] = array(
			'#type' => 'text_format',
			'#title' => 'PDF Body',
			'#default_value' => $data['pdf_body'],
			'#rows' => 20,
		);
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_letter_wizard_body_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$entity_node = $form_state['entity_node'];
	$current_step = $form_state['current_step'];

	$data['subject'] = $form_state['values']['subject'];
	if ($data['format'] == 'sms') {
	  $data['body'] = $form_state['values']['body'];
	} else {
	  $data['body'] = $form_state['values']['body']['value'];
	}
  $data['pdf_body'] = $form_state['values']['pdf_body']['value'];

	sirius_wizard_submit($info, $data, $entity_node, $current_step, "Letter body saved.");
}

/************************************************************************************************
 * Step 3: Recipients
 ************************************************************************************************/

function sirius_letter_wizard_recipients_email($info, $data, $entity_node, $current_step) { return drupal_get_form('sirius_letter_wizard_recipients_email_form', $info, $data, $entity_node, $current_step); }

function sirius_letter_wizard_recipients_email_form($form, &$form_state, $info, $data, $entity_node, $current_step) {
	$form_state['info'] = $info;
	$form_state['data'] = $data;
	$form_state['entity_node'] = $entity_node;
	$form_state['current_step'] = $current_step;
	$steps = sirius_wizard_steps($info, $data, $entity_node);

	$recipients = $data['recipients'];
	if ($recipients) { usort($recipients, function($a, $b) { return $a['delta'] <=> $b['delta']; }); }
	for ($i=0; $i<5; ++$i) {
		$recipients[] = array('description' => t('Additional Recipient #@i', array('@i' => ($i+1))));
	}

	$form['intro'] = array('#markup' => '<table><tr><th>Description</th><th>Name</th><th>Email</th><th>Disposition</th></tr>');

	$delta = 0;
	foreach ($recipients as $recipient) {
		$form["tr_$delta"] = array('#markup' => '<tr>');

		$form["description_markup_$delta"] = array('#markup' => '<td>' . $recipient['description'] . '</td>');
		$form["description_$delta"] = array('#type' => 'hidden', '#value' => $recipient['description']);

		$form["name_$delta"] = array(
			'#prefix' => '<td>',
			'#suffix' => '</td>',
			'#type' => 'textfield',
			'#default_value' => $recipient['name'],
		);

		$form["email_$delta"] = array(
			'#prefix' => '<td>',
			'#suffix' => '</td>',
			'#type' => 'textfield',
			'#default_value' => $recipient['email'],
		);

		$form["disposition_$delta"] = array(
			'#prefix' => '<td>',
			'#suffix' => '</td>',
			'#type' => 'select',
			'#default_value' => $recipient['disposition'],
			'#options' => array('skip' => t('Skip'), 'from' => t('From'), 'to' => t('To'), 'cc' => t('Cc'), 'bcc' => t('Bcc'))
		);

		if ($recipient['locked'] == 'yes') {
			$form["name_$delta"]['#disabled'] = TRUE;
			$form["email_$delta"]['#disabled'] = TRUE;
			$form["disposition_$delta"]['#disabled'] = TRUE;
		}

		$form["tr2_$delta"] = array('#markup' => '<tr>');
		++$delta;
	}

	$form['outro'] = array('#markup' => '</table>');
	$form_state['recipient_count'] = count($recipients);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_letter_wizard_recipients_email_form_validate($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$entity_node = $form_state['entity_node'];
	$current_step = $form_state['current_step'];
	$recipient_count = $form_state['recipient_count'];

	// Verify exactly one "From"
	$found_from = FALSE;
	for ($delta = 0; $delta < $recipient_count; ++$delta) {
		if ($form_state['values']["disposition_$delta"] == 'from') {
			if ($found_from) { form_set_error("disposition_$delta", 'Select exactly one recipient as the sender.'); }
			$found_from = TRUE;
		}
	}
	if (!$found_from) { form_set_error("disposition_0", 'Select exactly one recipient as the sender.'); }

	// Verify at least one "To"
	$found_to = 0;
	for ($delta = 0; $delta < $recipient_count; ++$delta) {
		if ($form_state['values']["disposition_$delta"] == 'to') {
			++$found_to;
		}
	}
	if (!$found_to) { form_set_error("disposition_0", 'Select at least one recipient as the main recipient'); }

	// Verify all email addresses
	for ($delta = 0; $delta < $recipient_count; ++$delta) {
		if ($form_state['values']["disposition_$delta"] == 'skip') { continue; }
		if (!sirius_email_validate($form_state['values']["email_$delta"])) { form_set_error("email_$delta", "Please enter a valid email address for recipient #$delta (" . $form_state['values']["email_$delta"] . ")"); }
	}
}

function sirius_letter_wizard_recipients_email_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$entity_node = $form_state['entity_node'];
	$current_step = $form_state['current_step'];
	$recipient_count = $form_state['recipient_count'];

	$recipients = array();
	for ($delta = 0; $delta < $recipient_count; ++$delta) {
		$recipient = array(
			'delta' => $delta,
			'disposition' => $form_state['values']["disposition_$delta"],
			'name' => $form_state['values']["name_$delta"],
			'description' => $form_state['values']["description_$delta"],
			'email' => $form_state['values']["email_$delta"],
		);
		if (!$recipient['email'] && !$recipient['name']) { continue; }
		$recipients[] = $recipient;
	}
	$data['recipients'] = $recipients;

	sirius_wizard_submit($info, $data, $entity_node, $current_step, "Letter body saved.");
}

/************************************************************************************************
 * Step 3: Review and Send
 ************************************************************************************************/

function sirius_letter_wizard_send($info, $data, $entity_node, $current_step) { return drupal_get_form('sirius_letter_wizard_send_form', $info, $data, $entity_node, $current_step); }

function sirius_letter_wizard_send_form($form, &$form_state, $info, $data, $entity_node, $current_step) {
	$form_state['info'] = $info;
	$form_state['data'] = $data;
	$form_state['entity_node'] = $entity_node;
	$form_state['current_step'] = $current_step;
	$steps = sirius_wizard_steps($info, $data, $entity_node);

	if ($data['format'] == 'pdf') { $submit_label = t('Download PDF'); }
	else { $submit_label = t("Send Email Now"); }

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => $submit_label,
	);

	return $form;
}

function sirius_letter_wizard_send_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$entity_node = $form_state['entity_node'];
	$current_step = $form_state['current_step'];

	if ($data['format'] == 'pdf') {
		$letter_node = node_load($data['letter_nid_0']);
		$pdf_body = $data['pdf_body'];
		$pdf_contents = sirius_letter_pdf_contents($entity_node, $letter_node, $pdf_body);
		$basename = sirius_letter_pdf_basename($entity_node, $letter_node);

		// Log the letter
		if ($form_state['values']['logging']) {
			$log_title = "Generated Letter: ";
			$is_first = TRUE;
			foreach ($letter_nodes as $letter_node) {
				if ($is_first) {
					$is_first = FALSE;
				} else {
					$log_title .= ' / ';
				}
				$log_title .= $letter_node->title;
			}

			$log_body = '';
			$log_node = grievance_letter_log($form_state, $log_title, $log_body, $pdf_contents, $basename);
		}

		// Dump to browser and exit
		header('Content-Type: application/pdf');
		header("Content-Disposition: attachment; filename=$basename.pdf");
		print $pdf_contents;
		drupal_exit();

	}

	sirius_wizard_submit($info, $data, $entity_node, $current_step, "Letter body saved.");
}

/************************************************************************************************
 * Utility Functions
 ************************************************************************************************/

function sirius_letter_htmltemplate() {
	return '===LETTER===';
}

function sirius_letter_permission_check($letter_node) {
	global $user;

	// Admin can send any letter
	if (user_access('sirius admin')) { return TRUE; }

	// @todo Legacy - grievance admin can send any letter
	if (user_access('grievance admin')) { return TRUE; }

	// No permissions set, return false
	if (!isset($letter_node->field_grievance_roles['und'])) { return FALSE; }

	// Does the user have nay of the specified roles?
	$user_roles = array_keys($user->roles);
	$letter_roles = sirius_fieldvals($letter_node, 'field_grievance_roles', 'value');
	if (array_intersect($user_roles, $letter_roles)) { return TRUE; }

	// Apparently not.
	return FALSE;
}

function sirius_letter_list($entity_node) {
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_letter_template');
  $query->fieldCondition('field_sirius_letter_content_type', 'value', $entity_node->type);
  $domain_nid = sirius_domain_nid();
  if ($domain_nid) { $query->fieldCondition('field_sirius_domain', 'target_id', $domain_nid); }
  $query->propertyOrderBy('title');
  $stmt = $query->execute();
  if (!$stmt['node']) { return array(); }
	$letter_nodes = node_load_multiple(array_keys($stmt['node']));

	$letter_nodes_with_access = array();
	foreach ($letter_nodes as $letter_nid => $letter_node) {
		if (sirius_letter_permission_check($letter_node)) { $letter_nodes_with_access[$letter_nid] = $letter_node; }
  }

  return $letter_nodes_with_access;
}


function sirius_letter_render_html($entity_node, $letter_nids, $other_tokens) {
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
				'node' => $entity_node,
				'grievance-letter' => $other_tokens,
			),
			array(
				'clear' => TRUE,
			)
		);

		if ($body) {
			$body .= '<div class="grievance-letter-page-break">&nbsp;</div>';
		}
		if ($letter_node->field_grievance_css['und'][0]['value']) {
			$body .= '<style>';
			$body .= $letter_node->field_grievance_css['und'][0]['value'];
			$body .= '</style>';
		}
		$body .= '<div class="grievance-letter-part">';
		$body .= $letter_part;
		$body .= '</div>';
	}

	$body = '<div id="grievance-letter-body">' . $body . '</div>';

	return $body;
}


function sirius_letter_pdf_basename($entity_node, $letter_node) {
	$basename = '';

	if ($entity_node->type == 'grievance') {
		if ($entity_node->field_grievance_last_name['und'][0]['value']) {
			$tmp = strtolower($entity_node->field_grievance_last_name['und'][0]['value']);
			$tmp = preg_replace('/[^a-z]/', '', $tmp);
			if ($tmp) { $basename .= $tmp . '_'; }
		}
		if ($entity_node->field_grievance_first_name['und'][0]['value']) {
			$tmp = strtolower($grievance_node->field_grievance_first_name['und'][0]['value']);
			$tmp = preg_replace('/[^a-z]/', '', $tmp);
			if ($tmp) { $basename .= $tmp . '_'; }
		}
		if ($entity_node->field_grievance_id['und'][0]['value']) {
			$tmp = strtolower($entity_node->field_grievance_id['und'][0]['value']);
			$tmp = preg_replace('/[^a-z0-9]/', '', $tmp);
			if ($tmp) { $basename .= $tmp . '_'; }
		}
	} else {
		$tmp  = strtolower($entity_node->title);
		$tmp = preg_replace('/[^a-z]/', '', $tmp);
		if ($tmp) { $basename .= $tmp . '_'; }
		if ($entity_node->field_sirius_id['und'][0]['value']) {
			$tmp = strtolower($entity_node->field_sirius_id['und'][0]['value']);
			$tmp = preg_replace('/[^a-z0-9]/', '', $tmp);
			if ($tmp) { $basename .= $tmp . '_'; }
		}
	}

	if ($letter_node->field_grievance_shortname['und'][0]['value']) {
		$tmp = strtolower($letter_node->field_grievance_shortname['und'][0]['value']);
		$tmp = preg_replace('/[^a-z0-9]/', '', $tmp);
		if ($tmp) { $basename .= $tmp . '_'; }
	}
	$basename .= date('Ymd');

	return $basename;
}

function sirius_letter_pdf_contents($entity_node, $letter_node, $letter_body_html) {
	$basename = grievance_letter_pdf_basename($entity_node, $letter_nids);

	$html_path = DRUPAL_ROOT . '/sites/default/files/private/grievance_gform/' . $basename . '.html';
	$pdf_path = DRUPAL_ROOT . '/sites/default/files/private/grievance_gform/' . $basename . '.pdf';

	// Fetch the letter template. This is silly, but there doesn't seem to be a straightforward way to tell Drupal "render this block of HTML as a page, and give me the result.".
	global $is_https;
	if ($is_https ||
		(isset($_SERVER['HTTP_X_FORWARDED_PROTO']) && $_SERVER['HTTP_X_FORWARDED_PROTO'] == 'https')) {
		$url = 'https://';
	} else { 
		$url = 'http://';
	}
	$url .= $_SERVER['SERVER_NAME'] . '/node/' . $entity_node->nid . '/loopback-letter';
	$postfields = array('letter_body' => $letter_body_html);
  $ch = curl_init();
  curl_setopt($ch, CURLOPT_URL, $url);
  curl_setopt($ch, CURLOPT_HEADER, 0);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
  if ($postfields) {
		curl_setopt($ch, CURLOPT_POST, 1);
		curl_setopt($ch, CURLOPT_POSTFIELDS, $postfields);
	}
  $html = curl_exec($ch);
  curl_close($ch);
	file_put_contents($html_path, $html);

	// Execute wkhtmltopdf
	if ($values['letterhead'] > 0) {
		$letterhead_node = node_load($values['letterhead']); 
		$json = json_decode($letterhead_node->field_sirius_json['und'][0]['value'], TRUE);
		$margin_args = '';
		foreach (array('left', 'right', 'top', 'bottom') as $which) {
			if ($json['page']["margin_$which"]) {
				$arg = escapeshellarg($json['page']["margin_$which"]);
			} else {
				$arg = "'15mm'";
			}
			$margin_args .= "--margin-$which $arg ";
		}

		$letterhead_pdf_uri = 'temporary://pdfgen_letterhead_' . $letterhead_node->nid . '.pdf';
		$letterhead_pdf_path = drupal_realpath($letterhead_pdf_uri);
		$letterhead_source = (object) $letterhead_node->field_sirius_attachments['und'][0];
		file_copy($letterhead_source, $letterhead_pdf_uri, FILE_EXISTS_REPLACE);
	}

	$cmd = '/usr/bin/xvfb-run -a ';
	$cmd .= '--server-args="-screen 0 1024x768x24" ';
	$cmd .= '/usr/local/bin/wkhtmltopdf ';
	$cmd .= $margin_args;
	$cmd .= '-s letter ';
	$cmd .= '--print-media-type ';
	$cmd .= '-q ';
	$cmd .= $html_path;
	$cmd .= ' ';
	$cmd .= $pdf_path;

	exec($cmd);
	drupal_set_message($cmd);

	if ($letterhead_node) {
		// Combine the letterhead with the source
		$cmd = "/usr/bin/pdftk $pdf_path multibackground $letterhead_pdf_path output $pdf_path.combined";
		exec($cmd);
		rename("$pdf_path.combined", "$pdf_path");
	}

	return file_get_contents($pdf_path);
}
