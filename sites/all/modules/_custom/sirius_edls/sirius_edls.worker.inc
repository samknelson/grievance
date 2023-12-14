<?php

function sirius_edls_worker_classification_options() {
  $options =& drupal_static(__FUNCTION__);

  if (!isset($options)) {
    $terms = sirius_taxonomy_load('grievance_job_classification', 'tid', 'full');
    $options = array();
    foreach ($terms as $tid => $term) {
      if ($term->field_sirius_name_short['und'][0]['value']) {
        $options[$tid] = $term->field_sirius_name_short['und'][0]['value'];
      } else {
        $options[$tid] = $term->name;
      }
    }
  }

  return $options;
}

function sirius_edls_worker_list($employer_nid, $date, $filters = array()) {
  // There are some challenges for this function:
  // - It will be called very frequently (many supervisors, refreshing their worker list every few seconds)
  // - It may return a lot of results (~2000 workers)
  // - It is expected to return a lot of data (worker list, assignments, previous department, etc.)
  //
  // For efficiency, we're breaking some compartmentalization rules here. In an ideal world, we'd run one
  // query to fetch all the workers, and then query each worker for their current assignment, etc. 
  // 
  // But we want to avoid running ~2000 individual queries every few seconds for every active user. So 
  // we fetch as much information as we can in the one big query, and then pad it with additional information 
  // here. It's a cumbersome code design, but much more efficient.

  $outsource_workers = sirius_edls_outsource_list($employer_nid, $date, $filters);
  $dispatch_workers = sirius_edls_dispatch_list($employer_nid, $date, $filters);
  $result = array_merge($outsource_workers, $dispatch_workers);
  // $result = array_splice($result, 0, 20);

  // Filter by "has_assignment". This is hard to do in the underlying raw SQL.
  $result = array_values(array_filter($result, function($v) use ($filters) {
    if ($filters['has_assignment_curr'] == 'include') {
      if (!$v['curr_sheet_nid']) { return FALSE; }
    }

    if ($filters['has_assignment_curr'] == 'exclude') {
      if ($v['curr_sheet_nid'] && ($v['curr_sheet_nid'] != $filters['curr_sheet_nid'])) { return FALSE; }
    }

    if ($filters['has_assignment_next'] == 'include') {
      if (!$v['next_sheet_nid']) { return FALSE; }
    }

    if ($filters['has_assignment_next'] == 'exclude') {
      if ($v['next_sheet_nid']) { return FALSE; }
    }

    return TRUE;
  }));

  // Add sheet-specific information. Again, this is cumbersome to do in the worker list SQL. 
  $sheet_lookup = array();
  foreach ($result as $delta => $record) {
    foreach (array('curr', 'prev', 'next') as $prefix) {
      $sheet_nid = $record["{$prefix}_sheet_nid"];
      if ($sheet_nid) {
        if (!$sheet_lookup[$sheet_nid]) {
          $sql = "select field_sirius_edls_sheet_status_value as status, field_grievance_department_tid_tid as department ";
          $sql .= "from node ";
          $sql .= "left join field_data_field_sirius_edls_sheet_status on field_data_field_sirius_edls_sheet_status.entity_type = 'node' and field_data_field_sirius_edls_sheet_status.entity_id = node.nid ";
          $sql .= "left join field_data_field_grievance_department_tid on field_data_field_grievance_department_tid.entity_type = 'node' and field_data_field_grievance_department_tid.entity_id = node.nid ";
          $sql .= "where node.nid = :sheet_nid ";
          $stmt = sirius_sql_query($sql, array(':sheet_nid' => $sheet_nid));
          $sheet_lookup[$sheet_nid] = $stmt->fetchAssoc();
        }

        $result[$delta]["${prefix}_status"] = $sheet_lookup[$sheet_nid]['status'];
        $result[$delta]["${prefix}_department"] = $sheet_lookup[$sheet_nid]['department'];
      }
    }
  }

  // Tidy case
  $sheet_lookup = array();
  foreach ($result as $delta => $record) {
    $result[$delta]['worker_name'] = ucwords(strtolower($result[$delta]['worker_name']));
  }

  return $result;
}


