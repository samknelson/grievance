<?php

/** 
 * Legacy support. There are calls to the old scheme that are problematic to update at the moment.
 */

function sirius_dispatch_job_get_dispatchable_workers_explain_by_worker_id($job_node, $worker_id, $filters = array()) {
	return sirius_dispatch_elig_explain_by_worker_id($job_node, $worker_id, $filters);
}

function sirius_dispatch_job_get_dispatchable_workers_explain($job_node, $worker_nid, $filters = array()) {
	return sirius_dispatch_elig_explain($job_node, $worker_nid, $filters);
}


function sirius_dispatch_elig_explain_by_worker_id($job_node, $worker_id, $filters = array()) {
	if (!$worker_id) {
		return array('success' => FALSE, 'msg' => 'Please enter a worker ID.');
	}
	$sql = "select entity_id from field_data_field_sirius_id where entity_type = 'node' and bundle = 'sirius_worker' and field_sirius_id_value = :id";
	$stmt = db_query($sql, array(':id' => $worker_id));
	$hr = $stmt->fetchAssoc();
	$worker_nid = $hr['entity_id'];
	if (!$worker_nid) {
		return array('success' => FALSE, 'msg' => "The worker with dispatch ID #$worker_id was not found.");
	}

	return sirius_dispatch_job_get_dispatchable_workers_explain($job_node, $worker_nid, $filters);
}


