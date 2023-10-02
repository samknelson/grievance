<?php


function sirius_trust_benefits($active = TRUE) {
	$query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_trust_benefit');
  $query->fieldCondition('field_sirius_domain', 'target_id', sirius_domain_nid());
  if ($active) {
	  $query->fieldCondition('field_sirius_active', 'value', 'Yes');
  }
  $query->propertyOrderBy('title');
  $stmt = $query->execute();
  if (!$stmt['node']) { return array(); }
  return node_load_multiple(array_keys($stmt['node']));
}

function sirius_trust_benefit_options($active = TRUE) {
	$nodes = sirius_trust_benefits($active);
	$options = array();
	foreach ($nodes as $nid => $node) { $options[$nid] = $node->title; }
	return $options;
}

function sirius_trust_worker_benefit_node_form_alter(&$form, &$form_state, $form_id) {
	// Admins can see the full form
	if (user_access('sirius admin')) {
		drupal_set_message("Administrative user: Full form is displayed. Please use caution.", 'warning');
		return;
	}

	// "Active" field is computed, so don't edit manually
	if ($form['field_sirius_active']) { $form['field_sirius_active']['#access'] = FALSE;  }

	// New record or existing record?
	$wb_node = $form['#entity'];
	if ($wb_node && $wb_node->nid) {
		$is_edit = TRUE;
	}

	// You can never change the worker or the relationship
	$form['field_sirius_worker']['#disabled'] = TRUE;
	$form['field_sirius_contact_relation']['#disabled'] = TRUE;
	$form['field_grievance_shop']['#disabled'] = TRUE;
	$form['field_sirius_trust_policy']['#disabled'] = TRUE;

	if ($is_edit) {
		// Existing record: You can't change the type or employer
		$form['field_sirius_trust_benefit']['#access'] = FALSE;
		$form['field_grievance_shop']['#access'] = FALSE;
	} else {
		// New record: Relationship type is required
		$form['field_sirius_trust_benefit']['#required'] = TRUE;
		$form['field_sirius_trust_benefit']['und']['#required'] = TRUE;

		// Include some confirmation text
		/*
		$contact_node = node_load($_REQUEST['sirius_contact']);
		$other_contact_node = node_load($_REQUEST['sirius_contact_alt']);
		if (!$contact_node || !$other_contact_node) {
			drupal_set_message('Must receive valid contact nodes.', 'error');
			$form['#access'] = FALSE;
			return;
		}

		drupal_set_message("Creating a relationship FROM " . $contact_node->title . " TO " . $other_contact_node->title, 'warning');
		*/
	}
}

function sirius_trust_worker_benefits_search($params) {
	$domain_nid = sirius_domain_nid();

	$sql_args = array();
	$sql = "select nid from node ";
	$sql .= "left join field_data_field_sirius_worker on field_data_field_sirius_worker.entity_type = 'node' and field_data_field_sirius_worker.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_trust_benefit on field_data_field_sirius_trust_benefit.entity_type = 'node' and field_data_field_sirius_trust_benefit.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_active on field_data_field_sirius_active.entity_type = 'node' and field_data_field_sirius_active.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_date_start on field_data_field_sirius_date_start.entity_type = 'node' and field_data_field_sirius_date_start.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_date_end on field_data_field_sirius_date_end.entity_type = 'node' and field_data_field_sirius_date_end.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_contact_relation on field_data_field_sirius_contact_relation.entity_type = 'node' and field_data_field_sirius_contact_relation.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
	$sql .= "where node.type = 'sirius_trust_worker_benefit' ";
	$sql .= "and field_sirius_domain_target_id = :domain_nid ";
	$sql_args[':domain_nid'] = $domain_nid;
  if ($params['worker_nid']) {
  	$sql .= "and field_sirius_worker_target_id = :worker_nid ";
  	$sql_args[':worker_nid'] = $params['worker_nid'];
  }
  if ($params['active']) {
  	$sql .= "and field_sirius_active_value = :active ";
  	$sql_args[':active'] = $params['active'];
  }
  if ($params['ts']) {
  	$sql .= "and field_sirius_date_start_value <= :date ";
  	$sql .= "and (field_sirius_date_end_value >= :date or field_sirius_date_end_value is null) ";
  	$sql_args[':date'] = gmdate('Y-m-d H:i:s', $params['ts']);
  }
  if ($params['exclude_nid']) {
  	$sql .= "and nid != :exclude_nid ";
  	$sql_args[':exclude_nid'] = $params['exclude_nid'];
  }
  if ($params['relationship_nids']) {
  	$sql .= "and field_sirius_contact_relation_target_id in (:relationship_nids) ";
  	$sql_args[':relationship_nids'] = $params['relationship_nids'];
  }
  if ($params['relationship_nid']) {
  	$sql .= "and field_sirius_contact_relation_target_id = :relationship_nid ";
  	$sql_args[':relationship_nid'] = $params['relationship_nid'];
  }
  if ($params['benefit_nids']) {
  	$sql .= "and field_sirius_trust_benefit_target_id in (:benefit_nids) ";
  	$sql_args[':benefit_nids'] = $params['benefit_nids'];
  }
  $sql .= "order by field_sirius_date_start_value desc ";
  $limit = $params['limit'];
  if (!$limit) { $limit = 100; }
  $sql .= "limit $limit";

  $stmt = sirius_sql_query($sql, $sql_args);
  $election_nids = array();
  while ($hr = $stmt->fetchAssoc()) {
  	$election_nids[] = $hr['nid'];
  }

  if (!$election_nids) { return array(); }
  
  if ($params['nids_only']) { return $election_nids; }
	return node_load_multiple($election_nids);
}

