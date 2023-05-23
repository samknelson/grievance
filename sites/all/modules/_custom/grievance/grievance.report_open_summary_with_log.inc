<?php

function grievance_report_open_summary_with_log_redirect() {
  drupal_goto('sirius/reports/grievance-report-open-summary-with-log/form');
}

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

  $options = array();
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
      'Make sure to select only log types OR statuses, not both. ' .
      '</div>',
  );

  $form['organizer'] = array(
    '#type' => 'entityreference',
    '#title' => t('Organizer'),
    '#era_entity_type' => 'user',
    '#era_bundles' => array(),
    '#era_query_settings' => array(),
    '#description' => t('Leave blank to select all organizers.'),
    '#prefix' => '<div class="form-start">',
  );

  $form['shop'] = array(
    '#type' => 'textfield',
    '#title' => t('Shop'),
    '#description' => t('Enter all or part of the shop name.  Leave blank to select all shops.'),
  );

  $form['shop_match'] = array(
    '#type' => 'select',
    '#title' => t('Exact match on shop name?'),
    '#options' => array('No' => t('No'), 'Yes' => t('Yes')),
  );

  $form['classaction'] = array(
    '#type' => 'select',
    '#title' => t('General Grievances Only?'),
    '#options' => array(NULL => t('Both Individual and General'), 'No' => t('Individual Grievnaces Only'), 'Yes' => t('General / Class Action Grievances Only')),
  );

  $broughtby = sirius_taxonomy_load('grievance_broughtby', 'tid', 'name');
  if ($broughtby) {
    $form['broughtby'] = array(
      '#type' => 'select',
      '#title' => 'Brought By',
      '#options' => array(NULL => t('Any')) + $broughtby,
    );
  }

  $form['col_1'] = array(
    '#type' => 'select',
    '#title' => 'First Column',
    '#options' => $options,
    '#multiple' => TRUE,
    '#size' => 10,
  );

  $form['col_2'] = array(
    '#type' => 'select',
    '#title' => 'Second Column',
    '#options' => $options,
    '#multiple' => TRUE,
    '#size' => 10,
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
    'nid' => t('NID'),
    'specialist' => t('Specialist'), 
    'shop' => t('Shop'), 
    'grievance_id' => t('Grievance ID'), 
    'first_name' => t('First Name'), 
    'last_name' => t('Last Name'), 
    'classification' => t('Classification'), 
    'cba' => t('CBA Provision(s) Violated'), 
    'col_1_count' => t('Col 1 Count'),
    'col_1_date' => t('Col 1 First Date'),
    'col_2_count' => t('Col 2 Count'),
    'col_2_date' => t('Col 2 First Date'),
    'current_status' => t('Current Status'),
  );
}

