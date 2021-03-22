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

  $real_uid = $_SESSION['masquerading'];
  if ($real_uid) {
    $real_user = user_load($real_uid);
    $message .= ' [MASQ FROM ' . $real_user->name . ']';
    $json_to_add = array(
      'real_user_name' => $real_user->name,
      'real_user_uid' => $real_user->uid,
      'real_user_mail' => $real_user->mail,
    );

    if (is_object($json)) {
      $json->masquerade = $json_to_add;
    } else if (is_array($json)) {
      $json['masquerade'] = $json_to_add;
    } else {
      $json = json_decode($json, TRUE);
      $json['masquerade'] = $json_to_add;
    }
  }

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

  $domain_nid = sirius_domain_nid();
  if ($domain_nid) { $log_node->field_sirius_domain['und'][0]['target_id'] = $domain_nid; } 

  if ($files) {
    $log_node->field_sirius_attachments['und'] = $files;
  }
  if ($json) {
    if (is_array($json) || is_object($json)) {
      $json = json_encode($json);
    }
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

  return $log_node;
}

function sirius_node_access_log($log_node, $op, $account) {
  // Admin can do anything
  if (user_access('sirius admin')) { return NODE_ACCESS_IGNORE; }

  // Get the category
  $category = $log_node->field_sirius_category['und'][0]['value'];

  // @todo: The logic below should be delegated to modules.  But we're hardcoding it here for the time being.

  // "material"-type log entries can never be  seen or edited directly.
  if ($category == 'material') { return NODE_ACCESS_DENY; }

  // Check admin permissions
  if ($op == 'view' && user_access('sirius log view', $account)) { return NODE_ACCESS_ALLOW; }
  if (user_access('sirius log edit', $account)) { return NODE_ACCESS_ALLOW; }

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
    // '#collapsible' => TRUE,
    // '#collapsed' => TRUE,
    '#title' => '',
  );

  $form['log_wrapper']['log'] = array(
    '#type' => 'markup',
    '#markup' => views_embed_view('sirius_log', 'block', $param1, $param2),
  );

  return $form;
}

function sirius_log_views_exposed_form_alter(&$form, &$form_state) {
  // We decided not to do this, beause it might be HIPAA triggeing.
  /*
  if (isset($form_state['view']) && $form_state['view']->name == 'sirius_provider_worker_lookup') {
    $form['#submit'][] = 'sirius_log_views_exposed_form_track';
  }
  */
}

function sirius_log_views_exposed_form_track($form, &$form_state) {
  $msg = '';
  if ($form_state['values']) {
    foreach ($form_state['values'] as $key => $value) {
      if (!preg_match('/^field_/', $key)) { continue; }
      if (!$value) { continue; }
      if (is_object($value) || is_array($value)) { continue; }

      $title = $key;
      $title = preg_replace('/field_/', '', $title);
      if ($msg) { $msg .= ', '; }
      $msg .= "$title=$value";
    }
  }

  sirius_log(
    'views_exposed_form', 
    $form_state['view']->name, 
    $msg, 
    NULL, 
    NULL, 
    array(), 
    NULL,
    $form_state['values']
  );
}


function sirius_log_handlers($log_node, $content_type = NULL) {
  $handlers = node_load_multiple(sirius_fieldvals($log_node, 'field_sirius_log_handler', 'target_id'));
  if ($content_type) {
    $handlers = array_filter($handlers, function($handler_node) use ($content_type) {
      return $handler_node->type == $content_type;
    });
  }

  return $handlers;
}

function sirius_log_handler($log_node, $content_type) {
  $handlers = sirius_log_handlers($log_node, $content_type);
  if (!$handlers) { return NULL; }
  return reset($handlers); 
}
