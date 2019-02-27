<?php

/*
 * Employer-initiated DNCs
 */

function sirius_worker_skill_check($worker_node, $skill_term) {
	if (!$worker_node->field_sirius_skill['und']) { return FALSE; }

	foreach ($worker_node->field_sirius_skill['und'] as $tmp) {
		if ($tmp['tid'] == $skill_term->tid) { return TRUE; }
	}

	return FALSE;
}

function sirius_worker_skill_set($worker_node, $skill_term, $tf, $reason, $attachments) {
	if ($tf) {
		if (!sirius_worker_skill_check($worker_node, $skill_term)) {
			$worker_node->field_sirius_skill['und'][]['tid'] = $skill_term->tid;
			$need_save = TRUE;
		}
	}

	if (!$tf) {
		// Remove from the DNC list
		$to_clear = array();
		foreach ($worker_node->field_sirius_skill['und'] as $delta => $tmp) {
			if ($tmp['tid'] == $skill_term->tid) { $to_clear[] = $delta; }
		}
		if ($to_clear) {
			foreach ($to_clear as $delta) {
				unset($worker_node->field_sirius_skill['und'][$delta]);
			}
			$worker_node->field_sirius_skill[LANGUAGE_NONE] = array_values($worker_node->field_sirius_skill[LANGUAGE_NONE]);
			$need_save = TRUE;
		}
	}

	if ($need_save) {
		node_save($worker_node);
	}

	$category = 'worker:skill';
	if ($tf) { 
		$type = 'set';
		$message = $worker_node->title . ' has added the skill ' . $skill_term->name;
	}
	else {
		$type = 'clear';
		$message = $worker_node->title . ' has removed the skill ' . $skill_term->name;
	}
	$summary = $reason;
	$notes = $skill_term->tid;
	$handler_nids = array($worker_node->nid);
	sirius_log($category, $type, $message, $summary, $notes, $handler_nids, $attachments);
}
