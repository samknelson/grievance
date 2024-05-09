<?php

function sirius_dispatch_group_list_active($industry_tid = NULL, $ts = NULL) {
  if (!$ts) { $ts = time(); }
  $date = gmdate('Y-m-d H:i:s', $ts);

	$query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_dispatch_job_group');
  $query->fieldCondition('field_sirius_datetime', 'value', $date, '<=');
  $query->fieldCondition('field_sirius_datetime_completed', 'value', $date, '>=');
  if (is_array($industry_tid)) {
    $query->fieldCondition('field_sirius_industry', 'tid', $industry_tid, 'in');
  } else if ($industry_tid) {
    $query->fieldCondition('field_sirius_industry', 'tid', $industry_tid);
  }
  $query->propertyOrderBy('title');
  $stmt = $query->execute();
  if (!$stmt['node']) { return array(); }
  $nids = array_keys($stmt['node']);

  // node_load_multiple has too much overhead
  $sql = "select nid, title from node where nid in (:nids)";
  $stmt = db_query($sql, array(':nids' => $nids));
  $results = array();
  while ($hr = $stmt->fetchAssoc()) {
  	$results[$hr['nid']] = $hr['title'];
  }
  return $results;
}

function sirius_dispatch_group_list_upcoming($industry_tid = NULL, $ts = NULL) {
  if (!$ts) { $ts = time(); }
  $date = gmdate('Y-m-d H:i:s', $ts);

  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_dispatch_job_group');
  // $query->fieldCondition('field_sirius_datetime', 'value', $date, '<=');
  $query->fieldCondition('field_sirius_datetime_completed', 'value', $date, '>=');
  if (is_array($industry_tid)) {
    $query->fieldCondition('field_sirius_industry', 'tid', $industry_tid, 'in');
  } else if ($industry_tid) {
    $query->fieldCondition('field_sirius_industry', 'tid', $industry_tid);
  }
  $query->propertyOrderBy('title');
  $stmt = $query->execute();
  if (!$stmt['node']) { return array(); }
  $nids = array_keys($stmt['node']);

  // node_load_multiple has too much overhead
  $sql = "select nid, title from node where nid in (:nids)";
  $stmt = db_query($sql, array(':nids' => $nids));
  $results = array();
  while ($hr = $stmt->fetchAssoc()) {
    $results[$hr['nid']] = $hr['title'];
  }
  return $results;
}

function sirius_dispatch_group_merge($group_nid, $from_group_nid) {
  $group_node = node_load($group_nid);
  $from_group_node = node_load($from_group_nid);

  if (!$group_node) { return array('success' => FALSE, 'msg' => t('Target is not a node.')); }
  if (!$from_group_node) { return array('success' => FALSE, 'msg' => t('Source is not a node.')); }
  if ($from_group_node->field_sirius_domain['und'][0]['target_id'] != $group_node->field_sirius_domain['und'][0]['target_id']) { return array('success' => FALSE, 'msg' => t('Cannot merge records from different domains.')); }
  if ($group_node->type != 'sirius_dispatch_job_group') { return array('success' => FALSE, 'msg' => t('Target is not a group node.')); }
  if ($from_group_node->type != 'sirius_dispatch_job_group') { return array('success' => FALSE, 'msg' => t('Source is not a group node.')); }
  if ($from_group_nid == $group_nid) { return array('success' => FALSE, 'msg' => t('Cannot merge a group with itself.')); }

  // Anything which pointed to the old group should point to the new group.
  $sql = 'update field_data_field_sirius_dispatch_job_group set field_sirius_dispatch_job_group_target_id = :group_nid where field_sirius_dispatch_job_group_target_id = :from_group_nid';
  $stmt = db_query($sql, array(':group_nid' => $group_nid, ':from_group_nid' => $from_group_nid));

  $sql = 'update field_revision_field_sirius_dispatch_job_group set field_sirius_dispatch_job_group_target_id = :group_nid where field_sirius_dispatch_job_group_target_id = :from_group_nid';
  $stmt = db_query($sql, array(':group_nid' => $group_nid, ':from_group_nid' => $from_group_nid));

  $sql = 'update field_data_field_sirius_log_handler set field_sirius_log_handler_target_id = :group_nid where field_sirius_log_handler_target_id = :from_group_nid';
  $stmt = db_query($sql, array(':group_nid' => $group_nid, ':from_group_nid' => $from_group_nid));

  $sql = 'update field_revision_field_sirius_log_handler set field_sirius_log_handler_target_id = :group_nid where field_sirius_log_handler_target_id = :from_group_nid';
  $stmt = db_query($sql, array(':group_nid' => $group_nid, ':from_group_nid' => $from_group_nid));

  // Delete the old worker
  node_delete($from_group_node);

  // Resave the new worker
  node_save($group_node);

  return array('success' => TRUE, 'msg' => t('Records merged.'));
}