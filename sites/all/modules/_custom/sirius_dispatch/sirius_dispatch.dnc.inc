<?php

/*
 * Employer-initiated DNCs
 */

function sirius_dispatch_dnc_check($worker_node, $employer_node) {
	if (!$worker_node->field_sirius_dispatch_dncs['und']) { return FALSE; }

	foreach ($worker_node->field_sirius_dispatch_dncs['und'] as $tmp) {
		if ($tmp['target_id'] == $employer_node->nid) { return TRUE; }
	}

	return FALSE;
}

function sirius_dispatch_dnc_set($worker_node, $employer_node, $tf, $reason, $attachments) {
	if ($tf) {
		// Add to the DNC list
		if (!sirius_dispatch_dnc_check($worker_node, $employer_node)) {
			$worker_node->field_sirius_dispatch_dncs['und'][]['target_id'] = $employer_node->nid;
			$need_save = TRUE;
		}
	}

	if (!$tf) {
		// Remove from the DNC list
		$to_clear = array();
		foreach ($worker_node->field_sirius_dispatch_dncs['und'] as $delta => $tmp) {
			if ($tmp['target_id'] == $employer_node->nid) { $to_clear[] = $delta; }
		}
		if ($to_clear) {
			foreach ($to_clear as $delta) {
				unset($worker_node->field_sirius_dispatch_dncs['und'][$delta]);
			}
			$worker_node->field_sirius_dispatch_dncs[LANGUAGE_NONE] = array_values($worker_node->field_sirius_dispatch_dncs[LANGUAGE_NONE]);
			$need_save = TRUE;
		}
	}

	if ($need_save) {
		global $base_url;
		if ($tf) {
			$subject = "You have been added to the Do Not Call list for " . $employer_node->title;
			$body = "At the request of " . $employer_node->title . ", you have been placed on the Do Not Call list for this employer. The reason is provided below. ";
			$body .= "If you have any questions, please contact the office.";
			$body .= "<br /><br />";
			$body .= "To see the DNC record and any supporting information, please sign into your account at ";
			$body .= "<a href=\"$base_url\">$base_url</a>";
			$body .= "<hr><strong>Reason: </strong>";
			$body .= $reason;
		} else {
			$subject = "You have been removed from the Do Not Call list for " . $employer_node->title;
			$body = "At the request of " . $employer_node->title . ", you have been removed from the Do Not Call list for this employer. The reason is provided below. ";
			$body .= "If you have any questions, please contact the office. ";
			$body .= "<br /><br />";
			$body .= "To see the DNC record and any supporting information, please sign into your account at ";
			$body .= "<a href=\"$base_url\">$base_url</a>";
			$body .= "<hr><strong>Reason: </strong>";
			$body .= $reason;
		}

		sirius_email_send(
			array(
				'to' => $worker_node->field_sirius_email['und'][0]['value'],
				'subject' => $subject,
				'body' => $body,
				'handler_nids' => array($worker_node->nid)
			)
		);

		node_save($worker_node);
	}

	$category = 'dispatch:dnc';
	if ($tf) { 
		$type = 'set';
		$message = $worker_node->title . ' has been added to the DNC list for ' . $employer_node->title;
	}
	else {
		$type = 'clear';
		$message = $worker_node->title . ' has been removed from the DNC list for ' . $employer_node->title;
	}
	$summary = $reason;
	$handler_nids = array($worker_node->nid, $employer_node->nid);
	sirius_log($category, $type, $message, $summary, $notes, $handler_nids, $attachments);
}

function sirius_dispatch_dnc2_check($worker_node, $employer_node) {
	if (!$worker_node->field_sirius_dispatch_dncs2['und']) { return FALSE; }

	foreach ($worker_node->field_sirius_dispatch_dncs2['und'] as $tmp) {
		if ($tmp['target_id'] == $employer_node->nid) { return TRUE; }
	}

	return FALSE;
}

function sirius_dispatch_dnc2_set($worker_node, $employer_node, $tf, $reason, $attachments) {
	if ($tf) {
		// Add to the DNC list
		if (!sirius_dispatch_dnc2_check($worker_node, $employer_node)) {
			$worker_node->field_sirius_dispatch_dncs2['und'][]['target_id'] = $employer_node->nid;
			$need_save = TRUE;
		}
	}

	if (!$tf) {
		// Remove from the DNC list
		$to_clear = array();
		foreach ($worker_node->field_sirius_dispatch_dncs2['und'] as $delta => $tmp) {
			if ($tmp['target_id'] == $employer_node->nid) { $to_clear[] = $delta; }
		}
		if ($to_clear) {
			foreach ($to_clear as $delta) {
				unset($worker_node->field_sirius_dispatch_dncs2['und'][$delta]);
			}
			$worker_node->field_sirius_dispatch_dncs2[LANGUAGE_NONE] = array_values($worker_node->field_sirius_dispatch_dncs2[LANGUAGE_NONE]);
			$need_save = TRUE;
		}
	}

	if ($need_save) {
		node_save($worker_node);
	}

	$category = 'dispatch:dnc2';
	if ($tf) { 
		$type = 'set';
		$message = $worker_node->title . ' has been added to the DNC2 list for ' . $employer_node->title;
	}
	else {
		$type = 'clear';
		$message = $worker_node->title . ' has been removed from the DNC2 list for ' . $employer_node->title;
	}
	$summary = $reason;
	$handler_nids = array($worker_node->nid, $employer_node->nid);
	sirius_log($category, $type, $message, $summary, $notes, $handler_nids, $attachments);
}
