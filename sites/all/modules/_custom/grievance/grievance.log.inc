<?php

function grievance_node_tab_log($node) {
  drupal_set_title($node->title . ': ' . t('Log'));
  $entity_nid = $node->nid;

  $headfoot = grievance_node_tab_get_addlink("grievance-log", array("grievance" => $entity_nid), 'Add a new log entry');
  if ($node->field_grievance_phone['und'][0]['value'] || $node->field_grievance_email['und'][0]['email']) {
    $headfoot .= t('(Grievant: ');
    if ($node->field_grievance_phone['und'][0]['value']) {
      $headfoot .= '<strong>' . $node->field_grievance_phone['und'][0]['value'] . '</strong>';
    }
    if ($node->field_grievance_phone['und'][0]['value'] && $node->field_grievance_email['und'][0]['email']) {
      $headfoot .= ' / ';
    }
    if ($node->field_grievance_email['und'][0]['email']) {
      $headfoot .= '<strong>' . $node->field_grievance_email['und'][0]['email'] . '</strong>';
    }
    $headfoot .= ')';
  }

  $result = '';
  $result .= $headfoot;
  $result .= '<span class="printonly"><br>' . t('Grievance ID: ') . $node->field_grievance_id['und'][0]['value'] . '</span>';
  $result .= '<br>';
  $result .= views_embed_view('grievance_log', 'block', $entity_nid);
  $result .= $headfoot;

  return $result;
}