function sirius_edls_worker_fetch($employer_nid, $worker_id, $date = NULL) {
  if (!$date) { $date = date('Y-m-d'); }
  if (preg_match('/^O/', $worker_id)) {
    $record = sirius_edls_outsource_worker_fetch($employer_nid, $worker_id, $date);
  }
  else if (preg_match('/^I/', $worker_id)) {
    $record = sirius_edls_dispatch_worker_fetch($employer_nid, $worker_id, $date);
  } else {
    $record = array();
  }

  $sql = "select * from sirius_edls_assignments ";
  $sql .= "where sirius_edls_assignments.worker_id = :worker_id ";
  $sql .= "and sirius_edls_assignments.employer_nid = :employer_nid ";
  $sql .= "and sirius_edls_assignments.assignment_date = :date ";
  $stmt = sirius_sql_query($sql, array(':worker_id' => $worker_id, ':employer_nid' => $employer_nid, ':date' => $date));
  $record['assignments']['curr'] = $stmt->fetchAssoc();

  $sql = "select * from sirius_edls_assignments ";
  $sql .= "where sirius_edls_assignments.worker_id = :worker_id ";
  $sql .= "and sirius_edls_assignments.employer_nid = :employer_nid ";
  $sql .= "and sirius_edls_assignments.assignment_date < :date order by assignment_date desc limit 1";
  $stmt = sirius_sql_query($sql, array(':worker_id' => $worker_id, ':employer_nid' => $employer_nid, ':date' => $date));
  $record['assignments']['prev'] = $stmt->fetchAssoc();

  $sql = "select * from sirius_edls_assignments ";
  $sql .= "where sirius_edls_assignments.worker_id = :worker_id ";
  $sql .= "and sirius_edls_assignments.employer_nid = :employer_nid ";
  $sql .= "and sirius_edls_assignments.assignment_date > :date order by assignment_date limit 1";
  $stmt = sirius_sql_query($sql, array(':worker_id' => $worker_id, ':employer_nid' => $employer_nid, ':date' => $date));
  $record['assignments']['next'] = $stmt->fetchAssoc();

  return $record;

}

function sirius_edls_dispatch_worker_fetch($employer_nid, $worker_id, $date) {
  $real_id = preg_replace('/^I/', '', $worker_id);
  $worker_node = sirius_worker_get_by_id($real_id);
  $result = array(
    'worker_id' => $worker_id, 
    'worker_name' => $worker_node->field_sirius_name['und'][0]['family'] . ', ' . $worker_node->field_sirius_name['und'][0]['given'],
  );
  return $result;
}

function sirius_edls_worker_render_details($worker_record) {
  $html .= '<div class="sirius_edls_worker_detail">';
  $html .= '<h2>' . t('Assignment Details for ') . $worker_record['worker_name'] . ' (' . $worker_record['worker_id'] . ')' . '</h2>';
  $html .= sirius_edls_worker_render_details_assignment($worker_record, $worker_record['assignments']['prev'], array('title' => t('Previous Assignment')));
  $html .= sirius_edls_worker_render_details_assignment($worker_record, $worker_record['assignments']['curr'], array('title' => t('Current Assignment')));
  $html .= sirius_edls_worker_render_details_assignment($worker_record, $worker_record['assignments']['next'], array('title' => t('Next Assignment')));
  $html .= '</div>';

  return $html;
}

function sirius_edls_worker_render_details_assignment($worker_record, $assignment, $options = array()) {
  $class = 'sirius_edls_worker_detail_assignment';
  if ($assignment) { $class .= ' sirius_edls_worker_detail_assignment_full'; } else { $class .= ' sirius_edls_worker_detail_assignment_empty'; }
  $html .= "<div class=\"$class\">";
  $html .= '<h2>' . $options['title'] . '</h2>';
  if ($assignment) {
    $sheet_nid = $assignment['sheet_nid'];
    $sheet_node = node_load($sheet_nid);

    $status_options = sirius_edls_sheet_status_options();

    $html .= sirius_edls_supervisor_render($sheet_node) . '<br /><br />';

    $html .= '<strong>' . t('Status: ') . '</strong>' . $status_options[$sheet_node->field_sirius_edls_sheet_status['und'][0]['value']] . '<br />';
    $html .= '<strong>' . t('Date: ') . '</strong>' . date('l, F j, Y', strtotime($sheet_node->field_sirius_date_start['und'][0]['value'])) . '<br />';
    $html .= '<strong>' . t('Dept.: ') . '</strong>' . sirius_term_title($sheet_node->field_grievance_department_tid['und'][0]['tid']) . '<br />';
    $html .= '<strong>' . t('Job #:') . '</strong>' . sirius_text_to_html($sheet_node->field_sirius_job_number['und'][0]['value']) . '<br />';
    $html .= '<br />';

    if (drupal_valid_path("node/$sheet_nid/sirius_edls_sheet_view")) {
      $html .= l('View', "node/$sheet_nid/sirius_edls_sheet_view", array('attributes' => array('class' => array('tool-button'))));
    }
  } else {
    $html .= '[no assignment]';
  }
  $html .= '</div>';
  return $html;
}


