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

  $form['from_date'] = array(
    '#type' => 'date_popup',
    '#title' => 'Start Date',
    '#date_format' => 'm/d/Y',
    '#description' => t('Only grievances with an intake date after this date will be selected. Leave blank for all dates'),
  );

  $form['to_date'] = array(
    '#type' => 'date_popup',
    '#title' => 'End Date',
    '#date_format' => 'm/d/Y',
    '#description' => t('Leave blank for all dates'),
  );

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
    'employer_name' => 'Employer Name',
    'employer_nid' => 'Employer NID',
    'specialist_name' => 'Specialist Name',
    'specialist_uid' => 'Specialist UID',
    'grievant_name' => 'Grievant',
    'grievance_id' => 'Griev#',
    'intake_date' => 'Intake Date',
    'closed_date' => 'Closed Date',
    'days' => 'Number of Days',
  );
}

function grievance_report_average_time_pks(&$form_state) {
	$query = new EntityFieldQuery();
	$query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance');
  $query->fieldOrderBy('field_grievance_shop', 'target_id');
  if ($form_state['values']['from_date']) {
  	$from_ts = strtotime($form_state['values']['from_date']);
  	$query->propertyCondition('created', $from_ts, '>=');
  }
  if ($form_state['values']['to_date']) {
  	$from_ts = strtotime($form_state['values']['to_date']);
  	$query->propertyCondition('created', $from_ts, '<=');
  }
  $query->propertyOrderBy('nid');
	$stmt = $query->execute();
	if (!$stmt['node']) { return array(); }

	$nids = array_keys($stmt['node']);

	return array_chunk($nids, 100, TRUE);
}

function grievance_report_average_time_batch($grievance_nids, $values, $report_id, $report_action, &$context) {
	$rows = array(
		'is_multiple' => TRUE,
		'rows' => array(),
	);

	$from_status_tids = array();
	foreach (array_values($values['from_status']) as $status_tid) {
		if ($status_tid) { $from_status_tids[] = $status_tid; }
	}

	$to_status_tids = array();
	foreach (array_values($values['to_status']) as $status_tid) {
		if ($status_tid) { $to_status_tids[] = $status_tid; }
	}


  // Get the list of open and closed statuses
  $status_vocabulary = taxonomy_vocabulary_machine_name_load('grievance_status');
  $status_terms = array();
  $tmp = taxonomy_get_tree($status_vocabulary->vid);
  foreach ($tmp as $status_term) {
    $status_terms[$status_term->tid] = taxonomy_term_load($status_term->tid);
  }
  $open_status_tids = array(-999);
  $closed_status_tids = array(-999);

  foreach ($status_terms as $tid => $term) {
    if ($term->field_grievance_open['und'][0]['value'] == 1) {
      $open_status_tids[] = $tid;
    }
    if ($term->field_grievance_open['und'][0]['value'] == 0) {
      $closed_status_tids[] = $tid;
    }
  }

	foreach ($grievance_nids as $grievance_nid) {
		$row = grievance_report_average_time_nid($grievance_nid, $open_status_tids, $closed_status_tids, $from_status_tids, $to_status_tids, $values);
		if ($row) {
			$rows['rows'][$grievance_nid] = $row;
		}
	}

	return $rows;
}

