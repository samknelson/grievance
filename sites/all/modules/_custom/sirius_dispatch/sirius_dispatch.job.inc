<?php


function sirius_dispatch_job_count($dispatch_nid) {
	
}

function sirius_dispatch_job_recompute_from_dispatch($dispatch_node, $is_delete = FALSE) {
	$job_nid = $dispatch_node->field_sirius_dispatch_job['und'][0]['target_id'];
	if (!$job_nid) { return; }
	$job_node = node_load($job_nid);
	if (!$job_node) { return; }

	// Fetch from the database
	$sql = "select field_sirius_dispatch_status_value as status, field_data_field_sirius_dispatch_status.entity_id as nid ";
	$sql .= "from field_data_field_sirius_dispatch_status, field_data_field_sirius_dispatch_job ";
	$sql .= "where field_data_field_sirius_dispatch_status.entity_type = 'node' ";
	$sql .= "and field_data_field_sirius_dispatch_job.entity_type = 'node' ";
	$sql .= "and field_data_field_sirius_dispatch_status.entity_id = field_data_field_sirius_dispatch_job.entity_id ";
	$sql .= "and field_data_field_sirius_dispatch_job.field_sirius_dispatch_job_target_id = :job_nid ";
	$stmt = db_query($sql, array(':job_nid' => $job_nid));
	$dispatches = array();
	while ($hr = $stmt->fetchAssoc()) {
		$dispatches[$hr['nid']] = $hr['status'];
	}

	// Update with the current node, since it might not be written to the database yet
	if ($is_delete) {
		unset($dispatches[$dispatch_node->nid]);
	} else {
		$dispatches[$dispatch_node->nid] = $dispatch_node->field_sirius_dispatch_status['und'][0]['value'];
	}

	// Count
	$count_notified = 0;
	$count_yes = 0;
	$count_no = 0;
	foreach ($dispatches as $ignore => $status) {
		if ($status == 'notified') { ++$count_notified; }
		if ($status == 'accepted' || $status == 'accepted_secondary' || $status == 'resigned' || $status == 'vlri' || $status == 'terminated') { ++$count_yes; }
		if ($status == 'declined' || $status == 'notification failed' || $status == 'expired') { ++$count_no; }
	}

	$need_save = FALSE;

	if ($job_node->field_sirius_count_yes['und'][0]['value'] != $count_yes) {
		$need_save = TRUE; 
		$job_node->field_sirius_count_yes['und'][0]['value'] = $count_yes;
	}

	if ($job_node->field_sirius_count_no['und'][0]['value'] != $count_no) {
		$need_save = TRUE; 
		$job_node->field_sirius_count_no['und'][0]['value'] = $count_no;
	}

	if ($job_node->field_sirius_count_notified['und'][0]['value'] != $count_notified) {
		$need_save = TRUE; 
		$job_node->field_sirius_count_notified['und'][0]['value'] = $count_notified;
	}

	if ($need_save) {
		node_save($job_node);
	}
}

function sirius_dispatch_job_workerform_ajax($form, &$form_state) {
	return $form['fieldset_workers']['ajax'];
}

