<?php 

function sirius_dispatch_page_employer_job_list() {
	global $user;

	drupal_set_title(t('Job Requests'));

	$user_full = user_load($user->uid);
	$shop_nid = $user_full->field_grievance_shops['und'][0]['target_id'];
	if (!$shop_nid) {
		return t('No employer found. This probably means that the user has been misconfigured, and has not been assigned to a shop. Please contact the system administrator. (No field_grievance_shops).');
	}
	return views_embed_view('sirius_employer_job_list', 'block', $shop_nid);
}