function sirius_trust_worker_benefits($worker_node, $ts = NULL) {
	$params = array();
	$params['worker_nid'] = $worker_node->nid;
	if ($ts) {
		$params['ts'] = $ts; 
	} else {
		$params['active'] = 'Yes';
	}
	return sirius_trust_worker_benefits_search($params);
}

function sirius_trust_worker_benefit_node_presave($wb_node) {
	// Relationship is active only if it started before now, and it either hasn't ended yet, or it ended after now.
	$start_ts = sirius_fieldval($wb_node, 'field_sirius_date_start', 'ts');
	$end_ts = sirius_fieldval($wb_node, 'field_sirius_date_end', 'ts');
	$now_ts = time();

	if ($start_ts <= $now_ts && ($end_ts < 1 || $end_ts >= $now_ts)) {
		$wb_node->field_sirius_active['und'][0]['value'] = 'Yes';
	} else {
		$wb_node->field_sirius_active['und'][0]['value'] = 'No';
	}
}

function sirius_trust_benefit_get_by_id($id) {
  if (!$id) { return NULL; }

  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_trust_benefit');
  $query->fieldCondition('field_sirius_id', 'value', $id);
  $domain_nid = sirius_domain_nid();
  if ($domain_nid) { $query->fieldCondition('field_sirius_domain', 'target_id', $domain_nid); }
  $query->propertyOrderBy('nid', 'desc');
  $stmt = $query->execute();
  if (!$stmt['node']) { return NULL; }

  return node_load(array_keys($stmt['node'])[0]);
}

/**
 * Get the date that a worker started receiving a benefit. They must have had continuous coverage until the timestamp specified (defaults to today.)
 */

function sirius_trust_worker_benefit_start_ts($worker_node, $benefit_nids, $ts = NULL) {
	if (!$ts) { $ts = time(); }

	// We take either a single benefit or an array.
	if (!is_array($benefit_nids)) { $benefit_nids = array($benefit_nids); }

	$params = array();
	$params['worker_nid'] = $worker_node->nid;
	$params['benefit_nids'] = $benefit_nids;
	$wb_nodes = sirius_trust_worker_benefits_search($params);

	if (!$wb_nodes) { return NULL; }

	$grace = 25*60*60;

	$found = TRUE;
	$found_ts = $ts;
	$count = 0;
	while ($found) {
		++$count; 
		if ($count > 1000) { break; }

		$found = FALSE; 
		foreach ($wb_nodes as $wb_nid => $wb_node) {
			$start_ts = strtotime($wb_node->field_sirius_date_start['und'][0]['value']);
			$end_ts = strtotime($wb_node->field_sirius_date_end['und'][0]['value']);
			if ($start_ts === FALSE) { continue; }
			if ($end_ts === FALSE) { $end_ts = $ts + 1000000; }

			if ($start_ts <= $found_ts && $end_ts >= $found_ts) {
				$found = TRUE; 
				$found_ts = $start_ts - $grace;
				unset($wb_nodes[$wb_nid]);
				break;
			}
		}
	}

	if ($found_ts == $ts) { return NULL; }

	$found_ts += $grace;

	return $found_ts;
}

