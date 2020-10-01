<?php

function sirius_domain_stats_admin() {
  $form = array();

  $domains = sirius_domain_tree_render_as_select();
  $form['sirius_domain_stats_template'] = array(
    '#type' => 'select',
    '#title' => t('Template Domain'),
    '#default_value' => variable_get('sirius_domain_stats_template', ''),
    '#options' => $domains,
    '#description' => t('When generating statistics, one domain must be selected as a "template". This domain will be used to generate variable lists such as event types and participant statuses.'),
  );

  return system_settings_form($form);
}

function sirius_domain_stats_info($key) {
  $all_info = sirius_domain_stats_info_all();
  return $all_info[$key];
}

function sirius_domain_stats_info_all() {
  $info =& drupal_static(__FUNCTION__);
  if (isset($info)) { return $info; }

  // Fetch settings from the template domain
  $template_domain_nid = variable_get('sirius_domain_stats_template', '');
  if ($template_domain_nid) {
    $old_domain_nid = sirius_domain_nid();
    sirius_domain($template_domain_nid);
    $event_types = sirius_taxonomy_load('sirius_event_type', 'tid', 'name');
    $event_pstatuses = sirius_taxonomy_load('sirius_event_participant_status', 'tid', 'name');
    $event_proles = sirius_taxonomy_load('sirius_event_participant_role', 'tid', 'name');
    sirius_domain($old_domain_nid);
  }

  $ranges = sirius_domain_activity_ts_ranges();

  $info['user'] = array('title' => 'User');
  $info['user:total'] = array('title' => 'Total');
  $info["user:active"] = array('title' => 'Active');
  foreach ($ranges as $range_id => $range) {
    $info["user:active:$range_id"] = array('title' => $range['label']);
  }

  $info['user:by_role'] = array('title' => 'By Role');
  $roles = sirius_domain_stats_roles();
  foreach ($roles as $rid => $role_name) {
    $info["user:by_role:$rid"] = array('title' => $role_name);
    $info["user:by_role:$rid:total"] = array('title' => 'Total');
    $info["user:by_role:$rid:active"] = array('title' => 'Active');
    foreach ($ranges as $range_id => $range) {
      $info["user:by_role:$rid:active:$range_id"] = array('title' => $range['label']);
    }
  }

  $info['node'] = array('title' => 'Node');
  $info['node:total'] = array('title' => 'Total');
  $info["node:created"] = array('title' => 'Created');
  foreach ($ranges as $range_id => $range) {
    $info["node:created:$range_id"] = array('title' => $range['label']);
  }

  $info['node:by_type'] = array('title' => 'By Type');
  $node_types = sirius_domain_stats_node_types();
  foreach ($node_types as $type_id => $name) {
    $info["node:by_type:$type_id"] = array('title' => $name);
    $info["node:by_type:$type_id:created"] = array('title' => 'Created');
    foreach ($ranges as $range_id => $range) {
      $info["node:by_type:$type_id:created:$range_id"] = array('title' => $range['label']);
    }
  }

  $info['event'] = array('title' => 'Events');
  $info['event:total'] = array('title' => 'Total');
  $info['event:upcoming'] = array('title' => 'Upcoming');
  if ($template_domain_nid) {
    $info['event:by_type'] = array('title' => 'By Type');
    foreach ($event_types as $tid => $name) {
      $info["event:by_type:$tid"] = array('title' => $name);
      $info["event:by_type:$tid:total"] = array('title' => 'Total');
      $info["event:by_type:$tid:upcoming"] = array('title' => 'Upcoming');
    }

    $info["event:participant"] = array('title' => "Participants");
    $info["event:upcoming:participant"] = array('title' => "Participants");
    foreach ($event_proles as $prole_tid => $prole_name) {
    	$info["event:participant:$prole_tid"] = array('title' => $prole_name);
    	$info["event:upcoming:participant:$prole_tid"] = array('title' => $prole_name);
    	foreach ($event_pstatuses as $pstatus_tid => $pstatus_name) {
    		$info["event:participant:$prole_tid:$pstatus_tid"] = array('title' => $pstatus_name);
    		$info["event:upcoming:participant:$prole_tid:$pstatus_tid"] = array('title' => $pstatus_name);
    	}
    }
  }

  foreach ($info as $key => $value) {
    $parent_key = preg_replace('/\:?[^\:]+$/', '', $key);
    if ($parent_key) {
      $info[$key]['title_full'] = $info[$parent_key]['title_full'] . ': ' . $info[$key]['title'];
    } else {
      $info[$key]['title_full'] = $info[$key]['title'];
    }
    $info[$key]['depth'] = substr_count($key, ':');
  }

  ksort($info);

  return $info;
}