function sirius_dispatch_job_workerform($form, &$form_state, $states = NULL) {
	// Job node
	$job_node = $form_state['job_node'];
	$job_policy = sirius_dispatch_job_policy($job_node);

	// Can we create dispatches for ineligible workers? Usually we can, but some job types prohibit this.  In particular, you
	// can't create a dispatch on a rollover job for anybody who's not rollover.
	$must_be_eligible = ($job_policy['workers']['must_be_eligible'] == 'Yes');

	$member_status_terms = sirius_taxonomy_load('sirius_member_status', 'tid', 'full');
	$member_status_abbrv = array();
	foreach ($member_status_terms as $tid => $term) {
  	if ($term->field_sirius_name_short['und'][0]['value']) {
  		$term_name = $term->field_sirius_name_short['und'][0]['value'];
  	} else {
  		$term_name = $term->name;
  	}
  	$member_status_abbrv[$tid] = $term_name;
	}

	// drupal_add_js(drupal_get_path('module', 'sirius_dispatch') . '/js/sirius_dispatch.worker_filter.js', 'file');
	$form['fieldset_workers']  = array(
		'#type' => 'fieldset',
		'#title' => 'Requested Workers',
		'#states' => $states,
	);

	// Filter ID
	$form['fieldset_workers']['worker_id'] = array(
		'#type' => 'textfield',
		'#title' => 'Filter by member name or ID. Enter * to see all dispatchable workers.',
		'#size' => 20,
		'#states' => $states,
		'#prefix' => '<div class="container-inline">',
	);

	$form['fieldset_workers']['go'] = array(
		'#type' => 'button',
		'#value' => 'Filter',
    '#ajax' => array(
      'callback' => 'sirius_dispatch_job_workerform_ajax',
      'wrapper' => 'workers_filtered',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    // '#limit_validation_errors' => array(),
		'#suffix' => '</div>',
	);

	//
	// Which workers should we show? It's complicated, because we want to show some that are not dispatchable
	// 

	// We're searching for this worker
	$search_worker_id = $form_state['values']['fieldset_workers']['worker_id'];

	// We've selected these workers
	$selected_workers = array();
	if ($form_state['values']['fieldset_workers']['ajax']['workers']) {
		foreach ($form_state['values']['fieldset_workers']['ajax']['workers'] as $nid => $value) { 
			if ($value) { 
				$selected_workers[$nid] = TRUE;
			}
		}
	}

	// Find any dispatchable workers
	$dispatchable_workers = array();
	if ($search_worker_id || $selected_workers) { 
		$dispatchable_workers = sirius_dispatch_job_get_dispatchable_workers($job_node, 'name');
	}

	// Start with any dispatchable workers who are already checked, and who match on name or ID
	$workers = array();
	$found_id = FALSE;
	foreach ($dispatchable_workers as $nid => $worker) {
		$is_good = FALSE; 
		if ($selected_workers[$nid]) { $is_good = TRUE; }
		if ($worker['id'] == $search_worker_id) { $is_good = TRUE; $found_id = TRUE; }
		if ($search_worker_id == '*') { $is_good = TRUE; }
		if (stripos($worker['family'], $search_worker_id) !== FALSE) { $is_good = TRUE; }
		if (stripos($worker['given'], $search_worker_id) !== FALSE) { $is_good = TRUE; }
		if ($is_good) { $workers[$nid] = $worker; }
	}

	// Next, if we're searching by ID, and if the worker is non-dispatchable, add them with an explanation.
	$non_dispatchable_workers = array();
	foreach ($selected_workers as $nid => $ignore) {
		if (!$workers[$nid]) {
			$non_dispatchable_workers[$nid] = TRUE;
		}
	}
	if ($search_worker_id > 0 && !$found_id) {
		$nid = sirius_find_nid_by_id($search_worker_id, 'sirius_worker');
		if ($nid) { $non_dispatchable_workers[$nid] = TRUE; }
	}
	foreach ($non_dispatchable_workers as $nid => $ignore) {
		$sql = "select field_data_field_sirius_id.entity_id, field_sirius_id_value, field_sirius_name_family, field_sirius_name_given ";
		$sql .= "from field_data_field_sirius_id, field_data_field_sirius_name ";
		$sql .= "where field_data_field_sirius_id.entity_id = :nid ";
		$sql .= "and field_data_field_sirius_id.entity_type = 'node' and field_data_field_sirius_id.bundle = 'sirius_worker' ";
		$sql .= "and field_data_field_sirius_name.entity_type = 'node' and field_data_field_sirius_name.bundle = 'sirius_worker' ";
		$sql .= "and field_data_field_sirius_id.entity_id = field_data_field_sirius_name.entity_id ";
		$sql .= "order by field_data_field_sirius_id.entity_id desc limit 1 ";
		$stmt = db_query($sql, array(':nid' => $nid));
		$hr = $stmt->fetchAssoc();
		if ($hr['entity_id']) {
			$workers[$nid] = array();
			$workers[$nid]['id'] = $hr['field_sirius_id_value'];
			$workers[$nid]['family'] = $hr['field_sirius_name_family'];
			$workers[$nid]['given'] = $hr['field_sirius_name_given'];
			$result = sirius_dispatch_job_get_dispatchable_workers_explain($job_node, $nid);
			if (!$result['success']) {
				$workers[$nid]['msg'] = $result['msg'];	
			}
			
		}
	}

  $worker_options = array();
  foreach ($workers as $worker_nid => $worker) {
  	$tid = $worker['member_status_tid'];
  	$worker_options[$worker_nid] = $worker['family'] . ', ' . $worker['given'];
  	if ($tid) { $worker_options[$worker_nid] .= ' [' . $member_status_abbrv[$tid] . ']'; }
  	if ($worker['eba'] == 'Yes') { $worker_options[$worker_nid] .= ' [EBA]'; }
  	if ($worker['msg']) { $worker_options[$worker_nid] .= '<br><span class="msg">' . $worker['msg'] . '</span>'; }
  }

  $form['fieldset_workers']['ajax'] = array(
  	'#type' => 'fieldset',
  	'#prefix' => '<div id="workers_filtered">',
  	'#suffix' => '</div>',
  );

  /*
	$form['fieldset_workers']['ajax']['debug'] = array(
		'#type' => 'markup',
		'#markup' => 'NONCE: ' . rand(0, 10000) . ' / JOB: ' . $job_node->title . ' / ' . ' count: ' . count($dispatchable_workers) . ' / DEBUG: ' . $debug,
	);
	*/

  if ($worker_options) {
		$form['fieldset_workers']['ajax']['workers'] = array(
			'#type' => 'checkboxes',
			'#options' => $worker_options,
			'#title' => 'Workers',
			'#prefix' => "<div class=\"checkboxes-inline\">",
			'#suffix' => '</div><div style="clear: left;">&nbsp;</div></div>',
			'#states' => $states,
		);
		if ($must_be_eligible) {
			foreach ($non_dispatchable_workers as $nid => $ignore) {
				if ($worker_options[$nid]) {
					$form['fieldset_workers']['ajax']['workers'][$nid]['#disabled'] = TRUE;
				}
			}
		}
	} else if ($search_worker_id || $selected_workers) {
		$form['fieldset_workers']['ajax']['workers'] = array(
			'#type' => 'markup',
			'#markup' => t('No workers match your criteria.'),
			'#title' => 'Workers',
		);
	}

	return $form;	
}

function sirius_dispatch_job_get_dispatchable_workers($job_node, $order = 'name', $limit = 0, $filters = array()) {
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
				$sql .= "  where field_data_field_sirius_skill.entity_type = 'node' ";
				$sql .= "  and field_data_field_sirius_skill.entity_id = node.nid ";
				$sql .= "  and field_sirius_skill_tid = :skill_tid_$delta ";
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
	$sql .= "  select 1 from field_revision_field_sirius_dispatch_dncs ";
	$sql .= "  where field_revision_field_sirius_dispatch_dncs.entity_type = 'node' ";
	$sql .= "  and field_revision_field_sirius_dispatch_dncs.entity_id = node.nid ";
	$sql .= "  and field_sirius_dispatch_dncs_target_id in (:employer_nid1) ";
	$args[':employer_nid1'] = array($employer_nid);
	if ($facility_nid) { $args[':employer_nid1'][] = $facility_nid; }
	$sql .= ") ";

	// Exclude employee-initiated DNCs
	if ($job_policy['workers']['ignore_prefs_dnc'] != 'Yes') {
		$sql .= "and not exists (";
		$sql .= "  select 1 from field_revision_field_sirius_dispatch_dncs2 ";
		$sql .= "  where field_revision_field_sirius_dispatch_dncs2.entity_type = 'node' ";
		$sql .= "  and field_revision_field_sirius_dispatch_dncs2.entity_id = node.nid ";
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








function sirius_dispatch_job_get_dispatchable_workers_explain_by_worker_id($job_node, $worker_id, $filters = array()) {
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


function sirius_dispatch_job_get_dispatchable_workers_explain($job_node, $worker_nid, $filters = array()) {
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
	if (!$industry_tid) { return array('success' => FALSE, 'msg' => t('There is no work industry specified for this job.')); }

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
	if (!$filters['ignore_redispatch']) {
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
			foreach ($skill_tids as $delta => $skill_tid) {
				if (!in_array($skill_tid, sirius_fieldvals($worker_node, 'field_sirius_skill', 'tid'))) {
					$skill_term = taxonomy_term_load($skill_tid);
					return array('success' => FALSE, 'msg' => t('The worker does not have the skill @skill_name.', array('@skill_name' => $skill_term->name)));
				}
			}
		}
	}

	if (!$filters['ignore_skill'] && ($job_policy['workers']['ignore_prefs_skill'] != 'Yes')) {
		// Limit by skill avail -- which could be null, meaning all skills
		$skills_avail = sirius_fieldvals($worker_node, 'field_sirius_skills_avail', 'tid');
		if ($skill_tid && $skills_avail && !in_array($skill_tid, $skills_avail)) {
			return array('success' => FALSE, 'msg' => t('The worker has set a skill preference preventing them from accepting this job.'));
		}

		if (!$skill_tid && $worker_node->field_sirius_skills_availx['und'][0]['value'] == 'No') {
			return array('success' => FALSE, 'msg' => t('The worker has set a preference requesting only skilled jobs.'));
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

	// Exclude employer-initiated DNCs
	if ($facility_nid && in_array($facility_nid, sirius_fieldvals($worker_node, 'field_sirius_dispatch_dncs', 'target_id'))) {
		return array('success' => FALSE, 'msg' => t('The worker has been placed on the do-not-call list for this facility.'));
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

	// Workers
	$workers = sirius_dispatch_job_get_dispatchable_workers($job_node, 'dispatch', 0, $filters);
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