function sirius_edls_ms_list($employer_node, $type = 'name') {
  $terms = sirius_taxonomy_load('sirius_member_status', 'tid', 'full');
  $industry_tids = sirius_fieldvals($employer_node, 'field_sirius_industry', 'tid');

  $insource_ms_list = array();
  foreach ($terms as $tid => $term) {
    if ($industry_tids) {
      $term_industry_tids = sirius_fieldvals($term, 'field_sirius_industry', 'tid');
      if (!array_intersect($industry_tids, $term_industry_tids)) { continue; }
    }
    $insource_ms_list[$tid] = $term->name;
  }

  $outsource_ms_list = sirius_edls_outsource_ms_list($employer_node, $type);
  return $insource_ms_list + $outsource_ms_list;
}

function sirius_edls_dispatch_list($employer_nid, $date, $filters) {
  $employer_node = node_load($employer_nid);
  $industry_tids = sirius_fieldvals($employer_node, 'field_sirius_industry', 'tid');

  $sql_args = array();

  $sql = "select ";
  $sql .= "( ";
  $sql .= "  select sheet_nid from sirius_edls_assignments ";
  $sql .= "  where sirius_edls_assignments.worker_id = concat('I', field_sirius_id_value) ";
  $sql .= "  and sirius_edls_assignments.employer_nid = :employer_nid ";
  $sql .= "  and sirius_edls_assignments.assignment_date = :date ";
  $sql .= ") as curr_sheet_nid, ";
  $sql .= "( ";
  $sql .= "  select sheet_nid from sirius_edls_assignments ";
  $sql .= "  where sirius_edls_assignments.worker_id = concat('I', field_sirius_id_value) ";
  $sql .= "  and sirius_edls_assignments.employer_nid = :employer_nid ";
  $sql .= "  and sirius_edls_assignments.assignment_date < :date ";
  $sql .= "  order by assignment_date desc limit 1 ";
  $sql .= ") as prev_sheet_nid, ";
  $sql .= "( ";
  $sql .= "  select sheet_nid from sirius_edls_assignments ";
  $sql .= "  where sirius_edls_assignments.worker_id = concat('I', field_sirius_id_value) ";
  $sql .= "  and sirius_edls_assignments.employer_nid = :employer_nid ";
  $sql .= "  and sirius_edls_assignments.assignment_date > :date ";
  $sql .= "  order by assignment_date limit 1 ";
  $sql .= ") as next_sheet_nid, ";
  $sql .= "concat('I', field_sirius_id_value) as worker_id, ";
  $sql .= "concat(field_sirius_name_family, ', ', field_sirius_name_given) as worker_name, ";
  $sql .= "ms_term.tid as worker_ms ";
  $sql .= "from node ";
  $sql .= "left join field_data_field_sirius_id on field_data_field_sirius_id.entity_type = 'node' and field_data_field_sirius_id.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_name on field_data_field_sirius_name.entity_type = 'node' and field_data_field_sirius_name.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_member_status on field_data_field_sirius_member_status.entity_type = 'node' and field_data_field_sirius_member_status.entity_id = node.nid ";

  $sql .= "left join taxonomy_term_data ms_term on ms_term.tid = field_sirius_member_status_tid ";
  $sql .= "left join field_data_field_sirius_industry on field_data_field_sirius_industry.entity_type = 'taxonomy_term' and field_data_field_sirius_industry.entity_id = ms_term.tid ";

  $sql .= "left join field_data_field_sirius_worker on field_data_field_sirius_worker.entity_type = 'node' and field_data_field_sirius_worker.bundle = 'sirius_dispatch' and field_data_field_sirius_worker.field_sirius_worker_target_id = node.nid ";
  $sql .= "left join node dispatch_node on dispatch_node.nid = field_data_field_sirius_worker.entity_id ";

  $sql .= "left join field_data_field_sirius_dispatch_status on field_data_field_sirius_dispatch_status.entity_type = 'node' and field_data_field_sirius_dispatch_status.entity_id = dispatch_node.nid ";

  $sql .= "left join field_data_field_sirius_dispatch_job on field_data_field_sirius_dispatch_job.entity_type = 'node' and field_data_field_sirius_dispatch_job.entity_id = dispatch_node.nid ";
  $sql .= "left join node job_node on job_node.nid = field_sirius_dispatch_job_target_id ";

  $sql .= "left join field_data_field_grievance_shop on field_data_field_grievance_shop.entity_type = 'node' and field_data_field_grievance_shop.entity_id = job_node.nid ";

  $sql_args[':date'] = $date;

  $sql .= "where node.type = 'sirius_worker' ";

  $sql .= "and field_grievance_shop_target_id = :employer_nid ";
  $sql_args[':employer_nid'] = $employer_nid;

  $sql .= "and field_sirius_dispatch_status_value in ('accepted', 'accepted_secondary') ";

  $domain_nid = sirius_domain_nid();
  if ($domain_nid) {
    $sql .= "and field_sirius_domain_target_id = :domain_nid ";
    $sql_args[':domain_nid'] = $domain_nid;
  }

  if ($industry_tids) {
    $sql .= "and field_sirius_industry_tid in (:industry_tids) ";
    $sql_args[':industry_tids'] = $industry_tids;
  }

  if ($filters['nameid']) {
    $sql .= "and (node.title like :match or field_sirius_id_value like :match) ";
    $sql_args[':match'] = '%' . $filters['nameid'] . '%';
  }

  if ($filters['ms']) {
    $sql .= "and ms_term.tid = :ms ";
    $sql_args[':ms'] = $filters['ms'];
  }

  $sql .= "group by node.nid ";

  $sql .= "order by field_sirius_name_family, field_sirius_name_given ";

  $workers = array();
  $stmt = sirius_sql_query($sql, $sql_args);
  while ($hr = $stmt->fetchAssoc()) {
    $workers[] = $hr;
  }
  return $workers;
}