function grievance_report_open_summary_with_log_pks($form_state, $report_type, $report_id) {
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

  $shop = $form_state['values']['shop'];
  $shop_match = $form_state['values']['shop_match'];
  $organizer_nid = $form_state['values']['organizer']['entity_id'];
  $classaction = $form_state['values']['classaction'];
  $broughtby = $form_state['values']['broughtby'];

  $args = array();
  $args[':open_status_tids'] = $open_status_tids;

  $sql = "select distinct node.nid ";
  $sql .= "from node ";
  $sql .= "left join field_data_field_grievance_rep_organizer on (field_data_field_grievance_rep_organizer.entity_type = 'node' and field_data_field_grievance_rep_organizer.entity_id = node.nid) ";
  $sql .= "left join field_data_field_grievance_shop on (field_data_field_grievance_shop.entity_type = 'node' and node.nid = field_data_field_grievance_shop.entity_id) ";
  $sql .= "left join field_data_field_grievance_status on (field_data_field_grievance_status.entity_type = 'node' and node.nid = field_data_field_grievance_status.entity_id) ";
  $sql .= "left join field_data_field_grievance_classaction on (field_data_field_grievance_classaction.entity_type = 'node' and node.nid = field_data_field_grievance_classaction.entity_id) ";
  $sql .= "left join field_data_field_grievance_broughtby on (field_data_field_grievance_broughtby.entity_type = 'node' and node.nid = field_data_field_grievance_broughtby.entity_id) ";
  $sql .= "left join node shop_node on shop_node.nid = field_grievance_shop_target_id ";
  $sql .= "where node.type = 'grievance' ";
  $sql .= "and field_data_field_grievance_status.field_grievance_status_tid in (:open_status_tids) ";
  if ($shop) {
    if ($shop_match == 'Yes') {
      $sql .= "and shop_node.title = :shop ";
      $args[':shop'] = $shop;
    } else {
      $sql .= "and shop_node.title like :shop ";
      $args[':shop'] = '%' . $shop . '%';
    }
  }
  if ($organizer_nid) {
    $sql .= "and field_grievance_rep_organizer_target_id = :organizer_nid ";
    $args[':organizer_nid'] = $organizer_nid;
  }
  if ($classaction) {
    $sql .= "and field_grievance_classaction_value = :classaction ";
    $args[':classaction'] = $classaction;
  }
  if ($broughtby) {
    $sql .= "and field_grievance_broughtby_tid = :broughtby ";
    $args[':broughtby'] = $broughtby;
  }
  $sql .= "order by node.nid ";

  $stmt = db_query($sql, $args);
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

  // Basic grievancce information 
  $sql = "select users.name as user_name, ";
  $sql .= "shop_node.title as shop_title, ";
  $sql .= "field_grievance_id_value as id, ";
  $sql .= "field_grievance_first_name_value as first_name, "; 
  $sql .= "field_grievance_last_name_value as last_name, "; 
  $sql .= "class_term.name as classification, ";
  $sql .= "status_term.name as current_status_name ";
  $sql .= "from node ";
  $sql .= "left join field_data_field_grievance_rep_organizer on (field_data_field_grievance_rep_organizer.entity_type = 'node' and field_data_field_grievance_rep_organizer.entity_id = node.nid) ";
  $sql .= "left join field_data_field_grievance_shop on (field_data_field_grievance_shop.entity_type = 'node' and field_data_field_grievance_shop.entity_id = node.nid) ";
  $sql .= "left join field_data_field_grievance_first_name on (field_data_field_grievance_first_name.entity_type = 'node' and field_data_field_grievance_first_name.entity_id = node.nid) ";
  $sql .= "left join field_data_field_grievance_last_name on (field_data_field_grievance_last_name.entity_type = 'node' and field_data_field_grievance_last_name.entity_id = node.nid) ";
  $sql .= "left join field_data_field_grievance_id on (field_data_field_grievance_id.entity_type = 'node' and field_data_field_grievance_id.entity_id = node.nid) ";
  $sql .= "left join field_data_field_grievance_class_tid on (field_data_field_grievance_class_tid.entity_type = 'node' and field_data_field_grievance_class_tid.entity_id = node.nid) ";
  $sql .= "left join field_data_field_grievance_status on (field_data_field_grievance_status.entity_type = 'node' and field_data_field_grievance_status.entity_id = node.nid) ";
  $sql .= "left join users on field_grievance_rep_organizer_target_id = users.uid ";
  $sql .= "left join node shop_node on field_grievance_shop_target_id = shop_node.nid ";
  $sql .= "left join taxonomy_term_data class_term on field_grievance_class_tid_tid = class_term.tid ";
  $sql .= "left join taxonomy_term_data status_term on field_grievance_status_tid = status_term.tid ";
  $sql .= "where node.nid = :nid ";
  $stmt = db_query($sql, array(':nid' => $grievance_nid));
  $hr = $stmt->fetchAssoc();
  $row[] = $hr['user_name'];
  $row[] = $hr['shop_title'];
  $row[] = $hr['id'];
  $row[] = $hr['first_name'];
  $row[] = $hr['last_name'];
  $row[] = $hr['classification'];
  $current_status_name = $hr['current_status_name'];

  // Violations
  $sql = "select clause_node.title as clause_title ";
  $sql .= "from node ";
  $sql .= "left join field_data_field_grievance_clauseref on (field_data_field_grievance_clauseref.entity_type = 'node' and field_data_field_grievance_clauseref.entity_id = node.nid) ";
  $sql .= "left join node clause_node on clause_node.nid = field_grievance_clauseref_target_id ";
  $sql .= "where node.nid = :nid ";
  $sql .= "order by field_data_field_grievance_clauseref.delta ";
  $stmt = db_query($sql, array(':nid' => $grievance_nid));
  while ($hr = $stmt->fetchAssoc()) {
    if ($clause_title) { $clause_title .= '<br />'; }
    $clause_title .= $hr['clause_title'];
  }
  $row[] = $clause_title;

  for ($i=1; $i<=2; ++$i) {
    if ($values['col_' . $i]) {
      $tids = array();
      $which = '';
      foreach ($values['col_' . $i] as $key) {
        list ($which, $tid) = explode('|', $key);
        $tids[] = $tid;        
      }

      $sql = "select count(*) as c, min(field_grievance_status_date_value) as first_date ";
      $sql .= "from node ";
      $sql .= "left join field_data_field_grievance on (field_data_field_grievance.entity_type = 'node' and field_data_field_grievance.entity_id = node.nid) ";
      $sql .= "left join field_data_field_grievance_status on (field_data_field_grievance_status.entity_type = 'node' and field_data_field_grievance_status.entity_id = node.nid) ";
      $sql .= "left join field_data_field_grievance_status_date on (field_data_field_grievance_status_date.entity_type = 'node' and field_data_field_grievance_status_date.entity_id = node.nid) ";
      $sql .= "left join field_data_field_grievance_log_type on (field_data_field_grievance_log_type.entity_type = 'node' and field_data_field_grievance_log_type.entity_id = node.nid) ";
      $sql .= "where node.type = 'grievance_log' ";
      $sql .= "and field_grievance_target_id = :nid ";
      if ($which == 'type') { $sql .= "and field_grievance_log_type_tid in (:tids) "; } 
      else { $sql .= "and field_grievance_status_tid in (:tids) "; }
      $stmt = db_query($sql, array(':nid' => $grievance_nid, ':tids' => $tids));
      $hr = $stmt->fetchAssoc();
      $row[] = $hr['c'];
      $row[] = $hr['first_date'];
    } else {
      $row[] = 'N/A';
      $row[] = 'N/A';
    }
  }

  $row[] = $current_status_name;

  return $row;
}

