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
  // $phonenumber = '+' . $phonenumber;
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

// @todo
function sirius_sms_enabled() { return TRUE; }

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

  $possible = sirius_phonenumber_sms_possible($phone);
  if (!$possible) { return array('success' => FALSE, 'msg' => "$phone does not appear to be a mobile-enabled number."); }

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

  $name = sirius_domain_variable_name('sirius_sms_global_disable');
  $form[$name] = array(
    '#type' => 'select',
    '#title' => t('Global Disable'),
    '#options' => array(
      'No' => t('No'),
      'Yes' => t('Yes'),
    ),
    '#description' => t('Select "Yes" to globally disable SMS on this server.'),
    '#default_value' => variable_get($name, ''),
  );

  $name = sirius_domain_variable_name('sirius_sms_global_disable_whitelist');
  $form[$name] = array(
    '#type' => 'textarea',
    '#title' => t('Whitelist'),
    '#description' => t('When global disable is on, use the whitelist to indicate which phone numbers <strong>can</strong> receive SMS. Phone numbers must be in the format (xxx) xxx-xxxx.'),
    '#default_value' => variable_get($name, ''),
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

  $possible = sirius_phonenumber_voice_possible($phone);
  if (!$possible) { return array('success' => FALSE, 'msg' => "$phone does not appear to have a valid voice carrier."); }

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
  $context['callerid_nid'] = $params['callerid_nid'];
  $context['phonetree'] = $params['phonetree'];
  $context['amd'] = $params['amd'];

  $result = sirius_twilio_outgoing_voice($phone, $context);

  sirius_log(
    'voice', 
    'sending', 
    $phone, 
    $msg,
    $id, 
    $handler_nids
  );

  return $result; 
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

function sirius_phonenumber_node_validate($node, $form, &$form_state) {
  if (!sirius_phone_validate($node->field_sirius_id['und'][0]['value'])) {
   form_set_error('field_sirius_id', t('Please enter a valid phone number.'));
  }
}

function sirius_phonenumber_node_presave($node) {
  $node->field_sirius_id['und'][0]['value'] = sirius_phone_format($node->field_sirius_id['und'][0]['value']);
  $node->title = $node->field_sirius_id['und'][0]['value'];
}

function sirius_phonenumber_lookup($phonenumber) {
  // Look up the node
  $domain_nid = sirius_domain_nid();
  $phonenumber = sirius_phone_format($phonenumber);
  $sql_args = array(':phonenumber' => $phonenumber);
  $sql = "select field_data_field_sirius_id.entity_id from field_data_field_sirius_id ";
  $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = field_data_field_sirius_id.entity_id ";
  $sql .= "where field_data_field_sirius_id.entity_type = 'node' ";
  $sql .= "and field_data_field_sirius_id.bundle = 'sirius_phonenumber' ";
  $sql .= "and field_data_field_sirius_id.field_sirius_id_value = :phonenumber ";

  if ($domain_nid) {
    $sql .= "and field_sirius_domain_target_id = :domain_nid ";
    $sql_args[':domain_nid'] = $domain_nid;
  }
  $stmt = db_query($sql, $sql_args);
  $hr = $stmt->fetchAssoc();
  $nid = $hr['entity_id'];
  if ($nid) {
    $node = node_load($nid);
    sirius_minilog("Existing record found for $phonenumber.");
  } else {
    global $user;
    $node = new stdClass();
    $node->type = 'sirius_phonenumber';
    node_object_prepare($node);
    $node->language = LANGUAGE_NONE;
    $node->uid = $user->uid;
    $node->status = 1;
    $node->promote = 0;
    $node->comment = 0;
    $node->field_sirius_id['und'][0]['value'] = $phonenumber;
    if ($domain_nid) { $node->field_sirius_domain['und'][0]['target_id'] = $domain_nid; }
    sirius_minilog("Existing record not found for $phonenumber, creating a new one.");
  }

  return $node;
}

function sirius_phonenumber_check($phonenumber, $force_refresh = FALSE) {
  $node = sirius_phonenumber_lookup($phonenumber);
  $json = json_decode($node->field_sirius_json['und'][0]['value'], TRUE);

  if (!$force_refresh && (strtotime($node->field_sirius_datetime['und'][0]['value']) > time() - 60*60*24*30) ) {
    sirius_minilog("Returning cached value, last refreshed " . $node->field_sirius_datetime['und'][0]['value']);
    return $node;
  }

  sirius_minilog("Scanning ...");
  $result = sirius_twilio_lookup($phonenumber);
  $json['result'] = $result;
  sirius_minilog($result['msg']);
  $node->field_sirius_datetime['und'][0]['value'] = date('Y-m-d H:i:s');
  $node->field_sirius_json['und'][0]['value'] = json_encode($json);
  if ($json['result']['carrier']['mobile_network_code']) { $node->field_sirius_sms_possible['und'][0]['value'] = 'Yes'; } else { $node->field_sirius_sms_possible['und'][0]['value'] = 'No'; }
  if ($json['result']['carrier']['name']) { $node->field_sirius_voice_possible['und'][0]['value'] = 'Yes'; } else { $node->field_sirius_voice_possible['und'][0]['value'] = 'No'; }
  node_object_prepare($node);
  $node = node_submit($node);
  node_save($node);
  return $node;
}

function sirius_phonenumber_voice_possible($phonenumber) {
  $node = sirius_phonenumber_check($phonenumber);
  return $node->field_sirius_voice_possible['und'][0]['value'] != 'No';
}

function sirius_phonenumber_sms_possible($phonenumber) {
  $node = sirius_phonenumber_check($phonenumber);
  return $node->field_sirius_sms_possible['und'][0]['value'] != 'No';
}

function sirius_page_phonenumber_list() {
  return views_embed_view('sirius_phonenumbers', 'block_1');
}

function sirius_page_phonenumber_check() {
  return drupal_get_form('sirius_page_phonenumber_check_form');
}

function sirius_page_phonenumber_check_form($form, &$form_state) {
  $form['phonenumber'] = array(
    '#type' => 'textfield', 
    '#title' => t('Phone Number'),
    '#required' => TRUE,
  );

  $form['force'] = array(
    '#type' => 'select', 
    '#title' => t('Force recheck?'),
    '#required' => TRUE,
    '#options' => array(
      'no' => t('No, look up our cached value'),
      'yes' => t('Yes, force a rescan of this phone number'),
    ),
    '#default_value' => 'no',
  );

  $form['submit'] = array(
    '#type' => 'submit', 
    '#value' => t('Submit'),
  );

  $log = sirius_minilog_render_html();
  if ($log) { $form['results'] = array('#markup' => '<hr>' . $log); }

  return $form;
}

function sirius_page_phonenumber_check_form_validate($form, &$form_state) {
  if (!sirius_phone_validate($form_state['values']['phonenumber'])) {
    form_set_error('phonenumber', t('Please enter a valid phone number.'));
  }
}

function sirius_page_phonenumber_check_form_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
  $phonenumber = $form_state['values']['phonenumber'];
  if ($form_state['values']['force'] == 'yes') { $force = TRUE; } else { $force = FALSE; }
  $node = sirius_phonenumber_check($phonenumber, $force);
  sirius_minilog("SMS Possible: " . $node->field_sirius_sms_possible['und'][0]['value']);
  sirius_minilog("Voice Possible: " . $node->field_sirius_voice_possible['und'][0]['value']);
  sirius_minilog("Details: " . l("Node " . $node->nid, 'node/' . $node->nid));
}

function sirius_sms_segment_count($html_id) {
  drupal_add_js(array('sirius_sms_segment_count' => array('html_id' => $html_id)), array('type' => 'setting'));
  drupal_add_js('sites/all/modules/_custom/sirius/js/sms_segment_count.js');
  drupal_add_css('sites/all/modules/_custom/sirius/css/sms_segment_count.css');

  $html = '<div class="sirius_sms_segment_count" id="' . $html_id . '_count">';
  $html .= t('SMS Segments:') . '<span class="stat messages"></span>';
  $html .= ' / ';
  $html .= t('Remaining in segment:') . '<span class="stat remaining"></span>';
  $html .= ' / ';
  $html .= t('Total length:') . '<span class="stat length"></span>';
  $html .= ' / ';
  // $html .= t('Per segment:') . '<span class="stat per_message"></span>';
  // $html .= ' / ';
  $html .= t('Encoding:') . '<span class="stat encoding"></span>';
  $html .= '</div>';

  return $html;
}