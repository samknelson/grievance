<?php

function grievance_report_open_summary_with_log() {
	return drupal_get_form('grievance_report_open_summary_with_log_form');
}

function grievance_report_open_summary_with_log_form($form, &$form_state) {
  $report_type = 'grievance-report-open-summary-with-log';
  report_cache_form_prepare($report_type, $form, $form_state);

  $status_vid = taxonomy_vocabulary_machine_name_load('grievance_status')->vid;
  $status_terms = taxonomy_get_tree($status_vid);
  $log_type_vid = taxonomy_vocabulary_machine_name_load('grievance_log_types')->vid;
  $log_type_terms = taxonomy_get_tree($log_type_vid);

  $options = array(NULL => '-- Select --');
  foreach ($status_terms as $ignore => $term) {
    $options["status|" . $term->tid] = "Status: " . $term->name;
  }

  foreach ($log_type_terms as $ignore => $term) {
    $options["type|" . $term->tid] = "Log type: " . $term->name;
  }

  $form['intro'] = array(
    '#type' => 'markup',
    '#markup' => '<div class="grievance-form-intro">' .
      'Use the form below to generate a report of <strong>open</strong> grievances which have one or two specific log types. ' .
      'For example, you might select the log type "first call" to count grievances that have or haven\'t had a first call. ' .
      'You might select "BOA Held" to count grievances that have or haven\'t had a board of adjustment.' .
      '</div>',
  );

  $form['col_1'] = array(
    '#type' => 'select',
    '#title' => 'First Column',
    '#options' => $options,
  );

  $form['col_2'] = array(
    '#type' => 'select',
    '#title' => 'Second Column',
    '#options' => $options,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Generate Report',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Generate',
  );
  
  return $form;
}

function grievance_report_open_summary_with_log_form_submit($form, &$form_state) { return report_cache_form_submit($form, $form_state); }

function grievance_report_open_summary_with_log_fields() {
  return array(
    t('NID'),
    t('Name'), 
    t('Shop'), 
    t('Col 1'),
    t('Col 2'),
  );
}

function grievance_report_open_summary_with_log_pks() {
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

  $sql = "select nid ";
  $sql .= "from node ";
  $sql .= "left join field_data_field_grievance_status on (field_data_field_grievance_status.entity_type = 'node' and node.nid = field_data_field_grievance_status.entity_id) ";
  $sql .= "where node.type = 'grievance' ";
  $sql .= "and field_data_field_grievance_status.field_grievance_status_tid in (:open_status_tids) ";
  $sql .= "order by node.nid ";

  $stmt = db_query($sql, array(':open_status_tids' => $open_status_tids));
  $nids = array();
  while ($hr = $stmt->fetchAssoc()) {
    $nids[] = $hr['nid'];
  }

  return array_chunk($nids, 100, TRUE);
}

function grievance_report_open_summary_with_log_batch($grievance_nids, $values, $report_id, $report_action, &$context) {
  $lookups = grievance_report_open_summary_with_log_lookups();

  $rows = array(
    'is_multiple' => TRUE,
    'rows' => array(),
  );
  foreach ($grievance_nids as $grievance_nid) {
    $row = grievance_report_open_summary_with_log_batch_nid($grievance_nid, $values, $context, $lookups);
    if ($row) {
      $rows['rows'][$grievance_nid] = $row;
    }
  }
  return $rows;
}

