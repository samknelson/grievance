<?php

function grievance_node_tab_log($node) {
  drupal_set_title($node->title . ': ' . t('Log'));
  $entity_nid = $node->nid;

  $result = '';
  $result .= grievance_node_tab_get_addlink("grievance-log", array("grievance" => $entity_nid), 'Add a new log entry');
  $result .= '<br>';
  $result .= views_embed_view('grievance_log', 'block', $entity_nid);
  $result .= grievance_node_tab_get_addlink("grievance-log", array("grievance" => $entity_nid), 'Add a new log entry');

  return $result;
}

function grievance_log_update($grievance_node, $op) {
	global $user;

  if ($op == 'insert') {

    $grievance_date = $grievance_node->field_grievance_date_1['und'][0]['value'];

    if ($grievance_date) {
      $ts = strtotime($grievance_date);
      $log_node = new stdClass();
      $log_node->title = t('Incident Occurred');
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

      $log_node->field_grievance_status_date = $grievance_node->field_grievance_date_1;

      $log_node = node_submit($log_node); // Prepare node for saving
      node_save($log_node);
    }
  }

  $log_node = new stdClass();
  if ($op == 'insert') {
    $log_node->title = t('Grievance Record Created');
  } else {
    $log_node->title = t('Grievance Record Updated');
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
  $log_node->field_grievance_log_type['und'][0]['tid'] = variable_get('grievance_log_update_type', '');

  $log_node->field_grievance_status_date = $grievance_node->field_grievance_status_date;
  if (!$log_node->field_grievance_status_date['und'][0]['value']) {
    $log_node->field_grievance_status_date['und'][0]['value'] = date('Y-m-d');
  }

  $log_node = node_submit($log_node); // Prepare node for saving
  node_save($log_node);
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

function grievance_log_list($grievance_node, $type_tids) {
  if (!is_array($log_tids)) { $log_tids = array($log_tids); }
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_log');
  $query->fieldCondition('field_grievance_log_type', 'tid', $type_tids, 'in');
  $query->fieldCondition('field_grievance', 'target_id', $grievance_node->nid);
  $query->fieldOrderBy('field_grievance_status_date', 'value');
  $stmt = $query->execute();

  if (!$stmt['node']) { return array(); }

  $log_nids = array_keys($stmt['node']);
  $log_nodes = node_load_multiple($log_nids);
  return $log_nodes;
}

function grievance_node_tab_timeline($grievance_node) {
  drupal_set_title($grievance_node->title . ': ' . t('Timeline'));

  // Get the contract template
  $contract_tplt_nid = $grievance_node->field_grievance_contract_tplt['und'][0]['target_id'];
  if (!$contract_tplt_nid) {
    drupal_set_message(t('This grievance does not have a contract configured.'), 'warning');
    return '';
  }

  // Get all the contract rules
  $rule_nodes = grievance_contract_list_rules($contract_tplt_nid);
  if (!$rule_nodes) {
    drupal_set_message(t('This contract has no rules configured.'), 'warning');
    return '';
  }

  // Get the relevant statuses and alerts
  $status_tids = array();
  $alert_tids = array();
  foreach ($rule_nodes as $rule_node) {
    $status_tids[] = $rule_node->field_grievance_status['und'][0]['tid'];
    $status_tids[] = $rule_node->field_grievance_next_status['und'][0]['tid'];
    $alert_tids[] = $rule_node->field_grievance_alert_tid['und'][0]['tid'];
  }
  $status_terms = taxonomy_term_load_multiple($status_tids);
  $alert_terms = taxonomy_term_load_multiple($alert_tids);

  // Get the timeline
  $log_update_tids = array(variable_get('grievance_log_update_type', ''), variable_get('grievance_log_incident_type', ''));
  $log_nodes = grievance_log_list($grievance_node, $log_update_tids);
  $dates_entered = array();
  $dates_left = array();
  $prev_status_tid = 0;
  foreach ($log_nodes as $log_node) {
    $status_tid = $log_node->field_grievance_status['und'][0]['tid'];
    if ($dates_entered[$status_tid]) { continue; }
    $dates_entered[$status_tid] = strtotime($log_node->field_grievance_status_date['und'][0]['value']);

    if ($status_tid != $prev_status_tid) {
      if (!$dates_left[$prev_status_tid]) {
        $dates_left[$prev_status_tid] = strtotime($log_node->field_grievance_status_date['und'][0]['value']);
      }
      $prev_status_tid = $status_tid;
    }
  }

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
  foreach ($rule_nodes as $rule_node) {
    $status_tid = $rule_node->field_grievance_status['und'][0]['tid'];
    $status_term = $status_terms[$status_tid];

    $next_status_tid = $rule_node->field_grievance_next_status['und'][0]['tid'];
    $next_status_term = $status_terms[$next_status_tid];

    $alert_tid = $rule_node->field_grievance_alert_tid['und'][0]['tid'];
    $alert_term = $alert_terms[$alert_tid];

    $date_entered = '';
    $deadline = '';
    if ($dates_entered[$status_tid]) {
      $date_entered = date('m/d/Y', $dates_entered[$status_tid]);
      $deadline = date('m/d/Y', strtotime(grievance_contract_next_deadline($contract_tplt_nid, $status_tid, $dates_entered[$status_tid])));
    }

    $date_left = '';
    if ($dates_left[$status_tid]) {
      $date_left = date('m/d/Y', $dates_left[$status_tid]);
    }

    $state = '';
    $state_class = '';
    $is_overdue = FALSE;
    if ($date_entered) {
      if ($date_left) { 
        if ($deadline && (strtotime($date_left) > strtotime($deadline))) {
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


    $data = array();

    $data[] = $status_term->name;
    $data[] = $alert_term->field_grievance_actor['und'][0]['value'];
    $data[] = $alert_term->name;
    $data[] = $next_status_term->name;
    $data[] = $date_entered;
    $data[] = $deadline;
    $data[] = $date_left;
    $data[] = $state;

    $rows[] = array('data' => $data, 'class' => array($state_class));
  }

  return theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => $attributes));

}
