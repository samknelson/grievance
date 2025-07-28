<?php

sirius_require('sirius_trust', 'sirius_trust.wb_scan.inc');
sirius_require('sirius_trust', 'sirius_trust.wb_scan_start.inc');
sirius_require('sirius_trust', 'sirius_trust.wb_scan_stop.inc');
sirius_require('sirius_trust', 'sirius_trust.wb_scan_changelog.inc');

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
	$form['field_sirius_trust_subscriber']['#disabled'] = TRUE;
	$form['field_sirius_trust_election']['#disabled'] = TRUE;

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

function sirius_trust_worker_benefits_check($worker_nid, $benefit_nid) {
	$params = array();
	$params['worker_nid'] = $worker_nid;
	$params['benefit_nid'] = $benefit_node;
	$params['active'] = 'Yes';
	$params['nids_only'] = TRUE;
	return sirius_trust_worker_benefits_search($params);
}

function sirius_trust_worker_benefits_search($params) {
	$domain_nid = sirius_domain_nid();

	$sql_args = array();
	$sql = "select node.nid from node ";
	$sql .= "left join field_data_field_sirius_worker on field_data_field_sirius_worker.entity_type = 'node' and field_data_field_sirius_worker.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_trust_subscriber on field_data_field_sirius_trust_subscriber.entity_type = 'node' and field_data_field_sirius_trust_subscriber.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_trust_election on field_data_field_sirius_trust_election.entity_type = 'node' and field_data_field_sirius_trust_election.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_trust_benefit on field_data_field_sirius_trust_benefit.entity_type = 'node' and field_data_field_sirius_trust_benefit.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_active on field_data_field_sirius_active.entity_type = 'node' and field_data_field_sirius_active.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_date_start on field_data_field_sirius_date_start.entity_type = 'node' and field_data_field_sirius_date_start.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_date_end on field_data_field_sirius_date_end.entity_type = 'node' and field_data_field_sirius_date_end.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_contact_relation on field_data_field_sirius_contact_relation.entity_type = 'node' and field_data_field_sirius_contact_relation.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
	$sql .= "left join node benefit_node on benefit_node.nid = field_sirius_trust_benefit_target_id ";
	$sql .= "left join field_data_field_sirius_trust_benefit_type on field_data_field_sirius_trust_benefit_type.entity_type = 'node' and field_data_field_sirius_trust_benefit_type.entity_id = benefit_node.nid ";
	$sql .= "where node.type = 'sirius_trust_worker_benefit' ";
	$sql .= "and field_sirius_domain_target_id = :domain_nid ";
	$sql_args[':domain_nid'] = $domain_nid;
  if ($params['worker_nid']) {
  	$sql .= "and field_sirius_worker_target_id = :worker_nid ";
  	$sql_args[':worker_nid'] = $params['worker_nid'];
  }
  if ($params['subscriber_nid']) {
  	$sql .= "and field_sirius_trust_subscriber_target_id = :subscriber_nid ";
  	$sql_args[':subscriber_nid'] = $params['subscriber_nid'];
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
  if ($params['end_date_range']) {
  	$sql .= "and (field_sirius_date_end_value >= :end_date_range_start and field_sirius_date_end_value <= :end_date_range_end) ";
  	$sql_args[':end_date_range_start'] = date('Y-m-d 00:00:00', strtotime($params['end_date_range']['start']));
  	$sql_args[':end_date_range_end'] = date('Y-m-d 00:00:00', strtotime($params['end_date_range']['end']));
  }
  if ($params['start_date_range']) {
  	$sql .= "and (field_sirius_date_start_value >= :start_date_range_start and field_sirius_date_start_value <= :start_date_range_end) ";
  	$sql_args[':start_date_range_start'] = date('Y-m-d 00:00:00', strtotime($params['start_date_range']['start']));
  	$sql_args[':start_date_range_end'] = date('Y-m-d 00:00:00', strtotime($params['start_date_range']['end']));
  }
  if ($params['end_date']) {
  	$sql .= "and field_sirius_date_end_value = :end_date ";
  	$sql_args[':end_date'] = date('Y-m-d 00:00:00', strtotime($params['end_date']));
  }
  if ($params['exclude_nid']) {
  	$sql .= "and nid != :exclude_nid ";
  	$sql_args[':exclude_nid'] = $params['exclude_nid'];
  }
  if ($params['relationship_nids']) {
  	$sql .= "and field_sirius_contact_relation_target_id in (:relationship_nids) ";
  	$sql_args[':relationship_nids'] = $params['relationship_nids'];
  }
  if ($params['exclude_self']) {
  	$sql .= "and field_sirius_worker_target_id != field_sirius_trust_subscriber_target_id ";
  }
  if ($params['relationship_nid']) {
  	if ($params['relationship_nid'] == 'NULL') {
	  	$sql .= "and field_sirius_contact_relation_target_id IS NULL ";
  	} else {
	  	$sql .= "and field_sirius_contact_relation_target_id = :relationship_nid ";
	  	$sql_args[':relationship_nid'] = $params['relationship_nid'];
	  }
  }
  if ($params['benefit_nids']) {
  	$sql .= "and field_sirius_trust_benefit_target_id in (:benefit_nids) ";
  	$sql_args[':benefit_nids'] = $params['benefit_nids'];
  }
  if ($params['benefit_nid']) {
  	$sql .= "and field_sirius_trust_benefit_target_id = :benefit_nid ";
  	$sql_args[':benefit_nid'] = $params['benefit_nid'];
  }
  if ($params['election_nid']) {
  	$sql .= "and field_sirius_trust_election_target_id = :election_nid ";
  	$sql_args[':election_nid'] = $params['election_nid'];
  }
  if ($params['benefit_types']) {
  	$sql .= "and field_sirius_trust_benefit_type_tid in (:benefit_types) ";
  	$sql_args[':benefit_types'] = $params['benefit_types'];
  }
  if ($params['benefit_type']) {
  	$sql .= "and field_sirius_trust_benefit_type_tid = :benefit_type ";
  	$sql_args[':benefit_type'] = $params['benefit_type'];
  }
  $sql .= "order by field_sirius_date_start_value desc ";
  $limit = $params['limit'];
  if (!$limit) { $limit = 100; }
  $sql .= "limit $limit";

  $stmt = sirius_sql_query($sql, $sql_args);
  $wb_nids = array();
  while ($hr = $stmt->fetchAssoc()) {
  	$wb_nids[] = $hr['nid'];
  }

  if (!$wb_nids) { return array(); }
  
  if ($params['nids_only']) { return $wb_nids; }
	return node_load_multiple($wb_nids);
}

function sirius_trust_worker_benefits($worker_node, $ts = NULL) {
	$params = array();
	$params['worker_nid'] = $worker_node->nid;
	if ($ts) {
		$params['ts'] = $ts; 
	} else {
		$params['active'] = 'Yes';
	}
	$wb_nodes = sirius_trust_worker_benefits_search($params);

	// Sort by benefit priority
	$benefit_nids = array();
	foreach ($wb_nodes as $wb_nid => $wb_node) { $benefit_nids[] = $wb_node->field_sirius_trust_benefit['und'][0]['target_id']; }
	$benefit_nids = array_unique($benefit_nids);
	$benefit_weights = array_flip(array_keys(sirius_trust_benefits_list_in_order($benefit_nids)));

 	uksort($wb_nodes, function($wb_nid_a, $wb_nid_b) use ($wb_nodes, $benefit_weights) {
 		$weight_a = $benefit_weights[$wb_nodes[$wb_nid_a]->field_sirius_trust_benefit['und'][0]['target_id']];
 		$weight_b = $benefit_weights[$wb_nodes[$wb_nid_b]->field_sirius_trust_benefit['und'][0]['target_id']];
 		return $weight_a <=> $weight_b;
 	});

	return $wb_nodes;
}





function sirius_trust_worker_benefits_sort_by_date($wb_nodes) {
	// Gather susbscriber, contact names
	foreach ($wb_nodes as $wb_nid => $wb_node) {
		$relationship_node = node_load($wb_node->field_sirius_contact_relation['und'][0]['target_id']);
		$wb_nodes[$wb_nid]->sort_dependent_name = sirius_node_title($relationship_node->field_sirius_contact_alt['und'][0]['target_id']);
		$wb_nodes[$wb_nid]->sort_subscriber_name = sirius_node_title($wb_node->field_sirius_trust_subscriber['und'][0]['target_id']);
		$wb_nodes[$wb_nid]->sort_benefit_name = sirius_node_title($wb_node->field_sirius_trust_benefit['und'][0]['target_id']);
	}

  uksort($wb_nodes, function($a, $b) use ($wb_nodes) {

    // $active_a = $wb_nodes[$a]->field_sirius_active['und'][0]['value'];
    // $active_b = $wb_nodes[$b]->field_sirius_active['und'][0]['value'];

  	$subscriber_name_a = $wb_nodes[$a]->sort_subscriber_name;
  	$subscriber_name_b = $wb_nodes[$b]->sort_subscriber_name;

  	$dependent_name_a = $wb_nodes[$a]->sort_dependent_name;
  	$dependent_name_b = $wb_nodes[$b]->sort_dependent_name;

  	$benefit_name_a = $wb_nodes[$a]->sort_benefit_name;
  	$benefit_name_b = $wb_nodes[$b]->sort_benefit_name;

    $start_a = $wb_nodes[$a]->field_sirius_date_start['und'][0]['value'];
    $start_b = $wb_nodes[$b]->field_sirius_date_start['und'][0]['value'];

    $end_a = $wb_nodes[$a]->field_sirius_date_end['und'][0]['value'];
    $end_b = $wb_nodes[$b]->field_sirius_date_end['und'][0]['value'];

    // drupal_set_message("Comparing: $dependent_name_a / $benefit_name_a / $start_a / $end_a / $active_a to $dependent_name_b / $benefit_name_b / $start_b / $end_b / $active_b");

    // Sort first by subscriber
  	$result = strcmp($subscriber_name_a, $subscriber_name_b);
  	if ($result) { return $result; }

  	// Next by dependent
  	$result = strcmp($dependent_name_a, $dependent_name_b);
  	if ($result) { return $result; }

  	// Next by benefit
  	$result = strcmp($benefit_name_a, $benefit_name_b);
  	if ($result) { return $result; }

    // if ( ($active_a == 'Yes') && ($active_b != 'Yes') ) { return -1; }
    // if ( ($active_a != 'Yes') && ($active_b == 'Yes') ) { return 1; }

  	// Next by end date
    if ($end_a && !$end_b) { return 1; }
    if ($end_b && !$end_a) { return -1; }

    $result = -1 * strcmp($end_a, $end_b);
    if ($result) { return $result; }

    // Next by start date
    $result = -1 * strcmp($start_a, $start_b);
    if ($result) { return $result; }
    return 0;
    // return strcmp($start_a, $start_b);
  });

  return $wb_nodes;
}

function sirius_trust_worker_benefit_node_presave($wb_node) {
	// Benefit is active only if it started before now, and it either hasn't ended yet, or it ended after now.
	$start_ts = sirius_fieldval($wb_node, 'field_sirius_date_start', 'ts');
	$end_ts = sirius_fieldval($wb_node, 'field_sirius_date_end', 'ts');
	$now_ts = time();

	if ($start_ts <= $now_ts && ($end_ts < 1 || $end_ts >= $now_ts)) {
		$wb_node->field_sirius_active['und'][0]['value'] = 'Yes';
	} else {
		$wb_node->field_sirius_active['und'][0]['value'] = 'No';
	}

	// Subscriber is either the dependent or the relationship target
	$relationship_node = node_load($wb_node->field_sirius_contact_relation['und'][0]['target_id']);
	if ($relationship_node) {
		$subscriber_contact_nid = $relationship_node->field_sirius_contact['und'][0]['target_id'];
		$subscriber_worker_nid = sirius_worker_get_by_contact_nid($subscriber_contact_nid, TRUE);
	}
	if (!$subscriber_worker_nid) {
		$subscriber_worker_nid = $wb_node->field_sirius_worker['und'][0]['target_id'];
	}
	$wb_node->field_sirius_trust_subscriber['und'][0]['target_id'] = $subscriber_worker_nid;

	$election_nid = sirius_trust_wb_find_election_nid($wb_node);
	if ($election_nid) {
		$wb_node->field_sirius_trust_election['und'][0]['target_id'] = $election_nid;
	} else {
		unset($wb_node->field_sirius_trust_election['und'][0]);
	}
}

function sirius_trust_wb_find_election_nid($wb_node) {
	// Need a subscriber and a benefit
	$subscriber_nid = $wb_node->field_sirius_trust_subscriber['und'][0]['target_id'];
	if (!$subscriber_nid) { return NULL; }

	$benefit_nid = $wb_node->field_sirius_trust_benefit['und'][0]['target_id'];
	if (!$benefit_nid) { return NULL; }

	$end_date = $wb_node->field_sirius_date_end['und'][0]['value'];

	// We want the most recent election which:
	// -- Is tied to the subscriber
	// -- Contains the required benefit
	// -- The election starts before the beneft ends

	$sql = "select nid from node ";
	$sql .= "left join field_data_field_sirius_worker on field_data_field_sirius_worker.entity_type = 'node' and field_data_field_sirius_worker.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_trust_benefits on field_data_field_sirius_trust_benefits.entity_type = 'node' and field_data_field_sirius_trust_benefits.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_date_start on field_data_field_sirius_date_start.entity_type = 'node' and field_data_field_sirius_date_start.entity_id = node.nid ";
	$sql .= "where node.type = 'sirius_trust_worker_election' ";

	$sql .= "and field_sirius_worker_target_id = :subscriber_nid ";
	$sql_args[':subscriber_nid'] = $subscriber_nid;

	$sql .= "and field_sirius_trust_benefits_target_id = :benefit_nid ";
	$sql_args[':benefit_nid'] = $benefit_nid;

	if ($end_date) {
		$sql .= "and field_sirius_date_start_value <= :end_date ";
		$sql_args[':end_date'] = $end_date;
	}

	$sql .= "order by field_sirius_date_start_value desc ";
	$sql .= "limit 1 ";

	$stmt = sirius_sql_query($sql, $sql_args);
	$hr = $stmt->fetchAssoc();
	return $hr['nid'];
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





























function sirius_trust_fetch_worker_benefits($worker_node, $direction, $params = array()) {
  if ($direction == 'direct') {
  	$params['worker_nid'] = $worker_node->nid;
  } else {
  	/*
	  $relationship_params = array();
  	$relationship_params['contact_nid'] = $worker_node->field_sirius_contact['und'][0]['target_id'];
  	$relationship_params['nids_only'] = TRUE;
		$relationship_nids = sirius_contact_relationships($relationship_params);
		if (!$relationship_nids) { return array(); }
		$params['relationship_nids'] = $relationship_nids;
		*/
		$params['subscriber_nid'] = $worker_node->nid;
		$params['exclude_self'] = TRUE;
  }

	return sirius_trust_worker_benefits_search($params);
}

function sirius_trust_render_worker_benefits($worker_node, $direction, $wb_nodes, $options = array()) {
	if (!$wb_nodes) {
		return '<p>' . t('No records found.') . '</p>';
	}

	$wb_nodes = sirius_trust_worker_benefits_sort_by_date($wb_nodes);

	static $relationship_type_terms;
	if (!isset($relationship_type_terms)) { $relationship_type_terms = sirius_taxonomy_load('sirius_contact_relationship_types', 'tid', 'full'); }

	$is_staff = Sirius_Trust_Access::getInstance()->access('staff', array('worker_node' => $worker_node));

  foreach ($wb_nodes as $wb_nid => $wb_node) {
  	$benefit_nid = $wb_node->field_sirius_trust_benefit['und'][0]['target_id'];
  	$benefit_node = node_load($benefit_nid);

  	$beneficiary_worker_node = node_load($wb_node->field_sirius_worker['und'][0]['target_id']);
  	$beneficiary_name = $beneficiary_worker_node->title;

  	$relationship_node = node_load($wb_node->field_sirius_contact_relation['und'][0]['target_id']);

  	$subscriber_worker_node = NULL;
  	$relationship_type_name = '';
  	if ($relationship_node) {
  		$subscriber_contact_nid = $relationship_node->field_sirius_contact['und'][0]['target_id'];
			$subscriber_worker_node = sirius_worker_get_by_contact_nid($subscriber_contact_nid);
			$relationship_type_tid = $relationship_node->field_sirius_contact_reltype['und'][0]['tid'];
			$relationship_type_term = $relationship_type_terms[$relationship_type_tid];
			if ($relationship_type_term) {
				$relationship_type_name = $relationship_type_term->name . ' to';
			}
  	} 

  	if ($subscriber_worker_node) {
  		$subscriber_name = $subscriber_worker_node->title;
  	} else {
  		$subscriber_name = $beneficiary_worker_node->title;
  		$relationship_type_name = '[self]';
  	}

  	$tools = array();
  	$tools[] = l('Details', "node/$benefit_nid", array('attributes' => array('class' => array('tool-button'))));

  	if ($is_staff) {
	  	$tools[] = l('Benefit', "node/$wb_nid", array('attributes' => array('class' => array('tool-button'))));
	  	if ($subscriber_worker_node) {
		  	$tools[] = l('Subscriber', "node/" . $subscriber_worker_node->nid . '/sirius_trust_worker_benefits', array('attributes' => array('class' => array('tool-button'))));
		  	$tools[] = l('Dependent', "node/" . $beneficiary_worker_node->nid . '/sirius_trust_worker_benefits', array('attributes' => array('class' => array('tool-button'))));
		  } else {
		  	$tools[] = l('Subscriber', "node/" . $beneficiary_worker_node->nid . '/sirius_trust_worker_benefits', array('attributes' => array('class' => array('tool-button'))));
		  }
		}

  	$relationship_type_term = $relationship_type_terms[$wb_node->field_sirius_contact_reltype['und'][0]['tid']];
  	$relationship_name = '';
  	if ($direction == 'from') {
  		$relationship_name = $relationship_type_term->name . ' to';
  	} else {
  		$relationship_name = $relationship_type_term->field_sirius_name_alt['und'][0]['value'] . ' of';
  	}

  	$start_ts = sirius_fieldval($wb_node, 'field_sirius_date_start', 'ts');
  	if ($start_ts > 0) { $start_date = gmdate('m/d/Y', $start_ts); } else { $start_date = '(no start date)'; }

  	$end_ts = sirius_fieldval($wb_node, 'field_sirius_date_end', 'ts');
  	if ($end_ts > 0) { $end_date = gmdate('m/d/Y', $end_ts); } else { $end_date = '(no end date)'; }

  	$employer_name = '';
  	$employer_node = node_load($wb_node->field_grievance_shop['und'][0]['target_id']);
  	if ($employer_node) { $employer_name = $employer_node->title; }

  	$wb_json = sirius_json_get($wb_node);

  	$notes = array();
  	if ($employer_name) { $notes[] = '<strong>Employer: </strong>' . $employer_name; }
  	if ($wb_node->field_sirius_notes['und'][0]['value']) { $notes[] = '<strong>Notes: </strong>' . $wb_node->field_sirius_notes['und'][0]['value']; }
  	if ($wb_json['scan']['msg']) { $notes[] = '<strong>Scan: </strong>' . $wb_json['scan']['msg']; }
  	if ($wb_json['scan']['warnings']) {
  		foreach ($wb_json['scan']['warnings'] as $plugin_key => $warning) {
  			$plugin_info = sirius_trust_eligibility_plugin_info($plugin_key);
  			$notes[] = '<strong>' . $plugin_info['title'] . ' Warning:</strong> ' . $warning;
  		}
  	}
  	$notes = join('<br>', $notes);

  	if (!$options['hide_active']) {
  		if ($wb_node->field_sirius_active['und'][0]['value'] == 'Yes') { $active = 'Active'; } else { $active = 'Inactive'; }
  	}

  	if ($relationship_node) {
  		$name_render = $subscriber_name . '<br>' . $relationship_type_name . '<br>' . '<strong>' . $beneficiary_name . '</strong>';
  	} else {
  		$name_render = '<strong>' . $beneficiary_name . '</strong>' . '<br />' . $relationship_type_name;
  	}

  	$row = array();
  	$row[] = $benefit_node->title;

	  if (!$options['hide_name']) {
	  	$row[] = $name_render;
	  }

  	$parts = array_filter(array($active, $start_date, $end_date));
  	$row[] = join('<br />', $parts);
  	if (!$options['hide_notes']) { $row[] = $notes; }
  	$row[] = join(' ', $tools);

  	if ($wb_node->field_sirius_active['und'][0]['value'] == 'Yes') {
  		if ($wb_json['scan']['warnings']) {
  	 		$class = 'warning';
	  	} else {
	  		$class = 'active';
	  	}
  	} else if ($start_ts > time()) {
  		$class = 'blue';
  	} else {
	  	$class = 'inactive';
  	}

  	$rows[] = array('data' => $row, 'class' => array($class));
  }

  if ($options['hide_header']) {
  	$header = array();
  } else {
	  $header = array();
	  $header[] = 'Benefit';

	  if (!$options['hide_name']) {
	  	$header[] = 'Subscriber<br>Relationship<br>Dependent';
	  }

		$parts = array();
		if (!$options['hide_active']) { $parts[] = 'Active'; }
		$parts[] = 'Start Date';
		$parts[] = 'End Date';
	  $header[] = join('<br />', $parts);

	  if (!$options['hide_notes']) { $header[] = 'Notes'; }
	  $header[] = 'Tools';
	}

	return sirius_table_render($header, $rows, array('class' => array('sirius_table_align')));
}

function sirius_trust_wb_scan_dashboard_data($prefs, $options = array()) {
	$status = sirius_domain_variable_get('sirius_trust_wb_scan_status', array());
	if (!$status) { return array(); }

	if ($status['total']) {
		$pct = 100 * round($status['complete'] / $status['total'], 2);
 	}


	$msg .= "Month: <strong>$status[ym] ($pct%)</strong><br />";
	$msg .= "Records: " . number_format($status['total'], 0) . "<br />";
	$msg .= "Complete: " . number_format($status['complete']) . "<br />";
	$msg .= "Pending: " . number_format($status['pending']) . "<br />";
	$msg .= "[mode = $status[mode]]";

	$tools = l(
		'Details', 
		'sirius/trust/benefits/scan', 
		array(
			'attributes' => array('class' => array('tool-button')),
		)
	);

	$row = array(
		'count' => $total,
		'tools' => $tools,
		'details' => $msg,
	);
	$rows[] = $row;

	return $rows;
}

