<?php

function sirius_ledger_feed_allocate_info() {
  $items = array();
  $items['ledger_feed_allocate'] = array(
    'name' => 'ledger_feed_allocate',
    'title' => 'A/R Payment Allocation',
    'description' => 'Allocate one or more payments. For example, if an employer submits a dues check accompanied by a spreadsheet indicating how much was withheld from each employee\'s paycheck, this feed can be used to allocate the payment against various employees\' accounts.',
    'steps' => array(
      'upload'    => array('label' => 'Upload',     'callback' => 'sirius_feed_wizard_upload'),
      'sheet'     => array('label' => 'Worksheet',  'callback' => 'sirius_feed_wizard_sheet'),
      'map'       => array('label' => 'Mapping',    'callback' => 'sirius_feed_wizard_map'),
      'payments'  => array('label' => 'Payments',    'callback' => 'sirius_ledger_feed_allocate_payments'),
      'verify'    => array('label' => 'Verify',     'callback' => 'sirius_feed_wizard_verify'),
      'process'   => array('label' => 'Process',    'callback' => 'sirius_feed_wizard_process'),
      'review'    => array('label' => 'Review',     'callback' => 'sirius_ledger_feed_allocate_review'),
    ),
    'feed' => array(
      'pks' => array(
        'id' => 'Badge ID',
        'nid' => 'Worker NID',
        'ssn' => 'SSN',
        'ein' => 'Employee ID',
      ),
      'fields' => array(
        'nid' => array('title' => 'Worker NID'),
        'ssn' => array('title' => 'SSN'),
        'id' => array('title' => 'Badge ID'),
        'ein' => array('title' => 'EIN'),
        'amount' => array('title' => 'Amount'),
        'payment_date' => array('title' => 'Payment Date', 'description' => 'Date that the payment will be credited.'),
        'charge_date' => array('title' => 'Charge Date', 'description' => 'For any charges generated as part of the upload, date that the charge will be posted.'),
        'account' => array('title' => 'Account', 'description' => 'May match the name, short name, or any of the aliases for the account'),
        'memo_prefix' => array('title' => 'Memo Prefix', 'description' => 'The memo will be the name of the payment. You may also select a field to prefix to the memo. Memos will be truncated to 255 characters.'),
        'memo_suffix' => array('title' => 'Memo Suffix', 'description' => 'The memo will be the name of the payment. You may also select a field to suffix to the memo. Memos will be truncated to 255 characters.'),
      ),
      'batch_size' => 500,
      'callback_row_verify' => 'sirius_ledger_feed_allocate_row_verify',
      'callback_row_process' =>'sirius_ledger_feed_allocate_row_process',
      'callback_preprocess' => 'sirius_ledger_feed_allocate_preprocess',
      'callback_postprocess' => 'sirius_ledger_feed_allocate_postprocess',
      'employer_access' => TRUE,
    ),
    'callback_launch' => 'sirius_feed_wizard_launch',
    'callback_launch_args' => array('ledger_feed_allocate'),
  );

  $employer = sirius_employer_get();
  if ($employer) { unset($items['ledger_feed_allocate']['feed']['fields']['employer']); }
  return $items;
}

function sirius_ledger_feed_allocate_payments($info, $data, $feed_node, $current_step) { return drupal_get_form('sirius_ledger_feed_allocate_payments_form', $info, $data, $feed_node, $current_step); }