function sirius_domain_stats_roles() {
  $roles = user_roles(TRUE);
  unset($roles[DRUPAL_AUTHENTICATED_RID]);
  foreach ($roles as $rid => $role) {
    if (variable_get("sirius_domain_userrmanage_$rid", 'omit') == 'omit') { unset($roles[$rid]); }
  }
  return $roles;
}

function sirius_domain_stats_node_types() {
  $node_types = array(
    'sirius_contact' => 'Contact',
    'sirius_worker' => 'Worker',
    'grievance' => 'Grievance',
    'grievance_settlement' => 'Grievance Settlement',
    'sirius_bulk' => 'Bulk Message',
    'grievance_shop' => 'Employer',
    'sirius_dispatch' => 'Dispatch',
    'sirius_dispatch_facility' => 'Dispatch Facility',
    'sirius_dispatch_job' => 'Dispatch Job',
    'sirius_event' => 'Event',
    'sirius_event_participant' => 'Event Participant',
    'sirius_feed' => 'Feed',
    'sirius_ledger_account' => 'A/R Account',
    'sirius_payment' => 'A/R Payment',
    'sirius_payperiod' => 'A/R Payperiod',
    'sirius_log' => 'Log Entry',
    'sirius_news' => 'News Item',
  );

  return $node_types;
}

function sirius_domain_stats_cron() {
  $last_ts = variable_get('sirius_domain_stats_cron_last_ts', 0);
  $now_ts = time();
  if ($now_ts - $last_ts < 60*60) { return; }
  variable_set('sirius_domain_stats_cron_last_ts', $now_ts);
  sirius_domain_stats_calc_all();
}

function sirius_domain_stats_calc_all() {
  // First compute the stats for each domain
  $sql_args = array();
  $sql = "select nid, title from node where type = 'sirius_domain' order by nid";
  $stmt = sirius_sql_query($sql, array());
  $domains = array();
  while ($hr = $stmt->fetchAssoc()) {
    $domains[$hr['nid']] = $hr['title'];
  }
  foreach ($domains as $domain_nid => $title) {
    sirius_domain_stats_calc($domain_nid);
  }

  // Next, compute the tree stats, starting from each root domain
  $sql = "select nid from node ";
  $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
  $sql .= "where node.type = 'sirius_domain' ";
  $sql .= "and field_sirius_domain_target_id IS NULL ";
  $stmt = sirius_sql_query($sql, array());
  while ($hr = $stmt->fetchAssoc()) {
    sirius_domain_stats_calc_tree($hr['nid']);
  }
}

