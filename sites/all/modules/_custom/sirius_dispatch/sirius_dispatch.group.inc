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
  if (!$stmt['node']) { return NULL; }
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