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
  if (preg_match('/^[0-9]/', $phonenumber)) { $phonenumber = '+' . $phonenumber; }
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

function sirius_phonenumber_node_validate($node, $form, &$form_state) {
  if (!sirius_phone_validate($node->field_sirius_id['und'][0]['value'])) {
   form_set_error('field_sirius_id', t('Please enter a valid phone number.'));
  }
}

function sirius_phonenumber_node_presave($node) {
  $phonenumber = sirius_phone_format($node->field_sirius_id['und'][0]['value']);
  $node->field_sirius_id['und'][0]['value'] = sirius_phone_format($node->field_sirius_id['und'][0]['value']);
  $node->title = $node->field_sirius_id['und'][0]['value'];

  $sms_possible = $voice_possible = 'Yes';

  $json = sirius_json_get($node);
  if ($json) {
    // Clean up legacy
    if ($json['result']) {
      if (!$json['scan']) { 
        $json['scan']['result'] = $json['result'];
      }
      unset($json['result']);
      sirius_json_set($node, $json);
    }

    // Check scan for carriers, to see if SMS or voice is possible.
    if ($json['scan']) {
      if (!$json['scan']['result']['carrier']['mobile_network_code']) {
        sirius_minilog("$phonenumber: No carrier code found, setting sms_possible = No");
        $sms_possible = 'No';
      }
      if (!$json['scan']['result']['carrier']['name']) {
        sirius_minilog("$phonenumber: No carrier name found, setting voice_possible = No");
        $voice_possible = 'No';
      }
    }

    // Check for manual block
    if ($json['manual_block']) {
      if ($json['manual_block']['sms'] == 'Yes') {
        sirius_minilog("$phonenumber: Manual SMS block found, setting sms_possible = No");
        $sms_possible = 'No';
      }
      if ($json['manual_block']['voice'] == 'Yes') {
        sirius_minilog("$phonenumber: Manual voice block found, setting voice_possible = No");
        $voice_possible = 'No';
      }
    }

    // //////////////////////
    // Check for user block on the whole phone number. 
    // This is legacy support. Going forward, user blocks are for specific senders only.
    // //////////////////////

    // If we ever *un-blocked* any sender, clear the global block.
    if ($json['user_block']['sms'] == 'Yes') {
      foreach ($json['user_block'] as $key => $value) {
        if (is_array($value) && $json['user_block'][$key]['sms'] == 'No') {
          $json['user_block']['sms'] = "No";
          drupal_set_message("Reset global block to \"no\".");
        }
      }
    }
    // If we have a global block, then stop sending altogether.
    if ($json['user_block']) {
      if ($json['user_block']['sms'] == 'Yes') {
        sirius_minilog("$phonenumber: User SMS block found, setting sms_possible = No");
        $sms_possible = 'No';
      }
      if ($json['user_block']['voice'] == 'Yes') {
        sirius_minilog("$phonenumber: User voice block found, setting voice_possible = No");
        $voice_possible = 'No';
      }
    }


    // Check for STOP error code
    if ($json['last_sms'] && !$json['last_sms']['result']['success']) {
      if ($json['last_sms']['result']['code'] == 21610) {
        sirius_minilog("$phonenumber: Last SMS had a result code of 21610, setting sms_possible = No");
        $sms_possible = 'No';
      }
    }
  }

  $node->field_sirius_sms_possible['und'][0]['value'] = $sms_possible;
  $node->field_sirius_voice_possible['und'][0]['value'] = $voice_possible;
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

function sirius_phonenumber_scan(&$node, $force_refresh = FALSE) {
  $phonenumber = $node->field_sirius_id['und'][0]['value'];
  $json = sirius_json_get($node);

  if (!$force_refresh && (strtotime($node->field_sirius_datetime['und'][0]['value']) > time() - 60*60*24*30) ) {
    sirius_minilog("Returning cached value, last refreshed " . $node->field_sirius_datetime['und'][0]['value']);
    return $node;
  }

  sirius_minilog("$phonenumber: Scanning ...");
  $result = sirius_twilio_lookup($phonenumber);
  $json['scan']['result'] = $result;
  $json['scan']['ts'] = time();
  sirius_minilog("$phonenumber: " . $result['msg']);
  $node->field_sirius_datetime['und'][0]['value'] = date('Y-m-d H:i:s');
  sirius_json_set($node, $json);
  node_object_prepare($node);
  $node = node_submit($node);
  node_save($node);
  return $node;
}

function sirius_phonenumber_voice_possible($phonenumber) {
  $node = sirius_phonenumber_lookup($phonenumber);
  if (sirius_domain_variable_get('sirius_twilio_lookup_enable_voice', FALSE)) {
    sirius_phonenumber_scan($node);
  }
  return $node->field_sirius_voice_possible['und'][0]['value'] != 'No';
}

function sirius_phonenumber_sms_possible($phonenumber, $from = NULL) {
  if (!$from) { $from = sirius_domain_variable_get('sirius_twilio_phone', ''); }
  $from = sirius_phone_format($from);

  $node = sirius_phonenumber_lookup($phonenumber);
  if (sirius_domain_variable_get('sirius_twilio_lookup_enable_sms', FALSE)) {
    sirius_phonenumber_scan($node);
  }

  // No SMS to this number at all?
  if ($node->field_sirius_sms_possible['und'][0]['value'] == 'No') { return FALSE; }

  // Check for STOPs from our sender
  $json = sirius_json_get($node);
  // if ($json['user_block']['sms'] == 'Yes') { return FALSE; }
  if ($json['user_block'][$from]['sms'] == 'Yes') { return FALSE; }

  return TRUE;
}

function sirius_page_phonenumber_list() {
  return views_embed_view('sirius_phonenumbers', 'block_1');
}

function sirius_page_phonenumber_check() {
  return drupal_get_form('sirius_page_phonenumber_check_form');
}

function sirius_page_phonenumber_check_form($form, &$form_state, $phonenumber_node = NULL) {
  if ($phonenumber_node) { 
    $form['phonenumber'] = array('#type' => 'value', '#value' => $phonenumber_node->field_sirius_id['und'][0]['value']);
    // damn form rebuild
    $phonenumber_node_latest = node_load($phonenumber_node->nid);
    $form['scan'] = array('#markup' => sirius_phonenumber_render_scan($phonenumber_node_latest));
  } else {
    $form['phonenumber'] = array(
      '#type' => 'textfield', 
      '#title' => t('Phone Number'),
      '#required' => TRUE,
    );
  }

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

  $node = sirius_phonenumber_lookup($phonenumber);
  sirius_phonenumber_scan($node, $force);
  sirius_minilog("SMS Possible: " . $node->field_sirius_sms_possible['und'][0]['value']);
  sirius_minilog("Voice Possible: " . $node->field_sirius_voice_possible['und'][0]['value']);
  sirius_minilog("Details: " . l("Node " . $node->nid, 'node/' . $node->nid));
}

function sirius_phone_render($phone) {
  if (!$phone) { return ''; }
  $phone = sirius_phone_format($phone);
  $phone = htmlspecialchars($phone);
  return "<a href=\"tel:$phone\">$phone</a>";
}

function sirius_phonenumber_render_scan($node) {
  drupal_add_css(drupal_get_path('module', 'sirius') . '/css/phone.css');

  $json = sirius_json_get($node);

  if (!$json['scan']) {
    $html .= t('Scan results are not available.');
  } else {
    
    if ($json['scan']['result']['carrier']) {
      $carrier_name = $json['scan']['result']['carrier']['name'];
      if (!$carrier_name) { $carrier_name = 'No carrier'; }

      $carrier_type = $json['scan']['result']['carrier']['type'];
      if (!$carrier_type) { $carrier_type = 'No type'; }

      $html .= " Carrier: $carrier_name / $carrier_type";

      if ($json['scan']['result']['carrier']['error_code']) {
        $html .= '<br />' . t('Error code: ') . sirius_twilio_render_error($json['scan']['result']['carrier']['error_code']);
      }
    }
    $html .= '<br />' . t('Scanned ') . date('r', $json['scan']['ts']);
    $html .= '<br />' . $json['scan']['result']['msg'];
  }

  $html = '<div class="sirius_phonenumber_scan">' . '<strong>' . t('Last Scan') . '</strong><br />' . $html . '</div>';

  return $html;
}

function sirius_phonenumber_render_last_sms($node) {
  drupal_add_css(drupal_get_path('module', 'sirius') . '/css/phone.css');

  $json = sirius_json_get($node);

  if (!$json['last_sms']) {
    $html .= t('Last SMS not available.');
  } else {
    $html .= $json['last_sms']['params']['sms'];
    $html .= '<br />' . t('Sent ') . date('r', $json['last_sms']['ts']);
    if (!$json['last_sms']['result']['success']) {
      $html .= '<br />' . $json['last_sms']['result']['msg'];
      if ($json['last_sms']['result']['details']) {
        $html .= '<br />' . $json['last_sms']['result']['details'];
      }
    }
  }

  $html = '<div class="sirius_phonenumber_last_sms">' . '<strong>' . t('Last Outgoing SMS') . '</strong><br />' . $html . '</div>';

  return $html;
}

function sirius_phonenumber_render_last_sms_incoming($node) {
  drupal_add_css(drupal_get_path('module', 'sirius') . '/css/phone.css');

  $json = sirius_json_get($node);

  if (!$json['last_sms_incoming']) {
    $html .= t('Last incoming SMS not available.');
  } else {
    $html .= $json['last_sms_incoming']['msg'];
    $html .= '<br />' . t('Sent ') . date('r', $json['last_sms_incoming']['ts']);
  }

  $html = '<div class="sirius_phonenumber_last_sms">' . '<strong>' . t('Last Incoming SMS') . '</strong><br />' . $html . '</div>';

  return $html;
}

function sirius_phonenumber_render_user_block($node) {
  drupal_add_css(drupal_get_path('module', 'sirius') . '/css/phone.css');

  $json = sirius_json_get($node);

  if (!$json['user_block']) {
    $html .= t('No user block.');
  } else {
    if ($json['user_block']['sms']) {
      $html .= "Overall SMS Block: " . $json['user_block']['sms'];
      $html .= "<br />Comments: " . $json['user_block']['comments'];
      $html .= '<br />' . t('Set on ') . date('r', $json['user_block']['ts']) . '<br />';
    }

    foreach ($json['user_block'] as $phone => $record) {
      if (!sirius_phone_validate($phone)) { continue; }
      $html .= "SMS block on <strong>$phone</strong>: " . $record['sms'];
      if ($json['user_block']['voice']) { $html .= "<br />Voice block: " . $record['voice']; }
      $html .= "<br />Comments: " . $record['comments'];
      $html .= '<br />' . t('Set on ') . date('r', $record['ts']);
    }
  }

  $html = '<div class="sirius_phonenumber_last_sms">' . '<strong>' . t('User Block') . '</strong><br />' . $html . '</div>';

  return $html;
}

function sirius_phonenumber_render_manual_block($node) {
  drupal_add_css(drupal_get_path('module', 'sirius') . '/css/phone.css');

  $json = sirius_json_get($node);

  if (!$json['manual_block']) {
    $html .= t('No administrative block.');
  } else {
    $html .= "SMS block: " . $json['manual_block']['sms'];
    $html .= "<br />Voice block: " . $json['manual_block']['voice'];
    $html .= "<br />Comments: " . $json['manual_block']['comments'];
    $html .= '<br />' . t('Set on ') . date('r', $json['manual_block']['ts']);
  }

  $html = '<div class="sirius_phonenumber_last_sms">' . '<strong>' . t('Administrative Block') . '</strong><br />' . $html . '</div>';

  return $html;
}
