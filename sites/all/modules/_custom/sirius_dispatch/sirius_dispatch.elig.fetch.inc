<?php

function sirius_dispatch_elig_fetch($job_node, $order = 'name', $limit = 0, $filters = array()) {
	sirius_minilog("Fetch starting; fetching $limit workers.");

	// What is our job policy?
	$job_policy = sirius_dispatch_job_policy($job_node);

	// Set the context. This will be passed to all the eligibility plugins.
	$plugins = sirius_dispatch_elig_info_all();
	$context = array();
	$context['job_node'] = $job_node;
	$context['job_policy'] = $job_policy;
	$context['order'] = $order;
	$context['limit'] = $limit;
	$context['filters'] = $filters;

	// We repeatedly fetch workers until we have enough.
	$done = FALSE;
	$offset = $no_infinity = 0;
	$workers = $inelig_workers = array();
	while (!$done) {
		// Just in case
		++$no_infinity; 
		if ($no_infinity > 500) {
			watchdog('sirius_dispatch', 'Infinite loop in sirius_dispatch_elig_fetch. Call the developer.'); 
			break;
		}
		sirius_minilog("Starting iteration #$no_infinity ...");

		// Try to get $limit new workers
		$new_limit = $limit - count($workers);
		sirius_minilog("... Fetching up to $new_limit workers with offset $offset");
		$new_workers = sirius_dispatch_elig_fetch_query($job_node, $order, $new_limit, $filters, $offset);
		sirius_minilog("... Found " . count($new_workers) . " candidates.");

		// Skip plugins?
		if ($filters['skip_plugins']) { 
			$workers = $new_workers;
			break;
		}

		// Iterate through all the plugins.
		$new_inelig_workers = array();
		foreach ($plugins as $plugin_key => $plugin) {
			// Only active plugins with an eligibility callback
			if ($job_policy['plugin'][$plugin_key]['active'] != 'Yes') { continue; }
			$plugin = sirius_dispatch_elig_instance($plugin_key);

			// Attempt to run the plugin
			$result = $plugin->elig($context, $new_workers);
			if (!$result['success']) { continue; }

			// If we found ineligible workers, remove them from our results...
			if ($result['inelig_workers']) {
				foreach ($result['inelig_workers'] as $worker_nid => $ignore) {
					$result['inelig_workers'][$worker_nid]['plugin_key'] = $plugin_key;
				}
				$new_inelig_workers += $result['inelig_workers'];
			}
		}

		// Remove any newly ineligible workers from our results, and add them to our running totals
		if ($new_inelig_workers) {
			$inelig_workers += $new_inelig_workers;
			foreach ($new_inelig_workers as $worker_nid => $inelig_record) {
				unset($new_workers[$worker_nid]);
			}
		}

		// Increment our total workers list
		$workers += $new_workers;
		sirius_minilog("... Of the new workers, " . count($new_inelig_workers) . " were ineligible. We now have " . count($new_workers) . " new workers, and " . count($workers) . " total workers.");

		// Are we done yet?
		if (!$limit) {
			sirius_minilog("... Fetched all possible workers");
			$done = TRUE;
		} else if (count($workers) >= $limit) {
			sirius_minilog("... We found enough workers.");
			$done = TRUE;
		} else if (!$new_inelig_workers) {
			sirius_minilog("... We didn't find enough workers, but none of the new ones were ineligible, so we have to give up.");
			$done = TRUE;
		} else {
			sirius_minilog("... We didn't find enough workers yet. We'll keep trying.");
			$done = FALSE;
			$offset = count($workers) + count($inelig_workers);
		}
	}

	// Cache ineligible workers, so that the next time we're called, we won't try to fetch them.
	if ($filters['set_eligibility_cache']) {
		foreach ($inelig_workers as $worker_nid => $inelig_record) {
			if (!$inelig_record['cache']) { continue; }

			sirius_dispatch_elig_cache_set(
				$job_node->nid, 
				$worker_nid, 
				$inelig_record['plugin_key'], 
				$inelig_record['details']
			);
		}
	}

	return $workers;
}

/**
 * This is a ginormous query to fetch the next set of eligible workers.
 * 
 * @todo: Many of the clauses in this query should really be converted into eligibility plugins.
 */