function sirius_worker_form_filters($options = array()) {
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name or ID'),
    '#id' => 'sirius_edls_worker_filter_nameid'
  );

  $form['members_status'] = array(
    '#type' => 'select',
    '#title' => t('Member Status'),
    '#options' => array(NULL => t('-- Select --')) + $options['ms_list'],
    '#id' => 'sirius_edls_worker_filter_ms'
  );

  $form['has_assignment_curr'] = array(
    '#type' => 'select',
    '#title' => t('Current Assignment'),
    '#options' => array(
      'NULL' => t('-- Select --'),
      'exclude' => t('Exclude'),
      'include' => t('Include'),
    ),
    '#default_value' => 'exclude',
    '#id' => 'sirius_edls_worker_filter_has_assignment_curr',
  );

  $form['has_assignment_next'] = array(
    '#type' => 'select',
    '#title' => t('Next Assignment'),
    '#options' => array(
      'NULL' => t('-- Select --'),
      'exclude' => t('Exclude'),
      'include' => t('Include'),
    ),
    '#id' => 'sirius_edls_worker_filter_has_assignment_next',
  );

  $form['search'] = array(
    '#type' => 'button',
    '#value' => t('Search'),
    '#id' => 'sirius_edls_worker_search',
  );

  return $form;
}

function sirius_worker_render_list($options = array()) {
  $html = '<div id="sirius_edls_workers">';
  foreach ($options['ms_list'] as $ms_key => $ms_name) {
    $html .= '<div class="sirius_edls_workers_ms_wrap sirius_edls_hidden" data-ms="' . $ms_key . '">';
    $html .= '<div class="sirius_edls_workers_ms_title">' . $ms_name . '</div>';
    $html .= '<div class="sirius_edls_workers_ms">';
    $html .= '</div>';
    $html .= '</div>';
  }

  $html .= '<div style="display:none" id="popup_worker_details_overlay"></div>';
  $html .= '<div style="display:none" id="popup_worker_details_wrap">';
  $html .= '<div id="popup_worker_details_content">';
  $html .= '[Details go here...]';
  $html .= '</div>'; // popup_worker_details_content
  $html .= '<div id="popup_worker_details_close"></div>';
  $html .= '</div>'; // popup_worker_details_wrap



  $html .= '</div>'; // sirius_worker_wrap

  return $html;
}

