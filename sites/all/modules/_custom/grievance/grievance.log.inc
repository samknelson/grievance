<?php

function grievance_node_tab_log($node) {
  drupal_set_title($node->title . ': ' . t('Log'));
  $entity_nid = $node->nid;

  $result = views_embed_view('grievance_log', 'block', $entity_nid);
  $result .= grievance_node_tab_get_addlink("grievance-log", array("grievance" => $entity_nid));

  return $result;
}

function grievance_log_update($grievance_node, $title) {
	global $user;

  $log_node = new stdClass();
  $log_node->title = $title;
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
  $log_node->field_grievance_log_type['und'][0]['tid'] = variable_get('grievance_log_update_type', '');

  $log_node = node_submit($log_node); // Prepare node for saving
  node_save($log_node);
}

function grievance_log_notify($grievance_node, $email, $cc_arr, $subject, $body, $attachments) {
  global $user;

  $log_node = new stdClass();
  $log_node->title = t('Notification sent to ') . $params[''];
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

  $comments = t("COMPANY NOTIFICATION SENT\n");
  $comments .= t('To: ') . $email . "\n";
  foreach ($cc_arr as $cc) {
    $comments .= t('Cc: ') . $email . "\n";
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
