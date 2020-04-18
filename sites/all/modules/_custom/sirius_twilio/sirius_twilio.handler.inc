<?php

/**********************************************************************************
 * SMS
 **********************************************************************************/

function sirius_twilio_handle_sms($msg, $phone) {
  $domain_nid = sirius_domain_nid();
  $msg = trim($msg);
  $msg = strtolower($msg);

  // I got this mostly implemented, then realized that Twilio does this natively beyond our control.
  /*
  if ($msg == 'stop' || $msg == 'resume') {
    $phonenumber_node = sirius_phonenumber_lookup($phone);
    $json = json_decode($phonenumber_node->field_sirius_json['und'][0]['value']);
    if ($msg == 'stop') {
      $text = "Text messaging from this site has been suspedned. Text RESUME to resume messaging.";
      $json['unsubscribe'] = TRUE;
      $json['msg'] = $text;
      $phonenumber_node->field_sirius_json['und'][0]['value'] = json_encode($json);
      $phonenumber_node->field_sirius_sms_possible['und'][0]['value'] = 'No';
      sirius_jsonfield_log($phonenumber_node, 'stop', $text);
    } else {
      $text = "Text messaging has resumed. Text STOP to stop receiving messages from this site.";
      $json['unsubscribe'] = FALSE;
      $json['msg'] = $text;
      $phonenumber_node->field_sirius_json['und'][0]['value'] = json_encode($json);
      $phonenumber_node->field_sirius_sms_possible['und'][0]['value'] = 'Yes';
      sirius_jsonfield_log($phonenumber_node, 'stop', $text);
    }
    node_save($phonenumber_node);
  } else {
  }
  */
  
  global $base_url;
  $text = "This system cannot respond to incoming messages. Please sign in at the link below.\n";
  $text .= $base_url;

  return sirius_twilio_outgoing_sms($phone, $text);
}

function sirius_twilio_handler_get_sms($phone, $msg) {
  $phone = sirius_phone_format_e164($phone);
  $msg = trim($msg);
  list($nid, $action) = preg_split('/\s+/', $msg);
  if (!$nid || !$action) { return NULL; }
  return $nid;
}

/**********************************************************************************
 * Voice
 **********************************************************************************/

function sirius_twilio_get_context_node($call_sid) {
  if (!$call_sid) { return NULL; }
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_log');
  $query->fieldCondition('field_sirius_category', 'value', 'twilio:call');
  $query->fieldCondition('field_sirius_notes', 'value', $call_sid);
  $query->propertyOrderBy('nid', 'desc');
  $query->range(0, 1);
  $stmt = $query->execute();
  if (!$stmt['node']) { return NULL; }
  return node_load(array_keys($stmt['node'])[0]);
}

function sirius_twilio_get_context($call_sid) {
  if (!$call_sid) { return array(); }
  $log_node = sirius_twilio_get_context_node($call_sid);
  $json = json_decode($log_node->field_sirius_json['und'][0]['value'], TRUE);
  return $json['context'];
}

function sirius_twilio_set_context($call_sid, $context, $type = 'context_set', $msg = NULL, $summary = NULL) {
  $log_node = sirius_twilio_get_context_node($call_sid);
  if (!$log_node) {
    sirius_log(
      'twilio:call', 
      $type, 
      $msg, 
      $summary,
      $call_sid, 
      $context['handler_nid'],
      NULL,
      array('context' => $context)
    );
  } else {
    $old_type = $log_node->field_sirius_type['und'][0]['value'];
    $old_msg = $log_node->field_sirius_message['und'][0]['value'];
    $old_summary = $log_node->field_sirius_notes['und'][0]['value'];
    $old_handler = $log_node->field_sirius_log_handler['und'][0]['target_id'];

    $log_node->field_sirius_type['und'][0]['value'] = $type;
    $log_node->field_sirius_message['und'][0]['value'] = substr($msg, 0, 254);
    $log_node->field_sirius_summary['und'][0]['value'] = substr($summary, 0, 254);

    $json = json_decode($log_node->field_sirius_json['und'][0]['value'], TRUE);
    $json['context'] = $context;
    $log_node->field_sirius_json['und'][0]['value'] = json_encode($json);
    sirius_jsonfield_log($log_node, 'context_set', $msg, array('old_type' => $old_type, 'old_msg' => $old_msg, 'old_summary' => $old_summary, 'old_handler' => $old_handler));
    node_save($log_node);
  }
}