function sirius_trust_worker_benefit_scan($mode = 'test') {
	sirius_minilog("Worker benefit scan: Starting (mode = $mode)");

	$mode = 'test';
	sirius_minilog("FEATURE UNDER DEVELOPMENT: Forcing mode = $mode", 'warning');


	$count = 0;
	$count_ok = 0;
	$count_invalid = 0;
	$count_warning = 0;

	$wb_nids = sirius_trust_worker_benefits_search(array('active' => 'Yes', 'nids_only' => TRUE));
	$wb_invalid_nids = array();

	foreach ($wb_nids as $wb_nid) {
		++$count;

		$wb_node = node_load($wb_nid);
		$wb_json_orig = $wb_json = sirius_json_get($wb_node);
		$need_save = FALSE;
		sirius_minilog("Scanning worker benefit " . l($wb_node->title . " ($wb_nid)", "node/$wb_nid"));

		$result = sirius_trust_worker_benefit_check_record($wb_node);
		if ($result['success']) {
			++$count_ok;

			if (!$result['warnings']) {
				sirius_minilog("-- Benefit OK");
				unset($wb_json['scan']['warnings']);
			} else {
				++$count_warning;
				foreach ($result['warnings'] as $msg) {
					sirius_minilog("-- Benefit warning: $msg");
				}
				$wb_json['scan']['warnings'] = $result['warnings'];
			}
		} else {
			++$count_invalid;
			sirius_minilog(" -- Benefit invalid: " . $result['msg'] . " The benefit will be expired.");

			$date = gmdate('Y-m-d', strtotime('-1 day'));
			$wb_node->field_sirius_date_end['und'][0]['value'] = $date;
			$wb_json['scan']['msg'] = $result['msg'];
			$wb_json['scan']['ts'] = time();
			$need_save = TRUE;
			if ($mode == 'live') {
				$log_node = sirius_log(
					'trust:wb:scan', 
					'terminated',
					'Automated scan terminated benefit: ' . $result['msg'], 
					'Automated scan terminated benefit: ' . $result['msg'], 
					NULL,
					array($wb_node->nid)
				);
			}
		}

		if (sirius_revision_changed_impl($wb_json, $wb_json_orig, 10) || sirius_revision_changed_impl($wb_json_orig, $wb_json, 10)) {
			$need_save = TRUE;
		}

		if ($need_save) {
			sirius_json_set($wb_node, $wb_json);
			if ($mode == 'live') {
				node_save($wb_node);
				sirius_minilog("-- Worker benefit updated.");
			} else {
				sirius_minilog("-- (Test mode, worker benefit not saved.)");
			}
		}
	}

	return array('success' => TRUE, 'msg' => "Scanned $count worker benefit records. Found $count_ok ok, and expired $count_invalid invalid records.");
}

function sirius_trust_worker_benefit_check_record($wb_node) {
	$need_save = FALSE;

	$subscriber_worker_node  = node_load($wb_node->field_sirius_worker['und'][0]['target_id']);
	if (!$subscriber_worker_node) { return array('success' => FALSE, 'msg' => 'No subscriber worker record.'); }

	$employer_node = node_load($wb_node->field_grievance_shop['und'][0]['target_id']);

	$policy_node = sirius_ledger_policy_lookup($subscriber_worker_node, $employer_node);
	if (!$policy_node) { return array('success' => FALSE, 'msg' => 'No policy record.'); }

	$relationship_node = node_load($form_state['values']['relationship_nid']);
	if ($relationship_node) {
		$dependent_contact_nid = $relationship_node->field_sirius_contact_alt['und'][0]['target_id'];
		$dependent_worker_node = sirius_worker_get_by_contact_nid($dependent_contact_nid);
	} else {
		$dependent_worker_node = $subscriber_worker_node;
	}
	if (!$dependent_worker_node) { return array('success' => FALSE, 'msg' => 'No dependent worker record.'); }

	$benefit_node = node_load($wb_node->field_sirius_trust_benefit['und'][0]['target_id']);
	if (!$benefit_node) { return array('success' => FALSE, 'msg' => 'No benefit record.'); }


	$wb_json = $wb_json_orig = sirius_json_get($wb_node);
	$eligibility_plugins = sirius_trust_eligibility_plugins_enabled($policy_node, $benefit_node);
	$warnings = array();
	foreach ($eligibility_plugins as $plugin_key) {
		$result = sirius_trust_eligibility_plugin_check($policy_node, $benefit_node, $subscriber_worker_node, $relationship_node, NULL, $plugin_key);
		if (!$result['success']) {
			$plugin_info = sirius_trust_eligibility_plugin_info($plugin_key);
			$result['msg'] = "Failed eligibility plugin " . $plugin_info['title'] . ': ' . $result['msg'];
			return $result;
		}

		if ($result['warning']) {
			$warnings[$plugin_key] = $result['msg'];
		}
	}

	return array('success' => TRUE, 'warnings' => $warnings);
}
