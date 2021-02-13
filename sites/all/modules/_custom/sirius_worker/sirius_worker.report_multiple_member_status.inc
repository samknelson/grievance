<?php

function sirius_worker_report_multiple_member_status_form($form, &$form_state, $report_type) {
  report_cache_form_prepare($report_type, $form, $form_state);
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Generate',
  );
  
  return $form;
}

function sirius_worker_report_multiple_member_status_form_submit($form, &$form_state) { return report_cache_form_submit($form, $form_state); }

function sirius_worker_report_multiple_member_status_fields() {
  $fields = array();
  $fields['worker_nid'] = 'Worker NID';
  $fields['worker_title'] = 'Name';
  $fields['worker_id'] = 'Worker ID';
  $fields["ms_tid_1"] = "Member Status 1 TID";
  $fields["ms_name_1"] = "Member Status 1 Name";
  $fields["ms_tid_2"] = "Member Status 2 TID";
  $fields["ms_name_2"] = "Member Status 2 Name";

  return $fields;
}

function sirius_worker_report_multiple_member_status_pks(&$form_state) {
  $domain_nid = sirius_domain_nid();
  $sql_args = array();

  $sql = "select distinct nid from node ";
  $sql .= "left join field_data_field_sirius_member_status on field_data_field_sirius_member_status.entity_type = 'node' and field_data_field_sirius_member_status.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
  $sql .= "where node.type = 'sirius_worker' ";
  $sql .= "and field_data_field_sirius_member_status.delta > 0 ";
  if ($domain_nid) { 
    $sql .= "and field_sirius_domain_target_id = :domain_nid ";
    $sql_args[':domain_nid'] = $domain_nid;
  } else {
    $sql .= "and field_sirius_domain_target_id is NULL ";
  }
  $sql .= "order by nid ";

  $stmt = sirius_sql_query($sql, $sql_args);
  $worker_nids = array();
  while ($hr = $stmt->fetchAssoc()) {
    $worker_nids[] = $hr['nid'];
  }
  return array_chunk($worker_nids, 100, TRUE);
}

function sirius_worker_report_multiple_member_status_batch($worker_nids, $values, $report_id, $report_action, &$context) {
  // Map member statuses to industries
  $ms_terms = sirius_taxonomy_load('sirius_member_status', 'tid', 'full');
  $industry_terms = sirius_taxonomy_load('sirius_industry', 'tid', 'name');
  foreach ($ms_terms as $ms_tid => $ms_term) {
    if (!$ms_term->field_sirius_industry['und'][0]) { continue; }
    foreach ($ms_term->field_sirius_industry['und'] as $tmp) {
      $industry_tid = $tmp['tid'];
      $ms_industry_map[$ms_tid][] = $industry_tid;
    }
  }

  $rows = array('is_multiple' => TRUE, 'rows' => array());
  $worker_nids_with_multiple = array();
  foreach ($worker_nids as $worker_nid) {
    $sql = "select distinct field_sirius_member_status_tid as ms_tid from field_data_field_sirius_member_status ";
    $sql .= "where entity_type = 'node' ";
    $sql .= "and entity_id = :worker_nid ";
    $stmt = db_query($sql, array(':worker_nid' => $worker_nid));
    $found_industries = array();
    $is_multiple = FALSE;
    while ($hr = $stmt->fetchAssoc()) {
      $ms_tid = $hr['ms_tid'];
      if ($ms_industry_map[$ms_tid]) {
        foreach ($ms_industry_map[$ms_tid] as $industry_tid) {
          if ($found_industries[$industry_tid]) { 
            $worker_nids_with_multiple[$worker_nid][] = array($found_industries[$industry_tid], $ms_tid);
            $is_multiple = TRUE;
          }
          $found_industries[$industry_tid] = $ms_tid; 
        }
      }
    }
    if ($is_multiple) { $worker_nids_with_multiple[] = $worker_nid; }
  }

  foreach ($worker_nids_with_multiple as $worker_nid => $pairs) {
    $sql = "select title from node where nid = :worker_nid";
    $stmt = db_query($sql, array(':worker_nid' => $worker_nid));
    $hr = $stmt->fetchAssoc();
    $worker_title = $hr['title'];

    $sql = "select field_sirius_id_value as id from field_data_field_sirius_id where entity_type = 'node' and entity_id = :worker_nid";
    $stmt = db_query($sql, array(':worker_nid' => $worker_nid));
    $hr = $stmt->fetchAssoc();
    $worker_id = $hr['id'];

    foreach ($pairs as $delta => $pair) {
      $row = array();
      $row['worker_nid'] = $worker_nid;
      $row['worker_title'] = $worker_title;
      $row['worker_id'] = $worker_id;
      $row['ms_tid_1'] = $pair[0];
      $row['ms_name_1'] = $ms_terms[$pair[0]]->name;
      $row['ms_tid_2'] = $pair[1];
      $row['ms_name_2'] = $ms_terms[$pair[1]]->name;
      $rows['rows']["$worker_nid|$delta"] = $row;
    }
  }

  return $rows;
}

function sirius_worker_report_multiple_member_status_view($report_type, $report_id) {
  // Load the parameters
  $query = db_select('report_cache');
  $query->fields('report_cache');
  $query->condition('report_id', $report_id);
  $query->condition('report_type', $report_type . ' :: PARAMETERS');
  $stmt = $query->execute();
  $hr = $stmt->fetchAssoc();
  $parameters = unserialize($hr['report_data']);
  $ratio_1_tid = $parameters['ratio_1'];
  $ratio_2_tid = $parameters['ratio_2'];
  $ratio_1_term = taxonomy_term_load($ratio_1_tid);
  $ratio_2_term = taxonomy_term_load($ratio_2_tid);

  $html = '';
  $html .= '<h2>';
  $html .= t('Workers with multiple member statuses');
  $html .= '</h2>';

  $query = db_select('report_cache');
  $query->fields('report_cache');
  $query->condition('report_id', $report_id);
  $query->condition('report_type', $report_type);
  $stmt = $query->execute();
  $data = array();
  while ($hr = $stmt->fetchAssoc()) {
    $hr = unserialize($hr['report_data']);
    $data[] = $hr;
  }

  $fields = sirius_worker_report_multiple_member_status_fields();
  $fields['tools'] = t('Tools');

  $rows = array();
  foreach ($data as $identifier => $subdata) {
    $subdata['tools'] = l('Edit', 'node/' . $subdata['worker_nid'] . '/sirius_worker_industry', array('attributes' => array('target' => '_blank', 'class' => array('tool-button'))));
    $rows[] = $subdata;
  }

  $header = array_values($fields);

  $attributes = array();
  $html .= theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => $attributes));
  return $html;
}
