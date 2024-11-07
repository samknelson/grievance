<?php

function sirius_dispatch_group_search($params) {
  $domain_nid = sirius_domain_nid();

  $sql_args = array();
  $sql = "select node.nid, title from node ";
  $sql .= "left join field_data_field_sirius_datetime on field_data_field_sirius_datetime.entity_type = 'node' and field_data_field_sirius_datetime.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_datetime_completed on field_data_field_sirius_datetime_completed.entity_type = 'node' and field_data_field_sirius_datetime_completed.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_industry on field_data_field_sirius_industry.entity_type = 'node' and field_data_field_sirius_industry.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";

  $sql .= "where node.type = 'sirius_dispatch_job_group' ";

  if ($domain_nid) {
    $sql .= "and field_sirius_domain_target_id = :domain_nid ";
    $sql_args[':domain_nid'] = $domain_nid;
  } else {
    $sql .= "and field_sirius_domain_target_id is null ";
  }
  if ($params['industry_tid']) {
    $sql .= "and field_sirius_industry_tid = :industry_tid ";
    $sql_args[':industry_tid'] = $params['industry_tid'];
  }
  if ($params['industry_tids']) {
    $sql .= "and field_sirius_industry_tid in (:industry_tids) ";
    $sql_args[':industry_tids'] = $params['industry_tids'];
  }
  if ($params['ts']) {
    $sql .= "and field_sirius_datetime_value <= :date ";
    $sql .= "and (field_sirius_datetime_completed_value >= :date or field_sirius_datetime_completed_value is null) ";
    $sql_args[':date'] = gmdate('Y-m-d H:i:s', $params['ts']);
  }
  if ($params['interval']) {

    // We want any schedule entries which overlap a given interval. There are five possibilities.
    // Interval: [...]
    // Event:    O---O
    //
    //    .....[..........]......
    // 1. .....[....O.....]......  A
    // 2. .....[..O----O..]......  A
    // 3. .....[......O---]--O...  A
    // 4. ..O--[--O.......]......  B
    // 5. ..O--[----------]--O...  B
    //
    // But all of those possibilities are covered by the following two criteria:
    // A: The start date of the event falls within the interval, OR
    // B: The start date of the interval falls within the event
    //
    // Re: https://stackoverflow.com/questions/325933/determine-whether-two-date-ranges-overlap
    // there's actually a simpler implementation, but it's problematic to implement because we can't assume that our 
    // event has an end date. The interval will always have a start and end, but the event itself might have a NULL end.

    $sql .= "and ( ";
    $sql .= "     (:interval_start <= field_sirius_datetime_value and field_sirius_datetime_value <= :interval_end) ";  // The start date of the event falls within the interval
    $sql .= "  or (field_sirius_datetime_value <= :interval_start and :interval_start <= field_sirius_datetime_completed_value) ";  // The start date of the interval falls within the event
    $sql .= "  or (field_sirius_datetime_value <= :interval_start and field_sirius_datetime_completed_value is null) ";              // The end ts is null
    $sql .= ") ";
    $sql_args[':interval_start'] = gmdate('Y-m-d H:i:s', $params['interval']['start_ts']);
    $sql_args[':interval_end'] = gmdate('Y-m-d H:i:s', $params['interval']['end_ts']);

  }
  if ($params['order']) {
    $order = $params['order'];
  } else {
    $order = "field_sirius_datetime_value, title ";
  }
  $sql .= "group by node.nid ";
  $sql .= "order by $order ";
  $limit = $params['limit'];
  if (!$limit) { $limit = 100; }
  $sql .= "limit $limit ";

  $stmt = sirius_sql_query($sql, $sql_args);
  $nids = array();
  while ($hr = $stmt->fetchAssoc()) {
    $nids[$hr['nid']] = $hr['title'];
  }

  if ($params['nids_only']) { return $nids; }
  return node_load_multiple($nids);
}

// @todo: These functions should just call _search()

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
  $domain_nid = sirius_domain_nid();
  if ($domain_nid) {
    $query->fieldCondition('field_sirius_domain', 'target_id', $domain_nid);
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