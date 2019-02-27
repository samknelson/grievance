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
		if ($status == 'accepted') { ++$count_yes; }
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

function sirius_dispatch_job_get_dispatchable_workers_render_form($job_node, $states = NULL) {
	drupal_add_js(drupal_get_path('module', 'sirius_dispatch') . '/js/sirius_dispatch.worker_filter.js', 'file');
	$form['fieldset_workers']  = array(
		'#type' => 'fieldset',
		'#title' => 'Requested Workers',
		'#states' => $states,
	);

	// Filter ID
	$form['fieldset_workers']['worker_id'] = array(
		'#type' => 'textfield',
		'#title' => 'Filter by member name or ID',
		'#size' => 20,
		'#states' => $states,
		'#attributes' => array('class' => array('sirius-worker-filter')),
	);

	// We're creating a "dummy" job node, that we can use to pass to get_dispatchable_workers
	$workers = sirius_dispatch_job_get_dispatchable_workers($job_node, 'name');
	$member_status_terms = sirius_taxonomy_load('sirius_member_status', 'tid', 'full');
  $worker_options = array();
  foreach ($workers as $worker_nid => $worker) {
  	$tid = $worker['member_status_tid'];
  	$term = $member_status_terms[$tid];
  	if ($term->field_sirius_name_short['und'][0]['value']) {
  		$term_name = $term->field_sirius_name_short['und'][0]['value'];
  	} else {
  		$term_name = $term->name;
  	}

  	$worker_options[$worker_nid] = $worker['family'] . ', ' . $worker['given'] . ' [' . $term_name . ']';
  	$worker_options[$worker_nid] .= ' <span class="sirius_invisible sirius_worker_id">' . $worker['id'] . '</span>';
  }

  if ($worker_options) {
		$form['fieldset_workers']['workers'] = array(
			'#type' => 'checkboxes',
			'#options' => $worker_options,
			'#title' => 'Workers',
			'#prefix' => "<div id=\"$worker_ajax_id\"><div class=\"checkboxes-inline sirius-worker-filter-target\">",
			'#suffix' => '</div><div style="clear: left;">&nbsp;</div></div>',
			'#states' => $states,
		);
	} else {
		$form['fieldset_workers']['workers'] = array(
			'#type' => 'markup',
			'#markup' => t('No qualified workers match your criteria.'),
			'#title' => 'Workers',
			'#prefix' => "<div id=\"$worker_ajax_id\">",
			'#suffix' => '</div>',
		);
	}

	return $form;	
}

