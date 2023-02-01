<?php

function sirius_steward_sched_access($action, $employer_nid) {
	// Must have an employer
	if (!$employer_nid) { return FALSE; }

	// Admins can do anything
	if (user_access('sirius steward admin')) { return TRUE; }
	if (user_access('sirius employer admin')) { return TRUE; }
	if (user_access('sirius domain admin')) { return TRUE; }
	if (user_access('sirius admin')) { return TRUE; }


	// Employers can view their own record
	if ($action == 'view') {
		if (user_access('sirius steward sched employer')) { 
			global $user;
			$user_full = user_load($user->uid);
			if ($user_full->field_grievance_shops['und'][0]['target_id'] == $employer_nid) { return TRUE; }
		}
	}

	// Workers can access this page for their own employer only
	if (user_access('sirius steward sched worker')) {
		$my_worker = sirius_worker_get();
		$my_employer_nid = $my_worker->field_grievance_shop['und'][0]['target_id'];
		if ($my_employer_nid == $employer_nid) { return TRUE; }
	}

	// Oh well.
	return FALSE;
}
