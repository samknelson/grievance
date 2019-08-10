<?php

function grievance_letter_form_change_letter_callback($form, $form_state) {
  $commands = array();

  $commands[] = ajax_command_replace("#grievance-letter-form-replace-global-cc", render($form['recipients']['extra']['global_cc']));
  $commands[] = ajax_command_replace("#grievance-letter-form-replace-subject", render($form['recipients']['extra']['subject']));
  $commands[] = ajax_command_replace("#grievance-letter-form-replace-body", render($form['recipients']['extra']['body']));
  $commands[] = ajax_command_replace("#grievance-letter-form-replace-update-rep", render($form['recipients']['extra']['update_company_representative']));
  $commands[] = ajax_command_replace("#grievance-letter-form-replace-logging", render($form['dest']['logging']));

  return array(
    '#type' => 'ajax',
    '#commands' => $commands
  );
}

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
  $options = array(NULL => '- Select -');
  $footnote = '';
  if ($stmt['node']) {
  	$letter_nodes = node_load_multiple(array_keys($stmt['node']));
  	foreach ($letter_nodes as $letter_nid => $letter_node) {
  		// Check permissions on the letter
			if (grievance_letter_permission_check($letter_node)) {
	  		$options[$letter_nid] = $letter_node->title;
	  		if ($letter_node->field_grievance_attachments['und'][0]['uri']) {
	  			$options[$letter_nid] .= '*';
	  			$footnote = t('* Letters marked with a star can only be downloaded individually.  They cannot be sent, logged, or bundled with other letters.');
	  		}
	  	}
  	}
  }

  for ($i=0; $i<5; ++$i) {
	  $form['letter']['letter_nid_' . $i] = array(
	    '#type' => 'select',
	    '#options' => $options,
	    '#title' => t('Select Letter #') . ($i+1),
	    '#ajax' => array(
	      'callback' => 'grievance_letter_form_change_letter_callback',
	      'method' => 'replace',
	    ),
	  );
	  if ($i == 0) {
			$form['letter']['letter_nid_' . $i]['#description'] = $footnote;
	    $form['letter']['letter_nid_' . $i]['#required'] = TRUE;

	  }
	}

	if (user_access('grievance letter variable delivery')) {
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
	}

	if (user_access('grievance letter variable letterhead')) {
		// @todo: I'm supporting some crappy legacy code here.
		$options = array('no' => t('No Letterhead'));
		if (variable_get('grievance_gform_wkhtmltopdfargs', '')) {
			$options['yes'] = t('Standard Letterhead');
		}
		$letterheads = sirius_letterhead_list();
		foreach ($letterheads as $nid => $title) {
			$options[$nid] = $title;
		}
	  // Letterhead
	  $form['letter']['letterhead'] = array(
	    '#type' => 'select',
	    '#options' => $options,
	    '#title' => t('Letterhead'),
	  );
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Recipients
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  $form['recipients'] = array(
  	'#type' => 'fieldset',
  	'#collapsible' => TRUE,
  	'#collapsed' => TRUE,
  	'#title' => t('Email Recipients'),
  	'#description' => t('Select the email recipients for this letter.'),
	);

	$global_ccs = array();
	$default_subject = '';
	$default_body = '';
	$default_update_rep = '';
	for ($i=0; $i<5; ++$i) {
		if ($form_state['values']['letter_nid_' . $i] > 0) {
			$letter_nid = $form_state['values']['letter_nid_' . $i];
			$letter_node = node_load($letter_nid);
			if ($letter_node->field_grievance_emails['und']) {
				foreach ($letter_node->field_grievance_emails['und'] as $tmp) {
					$global_ccs[$tmp['value']] = TRUE;
				}
			}
			if ($letter_node->field_grievance_notify_subject['und'][0]['value']) { 
				$default_subject = $letter_node->field_grievance_notify_subject['und'][0]['value'];
			}
			if ($letter_node->field_grievance_notify_body['und'][0]['value']) { 
				$default_body = $letter_node->field_grievance_notify_body['und'][0]['value'];
			}
			if ($letter_node->field_grievance_update_rep['und'][0]['value']) {
				$default_update_rep = $letter_node->field_grievance_update_rep['und'][0]['value'];
			}
		}
	}
	if ($default_subject) {
		$default_subject = token_replace($default_subject, array('node' => $grievance_node), array());
	}
	if ($default_body) {
		$default_body = token_replace($default_body, array('node' => $grievance_node), array());
	}
	if (!$default_update_rep) { $default_update_rep = 'Yes'; }
	$default_update_rep = strtolower($default_update_rep);
	$form_state['input']['body'] = $default_body;
	$form_state['input']['subject'] = $default_subject;
	$form_state['input']['update_company_representative'] = $default_update_rep;
	$global_cc_msg = join(', ', array_keys($global_ccs));
	if ($global_cc_msg) {
		$global_cc_msg = '<br>' . t("Note: the following address(es) will also be CC'd on this email: ") . '<strong>' .  $global_cc_msg . '</strong>';
	}
	$form['recipients']['extra']['global_cc'] = array(
		'#type' => 'markup',
		'#markup' => '<div id="grievance-letter-form-replace-global-cc">' . $global_cc_msg . '</div>',
	);

	$form['recipients']['extra']['subject'] = array(
		'#title' => 'Subject',
		'#type' => 'textfield',
		'#prefix' => '<div id="grievance-letter-form-replace-subject">',
		'#suffix' => '</div>',
	);

	$form['recipients']['extra']['body'] = array(
		'#title' => 'Body',
		'#type' => 'textarea',
		'#prefix' => '<div id="grievance-letter-form-replace-body">',
		'#suffix' => '</div>',
	);

	$form['recipients']['extra']['update_company_representative'] = array(
		'#title' => 'Update grievance company representative?',
		'#type' => 'select',
		'#options' => array(
			'yes' => 'Yes - The main recipient of this letter is the "Company Recipient" for the grievance',
			'no' => 'No - Leave the "Company Recipient" unchanged.'
		),
		'#default_value' => $default_update_rep,
		'#prefix' => '<div id="grievance-letter-form-replace-update-rep">',
		'#suffix' => '</div>',
	);

  // Get the company contacts
	$recipients = grievance_notify_company_contacts(array('shop_nid' => $shop_nid, 'status_nid' => $status_tid));

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
				$recipient['union'] = TRUE;

				$recipients[] = $recipient;
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
		$recipients[] = $recipient;
	}

	if ($grievance_node->field_grievance_email['und'][0]['email'] && !$nodupes[$grievance_node->field_grievance_email['und'][0]['email']]) {
		$recipient = array();
		$recipient['type'] = 'Grievant';
		$recipient['name'] = $grievance_node->field_grievance_first_name['und'][0]['value'] . ' ' . $grievance_node->field_grievance_last_name['und'][0]['value'];
		$recipient['email'] = $grievance_node->field_grievance_email['und'][0]['email'];
		$recipients[] = $recipient;
	}

	// Additional contacts
  if (!$form_state['num_addl_recipients']) {
    $form_state['num_addl_recipients'] = 0;
  }
  for ($i=0; $i<$form_state['num_addl_recipients']; $i++) {
		$recipient = array();
		$recipient['name'] = t('Other Contact #') . ($i+1);
		$recipient['is_blank'] = TRUE;

		$recipients[] = $recipient;
	}

	$form['recipients']['grievance_recipients']['grievance_recipients_wrapper_open'] = array(
		'#title' => '',
		'#type' => 'markup',
		'#markup' => '',
		'#prefix' => '<div class="grievance-recipients" id="grievance-recipients">'
	);

  $recipient_count = 0;

	$count = 0;
	foreach ($recipients as $recipient) {
		++$count;

		if ($recipient['is_blank']) {
			$form['recipients']['grievance_recipients']["recipient_${count}_intro"] = array(
				'#title' => '',
				'#type' => 'markup',
				'#markup' => '<div class="grievance-recipient-name">' . $recipient['name'] . '</div>',
				'#prefix' => "<div class=\"grievance-recipient grievance-recipient-$default_value\">",
			);

			$form['recipients']['grievance_recipients']["recipient_${count}_name"] = array(
				'#title' => 'Name',
				'#type' => 'textfield',
			);

			$form['recipients']['grievance_recipients']["recipient_${count}_email"] = array(
				'#title' => 'Email',
				'#type' => 'textfield',
			);

		} else {

      $classes = 'grievance-recipient ';

			$type_text = '';
			if ($recipient['contact_types']) {
				$type_text = '<div class="field-name-field-grievance-contact-types">';
				$type_text .= '<ul>';
				foreach ($recipient['contact_types'] as $tmp) {
					$term = taxonomy_term_load($tmp['tid']);
					$type_text .= '<li>' . $term->name . '</li>';
				}
				$type_text .= '</ul>';
				$type_text .= '</div>';
			}

			$form['recipients']['grievance_recipients']["recipient_${count}_intro"] = array(
				'#title' => '',
				'#type' => 'markup',
				'#markup' => $type_text . 
				  '<div class="grievance-recipient-name">' . $recipient['name'] . '</div>' . 
					'<div class="grievance-recipient-email">' . grievance_email_trim($recipient['email'], 25) . '</div>' . 
					'<div class="grievance-recipient-type">(' . $recipient['type'] . ')</div>',
				'#prefix' => '<div class="' . $classes . '" style="' . $style . '">',
			);

			$form['recipients']['grievance_recipients']["recipient_${count}_name"] = array(
				'#type' => 'hidden',
				'#value' => $recipient['name'],
			);

			$form['recipients']['grievance_recipients']["recipient_${count}_email"] = array(
				'#type' => 'hidden',
				'#value' => $recipient['email'],
			);
		}

		if ($recipient['nid']) {
			$form['recipients']['grievance_recipients']["recipient_${count}_nid"] = array(
				'#type' => 'hidden',
				'#value' => $recipient['nid'],
			);
		}


		$default_value = NULL;
		if ($recipient['email'] == $user->mail) {
			$default_value = 'from';
		} else if ($recipient['email'] == $grievance_node->field_grievance_co_email['und'][0]['email']) {
			$default_value = 'to';
		} else if ($recipient['secret']) {
			$default_value = 'bcc';
		} else if ($recipient['union']) {
			$default_value = 'cc';
		} else {
			$default_value = 'none';
		}

		$form['recipients']['grievance_recipients']["recipient_${count}_disposition"] = array(
			'#title' => t('This contact should be:'),
			'#type' => 'radios',
			'#options' => array(
				'none' => t('Skipped'),
				'from' => t('Sender (FROM)'),
				'to' => t('Main Recipient (TO)'),
				'cc' => t('Copied (CC)'),
				'bcc' => t('Blind Copied (BCC)'),
			),
			'#suffix' => '</div>',
			'#default_value' => $default_value,
		);
	}

  $form['recipients']['grievance_recipients']['addl_recipient'] = array(
    '#type' => 'submit',
    '#value' => t('Add Recipient'),
    '#submit' => array('grievance_letter_form_add_one'),
    '#ajax' => array(
      'callback' => 'grievance_letter_form_add_one_callback',
      'wrapper' => 'grievance-recipients',
    ),
		'#prefix' => '<div class="grievance-recipient">' . 
			'<div class="grievance-recipient-name">' . t('Other Contact #') . ($i+1) . '</div><br><br>',
		'#suffix' => '</div>',
	);

	$form['recipients']['grievance_recipients']['grievance_recipients_wrapper_conclusion'] = array(
		'#title' => '',
		'#type' => 'markup',
		'#markup' => '',
		'#prefix' => '</div>'
	);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Fax Recipients
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  $form['fax'] = array(
  	'#type' => 'fieldset',
  	'#collapsible' => TRUE,
  	'#collapsed' => TRUE,
  	'#title' => t('Fax Recipient'),
  	'#description' => t('Select or type in the fax recipient for this letter'),
	);

  // Get the company contacts
	$fax_recipients = grievance_notify_company_contacts(array('shop_nid' => $shop_nid, 'status_nid' => $status_tid));
	$found = FALSE;
	$fax_options = array(NULL => t('[select]'));
	$delta = 0;
	foreach ($fax_recipients as $recipient) {
		if ($recipient['fax']) {
			++$delta;
			$key = "$delta|" . $recipient['fax'];
			$found = TRUE;
			$fax_options[$key] = $recipient['fax'] . ': ' . $recipient['name'];
			if ($recipient['role']) { $fax_options[$key] .= ' (' . $recipient['role'] . ')'; }
			if ($recipient['description']) { $fax_options[$key] .= ' (' . $recipient['description'] . ')'; }
		}
	}
	if ($found) {
		$form['fax']['fax_recipient_dropdown'] = array(
			'#title' => 'Company Contacts',
			'#type' => 'select',	
			'#options' => $fax_options,
			'#attributes' => array('class' => array('grievance_fax_recipient_dropdown')),
		);
	}

	$form['fax']['fax_recipient'] = array(
		'#title' => 'Fax Number',
		'#type' => 'textfield',	
		'#attributes' => array('class' => array('grievance_fax_recipient')),
	);


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
    	'send-email' => t('Send the letter by email'), 
    	'send-fax' => t('Send the letter by fax'), 
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
		'#prefix' => '<div id="grievance-letter-form-replace-logging">',
		'#suffix' => '</div>',
  );
	$letter_nid = $form_state['values']['letter_nid_0'];
	$letter_node = node_load($letter_nid);
	$form_state['input']['logging'] = $letter_node->field_grievance_log_type['und'][0]['tid'];

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
			'#rows' => 25,
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

function grievance_letter_form_add_one_callback($form, $form_state) {
  return $form['recipients']['grievance_recipients'];
}

function grievance_letter_form_add_one($form, &$form_state) {
  $form_state['num_addl_recipients']++;
  $form_state['rebuild'] = TRUE;
}