function sirius_dispatch_job_get_dispatchable_workers($job_node, $order = 'name', $limit = 0, $filters = array()) {
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
	}

	// Get the employer nid
	$employer_nid = $job_node->field_grievance_shop['und'][0]['target_id'];

	// Tables
	$tables = array(
		'field_data_field_sirius_name',
		'field_data_field_sirius_worker_dispstatus',
		'field_data_field_sirius_member_status',
		'field_data_field_sirius_work_status',
		'field_data_field_sirius_dispatch_availdate',
		'field_data_field_sirius_dispatch_cbn',
		'field_data_field_sirius_id',
		'field_data_field_sirius_dispatch_hfe',
		'field_data_field_sirius_dispatch_hfe_until',
	);

	// Writing this in SQL for speed
	$args = array();

	// Fields
	$sql = "select node.nid, ";
	$sql .= "field_sirius_name_given as given, ";
	$sql .= "field_sirius_name_family as family, ";
	$sql .= "taxonomy_term_data_member_status.tid as member_status_tid, ";
	$sql .= "taxonomy_term_data_member_status.name as member_status_name, ";
	$sql .= "case ";
	foreach ($member_status_weights as $tid => $weight) {
		$sql .= "when taxonomy_term_data_member_status.tid = $tid then $weight ";
	}
	$sql .= "end as member_status_weight, ";
	$sql .= "field_sirius_dispatch_availdate_value as availdate, ";
	$sql .= "field_sirius_id_value as id ";

	// Tables
	$sql .= "from node ";
	foreach ($tables as $table) {
		$sql .= "left join $table on $table.entity_type = 'node' and $table.entity_id = node.nid ";
	}
	$sql .= "left join taxonomy_term_data taxonomy_term_data_member_status on taxonomy_term_data_member_status.tid = field_sirius_member_status_tid ";

	// Basic structural joins
	$sql .= "where node.type = 'sirius_worker' ";

	// Exclude current dispatches
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
	$skill_tid = $job_node->field_sirius_skill['und'][0]['tid'];
	if ($skill_tid) {
		$sql .= "and exists (";
		$sql .= "  select 1 from field_data_field_sirius_skill ";
		$sql .= "  where field_data_field_sirius_skill.entity_type = 'node' ";
		$sql .= "  and field_data_field_sirius_skill.entity_id = node.nid ";
		$sql .= "  and field_sirius_skill_tid = :skill_tid ";
		$sql .= ") ";
		$args[':skill_tid'] = $skill_tid;
	}

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
	}

	// Must be "not employed available"
	$sql .= "and field_sirius_worker_dispstatus_tid = :dispstatus_available ";
	$args[':dispstatus_available'] = sirius_get_special_tid('sirius_worker_dispatch_status|not_employed_available');

	// Exclude employer-initiated DNCs
	$sql .= "and not exists (";
	$sql .= "  select 1 from field_revision_field_sirius_dispatch_dncs ";
	$sql .= "  where field_revision_field_sirius_dispatch_dncs.entity_type = 'node' ";
	$sql .= "  and field_revision_field_sirius_dispatch_dncs.entity_id = node.nid ";
	$sql .= "  and field_sirius_dispatch_dncs_target_id = :employer_nid1 ";
	$sql .= ") ";
	$args[':employer_nid1'] = $employer_nid;

	// Exclude employee-initiated DNCs
	$sql .= "and not exists (";
	$sql .= "  select 1 from field_revision_field_sirius_dispatch_dncs2 ";
	$sql .= "  where field_revision_field_sirius_dispatch_dncs2.entity_type = 'node' ";
	$sql .= "  and field_revision_field_sirius_dispatch_dncs2.entity_id = node.nid ";
	$sql .= "  and field_sirius_dispatch_dncs2_target_id = :employer_nid2 ";
	$sql .= ") ";
	$args[':employer_nid2'] = $employer_nid;

	// Worker NID
	if ($filters['worker_nid']) {
		$sql .= "and node.nid = :worker_nid ";
		$args[':worker_nid'] = $filters['worker_nid'];
	}

	// CBN
	if ($filters['cbn']) {
		$sql .= "and ( ";
		$sql .= "  field_sirius_dispatch_cbn_value IS NULL ";
		$sql .= "  or field_sirius_dispatch_cbn_value = :cbn ";
		$sql .= ") ";
		$args[':cbn'] = $filters['cbn'];
	}

	// HFE
	$sql .= "and ( ";
	$sql .= "  field_sirius_dispatch_hfe_target_id = :hfe ";
	$sql .= "  or field_sirius_dispatch_hfe_target_id IS NULL ";
	$sql .= "  or field_sirius_dispatch_hfe_until_value IS NULL ";
	$sql .= "  or field_sirius_dispatch_hfe_until_value < NOW()";
	$args[':hfe'] = $employer_nid;
	$sql .= ") ";

	// Order
	if ($order == 'dispatch') {
		$sql .= "order by member_status_weight, field_sirius_dispatch_availdate_value, field_sirius_name_family, field_sirius_name_given ";
	} else {
		$sql .= "order by field_sirius_name_family, field_sirius_name_given ";
	}

	if ($limit) {
		$sql .= "limit $limit ";
	} 

	// Generate our worker list
	$stmt = db_query($sql, $args);
	$workers = array();
	while ($hr = $stmt->fetchAssoc()) {
		$workers[$hr['nid']] = $hr;
	}

	return $workers;
}