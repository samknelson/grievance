<?php

function sirius_letter_wizard_info() {
  $items = array();
  $items['letter'] = array(
    'name' => 'letter',
    'title' => 'Letter',
    'description' => 'Send a Letter.',
    'callback_step_list' => 'sirius_letter_wizard_steps',
  );
  return $items;
}

function sirius_letter_wizard_steps($data, $entity_node) {
  $steps = array();
  $steps['which'] = array('label' => 'Select Letter', 'callback' => 'sirius_letter_wizard_which');
  if ($data['format'] == 'email') {
  	$steps['recipients'] = array('label' => 'Recipients', 'callback' => 'sirius_letter_wizard_recipients_email');
  } else if ($data['format'] == 'sms') {
  	$steps['recipients'] = array('label' => 'Recipients', 'callback' => 'sirius_letter_wizard_recipients_phone');
  } else if ($data['format'] == 'fax') {
  	$steps['recipients'] = array('label' => 'Recipients', 'callback' => 'sirius_letter_wizard_recipients_fax');
  }
  $steps['body'] = array('label' => 'Subject and Body', 'callback' => 'sirius_letter_wizard_body');
  $steps['send'] = array('label' => 'Send', 'callback' => 'sirius_letter_wizard_send');
  $steps['review'] = array('label' => 'Review', 'callback' => 'sirius_letter_wizard_review');

  return $steps;
}

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
			if ($body) { $body .= '<hr>'; }
			$body .= $letter_template_node->field_grievance_notify_body['und'][0]['value'];
		}
		if (strlen($letter_template_node->body['und'][0]['value']) > 10) {
			if ($pdf_body) { $pdf_body .= '<hr>'; }
			$pdf_body .= $letter_template_node->body['und'][0]['value'];
		}
	}

	if ($data['format'] == 'sms') {
		$body = drupal_html_to_text("$subject\n\n$body");
		$subject = '';
	}

	$tokens = array('node' => $entity_node);
	$tokens['other_tokens']['delivery_tid'] = $data['tokens']['delivery'];

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

			// Shop contacts
			$status_tid = $entity_node->field_grievance_status['und'][0]['tid'];
			$shop_nid = $entity_node->field_grievance_shop['und'][0]['target_id'];
			$candidates = grievance_notify_company_contacts(array('shop_nid' => $shop_nid, 'status_nid' => $status_tid));
			foreach ($candidates as $candidate) {
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
				);
			}
		}
	}


  // /////////////////////////////////////////////////////////////////
	// Generate the default list of recipients
  // /////////////////////////////////////////////////////////////////
  //
  // A lot of logic here. At some point this probably could happily become a set of callbacks. But for now,
  // we're just building it all in one place.
  //
  
  if ($data['format'] == 'sms') {
		$recipients = array();

   	// Template recipients
		$json = json_decode($main_letter_template_node->field_sirius_json['und'][0]['value'], TRUE);
		if ($json['recipients']) {
			foreach ($json['recipients'] as $recipient) {
				if ($recipients[$recipient['phone']]) { continue; }
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
						if ($phone) { continue; }

						$recipient = array();
						$recipient['delta'] = $delta++;
						$recipient['description'] = $label;
						$recipient['uid'] = $rep_user->uid;
						$recipient['name'] = $rep_user->name;
						$recipient['email'] = $rep_user->mail;
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

	$data['recipients'] = $recipients;

	sirius_letter_set_title($letter_node);
	sirius_wizard_submit($info, $data, $letter_node, $current_step, "Letter template selected.");
	drupal_goto(current_path() . '/' . $letter_node->nid);
}

/************************************************************************************************
 * Step 2: Body
 ************************************************************************************************/

function sirius_letter_wizard_body($info, $data, $letter_node, $current_step) { return drupal_get_form('sirius_letter_wizard_body_form', $info, $data, $letter_node, $current_step); }

function sirius_letter_wizard_body_form($form, &$form_state, $info, $data, $letter_node, $current_step) {
	$form_state['info'] = $info;
	$form_state['data'] = $data;
	$form_state['letter_nid'] = $letter_node->nid;
	$form_state['current_step'] = $current_step;

	$entity_node = node_load($letter_node->field_sirius_log_handler['und'][0]['target_id']);
	$steps = sirius_wizard_steps($info, $data, $entity_node);

	if ($data['format'] == 'sms') {
		// Add a link shortener
	  drupal_add_js(drupal_get_path('module', 'sirius') . '/js/sirius.popup.js');
	  drupal_add_css(drupal_get_path('module', 'sirius') . '/css/sirius.popup.css');
	  $html .= '<br /><a href="#" class="sirius_popup_trigger tool-button">' . t('Link Shortener') . '</a>';
	  $html .= '<div style="display:none" class="sirius_popup_overlay"></div>';
	  $html .= '<div style="display:none" class="sirius_popup_wrap">';
	  $html .= '<div class="sirius_popup_contents">';
	  $shorten_block = block_load('shorten', 'shorten');
		$shorten_build = module_invoke('shorten', 'block_view', 'shorten');
		drupal_alter(array('block_view', "block_view_shorten_shorten"), $shorten_build, $shorten_block);
		$html .= $shorten_build['content'];
	  $html .= '</div>';
	  $html .= '<div class="sirius_popup_close"></div>';
	  $html .= '</div>';
	  $form['#prefix'] = $html;
	}

	if ($data['format'] == 'email') {
		$form['subject'] = array(
			'#type' => 'textfield',
			'#title' => t('Subject'),
			'#default_value' => $data['subject'],
		);
	}

	if ($data['format'] == 'email') {
		$form['body'] = array(
			'#type' => 'text_format',
			'#title' => t('Body'),
			'#default_value' => $data['body'],
			'#rows' => 20,
			'#format' => 'sirius_form_editor',
			'#allowed_formats' => array('full_html'),
		);
	}

	if ($data['format'] == 'sms') {
		$form['body'] = array(
			'#type' => 'textarea',
			'#title' => 'Body',
			'#default_value' => $data['body'],
			'#rows' => 10,
			'#id' => 'sms_body',
			'#description' => sirius_sms_segment_count('sms_body'),
		);
	}

	if ($data['format'] == 'email' || $data['format'] == 'fax' || $data['format'] == 'pdf') {
		$form['pdf_body'] = array(
			'#type' => 'text_format',
			'#title' => 'PDF Body',
			'#default_value' => $data['pdf_body'],
			'#rows' => 20,
			'#format' => 'sirius_form_editor',
			'#allowed_formats' => array('full_html'),
		);

		if (user_access('grievance letter variable letterhead')) {
			$options = array(NULL => t('No Letterhead'));
			$letterheads = sirius_letterhead_list();
			foreach ($letterheads as $nid => $title) {
				$options[$nid] = $title;
			}
		  $form['pdf_letterhead'] = array(
		    '#type' => 'select',
		    '#options' => $options,
		    '#title' => t('Letterhead'),
				'#default_value' => $data['pdf_letterhead'],
				'#format' => 'sirius_form_editor',
				'#allowed_formats' => array('full_html'),
		  );
		}
	}

	if ($data['format'] == 'email') {

		$other_material_nids = array();

		$query = new EntityFieldQuery();
	  $query->entityCondition('entity_type', 'node');
	  $query->entityCondition('bundle', 'grievance_attachment');
	  $query->fieldCondition('field_grievance', 'target_id', $entity_node->nid);
	  $query->propertyOrderBy('nid');
	  $stmt = $query->execute();
	  if ($stmt['node']) { $other_material_nids += array_keys($stmt['node']); }

	  $other_material_nodes = node_load_multiple($other_material_nids);
	  $other_material_options = array();
	  foreach ($other_material_nodes as $other_material_nid => $other_material_node) {
	  	if (!$other_material_node->field_grievance_attachments['und'][0]['fid']) { continue; }
	  	$other_material_options[$other_material_nid] = $other_material_node->title;
	  }
	  if ($other_material_options) {
	  	$form['upload_others'] = array(
	  		'#title' => t('Attach these materials'),
	  		'#type' => 'checkboxes',
	  		'#options' => $other_material_options,
	  	);
	  	if ($data['upload_others']) { $form['upload_others']['#default_value'] = $data['upload_others']; }
	  }

		$renderable = field_view_field('node', $letter_node, 'field_sirius_attachments', 'full');
		$renderable['#title'] = 'Currently Uploads';
		$form['uploads_current'] = array('#markup' => drupal_render($renderable));

		for ($i=0; $i<3; ++$i) {
			$form["upload_$i"] = array(
				'#type' => 'file',
				'#title' => t('Upload #@i', array('@i' => ($i+1))),
			);
		}

	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_letter_wizard_body_form_validate($form, &$form_state) {
	$info = field_info_instance('node', 'field_sirius_attachments', 'sirius_log');
	$extensions = $info['settings']['file_extensions'];
	for ($delta = 0; $delta < 3; ++$delta) {
	  $file = file_save_upload("upload_$delta", array(
	    'file_validate_extensions' => array($extensions),
	  ));
	  if ($file) {
	    $name = rand();
	    if ($file = file_move($file, 'private://attach_' . rand())) {
	      $form_state['storage']["attach_$delta"] = $file;
	    }
	    else {
	      form_set_error("attach_$delta", t("Failed to write the uploaded file to the site's file folder."));
	    }
	  }
	}
}

function sirius_letter_wizard_body_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$letter_node = node_load($form_state['letter_nid']);
	$current_step = $form_state['current_step'];

	$files = array();
	for ($delta = 0; $delta < 3; ++$delta) {
	  $file = $form_state['storage']["attach_$delta"];
	  if ($file) {
  		$file = (array) $file;
  		$file['display'] = 1;
			unset($form_state['storage']["attach_$delta"]);
  		unset($file['destination']);
  		unset($file['uri']);
  		$files[] = $file;
  	}
  }
  if ($files) { $letter_node->field_sirius_attachments['und'] = $files; }

	$entity_node = node_load($letter_node->field_sirius_log_handler['und'][0]['target_id']);

	$data['subject'] = $form_state['values']['subject'];
	$data['body'] = sirius_richtext_handle_input($form_state['values']['body']);
  $data['pdf_body'] = $form_state['values']['pdf_body']['value'];
  $data['pdf_letterhead'] = $form_state['values']['pdf_letterhead'];
  $data['upload_others'] = $form_state['values']['upload_others'];

	sirius_letter_set_title($letter_node);
	sirius_wizard_submit($info, $data, $letter_node, $current_step, "Letter body saved.");
}

/************************************************************************************************
 * Step 3: Recipients
 ************************************************************************************************/

function sirius_letter_wizard_recipients_email($info, $data, $letter_node, $current_step) { return drupal_get_form('sirius_letter_wizard_recipients_email_form', $info, $data, $letter_node, $current_step); }

function sirius_letter_wizard_recipients_email_form($form, &$form_state, $info, $data, $letter_node, $current_step) {
	$form_state['info'] = $info;
	$form_state['data'] = $data;
	$form_state['letter_nid'] = $letter_node->nid;
	$form_state['current_step'] = $current_step;

	$entity_node = node_load($letter_node->field_sirius_log_handler['und'][0]['target_id']);
	$steps = sirius_wizard_steps($info, $data, $entity_node);

	$recipients = $data['recipients'];
	if ($recipients) { usort($recipients, function($a, $b) { 
			if ($a['delta'] < $b['delta']) { return -1; }
			if ($a['delta'] > $b['delta']) { return 1; }
			return 0;
		});
	}
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
			'#options' => array('skip' => t('Skip'), 'Reply-To' => t('From'), 'to' => t('To'), 'cc' => t('Cc'), 'bcc' => t('Bcc'))
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
	$letter_node = node_load($form_state['letter_nid']);
	$current_step = $form_state['current_step'];
	$recipient_count = $form_state['recipient_count'];

	$entity_node = node_load($letter_node->field_sirius_log_handler['und'][0]['target_id']);


	// Verify exactly one "From"
	$found_from = FALSE;
	for ($delta = 0; $delta < $recipient_count; ++$delta) {
		if ($form_state['values']["disposition_$delta"] == 'Reply-To') {
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
	$letter_node = node_load($form_state['letter_nid']);
	$current_step = $form_state['current_step'];
	$recipient_count = $form_state['recipient_count'];

	$entity_node = node_load($letter_node->field_sirius_log_handler['und'][0]['target_id']);

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

	sirius_letter_set_title($letter_node);
	sirius_wizard_submit($info, $data, $letter_node, $current_step, "Recipients saved.");
}


/************************************************************************************************
 * Step 3: Recipients (Phone)
 ************************************************************************************************/

function sirius_letter_wizard_recipients_phone($info, $data, $letter_node, $current_step) { return drupal_get_form('sirius_letter_wizard_recipients_phone_form', $info, $data, $letter_node, $current_step); }

function sirius_letter_wizard_recipients_phone_form($form, &$form_state, $info, $data, $letter_node, $current_step) {
	$form_state['info'] = $info;
	$form_state['data'] = $data;
	$form_state['letter_nid'] = $letter_node->nid;
	$form_state['current_step'] = $current_step;

	$entity_node = node_load($letter_node->field_sirius_log_handler['und'][0]['target_id']);
	$steps = sirius_wizard_steps($info, $data, $entity_node);

	$recipients = $data['recipients'];
	if ($recipients) { usort($recipients, function($a, $b) { 
			if ($a['delta'] < $b['delta']) { return -1; }
			if ($a['delta'] > $b['delta']) { return 1; }
			return 0;
		});
	}
	for ($i=0; $i<5; ++$i) {
		$recipients[] = array('description' => t('Additional Recipient #@i', array('@i' => ($i+1))));
	}

	$form['intro'] = array('#markup' => '<table><tr><th>Description</th><th>Name</th><th>Phone</th><th>Disposition</th></tr>');

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

		$form["phone_$delta"] = array(
			'#prefix' => '<td>',
			'#suffix' => '</td>',
			'#type' => 'textfield',
			'#default_value' => $recipient['phone'],
		);

		$form["disposition_$delta"] = array(
			'#prefix' => '<td>',
			'#suffix' => '</td>',
			'#type' => 'select',
			'#default_value' => $recipient['disposition'],
			'#options' => array('skip' => t('Skip'), 'send' => t('Send'))
		);


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

function sirius_letter_wizard_recipients_phone_form_validate($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$letter_node = node_load($form_state['letter_nid']);
	$current_step = $form_state['current_step'];
	$recipient_count = $form_state['recipient_count'];

	$entity_node = node_load($letter_node->field_sirius_log_handler['und'][0]['target_id']);

	// Verify at least one "Send"
	$found_send = 0;
	for ($delta = 0; $delta < $recipient_count; ++$delta) {
		if ($form_state['values']["disposition_$delta"] == 'send') {
			++$found_send;
		}
	}
	if (!$found_send) { form_set_error("disposition_0", 'Select at least one recipient to send to'); }

	// Verify all email addresses
	for ($delta = 0; $delta < $recipient_count; ++$delta) {
		if ($form_state['values']["disposition_$delta"] == 'skip') { continue; }
		$phone = $form_state['values']["phone_$delta"];
		if (!sirius_phone_validate($phone)) { form_set_error("phone_$delta", "Please enter a valid phone address for recipient #$delta (" . $phone . ")"); }
	}
}

function sirius_letter_wizard_recipients_phone_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$letter_node = node_load($form_state['letter_nid']);
	$current_step = $form_state['current_step'];
	$recipient_count = $form_state['recipient_count'];

	$entity_node = node_load($letter_node->field_sirius_log_handler['und'][0]['target_id']);

	$recipients = array();
	for ($delta = 0; $delta < $recipient_count; ++$delta) {
		$recipient = array(
			'delta' => $delta,
			'disposition' => $form_state['values']["disposition_$delta"],
			'name' => $form_state['values']["name_$delta"],
			'description' => $form_state['values']["description_$delta"],
			'phone' => $form_state['values']["phone_$delta"],
		);
		if (!$recipient['phone'] && !$recipient['name']) { continue; }
		$recipients[] = $recipient;
	}
	$data['recipients'] = $recipients;

	sirius_letter_set_title($letter_node);
	sirius_wizard_submit($info, $data, $letter_node, $current_step, "Recipients saved.");
}

/************************************************************************************************
 * Step 3: Recipients (Fax)
 ************************************************************************************************/

function sirius_letter_wizard_recipients_fax($info, $data, $letter_node, $current_step) { return drupal_get_form('sirius_letter_wizard_recipients_fax_form', $info, $data, $letter_node, $current_step); }

function sirius_letter_wizard_recipients_fax_form($form, &$form_state, $info, $data, $letter_node, $current_step) {
	$form_state['info'] = $info;
	$form_state['data'] = $data;
	$form_state['letter_nid'] = $letter_node->nid;
	$form_state['current_step'] = $current_step;

	$entity_node = node_load($letter_node->field_sirius_log_handler['und'][0]['target_id']);
	$steps = sirius_wizard_steps($info, $data, $entity_node);

	$recipients = $data['recipients'];
	if ($recipients) { usort($recipients, function($a, $b) { 
			if ($a['delta'] < $b['delta']) { return -1; }
			if ($a['delta'] > $b['delta']) { return 1; }
			return 0;
		});
	}
	for ($i=0; $i<5; ++$i) {
		$recipients[] = array('description' => t('Additional Recipient #@i', array('@i' => ($i+1))));
	}

	$form['intro'] = array('#markup' => '<table><tr><th>Description</th><th>Name</th><th>Phone</th><th>Disposition</th></tr>');

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

		$form["phone_$delta"] = array(
			'#prefix' => '<td>',
			'#suffix' => '</td>',
			'#type' => 'textfield',
			'#default_value' => $recipient['phone'],
		);

		$form["disposition_$delta"] = array(
			'#prefix' => '<td>',
			'#suffix' => '</td>',
			'#type' => 'select',
			'#default_value' => $recipient['disposition'],
			'#options' => array('skip' => t('Skip'), 'send' => t('Send'))
		);


		$form["tr2_$delta"] = array('#markup' => '<tr>');
		++$delta;
		if ($delta >= 1) { break; }
	}

	$form['outro'] = array('#markup' => '</table>');
	$form_state['recipient_count'] = count($recipients);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_letter_wizard_recipients_fax_form_validate($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$letter_node = node_load($form_state['letter_nid']);
	$current_step = $form_state['current_step'];
	$recipient_count = $form_state['recipient_count'];

	$entity_node = node_load($letter_node->field_sirius_log_handler['und'][0]['target_id']);

	// Verify at least one "Send"
	$found_send = 0;
	for ($delta = 0; $delta < $recipient_count; ++$delta) {
		if ($form_state['values']["disposition_$delta"] == 'send') {
			++$found_send;
		}
	}
	if (!$found_send) { form_set_error("disposition_0", 'Select at least one recipient to send to'); }

	// Verify all email addresses
	for ($delta = 0; $delta < $recipient_count; ++$delta) {
		if ($delta >= 1) { break; }
		if ($form_state['values']["disposition_$delta"] == 'skip') { continue; }
		$phone = $form_state['values']["phone_$delta"];
		if (!sirius_phone_validate($phone)) { form_set_error("phone_$delta", "Please enter a valid phone address for recipient #$delta (" . $phone . ")"); }
	}
}

function sirius_letter_wizard_recipients_fax_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$letter_node = node_load($form_state['letter_nid']);
	$current_step = $form_state['current_step'];
	$recipient_count = $form_state['recipient_count'];

	$entity_node = node_load($letter_node->field_sirius_log_handler['und'][0]['target_id']);

	$recipients = array();
	for ($delta = 0; $delta < $recipient_count; ++$delta) {
		$recipient = array(
			'delta' => $delta,
			'disposition' => $form_state['values']["disposition_$delta"],
			'name' => $form_state['values']["name_$delta"],
			'description' => $form_state['values']["description_$delta"],
			'phone' => $form_state['values']["phone_$delta"],
		);
		if (!$recipient['phone'] && !$recipient['name']) { continue; }
		$recipients[] = $recipient;
	}
	$data['recipients'] = $recipients;

	sirius_letter_set_title($letter_node);
	sirius_wizard_submit($info, $data, $letter_node, $current_step, "Recipients saved.");
}

/************************************************************************************************
 * Step 3: Review and Send
 ************************************************************************************************/

function sirius_letter_wizard_send($info, $data, $letter_node, $current_step) { return drupal_get_form('sirius_letter_wizard_send_form', $info, $data, $letter_node, $current_step); }

function sirius_letter_wizard_send_form($form, &$form_state, $info, $data, $letter_node, $current_step) {
	$form_state['info'] = $info;
	$form_state['data'] = $data;
	$form_state['letter_nid'] = $letter_node->nid;
	$form_state['current_step'] = $current_step;
	$steps = sirius_wizard_steps($info, $data, $entity_node);

	$entity_node = node_load($letter_node->field_sirius_log_handler['und'][0]['target_id']);

	if ($entity_node->type == 'grievance') {
	  $log_type_options = sirius_taxonomy_load('grievance_log_types', 'tid', 'name', TRUE);
	  $form['logging'] = array(
	    '#type' => 'select',
	    '#options' => $log_type_options,
	    '#title' => t('Save to the grievance log as'),
	  );

		$letter_template_node = node_load($data['letter_template_nid_0']);

		if ($letter_template_node) {
			if ($letter_template_node->field_grievance_log_type['und'][0]['tid']) {
				$form['logging']['#default_value'] = $letter_template_node->field_grievance_log_type['und'][0]['tid'];
			}
			if ($letter_template_node->field_sirius_mustlog['und'][0]['value'] == 'Yes') {
				$form['logging']['#required'] = TRUE;
			}
		}
	}

	if ($data['format'] == 'pdf') { $submit_label = t('Download PDF'); }
	else if ($data['format'] == 'sms') { $submit_label = t('Send SMS Now'); }
	else if ($data['format'] == 'fax') { $submit_label = t('Send Fax Now'); }
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
	$letter_node = node_load($form_state['letter_nid']);
	$current_step = $form_state['current_step'];

	$entity_node = node_load($letter_node->field_sirius_log_handler['und'][0]['target_id']);

	$format = $data['format'];

	// Generate PDF if necessary
	if ($format == 'pdf' || $format == 'email' || $format == 'fax') {
		$pdf_basename = sirius_letter_pdf_basename($letter_node);
		$pdf_contents = sirius_letter_pdf_contents($letter_node);
	}

	// Log the letter
	if ($form_state['values']['logging']) {

		$log_title = "Letter: " . $data['format'] . ': ';
		$is_first = TRUE;
		for ($i=0; $i<100; ++$i) {
			if ($template_nid = $data['letter_template_nid_' . $i]) {
				if ($is_first) { $is_first = FALSE; } else { $log_title .= ' / '; }
				$template_node = node_load($template_nid);
				$log_title .= $template_node->title;
			}
		}
		$log_type_tid = $form_state['values']['logging'];

		$log_body = '';
		if ($data['recipients']) {
			foreach ($data['recipients'] as $recipient) {
				if ($recipient['disposition'] == 'skip') { continue; }
				if (preg_match('/\@/', $recipient_email)) {
					$log_body .= $recipient['disposition'] . ': ' . sirius_email_rfc822_format($recipient['name'], $recipient['email']) . "\n";
				} else {
					$log_body .= $recipient['disposition'] . ': ' . $recipient['name'] . ' ' . sirius_phone_format($recipient['phone']) . "\n";
				}
			}
		}
		if ($data['subject']) {
			$log_body .= "Subject: " . $data['subject'] . "\n";
		}
		if ($data['body']) {
			$log_body .= "Message: " . drupal_html_to_text($data['body']) . "\n";
		}

	  global $user;
	  $log_node = new stdClass();
	  $log_node->title = $log_title;
	  $log_node->type = 'grievance_log';
	  node_object_prepare($log_node);
	  $log_node->language = LANGUAGE_NONE;
	  $log_node->uid = $user->uid; 
	  $log_node->status = 1;
	  $log_node->promote = 0;
	  $log_node->comment = 0;
	  $log_node->field_grievance['und'][0]['target_id'] = $entity_node->nid;
	  $log_node->field_grievance_log_type['und'][0]['tid'] = $log_type_tid;
	  $log_node->field_grievance_status_date['und'][0]['value'] = date('Y-m-d H:i:s'); 
		if ($external_id) {
			$log_node->field_grievance_external_id['und'][0]['value'] = $external_id;
		}
	  $log_node->body['und'][0]['value'] = $log_body;

	  if ($pdf_contents) {
		  $file = file_save_data(
		    $pdf_contents, 
		    'private://grievance_attachments/' . $pdf_basename . '.pdf', 
		    FILE_EXISTS_RENAME
		  );
		  $file->display = 1;
		  $log_node->field_grievance_attachments["und"][0] = (array) $file;
		}

	  $log_node = node_submit($log_node); // Prepare node for saving
	  node_save($log_node);
	}

	// If format is PDF, just output it and exit
	if ($format == 'pdf') {
		// Dump to browser and exit
		header('Content-Type: application/pdf');
		header("Content-Disposition: attachment; filename=$pdf_basename.pdf");
		print $pdf_contents;

		// Can't call sirius_wizard_submit() because that redirects
		$data['steps'][$current_step]['complete'] = TRUE;
		$letter_node->field_sirius_type['und'][0]['value'] = 'sent';
		$data['sent_ts'] = time();
		sirius_letter_set_title($letter_node, 'sent', $data['sent_ts']);
		sirius_wizard_data_set($info, $letter_node, $data);
	}

	// If format is "fax", send a fax
	if ($format == 'fax') {
		$fax_number = $data['recipients'][0]['phone'];

		require_once DRUPAL_ROOT . '/sites/all/libraries/phaxio/lib/Phaxio/PhaxioException.php';
		require_once DRUPAL_ROOT . '/sites/all/libraries/phaxio/lib/Phaxio/PhaxioOperationResult.php';
		require_once DRUPAL_ROOT . '/sites/all/libraries/phaxio/lib/Phaxio/Phaxio.php';
		// use Phaxio\Phaxio;
		$phaxio = new \Phaxio\Phaxio(
			sirius_domain_variable_get('grievance_phaxio_api_key', ''), 
			sirius_domain_variable_get('grievance_phaxio_api_secret', ''), 
			sirius_domain_variable_get('grievance_phaxio_api_host', 'https://api.phaxio.com/v1/')
		);

		$path = '/tmp/' . $pdf_basename . '.pdf';
		file_put_contents($path, $pdf_contents);
		$callback_url = 'https://';
		$callback_url .= $_SERVER['SERVER_NAME'];
		$callback_url .= '/grievances/phaxio/callback-sent';
		$callback_url .= '/' . variable_get('grievance_phaxio_callback_token', '');
		$fax_result = $phaxio->sendFax(
			$fax_number, 
			array($path), 
			array('callback_url' => $callback_url)
		);
		unlink($path);

		if (!$fax_result->succeeded()) {
			drupal_set_message("Fax failed: " . $fax_result->getMessage(), 'error');
		} else {
			$fax_id = $fax_result->getData()['faxId'];
			drupal_set_message("Fax sent to " . $fax_number);
		}

		if ($log_node) {
			$log_node->field_grievance_external_id['und'][0]['value'] = 'phaxio_id_sent_' . $fax_id;
			node_save($log_node);
		}
	}

	// If format is "email", send
	if ($format == 'email') {
		$params = array();
		$params['handler_nids'] = array();
		$params['handler_nids'][] = $letter_node->nid;
		if ($log_node) { $params['handler_nids'][] = $log_node->nid; }
		$params['body'] = $data['body'];
		$params['subject'] =  $data['subject'];

		foreach ($data['recipients'] as $recipient) {
			$rfc822 = sirius_email_rfc822_format($recipient['name'], $recipient['email']);
			if (!$rfc822) { continue; }

			$disposition = $recipient['disposition'];
			if ($rfc822) {
				if ($params[$disposition]) { $params[$disposition] .= ','; }
				$params[$disposition] .= $rfc822;
			}
		}
		$params['headers']['bcc'] = $params['bcc'];
		$params['headers']['cc'] = $params['cc'];

		$params['attachments'] = array();
		if ($pdf_contents) {
			// Attach the grievance as a PDF
			$params['attachments'][] = array(
				'filecontent' => $pdf_contents,
				'filename' => $entity_node->type . '_' . $entity_node->nid . '_' . time() . '.pdf',
				'filemime' => 'application/pdf',
			);
		}

		if ($letter_node->field_sirius_attachments) {
			foreach ($letter_node->field_sirius_attachments['und'] as $delta => $record) {
				$uri = $record['uri'];
				$ext = pathinfo($uri, PATHINFO_EXTENSION);
				$tmp_uri = 'temporary://letter_' . rand() . ".$ext";
				file_unmanaged_copy($uri, $tmp_uri);
				$tmp_path = drupal_realpath($tmp_uri);
			  $fhandle = fopen($tmp_path, 'r');
  			$upload_contents = fread($fhandle, filesize($tmp_path));
			  fclose($fhandle);
			  file_unmanaged_delete($tmp_uri);
				$params['attachments'][] = array(
					'filecontent' => $upload_contents,
					'filename' => $entity_node->type . '_' . $entity_node->nid . '_' . $delta . '_' . time() . ".$ext",
					'filemime' => $record['filemime'],
				);
			}
		}

		if ($data['upload_others']) {
			foreach ($data['upload_others'] as $other_nid) {
				$other_node = node_load($other_nid);
				$record = $other_node->field_grievance_attachments['und'][0];
				if ($record) {
					$uri = $record['uri'];
					$ext = pathinfo($uri, PATHINFO_EXTENSION);
					$tmp_uri = 'temporary://letter_' . rand() . ".$ext";
					file_unmanaged_copy($uri, $tmp_uri);
					$tmp_path = drupal_realpath($tmp_uri);
				  $fhandle = fopen($tmp_path, 'r');
	  			$upload_contents = fread($fhandle, filesize($tmp_path));
				  fclose($fhandle);
				  file_unmanaged_delete($tmp_uri);
					$params['attachments'][] = array(
						'filecontent' => $upload_contents,
						'filename' => $entity_node->type . '_' . $entity_node->nid . '_' . $other_nid . '_' . time() . '.pdf',
						'filemime' => $record['filemime'],
					);
				}
			}
		}

		$result = sirius_email_send($params);
		if (!$result['success']) {
			drupal_set_message("Email delivery failed: " . $result['msg'] . ': ' . $result['details'], 'warning');
			$failure_count++;
			$status = 'error';
		} else {
			drupal_set_message("Email delivery successful: " . $result['msg']);
			$status = 'sent';
		}
	}

	// If format is "sms", send
	if ($format == 'sms') {
		$params = array();
		$params['sms'] = $data['body'];

		$failure_count = 0;
		foreach ($data['recipients'] as $recipient) {
			if ($recipient['disposition'] != 'send') { continue; }
			$params['to'] = $recipient['phone'];

			$result = sirius_sms_send($params);
			if (!$result['success']) {
				drupal_set_message("SMS delivery to $recipient[phone] failed: " . $result['msg'] . ': ' . $result['details'], 'warning');
				$failure_count++;
			} else {
				drupal_set_message("SMS delivery to $recipient[phone] successful: " . $result['msg']);
			}
		}

		if ($failure_count) { 
			drupal_set_message("Delivery failed to $failure_count recipients.", 'warning');
			$status = 'error';
		} else {
			$status = 'sent';
		}
	}

	$letter_node->field_sirius_type['und'][0]['value'] = $status;
	$data['sent_ts'] = time();

	if ($format == 'pdf') { drupal_exit(); }

	sirius_letter_set_title($letter_node, $status, $data['sent_ts']);
	sirius_wizard_submit($info, $data, $letter_node, $current_step, "Delivery complete.");
}

/************************************************************************************************
 * Step 4: Review
 ************************************************************************************************/

function sirius_letter_wizard_review($info, $data, $letter_node, $current_step) { 
	$info = sirius_wizard_info($wizard_name);

	if ($data['subject']) { $html = sirius_fakerender_field("Subject:", $data['subject']); }

	if ($data['recipients']) {
		foreach ($data['recipients'] as $recipient) {
			if ($recipient['disposition'] == 'skip') { continue; }
			if ($data['format'] == 'email') {
				$html .= sirius_fakerender_field($recipient['disposition'] . ':', htmlspecialchars(sirius_email_rfc822_format($recipient['name'], $recipient['email'])));
			} else if ($data['format'] == 'sms') {
				$html .= sirius_fakerender_field($recipient['disposition'] . ':', $recipient['name'] . ' [' . $recipient['phone'] . ']');
			}
		}
	}

	$renderable = field_view_field('node', $letter_node, 'field_sirius_attachments', 'full');
	$renderable['#title'] = 'Attachment';
	$html .= drupal_render($renderable);

	if ($data['upload_others']) {
		foreach ($data['upload_others'] as $other_nid) {
			$other_node = node_load($other_nid);
			$html .= sirius_fakerender_field("Attachment:", l($other_node->title, 'node/' . $other_node->nid));
		}
	}

	if (strlen($data['body']) > 20) {
		$html .= '<hr><h2>Body</h2><div style="padding: 20px; margin: 20px; border: 1px solid #cccccc;">';
		$html .= $data['body'];
		$html .= '</div>';
	}

	if (strlen($data['pdf_body']) > 20) {
		$html .= '<hr><h2>PDF Body</h2><div style="padding: 20px; margin: 20px; border: 1px solid #cccccc;">';
		$html .= $data['pdf_body'];
		$html .= '</div>';
	}

	return array('markup' => array('#markup' => $html));
}