function grievance_report_average_time_nid($grievance_nid, $open_status_tids, $closed_status_tids, $from_status_tids, $to_status_tids, $values) {
	$grievance_node = node_load($grievance_nid);

  $log_update_tids = array(
    variable_get('grievance_log_insert_type', ''), 
    variable_get('grievance_log_update_type_with_status_change', ''), 
  );
 
 	// Start date
 	$query = new EntityFieldQuery();
	$query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_log');
  $query->fieldCondition('field_grievance', 'target_id', $grievance_node->nid);
  $query->fieldCondition('field_grievance_log_type', 'tid', $log_update_tids, 'in');
 	$query->fieldCondition('field_grievance_status', 'tid', $from_status_tids, 'in');
  $query->fieldOrderBy('field_grievance_status_date', 'value');
  $query->range(0,1);
	$stmt = $query->execute();
	if (!$stmt['node']) { return NULL; }
	$log_node = node_load(array_keys($stmt['node'])[0]);
	$from_ts = strtotime($log_node->field_grievance_status_date['und'][0]['value']);

 	// End date
 	$query = new EntityFieldQuery();
	$query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_log');
  $query->fieldCondition('field_grievance', 'target_id', $grievance_node->nid);
  $query->fieldCondition('field_grievance_log_type', 'tid', $log_update_tids, 'in');
 	$query->fieldCondition('field_grievance_status', 'tid', $to_status_tids, 'in');
  $query->fieldOrderBy('field_grievance_status_date', 'value');
  $query->range(0,1);
	$stmt = $query->execute();
	if (!$stmt['node']) { return NULL; }
	$log_node = node_load(array_keys($stmt['node'])[0]);
	$to_ts = strtotime($log_node->field_grievance_status_date['und'][0]['value']);
	if ($to_ts < $from_ts) { return NULL; }

	$row['days'] = ($to_ts - $from_ts) / (60*60*24);

	// Closed date
 	$query = new EntityFieldQuery();
	$query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_log');
  $query->fieldCondition('field_grievance', 'target_id', $grievance_node->nid);
  $query->fieldCondition('field_grievance_log_type', 'tid', $log_update_tids, 'in');
 	$query->fieldCondition('field_grievance_status', 'tid', $closed_status_tids, 'in');
  $query->fieldOrderBy('field_grievance_status_date', 'value');
  $query->range(0,1);
	$stmt = $query->execute();
	if ($stmt['node']) { 
		$log_node = node_load(array_keys($stmt['node'])[0]);
		$closed_ts = strtotime($log_node->field_grievance_status_date['und'][0]['value']);
		$row['closed_date'] = date('m/d/Y', $closed_ts);
	}

  // Employer name
  // @todo node_load is expensive
  if ($grievance_node->field_grievance_shop['und'][0]['target_id']) {
    $employer_node = node_load($grievance_node->field_grievance_shop['und'][0]['target_id']);
    $row['employer_nid'] = $employer_node->nid;
    $row['employer_name'] = $employer_node->title;
  }

  // Specialist name
  // @todo user_load is expensive.
  if ($grievance_node->field_grievance_rep_organizer['und'][0]['target_id']) {
    $user = user_load($grievance_node->field_grievance_rep_organizer['und'][0]['target_id']);
    $row['specialist_name'] = $user->name;
    $row['specialist_uid'] = $user->uid;
  }

  // Grievant name, grievance ID
  $row['grievant_name'] = $grievance_node->field_grievance_first_name['und'][0]['value'] . ' ' . $grievance_node->field_grievance_last_name['und'][0]['value'];
  $row['grievance_id'] = $grievance_node->field_grievance_id['und'][0]['value'];
  $row['intake_date'] = date('m/d/Y', $grievance_node->created);

	return $row;
}

function grievance_report_average_time_time_between_statuses($grievance_node, $from_status_tids, $to_status_tids) {
  $log_update_tids = array(
    variable_get('grievance_log_insert_type', ''), 
    variable_get('grievance_log_update_type_with_status_change', ''), 
  );
 
 	// Start date
 	$query = new EntityFieldQuery();
	$query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_log');
  $query->fieldCondition('field_grievance', 'target_id', $grievance_node->nid);
  $query->fieldCondition('field_grievance_log_type', 'tid', $log_update_tids, 'in');
 	$query->fieldCondition('field_grievance_status', 'tid', $from_status_tids, 'in');
  $query->fieldOrderBy('field_grievance_status_date', 'value');
  $query->range(0,1);
	$stmt = $query->execute();
	if (!$stmt['node']) { return NULL; }
	$log_node = node_load(array_keys($stmt['node'])[0]);
	$from_ts = strtotime($log_node->field_grievance_status_date['und'][0]['value']);

 	// End date
 	$query = new EntityFieldQuery();
	$query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_log');
  $query->fieldCondition('field_grievance', 'target_id', $grievance_node->nid);
  $query->fieldCondition('field_grievance_log_type', 'tid', $log_update_tids, 'in');
 	$query->fieldCondition('field_grievance_status', 'tid', $to_status_tids, 'in');
  $query->fieldOrderBy('field_grievance_status_date', 'value');
  $query->range(0,1);
	$stmt = $query->execute();
	if (!$stmt['node']) { return NULL; }
	$log_node = node_load(array_keys($stmt['node'])[0]);
	$to_ts = strtotime($log_node->field_grievance_status_date['und'][0]['value']);

	if ($to_ts < $from_ts) { return NULL; }

	return ($to_ts - $from_ts) / (60*60*24);
}



function grievance_report_average_time_batch_render_specialist_summary($report_type, $report_id) {
  return grievance_report_average_time_batch_render_detail($report_type, $report_id, 'specialist', FALSE);
}

function grievance_report_average_time_batch_render_employer_summary($report_type, $report_id) {
  return grievance_report_average_time_batch_render_detail($report_type, $report_id, 'employer', FALSE);
}