function grievance_report_open_summary_with_log_batch_nid($grievance_nid, $values, $context, $lookups) {
  $row = array();
  $row[] = $grievance_nid;

  $sql = "select name from users, field_data_field_grievance_rep_organizer ";
  $sql .= "where users.uid = field_data_field_grievance_rep_organizer.field_grievance_rep_organizer_target_id ";
  $sql .= "and field_data_field_grievance_rep_organizer.entity_type = 'node' ";
  $sql .= "and field_data_field_grievance_rep_organizer.entity_id = :nid ";
  $stmt = db_query($sql, array(':nid' => $grievance_nid));
  $hr = $stmt->fetchAssoc();
  $row[] = $hr['name'];

  $sql = "select title from node, field_data_field_grievance_shop ";
  $sql .= "where node.nid = field_data_field_grievance_shop.field_grievance_shop_target_id ";
  $sql .= "and field_data_field_grievance_shop.entity_type = 'node' ";
  $sql .= "and field_data_field_grievance_shop.entity_id = :nid ";
  $stmt = db_query($sql, array(':nid' => $grievance_nid));
  $hr = $stmt->fetchAssoc();
  $row[] = $hr['title'];

  for ($i=1; $i<=2; ++$i) {
    if ($values['col_' . $i]) {
      list ($which, $tid) = explode('|', $values['col_' . $i]);
      if ($which == 'type') {
        $sql = "select count(*) as c from field_data_field_grievance, field_data_field_grievance_log_type ";
        $sql .= "where field_data_field_grievance.entity_type = 'node' ";
        $sql .= "and field_data_field_grievance_log_type.entity_type = 'node' ";
        $sql .= "and field_data_field_grievance_log_type.entity_id = field_data_field_grievance.entity_id ";
        $sql .= "and field_data_field_grievance.field_grievance_target_id = :nid ";
        $sql .= "and field_data_field_grievance_log_type.field_grievance_log_type_tid = :tid ";
      } else {
        $sql = "select count(*) as c from field_data_field_grievance, field_data_field_grievance_status ";
        $sql .= "where field_data_field_grievance.entity_type = 'node' ";
        $sql .= "and field_data_field_grievance_status.entity_type = 'node' ";
        $sql .= "and field_data_field_grievance_status.entity_id = field_data_field_grievance.entity_id ";
        $sql .= "and field_data_field_grievance.field_grievance_target_id = :nid ";
        $sql .= "and field_data_field_grievance_status.field_grievance_status_tid = :tid ";
      }
      $stmt = db_query($sql, array(':nid' => $grievance_nid, ':tid' => $tid));
      $hr = $stmt->fetchAssoc();
      $row[] = $hr['c'];
    } else {
      $row[] = 'N/A';
    }
  }

  return $row;
}

function grievance_report_open_summary_with_log_lookups() {
  // Get the list of open and closed statuses
  $status_vocabulary = taxonomy_vocabulary_machine_name_load('grievance_status');
  $status_terms = taxonomy_get_tree($status_vocabulary->vid);
  return array('status_terms' => $status_terms);
}

function grievance_report_open_summary_with_log_render_specialist($report_type, $report_id) {
  return grievance_report_open_summary_with_log_render($report_type, $report_id, 'specialist');
}

function grievance_report_open_summary_with_log_render_employer($report_type, $report_id) {
  return grievance_report_open_summary_with_log_render($report_type, $report_id, 'employer');
}

function grievance_report_open_summary_with_log_render($report_type, $report_id, $which) {
  // Load the parameters
  $query = db_select('report_cache');
  $query->fields('report_cache');
  $query->condition('report_id', $report_id);
  $query->condition('report_type', $report_type . ' :: PARAMETERS');
  $stmt = $query->execute();
  $hr = $stmt->fetchAssoc();
  $parameters = unserialize($hr['report_data']);

  $html = '';
  $html .= '<div class="grievance-report-summary-with-log-render">';
  $html .= '<h2>';
  $html .= t('Open Grievances Summary Report With Log ');
  if ($which == 'specialist') { 
    $html .= ' -- By Specialist'; 
  } else {
    $html .= ' -- By Employer'; 
  }
  $html .= '</h2>';

  $query = db_select('report_cache');
  $query->fields('report_cache');
  $query->condition('report_id', $report_id);
  $query->condition('report_type', $report_type);
  $stmt = $query->execute();
  $data = array();
  while ($hr = $stmt->fetchAssoc()) {
    $hr = unserialize($hr['report_data']);
    $total += $hr['amt'];

    if ($which == 'specialist') {
      $key = $hr[1];
    } else {
      $key = $hr[2];
    }

    if (!$key) { $key = '[unknown]'; }

    $data[$key]['total']++;
    $totals['total']++;

    if ($hr[3]) {
      $data[$key]['col_1_yes']++;
      $totals['col_1_yes']++;
    } else {
      $data[$key]['col_1_no']++;
      $totals['col_1_no']++;
    }

    if ($hr[4]) {
      $data[$key]['col_2_yes']++;
      $totals['col_2_yes']++;
    } else {
      $data[$key]['col_2_no']++;
      $totals['col_2_no']++;
    }
  }

  ksort($data);

  if ($parameters['col_1']) {
    list ($ignore, $tid) = explode('|', $parameters['col_1']);
    $col_1_term = taxonomy_term_load($tid);
  }

  if ($parameters['col_2']) {
    list ($ignore, $tid) = explode('|', $parameters['col_2']);
    $col_2_term = taxonomy_term_load($tid);
  }

  $header = array();
  if ($which == 'specialist') {
    $header[] = 'Specialist';
  } else {
    $header[] = 'Employer';
  }
  $header[] = 'Total';
  if ($parameters['col_1']) { $header[] = 'With: ' . $col_1_term->name; }
  if ($parameters['col_2']) { $header[] = 'With: ' . $col_2_term->name; }
  if ($parameters['col_1']) { $header[] = 'Without: ' . $col_1_term->name; }
  if ($parameters['col_2']) { $header[] = 'Without: ' . $col_2_term->name; }

  $rows = array();
  foreach ($data as $key => $sub_data) {
    $row = array();
    $row[] = $key;
    $row[] = number_format($sub_data['total']);

    if ($parameters['col_1']) { $row[] = number_format($sub_data['col_1_yes']); }
    if ($parameters['col_2']) { $row[] = number_format($sub_data['col_2_yes']); }
    if ($parameters['col_1']) { $row[] = number_format($sub_data['col_1_no']); }
    if ($parameters['col_2']) { $row[] = number_format($sub_data['col_2_no']); }
    $rows[] = $row;
  }

  $row = array();
  $row[] = '<strong>Totals</strong>';
  $row[] = '<strong>' . number_format($totals['total']) . '</strong>';
  if ($parameters['col_1']) { $row[] = '<strong>' . number_format($totals['col_1_yes']) . '</strong>'; }
  if ($parameters['col_2']) { $row[] = '<strong>' . number_format($totals['col_2_yes']) . '</strong>'; }
  if ($parameters['col_1']) { $row[] = '<strong>' . number_format($totals['col_1_no']) . '</strong>'; }
  if ($parameters['col_2']) { $row[] = '<strong>' . number_format($totals['col_2_no']) . '</strong>'; }
  $rows[] = $row;


  $attributes = array();

  $html .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => $attributes));
  $html .= '</div>';

  return $html;
}




























































