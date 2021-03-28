<?php

function grievance_node_tab_notification_base($grievance_node) {
	drupal_goto('node/' . $grievance_node->nid . '/notification/notification');
}

function grievance_node_tab_notification($grievance_node) {
  drupal_set_title($grievance_node->title . ': ' . t('Notification'));

	return drupal_get_form('grievance_notify_form', $grievance_node);
}

function grievance_notify_company_contacts($params = array()) {
	$shop_nid = $params['shop_nid'];
	$status_nid = $params['status_nid'];
	$include_contact_type_tid = $params['include_contact_type_tid'];
	$exclude_contact_type_tid = $params['exclude_contact_type_tid'];

	$contact_type_lookup = sirius_taxonomy_load('grievance_contact_types', 'tid', 'name');

	// Find shop contacts associated with the shop
	$recipients = array();
	$shop_node = node_load($shop_nid);

	if ($shop_nid) {
		$query = new EntityFieldQuery();
		$query->entityCondition('entity_type', 'node');
	  $query->entityCondition('bundle', 'grievance_shop_contact');
	  $query->fieldCondition('field_grievance_shops', 'target_id', $shop_nid);
	  if ($include_contact_type_tid) {
	  	$query->fieldCondition('field_grievance_contact_types', 'tid', $include_contact_type_tid);
	  }
		$stmt = $query->execute();
		if ($stmt['node']) {
			$contact_nids = array_keys($stmt['node']);
			$contact_nodes = node_load_multiple($contact_nids);
			foreach ($contact_nodes as $contact_nid => $contact_node) {
				$contact_type_tids = sirius_fieldvals($contact_node, 'field_grievance_contact_types', 'tid');
				if ($exclude_contact_type_tid && in_array($exclude_contact_type_tid, $contact_type_tids)) { continue; }

				$recipient = array();

				$recipient['contact_type_names'] = array();
				foreach ($contact_type_tids as $contact_type_tid) {
					$recipient['contact_type_names'][] = $contact_type_lookup[$contact_type_tid];
				}

				$role = $contact_node->field_grievance_co_role['und'][0]['value'];
				if ($role) {
					$recipient['type'] = $role . t(' for ') . $shop_node->title;
				} else {
					$recipient['type'] = t('Shop contact for: ') . $shop_node->title;
				}

				$recipient['nid'] = $contact_node->nid;
				$recipient['name'] = $contact_node->field_grievance_co_name['und'][0]['value'];
				$recipient['email'] = $contact_node->field_grievance_co_email['und'][0]['email'];
				$recipient['phone'] = $contact_node->field_grievance_co_phone['und'][0]['value'];
				$recipient['fax'] = $contact_node->field_grievance_co_fax['und'][0]['value'];
				$recipient['shop_contact'] = TRUE;
				$recipient['contact_types'] = $contact_node->field_grievance_contact_types['und'];
				$recipients[$contact_node->nid] = $recipient;
			}
		}
	}

	// Find contacts associated with the company
	$company_nid = $shop_node->field_grievance_company['und'][0]['target_id'];
	$company_node = node_load($company_nid);

	if ($company_nid) {
		$query = new EntityFieldQuery();
		$query->entityCondition('entity_type', 'node');
	  $query->entityCondition('bundle', 'grievance_shop_contact');
	  $query->fieldCondition('field_grievance_company', 'target_id', $company_nid);
	  if ($include_contact_type_tid) {
	  	$query->fieldCondition('field_grievance_contact_types', 'tid', $include_contact_type_tid);
	  }
		$stmt = $query->execute();
		if ($stmt['node']) {
			$contact_nids = array_keys($stmt['node']);
			$contact_nodes = node_load_multiple($contact_nids);
			foreach ($contact_nodes as $contact_nid => $contact_node) {

				if ($exclude_contact_type_tid) {
					$found = FALSE;
					if ($contact_node->field_grievance_contact_types) {
						foreach ($contact_node->field_grievance_contact_types['und'] as $tmp) {
							if ($tmp['tid'] == $exclude_contact_type_tid) {
								$found = TRUE;
							}
						}
					}
					if ($found) { continue; }
				}

				$recipient = array();

				$role = $contact_node->field_grievance_co_role['und'][0]['value'];
				if ($role) {
					$recipient['type'] = $role . t(' for ') . $company_node->title;
				} else {
					$recipient['type'] = t('Copmany contact for: ') . $company_node->title;
				}

				$recipient['nid'] = $contact_node->nid;
				$recipient['name'] = $contact_node->field_grievance_co_name['und'][0]['value'];
				$recipient['email'] = $contact_node->field_grievance_co_email['und'][0]['email'];
				$recipient['company_contact'] = TRUE;
				$recipient['phone'] = $contact_node->field_grievance_co_phone['und'][0]['value'];
				$recipient['fax'] = $contact_node->field_grievance_co_fax['und'][0]['value'];
				$recipient['contact_types'] = $contact_node->field_grievance_contact_types['und'];
				$recipients[$contact_node->nid] = $recipient;
			}
		}
	}

	return $recipients;
}

