<?php

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

	if ($data['format'] == 'email') {
		if (strlen($data['pdf_body']) > 10) {
			$form['action'] = array(
				'#type' => 'select',
				'#title' => t('Action'),
				'#options' => array(
					'send' => t('Send Email Now'),
					'preview_download' => t('Download PDF')
				),
			);
		}
	}

	if ($data['format'] == 'pdf') { $submit_label = t('Download PDF'); }
	else if ($data['format'] == 'sms') { $submit_label = t('Send SMS Now'); }
	else if ($data['format'] == 'postal') { $submit_label = t('Send Postal Letter Now'); }
	else { $submit_label = t("Submit"); }

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

	$action = $form_state['values']['action'];
	if (!$action) { $action = 'send'; }

	$format = $data['format'];

	// Generate PDF if necessary
	if ($format == 'pdf' || $format == 'email') {
		$pdf_basename = sirius_letter_pdf_basename($letter_node);
		$pdf_contents = sirius_letter_pdf_contents($letter_node);
	}

	// Log the letter
	if ($form_state['values']['logging'] && ($action != 'preview_download')) {

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

		if ($letter_node->field_sirius_attachments) {
			foreach ($letter_node->field_sirius_attachments['und'] as $delta => $record) {
				$log_body .= "Attached: Grievance attachment " . $record['filename'] . ' / original filename ' . $record['origname'] . "\n";
			}
		}

		if ($data['upload_others']) {
			$keys = array_filter(array_values($data['upload_others']));
			foreach ($keys as $key) {
				list ($other_nid, $delta) = explode('|', $key);
				if (!$delta) { $delta = 0; }
				$other_node = node_load($other_nid);
				$record = $other_node->field_grievance_attachments['und'][$delta];
				$log_body .= "Attached: Letter attachment " . $record['filename'] . ' / original filename ' . $record['origname'] . "\n";
			}
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
	if ($format == 'pdf' || $action == 'preview_download') {
		// Dump to browser and exit
		header('Content-Type: application/pdf');
		header("Content-Disposition: attachment; filename=$pdf_basename.pdf");
		print $pdf_contents;
		if ($action == 'preview_download'); { exit(); }

		// Can't call sirius_wizard_submit() because that redirects
		$data['steps'][$current_step]['complete'] = TRUE;
		$letter_node->field_sirius_type['und'][0]['value'] = 'sent';
		$data['sent_ts'] = time();
		sirius_letter_set_title($letter_node, 'sent', $data['sent_ts']);
		sirius_wizard_data_set($info, $letter_node, $data);
	}

	// If format is "email", send
	if ($format == 'email') {
		// findme

		// Get the first letter template (ugh)
	  $letter_template_nodes = array();
	  for ($i=0; $i<5; ++$i) {
	  	$letter_template_nid = $data["letter_template_nid_$i"];
	  	if ($letter_template_nid) { break; }
	  }
  	$letter_template_node = node_load($letter_template_nid);

  	// Set the grievnace shop contact
	  if (
	  	($entity_node->type == 'grievance') && 
	  	($letter_template_node) && 
	  	($letter_template_node->field_grievance_update_rep['und'][0]['value'] == 'Yes') &&
	  	$data['recipients']
	  	) {

	  	$is_changed = FALSE;

	  	foreach ($data['recipients'] as $recipient) {
	  		if ($recipient['disposition'] != 'to') { continue; }

				if ($entity_node->field_grievance_co_name['und'][0]['value'] != $recipient['name']) {
					$entity_node->field_grievance_co_name['und'][0]['value'] = $recipient['name'];
					$is_changed = TRUE;
				}

				if ($entity_node->field_grievance_co_email['und'][0]['email'] != $recipient['email']) {
					$entity_node->field_grievance_co_email['und'][0]['email'] = $recipient['email'];
					$is_changed = TRUE;
				}

				if ($recipient['nid']) {
					$recipient_node = node_load($recipient['nid']);
					$fields = array(
						'field_grievance_co_phone',
						'field_grievance_co_address',
						'field_grievance_co_address_2',
						'field_grievance_co_city',
						'field_grievance_co_state',
						'field_grievance_co_zip'
					);
					foreach ($fields as $field) {
						if ($entity_node->{$field}['und'][0]['value'] != $recipient_node->{$field}['und'][0]['value']) {
							$entity_node->{$field}['und'][0]['value'] = $recipient_node->{$field}['und'][0]['value'];
							$is_changed = TRUE;
						}
					}
				}

				if ($is_changed) { node_save($entity_node); }
				break; 
			}
	  }

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
			$keys = array_filter(array_values($data['upload_others']));
			foreach ($keys as $key) {
				list ($other_nid, $delta) = explode('|', $key);
				if (!$delta) { $delta = 0; }
				$other_node = node_load($other_nid);
				$record = $other_node->field_grievance_attachments['und'][$delta];
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
						'filename' => $entity_node->type . '_' . $entity_node->nid . '_' . $other_nid . '_' . time() . ".$ext",
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

			$params['handler_nids'] = array();
			$params['handler_nids'][] = $letter_node->nid;
			if ($log_node) { $params['handler_nids'][] = $log_node->nid; }

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

	// If format is "sms", send
	if ($format == 'postal') {
		if (!sirius_domain_component_enabled('postal')) { drupal_set('Postal module is not enabled.', 'error'); return; }
		$instance = sirius_postal_client_instance();
		if (!$instance) { drupal_set('No postal client has been configured.', 'error'); return; }

		$params = array();

		$params['to'] = $data['recipients'][0]['addr'];
		$params['from'] = $data['recipients'][1]['addr'];
		$params['body'] = $data['body'];
		$params['options'] = $data['postal_options'];
		// $params['id'] = $letter_node->nid;
		$params['handler_nids'] = array($letter_node->nid);
		$result = $instance->send($params);

		$failure_count = 0;
		if (!$result['success']) {
			drupal_set_message("Postal delivery failed: " . $result['msg'] . ': ' . $result['details'], 'warning');
			$failure_count++;
		} else {
			drupal_set_message("Postal delivery successful.");
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