function grievance_report_open_summary_with_log_lookups() {
  // Get the list of open and closed statuses
  $status_vocabulary = taxonomy_vocabulary_machine_name_load('grievance_status');
  $status_terms = taxonomy_get_tree($status_vocabulary->vid);
  return array('status_terms' => $status_terms);
}

function grievance_report_open_summary_with_log_render_specialist_detail($report_type, $report_id) {
  return grievance_report_open_summary_with_log_render_detail($report_type, $report_id, 'specialist');
}

function grievance_report_open_summary_with_log_render_employer_detail($report_type, $report_id) {
  return grievance_report_open_summary_with_log_render_detail($report_type, $report_id, 'employer');
}

function grievance_report_open_summary_with_log_render_specialist($report_type, $report_id) {
  return grievance_report_open_summary_with_log_render($report_type, $report_id, 'specialist');
}

function grievance_report_open_summary_with_log_render_employer($report_type, $report_id) {
  return grievance_report_open_summary_with_log_render($report_type, $report_id, 'employer');
}

function grievance_report_open_summary_with_log_render($report_type, $report_id, $which) {
  $csv = $_REQUEST['csv'];

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
  $html .= t('Open Grievances Summary Report');
  if ($which == 'specialist') { 
    $html .= ' -- By Specialist'; 
  } else {
    $html .= ' -- By Employer'; 
  }
  $html .= '</h2>';

  if ($parameters['classaction']) {
    $html .= '<strong>' . t('General / Class Action: ') . '</strong>' . $parameters['classaction'] . '<br />';
  }
  if ($parameters['shop']) {
    $html .= '<strong>' . t('Shop: ') . '</strong>' . $parameters['shop'] . '<br />';
  }

  for ($i=1; $i<=2; ++$i) {
    if ($parameters["col_$i"]) {
      $col_str = '';
      foreach ($parameters["col_$i"] as $key) {
        list ($ignore, $tid) = explode('|', $key);
        $term = taxonomy_term_load($tid);
        if ($col_str) { $col_str .= ', '; }
        $col_str .= $term->name;
      }
      $html .= '<strong>' . "Milestone #" . $i . ':</strong>' . $col_str . '<br />';
    }
  }

  $fields = grievance_report_open_summary_with_log_fields();

  $query = db_select('report_cache');
  $query->fields('report_cache');
  $query->condition('report_id', $report_id);
  $query->condition('report_type', $report_type);
  $stmt = $query->execute();
  $data = array();
  while ($hr = $stmt->fetchAssoc()) {
    $raw_hr = unserialize($hr['report_data']);
    $hr = array();
    $delta = 0;
    foreach ($fields as $name => $prompt) {
      $hr[$name] = $raw_hr[$delta];
      ++$delta;
    }

    if ($which == 'specialist') {
      $key = $hr['specialist'];
    } else {
      $key = $hr['shop'];
    }

    if (!$key) { $key = '[unknown]'; }

    $data[$key]['total']++;
    $totals['total']++;

    if ($hr['col_1_count']) {
      $data[$key]['col_1_yes']++;
      $totals['col_1_yes']++;
    } else {
      $data[$key]['col_1_no']++;
      $totals['col_1_no']++;
    }

    if ($hr['col_2_count']) {
      $data[$key]['col_2_yes']++;
      $totals['col_2_yes']++;
    } else {
      $data[$key]['col_2_no']++;
      $totals['col_2_no']++;
    }
  }

  ksort($data);

  $header = array();
  if ($which == 'specialist') {
    $header[] = 'Specialist';
  } else {
    $header[] = 'Employer';
  }
  $header[] = 'Total';
  if ($parameters['col_1']) { $header[] = 'With Milestone #1'; }
  if ($parameters['col_2']) { $header[] = 'With Milestone #2'; }
  if ($parameters['col_1']) { $header[] = 'Without Milestone #1'; }
  if ($parameters['col_2']) { $header[] = 'Without Milestone #2'; }

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

  if ($csv) {
    $filename = "open_grievances_summary_${which}_";

    $r = $report_id;
    $r = preg_replace('/[^a-zA-Z0-9_ ]/', '', $r);
    $r = preg_replace('/ +/', '_', $r);
    $filename .= $r;
    $filename .= ".csv";

    drupal_add_http_header('Content-Type', 'text/csv');
    drupal_add_http_header('Content-Disposition', 'attachment;filename=' . $filename);
    $fp = fopen('php://output', 'w');
    fputcsv($fp, $header);
    foreach ($rows as $row) { 
      foreach ($row as $delta => $val) { $row[$delta] = strip_tags($val); }
      fputcsv($fp, $row);
    }
    drupal_exit();
  }

  $attributes = array();

  $html .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => $attributes));
  $html .= '</div>';

  $query = $_GET;
  unset($query['q']);
  $query['csv'] = 1;
  $html .= l('CSV', current_path(), array('attributes' => array('class' => array('tool-button')), 'query' => $query));

  return $html;
}

