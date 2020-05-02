<?php

function grievance_report_average_time() {
  return drupal_get_form('grievance_report_average_time_form');
}

function grievance_report_average_time_form($form, &$form_state) {
  $report_type = 'grievance-report-average-time';
  report_cache_form_prepare($report_type, $form, $form_state);

  $status_vocabulary = taxonomy_vocabulary_machine_name_load('grievance_status');
  $status_terms = taxonomy_get_tree($status_vocabulary->vid);
  $status_options = array(NULL => '-- Select --');
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

  for ($phase=0; $phase<6; ++$phase) {
    if ($phase == 0) { $title = "Compute average time FROM any of these statuses ..."; } 
    else if ($phase == 1) { $title = "... TO any of these statuses ..."; }
    else if ($phase == 5) {  $title = "... AND THEN TO any of these statuses."; }
    else { $title = "... AND THEN TO any of these statuses ..."; }

    $form["phase_$phase"] = array(
      '#type' => 'fieldset',
      '#title' => $title,
      '#attributes' => array('class' => array('status-list')),
    );

    for ($i=0; $i<6; ++$i) {
      $form["phase_$phase"]["status_${phase}_$i"] = array(
        '#type' => 'select',
        '#options' => $status_options,
      );
    }
  }

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
    'date_0' => 'Phase 1 Date',
    'date_1' => 'Phase 2 Date',
    'date_2' => 'Phase 3 Date',
    'date_3' => 'Phase 4 Date',
    'date_4' => 'Phase 5 Date',
    'date_5' => 'Phase 6 Date',
    'days_0' => 'Phase 1 Days',
    'days_1' => 'Phase 2 Days',
    'days_2' => 'Phase 3 Days',
    'days_3' => 'Phase 4 Days',
    'days_4' => 'Phase 5 Days',
    'days_5' => 'Phase 6 Days',
    'total_days_0' => 'Phase 1 Total Days',
    'total_days_1' => 'Phase 2 Total Days',
    'total_days_2' => 'Phase 3 Total Days',
    'total_days_3' => 'Phase 4 Total Days',
    'total_days_4' => 'Phase 5 Total Days',
    'total_days_5' => 'Phase 6 Total Days',
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

  // Get the status IDs we're using
  $from_to_status_tids = array();
  for ($phase=0; $phase<6; ++$phase) {
    for ($i=0; $i<10; ++$i) {
      if ($values["status_${phase}_$i"]) {
        $from_to_status_tids[$phase][$i] = $values["status_${phase}_$i"];
      }
    }
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
		$row = grievance_report_average_time_nid($grievance_nid, $open_status_tids, $closed_status_tids, $from_to_status_tids, $values);
		if ($row) {
			$rows['rows'][$grievance_nid] = $row;
		}
	}

	return $rows;
}