function grievance_log_update($grievance_node, $op) {
	global $user;

  // 
  // Log the update / insert event
  //

  $log_node = new stdClass();
  if ($op == 'insert') {
    $log_node->title = t('Grievance Record Created');
    $log_type_tid = variable_get('grievance_log_insert_type', '');
  } else {
    if ($grievance_node->grievance_status_changed) {
      $log_node->title = t('Grievance Record Updated with status change');
      $log_type_tid = variable_get('grievance_log_update_type_with_status_change', '');
    } else {
      $log_node->title = t('Grievance Record Updated');
      $log_type_tid = variable_get('grievance_log_update_type_without_status_change', '');
    }
  }
  $log_node->type = 'grievance_log';
  node_object_prepare($log_node);
  $log_node->language = LANGUAGE_NONE;
  $log_node->uid = $user->uid; 
  $log_node->status = 1;
  $log_node->promote = 0;
  $log_node->comment = 0;

  $log_node->field_grievance['und'][0]['target_id'] = $grievance_node->nid;
  $log_node->field_grievance_status = $grievance_node->field_grievance_status;
  $log_node->field_grievance_alert = $grievance_node->field_grievance_alert;
  $log_node->field_grievance_alert_tid = $grievance_node->field_grievance_alert_tid;
  $log_node->field_grievance_alert_date = $grievance_node->field_grievance_alert_date;
  $log_node->field_grievance_log_type['und'][0]['tid'] = $log_type_tid;

  $log_node->field_grievance_status_date = $grievance_node->field_grievance_status_date;
  if (!$log_node->field_grievance_status_date['und'][0]['value']) {
    $log_node->field_grievance_status_date['und'][0]['value'] = date('Y-m-d');
  }

  $log_node = node_submit($log_node); // Prepare node for saving
  node_save($log_node);

  //
  // Log the event date, which starts the timeline.  But only if we don't already have one.
  //

  $incident_type_tid = variable_get('grievance_log_incident_type', '');
  $probe = grievance_log_list($grievance_node, array($incident_type_tid));
  if (!$probe) {
    $event_date = $grievance_node->field_grievance_date_2['und'][0]['value'];
    if (!$event_date) {
      $event_date = $grievance_node->field_grievance_date_1['und'][0]['value'];
    }
    if (!$event_date) {
      $event_date = date('Y-m-d 00:00:00', strtotime('-15 days'));
    }

    if ($event_date) {
      $ts = strtotime($event_date);
      $log_node = new stdClass();
      $log_node->title = t('Event date, timeline starts');
      $log_node->type = 'grievance_log';
      node_object_prepare($log_node);
      $log_node->language = LANGUAGE_NONE;
      $log_node->uid = $user->uid; 
      $log_node->status = 1;
      $log_node->promote = 0;
      $log_node->comment = 0;

      $log_node->field_grievance['und'][0]['target_id'] = $grievance_node->nid;
      $log_node->field_grievance_status['und'][0]['tid'] =  variable_get('grievance_log_incident_status', '');
      $log_node->field_grievance_log_type['und'][0]['tid'] = variable_get('grievance_log_incident_type', '');

      $log_node->field_grievance_status_date['und'][0]['value'] = date('Y-m-d H:i:00', strtotime($event_date));

      $log_node = node_submit($log_node); // Prepare node for saving
      node_save($log_node);
    }
  }

  //
  // Log the assignment
  //

  if ($grievance_node->grievance_assignment_changed) {
    $log_node = new stdClass();
    $log_node->field_grievance['und'][0]['target_id'] = $grievance_node->nid;
    $assignee_uid = $grievance_node->field_grievance_rep_assignee['und'][0]['target_id'];
    if ($assignee_uid) {
      $assignee_user = user_load($assignee_uid);
      $title = t('Grievance assigned to: ') . $assignee_user->name;
    } else {
      $title = t('Grievance Unassigned');
    }
    $log_node->title = $title;
    $log_node->type = 'grievance_log';
    node_object_prepare($log_node);
    $log_node->language = LANGUAGE_NONE;
    $log_node->uid = $user->uid; 
    $log_node->status = 1;
    $log_node->promote = 0;
    $log_node->comment = 0;
    $log_node->body['und'][0]['value'] = $grievance_node->field_grievance_assignee_notes['und'][0]['value'];
    $log_node->field_grievance_log_type['und'][0]['tid'] = variable_get('grievance_log_assignment_type', '');
    $log_node->field_grievance_status_date['und'][0]['value'] = date('Y-m-d H:i:s');

    $log_node = node_submit($log_node); // Prepare node for saving
    node_save($log_node);
  }

  //
  // Log any intake dates
  //

  if ($op == 'insert') {
    for ($i=1; $i<=5; ++$i) {
      $log_type_tid = variable_get("grievance_log_intake_${i}_type", '');
      if (!$log_type_tid) { continue; }
      $log_type_term = taxonomy_term_load($log_type_tid);
      $date = $grievance_node->{"grievance_intake_log_${i}"};
      if (!$date) { continue; }

      $log_node = new stdClass();
      $log_node->field_grievance['und'][0]['target_id'] = $grievance_node->nid;
      $log_node->title = $log_type_term->name;
      $log_node->type = 'grievance_log';
      node_object_prepare($log_node);
      $log_node->language = LANGUAGE_NONE;
      $log_node->uid = $user->uid; 
      $log_node->status = 1;
      $log_node->promote = 0;
      $log_node->comment = 0;
      $log_node->field_grievance_log_type['und'][0]['tid'] = $log_type_tid;
      $log_node->field_grievance_status_date['und'][0]['value'] = $date;
      $log_node = node_submit($log_node); // Prepare node for saving
      node_save($log_node);
    }
  }
}

function grievance_log_notify_check($grievance_node) {
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_log');
  $query->fieldCondition('field_grievance_log_type', 'tid', variable_get('grievance_log_notify_type', ''));
  $query->fieldCondition('field_grievance', 'target_id', $grievance_node->nid);
  $stmt = $query->execute();

  if (!$stmt['node']) { return FALSE; }

  $log_nids = array_keys($stmt['node']);
  $log_nodes = node_load_multiple($log_nids);

  foreach ($log_nodes as $log_node) {
    if ($log_node->field_grievance_status['und'][0]['tid'] == $grievance_node->field_grievance_status['und'][0]['tid']) {
      return TRUE; 
    }
  }

  return FALSE;
}

