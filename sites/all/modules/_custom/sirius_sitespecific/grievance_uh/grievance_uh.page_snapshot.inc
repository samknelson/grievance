<?php

function grievance_uh_page_snapshot() {
	return drupal_get_form('grievance_uh_page_snapshot_form');
}

function grievance_uh_page_snapshot_form($form, &$form_state) {
  $form['start_date'] = array(
    '#title' => t('Start Date'),
    '#type' => 'date_popup',
    '#attributes' => array('autocomplete' => 'off'),
    '#required' => TRUE,
    '#date_format' => 'Y-m-d',
    '#default_value' => date('Y-m-d', strtotime('-3 months')),
  );

  $form['end_date'] = array(
    '#title' => t('End Date'),
    '#type' => 'date_popup',
    '#attributes' => array('autocomplete' => 'off'),
    '#required' => TRUE,
    '#date_format' => 'Y-m-d',
    '#default_value' => date('Y-m-d', strtotime('+1 day')),
  );

  $form['output'] = array(
    '#title' => t('Output'),
    '#type' => 'select',
    '#options' => array(
    	'screen' => t('Screen'),
    	'csv' => t('CSV'),
    	'json' => t('JSON'),
    ),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  if ($form_state['results']) {
  	$form['results'] = array('#markup' => '<hr><h2>Results</h2>' . $form_state['results']);
  }
  
  return $form;
}

function grievance_uh_page_snapshot_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;

	$start_ts = strtotime($form_state['values']['start_date']);
	$end_ts = strtotime($form_state['values']['end_date']);

	$form_state['results'] = 'Hello, world.';

	$snapshot = grievance_uh_snapshot($start_ts, $end_ts);

	$output = $form_state['values']['output'];
	if ($output == 'json') {
		drupal_json_output($snapshot);
		exit();
	}

	if ($output == 'csv') {
		$fp = fopen('php://output', 'w');
		$filename = "snapshot_";
		$filename .= $snapshot['local_id'];
		$filename .= "_";
		$filename .= date('Ymd', $start_ts);
		$filename .= "_";
		$filename .= date('Ymd', $end_ts);
		$filename .= ".csv";
		drupal_add_http_header('Content-Type', 'text/csv');
		drupal_add_http_header('Content-Disposition', 'attachment;filename=' . $filename);
		fputcsv($fp, array_keys($snapshot));
		fputcsv($fp, array_values($snapshot));
		exit();
	}

	$results = '';
	foreach ($snapshot as $key => $value) {
		$results .= sirius_fakerender_field("$key:", $value);
	}
	$form_state['results'] = $results;
}