function grievance_report_average_time_nid($grievance_nid, $open_status_tids, $closed_status_tids, $from_to_status_tids, $values) {
	$grievance_node = node_load($grievance_nid);

  // Find break points for each status
  $all_ts = array();
  $found_count = 0;
  foreach ($from_to_status_tids as $phase => $statuses) {
    $status_tids = array_values($statuses);
    if (!$status_tids) { continue; }

    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'node');
    $query->entityCondition('bundle', 'grievance_log');
    $query->fieldCondition('field_grievance', 'target_id', $grievance_node->nid);
    // $query->fieldCondition('field_grievance_log_type', 'tid', $log_update_tids, 'in');
    $query->fieldCondition('field_grievance_status', 'tid', $status_tids, 'in');
    $query->fieldOrderBy('field_grievance_status_date', 'value');
    // Eliminate some stupid bad data.
    $query->fieldCondition('field_grievance_status_date', 'value', '1980-1-1', '>=');
    $query->fieldCondition('field_grievance_status_date', 'value', '2050-1-1', '<=');
    $query->range(0,1);
    $stmt = $query->execute();
    if (!$stmt['node']) { continue; }
    $log_node = node_load(array_keys($stmt['node'])[0]);
    $all_ts[$phase] = strtotime($log_node->field_grievance_status_date['und'][0]['value']);
    $row["date_$phase"] = date('m/d/Y', $all_ts[$phase]);
    ++$found_count;
  }
  if ($found_count < 2) { return NULL; }

  // Compute the days
  foreach (array_keys($from_to_status_tids) as $phase) {
    if (!$all_ts[$phase]) { continue; }

    if ($all_ts[$phase-1] && $all_ts[$phase] >= $all_ts[$phase-1]) {
      $row["days_$phase"] = round( ($all_ts[$phase] - $all_ts[$phase-1]) / (60*60*24), 2);
    }
    if ($all_ts[0] && $all_ts[$phase] >= $all_ts[0]) {
      $row["total_days_$phase"] = round( ($all_ts[$phase] - $all_ts[0]) / (60*60*24), 2);
    }
  }

	// Closed date
 	$query = new EntityFieldQuery();
	$query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_log');
  $query->fieldCondition('field_grievance', 'target_id', $grievance_node->nid);
  // $query->fieldCondition('field_grievance_log_type', 'tid', $log_update_tids, 'in');
 	$query->fieldCondition('field_grievance_status', 'tid', $closed_status_tids, 'in');
  $query->fieldOrderBy('field_grievance_status_date', 'value');
  // Eliminate some stupid bad data.
  $query->fieldCondition('field_grievance_status_date', 'value', '1980-1-1', '>=');
  $query->fieldCondition('field_grievance_status_date', 'value', '2050-1-1', '<=');
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

  // Get the status IDs we're using
  $from_to_status_tids = array();
  for ($phase=0; $phase<6; ++$phase) {
    for ($i=0; $i<10; ++$i) {
      if ($parameters["status_${phase}_$i"]) {
        $from_to_status_tids[$phase][$i] = $parameters["status_${phase}_$i"];
      }
    }
  }

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

  // Introductory text
  foreach ($from_to_status_tids as $phase => $statuses) {
    $status_tids = array_values($statuses);
    if (!$status_tids) { continue; }

    $status_terms = taxonomy_term_load_multiple($status_tids);
    $status_name = '';
    foreach ($status_terms as $ignore => $term) {
      if ($status_name) { $status_name .= ', '; }
      $status_name .= $term->name; 
    }

    $html .= "<strong>Phase " . ($phase+1) . ":</strong> " . $status_name . '<br>';
  }


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

  $grand_total_count = $grand_total_sum = $grand_total_running_sum = array();

  if ($detail) {
    $header = array();
    $super_header = array();

	  $header[] = 'Grievant';
	  $header[] = 'Griev#';
	  $header[] = 'Intake Date';
	  $header[] = 'Closed Date';
    foreach ($from_to_status_tids as $phase => $ignore) {
      $header[] = "Phase " . ($phase+1) . " Entered";
      if ($phase > 0) {
        $header[] = "Phase " . ($phase+1) . " Days";
      }
      if ($phase > 1) {
        $header[] = "Phase " . ($phase+1) . " Total Days";
      }
    }
  } else {
	  $header = array();
	  if ($which == 'specialist') { $header[] = 'Specialist'; } else { $header[] = 'Employer'; }
    foreach ($from_to_status_tids as $phase => $ignore) {
      if ($phase > 0) {
        $header[] = "Phase " . ($phase+1) . " Days";
      }
      if ($phase > 1) {
        $header[] = "Phase " . ($phase+1) . " Total Days";
      }
    }
  }
  $col_count = count($header);

  foreach ($data as $key => $sub_data) {
    $total_count = $total_sum = $total_running_sum = array();

  	if ($detail) { 
      $row = array();
      $row[] = array('data' => "<h2>$key</h2>", 'colspan' => $col_count);
      $rows[] = $row;
	  } else {
	  	$row = array();
	  	$row[] = $key;
	  }


    foreach ($sub_data as $hr) {

      foreach ($from_to_status_tids as $phase => $ignore) {
        if ($hr["date_$phase"]) {
          $total_count[$phase]++;
          $total_sum[$phase] += $hr["days_$phase"];
          $total_running_sum[$phase] += $hr["total_days_$phase"];

          $grand_total_count[$phase]++;
          $grand_total_sum[$phase] += $hr["days_$phase"];
          $grand_total_running_sum[$phase] += $hr["total_days_$phase"];
        }
      }

      if ($detail) {
	      $row = array();
	      $row[] = $hr['grievant_name'];
	      $row[] = $hr['grievance_id'];
	      $row[] = $hr['intake_date'];
	      $row[] = $hr['closed_date'];

        foreach ($from_to_status_tids as $phase => $ignore) {
          $row[] = $hr["date_$phase"];
          if ($phase > 0) {
            $row[] = $hr["days_$phase"];
          }
          if ($phase > 1) {
            $row[] = $hr["total_days_$phase"];
          }
        }

	      $rows[] = $row;
	    }
    }

    $avg = 0;
    if ($detail) {
	    $total_row = array();
	    $total_row[] = '<strong>' . t('Average') . '</strong>';
	    $total_row[] = '';
	    $total_row[] = '';
	    $total_row[] = '';
      foreach ($from_to_status_tids as $phase => $ignore) {
        $total_row[] = '';
        if ($phase > 0) {
          $total_row[] = ($total_count[$phase] ? round($total_sum[$phase] / $total_count[$phase], 2) : 'n/a');
        }
        if ($phase > 1) {
          $total_row[] = ($total_count[$phase] ? round($total_running_sum[$phase] / $total_count[$phase], 2) : 'n/a');
        }
      }

	    $rows[] = $total_row;

	  } else {
      foreach ($from_to_status_tids as $phase => $ignore) {
        if ($phase > 0) {
          $row[] = ($total_count[$phase] ? round($total_sum[$phase] / $total_count[$phase], 2) : 'n/a');
        }
        if ($phase > 1) {
          $row[] = ($total_count[$phase] ? round($total_running_sum[$phase] / $total_count[$phase], 2) : 'n/a');
        }
      }
	  	$rows[] = $row;
	  }
  }

  $html .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => $attributes));

  $html .= '<br>';

  foreach ($from_to_status_tids as $phase => $ignore) {
    if ($phase > 0) {
      $html .= '<br>';
      $html .= "<strong>Phase " . ($phase+1) . " Overall Average:</strong> ";
      $html .= ($grand_total_count[$phase] ? round($grand_total_sum[$phase] / $grand_total_count[$phase], 2) : 'n/a');
    }
    if ($phase > 1) {
      $html .= '<br>';
      $html .= "<strong>Phase " . ($phase+1) . " Overall Total Average:</strong> ";
      $html .= ($grand_total_count[$phase] ? round($grand_total_running_sum[$phase] / $grand_total_count[$phase], 2) : 'n/a');
    }
  }

  $html .= '</div>';
  return $html;
}