function sirius_domain_stats_calc_tree($domain_nid, $depth = 0) {
  if ($depth > 1000) { return array(); }
  $all_keys = array_keys(sirius_domain_stats_info_all());
  $domain_node = node_load($domain_nid);
  $json = sirius_jsonfield_util_node_json_get($domain_node);
  $stats_total = $json['stats'];

  $sql = "select nid, title from node ";
  $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
  $sql .= "where node.type = 'sirius_domain' ";
  $sql .= "and field_sirius_domain_target_id = :domain_nid ";
  $stmt = sirius_sql_query($sql, array(':domain_nid' => $domain_nid));
  $child_domain_nids = array();
  while ($hr = $stmt->fetchAssoc()) {
    $child_domain_nids[] = $hr['nid'];
  }

  foreach ($child_domain_nids as $child_domain_nid) {
    $child_domain_node = node_load($child_domain_nid);
    $stats_sub = sirius_domain_stats_calc_tree($child_domain_nid, $depth+1);
    foreach ($all_keys as $key) {
      if (!$stats_total[$key] && !$stats_sub[$key]) { continue; }
      $stats_total[$key] += $stats_sub[$key];
    }
  }

  $json['stats_total'] = $stats_total;
  sirius_jsonfield_util_node_json_set($domain_node, $json);
  node_save($domain_node);

  return $stats_total;
}

