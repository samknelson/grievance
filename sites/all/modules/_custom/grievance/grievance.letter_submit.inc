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
		// Sometimes a "letter" is just an attachment.  If so, we shortcut and just redirect to the attachment.
		foreach ($letter_nodes as $letter_nid => $letter_node) {
			if ($letter_node->field_grievance_attachments['und'][0]['uri']) {
				$url = file_create_url($letter_node->field_grievance_attachments['und'][0]['uri']);
				if ($url) {
					drupal_goto($url);
				}
			}
		}

		// Build the PDF
		$pdf_contents = grievance_letter_pdf_contents($grievance_node, $letter_nids, $form_state['values']);
		$basename = grievance_letter_pdf_basename($grievance_node, $letter_nids);

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

	// ////////////////////////////////////////////
	// Edit
	// ////////////////////////////////////////////

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
		if (count($recipients['to']) <> 1) {
			drupal_set_message(t('Exactly one contact must be the primary recipient (TO).'), 'error');
			return;
		}

		if (count($recipients['from']) <> 1) {
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
		$grievance_node = $form_state['grievance_node'];

		if ($form_state['values']['update_company_representative'] == 'yes') {
			$is_changed = FALSE;

			if ($grievance_node->field_grievance_co_name['und'][0]['value'] != $recipients['to'][0]['name']) {
				$grievance_node->field_grievance_co_name['und'][0]['value'] = $recipients['to'][0]['name'];
				$is_changed = TRUE;
			}

			if ($grievance_node->field_grievance_co_email['und'][0]['email'] != $recipients['to'][0]['email']) {
				$grievance_node->field_grievance_co_email['und'][0]['email'] = $recipients['to'][0]['email'];
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
					if ($grievance_node->{$field}['und'][0]['value'] != $recipient_node->{$field}['und'][0]['value']) {
						$grievance_node->{$field}['und'][0]['value'] = $recipient_node->{$field}['und'][0]['value'];
						$is_changed = TRUE;
					}
				}
			}

			if ($is_changed) {
				node_save($grievance_node);
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
		$pdf_contents = grievance_letter_pdf_contents($grievance_node, $letter_nids, $form_state['values']);
		$basename = grievance_letter_pdf_basename($grievance_node, $letter_nids);

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
		$params['handler_nids'] = array($grievance_node->nid);
		if ($log_node) { $params['handler_nids'][] = $log_node->nid; }

		$params['reply-to'] = $from;

		// Attach the grievance as a PDF
		$params['attachments'] = array();
		$params['attachments'][] = array(
			'filecontent' => $pdf_contents,
			'filename' => 'grievance_' . $grievance_node->nid . '_' . time() . '.pdf',
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
		$pdf_contents = grievance_letter_pdf_contents($grievance_node, $letter_nids, $form_state['values']);
		$basename = grievance_letter_pdf_basename($grievance_node, $letter_nids);

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

function grievance_letter_render_html($grievance_node, $letter_nids, $other_tokens) {
	// Add the worker node as a tokenized object
	$tokens = array();
	$token_aliases = array();

	$workersync_plugin = grievance_workersync_plugin_fetch();
	if ($workersync_plugin['id'] == 'local') {
		$worker_id = $grievance_node->field_grievance_min['und'][0]['value'];
		$worker_node = sirius_worker_get_by_id($worker_id);
		if ($worker_node) {
			$tokens['worker_node'] = $worker_node;
			$token_aliases['worker_node'] = 'node';
		}
	}

	if ($other_tokens['letter_body']) {
		return '<div id="grievance-letter-body">' . $other_tokens['letter_body'] . '</div>';
	}
	$letter_nodes = node_load_multiple($letter_nids);

	$tokens['node'] = $grievance_node;
	$tokens['other_tokens'] = $other_tokens;

	$body = '';
	if ($_REQUEST['uid'] && sirius_loopback_access()) { sirius_become_superuser(TRUE, $_REQUEST['uid']); }
	foreach ($letter_nodes as $letter_nid => $letter_node) {
		$letter_part = $letter_node->body['und'][0]['value'];
		$letter_part = token_replace(
			$letter_part, 
			$tokens,
			array(
				'clear' => TRUE,
				'aliases' => $token_aliases,
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

function grievance_letter_log($form_state, $log_title, $log_body, $pdf_contents, $basename, $external_id = NULL) {
	$grievance_node = $form_state['grievance_node'];
	$log_type_tid = $form_state['values']['logging'];

  global $user;
  $log_node = new stdClass();
  $log_node->title =$log_title;
  $log_node->type = 'grievance_log';
  node_object_prepare($log_node);
  $log_node->language = LANGUAGE_NONE;
  $log_node->uid = $user->uid; 
  $log_node->status = 1;
  $log_node->promote = 0;
  $log_node->comment = 0;

  $log_node->field_grievance['und'][0]['target_id'] = $grievance_node->nid;
  $log_node->field_grievance_log_type['und'][0]['tid'] = $log_type_tid;
  $log_node->field_grievance_status_date['und'][0]['value'] = date('Y-m-d H:i:s'); 
	if ($external_id) {
		$log_node->field_grievance_external_id['und'][0]['value'] = $external_id;
	}
  $log_node->body['und'][0]['value'] = $log_body;

  $file = file_save_data(
    $pdf_contents, 
    'private://grievance_attachments/' . $basename . '.pdf', 
    FILE_EXISTS_RENAME
  );
  $file->display = 1;
  $log_node->field_grievance_attachments["und"][0] = (array) $file;

  $log_node = node_submit($log_node); // Prepare node for saving
  node_save($log_node);
  return $log_node;
}