<?php

/**
 * Category - e.g. "email", "sms", "update"
 * Type - e.g. "employer_notification", "worker_notification", "dispatch_change"
 * Message - e.g. "Notification sent"
 * Summary - e.g. the body of the email, and the recipients
 * Notes - e.g. the SMTP traffic
 * Handler - The NID to which this log entry is attached
 */

function sirius_log(
  $category, 
  $type, 
  $message, 
  $summary, 
  $notes, 
  $handler_nids, 
  $files = NULL,
  $json = NULL) {
  global $user;
  $log_node = new stdClass();
  $log_node->type = 'sirius_log';
  $log_node->no_recurse = TRUE;
  node_object_prepare($log_node);
  $log_node->language = LANGUAGE_NONE;
  $log_node->uid = $user->uid;
  $log_node->status = 1;
  $log_node->promote = 0;
  $log_node->comment = 0;
  $log_node->title = substr(date('c') . " - " . $category . " - " . $type . " - " . $message, 0, 254);
  $log_node->field_sirius_category['und'][0]['value'] = $category;
  $log_node->field_sirius_type['und'][0]['value'] = $type;
  $log_node->field_sirius_message['und'][0]['value'] = substr($message, 0, 254);
  $log_node->field_sirius_summary['und'][0]['value'] = $summary;
  $log_node->field_sirius_notes['und'][0]['value'] = $notes;
  if ($files) {
    $log_node->field_sirius_attachments['und'] = $files;
  }
  if ($json) {
    $log_node->field_sirius_json['und'][0]['value'] = $json;
  }

  if (is_array($handler_nids)) {
    foreach ($handler_nids as $handler_nid) {
      if ($handler_nid) {
        $log_node->field_sirius_log_handler['und'][]['target_id'] = $handler_nid;
      }
    }
  } else if ($handler_nids) {
    $log_node->field_sirius_log_handler['und'][0]['target_id'] = $handler_nids;
  }
  $log_node = node_submit($log_node);
  node_object_prepare($log_node);
  node_save($log_node);
}

function sirius_node_access_log($log_node, $op, $account) {
  // Check admin permissions
  if ($op == 'view' && user_access('sirius log view', $account)) { return NODE_ACCESS_ALLOW; }
  if (user_access('sirius log edit', $account)) { return NODE_ACCESS_ALLOW; }

  // @todo: The logic below should be delegated to modules.  But we're hardcoding it here for the time being.

  // Get the category
  $category = $log_node->field_sirius_category['und'][0]['value'];

  // Workers can see their own dispatch:dnc nodes
  if ($category == 'dispatch:dnc' || $category == 'worker:member_status' || $category == 'worker:skill') {
    $worker_node = sirius_worker_get($account->uid);
    if ($log_node->field_sirius_log_handler['und'][0]['target_id'] == $worker_node->nid) {
      return NODE_ACCESS_ALLOW;
    }
  }
  
  // Nobody else can see any log entries
  return NODE_ACCESS_DENY;
}

function sirius_log_render_form($param1, $param2 = NULL) {
  $form = array();

  $form['log_wrapper'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => 'Change Log',
  );

  $form['log_wrapper']['log'] = array(
    '#type' => 'markup',
    '#markup' => views_embed_view('sirius_log', 'block', $param1, $param2),
  );

  return $form;
}