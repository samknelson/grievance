<?php

/**
 * A stupid little library that currently only handles US phone numbers
 */


/**
 * This is copied from the "phone" module, format_ca_phone_number().  I 
 * don't like any of the "real" phone fields, so I'm just hardcoding this for
 * now.  @todo handle phone numbers appropriately, with validation and a real
 * phone number field.
 */

function sirius_phone_format($phonenumber) {
  // define regular expression
  $regex = '/
    \D*            # ignore non-digits
    (\d*)          # an optional 1
    \D*            # optional separator
    ([2-9][0-8]\d) # area code (Allowed range of [2-9] for the first digit, [0-8] for the second, and [0-9] for the third digit)
    \D*            # optional separator
    ([2-9]\d{2})   # 3-digit prefix (cannot start with 0 or 1)
    \D*            # optional separator
    (\d{4})        # 4-digit line number
    \D*            # optional separator
    (\d*)          # optional extension
    \D*            # ignore trailing non-digits
    /x';

  // get digits of phone number
  preg_match($regex, $phonenumber, $matches);

  if (!$matches) { return $phonenumber; }

  $separator = '-';

  // construct ten-digit phone number
  $phonenumber = '(' . $matches[2] . ') ' . $matches[3] . $separator . $matches[4];

  // Optional extension
  if ($matches[5] != '') {
      $phonenumber .= ' x' . $matches[5];
  }

  return $phonenumber;
}

function sirius_phone_validate($phonenumber) {
	$phonenumber = sirius_phone_format($phonenumber);
	if (!preg_match('/\(\d\d\d\)\ \d\d\d\-\d\d\d\d/', $phonenumber)) {
		return FALSE;
	}

	return TRUE;
}

function sirius_phone_format_e164($phonenumber) {
	$phonenumber = preg_replace('/[^\d]/', '', $phonenumber);
  if (strlen($phonenumber) < 11) { $phonenumber = '1' . $phonenumber; }
  $phonenunmber = '+' . $phonenumber;
	return $phonenumber;
}

function sirius_phone_keypad($string) {
  $replacements = array(
    'a' => 2,
    'b' => 2,
    'c' => 2,
    'd' => 3,
    'e' => 3,
    'f' => 3,
    'g' => 4,
    'h' => 4,
    'i' => 4,
    'j' => 5,
    'k' => 5,
    'l' => 5,
    'm' => 6,
    'n' => 6,
    'o' => 6,
    'p' => 7,
    'q' => 7,
    'r' => 7,
    's' => 7,
    't' => 8,
    'u' => 8,
    'v' => 8,
    'w' => 9,
    'x' => 9,
    'y' => 9,
    'z' => 9,
  );

  $string = str_ireplace(array_keys($replacements), array_values($replacements), $string);
  $string = preg_replace('/[^0-9]/', '', $string);
  $string = substr($string, 0, 6);
  return $string;
}

function sirius_sms_send($params) {
  // Allow callers to set the X-Sirius-Id, and don't resent with the same ID. This allows callers to set unique keys.
  $id = $params['X-Sirius-Id'];
  if ($id) {
    if (sirius_sms_already_sent($id)) {
      return array('success' => FALSE, 'msg' => "Skipping sending for message with id $id, because it's already been sent.");
    }
  }

  $phone = $params['to'];
  $msg = $params['sms'];
  $handler_nids = $params['handler_nids'];
  if (is_array($handler_nids)) {
    $handler_nid = $handler_nids[0];
  } else {
    $handler_nid = $handler_nids;
  }

  $result = sirius_twilio_outgoing_sms($phone, $msg, $handler_nid);

  if ($result['success']) {
    sirius_log(
      'sms', 
      'sending', 
      $phone, 
      $msg,
      $id, 
      $handler_nids
    );
  } else {
    sirius_log(
      'sms', 
      'failed', 
      $phone, 
      $result['msg'] . ': ' . $msg,
      $id, 
      $handler_nids
    );
  }

  return $result;
}

