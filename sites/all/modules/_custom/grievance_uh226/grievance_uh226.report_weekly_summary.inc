<?php

function grievance_uh226_report_summary() {
  return drupal_get_form('grievance_uh226_report_summary_form');
}

function grievance_uh226_report_summary_form($form, &$form_state) {

  if ($form_state['values']['from_date'] && $form_state['values']['to_date']) {
    $from_date = $form_state['values']['from_date']['year'] . '-' . $form_state['values']['from_date']['month'] . '-' . $form_state['values']['from_date']['day'];
    $from_ts = strtotime($from_date);

    $to_date = $form_state['values']['to_date']['year'] . '-' . $form_state['values']['to_date']['month'] . '-' . $form_state['values']['to_date']['day'];
    $to_ts = strtotime($to_date);

    drupal_set_title("Weekly Summary Report: $from_date - $to_date");
  }

  $from_ts = strtotime('-7 days');

  $form['open'] = array('#markup' => '<div class="sirius_noprint">');

  $form['from_date'] = array(
    '#type' => 'date',
    '#title' => t('From Date'),
    '#required' => TRUE,
    '#default_value' => array(
      'year' => date('Y', $from_ts),
      'month' => date('n', $from_ts),
      'day' => date('j', $from_ts),
    ),
  );

  $form['to_date'] = array(
    '#type' => 'date',
    '#title' => t('Through Date'),
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Generate Report',
  );

  $form['close'] = array('#markup' => '</div>');

  if ($form_state['results']) {
    $form['results'] = array(
      '#type' => 'markup',
      '#markup' => $form_state['results'],
    );
  }

  return $form;
}