function grievance_log_notify(
  $grievance_node, 
  $from, 
  $to, 
  $cc,
  $subject, 
  $body, 
  $attachments
) {
  global $user;

  $log_node = new stdClass();
  $log_node->title = t('Notification sent to ') . $to;
  $log_node->type = 'grievance_log';
  node_object_prepare($log_node);
  $log_node->language = LANGUAGE_NONE;
  $log_node->uid = $user->uid; 
  $log_node->status = 1;
  $log_node->promote = 0;
  $log_node->comment = 0;

  $log_node->field_grievance['und'][0]['target_id'] = $grievance_node->nid;
  $log_node->field_grievance_status = $grievance_node->field_grievance_status;
  $log_node->field_grievance_alert = $grievance_node->field_grievance_alert;
  $log_node->field_grievance_alert_date = $grievance_node->field_grievance_alert_date;
  $log_node->field_grievance_log_type['und'][0]['tid'] = variable_get('grievance_log_notify_type', '');
  $log_node->field_grievance_status_date['und'][0]['value'] = date('Y-m-d'); 

  $comments = t("COMPANY NOTIFICATION SENT\n");
  $comments .= t('From: ') . $from . "\n";
  $comments .= t('To: ') . $to . "\n";
  if ($cc) {
    $comments .= t('Cc: ') . $cc . "\n";
  }
  $comments .= t('Subject: ') . $subject . "\n\n";
  $comments .= $body;

  $log_node->body['und'][0]['value'] = $comments;

  if ($attachments) {
    foreach ($attachments as $attachment) {
      $file = file_save_data(
        $attachment['filecontent'], 
        'private://grievance_attachments/' . $attachment['filename'], 
        FILE_EXISTS_RENAME
      );

      $file->display = 1;
      $log_node->field_grievance_attachments["und"][0] = (array) $file;
    }
  }

  $log_node = node_submit($log_node); // Prepare node for saving
  node_save($log_node);
}

function grievance_log_member_forward(
  $grievance_node, 
  $to, 
  $subject, 
  $body, 
  $attachments
) {
  global $user;

  $log_node = new stdClass();
  $log_node->title = t('Member notification sent to: ') . htmlspecialchars($to);
  $log_node->type = 'grievance_log';
  node_object_prepare($log_node);
  $log_node->language = LANGUAGE_NONE;
  $log_node->uid = $user->uid; 
  $log_node->status = 1;
  $log_node->promote = 0;
  $log_node->comment = 0;

  $log_node->field_grievance['und'][0]['target_id'] = $grievance_node->nid;
  $log_node->field_grievance_status = $grievance_node->field_grievance_status;
  $log_node->field_grievance_alert = $grievance_node->field_grievance_alert;
  $log_node->field_grievance_alert_date = $grievance_node->field_grievance_alert_date;
  $log_node->field_grievance_log_type['und'][0]['tid'] = variable_get('grievance_log_member_forward_type', '');
  $log_node->field_grievance_status_date['und'][0]['value'] = date('Y-m-d'); 

  $comments = t("MEMBER NOTIFICATION SENT\n");
  $comments .= t('To: ') . $to . "\n";
  $comments .= t('Subject: ') . $subject . "\n\n";
  $comments .= $body;

  $log_node->body['und'][0]['value'] = $comments;

  if ($attachments) {
    foreach ($attachments as $attachment) {
      $file = file_save_data(
        $attachment['filecontent'], 
        'private://grievance_attachments/' . $attachment['filename'], 
        FILE_EXISTS_RENAME
      );

      $file->display = 1;
      $log_node->field_grievance_attachments["und"][0] = (array) $file;
    }
  }

  $log_node = node_submit($log_node); // Prepare node for saving
  node_save($log_node);
}

function grievance_log_list($grievance_nid, $type_tids) {
  if (is_object($grievance_nid)) {
    $grievance_nid = $grievance_nid->nid;
  }

  if (!is_array($log_tids)) { $log_tids = array($log_tids); }
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_log');
  $query->fieldCondition('field_grievance_log_type', 'tid', $type_tids, 'in');
  $query->fieldCondition('field_grievance', 'target_id', $grievance_nid);
  $query->fieldOrderBy('field_grievance_status_date', 'value');
  $stmt = $query->execute();

  if (!$stmt['node']) { return array(); }

  $log_nids = array_keys($stmt['node']);
  $log_nodes = node_load_multiple($log_nids);
  return $log_nodes;
}