function grievance_report_open_summary_with_log_render_detail($report_type, $report_id, $which) {
  $csv = $_REQUEST['csv'];

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
  $html .= t('Open Grievances Detail Report');
  if ($which == 'specialist') { 
    $html .= ' -- By Specialist'; 
  } else {
    $html .= ' -- By Employer'; 
  }
  $html .= '</h2>';

  if ($parameters['classaction']) {
    $html .= '<strong>' . t('General / Class Action: ') . '</strong>' . $parameters['classaction'] . '<br />';
  }
  if ($parameters['shop']) {
    $html .= '<strong>' . t('Shop: ') . '</strong>' . $parameters['shop'] . '<br />';
  }

  for ($i=1; $i<=2; ++$i) {
    if ($parameters["col_$i"]) {
      $col_str = '';
      foreach ($parameters["col_$i"] as $key) {
        list ($ignore, $tid) = explode('|', $key);
        $term = taxonomy_term_load($tid);
        if ($col_str) { $col_str .= ', '; }
        $col_str .= $term->name;
      }
      $html .= '<strong>' . "Milestone #" . $i . '</strong>' . $col_str . '<br />';
    }
  }

  $fields = grievance_report_open_summary_with_log_fields();

  $query = db_select('report_cache');
  $query->fields('report_cache');
  $query->condition('report_id', $report_id);
  $query->condition('report_type', $report_type);
  $stmt = $query->execute();
  $data = array();
  while ($hr = $stmt->fetchAssoc()) {
    $raw_hr = unserialize($hr['report_data']);
    $hr = array();
    $delta = 0;
    foreach ($fields as $name => $prompt) {
      $hr[$name] = $raw_hr[$delta];
      ++$delta;
    }

    if ($which == 'specialist') {
      $key = $hr['specialist'];
    } else {
      $key = $hr['shop'];
    }

    if (!$key) { $key = '[unknown]'; }

    $data[$key][] = $hr;
  }

  ksort($data);

  $header = array();
  if ($which == 'specialist') {
    $header[] = t('Employer');
  } else {
    $header[] = t('Specialist');
  }
  $header[] = t('Grievance ID');
  $header[] = t('First Name');
  $header[] = t('Last Name');
  $header[] = t('Classification');
  $header[] = t('Current Status');
  $header[] = t('CBA Provision(s) Violated');
  for ($i=1; $i<=2; ++$i) {
    if ($parameters["col_$i"]) {
      $header[] = t("Milestone #$i Date");
    }
  }
  if (!$csv) { $header[] = t('Tools'); }

  $rows = array();
  foreach ($data as $key => $sub_data) {
    $row = array();
    if ($csv) {
      $row[] = $key;
    } else {
      $row[] = array('data' => "<strong>$key</strong>", 'colspan' => count($header));
    }
    $rows[] = $row;

    foreach ($sub_data as $hr) {
      $row = array();
      if ($which == 'specialist') { $row[] = $hr['shop']; } 
      else { $row[] = $hr['specialist']; }
      $row[] = $hr['grievance_id'];
      $row[] = $hr['first_name'];
      $row[] = $hr['last_name'];
      $row[] = $hr['classification'];
      $row[] = $hr['current_status'];
      if ($csv) {
        $row[] = preg_replace('/\<br \/\>/', ', ', $hr['cba']);
      } else {
        $row[] = $hr['cba'];
      }
      for ($i=1; $i<=2; ++$i) {
        if ($parameters["col_$i"]) {
          if ($hr["col_${i}_date"]) { $row[] = date('Y-m-d', strtotime($hr["col_${i}_date"])); } 
          else { $row[] = 'N/A'; }
        }
      }

      if (!$csv) {
        $row[] = l('View', 'node/' . $hr['nid'], array('attributes' => array('class' => array('tool-button'))));
      }
      $rows[] = $row;
    }
  }

  if ($csv) {
    $filename = "open_grievances_detail_${which}_";

    $r = $report_id;
    $r = preg_replace('/[^a-zA-Z0-9_ ]/', '', $r);
    $r = preg_replace('/ +/', '_', $r);
    $filename .= $r;
    $filename .= ".csv";

    drupal_add_http_header('Content-Type', 'text/csv');
    drupal_add_http_header('Content-Disposition', 'attachment;filename=' . $filename);
    $fp = fopen('php://output', 'w');
    fputcsv($fp, $header);
    foreach ($rows as $row) { fputcsv($fp, $row); }
    drupal_exit();
  }

  $attributes = array();

  $html .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => $attributes));
  $html .= '</div>';

  $query = $_GET;
  unset($query['q']);
  $query['csv'] = 1;
  $html .= l('CSV', current_path(), array('attributes' => array('class' => array('tool-button')), 'query' => $query));

  return $html;
}