function grievance_notify_form($form, &$form_state, $grievance_node) {
	$form_state['grievance_node'] = $grievance_node;

	// 
	// Status term
	//

	$status_tid = $grievance_node->field_grievance_status['und'][0]['tid'];
	$status_term = taxonomy_term_load($status_tid);

	//
	// Company Recipients
	//

	$form['grievance_company_recipients_intro'] = array(
		'#title' => '',
		'#type' => 'markup',
		'#markup' => '<h2>' . t('Company Recipients') . '</h2>' . t('Select <strong>one</strong> company contact as the primary recipient of the notification.  This contact will be entered into the grievance form.  Arbitrarily many company contacts may also be selected as secondary recipients.  These contacts will be CC\'d on the notification, but will not be recorded in the grievance form.'),
		'#prefix' => '<div class="grievance-company-recipients-intro">',
		'#suffix' => '</div>'
	);

	$shop_nid = $grievance_node->field_grievance_shop['und'][0]['target_id'];
	$recipients = grievance_notify_company_contacts(array('shop_nid' => $shop_nid, 'status_nid' => $status_tid));

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

	$form['grievance_recipients_company']['grievance_company_contacts_wrapper_open'] = array(
		'#title' => '',
		'#type' => 'markup',
		'#markup' => '',
		'#prefix' => '<div class="grievance-recipients" id="grievance-recipients-company">'
	);

  $contact_count_company = 0;
  $contact_count_shop = 0;
  foreach ($recipients as $contact) {
  	if ($contact['is_blank']) { continue; }
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

  $contacts_markup .= '</div>';

	$form['grievance_recipients_company']['grievance_company_contacts_wrapper_intro'] = array(
		'#title' => '',
		'#type' => 'markup',
		'#markup' => $contacts_markup,
	);

	$count = 0;
	foreach ($recipients as $recipient) {
		++$count;

		if ($recipient['is_blank']) {
			$form['grievance_recipients_company']["company_recipient_${count}_intro"] = array(
				'#title' => '',
				'#type' => 'markup',
				'#markup' => '<div class="grievance-recipient-name">' . $recipient['name'] . '</div>',
				'#prefix' => "<div class=\"grievance-recipient grievance-recipient-$default_value\">",
			);

			$form['grievance_recipients_company']["company_recipient_${count}_name"] = array(
				'#title' => 'Name',
				'#type' => 'textfield',
			);

			$form['grievance_recipients_company']["company_recipient_${count}_email"] = array(
				'#title' => 'Email',
				'#type' => 'textfield',
			);

		} else {

      $classes = 'grievance-recipient';

      if ($recipient['shop_contact']) {
        $classes .= ' grievance-recipient-shop';
      }
      if ($recipient['company_contact']) {
        $classes .= ' grievance-recipient-company';
      }

      $style = '';

			$form['grievance_recipients_company']["company_recipient_${count}_intro"] = array(
				'#title' => '',
				'#type' => 'markup',
				'#markup' => '<div class="grievance-recipient-name">' . $recipient['name'] . '</div>' . 
					'<div class="grievance-recipient-email">' . grievance_email_trim($recipient['email'], 25) . '</div>' . 
					'<div class="grievance-recipient-type">(' . $recipient['type'] . ')</div>',
				'#prefix' => '<div class="' . $classes . '" style="' . $style . '">',
			);

			$form['grievance_recipients_company']["company_recipient_${count}_name"] = array(
				'#type' => 'hidden',
				'#value' => $recipient['name'],
			);

			$form['grievance_recipients_company']["company_recipient_${count}_email"] = array(
				'#type' => 'hidden',
				'#value' => $recipient['email'],
			);
		}

		if ($recipient['nid']) {
			$form['grievance_recipients_company']["company_recipient_${count}_nid"] = array(
				'#type' => 'hidden',
				'#value' => $recipient['nid'],
			);
		}

		$form['grievance_recipients_company']["company_recipient_${count}_disposition"] = array(
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

  $form['grievance_recipients_company']['addl_company_recipient'] = array(
    '#type' => 'submit',
    '#value' => t('Add Company Recipient'),
    '#submit' => array('grievance_notify_form_add_one_company'),
    '#ajax' => array(
      'callback' => 'grievance_notify_form_add_one_company_callback',
      'wrapper' => 'grievance-recipients-company',
    ),
		'#prefix' => '<div class="grievance-recipient">' . 
			'<div class="grievance-recipient-name">' . t('Other Company Contact #') . ($i+1) . '</div><br><br>',
		'#suffix' => '</div>',
	);
	
	$form['grievance_recipients_company']['grievance_company_contacts_wrapper_conclusion'] = array(
		'#title' => '',
		'#type' => 'markup',
		'#markup' => '',
		'#prefix' => '</div>'
	);



	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Union Recipients
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

	$form['grievance_union_recipients_intro'] = array(
		'#title' => '',
		'#type' => 'markup',
		'#markup' => '<h2>' . t('Union Recipients') . '</h2>' . t('The following union contacts may be CC\'d or BCC\'d.'),
		'#prefix' => '<div class="grievance-union-recipients-intro">',
		'#suffix' => '</div>'
	);

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

	$form['grievance_recipients_union']['grievance_union_contacts_wrapper_open'] = array(
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
			$form['grievance_recipients_union']["union_recipient_${count}_intro"] = array(
				'#title' => '',
				'#type' => 'markup',
				'#markup' => '<div class="grievance-recipient-name">' . $recipient['name'] . '</div>',
				'#prefix' => '<div class="grievance-recipient">',
			);

			$form['grievance_recipients_union']["union_recipient_${count}_name"] = array(
				'#title' => 'Name',
				'#type' => 'textfield',
			);

			$form['grievance_recipients_union']["union_recipient_${count}_email"] = array(
				'#title' => 'Email',
				'#type' => 'textfield',
			);

		} else {

			$form['grievance_recipients_union']["union_recipient_${count}_intro"] = array(
				'#title' => '',
				'#type' => 'markup',
				'#markup' => '<div class="grievance-recipient-name">' . $recipient['name'] . '</div>' . 
					'<div class="grievance-recipient-email">' . grievance_email_trim($recipient['email'], 25) . '</div>' . 
					'<div class="grievance-recipient-type">(' . $recipient['type'] . ')</div>',
				'#prefix' => "<div class=\"grievance-recipient grievance-recipient-$default_value\">",
			);

			$form['grievance_recipients_union']["union_recipient_${count}_name"] = array(
				'#type' => 'hidden',
				'#value' => $recipient['name'],
			);

			$form['grievance_recipients_union']["union_recipient_${count}_email"] = array(
				'#type' => 'hidden',
				'#value' => $recipient['email'],
			);
		}

		$form['grievance_recipients_union']["union_recipient_${count}_disposition"] = array(
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

  $form['grievance_recipients_union']['addl_union_recipient'] = array(
    '#type' => 'submit',
    '#value' => t('Add Union Recipient'),
    '#submit' => array('grievance_notify_form_add_one_union'),
    '#ajax' => array(
      'callback' => 'grievance_notify_form_add_one_union_callback',
      'wrapper' => 'grievance-recipients-union',
    ),
		'#prefix' => '<div class="grievance-recipient">' . 
			'<div class="grievance-recipient-name">' . t('Other Union Contact #') . ($i+1) . '</div><br><br>',
		'#suffix' => '</div>',
	);

	$form['grievance_recipients_union']['grievance_union_contacts_wrapper_conclusion'] = array(
		'#title' => '',
		'#type' => 'markup',
		'#markup' => '',
		'#prefix' => '</div>'
	);

	$form['grievance_message_intro'] = array(
		'#title' => '',
		'#type' => 'markup',
		'#markup' => '<h2>' . t('Message') . '</h2>' . t('Enter the message that the contacts selected above should receive.'),
		'#prefix' => '<div class="grievance-message-intro">',
		'#suffix' => '</div>',
	);

	$hr = grievance_notify_template($grievance_node);
	$default_subject = $hr['subject'];
	$default_body = $hr['body'];

  $form['subject'] = array(
    '#title' => t('Subject'),
    '#type' => 'textfield',
    '#required' => 1,
    '#default_value' => $default_subject,
   );

  $form['body'] = array(
    '#title' => t('Body'),
    '#type' => 'textarea',
		'#rows' => 20,
    '#required' => 1,
    '#default_value' => $default_body,
	);

  // Update the status

	$form['grievance_status_intro'] = array(
		'#title' => '',
		'#type' => 'markup',
		'#markup' => '<h2>' . t('Change Status') . '</h2>' . t('After sending the notification, change the grievance status.'),
	);

  $form['grievance_new_status'] = array(
    '#type' => 'select',
    '#options' => taxonomy_allowed_values(field_info_field('field_grievance_status')),
    '#title' => t('Change Status To'),
    '#default_value' => $grievance_node->field_grievance_status['und'][0]['tid'],
   );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Send and Log Grievance Notification',
    '#prefix' => '<div class="field-grievance-submit-wrapper">',
    '#suffix' => '</div>',
  );

	return $form;
}

function grievance_notify_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;

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

	// Next, send the notification.
	$to = $company_recipients['primary'][0]['name'] . ' <' . $company_recipients['primary'][0]['email'] . '>';
	$from = $union_recipients['primary'][0]['name'] . ' <' . $union_recipients['primary'][0]['email'] . '>';

	$cc = '';
	if ($company_recipients['secondary']) {
		foreach ($company_recipients['secondary'] as $recipient) {
			if ($cc) { $cc .= ', '; }
			$cc .= $recipient['name'] . ' <' . $recipient['email'] . '>';
		}
	}
	if ($union_recipients['secondary']) {
		foreach ($union_recipients['secondary'] as $recipient) {
			if ($cc) { $cc .= ', '; }
			$cc .= $recipient['name'] . ' <' . $recipient['email'] . '>';
		}
	}

	$bcc = '';
	if ($union_recipients['secret']) {
		foreach ($union_recipients['secret'] as $recipient) {
			if ($bcc) { $bcc .= ', '; }
			$bcc .= $recipient['name'] . ' <' . $recipient['email'] . '>';
		}
	}

	$params = array();
	$params['body'] = $form_state['values']['body'];
	$params['subject'] =  $form_state['values']['subject'];
	if ($cc) {
		$params['headers']['Cc'] = $cc;
	}
	if ($bcc) {
		$params['headers']['Bcc'] = $bcc;
	}
	$params['from'] = $from;
	$params['attachments'] = array();

	// Attach the grievance as a PDF
	$params['attachments'][] = array(
		'filecontent' => grievance_gform_pdf_contents($grievance_node),
		'filename' => 'grievance_' . $node->nid . '_' . time() . '.pdf',
		'filemime' => 'grievance/pdf',
	);

	drupal_mail(
		'grievance', 
		'notify', 
		$to, 
		LANGUAGE_NONE, 
		$params
	);

	grievance_log_notify(
		$grievance_node,
		$from, 
		$to,
		$cc,
		$params['subject'],
		$params['body'],
		$params['attachments']
	);

	// Now update the node, if the status has changed.
	$new_status_tid = $form_state['values']['grievance_new_status'];
	if ($new_status_tid != $grievance_node->field_grievance_status['und'][0]['tid']) {
		$grievance_node->field_grievance_status['und'][0]['tid'] = $new_status_tid;
		$grievance_node->field_grievance_status_date['und'][0]['value'] = date('Y-m-d');
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

	$form_state['rebuild'] = FALSE;
	$form_state['redirect'] = 'node/' . $grievance_node->nid . '/gform';
}

function grievance_notify_form_add_one_company($form, &$form_state) {
  $form_state['num_addl_company_recipients']++;
  $form_state['rebuild'] = TRUE;
}

function grievance_notify_form_add_one_company_callback($form, $form_state) {
  return $form['grievance_recipients_company'];
}

function grievance_notify_form_add_one_union($form, &$form_state) {
  $form_state['num_addl_union_recipients']++;
  $form_state['rebuild'] = TRUE;
}

function grievance_notify_form_add_one_union_callback($form, $form_state) {
  return $form['grievance_recipients_union'];
}

function grievance_email_trim($email, $length) {
	if (strlen($email) < $length) { return $email; }
	return substr($email, 0, $length-3) . '&hellip;';
}

function grievance_notify_template($grievance_node) {
	$result = array();

	$status_tid = $grievance_node->field_grievance_status['und'][0]['tid'];
	if (!$status_tid) { return NULL; }

	$status_term = taxonomy_term_load($status_tid);
	if (!$status_term) { return NULL; }

	$contract_template_nid = $grievance_node->field_grievance_contract_tplt['und'][0]['target_id'];
	if ($contract_template_nid) {
		$query = new EntityFieldQuery();
		$query->entityCondition('entity_type', 'node');
	  $query->entityCondition('bundle', 'grievance_contract_rule');
	  $query->fieldCondition('field_grievance_contract_tplt', 'target_id', $contract_template_nid);
	  $query->fieldCondition('field_grievance_status', 'tid', $status_tid);
		$stmt = $query->execute();
		if ($stmt['node']) {
			$rule_nids = array_keys($stmt['node']);
			$rule_nodes = node_load_multiple($rule_nids);
			foreach ($rule_nodes as $rule_node) {
				if ($rule_node->field_grievance_notify_body['und'][0]['value']) {
					$result['subject'] = token_replace(
						$rule_node->field_grievance_notify_subject['und'][0]['value'], 
						array('node' => $grievance_node), 
						array()
					);


					$result['body'] = token_replace(
						$rule_node->field_grievance_notify_body['und'][0]['value'], 
						array('node' => $grievance_node), 
						array()
					);
				}
			}
		}
	}

	if (!$result['body']) {
		$result['subject'] = $status_term->field_grievance_notify_subject['und'][0]['value'];
		$result['body'] = $status_term->field_grievance_notify_body['und'][0]['value'];
	}

	return $result;
}