function grievance_uh_snapshot($start_ts, $end_ts) {
	$snapshot = array();

	// Local Number
	$local_id = sirius_domain_variable_get('grievance_uh_local_id');
	if (!$local_id) {
		drupal_set_message("Local ID not found. Make sure to set it on the " . l('configuration page', 'grievance/uh/config'), 'warning');
	}
	$snapshot['local_id'] = $local_id;

	// Find open and closed statuses
	$all_status_terms = sirius_taxonomy_load('grievance_status', 'tid', 'full');
	$open_status_tids = array();
	$closed_status_tids = array();
	foreach ($all_status_terms as $tid => $term) {
		if ($term->field_grievance_open['und'][0]['value'] == 1) { 
			$open_status_tids[] = $tid;
		} else {
			$closed_status_tids[] = $tid;
		}
	}
	// Lazy way to avoid SQL errors
	if (!$open_status_tids) 		{ $open_status_tids[] = -1; }
	if (!$closed_status_tids) 	{ $closed_status_tids[] = -1; }

	// Find all inactive employers
	$sql_args = array();
	$sql = "select nid from node ";
	$sql .= "left join field_data_field_sirius_active on field_data_field_sirius_active.entity_type = 'node' and field_data_field_sirius_active.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
	$sql .= "where node.type = 'grievance_shop' ";
	$sql .= "and field_sirius_active_value = 'No' ";
	if ($domain_nid) {
		$sql .= "and field_sirius_domain_target_id = :domain_nid ";
		$sql_args[':domain_nid'] = $domain_nid;
	} else {
		$sql .= "and field_sirius_domain_target_id is null ";
	}
	$stmt = sirius_sql_query($sql, $sql_args);
	$inactive_employers = array();
	while ($hr = $stmt->fetchAssoc()) { $inactive_employers[] = $hr['nid']; }
	if (!$inactive_employers) { $inactive_employers = array(-1); }

	// Fetch open and closed grievances
	$domain_nid = sirius_domain_nid();

	$each = array();
	$each['total_open'] = $open_status_tids;
	$each['total_closed'] = $closed_status_tids;
	foreach ($each as $snapshot_key => $status_tids) {
		$sql_args = array();
		$sql = "select count(*) as c from node ";
		$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
		$sql .= "left join field_data_field_grievance_status on field_data_field_grievance_status.entity_type = 'node' and field_data_field_grievance_status.entity_id = node.nid ";
		$sql .= "where node.type = 'grievance' ";
		if ($domain_nid) {
			$sql .= "and field_sirius_domain_target_id = :domain_nid ";
			$sql_args[':domain_nid'] = $domain_nid;
		} else {
			$sql .= "and field_sirius_domain_target_id is null ";
		}
		$sql .= "and field_grievance_status_tid in (:status_tids) ";
		$sql_args[':status_tids'] = $status_tids;
		$stmt = sirius_sql_query($sql, $sql_args);
		$hr = $stmt->fetchAssoc();
		$snapshot[$snapshot_key] = $hr['c'];
	}

	// Grievances created in the last 3 months
	$sql_args = array();
	$sql = "select count(distinct nid) as c, count(distinct field_grievance_shop_target_id) as c2 from node ";
	$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
	$sql .= "left join field_data_field_grievance_shop on field_data_field_grievance_shop.entity_type = 'node' and field_data_field_grievance_shop.entity_id = node.nid ";
	$sql .= "where node.type = 'grievance' ";
	if ($domain_nid) {
		$sql .= "and field_sirius_domain_target_id = :domain_nid ";
		$sql_args[':domain_nid'] = $domain_nid;
	} else {
		$sql .= "and field_sirius_domain_target_id is null ";
	}
	$sql .= "and created >= :start_ts ";
	$sql .= "and created <= :end_ts ";
	$sql_args[':start_ts'] = $start_ts;
	$sql_args[':end_ts'] = $end_ts;
	$stmt = sirius_sql_query($sql, $sql_args);
	$hr = $stmt->fetchAssoc();
	$snapshot['created'] = $hr['c'];
	$snapshot['active_employers'] = $hr['c2'];

	// Grievances resolved in the last 3 months
	$sql_args = array();
	$sql = "select node.nid as c from node ";
	$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
	$sql .= "left join field_data_field_grievance on field_data_field_grievance.entity_type = 'node' and field_data_field_grievance.bundle = 'grievance_log' and field_data_field_grievance.field_grievance_target_id = node.nid ";
	$sql .= "left join node log_node on log_node.nid = field_data_field_grievance.entity_id ";
	$sql .= "left join field_data_field_grievance_status on field_data_field_grievance_status.entity_type = 'node' and field_data_field_grievance_status.entity_id = log_node.nid ";
	$sql .= "where node.type = 'grievance' ";
	if ($domain_nid) {
		$sql .= "and field_sirius_domain_target_id = :domain_nid ";
		$sql_args[':domain_nid'] = $domain_nid;
	} else {
		$sql .= "and field_sirius_domain_target_id is null ";
	}
	$sql .= "and field_grievance_status_tid in (:closed_status_tids) ";
	$sql .= "group by node.nid ";
	$sql .= "having (min(log_node.created) >= :start_ts and min(log_node.created) <= :end_ts) ";
	$sql_args[':closed_status_tids'] = $closed_status_tids;
	$sql_args[':start_ts'] = $start_ts;
	$sql_args[':end_ts'] = $end_ts;
	$stmt = sirius_sql_query($sql, $sql_args);
	$count = 0;
	while ($hr = $stmt->fetchAssoc()) { ++$count; }
	$snapshot['closed'] = $count;

	// Settlement
	$sql_args = array();
	$sql = "select sum(field_grievance_amt_value) as s ";
	$sql .= "from node ";
	$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
	$sql .= "left join field_data_field_grievance_amt on field_data_field_grievance_amt.entity_type = 'node' and field_data_field_grievance_amt.entity_id = node.nid ";
	$sql .= "where node.type = 'grievance_settlement' ";
	if ($domain_nid) {
		$sql .= "and field_sirius_domain_target_id = :domain_nid ";
		$sql_args[':domain_nid'] = $domain_nid;
	} else {
		$sql .= "and field_sirius_domain_target_id is null ";
	}
	$sql .= "and created >= :start_ts ";
	$sql .= "and created <= :end_ts ";
	$sql_args[':start_ts'] = $start_ts;
	$sql_args[':end_ts'] = $end_ts;
	$stmt = sirius_sql_query($sql, $sql_args);
	$count = 0;
	$hr = $stmt->fetchAssoc();
	$snapshot['settlement_amt'] = '$' . number_format($hr['s'], 2);

	// Number of employers
	$snapshot['employer_count'] = count(sirius_employer_list());

	// Get our excluded role
	$excluded_role_name = 'uh tester';
	$sql_args = array();
	$sql = "select rid from role where name = :excluded_role_name ";
	$sql_args[':excluded_role_name'] = $excluded_role_name;
	$stmt = sirius_sql_query($sql, $sql_args);
	$hr = $stmt->fetchAssoc();
	$excluded_rid = $hr['rid'];
	if (!$excluded_rid) {
		drupal_set_message("There is no role called $excluded_role_name. Please make sure to configure one.", 'warning');
		$excluded_rid = -1;
	}

	// Number of active users in the time period, who *aren't* in the specified special role.
	$sql_args = array();
	$sql = "select count(distinct users.uid) as c ";
	$sql .= "from users ";
	$sql .= "where not exists (select 1 from users_roles where users_roles.uid = users.uid and users_roles.rid = :excluded_rid) ";
	$sql .= "and access >= :start_ts ";
	$sql .= "and access <= :end_ts ";
	$sql_args[':start_ts'] = $start_ts;
	$sql_args[':end_ts'] = $end_ts;
	$sql_args[':excluded_rid'] = $excluded_rid;
	$stmt = sirius_sql_query($sql, $sql_args);
	$hr = $stmt->fetchAssoc();
	$snapshot['active_users'] = $hr['c'];

	// Number of active users in the time period, who *aren't* in the specified special role.
	$sql_args = array();
	$sql = "select count(distinct users.uid) as c ";
	$sql .= "from users ";
	$sql .= "where not exists (select 1 from users_roles where users_roles.uid = users.uid and users_roles.rid = :excluded_rid) ";
	$sql .= "and access >= :start_ts ";
	$sql .= "and access <= :end_ts ";
	$sql_args[':start_ts'] = $start_ts;
	$sql_args[':end_ts'] = $end_ts;
	$sql_args[':excluded_rid'] = $excluded_rid;
	$stmt = sirius_sql_query($sql, $sql_args);
	$hr = $stmt->fetchAssoc();
	$snapshot['active_users'] = $hr['c'];

	// Number of active contract templates
	$sql_args = array();
	$sql = "select count(*) as c ";
	$sql .= "from node ";
	$sql .= "left join field_data_field_sirius_active on field_data_field_sirius_active.entity_type = 'node' and field_data_field_sirius_active.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
	$sql .= "where type = 'grievance_contract_template' ";
	$sql .= "and (field_sirius_active_value is null or field_sirius_active_value = 'Yes') ";
	if ($domain_nid) {
		$sql .= "and field_sirius_domain_target_id = :domain_nid ";
		$sql_args[':domain_nid'] = $domain_nid;
	} else {
		$sql .= "and field_sirius_domain_target_id is null ";
	}
	$stmt = sirius_sql_query($sql, $sql_args);
	$hr = $stmt->fetchAssoc();
	$snapshot['active_contracts'] = $hr['c'];

	// Number of unresolved grievances at an inactive employer.  (This should be zero, but its a good thing to monitor).
	$sql_args = array();
	$sql = "select count(*) as c ";
	$sql .= "from node ";
	$sql .= "left join field_data_field_grievance_status on field_data_field_grievance_status.entity_type = 'node' and field_data_field_grievance_status.entity_id = node.nid ";
	$sql .= "left join field_data_field_grievance_shop on field_data_field_grievance_shop.entity_type = 'node' and field_data_field_grievance_shop.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
	$sql .= "where type = 'grievance' ";
	$sql .= "and field_grievance_status_tid in (:open_status_tids) ";
	$sql_args[':open_status_tids'] = $open_status_tids;
	$sql .= "and field_grievance_shop_target_id in (:inactive_employers) ";
	$sql_args[':inactive_employers'] = $inactive_employers;
	if ($domain_nid) {
		$sql .= "and field_sirius_domain_target_id = :domain_nid ";
		$sql_args[':domain_nid'] = $domain_nid;
	} else {
		$sql .= "and field_sirius_domain_target_id is null ";
	}
	$stmt = sirius_sql_query($sql, $sql_args);
	$hr = $stmt->fetchAssoc();
	$snapshot['open_grievance_inactive_employer'] = $hr['c'];


	return $snapshot;
}

