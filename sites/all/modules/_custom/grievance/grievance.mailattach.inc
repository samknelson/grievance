<?php

function grievance_mailattach_fetch() {
	// Get our connection information.  If we're not configured, quit silently.
	$grievance_mailattach_user = variable_get('grievance_mailattach_user', '');
	$grievance_mailattach_pass = variable_get('grievance_mailattach_pass', '');
	$grievance_mailattach_imapstr = variable_get('grievance_mailattach_imapstr', '');
	if (!$grievance_mailattach_user || !$grievance_mailattach_pass || !$grievance_mailattach_imapstr) {
		return;
	}

	// Connect
	$mbox = imap_open($grievance_mailattach_imapstr, $grievance_mailattach_user, $grievance_mailattach_pass);
	if (!$mbox) { 
		watchdog('grievance_mailattach', "Unable to connect to $grievance_mailattach_imapstr as $grievance_mailattach_user: " . imap_last_error());
	}

	// Get the number of messages
	$MC = imap_check($mbox);

	// Get the list of messages
	$messages = imap_fetch_overview($mbox,"1:{$MC->Nmsgs}",0);
	if (!$messages) {
		watchdog('grievance_mailattach', 'No messages found.');
		return;
	}

	// For each message
	foreach ($messages as $message) {

		watchdog('grievance_mailattach', 'Processing message <pre>' . print_r($message, 1) . '</pre>');

		// 
		// Get the user
		//

		$from = $message->from;
		if (!$from) {
			watchdog('grievance_mailattach', "No FROM address in message; skipping: <pre>" . print_r($message, 1) . "</pre>");
			continue;
		}
		$parts = mailparse_rfc822_parse_addresses($from);
		if (!$parts) {
			watchdog('grievance_mailattach', "Unable to parse the FROM address ($from); skipping: <pre>" . print_r($message, 1) . "</pre>");
			continue;
		}
		$from_mail = $parts[0]['address'];
		if (!$from_mail) {
			watchdog('grievance_mailattach', "Unable to parse the FROM address ($from); skipping: <pre>" . print_r($message, 1) . "</pre>");
			continue;
		}
		$user = user_load_by_mail($from_mail);
		if (!$user) {
			watchdog('grievance_mailattach', "Message not sent by a valid user ($from_mail); rejecting: <pre>" . print_r($message, 1) . "</pre>");
			grievance_mailattach_fetch_reject($from_mail, "The sender's email address ($from_mail) does not correspond to a valid system user.");
			continue;
		}

		//
		// Get the grievance
		//

		$subject = $message->subject;
		if (!$subject) {
			watchdog('grievance_mailattach', "Mail subject not found; rejecting: <pre>" . print_r($message, 1) . "</pre>");
			grievance_mailattach_fetch_reject($from_mail, "The subject of the email was not found.");
			continue;
		}
		list ($grievance_id, $rest) = explode(':', $subject);
		$grievance_id = preg_replace('/re\:?/i', '', $grievance_id);
		$grievance_id = trim($grievance_id);
		$grievance_id = preg_replace('/ .+/', '', $grievance_id);
		if (!$grievance_id) {
			watchdog('grievance_mailattach', "Grievance ID missing; rejecting: <pre>" . print_r($message, 1) . "</pre>");
			grievance_mailattach_fetch_reject($from_mail, "Unable to determine which grievance ID you want to use.  Make sure to put the ID number at the very beginning of the subject of your emails.");
			continue;
		}

		$grievance_node = grievance_fetch_by_id($grievance_id);
		if (!$grievance_node) {
			watchdog('grievance_mailattach', "Unable to find grievance id ($grievance_id); rejecting: <pre>" . print_r($message, 1) . "</pre>");
			grievance_mailattach_fetch_reject($from_mail, "Unable to determine which grievance you want to use (couldn't find a grievance with id [$grievance_id]).  Make sure to put the ID number at the very beginning of the subject of your emails.");
			continue;
		}

		//
		// Get the structure of the message
		//

		$msgno = $message->msgno;
		$structure = imap_fetchstructure($mbox, $msgno);

		//
		// Fetch all attachments.  @todo I don't like what this does to memory.
		//

		$attachments = grievance_imap_fetch_all_parts($mbox, $msgno, $structure);

		//
		// Create the attachment node
		//

	  $attachment_node = new stdClass();
	  $attachment_node->title = "Emailed attachment fetched " . date('Y-m-d');
	  $attachment_node->type = 'grievance_attachment';
	  node_object_prepare($attachment_node);
	  $attachment_node->language = LANGUAGE_NONE;
	  $attachment_node->uid = $user->uid; 
	  $attachment_node->status = 1;
	  $attachment_node->promote = 0;
	  $attachment_node->comment = 0;
	  $attachment_node->field_grievance['und'][0]['target_id'] = $grievance_node->nid;
	  if ($rest) {
	  	$attachment_node->field_grievance_label['und'][0]['value'] = trim($rest);
	  }
	  if (variable_get('grievance_mailattach_type', '')) {
	  	$attachment_node->field_grievance_document_type['und'][0]['tid'] = variable_get('grievance_mailattach_type', '');
	  }
	  $idx = 0;
	  foreach ($attachments as $idx => &$attachment) {
  		if (!$attachment['filename']) {
  			if ($idx == 0) {
  				$attachment['filename'] = "body-$idx.txt";
  			} else {
  				$attachment['filename'] = "body-$idx.html";
  			}
  		}
      $file = file_save_data(
        $attachment['bindata'], 
        'private://grievance_attachments/' . $attachment['filename'],
        FILE_EXISTS_RENAME
      );
      $file->display = 1;
      if (!$attachment_node->field_grievance_attachments["und"]) {
      	$attachment_node->field_grievance_attachments["und"] = array();
      }
      $attachment_node->field_grievance_attachments["und"][] = (array) $file;
  	} 

	  $attachment_node = node_submit($attachment_node); // Prepare node for saving
	  node_save($attachment_node);

		grievance_mailattach_fetch_accept($from_mail, t("Your email has been added as an attachment to: ") . $grievance_node->title);

	}

	// Delete all the messages
	foreach ($messages as $message) {
		$msgno = $message->msgno;
		imap_delete($mbox, $msgno);
	}

	// Expunge any deleted messages
	imap_expunge($mbox);

	// Done with imap
	imap_close($mbox);
}