function grievance_log_status_date($grievance_nid, $status_tid, $direction = 'asc') {
  $log_update_tids = array(
    variable_get('grievance_log_insert_type', ''), 
    variable_get('grievance_log_update_type_with_status_change', ''), 
    variable_get('grievance_log_update_type_without_status_change', ''), 
    variable_get('grievance_log_incident_type', '')
  );
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_log');
  $query->fieldCondition('field_grievance_log_type', 'tid', $log_update_tids, 'in');
  $query->fieldCondition('field_grievance', 'target_id', $grievance_nid);
  if (is_array($status_tid)) {
    $query->fieldCondition('field_grievance_status', 'tid', $status_tid, 'in');
  } else {
    $query->fieldCondition('field_grievance_status', 'tid', $status_tid);
  }
  $query->fieldOrderBy('field_grievance_status_date', 'value', $direction);
  $stmt = $query->execute();
  if ($stmt['node']) {
    $tmp = array_keys($stmt['node']);
    $log_nid = array_shift($tmp);
    $log_node = node_load($log_nid);
    return strtotime($log_node->field_grievance_status_date['und'][0]['value']);
  }

  return NULL;
}

function grievance_log_status_entry($grievance_nid, $status_tid, $direction = 'asc') {
  $log_update_tids = array(
    variable_get('grievance_log_insert_type', ''), 
    variable_get('grievance_log_update_type_with_status_change', ''), 
    variable_get('grievance_log_update_type_without_status_change', ''), 
    variable_get('grievance_log_incident_type', '')
  );
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_log');
  $query->fieldCondition('field_grievance_log_type', 'tid', $log_update_tids, 'in');
  $query->fieldCondition('field_grievance', 'target_id', $grievance_nid);
  if (is_array($status_tid)) {
    $query->fieldCondition('field_grievance_status', 'tid', $status_tid, 'in');
  } else {
    $query->fieldCondition('field_grievance_status', 'tid', $status_tid);
  }
  $query->fieldOrderBy('field_grievance_status_date', 'value', $direction);
  $stmt = $query->execute();
  if ($stmt['node']) {
    $tmp = array_keys($stmt['node']);
    $log_nid = array_shift($tmp);
    $log_node = node_load($log_nid);
    return $log_node;
  }

  return NULL;
}


function grievance_log_entry_date($grievance_nid, $log_type_tid) {
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_log');
  if (is_array($log_type_tid)) {
    $query->fieldCondition('field_grievance_log_type', 'tid', $log_type_tid, 'in');
  } else {
    $query->fieldCondition('field_grievance_log_type', 'tid', $log_type_tid);
  }
  $query->fieldCondition('field_grievance', 'target_id', $grievance_nid);
  $query->fieldOrderBy('field_grievance_status_date', 'value');
  $stmt = $query->execute();
  if ($stmt['node']) {
    $tmp = array_keys($stmt['node']);
    $log_nid = array_shift($tmp);
    $log_node = node_load($log_nid);
    return strtotime($log_node->field_grievance_status_date['und'][0]['value']);
  }

  return NULL;
}


