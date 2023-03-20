<?php

function unite_report_datawarehouse_actors_fields() {
  return array(
    'nid' => 'Actor NID',
    'name' => 'Actor Name',
  );
}

function unite_report_datawarehouse_actors_form($form, &$form_state, $report_type) {
  report_cache_form_prepare($report_type, $form, $form_state);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Generate',
  );
  
  return $form;
}

function unite_report_datawarehouse_actors_form_submit($form, &$form_state) { return report_cache_form_submit($form, $form_state); }

function unite_report_datawarehouse_actors_pks(&$form_state) {
  $sql_args = array();
  $sql = "select nid from node where type = 'actor' order by nid";
  $stmt = sirius_sql_query($sql, $sql_args);
  $nids = array();
  while ($hr = $stmt->fetchAssoc()) { $nids[] = $hr['nid']; }
  return array_chunk($nids, 1000, TRUE);
}

function unite_report_datawarehouse_actors_batch($nids, $values, $report_id, $report_action, &$context) {
  $rows = array('is_multiple' => TRUE, 'rows' => array());
  foreach ($nids as $nid) {
    $row = array();
    $row['nid'] = $nid;
    $row['name'] = sirius_node_title($nid);
    $rows['rows'][$nid] = $row;
  }
  return $rows;
}