function sirius_ledger_feed_allocate_payments_form($form, &$form_state, $info, $data, $feed_node, $current_step) {
  $form_state['info'] = $info;
  $form_state['data'] = $data;
  $form_state['feed_node'] = $feed_node;
  $form_state['current_step'] = $current_step;
  $steps = sirius_wizard_steps($info, $data, $feed_node);
  $last_data = sirius_feed_lastdata($info, $feed_node);

  $form['#tree'] = TRUE;
  $form['intro'] = array(
    '#markup' => '<div class="sirius_form_intro">' . 
      t('For each of the accounts included in this spreadsheet, select the payment that will be allocated. The total amount for the payment must equal the amount for the fund included in this upload.') . 
      '</div>',
  );

  $domain_nid = $feed_node->field_sirius_domain['und'][0]['target_id'];
  $employer_nid =  $feed_node->field_grievance_shop['und'][0]['target_id'];
  if (!$employer_nid) {
    drupal_set_message("Please select an employer", 'error');
    return array();
  }

  $accounts = sirius_ledger_account_list();
  foreach ($accounts as $account_nid => $account_name) {
    // Get the list of available payments for this employer and account. SQL for speed
    $sql_args = array();
    $sql = "select nid, title ";
    $sql .= "from node ";
    $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_ledger_account on field_data_field_sirius_ledger_account.entity_type = 'node' and field_data_field_sirius_ledger_account.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_payer on field_data_field_sirius_payer.entity_type = 'node' and field_data_field_sirius_payer.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_payment_status on field_data_field_sirius_payment_status.entity_type = 'node' and field_data_field_sirius_payment_status.entity_id = node.nid ";
    $sql .= "where node.type = 'sirius_payment' ";
    $sql .= "and field_sirius_payment_status_value != 'Cleared' ";
    if ($domain_nid) {
      $sql .= "and field_sirius_domain_target_id = :domain_nid ";
      $sql_args[':domain_nid'] = $domain_nid;
    } else {
      $sql .= "and field_sirius_domain_target_id IS NULL ";
    }
    $sql .= "and field_sirius_ledger_account_target_id = :account_nid ";
    $sql_args[':account_nid'] = $account_nid;
    $sql .= "and field_sirius_payer_target_id = :employer_nid ";
    $sql_args[':employer_nid'] = $employer_nid;
    $sql .= "order by nid desc limit 100 ";
    $stmt = sirius_sql_query($sql, $sql_args);
    $payment_options = array(NULL => '-- Select --');
    while ($hr = $stmt->fetchAssoc()) {
      $payment_options[$hr['nid']] = $hr['title'];
    }

    $form['payments'][$account_nid] = array(
      '#title' => t('Payment for: ') . $account_name,
      '#type' => 'select', 
      '#options' => $payment_options,
      '#default_value' => $data['payments'][$account_nid],
    );
  }

  $form['payment_settings']['ledger_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('Enter payments as of'),
    '#attributes' => array(
      'autocomplete' => 'off',
    ),
    '#description' => t('If there is no payment date in the upload, use this date.'),
  );
  if ($data['payment_settings']['ledger_date']) { $form['payment_settings']['ledger_date']['#default_value'] = $data['payment_settings']['ledger_date']; }

  $form['charge_settings']['monthly'] = array(
    '#type' => 'select',
    '#title' => t('Create monthly charges for all workers on this upload?'),
    '#options' => array(
      NULL => t('-- Select --'),
      'No' => t('No'),
      'Yes' => t('Yes'),
    ),
    '#description' => t('If you select "Yes", then each worker who appears on this workload will also have a monthly charge with the trigger "Payment".'),
    '#id' => 'sirius_ledger_form_charges_monthly',
  );
  if ($data['charge_settings']['monthly']) { $form['charge_settings']['monthly']['#default_value'] = $data['charge_settings']['monthly']; }

  $form['charge_settings']['ledger_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('Enter charges as of'),
    '#states' => array(
      'visible' => array(
        ":input[id=\"sirius_ledger_form_charges_monthly\"]" => array('value' => 'Yes')
      ),
    ),
    '#attributes' => array(
      'autocomplete' => 'off',
    ),
    '#description' => t('If there is no charge date in the upload, use this date.'),
  );
  if ($data['charge_settings']['ledger_date']) { $form['charge_settings']['ledger_date']['#default_value'] = $data['charge_settings']['ledger_date']; }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  return $form;
}

function sirius_ledger_feed_allocate_payments_form_submit($form, &$form_state) {
  $info = $form_state['info'];
  $data = $form_state['data'];
  $feed_node = $form_state['feed_node'];
  $current_step = $form_state['current_step'];
  $data['payments'] = $form_state['values']['payments'];
  $data['payment_settings'] = $form_state['values']['payment_settings'];
  $data['charge_settings'] = $form_state['values']['charge_settings'];

  sirius_wizard_submit($info, $data, $feed_node, $current_step, "Payment selection saved.");
}

