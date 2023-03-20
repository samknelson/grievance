<?php

function sirius_smf_feed_notes_row_process($feed_node, $info, $data, $row) {

  $result = sirius_smf_feed_notes_row_verify($feed_node, $info, $data, $row, TRUE);
  if (!$result['success']) { return $result; }
  $warnings = $result['warnings'];

  $domain_nid = sirius_domain_nid();
  $fields = &drupal_static(__FUNCTION__);
  if (!isset($fields)) { $fields = sirius_feed_wizard_fields($info, $data, $feed_node, $current_step); }

  $log_text = array();
  foreach (array('P', 'Q', 'R', 'S', 'T') as $col) {
    if ($row[$col]) {
      $log_text[] = $row[$col];
    }
  }
  $log_text = join("\n", $log_text);
  $pk = $row['pk'];
  $log_nid = $row['log_nid'];
  $entity_nid = $row['entity_nid'];

  if ($log_nid) {
    $log_node = node_load($log_nid);
    $log_node->field_sirius_summary['und'][0]['value'] = $log_text;
    node_save($log_node);
    $msg = "Updated an existing log entry.";
  } else {
    $log_node = sirius_log('smf:notes', 'raw', 'Imported notes', $log_text, $pk, array($entity_nid));
    $msg = "Created a new log entry.";
  }

  // Manually set the log date
  $date = $row['date'];
  $ts = strtotime($date);
  $sqls = array(
    "update node set created = :ts, changed = :ts where nid = :nid",
    "update node_revision set timestamp = :ts where nid = :nid",
  );
  foreach ($sqls as $sql) {
    $stmt = sirius_sql_query($sql, array(':ts' => $ts, 'nid' => $log_node->nid));
  }

  return array('success' => FALSE, 'msg' => $msg);
}
