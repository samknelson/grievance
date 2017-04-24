<?php

function grievance_report_average_time() {
  return drupal_get_form('grievance_report_average_time_form');
}

function grievance_report_average_time_form($form, &$form_state) {
  $report_type = 'grievance-report-average-time';
  report_cache_form_prepare($report_type, $form, $form_state);

  $status_vocabulary = taxonomy_vocabulary_machine_name_load('grievance_status');
  $status_terms = taxonomy_get_tree($status_vocabulary->vid);
  $status_options = array();
  foreach ($status_terms as $status_term) {
    $status_options[$status_term->tid] = $status_term->name;
  }

  $form["from_status"] = array(
    '#type' => 'checkboxes',
    '#options' => $status_options,
    '#title' => t('Compute average time FROM any of these statuses'),
  );

  $form["to_status"] = array(
    '#type' => 'checkboxes',
    '#options' => $status_options,
    '#title' => t('Compute average time TO any of these statuses'),
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Generate',
  );
  
  return $form;
}

function grievance_report_average_time_form_submit($form, &$form_state) { return report_cache_form_submit($form, $form_state); }

function grievance_report_average_time_fields() {
  return array(
  	t('Company'), 
  	t('Shop'), 
  	t('Number of Grievances'),
  	t('Average Number of Days'),
  );
}

function grievance_report_average_time_pks() {
	$query = new EntityFieldQuery();
	$query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance');
  $query->fieldOrderBy('field_grievance_shop', 'target_id');
  $query->propertyOrderBy('nid');
	$stmt = $query->execute();
	if (!$stmt['node']) { return array(); }

	$all_nids = array_keys($stmt['node']);

	$result = array();
	$count = 0;
	$start = 0;
	$end = 0;
	$last_nid = 0;

	foreach ($all_nids as $nid) {
		++$count;
		if ($count % 5 == 0) {
			$result[] = "$start-$last_nid";
			$start = $nid;
		}

		$end = $nid;
		$last_nid = $nid;
	}

	$result[] = "$last_nid-10000000";
	$result[] = 'PLACEHOLDER_FINAL_NID';
	return $result;
}

function grievance_report_average_time_batch($grievance_nid_range, $values, $report_id, $report_action, &$context) {
	if ($grievance_nid_range == 'PLACEHOLDER_FINAL_NID') {
		grievance_report_average_time_batch_nid($grievance_nid_range, $values, $report_id, $report_action, $context);
	} else {
		list ($from_nid, $to_nid) = explode('-', $grievance_nid_range);
		$query = new EntityFieldQuery();
		$query->entityCondition('entity_type', 'node');
	  $query->entityCondition('bundle', 'grievance');
	  $query->propertyCondition('nid', $from_nid, '>=');
	  $query->propertyCondition('nid', $to_nid, '<=');
	  $query->fieldOrderBy('field_grievance_shop', 'target_id');
	  $query->propertyOrderBy('nid');
		$stmt = $query->execute();
		if ($stmt['node']) {
			$nids = array_keys($stmt['node']);
			foreach ($nids as $nid) {
				grievance_report_average_time_batch_nid($nid, $values, $report_id, $report_action, $context);
			}
		}
	}
}

function grievance_report_average_time_batch_nid($grievance_nid, $values, $report_id, $report_action, &$context) {
	$grievance_node = node_load($grievance_nid);
	$shop_nid = $grievance_node->field_grievance_shop['und'][0]['target_id'];
	$last_shop_nid = $context['results']['last_shop_nid'];
	$result = NULL;

	if ($last_shop_nid && ($shop_nid != $last_shop_nid)) {
		$count = $context['results']['count'];
		$total = $context['results']['total'];
		if ($count) { $value = round($total / $count, 2); } else { $value = 0; }

		$shop_node = node_load($last_shop_nid);
		$company_nid = $shop_node->field_grievance_company['und'][0]['target_id'];
		$company_node = node_load($company_nid);

		$result = array($company_node->title, $shop_node->title, $count, $value);

		$context['results']['count'] = $context['results']['total'] = NULL;
	}

	if ($grievance_node) {
		$from_status_tids = array();
		foreach (array_values($values['from_status']) as $status_tid) {
			if ($status_tid) { $from_status_tids[] = $status_tid; }
		}

		$to_status_tids = array();
		foreach (array_values($values['to_status']) as $status_tid) {
			if ($status_tid) { $to_status_tids[] = $status_tid; }
		}

		$days = grievance_report_average_time_time_between_statuses($grievance_node, $from_status_tids, $to_status_tids);

		if ($days >= 0) {
			$context['results']['count']++;
			$context['results']['total'] += $days;
		}
	}

	$context['results']['last_shop_nid'] = $shop_nid;

	return $result;
}

function grievance_report_average_time_time_between_statuses($grievance_node, $from_status_tids, $to_status_tids) {
  $log_update_tids = array(variable_get('grievance_log_update_type', ''), variable_get('grievance_log_incident_type', ''));
	$log_nodes = grievance_log_list($grievance_node, $log_update_tids);

	$start_date = NULL;
	$end_date = NULL;
	foreach ($log_nodes as $log_node) {
		$status_tid = $log_node->field_grievance_status['und'][0]['tid'];
		if (!$status_tid) { continue; }

		if (in_array($status_tid, $from_status_tids) && !$from_date) {
			$from_date = $log_node->field_grievance_status_date['und'][0]['value'];
		}

		if (in_array($status_tid, $to_status_tids) && !$to_date) {
			$to_date = $log_node->field_grievance_status_date['und'][0]['value'];
		}
	}

	if (!$from_date || !$to_date) { return -1; }

	$date1 = new DateTime($from_date);
	$date2 = new DateTime($to_date);

	return $date2->diff($date1)->format("%a");
}