function sirius_ledger_feed_allocate_row_verify($feed_node, $info, $data, &$row, $precompute) {
  $pk = $data['pk'];
  if (!$pk) { $pk = 'id'; }

  static $rowcount;
  $rowcount++;
  // if ($rowcount > 1000) { return array('success' => FALSE, 'msg' => 'Discounting subsequent rows.'); }

	// Trim all
	foreach ($row as $key => $value) {
    if (!is_array($value)) { $row[$key] = trim($value); }
	}

  if ($row['ssn']) { $row['ssn'] = sirius_ssn_format($row['ssn']); }

  if ($pk == 'id') {
   if (!preg_match('/^[a-zA-Z0-9_]+$/', $row['id'])) { return array('success' => FALSE, 'msg' => 'Invalid Member ID'); }
  } else if ($pk == 'id3') {
   if (!preg_match('/^[a-zA-Z0-9_]+$/', $row['id3'])) { return array('success' => FALSE, 'msg' => 'Invalid ID3'); }
  } else if ($pk == 'ssn') {
    if (!sirius_ssn_validate($row['ssn'])) { return array('success' => FALSE, 'msg' => 'Invalid SSN'); }
  } else if ($pk == 'ein') {
    if (!$row['ein']) { return array('success' => FALSE, 'msg' => 'No EIN'); }
  } else if ($pk) {
    return array('success' => FALSE, 'msg' => "Don't know how to look up the worker; unknown pk $pk.");
  } else {
    return array('success' => FALSE, 'msg' => "Don't know how to look up the worker; no pk has been set for this upload.");
  }

  $employer_nid = $feed_node->field_grievance_shop['und'][0]['target_id'];
  if (!$employer_nid) { 
    return array('success' => FALSE, 'msg' => 'Employer not found.'); 
  }

  if (!$row['account']) {
    return array('success' => FALSE, 'msg' => "No account specified.");
  }

  $row['amount'] = preg_replace('/^\$/', '', $row['amount']);
  if (!is_numeric($row['amount'])) {
    return array('success' => FALSE, 'msg' => 'Amount is not a numeric value.');
  }

  static $default_payment_ts;
  if (!isset($default_payment_ts)) { $default_payment_ts = strtotime($data['payment_settings']['ledger_date']); }
  $payment_ts = 0;
  if ($row['payment_date']) {
    $payment_ts = strtotime($row['payment_date']);
  } else {
    $payment_ts = $default_payment_ts;
  }
  if ($payment_ts < 1) {
    return array('success' => FALSE, 'msg' => "Invalid or missing payment date.");
  }

  static $default_charge_ts;
  if (!isset($default_charge_ts)) { $default_charge_ts = strtotime($data['charge_settings']['ledger_date']); }
  $charge_ts = 0;
  if ($row['charge_date']) {
    $charge_ts = strtotime($row['charge_date']);
  } else {
    $charge_ts = $default_charge_ts;
  }
  if ($data['charge_settings']['monthly'] == 'Yes') {
    if ($charge_ts < 1) {
      return array('success' => FALSE, 'msg' => "Invalid or missing charge date.");
    }
  }


  // Worker
  if ($precompute) {
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

    // Payment NID
    $payment_nid = $data['payments'][$account_nid];
    if (!$payment_nid) {
      return array('success' => FALSE, 'msg' => 'There is no payment node selected for this account.');
    }
    $row['payment_nid'] = $payment_nid;

  }

  return array('success' => TRUE, 'msg' => 'Looks good.');
}

function sirius_ledger_feed_allocate_row_process($feed_node, $info, $data, $row) {
  // return array('success' => FALSE, 'msg' => 'Disabled');
	$result = sirius_ledger_feed_allocate_row_verify($feed_node, $info, $data, $row, TRUE);
	if (!$result['success']) { return $result; }
  if (!$row['worker_nid']) { return array('success' => FALSE, 'msg' => 'No worker.'); }
  $worker_nid = $row['worker_nid'];
  $ts = $row['ts'];
  $employer_nid = $row['employer_nid'];
  $account_nid = $row['account_nid'];
  $payment_nid = $row['payment_nid'];

  $payment_node = node_load($payment_nid);
  if ($payment_node->field_sirius_payment_status_value['und'][0]['value'] == 'Cleared') { 
    return array('success' => FALSE, 'msg' => "This payment has a status of \"Cleared\". No entries can be created or updated.");
  }
  $memo = sirius_ledger_payment_memo($payment_node);
  if ($row['memo_prefix']) { $memo = $row['memo_prefix'] . ' ' . $memo; }
  if ($row['memo_suffix']) { $memo = $memo . ' ' . $row['memo_suffix']; }
  $memo = substr($memo, 0, 255);

  $amount = $row['amount'];
  $worker_nid = $row['worker_nid'];
  $worker_node = node_load($worker_nid);
  if (!$worker_node) { return array('success' => FALSE, 'msg' => 'Unable to load worker.'); }
  $contact_nid = $worker_node->field_sirius_contact['und'][0]['target_id'];
  if (!$contact_nid) { return array('success' => FALSE, 'msg' => 'There is no contact associated with the worker.'); }

  $entry = array();
  $entry['ledger_amount'] = -1 * $amount;
  $entry['ledger_account'] = $payment_node->field_sirius_ledger_account['und'][0]['target_id'];
  $entry['ledger_status'] = $payment_node->field_sirius_payment_status['und'][0]['value'];
  $entry['ledger_memo'] = $memo;
  $entry['ledger_ts'] = $row['payment_ts'];
  $entry['ledger_reference'] = $payment_node->nid;
  $entry['ledger_participant'] = $contact_nid;
  // static $count;
  // ++$count;
  // if ($count > 10) { return array('success' => FALSE, 'msg' => 'Suppressing.'); }
  // dpm($entry);
  sirius_ledger_ar_insert($entry);

  if ($amount > 0) {
    if ($data['charge_settings']['monthly'] == 'Yes') {
      $charged = sirius_ledger_monthly_charge_worker($worker_nid, 'live', 'payment', $row['charge_ts']);
    }
  }

  if ($charged) {
    return array('success' => TRUE, 'msg' => 'Payment allocated and charge created.');
  } else {
    return array('success' => TRUE, 'msg' => 'Payment allocated.');
  }
}