function grievance_uh226_report_summary_form_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;

  // Get the list of open and closed statuses
  $status_vocabulary = taxonomy_vocabulary_machine_name_load('grievance_status');
  $status_terms = array();
  $tmp = taxonomy_get_tree($status_vocabulary->vid);
  foreach ($tmp as $status_term) {
    $status_terms[$status_term->tid] = taxonomy_term_load($status_term->tid);
  }
  $open_status_tids = array(-999);
  $closed_status_tids = array(-999);
  $arbitration_status_tids = array(-999);
  $fmcs_status_tids = array(-999);
  $adr_status_tids = array(-999);
  $formal_arb_held_status_tids = array(-999);
  $exp_arb_status_tids = array(-999);
  $victory_status_tids = array(-999);
  $boa_held_status_tids = array(-999);
  $mediation_status_tids = array(-999);

  foreach ($status_terms as $tid => $term) {
    if ($term->field_grievance_open['und'][0]['value'] == 1) {
      $open_status_tids[] = $tid;
    }
    if ($term->field_grievance_open['und'][0]['value'] == 0) {
      $closed_status_tids[] = $tid;
    }
    if (preg_match('/referred/i', $term->name)) {
      $arbitration_status_tids[] = $tid;
    }
    if (preg_match('/FMCS Held/i', $term->name)) {
      $fmcs_status_tids[] = $tid;
    }
    if (preg_match('/Mediation Held/i', $term->name)) {
      $mediation_status_tids[] = $tid;
    }
    if (preg_match('/ADR Held/i', $term->name)) {
      $adr_status_tids[] = $tid;
    }
    if (preg_match('/Formal Arbitration Held/i', $term->name)) {
      $formal_status_tids[] = $tid;
    }
    if (preg_match('/Expedited Arbitration Held/i', $term->name)) {
      $exp_arb_status_tids[] = $tid;
    }
    if (preg_match('/(resolved|won)/i', $term->name) && 
        !preg_match('/unresolved/i', $term->name)) {
      $victory_status_tids[] = $tid;
    }
    if (preg_match('/BOA Held/i', $term->name)) {
      $boa_held_status_tids[] = $tid;
    }
  }

  // Get the list of "Term" nids
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_contract_clause');
  $query->propertyCondition('title', '%Discharge%', 'like');
  $stmt = $query->execute();
  $term_nids = array(-999);
  if ($stmt['node']) {
    $term_nids = array_keys($stmt['node']);
  }

  // Get the list of "Resolution Procedure" tids (aka "arbs")
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'taxonomy_term');
  $query->entityCondition('bundle', 'grievance_status');
  $query->fieldCondition('field_grievance_resproc', 'tid', 'NULL', '!=');
  $query->fieldCondition('field_grievance_open', 'value', 1);
  $stmt = $query->execute();
  $arb_status_tids = array(-999);
  if ($stmt['taxonomy_term']) {
    $arb_status_tids = array_keys($stmt['taxonomy_term']);
  }

  // Get the list of log types corresponding to "call to grievant".
  // @todo this should be configurable, but what the heck.
  $log_type_call_tids = array(441, 503, 85);
  $log_type_first_call_tid = 503;
  $log_tag_left_message_tid = 874;
  $status_boa_held_tid = 112;

  // Start date
  $from_date = $form_state['values']['from_date']['year'] . '-' . $form_state['values']['from_date']['month'] . '-' . $form_state['values']['from_date']['day'];
  $from_ts = strtotime($from_date);

  $to_date = $form_state['values']['to_date']['year'] . '-' . $form_state['values']['to_date']['month'] . '-' . $form_state['values']['to_date']['day'];
  $to_ts = strtotime($to_date);

  // Get all users.  We really only want organizers, but potentially some non-organizers might be assigned...
  $sql = "select * from users where status = 1 order by name";
  $stmt = db_query($sql, array());
  $users = array();

  while ($hr = $stmt->fetchAssoc()) {
    $hr['closed'] = 0;
    $hr['victories'] = 0;
    $hr['boa_held'] = 0;
    $hr['adr_held'] = 0;
    $hr['fmcs_held'] = 0;
    $hr['mediation_held'] = 0;
    $hr['formal_held'] = 0;
    $hr['exp_arb_held'] = 0;
    $users[$hr['uid']] = $hr;
  }

  // Fetch everything closed this week
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_log');
  $query->fieldCondition('field_grievance_status_date', 'value', $from_date, ">=");
  $query->fieldCondition('field_grievance_status_date', 'value', $to_date, "<=");
  $query->fieldCondition('field_grievance_log_type', 'tid', variable_get('grievance_log_update_type_with_status_change', ''));
  $query->fieldCondition('field_grievance_status', 'tid', $closed_status_tids, 'in');
  $stmt = $query->execute();
  $count_once = array();
  if ($stmt['node']) {
    $log_nids = array_keys($stmt['node']);
    foreach ($log_nids as $log_nid) {
      $log_node = node_load($log_nid);
      $grievance_nid = $log_node->field_grievance['und'][0]['target_id'];
      if ($count_once[$grievance_nid]) { continue; }
      $count_once[$grievance_nid] = TRUE;
      $grievance_node = node_load($grievance_nid);
      if ($grievance_node->field_grievance_rep_organizer['und'][0]['target_id']) {
        $users[$grievance_node->field_grievance_rep_organizer['und'][0]['target_id']]['closed']++;
      }
    }
  }

  // Fetch victories this week
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_log');
  $query->fieldCondition('field_grievance_status_date', 'value', $from_date, ">=");
  $query->fieldCondition('field_grievance_status_date', 'value', $to_date, "<=");
  $query->fieldCondition('field_grievance_log_type', 'tid', variable_get('grievance_log_update_type_with_status_change', ''));
  $query->fieldCondition('field_grievance_status', 'tid', $victory_status_tids, 'in');
  $stmt = $query->execute();
  $count_once = array();
  if ($stmt['node']) {
    $log_nids = array_keys($stmt['node']);
    foreach ($log_nids as $log_nid) {
      $log_node = node_load($log_nid);
      $grievance_nid = $log_node->field_grievance['und'][0]['target_id'];
      if ($count_once[$grievance_nid]) { continue; }
      $count_once[$grievance_nid] = TRUE;
      $grievance_node = node_load($grievance_nid);
      if ($grievance_node->field_grievance_rep_organizer['und'][0]['target_id']) {
        $users[$grievance_node->field_grievance_rep_organizer['und'][0]['target_id']]['victories']++;
      }
    }
  }

  // Fetch ADR Held this week
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_log');
  $query->fieldCondition('field_grievance_status_date', 'value', $from_date, ">=");
  $query->fieldCondition('field_grievance_status_date', 'value', $to_date, "<=");
  $query->fieldCondition('field_grievance_log_type', 'tid', variable_get('grievance_log_update_type_with_status_change', ''));
  $query->fieldCondition('field_grievance_status', 'tid', $adr_status_tids, 'in');
  $stmt = $query->execute();
  $count_once = array();
  if ($stmt['node']) {
    $log_nids = array_keys($stmt['node']);
    foreach ($log_nids as $log_nid) {
      $log_node = node_load($log_nid);
      $grievance_nid = $log_node->field_grievance['und'][0]['target_id'];
      if ($count_once[$grievance_nid]) { continue; }
      $count_once[$grievance_nid] = TRUE;
      $grievance_node = node_load($grievance_nid);
      if ($grievance_node->field_grievance_rep_organizer['und'][0]['target_id']) {
        $users[$grievance_node->field_grievance_rep_organizer['und'][0]['target_id']]['adr_held']++;
      }
    }
  }

  // Fetch FMCS Held this week
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_log');
  $query->fieldCondition('field_grievance_status_date', 'value', $from_date, ">=");
  $query->fieldCondition('field_grievance_status_date', 'value', $to_date, "<=");
  $query->fieldCondition('field_grievance_log_type', 'tid', variable_get('grievance_log_update_type_with_status_change', ''));
  $query->fieldCondition('field_grievance_status', 'tid', $fmcs_status_tids, 'in');
  $stmt = $query->execute();
  $count_once = array();
  if ($stmt['node']) {
    $log_nids = array_keys($stmt['node']);
    foreach ($log_nids as $log_nid) {
      $log_node = node_load($log_nid);
      $grievance_nid = $log_node->field_grievance['und'][0]['target_id'];
      if ($count_once[$grievance_nid]) { continue; }
      $count_once[$grievance_nid] = TRUE;
      $grievance_node = node_load($grievance_nid);
      if ($grievance_node->field_grievance_rep_organizer['und'][0]['target_id']) {
        $users[$grievance_node->field_grievance_rep_organizer['und'][0]['target_id']]['fmcs_held']++;
      }
    }
  }

  // Fetch Mediation Held this week
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_log');
  $query->fieldCondition('field_grievance_status_date', 'value', $from_date, ">=");
  $query->fieldCondition('field_grievance_status_date', 'value', $to_date, "<=");
  $query->fieldCondition('field_grievance_log_type', 'tid', variable_get('grievance_log_update_type_with_status_change', ''));
  $query->fieldCondition('field_grievance_status', 'tid', $mediation_status_tids, 'in');
  $stmt = $query->execute();
  $count_once = array();
  if ($stmt['node']) {
    $log_nids = array_keys($stmt['node']);
    foreach ($log_nids as $log_nid) {
      $log_node = node_load($log_nid);
      $grievance_nid = $log_node->field_grievance['und'][0]['target_id'];
      if ($count_once[$grievance_nid]) { continue; }
      $count_once[$grievance_nid] = TRUE;
      $grievance_node = node_load($grievance_nid);
      if ($grievance_node->field_grievance_rep_organizer['und'][0]['target_id']) {
        $users[$grievance_node->field_grievance_rep_organizer['und'][0]['target_id']]['mediation_held']++;
      }
    }
  }

  // Fetch formal Held this week
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_log');
  $query->fieldCondition('field_grievance_status_date', 'value', $from_date, ">=");
  $query->fieldCondition('field_grievance_status_date', 'value', $to_date, "<=");
  $query->fieldCondition('field_grievance_log_type', 'tid', variable_get('grievance_log_update_type_with_status_change', ''));
  $query->fieldCondition('field_grievance_status', 'tid', $formal_status_tids, 'in');
  $stmt = $query->execute();
  $count_once = array();
  if ($stmt['node']) {
    $log_nids = array_keys($stmt['node']);
    foreach ($log_nids as $log_nid) {
      $log_node = node_load($log_nid);
      $grievance_nid = $log_node->field_grievance['und'][0]['target_id'];
      if ($count_once[$grievance_nid]) { continue; }
      $count_once[$grievance_nid] = TRUE;
      $grievance_node = node_load($grievance_nid);
      if ($grievance_node->field_grievance_rep_organizer['und'][0]['target_id']) {
        $users[$grievance_node->field_grievance_rep_organizer['und'][0]['target_id']]['formal_held']++;
      }
    }
  }

  // Fetch Expedited Arb Held this week
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_log');
  $query->fieldCondition('field_grievance_status_date', 'value', $from_date, ">=");
  $query->fieldCondition('field_grievance_status_date', 'value', $to_date, "<=");
  $query->fieldCondition('field_grievance_log_type', 'tid', variable_get('grievance_log_update_type_with_status_change', ''));
  $query->fieldCondition('field_grievance_status', 'tid', $exp_arb_status_tids, 'in');
  $stmt = $query->execute();
  $count_once = array();
  if ($stmt['node']) {
    $log_nids = array_keys($stmt['node']);
    foreach ($log_nids as $log_nid) {
      $log_node = node_load($log_nid);
      $grievance_nid = $log_node->field_grievance['und'][0]['target_id'];
      if ($count_once[$grievance_nid]) { continue; }
      $count_once[$grievance_nid] = TRUE;
      $grievance_node = node_load($grievance_nid);
      if ($grievance_node->field_grievance_rep_organizer['und'][0]['target_id']) {
        $users[$grievance_node->field_grievance_rep_organizer['und'][0]['target_id']]['exp_arb_held']++;
      }
    }
  }

  // Fetch BOA Held
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_log');
  $query->fieldCondition('field_grievance_status_date', 'value', $from_date, ">=");
  $query->fieldCondition('field_grievance_status_date', 'value', $to_date, "<=");
  $query->fieldCondition('field_grievance_log_type', 'tid', variable_get('grievance_log_update_type_with_status_change', ''));
  $query->fieldCondition('field_grievance_status', 'tid', $boa_held_status_tids, 'in');
  $stmt = $query->execute();
  $count_once = array();
  if ($stmt['node']) {
    $log_nids = array_keys($stmt['node']);
    foreach ($log_nids as $log_nid) {
      $log_node = node_load($log_nid);
      $grievance_nid = $log_node->field_grievance['und'][0]['target_id'];
      if ($count_once[$grievance_nid]) { continue; }
      $count_once[$grievance_nid] = TRUE;
      $grievance_node = node_load($grievance_nid);
      if ($grievance_node->field_grievance_rep_organizer['und'][0]['target_id']) {
        $users[$grievance_node->field_grievance_rep_organizer['und'][0]['target_id']]['boa_held']++;
      }
    }
  }

  // Our report data
  $report_rows = array();

  // For each user
  foreach ($users as $uid => $user) {
    // Our report row
    $report_row = array();
    $report_row['name'] = $user['name'];
    $report_row['closed'] = $user['closed'];
    $report_row['victories'] = $user['victories'];
    $report_row['boa_held'] = $user['boa_held'];
    $report_row['adr_held'] = $user['adr_held'];
    $report_row['fmcs_held'] = $user['fmcs_held'];
    $report_row['formal_held'] = $user['formal_held'];
    $report_row['mediation_held'] = $user['mediation_held'];
    $report_row['exp_arb_held'] = $user['exp_arb_held'];

    // Fetch the number of open grievances for each user
    $query = new EntityFieldQuery;
    $query->entityCondition('entity_type', 'node');
    $query->entityCondition('bundle', 'grievance');
    $query->fieldCondition('field_grievance_rep_organizer', 'target_id', $user['uid']);
    $query->fieldCondition('field_grievance_status', 'tid', $open_status_tids, 'in');
    $stmt = $query->execute();
    if (!$stmt['node']) { continue; }
    $report_row['open'] = count(array_keys($stmt['node']));

    // Fetch the number of arbs
    $query = new EntityFieldQuery;
    $query->entityCondition('entity_type', 'node');
    $query->entityCondition('bundle', 'grievance');
    $query->fieldCondition('field_grievance_rep_organizer', 'target_id', $user['uid']);
    $query->fieldCondition('field_grievance_status', 'tid', $open_status_tids, 'in');
    $query->fieldCondition('field_grievance_status', 'tid', $arb_status_tids, 'in');
    $stmt = $query->execute();
    $report_row['arb'] = 0;
    if ($stmt['node']) {
      $report_row['arb'] = count(array_keys($stmt['node']));
    }

    // Fetch the total calls this week
    /*
    $query = new EntityFieldQuery;
    $query->entityCondition('entity_type', 'node');
    $query->entityCondition('bundle', 'grievance_log');
    $query->fieldCondition('field_grievance_log_type', 'tid', $log_type_call_tids, 'in');
    $query->fieldCondition('field_grievance_status_date', 'value', $from_date, ">=");
    $query->fieldCondition('field_grievance_status_date', 'value', $to_date, "<=");
    $query->propertyCondition('uid', $user['uid']);
    $stmt = $query->execute();
    $report_row['calls'] = 0;
    if ($stmt['node']) {
      $report_row['calls'] = count(array_keys($stmt['node']));
    }
    */

    $sql = "select count(*) as c ";
    $sql .= "from node grievance_node ";
    $sql .= "join field_data_field_grievance_rep_organizer ";
    $sql .= "  on field_data_field_grievance_rep_organizer.entity_type = 'node' ";
    $sql .= "  and field_data_field_grievance_rep_organizer.entity_id = grievance_node.nid ";
    $sql .= "  and field_grievance_rep_organizer_target_id = :uid ";
    $sql .= "join field_data_field_grievance ";
    $sql .= "  on field_data_field_grievance.entity_type = 'node' ";
    $sql .= "  and field_data_field_grievance.bundle = 'grievance_log' ";
    $sql .= "  and field_grievance_target_id = grievance_node.nid ";
    $sql .= "join field_data_field_grievance_log_type ";
    $sql .= "  on field_data_field_grievance_log_type.entity_type = 'node' ";
    $sql .= "  and field_data_field_grievance_log_type.entity_id = field_data_field_grievance.entity_id ";
    $sql .= "  and field_grievance_log_type_tid in (:log_type_call_tids) ";
    $sql .= "join field_data_field_grievance_status_date ";
    $sql .= "  on field_data_field_grievance_status_date.entity_type = 'node' ";
    $sql .= "  and field_data_field_grievance_status_date.entity_id = field_data_field_grievance.entity_id ";
    $sql .= "  and field_grievance_status_date_value >= :from_date ";
    $sql .= "  and field_grievance_status_date_value <= :to_date ";
    $args = array(
      ':uid' => $user['uid'],
      ':log_type_call_tids' => $log_type_call_tids,
      ':from_date' => $from_date,
      ':to_date' => $to_date,
    );
    $stmt = db_query($sql, $args);
    $hr = $stmt->fetchAssoc();
    $report_row['calls'] = $hr['c'];

    // Fetch the first calls this week
    /*
    $query = new EntityFieldQuery;
    $query->entityCondition('entity_type', 'node');
    $query->entityCondition('bundle', 'grievance_log');
    $query->fieldCondition('field_grievance_log_type', 'tid', $log_type_first_call_tid);
    $query->fieldCondition('field_grievance_status_date', 'value', $from_date, ">=");
    $query->fieldCondition('field_grievance_status_date', 'value', $to_date, "<=");
    $query->propertyCondition('uid', $user['uid']);
    $stmt = $query->execute();
    $report_row['first_calls'] = 0;
    if ($stmt['node']) {
      $report_row['first_calls'] = count(array_keys($stmt['node']));
    }
    */
    $sql = "select count(*) as c ";
    $sql .= "from node grievance_node ";
    $sql .= "join field_data_field_grievance_rep_organizer ";
    $sql .= "  on field_data_field_grievance_rep_organizer.entity_type = 'node' ";
    $sql .= "  and field_data_field_grievance_rep_organizer.entity_id = grievance_node.nid ";
    $sql .= "  and field_grievance_rep_organizer_target_id = :uid ";
    $sql .= "join field_data_field_grievance ";
    $sql .= "  on field_data_field_grievance.entity_type = 'node' ";
    $sql .= "  and field_data_field_grievance.bundle = 'grievance_log' ";
    $sql .= "  and field_grievance_target_id = grievance_node.nid ";
    $sql .= "join field_data_field_grievance_log_type ";
    $sql .= "  on field_data_field_grievance_log_type.entity_type = 'node' ";
    $sql .= "  and field_data_field_grievance_log_type.entity_id = field_data_field_grievance.entity_id ";
    $sql .= "  and field_grievance_log_type_tid in (:log_type_call_tids) ";
    $sql .= "join field_data_field_grievance_status_date ";
    $sql .= "  on field_data_field_grievance_status_date.entity_type = 'node' ";
    $sql .= "  and field_data_field_grievance_status_date.entity_id = field_data_field_grievance.entity_id ";
    $sql .= "  and field_grievance_status_date_value >= :from_date ";
    $sql .= "  and field_grievance_status_date_value <= :to_date ";
    $args = array(
      ':uid' => $user['uid'],
      ':log_type_call_tids' => $log_type_first_call_tid,
      ':from_date' => $from_date,
      ':to_date' => $to_date,
    );
    $stmt = db_query($sql, $args);
    $hr = $stmt->fetchAssoc();
    $report_row['first_calls'] = $hr['c'];

    // Fetch the "Left Message" calls
    /*
    $query = new EntityFieldQuery;
    $query->entityCondition('entity_type', 'node');
    $query->entityCondition('bundle', 'grievance_log');
    $query->fieldCondition('field_grievance_log_type', 'tid', $log_type_call_tids, 'in');
    $query->fieldCondition('field_grievance_log_tags', 'tid', $log_tag_left_message_tid);
    $query->fieldCondition('field_grievance_status_date', 'value', $from_date, ">=");
    $query->fieldCondition('field_grievance_status_date', 'value', $to_date, "<=");
    $query->propertyCondition('uid', $user['uid']);
    $stmt = $query->execute();
    $report_row['left_message'] = 0;
    if ($stmt['node']) {
      $report_row['left_message'] = count(array_keys($stmt['node']));
    }
    */
    $sql = "select count(*) as c ";
    $sql .= "from node grievance_node ";
    $sql .= "join field_data_field_grievance_rep_organizer ";
    $sql .= "  on field_data_field_grievance_rep_organizer.entity_type = 'node' ";
    $sql .= "  and field_data_field_grievance_rep_organizer.entity_id = grievance_node.nid ";
    $sql .= "  and field_grievance_rep_organizer_target_id = :uid ";
    $sql .= "join field_data_field_grievance ";
    $sql .= "  on field_data_field_grievance.entity_type = 'node' ";
    $sql .= "  and field_data_field_grievance.bundle = 'grievance_log' ";
    $sql .= "  and field_grievance_target_id = grievance_node.nid ";
    $sql .= "join field_data_field_grievance_log_type ";
    $sql .= "  on field_data_field_grievance_log_type.entity_type = 'node' ";
    $sql .= "  and field_data_field_grievance_log_type.entity_id = field_data_field_grievance.entity_id ";
    $sql .= "  and field_grievance_log_type_tid in (:log_type_call_tids) ";
    $sql .= "join field_data_field_grievance_status_date ";
    $sql .= "  on field_data_field_grievance_status_date.entity_type = 'node' ";
    $sql .= "  and field_data_field_grievance_status_date.entity_id = field_data_field_grievance.entity_id ";
    $sql .= "  and field_grievance_status_date_value >= :from_date ";
    $sql .= "  and field_grievance_status_date_value <= :to_date ";
    $sql .= "join field_data_field_grievance_log_tags ";
    $sql .= "  on field_data_field_grievance_log_tags.entity_type = 'node' ";
    $sql .= "  and field_data_field_grievance_log_tags.entity_id = field_data_field_grievance.entity_id ";
    $sql .= "  and field_grievance_log_tags_tid = :log_tag_left_message_tid ";
    $args = array(
      ':uid' => $user['uid'],
      ':log_type_call_tids' => $log_type_call_tids,
      ':from_date' => $from_date,
      ':to_date' => $to_date,
      ':log_tag_left_message_tid' => $log_tag_left_message_tid,
    );
    $stmt = db_query($sql, $args);
    $hr = $stmt->fetchAssoc();
    $report_row['left_message'] = $hr['c'];

    // Fetch "New This Week" 
    $query = new EntityFieldQuery;
    $query->entityCondition('entity_type', 'node');
    $query->entityCondition('bundle', 'grievance');
    $query->fieldCondition('field_grievance_rep_organizer', 'target_id', $user['uid']);
    $query->propertyCondition('created', $from_ts, ">=");
    $query->propertyCondition('created', $to_ts, "<=");
    $stmt = $query->execute();
    $report_row['new'] = 0;
    if ($stmt['node']) {
      $report_row['new'] = count(array_keys($stmt['node']));
    }

    $term_log_type_tid = variable_get('grievance_log_term_type', 0);
    $open_status_str = join(",", $open_status_tids);

    // Total terms
    $sql = "select count(distinct node_grievance.nid) as c ";

    $sql .= "from ";
    $sql .= "node node_grievance, ";
    $sql .= "field_data_field_grievance_status, ";
    $sql .= "field_data_field_grievance_rep_organizer, ";
    $sql .= "field_data_field_grievance, ";
    $sql .= "node node_log, ";
    $sql .= "field_data_field_grievance_log_type, ";
    $sql .= "field_data_field_grievance_status_date ";

    $sql .= "where ";

    $sql .= "field_data_field_grievance_status.entity_type = 'node' and ";
    $sql .= "field_data_field_grievance_status.entity_id = node_grievance.nid and ";
    $sql .= "field_data_field_grievance_status.field_grievance_status_tid in ($open_status_str) and ";

    $sql .= "field_data_field_grievance_rep_organizer.entity_type = 'node' and ";
    $sql .= "field_data_field_grievance_rep_organizer.entity_id = node_grievance.nid and ";
    $sql .= "field_data_field_grievance_rep_organizer.field_grievance_rep_organizer_target_id = " . $user['uid'] . " and ";

    $sql .= "field_data_field_grievance.entity_type = 'node' and ";
    $sql .= "field_data_field_grievance.bundle = 'grievance_log' and ";
    $sql .= "field_data_field_grievance.field_grievance_target_id = node_grievance.nid and ";

    $sql .= "node_log.type = 'grievance_log' and ";
    $sql .= "node_log.nid = field_data_field_grievance.entity_id and ";

    $sql .= "field_data_field_grievance_log_type.entity_type = 'node' and ";
    $sql .= "field_data_field_grievance_log_type.entity_id = node_log.nid and ";
    $sql .= "field_data_field_grievance_log_type.field_grievance_log_type_tid = $term_log_type_tid and ";

    $sql .= "field_data_field_grievance_status_date.entity_type = 'node' and ";
    $sql .= "field_data_field_grievance_status_date.entity_id = node_log.nid and ";

    $sql .= "node_grievance.type = 'grievance' ";

    $stmt = db_query($sql, array());
    $hr = $stmt->fetchAssoc();
    $report_row['terms_total'] = $hr['c'];


    // Terms this week
    $sql = "select count(distinct node_grievance.nid) as c ";

    $sql .= "from ";
    $sql .= "node node_grievance, ";
    $sql .= "field_data_field_grievance_status, ";
    $sql .= "field_data_field_grievance_rep_organizer, ";
    $sql .= "field_data_field_grievance, ";
    $sql .= "node node_log, ";
    $sql .= "field_data_field_grievance_log_type, ";
    $sql .= "field_data_field_grievance_status_date ";

    $sql .= "where ";

    $sql .= "field_data_field_grievance_status.entity_type = 'node' and ";
    $sql .= "field_data_field_grievance_status.entity_id = node_grievance.nid and ";
    $sql .= "field_data_field_grievance_status.field_grievance_status_tid in ($open_status_str) and ";

    $sql .= "field_data_field_grievance_rep_organizer.entity_type = 'node' and ";
    $sql .= "field_data_field_grievance_rep_organizer.entity_id = node_grievance.nid and ";
    $sql .= "field_data_field_grievance_rep_organizer.field_grievance_rep_organizer_target_id = " . $user['uid'] . " and ";

    $sql .= "field_data_field_grievance.entity_type = 'node' and ";
    $sql .= "field_data_field_grievance.bundle = 'grievance_log' and ";
    $sql .= "field_data_field_grievance.field_grievance_target_id = node_grievance.nid and ";

    $sql .= "node_log.type = 'grievance_log' and ";
    $sql .= "node_log.nid = field_data_field_grievance.entity_id and ";

    $sql .= "field_data_field_grievance_log_type.entity_type = 'node' and ";
    $sql .= "field_data_field_grievance_log_type.entity_id = node_log.nid and ";
    $sql .= "field_data_field_grievance_log_type.field_grievance_log_type_tid = $term_log_type_tid and ";

    $sql .= "field_data_field_grievance_status_date.entity_type = 'node' and ";
    $sql .= "field_data_field_grievance_status_date.entity_id = node_log.nid and ";
    $sql .= "field_data_field_grievance_status_date.field_grievance_status_date_value >= :from_date and ";
    $sql .= "field_data_field_grievance_status_date.field_grievance_status_date_value <= :to_date and ";

    $sql .= "node_grievance.type = 'grievance' ";

    $stmt = db_query($sql, array(':from_date' => $from_date, ':to_date' => $to_date));
    $hr = $stmt->fetchAssoc();
    $report_row['terms'] = $hr['c'];

    // Fetch Arbitrations
    $query = new EntityFieldQuery;
    $query->entityCondition('entity_type', 'node');
    $query->entityCondition('bundle', 'grievance');
    $query->fieldCondition('field_grievance_status', 'tid', $open_status_tids, 'in');
    $query->fieldCondition('field_grievance_status', 'tid', $arbitration_status_tids, 'in');
    $query->fieldCondition('field_grievance_rep_organizer', 'target_id', $user['uid']);
    $stmt = $query->execute();
    $report_row['arbitration'] = 0;
    if ($stmt['node']) {
      $report_row['arbitration'] = count(array_keys($stmt['node']));
    }

    // Fetch Formal Arb Held
    $query = new EntityFieldQuery;
    $query->entityCondition('entity_type', 'node');
    $query->entityCondition('bundle', 'grievance');
    $query->fieldCondition('field_grievance_status', 'tid', $open_status_tids, 'in');
    $query->fieldCondition('field_grievance_status', 'tid', $formal_arb_held_status_tids, 'in');
    $query->fieldCondition('field_grievance_rep_organizer', 'target_id', $user['uid']);
    $stmt = $query->execute();
    $report_row['formal_arb_held'] = 0;
    if ($stmt['node']) {
      $report_row['formal_arb_held'] = count(array_keys($stmt['node']));
    }

    $report_rows[] = $report_row;
  }

  $headers = array(
    'name' => t('Specialist'),
    'open' => t('Total Open'),
    'new' => t('New'),
    'calls' => t('Total Calls'),
    'first_calls' => t('First Calls'),
    'left_message' => t('Left Message'),
    'terms_total' => t('Total Terms'),
    'terms' => t('Terms'),
    'boa_held' => t('BOA Held'),
    'arb' => t('Total in Arb.'),
    'adr_held' => t('ADR'),
    'exp_arb_held' => t('Exp. Arb.'),
    'fmcs_held' => t('FMCS'),
    'formal_held' => t('Formal Arb'),
    'mediation_held' => t('Mediation'),
    'closed' => t('Closed'),
    'victories' => t('Victories'),
  );

  $totals = array();
  foreach ($report_rows as $report_row) {
    foreach ($headers as $key => $value) {
      if ($key == 'name') {
        $totals[$key] = 'Total';
      } else {
        $totals[$key] += $report_row[$key];
      }
    }
  }

  $html = '';

  $html .= '<style>';
  $html .= '.uh226_report_weekly_summary th { border: 1px solid #333333; text-align: center; padding-left: 5px; margin-left: 0; padding-right: 5px; margin-right: 0; padding-top: 10px; padding-bottom: 10px;}';
  $html .= '.uh226_report_weekly_summary td { border: 1px solid #333333; padding-left: 0; padding-right: 0; margin-left: 0; margin-right: 0; text-align: center; }';
  $html .= '</style>';
  $html .= '<table class="grievance_simple_table uh226_report_weekly_summary">';

  $html .= '<thead>';
  $html .= '<tr>';
  $html .= '<th rowspan="2">' . $headers['name'] . '</th>';
  $html .= '<th rowspan="2">' . $headers['open'] . '</th>';
  $html .= '<th colspan="4">This Week</th>';
  $html .= '<th rowspan="2">' . $headers['terms_total'] . '</th>';
  $html .= '<th colspan="2">This Week</th>';
  $html .= '<th rowspan="2">' . $headers['arb'] . '</th>';
  $html .= '<th colspan="5">Held This Week</th>';
  $html .= '<th colspan="2">This Week</th>';
  $html .= '</tr>';

  $html .= '<tr>';
  foreach ($headers as $key => $value) {

  	if ($key == 'open') { continue; }
  	if ($key == 'name') { continue; }
  	if ($key == 'terms_total') { continue; }
  	if ($key == 'arb') { continue; }
    $html .= '<th>' . $value . '</th>';
  }
  $html .= '</tr>';
  $html .= '</thead>';

  $html .= '<tbody>';
  foreach ($report_rows as $report_row) {
    $html .= '<tr>';
    foreach ($headers as $key => $value) {
      $html .= '<td>' . htmlspecialchars($report_row[$key]) . '</td>';
    }
    $html .= '</tr>';
  }
  $html .= '</tr>';
  $html .= '</tbody>';

  $html .= '<tfoot>';
  $html .= '<tr>';
  foreach ($headers as $key => $value) {
    $html .= '<th>' . htmlspecialchars($totals[$key]) . '</th>';
  }
  $html .= '</tr>';
  $html .= '</tfoot>';

  $html .= '</table>';

  $form_state['results'] = $html;
}
