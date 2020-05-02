<?php

function sirius_node_tab_entity_letter($node) {
	return sirius_node_tab_wizard($node, 'letter');


	//return drupal_get_form('sirius_node_tab_entity_letter_form', $node);
}

function sirius_node_tab_entity_letter_form($form, &$form_state, $node) {
	$form_state['nid'] = $node->nid;

	if ($node->type == 'grievance') {
		$status_tid = $node->field_grievance_status['und'][0]['tid'];
		$status_term = taxonomy_term_load($status_tid);
		$shop_nid = $node->field_grievance_shop['und'][0]['target_id'];
	}

  drupal_add_library('system', 'ui.accordion');
  drupal_add_js('jQuery(document).ready(function () { jQuery("#sirius_accordion").accordion({active: false, collapsible: true, autoehight: true, heightStyle: "content"}); });', 'inline');

  $form['accordion_open'] = array('#markup' => '<div id="sirius_accordion">');

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Letter to send
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  $form['accordion_open_letter'] = array('#markup' => '<div class="sirius_accordion_label">' . t('Which letter do you want to send?') . '</div><div class="sirius_accordion_body">');

	// Get the possible letters (as checkboxes)
	$letter_nodes = sirius_letter_list($node);
  $options = array(NULL => t('-- Select --'));
	foreach ($letter_nodes as $letter_nid => $letter_node) {
		$options[$letter_nid] = $letter_node->title;
	}

  for ($i=0; $i<5; ++$i) {
	  $form['letter']['letter_nid_' . $i] = array(
	    '#type' => 'select',
	    '#options' => $options,
	    '#title' => t('Select Letter #') . ($i+1),
	    '#ajax' => array(
	      'callback' => 'sirius_node_tab_entity_letter_form_callback',
	      'method' => 'replace',
	    ),
	    // '#prefix' => '<div style="float: left; padding-right: 10px;">',
	    // '#suffix' => '</div>',
	  );
	  if ($i == 0) {
	    $form['letter']['letter_nid_' . $i]['#required'] = TRUE;
	  }
	}

	if (user_access('grievance letter variable delivery')) {
	  // Get the delivery options, as a substitution variable
	  $delivery_options = sirius_taxonomy_load('grievance_delivery', 'tid', 'name', TRUE);
	  if (count($delivery_options) > 1) {
			$form['letter']['delivery_tid'] = array(
				'#type' => 'select',
				'#options' => $delivery_options,
				'#title' => t('Delivery'),
			);
		}
	}

	if (user_access('grievance letter variable letterhead')) {
	  // Letterhead
		$letterheads = sirius_letterhead_list();
		if ($letterheads) {
			$letterhead_options = array(NULL => t('-- Select --'));
			foreach ($letterheads as $nid => $title) {
				$letterhead_options[$nid] = $title;
			}
		  $form['letter']['letterhead'] = array(
		    '#type' => 'select',
		    '#options' => $letterhead_options,
		    '#title' => t('Letterhead'),
		  );
		}
	}

  $form['accordion_close_letter'] = array('#markup' => '</div>');

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Subject and Body
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  $form['accordion_open_body'] = array('#markup' => '<div class="sirius_accordion_label">' . t('Letter Text') . '</div><div class="sirius_accordion_body">');

	$form['subject'] = array(
		'#title' => 'Subject',
		'#type' => 'textfield',
		'#prefix' => '<div id="grievance-letter-form-replace-subject">',
		'#suffix' => '</div>',
	);

	$form['body'] = array(
		'#title' => 'Body',
		'#type' => 'textarea',
		'#prefix' => '<div id="grievance-letter-form-replace-body">',
		'#suffix' => '</div>',
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
		$default_subject = token_replace($default_subject, array('node' => $node), array());
	}
	if ($default_body) {
		$default_body = token_replace($default_body, array('node' => $node), array());
	}

	$form_state['input']['body'] = $default_body;
	$form_state['input']['subject'] = $default_subject;

  $form['accordion_close_body'] = array('#markup' => '</div>');

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Recipients
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  $form['accordion_open_recipients'] = array('#markup' => '<div class="sirius_accordion_label">' . t('Recipients') . '</div><div class="sirius_accordion_body">');

	$global_cc_msg = join(', ', array_keys($global_ccs));
	if ($global_cc_msg) {
		$global_cc_msg = '<br>' . t("Note: the following address(es) will also be CC'd on this email: ") . '<strong>' .  $global_cc_msg . '</strong>';
	}
	$form['recipients']['extra']['global_cc'] = array(
		'#type' => 'markup',
		'#markup' => '<div id="grievance-letter-form-replace-global-cc">' . $global_cc_msg . '</div>',
	);

	if ($node->type == 'grievance') {
		if (!$default_update_rep) { $default_update_rep = 'Yes'; }
		$default_update_rep = strtolower($default_update_rep);
		$form_state['input']['update_company_representative'] = $default_update_rep;
	
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
			if (isset($node->{$field}['und'])) {
				$info = field_info_instance('node', $field, 'grievance');
				$label = $info['label'];

				foreach ($node->{$field}['und'] as $tmp) {
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

	if ($node->field_grievance_email['und'][0]['email'] && !$nodupes[$node->field_grievance_email['und'][0]['email']]) {
		$recipient = array();
		$recipient['type'] = 'Grievant';
		$recipient['name'] = $node->field_grievance_first_name['und'][0]['value'] . ' ' . $node->field_grievance_last_name['und'][0]['value'];
		$recipient['email'] = $node->field_grievance_email['und'][0]['email'];
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

	$form['grievance_recipients_wrapper_open'] = array(
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
			$form["recipient_${count}_intro"] = array(
				'#title' => '',
				'#type' => 'markup',
				'#markup' => '<div class="grievance-recipient-name">' . $recipient['name'] . '</div>',
				'#prefix' => "<div class=\"grievance-recipient grievance-recipient-$default_value\">",
			);

			$form["recipient_${count}_name"] = array(
				'#title' => 'Name',
				'#type' => 'textfield',
			);

			$form["recipient_${count}_email"] = array(
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

			$form["recipient_${count}_intro"] = array(
				'#title' => '',
				'#type' => 'markup',
				'#markup' => $type_text . 
				  '<div class="grievance-recipient-name">' . $recipient['name'] . '</div>' . 
					'<div class="grievance-recipient-email">' . grievance_email_trim($recipient['email'], 25) . '</div>' . 
					'<div class="grievance-recipient-type">(' . $recipient['type'] . ')</div>',
				'#prefix' => '<div class="' . $classes . '" style="' . $style . '">',
			);

			$form["recipient_${count}_name"] = array(
				'#type' => 'hidden',
				'#value' => $recipient['name'],
			);

			$form["recipient_${count}_email"] = array(
				'#type' => 'hidden',
				'#value' => $recipient['email'],
			);
		}

		if ($recipient['nid']) {
			$form["recipient_${count}_nid"] = array(
				'#type' => 'hidden',
				'#value' => $recipient['nid'],
			);
		}


		$default_value = NULL;
		if ($recipient['email'] == $user->mail) {
			$default_value = 'from';
		} else if ($recipient['email'] == $node->field_grievance_co_email['und'][0]['email']) {
			$default_value = 'to';
		} else if ($recipient['secret']) {
			$default_value = 'bcc';
		} else if ($recipient['union']) {
			$default_value = 'cc';
		} else {
			$default_value = 'none';
		}

		$form["recipient_${count}_disposition"] = array(
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

  $form['addl_recipient'] = array(
    '#type' => 'submit',
    '#value' => t('Add Recipient'),
    '#submit' => array('sirius_node_tab_entity_letter_form_add_one'),
    '#ajax' => array(
      'callback' => 'sirius_node_tab_entity_letter_form_add_one_callback',
      'wrapper' => 'grievance-recipients',
    ),
		'#prefix' => '<div class="grievance-recipient">' . 
			'<div class="grievance-recipient-name">' . t('Other Contact #') . ($i+1) . '</div><br><br>',
		'#suffix' => '</div>',
		'#limit_validation_errors' => array(),
	);

	$form['grievance_recipients_wrapper_conclusion'] = array(
		'#title' => '',
		'#type' => 'markup',
		'#markup' => '',
		'#prefix' => '</div>'
	);

  $form['accordion_close_recipients'] = array('#markup' => '</div>');


	if (sirius_fax_enabled()) {
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
	  if ($node->type == 'grievance') {
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
	}


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
    	'send-email' => t('Send the letter by email'), 
    	'send-fax' => t('Send the letter by fax'), 
    ),
    '#title' => t('Output'),
  );

  if ($node->type == 'grievance') {
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
	}

  $form['dest']['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Go',
  );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// On-Screen Preview
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // Edit
	$form['letter_body'] = array(
		'#title' => 'Edit the Letter',
		'#type' => 'text_format',
		'#default_value' => $form_state['edit_result'],
		'#format' => 'full_html',
		'#rows' => 25,
	);

  $form['accordion_close'] = array('#markup' => '</div>');
	
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

function sirius_node_tab_entity_letter_form_add_one_callback($form, $form_state) {
  return $form['recipients']['grievance_recipients'];
}

function sirius_node_tab_entity_letter_form_add_one($form, &$form_state) {
  $form_state['num_addl_recipients']++;
  $form_state['rebuild'] = TRUE;
}

function sirius_node_tab_entity_letter_form_callback($form, $form_state) {
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


function sirius_node_tab_entity_letter_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
	$form_state['result'] = NULL;
	$node = node_load($form_state['nid']);
	$output = $form_state['values']['output'];
	if (!$output) {
		drupal_set_message(t('No output specified.'), 'error');
		return;
	}

	// Get the letter nid
	$letter_nids = array();
	for ($i=0; $i<5; ++$i) {
		$letter_nid = $form_state['values']['letter_nid_' . $i];
		if ($letter_nid) {
			$letter_nids[] = $letter_nid;
		}
	}
	if (!$letter_nids) {
		drupal_set_message(t('No letter selected.'), 'error');
		return;
	}

	$letter_nodes = node_load_multiple($letter_nids);

	// Find out if we must log
	if ($node->type == 'grievance') {
		$mustlog = FALSE;
		foreach ($letter_nodes as $letter_nid => $letter_node) {
			if ($letter_node->field_sirius_mustlog['und'][0]['value'] == 'Yes') {
				$mustlog = TRUE;
			}
		}

		if ($mustlog) {
			if ($output == 'pdf' || $output == 'send-email' || $output == 'send-fax') {
				if (!$form_state['values']['logging']) {
					drupal_set_message(t('Letters of this type MUST be logged.'), 'error');
					return;
				}
			}
		}
	}

	$global_cc_recipients = array();
	foreach ($letter_nodes as $letter_node) {
		if ($letter_node->field_grievance_emails['und']) {
			foreach ($letter_node->field_grievance_emails['und'] as $tmp) {
				$global_cc_recipients[$tmp['value']] = TRUE;
			}
		}
	}
	$global_cc_recipients = array_keys($global_cc_recipients);

	// ////////////////////////////////////////////
	// PDF
	// ////////////////////////////////////////////

	if ($output == 'pdf') {
		// Build the PDF
		$pdf_contents = grievance_letter_pdf_contents($node, $letter_nids, $form_state['values']);
		$basename = sirius_letter_pdf_basename($node, $letter_nids);

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

	// ////////////////////////////////////////////
	// Screen
	// ////////////////////////////////////////////

	if ($output == 'preview') {
		drupal_set_message(t('The letter(s) are displayed at the bottom of this page.'));
		$form_state['result'] = sirius_letter_render_html(
			$node, 
			$letter_nids, 
			array(
				'delivery_tid' => $form_state['values']['delivery_tid'],
				'letter_body' => $form_state['values']['letter_body']['value'],
			)
		);
		return;
	}

	// ////////////////////////////////////////////
	// Edit
	// ////////////////////////////////////////////

	if ($output == 'edit') {
		drupal_set_message(t('The letter(s) can be edited at the bottom of this page.'));
		$form_state['edit_result'] = sirius_letter_render_html(
			$node, 
			$letter_nids, 
			array(
				'delivery_tid' => $form_state['values']['delivery_tid'],
				'body' => $form_state['values']['letter_body'],
			)
		);
		return;
	}

	// ////////////////////////////////////////////
	// Send Email
	// ////////////////////////////////////////////

	if ($output == 'send-email') {
		// Get the company recipients.  @TODO harcoding 100 is lazy.
		$recipients = array();
		for ($i=0; $i<100; ++$i) {
			$name = $form_state['values']["recipient_{$i}_name"];
			$email = $form_state['values']["recipient_{$i}_email"];
			$disposition = $form_state['values']["recipient_{$i}_disposition"];
			$contact_nid = $form_state['values']["recipient_{$i}_nid"];

			// Skip unless we have at least one value
			if (!$name && !$email && !$disposition) { 
				continue;
			}

			// Need email and disposition
			if (!$email || !$disposition) {
				if ($disposition != 'none') {
					drupal_set_message(t('For each contact, enter the email address and how the contact should be handled (Skip, TO, Sender, CC, or BCC)'), 'error');
					return;
				}
			}

			$recipients[$disposition][] = array(
				'name' => $name, 
				'email' => $email,
				'nid' => $contact_nid,
			);
		}
		if (!$recipients['to'] || count($recipients['to']) <> 1) {
			drupal_set_message(t('Exactly one contact must be the primary recipient (TO).'), 'error');
			return;
		}

		if (!$recipients['from'] || count($recipients['from']) <> 1) {
			drupal_set_message(t('Exactly one contact must be the sender (FROM).'), 'error');
			return;
		}

		$subject = $form_state['values']['subject'];
		$body = $form_state['values']['body'];
		if (!$subject || !$body) {
			drupal_set_message(t('Enter a subject and a body for the email.  The letter will be attached as a PDF document.'), 'error');
			return;
		}

		// We've passed the tests, and we're committed to sending the email.
		// First, we want to update the node with the primary recipient.

		if ($form_state['values']['update_company_representative'] == 'yes') {
			$is_changed = FALSE;

			if ($node->field_grievance_co_name['und'][0]['value'] != $recipients['to'][0]['name']) {
				$node->field_grievance_co_name['und'][0]['value'] = $recipients['to'][0]['name'];
				$is_changed = TRUE;
			}

			if ($node->field_grievance_co_email['und'][0]['email'] != $recipients['to'][0]['email']) {
				$node->field_grievance_co_email['und'][0]['email'] = $recipients['to'][0]['email'];
				$is_changed = TRUE;
			}

			if ($recipients['to'][0]['nid']) {
				$recipient_node = node_load($recipients['to'][0]['nid']);
				$fields = array(
					'field_grievance_co_phone',
					'field_grievance_co_address',
					'field_grievance_co_address_2',
					'field_grievance_co_city',
					'field_grievance_co_state',
					'field_grievance_co_zip'
				);
				foreach ($fields as $field) {
					if ($node->{$field}['und'][0]['value'] != $recipient_node->{$field}['und'][0]['value']) {
						$node->{$field}['und'][0]['value'] = $recipient_node->{$field}['und'][0]['value'];
						$is_changed = TRUE;
					}
				}
			}

			if ($is_changed) {
				node_save($node);
			}
		}


		$to = $recipients['to'][0]['name'] . ' <' . $recipients['to'][0]['email'] . '>';
		$from = $recipients['from'][0]['name'] . ' <' . $recipients['from'][0]['email'] . '>';

		$cc = '';
		if ($recipients['cc']) {
			foreach ($recipients['cc'] as $recipient) {
				if ($cc) { $cc .= ', '; }
				$cc .= $recipient['name'] . ' <' . $recipient['email'] . '>';
			}
		}
		foreach ($global_cc_recipients as $global_cc_recipient) {
			if ($cc) { $cc .= ', '; }
			$cc .= $global_cc_recipient;
		}
		$bcc = '';
		if ($recipients['bcc']) {
			foreach ($recipients['bcc'] as $recipient) {
				if ($bcc) { $bcc .= ', '; }
				$bcc .= $recipient['name'] . ' <' . $recipient['email'] . '>';
			}
		}

		// Build the PDF
		$pdf_contents = grievance_letter_pdf_contents($node, $letter_nids, $form_state['values']);
		$basename = sirius_letter_pdf_basename($node, $letter_nids);

		// Log the letter
		if ($form_state['values']['logging']) {
			$log_title = "Letter Sent by Email: ";
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
			$log_body .= "From: $from\n";
			$log_body .= "To: $to\n";
			if ($cc) {
				$log_body .= "Cc: $cc\n";
			}
			if ($bcc) {
				$log_body .= "Bcc: $bcc\n";
			}
			$log_body .= "Subject: $subject\n";
			$log_body .= "\n$body";

			$log_node = grievance_letter_log($form_state, $log_title, $log_body, $pdf_contents, $basename);
		}

		$params = array();
		$params['body'] = $body;
		$params['subject'] =  $subject;
		$params['to'] = $to;
		if ($cc) { $params['headers']['Cc'] = $cc; }
		if ($bcc) { $params['headers']['Bcc'] = $bcc; }
		$params['handler_nids'] = array($node->nid);
		if ($log_node) { $params['handler_nids'][] = $log_node->nid; }

		$params['reply-to'] = $from;

		// Attach the grievance as a PDF
		$params['attachments'] = array();
		$params['attachments'][] = array(
			'filecontent' => $pdf_contents,
			'filename' => 'grievance_' . $node->nid . '_' . time() . '.pdf',
			'filemime' => 'grievance/pdf',
		);

		$result = sirius_email_send($params);

		if (!$result['success']) {
			drupal_set_message(t('Message failed: ') . $result['msg'], 'error');
		} else {
			drupal_set_message(t('Sent Notification: ') . htmlspecialchars($subject));
			drupal_set_message(t('Notification FROM: ') . htmlspecialchars($params['reply-to']));
			drupal_set_message(t('Notification TO: ') . htmlspecialchars($to));
			if ($cc) { drupal_set_message(t('Notification CC: ') . htmlspecialchars($cc)); }
			if ($bcc) { drupal_set_message(t('Notification BCC: ') . htmlspecialchars($bcc)); }
		}

		return;
	}

	// ////////////////////////////////////////////
	// Send Email
	// ////////////////////////////////////////////

	if ($output == 'send-fax') {
		
		$fax_number = $form_state['values']['fax_recipient'];
		if (!$fax_number) {
			drupal_set_message(t('Please select a fax recipient in order to send a fax.'), 'error');
			return;
		}

		require_once DRUPAL_ROOT . '/sites/all/libraries/phaxio/lib/Phaxio/PhaxioException.php';
		require_once DRUPAL_ROOT . '/sites/all/libraries/phaxio/lib/Phaxio/PhaxioOperationResult.php';
		require_once DRUPAL_ROOT . '/sites/all/libraries/phaxio/lib/Phaxio/Phaxio.php';
		// use Phaxio\Phaxio;
		$phaxio = new \Phaxio\Phaxio(variable_get('grievance_phaxio_api_key', ''), variable_get('grievance_phaxio_api_secret', ''), variable_get('grievance_phaxio_api_host', 'https://api.phaxio.com/v1/'));
		$pdf_contents = grievance_letter_pdf_contents($node, $letter_nids, $form_state['values']);
		$basename = sirius_letter_pdf_basename($node, $letter_nids);

		$path = '/tmp/' . $basename . '.pdf';
		file_put_contents($path, $pdf_contents);
		// $fax_result = $phaxio->sendFax('866-691-5968', array($path), array());
		// $fax_result = $phaxio->sendFax('123-456-7890', array($path), array());
		// global $base_url;
		global $is_https;
		if ($is_https ||
			(isset($_SERVER['HTTP_X_FORWARDED_PROTO']) && $_SERVER['HTTP_X_FORWARDED_PROTO'] == 'https')) {
			$callback_url = 'https://';
		} else { 
			$callback_url = 'http://';
		}
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
			// Log the letter
			if ($form_state['values']['logging']) {
				$log_title = "Faxed to $fax_number: ";
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
				grievance_letter_log($form_state, $log_title, $log_body, $pdf_contents, $basename, 'phaxio_id_sent_' . $fax_id);
			}
		}

		return;
	}

	drupal_set_message(t('Unknown output: [@output]', array('@output' => $output)), 'error');
	return;
}


function sirius_letter_log($form_state, $log_title, $log_body, $pdf_contents, $basename, $external_id = NULL) {
	// @tood
}