function grievance_node_tab_timeline($grievance_node) {
  drupal_set_title($grievance_node->title . ': ' . t('Timeline'));

  $timeline = grievance_contract_compute_timeline($grievance_node);
  if (!$timeline) { $timeline = array(); }
  if (!$timeline['rule_nodes']) { $timeline['rule_nodes'] = array(); }

  // Theme our output
  $header = array(
    t('Status'),
    t('Actor'),
    t('Action'),
    t('Next Status'),
    t('Date Entered'),
    t('Deadline'),
    t('Date Completed'),
    t('State'),
  );
  $attributes = array('class' => array('grievance-timeline'));;
  $rows = array();

  foreach ($timeline['rule_nodes'] as $rule_node) {
    $rule_nid = $rule_node->nid;
    $alert_tid = $rule_node->field_grievance_alert_tid['und'][0]['tid'];
    $alert_term = $timeline['alert_terms'][$alert_tid];

    if ($timeline[$rule_nid]['entries']) {

      foreach ($timeline[$rule_nid]['entries'] as $entry) {

        $row = array();

        $state = '';
        $state_class = '';
        $is_overdue = FALSE;
        if ($entry['entered_on']) {
          if ($entry['left_on']) { 
            if ($entry['deadline'] && ($entry['left_on'] > $entry['deadline'])) {
              $state = t('Complete (late)');
              $state_class = 'overdue';
            } else {
              $state = t('Complete');
              $state_class = 'complete';
            }
          } else {
            $state = t('Pending');
            $state_class = 'pending';
          }
        }

        $row[] = $timeline['status_terms'][$entry['entered_because']]->name;
        $row[] = $alert_term->field_grievance_actor['und'][0]['value'];
        $row[] = $alert_term->name;

        if ($entry['left_because']) {
          $row[] = $timeline['status_terms'][$entry['left_because']]->name;
        } else {
          $row[] = $timeline[$rule_nid]['next_status_display'];
        }

        if ($entry['entered_on']) {
          $row[] = date('m/d/Y', $entry['entered_on']);
        } else {
          $row[] = '';
        }

        if ($entry['deadline']) {
          $row[] = date('m/d/Y', $entry['deadline']);
        } else {
          $row[] = '';
        }


        if ($entry['left_on']) {
          $row[] = date('m/d/Y', $entry['left_on']);
        } else {
          $row[] = '';
        }

        $row[] = $state;

        $rows[] = array('data' => $row, 'class' => array($state_class));

      }

    } else {

      $row = array();

      $state_class = '';

      $row[] = $timeline[$rule_nid]['from_status_display'];
      $row[] = $alert_term->field_grievance_actor['und'][0]['value'];
      $row[] = $alert_term->name;
      $row[] = $timeline[$rule_nid]['next_status_display'];
      $row[] = '';
      $row[] = '';
      $row[] = '';
      $row[] = '';

      $rows[] = array('data' => $row, 'class' => array($state_class));

    }

    /*
    foreach ($from_status_tids as $from_status_tid) {
      // @TODO need the earliest here.
      if ($dates_entered[$from_status_tid]) {
        $date_entered = date('m/d/Y', $dates_entered[$from_status_tid]);
        $deadline = date('m/d/Y', strtotime(grievance_contract_next_deadline($contract_tplt_nid, $from_status_tid, $dates_entered[$from_status_tid])));
      }
      if ($dates_left[$status_tid]) {
        $date_left = date('m/d/Y', $dates_left[$status_tid]);
      }
    }



    $data = array();

    $from_status_display = '';
    foreach ($from_status_terms as $from_status_term) {
      if ($from_status_display) { $from_status_display .= ', '; }
      $from_status_display .= $from_status_term->name;
    }

    $next_status_display = '';
    foreach ($next_status_terms as $next_status_term) {
      if ($next_status_display) { $next_status_display .= ', '; }
      $next_status_display .= $next_status_term->name;
    }

    $data[] = $from_status_display;
    $data[] = $alert_term->field_grievance_actor['und'][0]['value'];
    $data[] = $alert_term->name;
    $data[] = $next_status_display;
    $data[] = $date_entered;
    $data[] = $deadline;
    $data[] = $date_left;
    $data[] = $state;

    $rows[] = array('data' => $data, 'class' => array($state_class));
    */
  }

  return theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => $attributes));
}

function grievance_log_views_exposed_form_alter($form, &$form_state) {
  if (isset($form['field_grievance_log_type_tid']['#options'])) {

    $skip_tids = array();
    foreach (array('grievance_log_update_type_without_status_change', 'grievance_log_phaxio_callback_type') as $skip_variable_name) {
      $skip_tids[] = variable_get($skip_variable_name, '');
    }

    $default = array();
    foreach (array_keys($form['field_grievance_log_type_tid']['#options']) as $tid) {
      $found = FALSE;
      foreach ($skip_tids as $skip_tid) {
        if ($tid == $skip_tid) { $found = TRUE; }
      }
      if (!$found) {
        $default[$tid] = $tid;
      }
    }

    // This is what we want to do, but it doesn't work with AHAH checkboxes grr.
    // $form['field_grievance_log_type_tid']['#default_value'] = $default;

    // So this is the garbage that we have to do.
    if (!$form_state['input']['field_grievance_log_type_tid']) {
      $form_state['input']['field_grievance_log_type_tid'] = $default;
    }
  }
}