function sirius_sms_already_sent($id) {
  // SQL for speed
  $sql = "select * from node ";
  $sql .= "join field_data_field_sirius_notes on field_data_field_sirius_notes.entity_type = 'node' and field_data_field_sirius_notes.entity_id = node.nid ";
  $sql .= "join field_data_field_sirius_category on field_data_field_sirius_category.entity_type = 'node' and field_data_field_sirius_category.entity_id = node.nid ";
  $sql .= "join field_data_field_sirius_type on field_data_field_sirius_type.entity_type = 'node' and field_data_field_sirius_type.entity_id = node.nid ";
  $sql .= "where node.type = 'sirius_log' ";
  $sql .= "and field_sirius_notes_value = :id ";
  $sql .= "and field_sirius_category_value = 'sms' ";
  $sql .= "and field_sirius_type_value = 'sending' ";
  $stmt = db_query($sql, array(':id' => $id));
  $hr = $stmt->fetchAssoc();
  if ($hr) { return TRUE; }
  return FALSE;
}

function sirius_sms_admin() {
  $form["sirius_sms_global_disable"] = array(
    '#type' => 'select',
    '#title' => t('Global Disable'),
    '#options' => array(
      'No' => t('No'),
      'Yes' => t('Yes'),
    ),
    '#description' => t('Select "Yes" to globally disable SMS on this server.'),
    '#default_value' => variable_get("sirius_sms_global_disable", ''),
  );

  $form["sirius_sms_global_disable_whitelist"] = array(
    '#type' => 'textarea',
    '#title' => t('Whitelist'),
    '#description' => t('When global disable is on, use the whitelist to indicate which phone numbers <strong>can</strong> receive SMS. Phone numbers must be in the format (xxx) xxx-xxxx.'),
    '#default_value' => variable_get("sirius_sms_global_disable_whitelist", ''),
    '#states' => array(
      'visible' => array(
        array(':input[name="sirius_sms_global_disable"]' => array('value' => 'Yes')),
      )
    ),
  );

  return system_settings_form($form);
}


function sirius_voice_send($params) {
  // Allow callers to set the X-Sirius-Id, and don't resent with the same ID. This allows callers to set unique keys.
  $id = $params['X-Sirius-Id'];
  if ($id) {
    if (sirius_voice_already_sent($id)) {
      return array('success' => FALSE, 'msg' => "Skipping sending for message with id $id, because it's already been sent.");
    }
  }

  $phone = $params['to'];
  $msg = $params['voice'];
  $handler_nids = $params['handler_nids'];
  if (is_array($handler_nids)) {
    $handler_nid = $handler_nids[0];
  } else {
    $handler_nid = $handler_nids;
  }

  $context = array();
  $context['outgoing_msg'] = $msg;
  $context['handler_nid'] = $handler_nid;
  $context['worker_nid'] = $params['worker_nid'];


  sirius_twilio_outgoing_voice($phone, $context);

  sirius_log(
    'voice', 
    'sending', 
    $phone, 
    $msg,
    $id, 
    $handler_nids
  );

  return array('success' => TRUE, 'msg' => 'Voice message sent.');
}

function sirius_voice_already_sent($id) {
  // SQL for speed
  $sql = "select * from node ";
  $sql .= "join field_data_field_sirius_notes on field_data_field_sirius_notes.entity_type = 'node' and field_data_field_sirius_notes.entity_id = node.nid ";
  $sql .= "join field_data_field_sirius_category on field_data_field_sirius_category.entity_type = 'node' and field_data_field_sirius_category.entity_id = node.nid ";
  $sql .= "join field_data_field_sirius_type on field_data_field_sirius_type.entity_type = 'node' and field_data_field_sirius_type.entity_id = node.nid ";
  $sql .= "where node.type = 'sirius_log' ";
  $sql .= "and field_sirius_notes_value = :id ";
  $sql .= "and field_sirius_category_value = 'voice' ";
  $sql .= "and field_sirius_type_value = 'sending' ";
  $stmt = db_query($sql, array(':id' => $id));
  $hr = $stmt->fetchAssoc();
  if ($hr) { return TRUE; }
  return FALSE;
}