function grievance_report_average_time_batch_render_specialist_detail($report_type, $report_id) {
  return grievance_report_average_time_batch_render_detail($report_type, $report_id, 'specialist', TRUE);
}

function grievance_report_average_time_batch_render_employer_detail($report_type, $report_id) {
  return grievance_report_average_time_batch_render_detail($report_type, $report_id, 'employer', TRUE);
}

function grievance_report_average_time_batch_render_detail($report_type, $report_id, $which, $detail) {
  // Load the parameters
  $query = db_select('report_cache');
  $query->fields('report_cache');
  $query->condition('report_id', $report_id);
  $query->condition('report_type', $report_type . ' :: PARAMETERS');
  $stmt = $query->execute();
  $hr = $stmt->fetchAssoc();
  $parameters = unserialize($hr['report_data']);

  // Construct the page title
  $html = '';

  $html .= '<div class="grievance-report-average-time">';

  $html .= '<h2>';
  if ($parameters['from_date'] && $parameters['to_date']) {
    $html .= t('Grievances from @from_date to @to_date', array(
        '@from_date' => date('m/d/Y', strtotime($parameters['from_date'])),
        '@to_date' => date('m/d/Y', strtotime($parameters['to_date'])),
      )
    );
  } else if ($parameters['from_date']) {
    $html .= t('Grievances from @from_date', array(
        '@from_date' => date('m/d/Y', strtotime($parameters['from_date'])),
      )
    );
  } else if ($parameters['to_date']) {
    $html .= t('Grievances through @to_date', array(
        '@to_date' => date('m/d/Y', strtotime($parameters['to_date'])),
      )
    );
  } else {
    $html .= 'All Dates';
  }
  if ($detail) {
  	$html .= ' -- Detail by ';
  } else {
  	$html .= ' -- Summary by ';
  }
  if ($which == 'employer') { $html .= 'Employer'; } else { $html .= 'Specialist'; }
  $html .= '</h2>';

  // Walk the data
  $data = array();
  $query = db_select('report_cache');
  $query->fields('report_cache');
  $query->condition('report_id', $report_id);
  $query->condition('report_type', $report_type);
  $stmt = $query->execute();
  $sub_totals = array();
  $totals = array();
  while ($hr = $stmt->fetchAssoc()) {
    $hr = unserialize($hr['report_data']);
    if ($which == 'specialist') {
      $key = $hr['specialist_name'];
    } else {
      $key = $hr['employer_name'];
    }
    $data[$key][] = $hr;
  }

  $attributes = array();

  ksort($data);

  $grand_total_count = 0;
  $grand_total_sum = 0;

  if ($detail) {
	  $header = array();
	  $header[] = 'Grievant';
	  $header[] = 'Griev#';
	  $header[] = 'Intake Date';
	  $header[] = 'Closed Date';
	  $header[] = 'Number of Days';
  } else {
	  $header = array();
	  if ($which == 'specialist') { $header[] = 'Specialist'; } else { $header[] = 'Employer'; }
	  $header[] = 'Number of Days';
  }

  foreach ($data as $key => $sub_data) {
  	if ($detail) { 
  		$html .= '<h3>' . $key . '</h3>';
	    $rows = array();
	  } else {
	  	$row = array();
	  	$row[] = $key;
	  }
    $total_count = 0;
    $total_sum = 0;
    foreach ($sub_data as $hr) {
      ++$total_count;
      $total_sum += $hr['days'];
      ++$grand_total_count;
      $grand_total_sum += $hr['days'];

      if ($detail) {
	      $row = array();
	      $row[] = $hr['grievant_name'];
	      $row[] = $hr['grievance_id'];
	      $row[] = $hr['intake_date'];
	      $row[] = $hr['closed_date'];
	      $row[] = $hr['days'];
	      $rows[] = $row;
	    }
    }

    $avg = 0;
    if ($total_count) { $avg = round($total_sum / $total_count, 2); }

    if ($detail) {
	    $total_row = array();
	    $total_row[] = '<strong>' . t('Average') . '</strong>';
	    $total_row[] = '';
	    $total_row[] = '';
	    $total_row[] = '';
	    $total_row[] = $avg;

	    $rows[] = $total_row;

	    $html .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => $attributes));
	  } else {
	  	$row[] = $avg;
	  	$rows[] = $row;
	  }
  }

  if (!$detail) {
    $html .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => $attributes));
  }

  $html .= '<br>';
  $grand_total_avg = 0;
  if ($grand_total_count) { $grand_total_avg = round($grand_total_sum / $grand_total_count, 2); }
  $html .= '<strong>' . t('Overall Average: ') . $grand_total_avg . '</strong>';
  $html .= '</div>';
  return $html;
}