function sirius_dispatch_elig_explain($job_node, $worker_nid, $filters = array()) {
	$job_policy = sirius_dispatch_job_policy($job_node);

	// Get dispatchable work statuses
	$all_work_statuses = sirius_taxonomy_load('sirius_work_status', 'tid', 'full');
	$work_status_tids = array();
	foreach ($all_work_statuses as $tid => $term) {
		if ($term->field_sirius_dispatch_available['und'][0]['value'] == 'Yes') {
			$work_status_tids[] = $tid;
		}
	}
	if (!$work_status_tids) { return array('success' => FALSE, 'msg' => t('There is no work status specified for this job.')); }

	// Get the industry TID
	$industry_tid = $job_node->field_sirius_industry['und'][0]['tid'];
	if (!$industry_tid) { return array('success' => FALSE, 'msg' => t('There is no industry specified for this job.')); }
	$industry_proxy = sirius_taxonomy_proxy($industry_tid);

	// Get the dispatchable member statuses for this job.
	$member_status_tids = array();
	if ($job_node->field_sirius_member_status['und'][0]['tid']) {
		foreach ($job_node->field_sirius_member_status['und'] as $tmp) {
			$member_status_tids[] = $tmp['tid'];
		}
	}
	if (!$member_status_tids) { return array('success' => FALSE, 'msg' => t('There is no member status specified for this job.')); }

	$worker_node = node_load($worker_nid);
	$employer_nid = $job_node->field_grievance_shop['und'][0]['target_id'];
	$employer_node = node_load($employer_nid);

	// Get the facility NID
	$facility_nid = $job_node->field_sirius_dispatch_facility['und'][0]['target_id'];

	// Exclude current dispatches
	if (!$filters['redispatch'] && !$filters['ignore_redispatch']) {
		$sql = "select count(*) as c from ";
		$sql .= "node, ";
		$sql .= "field_data_field_sirius_worker, ";
		$sql .= "field_data_field_sirius_dispatch_job ";
		$sql .= "where node.type = 'sirius_dispatch' ";
		$sql .= "and field_data_field_sirius_worker.entity_type = 'node' and field_data_field_sirius_worker.entity_id = node.nid ";
		$sql .= "and field_data_field_sirius_dispatch_job.entity_type = 'node' and field_data_field_sirius_dispatch_job.entity_id = node.nid ";
		$sql .= "and field_data_field_sirius_dispatch_job.field_sirius_dispatch_job_target_id = :job_nid ";
		$sql .= "and field_data_field_sirius_worker.field_sirius_worker_target_id = :worker_nid ";
		$stmt = db_query($sql, array(':job_nid' => $job_node->nid, ':worker_nid' => $worker_nid));
		$hr = $stmt->fetchAssoc();
		if ($hr['c']) {
			return array('success' => FALSE, 'msg' => t('There is already a dispatch for this worker and this job.'));
		}
	}

	// Rollover
	if ($job_policy['workers']['must_be_rollover'] == 'Yes') {
		$roll_cutoff = strtotime('-7 days');
		$sql = "  select nid "; 
		$sql .= "  from node roll_n, ";
		$sql .= "  field_data_field_sirius_log_handler roll_h1, ";
		$sql .= "  field_data_field_sirius_log_handler roll_h2, ";
		$sql .= "  field_data_field_sirius_category roll_c ";
		$sql .= "  where roll_n.type = 'sirius_log' ";
		$sql .= "  and roll_n.created >= :roll_cutoff ";
		$sql .= "  and roll_h1.entity_type = 'node' and roll_h1.entity_id = roll_n.nid and roll_h1.field_sirius_log_handler_target_id = :worker_nid ";
		$sql .= "  and roll_h2.entity_type = 'node' and roll_h2.entity_id = roll_n.nid and roll_h2.field_sirius_log_handler_target_id = :roll_employer_nid ";
		$sql .= "  and roll_c.entity_type = 'node' and roll_c.entity_id = roll_n.nid and roll_c.field_sirius_category_value = 'dispatch:rollover' ";
		$args = array();
		$args[':roll_employer_nid'] = $employer_nid;
		$args[':roll_cutoff'] = $roll_cutoff;
		$args[':worker_nid'] = $worker_nid;
		$stmt = db_query($sql, $args);
		$hr = $stmt->fetchAssoc();
		if (!$hr['nid']) {
			return array('success' => FALSE, 'msg' => t('There is no rollover entry to this employer in the last 7 days.'));
		}
	}

	// Limit by member status
	if (!array_intersect(sirius_fieldvals($worker_node, 'field_sirius_member_status', 'tid'), $member_status_tids)) {
		return array('success' => FALSE, 'msg' => t('The worker does not have one of the member statuses required.'));
	}

	// Limit by work status
	if (!array_intersect(sirius_fieldvals($worker_node, 'field_sirius_work_status', 'tid'), $work_status_tids)) {
		return array('success' => FALSE, 'msg' => t('The worker does not have a work status that is eligible for dispatch.'));
	}

	// Limit by industry.  All jobs need an industry
	if (!in_array($job_node->field_sirius_industry['und'][0]['tid'], sirius_fieldvals($worker_node, 'field_sirius_industry', 'tid'))) {
		return array('success' => FALSE, 'msg' => t('This worker is not able to work in the industry for this job.'));
	}

	$skill_tid = $job_node->field_sirius_skill['und'][0]['tid'];
	if (!$filters['ignore_skill']) {
		$skill_tids = sirius_fieldvals($job_node, 'field_sirius_skill', 'tid');
		if ($skill_tids) {
			$my_skill_tids = sirius_fieldvals($worker_node, 'field_sirius_skill', 'tid');
			$my_skill_expires = sirius_fieldvals($worker_node, 'field_sirius_skill_expire', 'value');
			foreach ($skill_tids as $delta => $skill_tid) {
				$delta = array_search($skill_tid, $my_skill_tids);
				if ($delta === FALSE) {
					$skill_term = taxonomy_term_load($skill_tid);
					return array('success' => FALSE, 'msg' => t('The worker does not have the skill @skill_name.', array('@skill_name' => $skill_term->name)));
				}
				if ($my_skill_expires[$delta] <= date('Y-m-d 00:00:00')) {
					$skill_term = taxonomy_term_load($skill_tid);
					return array('success' => FALSE, 'msg' => t('The skill @skill_name expired on @date.', array(
						'@skill_name' => $skill_term->name,
						'@date' => $my_skill_expires[$delta],
					)));
				}
			}
		}
	}

	if (!$filters['ignore_skill'] && ($job_policy['workers']['ignore_prefs_skill'] != 'Yes')) {
		// Limit by skill avail -- which could be null, meaning all skills
		$skills_avail = sirius_fieldvals($worker_node, 'field_sirius_skills_avail', 'tid');

		$ok = TRUE;
		if ($skill_tid && $skills_avail && !in_array($skill_tid, $skills_avail)) {
			$ok = FALSE;
		}

		if (!$skill_tid && $worker_node->field_sirius_skills_availx['und'][0]['value'] == 'No') {
			$ok = FALSE;
		}

		if (!$ok) {
			if (!$skills_avail) {
				$msg = "The worker has set a preference for only skilled jobs.";
			} else {
				$tmp = array();
				foreach ($skills_avail as $term_tid) { $tmp[] = sirius_term_title($term_tid); }
				$msg = t("The worker has set a preference for skills: ") . join(', ', $tmp);
				if (strlen($msg) > 250) { $msg = substr($msg, 0, 245) . ' ...'; }
			}
			return array('success' => FALSE, 'msg' => $msg);
		}
	}

	// Limit by department avail -- which could be null, meaning all departments
	if (!$filters['ignore_department'] && ($job_policy['workers']['ignore_prefs_dept'] != 'Yes')) {
		$department_tid = $job_node->field_grievance_departments['und'][0]['tid'];
		if ($department_tid) {
			$department_tids = sirius_fieldvals($worker_node, 'field_grievance_departments', 'tid');
			if (in_array($department_tid, $department_tids)) {
				return array('success' => FALSE, 'msg' => t('The worker has set a preference to NOT receive calls from the department selected for this job.'));
			}
		} 
	}

	// Dispatch statuses: not employed / available
	$dispatch_statuses = array();
	$dispatch_statuses[] = sirius_get_special_tid('sirius_worker_dispatch_status|not_employed_available');
	if ($job_policy['workers']['may_be_unavailable'] == 'Yes') {
		$dispatch_statuses[] = sirius_get_special_tid('sirius_worker_dispatch_status|not_employed_not_available');
	}
	if ($job_policy['workers']['may_be_employed'] == 'Yes') {
		$dispatch_statuses[] = sirius_get_special_tid('sirius_worker_dispatch_status|employed');
	}

	if (!$filters['ignore_dispatch_status']) {
		$dispstatus_tid = sirius_fieldval($worker_node, 'field_sirius_worker_dispstatus', 'tid');
		if (!in_array($dispstatus_tid, $dispatch_statuses)) {
			if ($job_node->field_sirius_dispatch_eba['und'][0]['value'] != 'Yes') {
				return array('success' => FALSE, 'msg' => t('The worker is not signed in to the out of work list.'));
			}

			if ($dispstatus_tid != sirius_get_special_tid('sirius_worker_dispatch_status|employed')) {
				return array('success' => FALSE, 'msg' => t('The worker is not signed in to the out of work list, and is not currently employed.'));
			}

			if (sirius_fieldval($worker_node, 'field_sirius_dispatch_eba') != 'Yes') {
				return array('success' => FALSE, 'msg' => t('The worker is employed, and is not employed-but-available.'));
			}

			$worker_eba_dates = sirius_fieldvals($worker_node, 'field_sirius_dispatch_eba_dates');
			$job_eba_dates = sirius_fieldvals($job_node, 'field_sirius_dispatch_eba_dates');
			foreach ($job_eba_dates as $date) {
				if (!in_array($date, $worker_eba_dates)) {
					return array('success' => FALSE, 'msg' => t('The worker is currently employed and accepting EBA calls, but is not available to work on @date.', array('@date' => date('Y-m-d', strtotime($date)))));
				}
			}
		}
	}

	$event_nids = sirius_fieldvals($job_node, 'field_sirius_events', 'target_id');
	if ($event_nids && !$filters['ignore_event']) {
		$event_node = node_load($event_nids[0]);
		$event_type_term = taxonomy_term_load($event_node->field_sirius_event_type['und'][0]['tid']);
		$json = json_decode($event_type_term->field_sirius_json['und'][0]['value'], TRUE);
		list ($prole_tid, $pstatus_tid) = explode('|', $json['bullpen']['pre_dispatch']);
		$sql = "select count(*) as c from "; 
		$sql .= "field_data_field_sirius_contact worker_contact, ";
		$sql .= "field_data_field_sirius_contact participant_contact, ";
		$sql .= "field_data_field_sirius_event, ";
		$sql .= "field_data_field_sirius_event_prole, ";
		$sql .= "field_data_field_sirius_event_pstatus ";

		$sql .= "where worker_contact.entity_type = 'node' ";
		$sql .= "and worker_contact.entity_id = :worker_nid ";

		$sql .= "and participant_contact.entity_type = 'node' ";
		$sql .= "and participant_contact.field_sirius_contact_target_id = worker_contact.field_sirius_contact_target_id ";
		$sql .= "and participant_contact.bundle = 'sirius_event_participant' ";

		$sql .= "and field_data_field_sirius_event.entity_type = 'node' ";
		$sql .= "and field_data_field_sirius_event.entity_id = participant_contact.entity_id ";
		$sql .= "and field_data_field_sirius_event.field_sirius_event_target_id in (:event_nids) ";

		$sql .= "and field_data_field_sirius_event_prole.entity_type = 'node' ";
		$sql .= "and field_data_field_sirius_event_prole.entity_id = participant_contact.entity_id ";
		$sql .= "and field_data_field_sirius_event_prole.field_sirius_event_prole_tid = :prole_tid ";

		$sql .= "and field_data_field_sirius_event_pstatus.entity_type = 'node' ";
		$sql .= "and field_data_field_sirius_event_pstatus.entity_id = participant_contact.entity_id ";
		$sql .= "and field_data_field_sirius_event_pstatus.field_sirius_event_pstatus_tid = :pstatus_tid ";
		$args = array();
		$args[':worker_nid'] = $worker_nid;
		$args[':event_nids'] = $event_nids;
		$args[':prole_tid'] = $prole_tid;
		$args[':pstatus_tid'] = $pstatus_tid;
		$stmt = db_query($sql, $args);
		$hr = $stmt->fetchAssoc();
		if (!$hr['c']) {
			return array('success' => FALSE, 'msg' => t('The worker is not currently signed in to the bullpen associated with this job.'));
		}
	}

	// Exclude employer-initiated DNCs
	if ($employer_nid && in_array($employer_nid, sirius_fieldvals($worker_node, 'field_sirius_dispatch_dncs', 'target_id'))) {
		return array('success' => FALSE, 'msg' => t('The worker has been placed on the do-not-call list for this employer.'));
	}

	// Exclude facility DNCs
	if ($facility_nid && in_array($facility_nid, sirius_fieldvals($worker_node, 'field_sirius_dispatch_dncs', 'target_id'))) {
		return array('success' => FALSE, 'msg' => t('The worker has been placed on the do-not-call list for this facility.'));
	}

	// Exclude industry DNCs
	if ($industry_proxy && in_array($industry_proxy->nid, sirius_fieldvals($worker_node, 'field_sirius_dispatch_dncs', 'target_id'))) {
		return array('success' => FALSE, 'msg' => t('The worker has been placed on the do-not-call list for this industry.'));
	}

	// Exclude employee-initiated DNCs
	if ($job_policy['workers']['ignore_prefs_dnc'] != 'Yes') {
		if ($employer_nid && in_array($employer_nid, sirius_fieldvals($worker_node, 'field_sirius_dispatch_dncs2', 'target_id'))) {
			return array('success' => FALSE, 'msg' => t('The worker has placed a worker-initiated do-not-call on this employer.'));
		}
	}

	// CBN
	if (!$filters['ignore_cbn'] && ($job_policy['workers']['ignore_prefs_cbn'] != 'Yes')) {
		if ($filters['cbn'] == 'No') {
			if (sirius_fieldval($worker_node, 'field_sirius_dispatch_cbn', 'value') == 'Yes') {
				return array('success' => FALSE, 'msg' => t('The worker has requested to only receive call-by-names.'));
			}
		}
	}

	// HFE
	if (!$filters['ignore_hfe'] && ($job_policy['workers']['ignore_prefs_hfe'] != 'Yes')) {
		$hfe = sirius_fieldval($worker_node, 'field_sirius_dispatch_hfe', 'target_id');
		$hfe_ts = sirius_fieldval($worker_node, 'field_sirius_dispatch_hfe_until', 'ts');
		if ($employer_nid && $hfe && $hfe != $employer_nid && $hfe_ts > time()) {
			return array('success' => FALSE, 'msg' => t('The worker is holding for a call from a different employer. The hold expires on @date', array('@date' => date('Y-m-d H:i:s', $hfe_ts))));
		}
	}

	// Cached ineligibility
	if (!$filters['ignore_inelig_cache'] && $job_node->nid) {
		$sql = "select * from sirius_dispatch_elig_cache ";
		$sql .= "where sirius_dispatch_elig_cache.job_nid = :job_nid ";
		$sql .= "and sirius_dispatch_elig_cache.worker_nid = :worker_nid ";
		$args = array();
		$args[':worker_nid'] = $worker_nid;
		$args[':job_nid'] = $job_node->nid;
		$stmt = sirius_sql_query($sql, $args);
		$hr = $stmt->fetchAssoc();
		if ($hr) {
			$plugin = sirius_dispatch_elig_info($hr['plugin']);
			return array(
				'success' => FALSE, 
				'msg' => t(
					'The worker has been flagged as ineligible for this job because of the eligibility plugin @plugin: @details', array(
						'@plugin' => $plugin['name'],
						'@details' => $hr['details']
					)
				)
			);
		}
	}

	// Plugins
	$context = array();
	$context['job_node'] = $job_node;
	$context['job_policy'] = $job_policy;
	$context['order'] = $order;
	$context['limit'] = $limit;
	$context['filters'] = $filters;

	$workers = array($worker_nid => array());

	$plugins = sirius_dispatch_elig_info_all();
	foreach ($plugins as $plugin_key => $plugin) {
		if ($job_policy['plugin'][$plugin_key]['active'] != 'Yes') { continue; }

		if ($filters['ignore_plugins'][$plugin_key]) { continue; }

		$plugin = sirius_dispatch_elig_instance($plugin_key);
		$result = $plugin->elig($context, $workers);
		if (!$result['success']) { continue; }

		// Handle workers that are marked "ineligible";
		if ($result['inelig_workers'][$worker_nid]) {
			return array(
				'success' => FALSE, 
				'msg' => t(
					'The worker failed the eligibility plugin @plugin: @details', array(
						'@plugin' => $plugin->name,
						'@details' => $result['inelig_workers'][$worker_nid]['details'],
					)
				)
			);
		}
	}

	// Workers
	$filters['skip_plugins'] = TRUE;
	$workers = sirius_dispatch_elig_fetch($job_node, 'dispatch', 0, $filters);
	$position = 0;
	foreach ($workers as $probe_nid => $ignore) {
		++$position;
		if ($probe_nid == $worker_node->nid) { $found = TRUE; break; }
	}
	$total = count($workers);

	if (!$found) {
		return array('success' => FALSE, 'msg' => t('The worker passed all the tests, but does not appear on the dispatchable worker list for this job. Please contact the system administrator.'));
	}

	return array(
		'success' => TRUE, 
		'msg' => t('The worker may be dispatched and is at position #@position / @total', array('@position' => $position, '@total' => $total)),
		'position' => $position,
		'total' => $total
	);
}