function grievance_report_open_summary_with_log_formxyz_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;

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
  $params[':open_status_tids'] = $open_status_tids;


  // Get the column 1 and 2
  $col_1 = $form_state['values']['col_1'];
  $col_2 = $form_state['values']['col_2'];
  list($col_1_which, $col_1_tid) = explode('|', $col_1);
  list($col_2_which, $col_2_tid) = explode('|', $col_2);

  // Do this as one SQL query, because it's so much quicker
  /*
  $sql = "select nid, ";
  $sql .= "field_grievance_rep_organizer_target_id, ";
  $sql .= "field_grievance_shop_target_id ";
  if ($col_1) {
    if ($col_1_which == 'type') {
      $sql .= ', count(log1_type.field_grievance_log_type_tid) as col_1 ';
    } else {
      $sql .= ', count(log1_status.field_grievance_status_tid) as col_1 ';
    }
  } 
  $sql .= "from node ";
  $sql .= "left join field_data_field_grievance_status on (field_data_field_grievance_status.entity_type = 'node' and node.nid = field_data_field_grievance_status.entity_id) ";
  $sql .= "left join field_data_field_grievance_rep_organizer on (field_data_field_grievance_rep_organizer.entity_type = 'node' and node.nid = field_data_field_grievance_rep_organizer.entity_id) ";
  $sql .= "left join field_data_field_grievance_shop on (field_data_field_grievance_shop.entity_type = 'node' and node.nid = field_data_field_grievance_shop.entity_id) ";

  if ($col_1) {
    $sql .= "left join field_data_field_grievance field_data_field_grievance_col_1 on (field_data_field_grievance_col_1.entity_type = 'node' and field_data_field_grievance_col_1.bundle = 'grievance_log' and field_data_field_grievance_col_1.field_grievance_target_id = node.nid) ";
    $params[':col_1_tid'] = $col_1_tid;
    $sql .= "left join field_data_field_grievance log1 on (log1.entity_type = 'node' and log1.bundle = 'grievance_log' and log1.field_grievance_target_id = node.nid) ";
    if ($col_1_which == 'type') {
      $sql .= "left join field_data_field_grievance_log_type log1_type on (log1_type.entity_type = 'node' and log1_type.entity_id = log1.entity_id and log1_type.field_grievance_log_type_tid = :col_1_tid) ";
    } else {
      $sql .= "left join field_data_field_grievance_status log1_status on (log1_status.entity_type = 'node' and log1_status.entity_id = log1.entity_id and log1_status.field_grievance_status_tid = :col_1_tid) ";
    }
  }

  if ($col_2) {
    $sql .= "left join field_data_field_grievance field_data_field_grievance_col_2 on (field_data_field_grievance_col_2.entity_type = 'node' and field_data_field_grievance_col_2.bundle = 'grievance_log' and field_data_field_grievance_col_2.field_grievance_target_id = node.nid) ";
    $params[':col_2_tid'] = $col_2_tid;
    $sql .= "left join field_data_field_grievance log2 on (log2.entity_type = 'node' and log2.bundle = 'grievance_log' and log2.field_grievance_target_id = node.nid) ";
    if ($col_2_which == 'type') {
      $sql .= "left join field_data_field_grievance_log_type log2_type on (log2_type.entity_type = 'node' and log2_type.entity_id = log2.entity_id and log2_type.field_grievance_log_type_tid = :col_2_tid) ";
    } else {
      $sql .= "left join field_data_field_grievance_status log2_status on (log2_status.entity_type = 'node' and log2_status.entity_id = log2.entity_id and log2_status.field_grievance_status_tid = :col_2_tid) ";
    }
  }

  $sql .= "where node.type = 'grievance' ";
  $sql .= "and field_data_field_grievance_status.field_grievance_status_tid in (:open_status_tids) ";
  $sql .= "group by node.nid ";
  */

  $header = array(
  	'Name',
  	'Open',
    'Col 1 Yes',
    'Col 2 Yes',
    'Col 1 No',
  	'Col 2 No',
  );
  $attributes = array();

  $stmt = db_query($sql, $params);
  $counts = array();

  $count_total = 0;
  $count_by_specialist = array();
  $count_by_shop = array();

  while ($hr = $stmt->fetchAssoc()) {
  	++$count;

    ++$count_by_specialist[$hr['field_grievance_rep_organizer_target_id']]['total'];
    ++$count_by_shop[$hr['field_grievance_shop_target_id']]['total'];

    if ($hr['col_1']) {
      ++$count_by_specialist[$hr['field_grievance_rep_organizer_target_id']]['col_1_yes'];
      ++$count_by_shop[$hr['field_grievance_shop_target_id']]['col_1_yes'];
    } else {
      ++$count_by_specialist[$hr['field_grievance_rep_organizer_target_id']]['col_1_no'];
      ++$count_by_shop[$hr['field_grievance_shop_target_id']]['col_1_no'];
    }

    if ($hr['col_2']) {
      ++$count_by_specialist[$hr['field_grievance_rep_organizer_target_id']]['col_2_yes'];
      ++$count_by_shop[$hr['field_grievance_shop_target_id']]['col_2_yes'];
    } else {
      ++$count_by_specialist[$hr['field_grievance_rep_organizer_target_id']]['col_2_no'];
      ++$count_by_shop[$hr['field_grievance_shop_target_id']]['col_2_no'];
    }
  }

  $which = $form_state['values']['which'];

  if ($which == 'specialist') {
  	$lookups = user_load_multiple(array_keys($count_by_specialist));
  	$count_by_which = $count_by_specialist;
  } else {
  	$lookups = node_load_multiple(array_keys($count_by_shop));
  	$count_by_which = $count_by_shop;
  }

  foreach ($count_by_which as $id => &$counts) {
  	if (!$id) {
  		$counts['name'] = '(Not assigned)';
  		$counts['id'] = 0;
  	} else {
  		if ($which == 'specialist') {
  			$counts['name'] = $lookups[$id]->name;
  		} else {
  			$counts['name'] = $lookups[$id]->title;
  		}
  		$counts['id'] = $id;
  	}
  }

  usort($count_by_which, function($a, $b) { 
  	if (!$a['id']) { return 1; }
  	if (!$b['id']) { return -1; }
  	return strcmp($a['name'], $b['name']);
	});

  $rows = array();
  foreach ($count_by_which as $id => &$counts) {
  	$row = array();
  	$row[] = $counts['name'];
    $row[] = $counts['total'];
    $row[] = $counts['col_1_yes'];
    $row[] = $counts['col_1_no'];
  	$rows[] = $row;
  }

  /*
  $row = array();
  $row[] = '<strong>Total</strong>';
	foreach ($breakpoints as $breakpoint) {
		$row[] = '<strong>' . number_format($count_total[$breakpoint], 0) . '</strong>';
	}
	$rows[] = $row;
  */


	$form_state['results'] = theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => $attributes));
}

