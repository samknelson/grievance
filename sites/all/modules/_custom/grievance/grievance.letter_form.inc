<?php

function grievance_letter_form($form, &$form_state, $grievance_node) {
	$form_state['grievance_node'] = $grievance_node;
	$status_tid = $grievance_node->field_grievance_status['und'][0]['tid'];
	$status_term = taxonomy_term_load($status_tid);
	$shop_nid = $grievance_node->field_grievance_shop['und'][0]['target_id'];

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Letter to send
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  $form['letter'] = array(
  	'#type' => 'fieldset',
  	'#title' => t('What letter do you want to send?'),
  );

	// Get the possible letters (as checkboxes)
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_letter_template');
  $query->propertyOrderBy('title');
  $stmt = $query->execute();
  $options = array();
  $footnote = '';
  if ($stmt['node']) {
  	$letter_nodes = node_load_multiple(array_keys($stmt['node']));
  	foreach ($letter_nodes as $letter_nid => $letter_node) {
  		// Check permissions on the letter
			if (grievance_letter_permission_check($letter_node)) {
	  		$options[$letter_nid] = $letter_node->title;
	  		if ($letter_node->field_grievance_attachments['und'][0]['uri']) {
	  			$options[$letter_nid] .= '*';
	  			$footnote = t('* These letters can only be downloaded individually.  They cannot be sent, logged, or bundled with other letters.');
	  		}
	  	}
  	}
  }

  $form['letter']['letter_nid'] = array(
    '#type' => 'checkboxes',
    '#options' => $options,
    '#title' => t('Select Letter(s)'),
    '#required' => TRUE,
    '#description' => $footnote,
  );

  // Get the delivery options, as a substitution variable
	$delivery_vocabulary = taxonomy_vocabulary_machine_name_load('grievance_delivery');
	$delivery_terms = taxonomy_get_tree($delivery_vocabulary->vid);
	$delivery_options = array(NULL => t('-- select --'));
	foreach ($delivery_terms as $term) {
		$delivery_options[$term->tid] = str_repeat('-', $term->depth) . $term->name;
	}
   $form['letter']['delivery_tid'] = array(
    '#type' => 'select',
    '#options' => $delivery_options,
    '#title' => t('Delivery'),
  );

  // Letterhead
  $form['letter']['letterhead'] = array(
    '#type' => 'select',
    '#options' => array(
    	'no' => t('No Letterhead'),
    	'yes' => t('Standard Letterhead'),
    ),
    '#title' => t('Letterhead'),
  );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Company Recipients
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  /*
  $form['company'] = array(
  	'#type' => 'fieldset',
  	'#collapsible' => TRUE,
  	'#collapsed' => TRUE,
  	'#title' => t('Who at the <strong>COMPANY</strong> should receive the letter?'),
  	'#description' => t('Select <strong>at most one</strong> company contact as the primary recipient of the notification.  This contact will be entered into the grievance form.  Arbitrarily many company contacts may also be selected as secondary recipients.  These contacts will be CC\'d on the notification.'),
	);

	$recipients = grievance_notify_company_contacts($shop_nid, $status_tid);

	// Additional contacts
  if (!$form_state['num_addl_company_recipients']) {
    $form_state['num_addl_company_recipients'] = 0;
  }
  for ($i=0; $i<$form_state['num_addl_company_recipients']; $i++) {
		$recipient = array();
		$recipient['name'] = t('Other Company Contact #') . ($i+1);
		$recipient['is_blank'] = TRUE;

		$recipients[] = $recipient;
	}

	$form['company']['grievance_recipients_company']['grievance_company_contacts_wrapper_open'] = array(
		'#title' => '',
		'#type' => 'markup',
		'#markup' => '',
		'#prefix' => '<div class="grievance-recipients" id="grievance-recipients-company">'
	);

  $contact_count_company = 0;
  $contact_count_shop = 0;
  $contact_count_statusmatch = 0;
  $contact_count_nostatusmatch = 0;
  foreach ($recipients as $contact) {
  	if ($contact['is_blank']) { continue; }
    if ($contact['statusmatch']) { ++$contact_count_statusmatch; } else { ++$contact_count_nostatusmatch; }
    if ($contact['shop_contact']) { ++$contact_count_shop; }
    if ($contact['company_contact']) { ++$contact_count_company; }
  }
  $contacts_markup = '';
  $contacts_markup .= '<div style="clear: both" id="grievance_contact_controls">';

  if ($contact_count_shop) {
    $contacts_markup .= '<a href="#" id="grievance-contact-hide-shop">[' . t('Hide shop contacts') . ']</a> ';
    $contacts_markup .= '<a href="#" id="grievance-contact-show-shop" style="display: none;">[' . t('Show shop contacts') . ']</a> ';
  }

  if ($contact_count_company) {
    $contacts_markup .= '<a href="#" id="grievance-contact-hide-company">[' . t('Hide company contacts') . ']</a> ';
    $contacts_markup .= '<a href="#" id="grievance-contact-show-company" style="display: none;">[' . t('Show company contacts') . ']</a> ';
  }

  $grievance_contacts_hide_nostatusmatch = variable_get('grievance_contacts_hide_nostatusmatch', '');
  if ($grievance_contacts_hide_nostatusmatch == "Yes") {
    if ($contact_count_nostatusmatch) {
      $contacts_markup .= '<a href="#" id="grievance-contact-hide-nostatusmatch" style="display: none;">[' . t('Hide non-matching statuses') . ']</a> ';
      $contacts_markup .= '<a href="#" id="grievance-contact-show-nostatusmatch">[' . t('Show non-matching statuses') . ']</a> ';
    }
  } else {
    if ($contact_count_statusmatch) {
      $contacts_markup .= '<a href="#" id="grievance-contact-hide-nostatusmatch">[' . t('Hide non-matching statuses') . ']</a> ';
      $contacts_markup .= '<a href="#" id="grievance-contact-show-nostatusmatch" style="display: none;">[' . t('Show non-matching statuses') . ']</a> ';
    }
  }
  $contacts_markup .= '</div>';

	$form['company']['grievance_recipients_company']['grievance_company_contacts_wrapper_intro'] = array(
		'#title' => '',
		'#type' => 'markup',
		'#markup' => $contacts_markup,
	);

	$count = 0;
	foreach ($recipients as $recipient) {
		++$count;

		if ($recipient['is_blank']) {
			$form['company']['grievance_recipients_company']["company_recipient_${count}_intro"] = array(
				'#title' => '',
				'#type' => 'markup',
				'#markup' => '<div class="grievance-recipient-name">' . $recipient['name'] . '</div>',
				'#prefix' => "<div class=\"grievance-recipient grievance-recipient-$default_value\">",
			);

			$form['company']['grievance_recipients_company']["company_recipient_${count}_name"] = array(
				'#title' => 'Name',
				'#type' => 'textfield',
			);

			$form['company']['grievance_recipients_company']["company_recipient_${count}_email"] = array(
				'#title' => 'Email',
				'#type' => 'textfield',
			);

		} else {

      $classes = 'grievance-recipient';
      if ($recipient['statusmatch']) {
        $classes .= ' grievance-recipient-statusmatch';
      } else {
        $classes .= ' grievance-recipient-nostatusmatch';
      }

      if ($recipient['shop_contact']) {
        $classes .= ' grievance-recipient-shop';
      }
      if ($recipient['company_contact']) {
        $classes .= ' grievance-recipient-company';
      }

      $style = '';
      if ($grievance_contacts_hide_nostatusmatch == "Yes" && !$recipient['statusmatch']) {
        $style .= 'display: none;';
      }


			$form['company']['grievance_recipients_company']["company_recipient_${count}_intro"] = array(
				'#title' => '',
				'#type' => 'markup',
				'#markup' => '<div class="grievance-recipient-name">' . $recipient['name'] . '</div>' . 
					'<div class="grievance-recipient-email">' . grievance_email_trim($recipient['email'], 25) . '</div>' . 
					'<div class="grievance-recipient-type">(' . $recipient['type'] . ')</div>',
				'#prefix' => '<div class="' . $classes . '" style="' . $style . '">',
			);

			$form['company']['grievance_recipients_company']["company_recipient_${count}_name"] = array(
				'#type' => 'hidden',
				'#value' => $recipient['name'],
			);

			$form['company']['grievance_recipients_company']["company_recipient_${count}_email"] = array(
				'#type' => 'hidden',
				'#value' => $recipient['email'],
			);
		}

		if ($recipient['nid']) {
			$form['company']['grievance_recipients_company']["company_recipient_${count}_nid"] = array(
				'#type' => 'hidden',
				'#value' => $recipient['nid'],
			);
		}

		$form['company']['grievance_recipients_company']["company_recipient_${count}_disposition"] = array(
			'#title' => t('This contact should be:'),
			'#type' => 'radios',
			'#options' => array(
				'none' => t('Skipped'),
				'primary' => t('Main recipient (TO)'),
				'secondary' => t('Copied (CC)')
			),
			'#suffix' => '</div>',
			'#default_value' => 'none',
		);
	}

  $form['company']['grievance_recipients_company']['addl_company_recipient'] = array(
    '#type' => 'submit',
    '#value' => t('Add Company Recipient'),
    '#submit' => array('grievance_letter_form_add_one_company'),
    '#ajax' => array(
      'callback' => 'grievance_letter_form_add_one_company_callback',
      'wrapper' => 'grievance-recipients-company',
    ),
		'#prefix' => '<div class="grievance-recipient">' . 
			'<div class="grievance-recipient-name">' . t('Other Company Contact #') . ($i+1) . '</div><br><br>',
		'#suffix' => '</div>',
	);
	
	$form['company']['grievance_recipients_company']['grievance_company_contacts_wrapper_conclusion'] = array(
		'#title' => '',
		'#type' => 'markup',
		'#markup' => '',
		'#prefix' => '</div>'
	);
	*/

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Union Recipients
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*
  $form['union'] = array(
  	'#type' => 'fieldset',
  	'#title' => t('Who at the <strong>UNION</strong> should receive the letter?'),
  	'#collapsible' => TRUE,
  	'#collapsed' => TRUE,
  	'#description' => t(''),
	);

	$union_recipients = array();

	$fields = array(
		'field_grievance_rep_filed',
		'field_grievance_rep_organizer',
		'field_grievance_rep_lead',
		'field_grievance_rep_manager',
		'field_grievance_rep_watching',
		'field_grievance_rep_assignee',
	);

	$nodupes = array();
	foreach ($fields as $field) {
		if (isset($grievance_node->{$field}['und'])) {
			$info = field_info_instance('node', $field, 'grievance');
			$label = $info['label'];

			foreach ($grievance_node->{$field}['und'] as $tmp) {
				$uid = $tmp['target_id'];
				if (!$uid) { continue; }
				$user = user_load($uid);
				if (!$user) { continue; }

				if ($nodupes[$user->mail]) { continue; }
				$nodupes[$user->mail] = TRUE;


				$recipient = array();
				$recipient['type'] = $label;
				$recipient['uid'] = $uid;
				$recipient['name'] = $user->name;
				$recipient['email'] = $user->mail;
				if ($field == 'field_grievance_rep_watching') {
					$recipient['secret'] = TRUE;
				}

				$union_recipients[] = $recipient;
			}
		}
	}

	global $user;
	if (!$nodupes[$user->mail]) {
		$nodupes[$user->mail] = TRUE;
		$recipient = array();
		$recipient['type'] = 'Me (current user)';
		$recipient['uid'] = $user->uid;
		$recipient['name'] = $user->name;
		$recipient['email'] = $user->mail;
		$union_recipients[] = $recipient;
	}

	// Additional contacts
  if (!$form_state['num_addl_union_recipients']) {
    $form_state['num_addl_union_recipients'] = 0;
  }
	for ($i=0; $i<$form_state['num_addl_union_recipients']; ++$i) {
		$recipient = array();
		$recipient['name'] = t('Other Union Contact #') . ($i+1);
		$recipient['is_blank'] = TRUE;
		$union_recipients[] = $recipient;
	}

	$form['union']['grievance_recipients_union']['grievance_union_contacts_wrapper_open'] = array(
		'#title' => '',
		'#type' => 'markup',
		'#markup' => '',
		'#prefix' => '<div class="grievance-recipients" id="grievance-recipients-union">'
	);

	$count = 0;
	foreach ($union_recipients as $recipient) {
		++$count;

		if ($recipient['email'] == $user->mail) {
			$default_value = 'primary';
		} else if ($recipient['secret']) {
			$default_value = 'secret';
		} else if ($recipient['email']) {
			$default_value = 'secondary';
		} else {
			$default_value = 'none';
		}

		if ($recipient['is_blank']) {
			$form['union']['grievance_recipients_union']["union_recipient_${count}_intro"] = array(
				'#title' => '',
				'#type' => 'markup',
				'#markup' => '<div class="grievance-recipient-name">' . $recipient['name'] . '</div>',
				'#prefix' => '<div class="grievance-recipient">',
			);

			$form['union']['grievance_recipients_union']["union_recipient_${count}_name"] = array(
				'#title' => 'Name',
				'#type' => 'textfield',
			);

			$form['union']['grievance_recipients_union']["union_recipient_${count}_email"] = array(
				'#title' => 'Email',
				'#type' => 'textfield',
			);

		} else {

			$form['union']['grievance_recipients_union']["union_recipient_${count}_intro"] = array(
				'#title' => '',
				'#type' => 'markup',
				'#markup' => '<div class="grievance-recipient-name">' . $recipient['name'] . '</div>' . 
					'<div class="grievance-recipient-email">' . grievance_email_trim($recipient['email'], 25) . '</div>' . 
					'<div class="grievance-recipient-type">(' . $recipient['type'] . ')</div>',
				'#prefix' => "<div class=\"grievance-recipient grievance-recipient-$default_value\">",
			);

			$form['union']['grievance_recipients_union']["union_recipient_${count}_name"] = array(
				'#type' => 'hidden',
				'#value' => $recipient['name'],
			);

			$form['union']['grievance_recipients_union']["union_recipient_${count}_email"] = array(
				'#type' => 'hidden',
				'#value' => $recipient['email'],
			);
		}

		$form['union']['grievance_recipients_union']["union_recipient_${count}_disposition"] = array(
			'#title' => t('This contact should be:'),
			'#type' => 'radios',
			'#options' => array(
				'none' => t('Skipped'),
				'primary' => t('Sender (FROM)'),
				'secondary' => t('Copied (CC)'),
				'secret' => t('Blind copied (BCC)'),
			),
			'#default_value' => $default_value,
			'#suffix' => '</div>'
		);
	}

  $form['union']['grievance_recipients_union']['addl_union_recipient'] = array(
    '#type' => 'submit',
    '#value' => t('Add Union Recipient'),
    '#submit' => array('grievance_letter_form_add_one_union'),
    '#ajax' => array(
      'callback' => 'grievance_letter_form_add_one_union_callback',
      'wrapper' => 'grievance-recipients-union',
    ),
		'#prefix' => '<div class="grievance-recipient">' . 
			'<div class="grievance-recipient-name">' . t('Other Union Contact #') . ($i+1) . '</div><br><br>',
		'#suffix' => '</div>',
	);

	$form['union']['grievance_recipients_union']['grievance_union_contacts_wrapper_conclusion'] = array(
		'#title' => '',
		'#type' => 'markup',
		'#markup' => '',
		'#prefix' => '</div>'
	);
	*/

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Output and Logging
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  $form['dest'] = array(
  	'#type' => 'fieldset',
  	'#title' => t('Ready to send the letter?'),
  );

  $form['dest']['output'] = array(
    '#type' => 'select',
    '#options' => array(
    	'pdf' => t('Download the letter in PDF format'),
    	'preview' => t('Preview the letter on the screen'),
    	'edit' => t('Edit the letter before sending'),
    	/* 'send-email' => t('Send the letter by email'), */
    ),
    '#title' => t('Output'),
  );

  $log_types_vocabulary = taxonomy_vocabulary_machine_name_load('grievance_log_types');
  $log_type_options = array(NULL => '[select]');
  $tmp = taxonomy_get_tree($log_types_vocabulary->vid);
  $found = FALSE;
  foreach ($tmp as $term) {
    $log_type_options[$term->tid] = $term->name;
  }

  $form['dest']['logging'] = array(
    '#type' => 'select',
    '#options' => $log_type_options,
    '#title' => t('Save letter to the log as'),
  );

  $form['dest']['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Go',
  );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// On-Screen Preview
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // Edit
  if ($form_state['edit_result']) {
		$form['letter_body'] = array(
			'#title' => 'Edit the Letter',
			'#type' => 'text_format',
			'#default_value' => $form_state['edit_result'],
			'#format' => 'full_html',
		);
  }
	
	// Screen Preview
  if ($form_state['result']) {
  	$markup = '<h2>' . t('Preview') . '</h2>';
  	$markup .= '<hr>';
  	$markup .= $form_state['result'];
  	$markup .= '<hr>';
  	$form['markup'] = array(
  		'#type' => 'markup',
  		'#markup' => $markup,
  	);
  }


	return $form;
}

function grievance_letter_form_add_one_company_callback($form, $form_state) {
  return $form['company']['grievance_recipients_company'];
}

function grievance_letter_form_add_one_company($form, &$form_state) {
  $form_state['num_addl_company_recipients']++;
  $form_state['rebuild'] = TRUE;
}

function grievance_letter_form_add_one_union($form, &$form_state) {
  $form_state['num_addl_union_recipients']++;
  $form_state['rebuild'] = TRUE;
}

function grievance_letter_form_add_one_union_callback($form, $form_state) {
  return $form['union']['grievance_recipients_union'];
}
