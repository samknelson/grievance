<?php

function sirius_worker_page_employer_worker_list_access($member_status_tid = NULL) {
	if (!user_access('sirius dispatch employer')) { return FALSE; }

	if (!$member_status_tid) { return TRUE; }

	$open_statuses = sirius_worker_member_status_get_open_statuses();
	if ($open_statuses[$member_status_tid]) { return TRUE; }
	return FALSE; 
}

function sirius_worker_page_employer_worker_list($member_status_tid = NULL) {

	$title = '';

	if (!$member_status_tid) {
		$title = t('Current Employed List');
		global $user;
		$user_full = user_load($user->uid);
		$shop_nid = $user_full->field_grievance_shops['und'][0]['target_id'];
		$result .= views_embed_view('sirius_employer_workers', 'block_1', $shop_nid);
	} else {
		$member_status_term = taxonomy_term_load($member_status_tid);
		$title = t('Out of Work List') . ' - ' . $member_status_term->name;
		$result .= views_embed_view('sirius_employer_workers', 'block', $member_status_tid);
	}

	drupal_set_title($title);

	return $result;
}

function sirius_worker_page_employer_worker_list_ineligible_access() {
	if (!user_access('sirius dispatch employer')) { return FALSE; }
	return TRUE;
}

function sirius_worker_page_employer_worker_list_ineligible() {

	$title = t('Employed but Ineligible');
	drupal_set_title($title);

	global $user;
	$user_full = user_load($user->uid);
	$shop_nid = $user_full->field_grievance_shops['und'][0]['target_id'];
	$result .= views_embed_view('sirius_employer_workers_ineligible', 'block_1', $shop_nid);

	return $result;
}