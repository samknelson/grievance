<?php

function sirius_trust_eligibility_plugin_election_check($settings, $params) {
	$benefit_node = $params['benefit_node'];
	$subscriber_worker_node = $params['subscriber_worker_node'];
	$ts = $params['ts'];

	// drupal_set_message("Trying to determine if Dependent: " . $dependent_worker_node->title . " subsdriber " . $subscriber_worker_node->title . " benefit " . $benefit_node->title . " on $ts");;

	$date = gmdate('Y-m-d', $ts);

	// Find out if the subscriber has an election on file for this benefit
	$sql_args = array();
	$sql = "select distinct node.nid ";
	$sql .= "from node ";
	$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type ='node' and field_data_field_sirius_domain.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_date_start on field_data_field_sirius_date_start.entity_type ='node' and field_data_field_sirius_date_start.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_date_end on field_data_field_sirius_date_end.entity_type ='node' and field_data_field_sirius_date_end.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_worker on field_data_field_sirius_worker.entity_type ='node' and field_data_field_sirius_worker.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_trust_benefits on field_data_field_sirius_trust_benefits.entity_type ='node' and field_data_field_sirius_trust_benefits.entity_id = node.nid ";
	$sql .= "where node.type = 'sirius_trust_worker_election' ";

	$sql .= "and field_sirius_worker_target_id = :worker_nid ";
	$sql_args[':worker_nid'] = $subscriber_worker_node->nid;

	$sql .= "and field_sirius_domain_target_id = :domain_nid ";
	$sql_args[':domain_nid'] = sirius_domain_nid();

	$sql .= "and field_sirius_date_start_value <= :date ";
	$sql_args[':date'] = $date;

	$sql .= "and (field_sirius_date_end_value is null or field_sirius_date_end_value >= :date2) ";
	$sql_args[':date2'] = $date;

	$sql .= "and field_sirius_trust_benefits_target_id = :benefit_nid ";
	$sql_args[':benefit_nid'] = $benefit_node->nid;

	$sql .= "order by created desc limit 1 ";
	$stmt = sirius_sql_query($sql, $sql_args);
	$hr = $stmt->fetchAssoc();
	if (!$hr['nid']) {
		$result = array();
		$result['success'] = FALSE;
		$result['msg'] = 'No elections were found by ' . $subscriber_worker_node->title . ' for ' . $benefit_node->title . ' that were active on ' . $date . '.';

		$paths = array(
			"node/" . $subscriber_worker_node->nid . '/sirius_trust_worker_elections' => 'Elections',
		);
		$tools = array();
		foreach ($paths as $path => $label) {
			if (drupal_valid_path($path)) {
				$tools[] = l($label, $path, array('attributes' => array('class' => array('tool-button'))));
			}
		}
		$result['tools'] = $tools;
		return $result;
	}

	$election_node = node_load($hr['nid']);

	$result = array();
	$result['success'] = TRUE;
	$result['msg'] = 'Election on ' . date('Y-m-d', sirius_fieldval($election_node, 'field_sirius_date_start', 'ts'));

	$paths = array(
		"node/" . $election_node->nid => 'Election',
	);
	$tools = array();
	foreach ($paths as $path => $label) {
		if (drupal_valid_path($path)) {
			$tools[] = l($label, $path, array('attributes' => array('class' => array('tool-button'))));
		}
	}
	$result['tools'] = $tools;
	return $result;
}