function grievance_mailattach_fetch_reject($from_mail, $msg) {
	$params = array();
	$params['subject'] = "Attachment rejected";
	$params['body'] =  "Your email to $grievance_mailattach_user was rejected: $msg";
	drupal_mail('grievance', 'mailattach', $from_mail, LANGUAGE_NONE, $params);
	// print "Failing with $msg\n";
}

function grievance_mailattach_fetch_accept($from_mail, $msg) {
	$params = array();
	$params['subject'] = "Attachment accepted";
	$params['body'] =  "Your email was accepted: $msg";
	drupal_mail('grievance', 'mailattach', $from_mail, LANGUAGE_NONE, $params);
	// print "Failing with $msg\n";
}

function grievance_imap_fetch_all_parts($mbox, $msgno, $structure, $path = NULL) {
	$attachments = array(); 

	if (isset($structure->parts) && count($structure->parts)) {
		$i = 0;
		foreach ($structure->parts as $part) {
			++$i;
			$sub_path = $path;
			if ($sub_path) { $sub_path .= '.'; }
			$sub_path .= $i;
			$attachments = array_merge($attachments, grievance_imap_fetch_all_parts($mbox, $msgno, $part, $sub_path));
		}
	} else {
		if (!$path) { $path = 1; }
		$attachment = array();

		if($structure->dparameters) {
			foreach($structure->dparameters as $object) {
				if(strtolower($object->attribute) == 'filename') {
					$attachment['is_attachment'] = true;
					$attachment['filename'] = $object->value;
				}
			}
		}

		if($structure->ifparameters) {
			foreach($structure->parameters as $object) {
				if(strtolower($object->attribute) == 'name') {
					$attachment['is_attachment'] = true;
					$attachment['name'] = $object->value;
				}
			}
		}

		$encoded = imap_fetchbody($mbox, $msgno, $path);
		if($structure->encoding == 3) { // 3 = BASE64
			$attachment['bindata'] = base64_decode($encoded);
		}
		elseif($structure->encoding == 4) { // 4 = QUOTED-PRINTABLE
			$attachment['bindata'] = quoted_printable_decode($encoded);
		} else {
			$attachment['bindata'] = $encoded;
		}

		$attachments[] = $attachment;
	}

	return $attachments;
}


