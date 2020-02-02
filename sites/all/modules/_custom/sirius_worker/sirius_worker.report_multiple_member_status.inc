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

  for ($i=0; $i<=5; ++$i) {
    $fields["ms_tid_$i"] = "Member Status $i TID";
    $fields["ms_name_$i"] = "Member Status $i Name";
  }

  return $fields;
}

function sirius_worker_report_multiple_member_status_pks(&$form_state) {

  /*
  $sql = "select distinct field_data_field_sirius_member_status.entity_id ";
  $sql .= "from field_data_field_sirius_member_status ";
  $sql .= "join field_data_field_sirius_industry on field_data_field_sirius_industry.entity_type = 'taxonomy_term' and field_data_field_sirius_industry.bundle = 'sirius_member_status' and field_data_field_sirius_industry.entity_id = field_sirius_member_status_tid ";
  $sql .= "where count(distinct field_sirius_member_status_tid) > count(distinct field_data_field_sirius_industry.entity_id) ";
  $sql .= "group by field_data_field_sirius_member_status.entity_id ";
  $sql .= "order by field_data_field_sirius_member_status.entity_id ";
  */
  /*
  $sql = "select distinct entity_id as nid from field_data_field_sirius_member_status ";
  $sql .= "where entity_type = 'node' ";
  $sql .= "and bundle = 'sirius_worker' ";
  $sql .= "and delta > 0 ";
  $sql .= "order by entity_id ";
  */
  $stmt = db_query($sql, array());
  $worker_nids = array();
  while ($hr = $stmt->fetchAssoc()) {
    $worker_nids[] = $hr['nid'];
  }
  return array_chunk($worker_nids, 100, TRUE);
}

function sirius_worker_report_multiple_member_status_batch($worker_nids, $values, $report_id, $report_action, &$context) {
  $rows = array('is_multiple' => TRUE, 'rows' => array());
  foreach ($worker_nids as $worker_nid) {
    $row = array();
    $row['worker_nid'] = $worker_nid;
    $rows['rows'][$worker_nid] = $row;

    /*
    $sql = "select * from node, $table where node.nid = $table.entity_id and bundle = 'sirius_worker' and $col = :identifier";
    $stmt = db_query($sql, array(':identifier' => $identifier));
    while ($hr = $stmt->fetchAssoc()) {
      $row = array();
      $row['worker_nid'] = $hr['nid'];
      $row['worker_title'] = $hr['title'];
      if (!$row['worker_title']) { $row['worker_title'] = '(no name)'; }
      $row['identifier'] = $identifier;
      $rows['rows'][$identifier . '/' . $hr["nid"]] = $row;
    }
    */
  }
  return $rows;
}

/*
function sirius_worker_report_duplicate_id_view($report_type, $report_id) {
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
  $html .= 'Duplicate workers by ' . $parameters['key'];
  $html .= '</h2>';

  $query = db_select('report_cache');
  $query->fields('report_cache');
  $query->condition('report_id', $report_id);
  $query->condition('report_type', $report_type);
  $stmt = $query->execute();
  $data = array();
  while ($hr = $stmt->fetchAssoc()) {
    $hr = unserialize($hr['report_data']);
    $identifier = $hr['identifier'];
    $data[$identifier][] = $hr;
  }

  ksort($data);

  $rows = array();
  foreach ($data as $identifier => $subdata) {
    $is_first = TRUE;
    foreach ($subdata as $delta => $hr) {
      $prev_worker_nid = $subdata[$delta-1]['worker_nid'];
      $next_worker_nid = $subdata[$delta+1]['worker_nid'];

      $tools = l('View', 'node/' . $hr['worker_nid'], array('attributes' => array('class' => 'tool-button', 'target' => '_blank')));
      if ($prev_worker_nid) { 
        $tools .= ' ' . l('Merge from Above', 'node/' . $hr['worker_nid'] . '/sirius_worker_merge', array('attributes' => array('class' => 'tool-button', 'target' => '_blank'), 'query' => array('from_worker_nid' => $prev_worker_nid)));
      }
      if ($next_worker_nid) { 
        $tools .= ' ' . l('Merge from Below', 'node/' . $hr['worker_nid'] . '/sirius_worker_merge', array('attributes' => array('class' => 'tool-button', 'target' => '_blank'), 'query' => array('from_worker_nid' => $next_worker_nid)));
      }

      $row = array(); 
      if ($is_first) {
        $is_first = FALSE;
        $row[] = '<strong>' . $identifier . '</strong>';
      } else { 
        $row[] = '';
      }
      $row[] = $hr['worker_title'];
      $row[] = $tools;
      $rows[] = $row;
    }
  }

  $header = array(
    'Identifier',
    'Worker Name',
    'Tools',
  );

  $attributes = array();
  $html .= theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => $attributes));
  return $html;
}
*/