function sirius_ledger_feed_allocate_preprocess($feed_node, $info, $data) {
  $accounts = sirius_ledger_account_list();
  if ($data['payments']) {
    foreach ($data['payments'] as $account_nid => $payment_nid) {
      if (!$payment_nid) { continue; }
      $payment_node = node_load($payment_nid);
      drupal_set_message("Clearing all previous entries for: " . $payment_node->title);
      sirius_ledger_ar_delete_by_reference($payment_nid);
    }
  }
}

function sirius_ledger_feed_allocate_postprocess($feed_node, $info, $data) {
  $accounts = sirius_ledger_account_list();
  if ($data['payments']) {
    foreach ($data['payments'] as $account_nid => $payment_nid) {
      if (!$payment_nid) { continue; }
      $payment_node = node_load($payment_nid);
      if (!$payment_node) { continue; }
      drupal_set_message("Finalizing allocation for: " . $payment_node->title);
      $payment_json = sirius_jsonfield_util_node_json_get($payment_node);
      $payment_json['allocate']['feed']['nid'] = $feed_node->nid;
      sirius_jsonfield_util_node_json_set($payment_node, $payment_json);
      $payment_node->field_sirius_ledger_allocated['und'][0]['value'] = 'Yes';
      node_save($payment_node);
    }
  }
}


function sirius_ledger_feed_allocate_review($info, $data, $feed_node, $current_step) { return drupal_get_form('sirius_ledger_feed_allocate_review_form', $info, $data, $feed_node, $current_step); }

function sirius_ledger_feed_allocate_review_form($form, &$form_state, $info, $data, $feed_node, $current_step) {
  $form_state['info'] = $info;
  $form_state['data'] = $data;
  $form_state['feed_node'] = $feed_node;
  $form_state['current_step'] = $current_step;

  $payment_html = '<div class="sirius_ledger_feed_allocate_payments">';
  $accounts = sirius_ledger_account_list();
  foreach ($accounts as $account_nid => $account_name) {
    $payment_nid = $data['payments'][$account_nid];
    $payment_node = node_load($payment_nid);
    if (!$payment_node) { continue; }
    $payment_html .= '<h2>' . $account_name . '</h2>';
    $payment_html .= "Payments were allocated to: " . $payment_node->title;
    $payment_html .= '<br />';
    $allocated_balance = sirius_ledger_ar_fetch(array('ledger_reference' => $payment_nid, 'balance' => TRUE));
    $payment_balance = $payment_node->field_sirius_dollar_amt['und'][0]['value'];
    $payment_html .= "Payment Amount: <strong>" . sirius_ledger_ar_format_balance($payment_balance) . '</strong><br />';
    $payment_html .= "Allocated Amount: <strong>" . sirius_ledger_ar_format_balance($allocated_balance) . '</strong><br />';
    if ($payment_balance == $allocated_balance) {
      $payment_html .= "The amount allocated matches the amount received. This payment may be posted.<br />";
    } else {
      $payment_html .= "The amount allocated does not match the amount received. This payment may not be posted.<br />";
    }
    $payment_html .= l('View Allocation', "node/$payment_nid/sirius_payment_allocate", array('attributes' => array('class' => array('tool-button'))));
    $payment_html .= l('Post', "node/$payment_nid/sirius_payment_post", array('attributes' => array('class' => array('tool-button'))));
  }
  $payment_html .= '</div>';
  $form['payment_html'] = array('#markup' => $payment_html);

  $summary_rows = array();
  foreach ($data['feed']['counts'] as $key => $value) { $summary_rows[] = array($key, $value); }
  $html .= '<h2>' . t('Results') . '</h2>';
  $html .= theme_table(array('rows' => $summary_rows, 'header' => array('Type', 'Count'), 'attributes' => array()));
  $form['review'] = array('#markup' => $html);

  $renderable = field_view_field('node', $feed_node, 'field_sirius_attachments', 'full');
  $form['attachments_intro'] = array('#markup' => '<div>' . t('The first file below contains the original upload data. The second contains the full set of results.') . '</div>');
  $form['attachments'] = array('#markup' => drupal_render($renderable));

  return $form;
}