function sirius_domain_stats_calc($domain_nid) {
  $domain_node = node_load($domain_nid);
  sirius_minilog($domain_node->title . ": Starting statistics computation.");
  $info_all = sirius_domain_stats_info_all();
  $ranges = sirius_domain_activity_ts_ranges();
  $template_domain_nid = variable_get('sirius_domain_stats_template', '');
  $gmdate = gmdate('Y-m-d H:i:s');

  //////////////////////////////////////////////////////////////////////////////////////
  // USERS 
  //////////////////////////////////////////////////////////////////////////////////////

  // Users: Total
  $sql = "select count(*) as c ";
  $sql .= "from users ";
  $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'user' and field_data_field_sirius_domain.entity_id = users.uid ";
  $sql .= "where field_sirius_domain_target_id = :domain_nid ";
  $sql_args = array(':domain_nid' => $domain_nid);
  $stmt = sirius_sql_query($sql, $sql_args);
  $hr = $stmt->fetchAssoc();
  $count = $hr['c'];
  $key = "user:total";
  $results[$key] = $count;
  sirius_minilog($domain_node->title . ": " . $info_all[$key]['title_full'] . ": Found $count records");

  // Users: By Activity
  foreach ($ranges as $range_id => $range) {
    $sql = "select count(*) as c ";
    $sql .= "from users ";
    $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'user' and field_data_field_sirius_domain.entity_id = users.uid ";
    $sql .= "where field_sirius_domain_target_id = :domain_nid ";
    $sql .= "and access >= :ts";
    $sql_args = array(':domain_nid' => $domain_nid, ':ts' => $range['ts']);
    $stmt = sirius_sql_query($sql, $sql_args);
    $hr = $stmt->fetchAssoc();
    $count = $hr['c'];
    $key = "user:active:$range_id";
    $results[$key] = $count;
    sirius_minilog($domain_node->title . ": " . $info_all[$key]['title_full'] . ": Found $count records");
  }

  // Users: By Role
  $sql = "select rid, count(*) as c ";
  $sql .= "from users ";
  $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'user' and field_data_field_sirius_domain.entity_id = users.uid ";
  $sql .= "left join users_roles on users_roles.uid = users.uid ";
  $sql .= "where field_sirius_domain_target_id = :domain_nid ";
  $sql .= "group by rid ";
  $sql_args = array(':domain_nid' => $domain_nid);
  $stmt = sirius_sql_query($sql, $sql_args);
  while ($hr = $stmt->fetchAssoc()) {
    $rid = $hr['rid'];
    $count = $hr['c'];
    $key = "user:by_role:$rid:total";
    if (!$info_all[$key]) { continue; }
    $results[$key] = $count;
    sirius_minilog($domain_node->title . ": " . $info_all[$key]['title_full'] . ": Found $count records");

    foreach ($ranges as $range_id => $range) {
      $sql = "select count(*) as c ";
      $sql .= "from users ";
      $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'user' and field_data_field_sirius_domain.entity_id = users.uid ";
      $sql .= "left join users_roles on users_roles.uid = users.uid ";
      $sql .= "where field_sirius_domain_target_id = :domain_nid ";
      $sql .= "and access >= :ts ";
      $sql .= "and rid = :rid ";
      $sql_args = array(':domain_nid' => $domain_nid, ':rid' => $rid, ':ts' => $range['ts']);
      $sub_stmt = sirius_sql_query($sql, $sql_args);
      $hr = $sub_stmt->fetchAssoc();
      $count = $hr['c'];
      $key = "user:by_role:$rid:active:$range_id";
      $results[$key] = $count;
      sirius_minilog($domain_node->title . ": " . $info_all[$key]['title_full'] . ": Found $count records");
    }
  }

  //////////////////////////////////////////////////////////////////////////////////////
  // NODES
  //////////////////////////////////////////////////////////////////////////////////////

  // Nodes: Total
  $sql = "select count(*) as c ";
  $sql .= "from node ";
  $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
  $sql .= "where field_sirius_domain_target_id = :domain_nid ";
  $sql_args = array(':domain_nid' => $domain_nid);
  $stmt = sirius_sql_query($sql, $sql_args);
  $hr = $stmt->fetchAssoc();
  $count = $hr['c'];
  $key = "node:total";
  $results[$key] = $count;
  sirius_minilog($domain_node->title . ": " . $info_all[$key]['title_full'] . ": Found $count records");

  // Nodes: By Created
  foreach ($ranges as $range_id => $range) {
    $sql = "select count(*) as c ";
    $sql .= "from node ";
    $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
    $sql .= "where field_sirius_domain_target_id = :domain_nid ";
    $sql .= "and created >= :ts";
    $sql_args = array(':domain_nid' => $domain_nid, ':ts' => $range['ts']);
    $stmt = sirius_sql_query($sql, $sql_args);
    $hr = $stmt->fetchAssoc();
    $count = $hr['c'];
    $key = "node:created:$range_id";
    if (!$info_all[$key]) { continue; }
    $results[$key] = $count;
    sirius_minilog($domain_node->title . ": " . $info_all[$key]['title_full'] . ": Found $count records");
  }

  // Nodes: By Type
  $sql = "select type, count(*) as c ";
  $sql .= "from node ";
  $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
  $sql .= "where field_sirius_domain_target_id = :domain_nid ";
  $sql .= "group by type ";
  $sql_args = array(':domain_nid' => $domain_nid);
  $stmt = sirius_sql_query($sql, $sql_args);
  while ($hr = $stmt->fetchAssoc()) {
    $type_id = $hr['type'];
    $count = $hr['c'];
    $key = "node:by_type:$type_id";
    if (!$info_all[$key]) { continue; }
    $results[$key] = $count;
    sirius_minilog($domain_node->title . ": " . $info_all[$key]['title_full'] . ": Found $count records");

    foreach ($ranges as $range_id => $range) {
      $sql = "select count(*) as c ";
      $sql .= "from node ";
      $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
      $sql .= "where field_sirius_domain_target_id = :domain_nid ";
      $sql .= "and created >= :ts ";
      $sql .= "and type = :type_id ";
      $sql_args = array(':domain_nid' => $domain_nid, ':type_id' => $type_id, ':ts' => $range['ts']);
      $sub_stmt = sirius_sql_query($sql, $sql_args);
      $hr = $sub_stmt->fetchAssoc();
      $count = $hr['c'];
      $key = "node:by_type:$type_id:created:$range_id";
      $results[$key] = $count;
      sirius_minilog($domain_node->title . ": " . $info_all[$key]['title_full'] . ": Found $count records");
    }
  }

  //////////////////////////////////////////////////////////////////////////////////////
  // EVENTS 
  //////////////////////////////////////////////////////////////////////////////////////

  // Events: Total
  $sql = "select count(*) as c ";
  $sql .= "from node ";
  $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
  $sql .= "where field_sirius_domain_target_id = :domain_nid ";
  $sql .= "and type = 'sirius_event' ";
  $sql_args = array(':domain_nid' => $domain_nid);
  $stmt = sirius_sql_query($sql, $sql_args);
  $hr = $stmt->fetchAssoc();
  $count = $hr['c'];
  $key = "event:total";
  $results[$key] = $count;
  sirius_minilog($domain_node->title . ": " . $info_all[$key]['title_full'] . ": Found $count records");

  // Events: Upcoming
  $sql = "select count(*) as c ";
  $sql .= "from node ";
  $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
  $sql .= "where field_sirius_domain_target_id = :domain_nid ";
  $sql .= "and type = 'sirius_event' ";
  $sql .= "and exists ( ";
  $sql .= "  select 1 from field_data_field_sirius_daterepeat ";
  $sql .= "  where field_data_field_sirius_daterepeat.entity_type = 'node' ";
  $sql .= "  and field_data_field_sirius_daterepeat.entity_id = node.nid ";
  $sql .= "  and (field_sirius_daterepeat_value >= :gmdate or field_sirius_daterepeat_value2 >= :gmdate) ";
  $sql .= ") ";
  $sql_args = array(':domain_nid' => $domain_nid, ':gmdate' => $gmdate);
  $stmt = sirius_sql_query($sql, $sql_args);
  $hr = $stmt->fetchAssoc();
  $count = $hr['c'];
  $key = "event:upcoming";
  $results[$key] = $count;
  sirius_minilog($domain_node->title . ": " . $info_all[$key]['title_full'] . ": Found $count records");

  if ($template_domain_nid) {
    // Events: By Type
    $sql = "select field_sirius_event_type_tid, field_sirius_term_source_value, count(*) as c ";
    $sql .= "from node ";
    $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_event_type on field_data_field_sirius_event_type.entity_type = 'node' and field_data_field_sirius_event_type.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_term_source on field_data_field_sirius_term_source.entity_type = 'taxonomy_term' and field_data_field_sirius_term_source.entity_id = field_sirius_event_type_tid ";
    $sql .= "where field_sirius_domain_target_id = :domain_nid ";
    $sql .= "and type = 'sirius_event' ";
    $sql .= "group by field_sirius_event_type_tid";
    $sql_args = array(':domain_nid' => $domain_nid);
    $stmt = sirius_sql_query($sql, $sql_args);
    while ($hr = $stmt->fetchAssoc()) {
      $local_tid = $hr['field_sirius_event_type_tid'];
      if ($domain_nid == $template_domain_nid) { 
        $tid = $hr['field_sirius_event_type_tid'];
      } else {
        $tid = $hr['field_sirius_term_source_value'];
      }
      if (!$tid) { continue; }
      $count = $hr['c'];
      $key = "event:by_type:$tid:total";
      if (!$info_all[$key]) { continue; }
      $results[$key] = $count;
      sirius_minilog($domain_node->title . ": " . $info_all[$key]['title_full'] . ": Found $count records");

      $sql = "select count(*) as c ";
      $sql .= "from node ";
      $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
      $sql .= "left join field_data_field_sirius_event_type on field_data_field_sirius_event_type.entity_type = 'node' and field_data_field_sirius_event_type.entity_id = node.nid ";
      $sql .= "where field_sirius_domain_target_id = :domain_nid ";
      $sql .= "and type = 'sirius_event' ";
      $sql .= "and field_sirius_event_type_tid = :local_tid ";
      $sql .= "and exists ( ";
      $sql .= "  select 1 from field_data_field_sirius_daterepeat ";
      $sql .= "  where field_data_field_sirius_daterepeat.entity_type = 'node' ";
      $sql .= "  and field_data_field_sirius_daterepeat.entity_id = node.nid ";
      $sql .= "  and (field_sirius_daterepeat_value >= :gmdate or field_sirius_daterepeat_value2 >= :gmdate) ";
      $sql .= ") ";
      $sql_args = array(':domain_nid' => $domain_nid, ':local_tid' => $local_tid, ':gmdate' => $gmdate);
      $stmt = sirius_sql_query($sql, $sql_args);
      $hr = $stmt->fetchAssoc();
      $count = $hr['c'];
      $key = "event:by_type:$tid:upcoming";
      if (!$info_all[$key]) { continue; }
      $results[$key] = $count;
      sirius_minilog($domain_node->title . ": " . $info_all[$key]['title_full'] . ": Found $count records");
    }

    // Participants: By event type, participant role, and participant status
    $sql = "select type_source.field_sirius_term_source_value as type_source_tid, ";
    $sql .= "pstatus_source.field_sirius_term_source_value as pstatus_source_tid, ";
    $sql .= "prole_source.field_sirius_term_source_value as prole_source_tid, ";
    $sql .= "field_sirius_event_type_tid, ";
    $sql .= "field_sirius_event_pstatus_tid, ";
    $sql .= "field_sirius_event_prole_tid, ";
    $sql .= "count(*) as c ";
    $sql .= "from node ";
    $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_event_type on field_data_field_sirius_event_type.entity_type = 'node' and field_data_field_sirius_event_type.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_term_source type_source on type_source.entity_type = 'taxonomy_term' and type_source.entity_id = field_sirius_event_type_tid ";

    $sql .= "left join field_data_field_sirius_event on field_data_field_sirius_event.entity_type = 'node' and field_data_field_sirius_event.field_sirius_event_target_id = node.nid ";
    $sql .= "left join node participant_node on participant_node.nid = field_data_field_sirius_event.entity_id ";
    $sql .= "left join field_data_field_sirius_event_pstatus on field_data_field_sirius_event_pstatus.entity_type = 'node' and field_data_field_sirius_event_pstatus.entity_id = participant_node.nid ";
    $sql .= "left join field_data_field_sirius_term_source pstatus_source on pstatus_source.entity_type = 'taxonomy_term' and pstatus_source.entity_id = field_sirius_event_pstatus_tid ";
    $sql .= "left join field_data_field_sirius_event_prole on field_data_field_sirius_event_prole.entity_type = 'node' and field_data_field_sirius_event_prole.entity_id = participant_node.nid ";
    $sql .= "left join field_data_field_sirius_term_source prole_source on prole_source.entity_type = 'taxonomy_term' and prole_source.entity_id = field_sirius_event_prole_tid ";

    $sql .= "where field_sirius_domain_target_id = :domain_nid ";
    $sql .= "and node.type = 'sirius_event' ";
    $sql .= "group by field_sirius_event_pstatus_tid, field_sirius_event_prole_tid ";

    $sql_args = array(':domain_nid' => $domain_nid);
    $stmt = sirius_sql_query($sql, $sql_args);
    $count = 0;
    while ($hr = $stmt->fetchAssoc()) {
      if ($domain_nid == $template_domain_nid) { 
        $type_tid = $hr['field_sirius_event_type_tid'];
        $prole_tid = $hr['field_sirius_event_prole_tid'];
        $pstatus_tid = $hr['field_sirius_event_pstatus_tid'];
      } else {
        $type_tid = $hr['type_source_tid'];
        $prole_tid = $hr['prole_source_tid'];
        $pstatus_tid = $hr['pstatus_source_tid'];
      }
      if (!$prole_tid || !$pstatus_tid) { continue; }
      $key = "event:participant:$prole_tid:$pstatus_tid";
      if (!$info_all[$key]) { continue; }
      $count = $hr['c'];
      if (!$count) { $count = 0; }
      $results[$key] = $count;
      sirius_minilog($domain_node->title . ": " . $info_all[$key]['title_full'] . ": Found $count records");

      $key = "event:participant:$prole_tid";
      $results[$key] += $count;
    }

    // Participants: Upcoming: By event type, participant role, and participant status
    $sql = "select type_source.field_sirius_term_source_value as type_source_tid, ";
    $sql .= "pstatus_source.field_sirius_term_source_value as pstatus_source_tid, ";
    $sql .= "prole_source.field_sirius_term_source_value as prole_source_tid, ";
    $sql .= "field_sirius_event_type_tid, ";
    $sql .= "field_sirius_event_pstatus_tid, ";
    $sql .= "field_sirius_event_prole_tid, ";
    $sql .= "count(*) as c ";
    $sql .= "from node ";
    $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_event_type on field_data_field_sirius_event_type.entity_type = 'node' and field_data_field_sirius_event_type.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_term_source type_source on type_source.entity_type = 'taxonomy_term' and type_source.entity_id = field_sirius_event_type_tid ";

    $sql .= "left join field_data_field_sirius_event on field_data_field_sirius_event.entity_type = 'node' and field_data_field_sirius_event.field_sirius_event_target_id = node.nid ";
    $sql .= "left join node participant_node on participant_node.nid = field_data_field_sirius_event.entity_id ";
    $sql .= "left join field_data_field_sirius_event_pstatus on field_data_field_sirius_event_pstatus.entity_type = 'node' and field_data_field_sirius_event_pstatus.entity_id = participant_node.nid ";
    $sql .= "left join field_data_field_sirius_term_source pstatus_source on pstatus_source.entity_type = 'taxonomy_term' and pstatus_source.entity_id = field_sirius_event_pstatus_tid ";
    $sql .= "left join field_data_field_sirius_event_prole on field_data_field_sirius_event_prole.entity_type = 'node' and field_data_field_sirius_event_prole.entity_id = participant_node.nid ";
    $sql .= "left join field_data_field_sirius_term_source prole_source on prole_source.entity_type = 'taxonomy_term' and prole_source.entity_id = field_sirius_event_prole_tid ";

    $sql .= "and exists ( ";
    $sql .= "  select 1 from field_data_field_sirius_daterepeat ";
    $sql .= "  where field_data_field_sirius_daterepeat.entity_type = 'node' ";
    $sql .= "  and field_data_field_sirius_daterepeat.entity_id = node.nid ";
    $sql .= "  and (field_sirius_daterepeat_value >= :gmdate or field_sirius_daterepeat_value2 >= :gmdate) ";
    $sql .= ") ";

    $sql .= "where field_sirius_domain_target_id = :domain_nid ";
    $sql .= "and node.type = 'sirius_event' ";
    $sql .= "group by field_sirius_event_pstatus_tid, field_sirius_event_prole_tid ";

    $sql_args = array(':domain_nid' => $domain_nid, ':gmdate' => $gmdate);
    $stmt = sirius_sql_query($sql, $sql_args);
    $count = 0;
    while ($hr = $stmt->fetchAssoc()) {
      if ($domain_nid == $template_domain_nid) { 
        $type_tid = $hr['field_sirius_event_type_tid'];
        $prole_tid = $hr['field_sirius_event_prole_tid'];
        $pstatus_tid = $hr['field_sirius_event_pstatus_tid'];
      } else {
        $type_tid = $hr['type_source_tid'];
        $prole_tid = $hr['prole_source_tid'];
        $pstatus_tid = $hr['pstatus_source_tid'];
      }
      if (!$prole_tid || !$pstatus_tid) { continue; }
      $key = "event:upcoming:participant:$prole_tid:$pstatus_tid";
      if (!$info_all[$key]) { continue; }
      $count = $hr['c'];
      if (!$count) { $count = 0; }
      $results[$key] = $count;
      sirius_minilog($domain_node->title . ": " . $info_all[$key]['title_full'] . ": Found $count records");

      $key = "event:upcoming:participant:$prole_tid";
      $results[$key] += $count;
    }
  }

  // Save results
  sirius_minilog($domain_node->title . ": Saving results.");
  $results['updated_ts'] = time();
  $json = sirius_jsonfield_util_node_json_get($domain_node);
  $json['stats'] = $results;
  sirius_jsonfield_util_node_json_set($domain_node, $json);
  node_save($domain_node);
  sirius_minilog($domain_node->title . ": Statistics computation complete.");
}