function grievance_log_node_form_alter(&$form, &$form_state, $form_id) {
  $form['field_grievance_alert']['#access'] = FALSE;
  $form['field_grievance_alert_tid']['#access'] = FALSE;
  $form['field_grievance_alert_date']['#access'] = FALSE;
  $form['field_grievance_external_id']['#access'] = FALSE;

  // Can change the status for update types
  $form['field_grievance_status']['#access'] = FALSE;
  if (isset($form['#node']) && isset($form['#node']->field_grievance_log_type['und'][0]['tid'])) {
    if (
      $form['#node']->field_grievance_log_type['und'][0]['tid'] == variable_get('grievance_log_update_type_with_status_change', '') ||
      $form['#node']->field_grievance_log_type['und'][0]['tid'] == variable_get('grievance_log_update_type_without_status_change', '')
    ) {
      $form['field_grievance_status']['#access'] = TRUE;
    }
  }

  // Restrict dropdown options for log type
  $form['field_grievance_log_type']['und']['#options'] = grievance_log_types_i_can_edit();

  // Make the "days" field conditional
  $timelines_extended_tid = variable_get('grievance_log_timelines_extended_type', '');
  if (!$timelines_extended_tid) {
    $form['field_grievance_days']['#access'] = FALSE;
  } else {
    $form['field_grievance_days']['#states'] = array(
      'visible' => array(
        ':input[name="field_grievance_log_type[und]"]' => array('value' => $timelines_extended_tid),
      ),
    );
  }
}

function grievance_log_intake_admin() {
  $form = array();

  $options = grievance_log_types_i_can_edit();

  for ($i=1; $i<=5; ++$i) {
    $form["grievance_log_intake_${i}_type"] = array(
      '#type' => 'select',
      '#options' => $options,
      '#title' => t("Intake Log Type $i"),
      '#default_value' => variable_get("grievance_log_intake_${i}_type", ''),
    );
  }

  return system_settings_form($form);
}

function grievance_node_access_grievance_log($node, $op, $account) {
  if ($op == 'view') { return NODE_ACCESS_IGNORE; }
  $log_type_tid = $node->field_grievance_log_type['und'][0]['tid'];
  if (!$log_type_tid) { return NODE_ACCESS_IGNORE; }
  $log_type_term = taxonomy_term_load($log_type_tid);
  if (!$log_type_term) { return NODE_ACCESS_IGNORE; }
  $types = grievance_log_types_i_can_edit($account);
  if (!$types[$log_type_tid]) { return NODE_ACCESS_DENY; }

  return NODE_ACCESS_IGNORE;
}

function grievance_log_types_i_can_edit($account = NULL) {
  // Get the account
  if ($account) {
    $uid = $account->uid;
  } else  {
    global $user;
    $uid = $user->uid;
  }
  if (!$uid) { return NULL; }

  // Use drupal_static to only compute once per user
  $result = &drupal_static(__FUNCTION__);
  if (isset($result) && isset($result[$uid])) { return $result[$uid]; }

  // Ok, do our computations
  if (!$result) { $result = array(); }
  $user_full = user_load($uid);

  // Generate the full set of options initially
  $options = array(NULL => t('- Select -'));
  $terms = taxonomy_allowed_values(field_info_field('field_grievance_log_type'));
  foreach ($terms as $tid => $term_name) {
    // Load the term
    $term = taxonomy_term_load($tid);

    // No roles means all roles
    if (!$term->field_grievance_roles['und'][0]['value']) { 
      $options[$tid] = $term_name; 
      continue;
    }

    // Check to see if we have any of the roles
    $found = FALSE;
    foreach ($term->field_grievance_roles['und'] as $tmp) {
      $rid = $tmp['value'];
      if ($user_full->roles[$rid]) {
        $found = TRUE; 
        break;
      }
    }

    // We do!
    if ($found) { 
      $options[$tid] = $term_name; 
    }
  }

  $result[$uid] = $options;
  return $result[$uid];
}

