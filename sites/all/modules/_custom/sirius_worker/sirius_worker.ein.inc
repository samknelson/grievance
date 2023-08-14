<?php

function sirius_worker_employee_lookup_all_employers($worker_nid) {
	$domain_nid = sirius_domain_nid();

	$sql_args = array();
	$sql = "select node.nid as employee_nid, ";
	$sql .= "field_grievance_shop_target_id as employer_nid, ";
	$sql .= "employer_node.title as employer_name, ";
	$sql .= "field_sirius_id_value as ein ";
	$sql .= "from node ";
	$sql .= "left join field_data_field_grievance_shop on field_data_field_grievance_shop.entity_type = 'node' and field_data_field_grievance_shop.entity_id = node.nid ";
	$sql .= "left join node employer_node on employer_node.nid = field_grievance_shop_target_id ";
	$sql .= "left join field_data_field_sirius_worker on field_data_field_sirius_worker.entity_type = 'node' and field_data_field_sirius_worker.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_id on field_data_field_sirius_id.entity_type = 'node' and field_data_field_sirius_id.entity_id = node.nid ";
	if ($domain_nid) {
		$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
	}
	$sql .= "where node.type = 'sirius_employee' ";
	$sql .= "and field_sirius_worker_target_id = :worker_nid ";
	$sql_args[':worker_nid'] = $worker_nid;
	if ($domain_nid) {
		$sql_args[':domain_nid'] = $domain_nid;
		$sql .= "and field_sirius_domain_target_id = :domain_nid ";
	}

	$stmt = sirius_sql_query($sql, $sql_args);
	$result = array();
	while ($hr = $stmt->fetchAssoc()) {
		$result[] = $hr;
	}
	return $result;
}


function sirius_worker_employee_lookup($worker_nid, $employer_nid) {
	$domain_nid = sirius_domain_nid();

	$sql_args = array();
	$sql = "select nid from node ";
	$sql .= "left join field_data_field_grievance_shop on field_data_field_grievance_shop.entity_type = 'node' and field_data_field_grievance_shop.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_worker on field_data_field_sirius_worker.entity_type = 'node' and field_data_field_sirius_worker.entity_id = node.nid ";
	if ($domain_nid) {
		$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
	}
	$sql .= "where node.type = 'sirius_employee' ";
	$sql .= "and field_grievance_shop_target_id = :employer_nid ";
	$sql_args[':employer_nid'] = $employer_nid;
	$sql .= "and field_sirius_worker_target_id = :worker_nid ";
	$sql_args[':worker_nid'] = $worker_nid;
	if ($domain_nid) {
		$sql_args[':domain_nid'] = $domain_nid;
		$sql .= "and field_sirius_domain_target_id = :domain_nid ";
	}

	$stmt = sirius_sql_query($sql, $sql_args);
	$hr = $stmt->fetchAssoc();

	return $hr['nid'];
}

function sirius_worker_ein_lookup($employer_nid, $ein) {
	if (!($employer_nid > 0)) { return NULL; }
	if (!$ein) { return NULL; }
	$domain_nid = sirius_domain_nid();

	$sql_args = array();
	$sql = "select field_sirius_worker_target_id from node ";
	$sql .= "left join field_data_field_grievance_shop on field_data_field_grievance_shop.entity_type = 'node' and field_data_field_grievance_shop.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_worker on field_data_field_sirius_worker.entity_type = 'node' and field_data_field_sirius_worker.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_id on field_data_field_sirius_id.entity_type = 'node' and field_data_field_sirius_id.entity_id = node.nid ";
	if ($domain_nid) {
		$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
	}
	$sql .= "where node.type = 'sirius_employee' ";
	$sql .= "and field_grievance_shop_target_id = :employer_nid ";
	$sql_args[':employer_nid'] = $employer_nid;
	$sql .= "and field_sirius_id_value = :ein ";
	$sql_args[':ein'] = $ein;
	if ($domain_nid) {
		$sql_args[':domain_nid'] = $domain_nid;
		$sql .= "and field_sirius_domain_target_id = :domain_nid ";
	}

	$stmt = sirius_sql_query($sql, $sql_args);
	$hr = $stmt->fetchAssoc();

	return $hr['field_sirius_worker_target_id'];
}

function sirius_worker_ein_set($ssn, $employer_nid, $ein) {
	$domain_nid = sirius_domain_nid();

	if (!$ssn) { return array('success' => FALSE, 'msg' => 'Please enter an SSN.'); }
	$ssn = sirius_ssn_format($ssn);
	if (!sirius_ssn_validate($ssn)) { return array('success' => FALSE, 'msg' => 'Please enter a valid SSN.'); }
	if (!$employer_nid) { return array('success' => FALSE, 'msg' => 'Please enter an employer.'); }
	if (!$ein) { return array('success' => FALSE, 'msg' => 'Please enter an EIN.'); }

	$worker_node = sirius_worker_get_by_ssn($ssn);
	if (!$worker_node) { return array('success' => FALSE, 'msg' => "No worker found with the SSN $ssn"); }

	$employee_nid = sirius_worker_employee_lookup($worker_node->nid, $employer_nid);
	if ($employee_nid) {
		$employee_node = node_load($employee_nid);
	} else {
		global $user;
		$employer_node = node_load($employer_nid);
		$employee_node = new stdClass();
		$employee_node->title = substr($worker_node->title . ' @ ' . $employer_node->title, 0, 250);
		$employee_node->type = 'sirius_employee';
		$employee_node->language = LANGUAGE_NONE;
		$employee_node->uid = $user->uid;
		$employee_node->status = 1;
		$employee_node->promote = 0;
		$employee_node->comment = 0;
		$employee_node->field_grievance_shop['und'][0]['target_id'] = $employer_nid;
		$employee_node->field_sirius_worker['und'][0]['target_id'] = $worker_node->nid;
		if ($domain_nid) { $employee_node->field_sirius_domain['und'][0]['target_id'] = $domain_nid; }
	}

	$old_ein = $employee_node->field_sirius_id['und'][0]['value'];
	if ($ein == $old_ein) { return array('success' => TRUE, 'msg' => 'Worker ' . $worker_node->title . ': EIN unchanged as ' . $ein); }
	$employee_node->field_sirius_id['und'][0]['value'] = $ein;
  node_object_prepare($employee_node);
  $payment_node = node_submit($employee_node);
  node_save($employee_node);

	if ($old_ein) { return array('success' => TRUE, 'msg' => 'Worker ' . $worker_node->title . ': EIN changed from ' . $old_ein . ' to ' . $ein); }
	else { return array('success' => TRUE, 'msg' => 'Worker ' . $worker_node->title . ': EIN set to ' . $ein); } 
}

function sirius_worker_ein_node_view($node, $view_mode, $langcode) {
	if ($node->type != 'sirius_worker') { return; }
	if (!$node->content['eins']) { return; }

	$employee_records = sirius_worker_employee_lookup_all_employers($node->nid);
	if (!$employee_records) { 
		unset($node->content['eins']);
		return;
	}

	$html = '';
	foreach ($employee_records as $employee_record) {
		if (!$employee_record['ein']) { continue; }
		if (!$employee_record['employer_name']) { continue; }
		$html .= sirius_fakerender_field($employee_record['employer_name'] . ': ', $employee_record['ein']);
	}
	if (!$html) {
		unset($node->content['eins']);
		return;
	}
	$node->content['eins'][0]['#markup'] = $html;
}