function sirius_dispatch_elig_fetch_query($job_node, $order = 'name', $limit = 0, $filters = array(), $offset = 0) {
	$job_policy = sirius_dispatch_job_policy($job_node);

	// Get dispatchable work statuses
	$all_work_statuses = sirius_taxonomy_load('sirius_work_status', 'tid', 'full');
	$work_status_tids = array();
	foreach ($all_work_statuses as $tid => $term) {
		if ($term->field_sirius_dispatch_available['und'][0]['value'] == 'Yes') {
			$work_status_tids[] = $tid;
		}
	}
	if (!$work_status_tids) { return array(); }

	// Get the industry TID
	$industry_tid = $job_node->field_sirius_industry['und'][0]['tid'];
	if (!$industry_tid) { return array(); }
	$industry_proxy = sirius_taxonomy_proxy($industry_tid);

	// Get the dispatchable member statuses for this job.
	$member_status_tids = array();
	if ($job_node->field_sirius_member_status['und'][0]['tid']) {
		foreach ($job_node->field_sirius_member_status['und'] as $tmp) {
			$member_status_tids[] = $tmp['tid'];
		}
	}
	if (!$member_status_tids) { return array(); }

	// Get the order of member statuses
	$member_status_weights = array();
	foreach ($member_status_tids as $member_status_tid) {
		$member_status_term = taxonomy_term_load($member_status_tid);
		$member_status_weights[$member_status_tid] = $member_status_term->field_grievance_weight['und'][0]['value'];
		if (!$member_status_weights[$member_status_tid]) { $member_status_weights[$member_status_tid] = 0; }
	}

	// Get the facility NID
	$facility_nid = $job_node->field_sirius_dispatch_facility['und'][0]['target_id'];

	// Get the employer nid
	$employer_nid = $job_node->field_grievance_shop['und'][0]['target_id'];

	// Tables
	$tables = array(
		'field_data_field_sirius_name',
		'field_data_field_sirius_worker_dispstatus',
		'field_data_field_sirius_member_status',
		'field_data_field_sirius_skills_availx',
		'field_data_field_sirius_work_status',
		'field_data_field_sirius_dispatch_availdate',
		'field_data_field_sirius_dispatch_cbn',
		'field_data_field_sirius_id',
		'field_data_field_sirius_dispatch_hfe',
		'field_data_field_sirius_dispatch_hfe_until',
		'field_data_field_sirius_dispatch_eba',
		'field_data_field_sirius_contact',
	);

	// Writing this in SQL for speed
	$args = array();

	// Fields
	$sql = "select ";
	if ($filters['count']) {
		$sql .= "count(*) as c ";
	} else {
		$sql .= "node.nid, ";
		$sql .= "field_sirius_name_given as given, ";
		$sql .= "field_sirius_name_family as family, ";
		$sql .= "taxonomy_term_data_member_status.tid as member_status_tid, ";
		$sql .= "taxonomy_term_data_member_status.name as member_status_name, ";
		$sql .= "field_sirius_contact_target_id as contact_nid, ";
		$sql .= "case ";
		foreach ($member_status_weights as $tid => $weight) {
			if (!$weight) { $weight = 0; }
			$sql .= "when taxonomy_term_data_member_status.tid = $tid then $weight ";
		}
		$sql .= "end as member_status_weight, ";
		$sql .= "field_sirius_dispatch_availdate_value as availdate, ";
		$sql .= "field_sirius_id_value as id, ";
		$sql .= "field_sirius_dispatch_eba_value as eba ";
	}

	// Tables
	$sql .= "from node ";
	foreach ($tables as $table) {
		$sql .= "left join $table on $table.entity_type = 'node' and $table.entity_id = node.nid ";
	}
	$sql .= "left join taxonomy_term_data taxonomy_term_data_member_status on taxonomy_term_data_member_status.tid = field_sirius_member_status_tid ";

	// Basic structural joins
	$sql .= "where node.type = 'sirius_worker' ";

	// Exclude current dispatches
	if (!$filters['redispatch'] && !$filters['ignore_redispatch'] && $job_node->nid) {
		$sql .= "and nid not in (";
		$sql .= "  select a1.field_sirius_worker_target_id as n from ";
		$sql .= "  node n1, ";
		$sql .= "  field_data_field_sirius_worker a1, ";
		$sql .= "  field_data_field_sirius_dispatch_job b1 ";
		$sql .= "  where n1.type = 'sirius_dispatch' ";
		$sql .= "  and a1.entity_type = 'node' and a1.entity_id = n1.nid ";
		$sql .= "  and b1.entity_type = 'node' and b1.entity_id = n1.nid ";
		$sql .= "  and b1.field_sirius_dispatch_job_target_id = :job_nid ";
		$sql .= ') ';
		$args[':job_nid'] = $job_node->nid;
	}

	// Limit by member status
	$sql .= "and field_sirius_member_status_tid in (:member_status_tids) ";
	$args[':member_status_tids'] = $member_status_tids;

	// Limit by work status
	$sql .= "and field_sirius_work_status_tid in (:work_status_tids) ";
	$args[':work_status_tids'] = $work_status_tids;

	// Limit by industry.  All jobs need an industry
	$sql .= "and exists (";
	$sql .= "  select 1 from field_data_field_sirius_industry ";
	$sql .= "  where field_data_field_sirius_industry.entity_type = 'node' ";
	$sql .= "  and field_data_field_sirius_industry.entity_id = node.nid ";
	$sql .= "  and field_sirius_industry_tid = :industry_tid ";
	$sql .= ") ";
	$args[':industry_tid'] = $industry_tid;

	// limit by skill
	$skill_tids = sirius_fieldvals($job_node, 'field_sirius_skill', 'tid');
	if (!$filters['ignore_skill'] && $skill_tids) {
		foreach ($skill_tids as $delta => $skill_tid) {
			if ($skill_tid) {
				$sql .= "and exists (";
				$sql .= "  select 1 from field_data_field_sirius_skill ";
				$sql .= "  left join field_data_field_sirius_skill_expire on ";
				$sql .= "    field_data_field_sirius_skill_expire.entity_type = 'node' and ";
				$sql .= "    field_data_field_sirius_skill_expire.entity_id = field_data_field_sirius_skill.entity_id and ";
				$sql .= "    field_data_field_sirius_skill_expire.delta = field_data_field_sirius_skill.delta ";
				$sql .= "  where field_data_field_sirius_skill.entity_type = 'node' ";
				$sql .= "  and field_data_field_sirius_skill.entity_id = node.nid ";
				$sql .= "  and field_sirius_skill_tid = :skill_tid_$delta ";
				$sql .= "  and ( ";
				$sql .= "    field_sirius_skill_expire_value is null ";
				$sql .= "    or field_sirius_skill_expire_value >= CURDATE() ";
				$sql .= "  )";
				$sql .= ") ";
				$args[":skill_tid_$delta"] = $skill_tid;
			}
		}
	}

	if (!$filters['ignore_skill'] && ($job_policy['workers']['ignore_prefs_skill'] != 'Yes')) {
		// Limit by skill avail -- which could be null, meaning all skills
		if ($skill_tid) {
			$sql .= "and ( ";
			$sql .= "  exists (";
			$sql .= "    select 1 from field_data_field_sirius_skills_avail ";
			$sql .= "    where field_data_field_sirius_skills_avail.entity_type = 'node' ";
			$sql .= "    and field_data_field_sirius_skills_avail.entity_id = node.nid ";
			$sql .= "    and field_sirius_skills_avail_tid = :skill_tid2 ";
			$sql .= "  ) ";
			$sql .= "  or ";
			$sql .= "  not exists (";
			$sql .= "    select 1 from field_data_field_sirius_skills_avail ";
			$sql .= "    where field_data_field_sirius_skills_avail.entity_type = 'node' ";
			$sql .= "    and field_data_field_sirius_skills_avail.entity_id = node.nid ";
			$sql .= "  ) ";
			$sql .= ") ";
			$args[':skill_tid2'] = $skill_tid;
		} else {
			$sql .= "and (field_sirius_skills_availx_value IS NULL or field_sirius_skills_availx_value = 'Yes') ";
		}
	}

	// Limit by department avail -- which could be null, meaning all departments
	if (!$filters['ignore_department'] && ($job_policy['workers']['ignore_prefs_dept'] != 'Yes')) {
		$department_tid = $job_node->field_grievance_departments['und'][0]['tid'];
		if ($department_tid) {
			$sql .= "and not exists ( ";
			$sql .= "  select 1 from field_data_field_grievance_departments ";
			$sql .= "  where field_data_field_grievance_departments.entity_type = 'node' ";
			$sql .= "  and field_data_field_grievance_departments.entity_id = node.nid ";
			$sql .= "  and field_grievance_departments_tid = :department_tid ";
			$sql .= ") ";
			$args[':department_tid'] = $department_tid;
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
		if ($job_node->field_sirius_dispatch_eba['und'][0]['value'] != 'Yes') {
			// Must be "not employed available"
			$sql .= "and field_sirius_worker_dispstatus_tid in (:dispatch_statuses) ";
		} else {
			// Must be "not employed available" or "employed" with a suitable EBA
			$sql .= "and ( ";
			$sql .= "  (field_sirius_worker_dispstatus_tid in (:dispatch_statuses) )  ";
			$sql .= "  or ";
			$sql .= "  ( ";
			$sql .= "    field_sirius_worker_dispstatus_tid = :dispstatus_employed ";
			$sql .= "    and field_sirius_dispatch_eba_value = 'Yes' ";
			if ($job_node->field_sirius_dispatch_eba_dates['und']) {
				foreach ($job_node->field_sirius_dispatch_eba_dates['und'] as $delta => $tmp) {
					$sql .= "    and exists ( ";
					$sql .= "      select 1 from field_data_field_sirius_dispatch_eba_dates eba_dates_$delta ";
					$sql .= "      where eba_dates_$delta.entity_type = 'node' ";
					$sql .= "      and eba_dates_$delta.entity_id = node.nid ";
					$sql .= "      and eba_dates_$delta.field_sirius_dispatch_eba_dates_value = :eba_dates_$delta ";
					$sql .= "      ) ";
					$args[":eba_dates_$delta"] = $tmp['value'];
				}
			}
			$sql .= "  ) ";
			$sql .= ") ";

			$args[':dispstatus_employed'] = sirius_get_special_tid('sirius_worker_dispatch_status|employed');
		}
		$args[':dispatch_statuses'] = $dispatch_statuses;
	}

	// Exclude employer-initiated DNCs
	$sql .= "and not exists (";
	$sql .= "  select 1 from field_data_field_sirius_dispatch_dncs ";
	$sql .= "  where field_data_field_sirius_dispatch_dncs.entity_type = 'node' ";
	$sql .= "  and field_data_field_sirius_dispatch_dncs.entity_id = node.nid ";
	$sql .= "  and field_sirius_dispatch_dncs_target_id in (:employer_nid1) ";
	$args[':employer_nid1'] = array($employer_nid);
	if ($facility_nid) { $args[':employer_nid1'][] = $facility_nid; }
	if ($industry_proxy) { $args[':employer_nid1'][] = $industry_proxy->nid; }
	$sql .= ") ";

	// Exclude employee-initiated DNCs
	if ($job_policy['workers']['ignore_prefs_dnc'] != 'Yes') {
		$sql .= "and not exists (";
		$sql .= "  select 1 from field_data_field_sirius_dispatch_dncs2 ";
		$sql .= "  where field_data_field_sirius_dispatch_dncs2.entity_type = 'node' ";
		$sql .= "  and field_data_field_sirius_dispatch_dncs2.entity_id = node.nid ";
		$sql .= "  and field_sirius_dispatch_dncs2_target_id = :employer_nid2 ";
		$sql .= ") ";
		$args[':employer_nid2'] = $employer_nid;
	}

	// Worker NID
	if ($filters['worker_nid']) {
		$sql .= "and node.nid = :worker_nid ";
		$args[':worker_nid'] = $filters['worker_nid'];
	}

	// CBN
	if (!$filters['ignore_cbn'] && ($job_policy['workers']['ignore_prefs_cbn'] != 'Yes')) {
		if ($filters['cbn'] == 'No') {
			$sql .= "and ( ";
			$sql .= "  field_sirius_dispatch_cbn_value IS NULL ";
			$sql .= "  or field_sirius_dispatch_cbn_value = 'No' ";
			$sql .= ") ";
		}
	}

	// HFE
	if (!$filters['ignore_hfe'] && ($job_policy['workers']['ignore_prefs_hfe'] != 'Yes')) {
		if ($employer_nid) {
			$sql .= "and ( ";
			$sql .= "  field_sirius_dispatch_hfe_target_id = :hfe ";
			$sql .= "  or field_sirius_dispatch_hfe_target_id IS NULL ";
			$sql .= "  or field_sirius_dispatch_hfe_until_value IS NULL ";
			$sql .= "  or field_sirius_dispatch_hfe_until_value < NOW()";
			$args[':hfe'] = $employer_nid;
			$sql .= ") ";
		}
	}

	// Rollover
	if ($job_policy['workers']['must_be_rollover'] == 'Yes') {
		$roll_cutoff = strtotime('-7 days');
		$sql .= "and exists (";
		$sql .= "  select nid "; 
		$sql .= "  from node roll_n, ";
		$sql .= "  field_data_field_sirius_log_handler roll_h1, ";
		$sql .= "  field_data_field_sirius_log_handler roll_h2, ";
		$sql .= "  field_data_field_sirius_category roll_c ";
		$sql .= "  where roll_n.type = 'sirius_log' ";
		$sql .= "  and roll_n.created >= :roll_cutoff ";
		$sql .= "  and roll_h1.entity_type = 'node' and roll_h1.entity_id = roll_n.nid and roll_h1.field_sirius_log_handler_target_id = node.nid ";
		$sql .= "  and roll_h2.entity_type = 'node' and roll_h2.entity_id = roll_n.nid and roll_h2.field_sirius_log_handler_target_id = :roll_employer_nid ";
		$sql .= "  and roll_c.entity_type = 'node' and roll_c.entity_id = roll_n.nid and roll_c.field_sirius_category_value = 'dispatch:rollover' ";
		$sql .= ") ";
		$args[':roll_employer_nid'] = $employer_nid;
		$args[':roll_cutoff'] = $roll_cutoff;
	}

	// Event
	$event_nids = sirius_fieldvals($job_node, 'field_sirius_events', 'target_id');
	if ($event_nids && !$filters['ignore_event']) {
		$event_node = node_load($event_nids[0]);
		$event_type_term = taxonomy_term_load($event_node->field_sirius_event_type['und'][0]['tid']);
		$json = json_decode($event_type_term->field_sirius_json['und'][0]['value'], TRUE);
		list ($prole_tid, $pstatus_tid) = explode('|', $json['bullpen']['pre_dispatch']);
		$sql .= "and exists (";
		$sql .= "select 1 from "; 
		$sql .= "field_data_field_sirius_contact worker_contact, ";
		$sql .= "field_data_field_sirius_contact participant_contact, ";
		$sql .= "field_data_field_sirius_event, ";
		$sql .= "field_data_field_sirius_event_prole, ";
		$sql .= "field_data_field_sirius_event_pstatus ";

		$sql .= "where worker_contact.entity_type = 'node' ";
		$sql .= "and worker_contact.entity_id = node.nid ";

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
		$sql .= ") ";
		$args[':event_nids'] = $event_nids;
		$args[':prole_tid'] = $prole_tid;
		$args[':pstatus_tid'] = $pstatus_tid;
	}

	// Cached ineligibility
	if (!$filters['ignore_inelig_cache'] && $job_node->nid) {
		$sql .= "and not exists (select 1 from sirius_dispatch_elig_cache ";
		$sql .= "where sirius_dispatch_elig_cache.job_nid = :inelig_cache_job_nid ";
		$sql .= "and sirius_dispatch_elig_cache.worker_nid = node.nid) ";
		$args[':inelig_cache_job_nid'] = $job_node->nid;
	}

	$sql .= "group by node.nid ";

	// Order
	if (!$filters['count']) {
		if ($order == 'dispatch') {
			$sql .= "order by member_status_weight, field_sirius_dispatch_availdate_value, field_sirius_name_family, field_sirius_name_given ";
		} else {
			$sql .= "order by field_sirius_name_family, field_sirius_name_given ";
		}

		if ($limit) {
			$sql .= "limit $limit ";
		} 
		if ($offset) {
			$sql .= "offset $offset ";
		}
	}

	// Execute
	$stmt = sirius_sql_query($sql, $args);

	// Count only?
	if ($filters['count']) { 
		$hr = $stmt->fetchAssoc();
		return $hr['c'];
	} 

	// Return full worker list.
	$workers = array();
	while ($hr = $stmt->fetchAssoc()) {
		$workers[$hr['nid']] = $hr;
	}

	// Add the rollover date to each worker. 
	if ($job_policy['workers']['must_be_rollover'] == 'Yes') {
		sirius_dispatch_rollover_dates($employer_nid, $workers, $roll_cutoff);
	}

	return $workers;
}
