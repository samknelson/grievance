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

function sirius_twilio_get_context($call_sid) {
  if (!$call_sid) { return array(); }
  
  // We look for the most recent outgoing log entry of type "twilio:conversation"
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_log');
  $query->fieldCondition('field_sirius_category', 'value', 'twilio:call');
  $query->fieldCondition('field_sirius_notes', 'value', $call_sid);
  $query->propertyOrderBy('nid', 'desc');
  $query->range(0, 1);
  $stmt = $query->execute();
  if (!$stmt['node']) { return NULL; }
  $log_node = node_load(array_keys($stmt['node'])[0]);
  return json_decode($log_node->field_sirius_json['und'][0]['value'], TRUE);
}

function sirius_twilio_set_context($call_sid, $context, $type = 'context_set', $msg = NULL, $notes = NULL) {
  $context_json = json_encode($context);

  sirius_log(
    'twilio:call', 
    $type, 
    $msg, 
    $notes,
    $call_sid, 
    $context['handler_nid'],
    NULL,
    $context_json
  );
}
