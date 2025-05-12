<?php

/************************************************************************************************
 * Step 1: Which
 ************************************************************************************************/

function sirius_letter_wizard_which($info, $data, $letter_node, $current_step) { return drupal_get_form('sirius_letter_wizard_which_form', $info, $data, $letter_node, $current_step); }

function sirius_letter_wizard_which_form($form, &$form_state, $info, $data, $letter_node, $current_step) {
	$form_state['info'] = $info;
	$form_state['data'] = $data;
	$form_state['letter_node'] = $letter_node;
	$form_state['current_step'] = $current_step;

	$entity_node = node_load($letter_node->field_sirius_log_handler['und'][0]['target_id']);
	$steps = sirius_wizard_steps($info, $data, $entity_node);

	$letter_templates = sirius_letter_template_list($entity_node);
	if (!$letter_templates) { drupal_set_message("You do not have access to any letter templates for this content type"); }
	$letter_options = array(NULL => t('-- Select --'));
	if ($letter_templates) {
		foreach ($letter_templates as $letter_template_nid => $letter_template_node) {
			$letter_options[$letter_template_nid] = $letter_template_node->title;
		}
	}


	$form['#tree'] = TRUE;
	for ($i=0; $i<5; ++$i) {
		$form["letter_template_nid_$i"] = array(
			'#type' => 'select',
			'#title' => t('Extra Letter @i', array('@i' => $i)),
			'#default_value' => $data["letter_template_nid_$i"],
			'#options' => $letter_options,
		);
		if ($i == 0) {
			$form["letter_template_nid_$i"]['#title'] = t('Main Letter');
			$form["letter_template_nid_$i"]['#required'] = TRUE;
		} else if ($i == 1) {
			$form["letter_template_nid_$i"]['#description'] = t('Extra letters are appended to the main letter, but do not affect the recipients or subject.');
		} 
	}

	$format_options = array(NULL => t('-- Select --')) + sirius_letter_format_options($entity_node);
	$form["format"] = array(
		'#type' => 'select',
		'#title' => t('Format'),
		'#default_value' => $data["format"],
		'#options' => $format_options,
		'#required' => TRUE,
	);

	$delivery_options = sirius_taxonomy_load('grievance_delivery', 'tid', 'name', TRUE); 
	if (count($delivery_options) > 1) {
		$form['tokens']['delivery'] = array(
			'#type' => 'select',
			'#options' => $delivery_options,
			'#title' => t('Show Delivery As'),
			'#default_value' => $data['tokens']['delivery'],
		);
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_letter_wizard_which_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$letter_node = $form_state['letter_node'];

	$entity_node = node_load($letter_node->field_sirius_log_handler['und'][0]['target_id']);
	$current_step = $form_state['current_step'];

  //
  // Store our form data
  //
  
  $letter_template_nodes = array();
  for ($i=0; $i<5; ++$i) {
  	$letter_template_nid = $form_state['values']["letter_template_nid_$i"];
  	$data["letter_template_nid_$i"] = $letter_template_nid;
		if (!$letter_template_nid) { continue; }
 		$letter_template_nodes[$letter_template_nid] = node_load($letter_template_nid);
 		if ($i == 0) { $main_letter_template_node = $letter_template_nodes[$letter_template_nid]; }
  }
  $data['format'] = $form_state['values']['format'];

  //
  // Generate default subject, body, and PDF body
  //

	$subject = '';
	$body = '';
	$pdf_body = '';
	foreach ($letter_template_nodes as $letter_template_nid => $letter_template_node) {
		if ($letter_template_node->field_grievance_notify_subject['und'][0]['value']) {
			if (!$subject) { 
				$subject .= $letter_template_node->field_grievance_notify_subject['und'][0]['value'];
			}
		}
		if (strlen($letter_template_node->field_grievance_notify_body['und'][0]['value']) > 10) {
			if ($body) { $body .= '<div class="grievance-letter-page-break">&nbsp;</div>'; }
			$body .= $letter_template_node->field_grievance_notify_body['und'][0]['value'];
		}
		if (strlen($letter_template_node->body['und'][0]['value']) > 10) {
			if ($pdf_body) { $pdf_body .= '<div class="grievance-letter-page-break">&nbsp;</div>'; }
			$pdf_body .= $letter_template_node->body['und'][0]['value'];
		}
	}

	if ($data['format'] == 'sms') {
		$body = drupal_html_to_text("$subject\n\n$body");
		$subject = '';
	}

	$tokens = array('node' => $entity_node);
	$tokens['other_tokens']['delivery_tid'] = $form_state['values']['tokens']['delivery'];

	$aliases = array();

	if ($entity_node->type == 'sirius_log') {
		if ($entity_node->field_sirius_log_handler['und']) {
			$deltas = array();
			foreach ($entity_node->field_sirius_log_handler['und'] as $tmp) {
				$handler_node = node_load($tmp['target_id']);
				$type = $handler_node->type;
				$delta = $deltas[$type];
				if (!$delta) { $delta = 0; }
				++$deltas[$type];
				$alias = "${type}_${delta}_node";
				$aliases[$alias] = 'node';
				$tokens[$alias] = $handler_node;
			}
		}
	}

	$tokens['my_node'] = $entity_node;
	$aliases['my_node'] = 'node';

	$alias_map = array(
		'sirius_worker' => 'worker_node',
		'sirius_contact' => 'contact_node',
		'grievance_shop' => 'employer_node',
	);
	
	if ($alias_map[$entity_node->type]) {
		$tokens[$alias_map[$entity_node->type]] = $entity_node;
		$aliases[$alias_map[$entity_node->type]] = 'node';
	}

	$subject = token_replace($subject, $tokens, array('clear' => TRUE, 'aliases' => $aliases));
	$body = token_replace($body, $tokens, array('clear' => TRUE, 'aliases' => $aliases));
	$pdf_body = token_replace($pdf_body, $tokens, array('clear' => TRUE, 'aliases' => $aliases));
	$data['subject'] = $subject;
	$data['body'] = $body;
	$data['pdf_body'] = $pdf_body;
	$data['tokens'] = $form_state['values']['tokens'];

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
		$json = json_decode($main_letter_template_node->field_sirius_json['und'][0]['value'], TRUE);
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
				'disposition' => 'Reply-To',
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


			// Main Contact from the grievance
			$candidate_email = $entity_node->field_grievance_co_email['und'][0]['email'];
			if ($candidate_email) {
				$recipients[$candidate_email]['email'] = $candidate_email;
				if ($entity_node->field_grievance_co_name['und'][0]['value']) {
					$recipients[$candidate_email]['name'] = $entity_node->field_grievance_co_name['und'][0]['value'];
				}
				$recipients[$candidate_email]['disposition'] = 'to';
				if (!$recipients[$candidate_email]['delta']) {
					$recipient[$candidate_email]['delta'] = $delta++;
				}
				if (!$recipients[$candidate_email]['description']) {
					$recipients[$candidate_email]['description'] = 'Assigned Shop Contact';
				}
			}

			// Shop Steward from the grievance
			$candidate_email = $entity_node->field_grievance_st_email['und'][0]['value'];
			if ($candidate_email) {
				$recipients[$candidate_email]['email'] = $candidate_email;
				if ($entity_node->field_grievance_st_name['und'][0]['value']) {
					$recipients[$candidate_email]['name'] = $entity_node->field_grievance_st_name['und'][0]['value'];
				}
				$recipients[$candidate_email]['disposition'] = 'skip';
				if (!$recipients[$candidate_email]['delta']) {
					$recipient[$candidate_email]['delta'] = $delta++;
				}
				if (!$recipients[$candidate_email]['description']) {
					$recipients[$candidate_email]['description'] = 'Assigned Shop Steward';
				}
			}

			$shop_nid = $entity_node->field_grievance_shop['und'][0]['target_id'];
		}

		if ($entity_node->type == 'grievance_shop') {
			$shop_nid = $entity_node->nid;
		}

		if ($shop_nid) {
			// Shop contacts
			$status_tid = $entity_node->field_grievance_status['und'][0]['tid'];
			$candidates = grievance_notify_company_contacts(array('shop_nid' => $shop_nid, 'status_nid' => $status_tid));
			foreach ($candidates as $candidate_nid => $candidate) {
				if (!$candidate['email']) { continue; }
				if ($recipients[$candidate['email']]) { continue; }
				$candidate_name = $candidate['name'];

				$candidate_description = $candidate['type'];
				if ($candidate['contact_type_names']) {
					if ($candidate_description) { $candidate_description .= '<br />'; }
					$candidate_description .= join('<br />', $candidate['contact_type_names']);
				}
				$recipients[$candidate['email']] = array(
					'name' => $candidate['name'],
					'email' => $candidate['email'],
					'description' => $candidate_description,
					'disposition' => 'skip',
					'delta' => $delta++,
					'nid' => $candidate_nid,
				);
			}
		}


		$context = array('entity_node' => $entity_node);
		drupal_alter('sirius_letter_recipients_email', $recipients, $context);
	}

  if ($data['format'] == 'sms') {
		$recipients = array();

   	// Template recipients
		$json = json_decode($main_letter_template_node->field_sirius_json['und'][0]['value'], TRUE);
		if ($json['recipients']) {
			foreach ($json['recipients'] as $recipient) {
				if (!$recipients[$recipient['phone']]) { continue; }
				$delta = $recipient['delta'];
				$recipients[$recipient['phone']] = $recipient;
			}
			++$delta;
		}

  	// Current user
		global $user;
		$user_full = user_load($user->uid);
		$phone = $user_full->field_grievance_phone['und'][0]['value'];
		if ($phone && !$recipients[$phone]) {
			$recipients[$phone] = array(
				'name' => $user->name,
				'phone' => $phone,
				'description' => 'Self',
				'disposition' => 'Reply-To',
				'delta' => $delta++,
			);
		}

		if ($entity_node->type == 'sirius_worker') {
			$phone = $entity_node->field_sirius_phone['und'][0]['value'];
			if ($phone && !$recipients[$phone]) {
				$recipient = array();
				$recipient['description'] = 'Worker';
				$recipient['name'] = $entity_node->title;
				$recipient['phone'] = $phone;
				$recipient['delta'] = $delta++;
				$recipient['disposition'] = 'skip';
				$recipients[$phone] = $recipient;
			}
		}

		if ($entity_node->type == 'grievance') {
		 	// Grievant
		 	$phone = $entity_node->field_grievance_phone['und'][0]['value'];
			if ($phone && !$recipients[$phone]) {
				$recipient = array();
				$recipient['description'] = 'Grievant';
				$recipient['name'] = $entity_node->field_grievance_first_name['und'][0]['value'] . ' ' . $entity_node->field_grievance_last_name['und'][0]['value'];
				$recipient['phone'] = $phone;
				$recipient['delta'] = $delta++;
				$recipient['disposition'] = 'skip';
				$recipients[$phone] = $recipient;
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
						$phone = $rep_user->field_grievance_phone['und'][0]['value'];
						if (!$phone) { continue; }

						$recipient = array();
						$recipient['delta'] = $delta++;
						$recipient['description'] = $label;
						$recipient['uid'] = $rep_user->uid;
						$recipient['name'] = $rep_user->name;
						$recipient['phone'] = $phone;
						$recipient['disposition'] = 'skip';
						$recipients[$phone] = $recipient;
					}
				}
			}

			// Shop contacts
			$status_tid = $entity_node->field_grievance_status['und'][0]['tid'];
			$shop_nid = $entity_node->field_grievance_shop['und'][0]['target_id'];
			$candidates = grievance_notify_company_contacts(array('shop_nid' => $shop_nid, 'status_nid' => $status_tid));
			foreach ($candidates as $candidate) {
				$phone = $candidate['phone'];
				if (!$phone) { continue; }
				if ($recipients[$phone]) { continue; }
				$candidate_name = $candidate['name'];
				$candidate_description = $candidate['type'];
				if ($candidate['contact_type_names']) {
					if ($candidate_description) { $candidate_description .= '<br />'; }
					$candidate_description .= join('<br />', $candidate['contact_type_names']);
				}
				$recipients[$phone] = array(
					'name' => $candidate['name'],
					'phone' => $phone,
					'description' => $candidate_description,
					'disposition' => 'skip',
					'delta' => $delta++,
				);
			}
		}
	}

  if ($data['format'] == 'postal') {
		if (!sirius_domain_component_enabled('postal')) { drupal_set('Postal module is not enabled.', 'error'); return; }
		$instance = sirius_postal_client_instance();
		if (!$instance) { drupal_set('No postal client has been configured.', 'error'); return; }

		$recipients = array();

		// Main recipient
		if ($entity_node->type == 'sirius_worker') {
			$recipient = array();
			$recipient['description'] = 'Worker';
			$recipient['name'] = $entity_node->title;
			$recipient['addr'] = $instance->addr_from_node($entity_node);
			$recipient['delta'] = 0;
			$recipient['disposition'] = 'to';
		} else {
			$recipient = array();
			$recipient['delta'] = 0;
			$recipient['disposition'] = 'to';
		}
		$recipients[] = $recipient;

		// Default sender
		$recipients[] = array(
			'delta' => 1,
			'disposition' => 'from',
			'name' => 'Sender',
			'addr' => $instance->get_from_default(),
		);
	}

	$data['recipients'] = $recipients;

	sirius_letter_set_title($letter_node);
	sirius_wizard_submit($info, $data, $letter_node, $current_step, "Letter template selected.");
	drupal_goto(current_path() . '/' . $letter_node->nid);
}
