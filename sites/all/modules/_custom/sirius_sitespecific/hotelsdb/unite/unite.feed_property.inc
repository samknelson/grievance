<?php

function unite_feed_property_info() {
  $items = array();
  $items['unite_feed_property'] = array(
    'name' => 'unite_feed_property',
    'title' => 'Properties',
    'description' => 'Import new or existing properties.',
    'steps' => array(
      'upload'    => array('label' => 'Upload',     'callback' => 'sirius_feed_wizard_upload'),
      'sheet'     => array('label' => 'Worksheet',  'callback' => 'sirius_feed_wizard_sheet'),
      'map'       => array('label' => 'Mapping',    'callback' => 'sirius_feed_wizard_map'),
      'options'   => array('label' => 'Options',    'callback' => 'unite_feed_property_options'),
      'verify'    => array('label' => 'Verify',     'callback' => 'sirius_feed_wizard_verify'),
      'process'   => array('label' => 'Process',    'callback' => 'sirius_feed_wizard_process'),
      'review'    => array('label' => 'Review',     'callback' => 'sirius_feed_wizard_review'),
    ),
    'feed' => array(
      'pks' => array(
        'nid' => 'Internal ID (NID)',
        // 'id' => 'External ID',
        'addr' => 'Address',
      ),
      'fields' => array(
        'nid' => array('title' => 'Internal ID (NID)'),
        'addr' => array('title' => 'Address'),
      ),
      'batch_size' => 50,
      'callback_row_verify' => 'unite_feed_property_row_verify',
      'callback_row_process' =>'unite_feed_property_row_process',
    ),
    'callback_launch' => 'sirius_feed_wizard_launch',
    'callback_launch_args' => array('unite_feed_property'),
  );

  return $items;
}

function unite_feed_property_options($info, $data, $feed_node, $current_step) { return drupal_get_form('unite_feed_property_options_form', $info, $data, $feed_node, $current_step); }

function unite_feed_property_options_form($form, &$form_state, $info, $data, $feed_node, $current_step) {
  $form_state['info'] = $info;
  $form_state['data'] = $data;
  $form_state['feed_node'] = $feed_node;
  $form_state['current_step'] = $current_step;
  $steps = sirius_wizard_steps($info, $data, $feed_node);
  $last_data = sirius_feed_lastdata($info, $feed_node);

  $form['#tree'] = TRUE;

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  return $form;
}

function unite_feed_property_options_form_submit($form, &$form_state) {
  $info = $form_state['info'];
  $data = $form_state['data'];
  $feed_node = $form_state['feed_node'];
  $current_step = $form_state['current_step'];
  $data['charge_settings'] = $form_state['values']['charge_settings'];

  sirius_wizard_submit($info, $data, $feed_node, $current_step, "Payment selection saved.");
}

