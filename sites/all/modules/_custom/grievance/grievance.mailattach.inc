<?php

function grievance_mailattach_fetch() {
	// Get our connection information
	$grievance_mailattach_user = 'grievance1-incoming@unitehere.org';
	$grievance_mailattach_pass = 'Local1!';
	$grievance_mailattach_imapstr = '{uhexch00.unitehere.org:993/imap/ssl}INBOX';

	// Connect
	$mbox = imap_open($grievance_mailattach_imapstr, $grievance_mailattach_user, $grievance_mailattach_pass);
	if (!$mbox) { 
		watchdog('grievance_mailattach', "Unable to connect to $grievance_mailattach_imapstr as $grievance_mailattach_user: " . imap_last_error());
	}

	// Get the number of messages
	$MC = imap_check($mbox);

	// Get the list of messages
	$messages = imap_fetch_overview($mbox,"1:{$MC->Nmsgs}",0);

	// For each message
	foreach ($messages as $message) {

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
			grievance_mailattach_fetch_reject($from_mail, "The sender's email address ($from_email) does not correspond to a valid system user.");
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
		$grievance_id = $subject;
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
			grievance_mailattach_fetch_reject($from_mail, "Unable to determine which grievance you want to use (couldn't find a grievance with id [$grievance_id].  Make sure to put the ID number at the very beginning of the subject of your emails.");
			continue;
		}

		//
		// Get the structure of the message
		//

		$msgno = $message->msgno;
		$structure = imap_fetchstructure($mbox, $msgno);

		if (!is_array($structure->parts)) { continue; }

		$attachments = array();
		if(isset($structure->parts) && count($structure->parts)) {
			for($i = 0; $i < count($structure->parts); $i++) {

				$attachments[$i] = array(
					'is_attachment' => false,
					'filename' => '',
					'name' => '',
					'attachment' => ''
				);
				
				if($structure->parts[$i]->ifdparameters) {
					foreach($structure->parts[$i]->dparameters as $object) {
						if(strtolower($object->attribute) == 'filename') {
							$attachments[$i]['is_attachment'] = true;
							$attachments[$i]['filename'] = $object->value;
						}
					}
				}
				
				if($structure->parts[$i]->ifparameters) {
					foreach($structure->parts[$i]->parameters as $object) {
						if(strtolower($object->attribute) == 'name') {
							$attachments[$i]['is_attachment'] = true;
							$attachments[$i]['name'] = $object->value;
						}
					}
				}
				
				if($attachments[$i]['is_attachment']) {
					$attachments[$i]['attachment'] = imap_fetchbody($mbox, $msgno, $i+1);
					if($structure->parts[$i]->encoding == 3) { // 3 = BASE64
						$attachments[$i]['attachment'] = base64_decode($attachments[$i]['attachment']);
					}
					elseif($structure->parts[$i]->encoding == 4) { // 4 = QUOTED-PRINTABLE
						$attachments[$i]['attachment'] = quoted_printable_decode($attachments[$i]['attachment']);
					}
				}
			}
		}

		foreach ($attachments as &$attachment) {
			$filename = $attachment['filename'];
			if (!$filename) { $filename = 'body.txt'; }
			$filename = $msgno . "-" . $filename;
			file_put_contents('/var/www/vhosts/grievances.clevernamehere.com/httpdocs/defunct/' . $filename, $attachment['attachment']);
			print "Printed to $filename\n";
		}
		continue;




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
	// drupal_mail('grievance', 'mailattach', $from_mail, LANGUAGE_NONE, $params);
	print "Failing with $msg\n";
}


function grievance_mailattach_fetch_purge($message) {

}