function unite_feed_property_row_verify($feed_node, $info, $data, &$row, $precompute) {
  $pk = $data['pk'];

  static $rowcount;
  $rowcount++;

	// Trim all
	foreach ($row as $key => $value) {
    if (!is_array($value)) { $row[$key] = trim($value); }
	}

  if ($precompute) {
    if ($pk == 'addr') {
      $addr_oneline = $row['addr'];
      $result = sirius_addr_geocode_impl($addr_oneline);
      if (!$result['success']) { return $result; }
      dpm($result);
      drupal_set_message("Postal Code: " . sirius_addr_get_compponent($result['point'], 'postal_code'));
      drupal_set_message("Postal Code Suffix: " . sirius_addr_get_compponent($result['point'], 'postal_code_suffix'));
    }
    return array('success' => FALSE, 'msg' => 'Whatever');
    $row['employer_nid'] = $employer_nid;
    $row['payment_ts'] = $payment_ts;
    $row['charge_ts'] = $charge_ts;

    $domain_nid = $feed_node->field_sirius_domain['und'][0]['target_id'];

    if ($pk == 'ein') {
      $worker_nid = sirius_worker_ein_lookup($employer_nid, $row['ein']);
      if (!$worker_nid) { return array('success' => FALSE, 'msg' => 'Worker not found with this EIN.'); }
    } else {
      if ($pk == 'ssn') { 
        $table = 'field_data_field_sirius_id';
        $column = 'field_sirius_id_value';
        $value = $row['id'];
      } else if ($pk == 'id') { 
        $table = 'field_data_field_sirius_id';
        $column = 'field_sirius_id_value';
        $value = $row['id'];
      } else if ($pk == 'id2') { 
        $table = 'field_data_field_sirius_id2';
        $column = 'field_sirius_id2_value';
        $value = $row['id2'];
      } else if ($pk == 'id3') { 
        $table = 'field_data_field_sirius_ssn';
        $column = 'field_sirius_ssn_value';
        $value = $row['ssn'];
      } 

      $sql = "select max(nid) as a, min(nid) as b from node ";
      $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
      $sql .= "left join $table on $table.entity_type = 'node' and $table.entity_id = node.nid ";
      $sql .= "where node.type = 'sirius_worker' ";
      $sql .= "and $column = :value ";
      if ($domain_nid) {
        $sql .= "and field_sirius_domain_target_id = :domain_nid ";
        $sql_args[':domain_nid'] = $domain_nid;
      } else {
        $sql .= "and field_sirius_domain_target_id IS NULL ";
      }
      $sql_args[':value'] = $value;
      $stmt = sirius_sql_query($sql, $sql_args);
      $hr = $stmt->fetchAssoc();
      $worker_nid = $hr['a'];
      $other_nid = $hr['b'];
      if (!$worker_nid) { return array('success' => FALSE, 'msg' => "Worker not found with this ID ($table, $column)."); }
      if ($worker_nid != $other_nid) { return array('success' => FALSE, 'msg' => "Multiple workers found with this ID ($table, $column)."); }
    }

    $row['worker_nid'] = $worker_nid;

    // Account NID
    $sql_args = array();
    $sql = "select max(nid) as a, min(nid) as b ";
    $sql .= "from node ";
    $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_name_short on field_data_field_sirius_name_short.entity_type = 'node' and field_data_field_sirius_name_short.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_json on field_data_field_sirius_json.entity_type = 'node' and field_data_field_sirius_json.entity_id = node.nid ";
    $sql .= "where node.type = 'sirius_ledger_account' ";
    $sql .= "and ( ";
    $sql .= "  field_sirius_name_short_value = :name_a ";
    $sql .= "  or title = :name_b ";
    $sql .= "  or nid = :name_c ";
    // Not sure why but cannot embed a parameter into json_contains
    $inline = preg_replace('/[^a-zA-Z0-9_ ]/', '', $row['account']);
    $sql .= "  or json_contains(field_sirius_json_value, '\"$inline\"', '$.settings.aliases') ";
    $sql .= ") ";
    $sql_args[':name_a'] = $sql_args[':name_b'] = $sql_args[':name_c'] = $row['account'];
    if ($domain_nid) {
      $sql .= "and field_sirius_domain_target_id = :domain_nid ";
      $sql_args[':domain_nid'] = $domain_nid;
    } else {
      $sql .= "and field_sirius_domain_target_id IS NULL ";
    }
    $stmt = sirius_sql_query($sql, $sql_args);
    $hr = $stmt->fetchAssoc();
    $account_nid = $hr['a'];
    $other_nid = $hr['b'];
    if (!$account_nid) { return array('success' => FALSE, 'msg' => "Account not found."); }
    if ($account_nid != $other_nid) { return array('success' => FALSE, 'msg' => "Multiple accounts found."); }
    if (!$account_nid) { return array('success' => FALSE, 'msg' => 'Account not found.'); }
    $row['account_nid'] = $account_nid;

  }

  return array('success' => TRUE, 'msg' => 'Looks good.');
}

function unite_feed_property_row_process($feed_node, $info, $data, $row) {
  // return array('success' => FALSE, 'msg' => 'Disabled');
	$result = unite_feed_property_row_verify($feed_node, $info, $data, $row, TRUE);
	if (!$result['success']) { return $result; }

  return array('success' => FALSE, 'msg' => 'Under Development');

  if ($charged) {
    return array('success' => TRUE, 'msg' => 'Payment allocated and charge created.');
  } else {
    return array('success' => TRUE, 'msg' => 'Payment allocated.');
  }
}
