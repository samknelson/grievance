<?php

class Sirius_Ledger_Report_Fourmonth extends Report_Cache_Report_Generator {
  public $type = 'sirius-ledger-fourmonth';

  public function info($result = array()) {
    $result = array(
      'name' => t('Four Month'),
      'description' => '',
      'access' => 'sirius ledger admin',
      'preview' => array(
        'preview_links' => TRUE,
      ),
      'dashboard' => TRUE,
      'module' => 'sirius_ledger',
      'file' => 'reports/fourmonth.inc',
      'row_encoding' => 'json',
    );

    return parent::info($result);
  }

  public function fields() {
    $fields['participant_nid'] = 'Participant NID';
    $fields['participant_name'] = 'Participant Name';

    $fields['balance_0'] = 'Balance This Month';
    $fields['payments_0'] = 'Payments This Month';
    $fields['charges_0'] = 'Charges This Month';

    $fields['balance_1'] = 'Balance 1 Month Prev';
    $fields['payments_1'] = 'Payments 1 Month Prev';
    $fields['charges_1'] = 'Charges 1 Month Prev';

    $fields['balance_2'] = 'Balance 2 Month Prev';
    $fields['payments_2'] = 'Payments 2 Month Prev';
    $fields['charges_2'] = 'Charges 2 Month Prev';

    $fields['balance_3'] = 'Balance 3 Month Prev';
    $fields['payments_3'] = 'Payments 3 Month Prev';
    $fields['charges_3'] = 'Charges 3 Month Prev';

    $fields['balance_4'] = 'Balance 4 Month Prev';
    $fields['payments_4'] = 'Payments 4 Month Prev';
    $fields['charges_4'] = 'Charges 4 Month Prev';

    $fields['balance_5'] = 'Balance 5 Month Prev';

    $fields['last_payment_date'] = 'Last Payment Date';

    return $fields;
  }

  public function preview_links($row) {
    $context = func_get_arg(1);
    $parameters = $context['values']['parameters'];

    $links = array();

    $account_nid = $parameters['account_nid'];
    $participant_nid = $row['participant_nid'];
    if (!$account_nid || !$participant_nid) { return array(); }

    $ea_instance = Sirius_Ledger_Ea::getInstance();
    $ea_node = $ea_instance->fetch($participant_nid, $account_nid, FALSE);
    if (!$ea_node) { return array(); }
    
    $links['node/' . $ea_node->nid . '/sirius_ea'] = 'View';

    return $links;
  }

  public function form() {
    $accounts = sirius_ledger_account_list();

    $form['account_nid'] = array(
      '#type' => 'select',
      '#title' => t('Account'),
      '#options' => array(NULL => t('-- Select --')) + $accounts,
      '#required' => TRUE,
    );

    $form['year'] = array(
      '#type' => 'textfield',
      '#title' => t('Year'),
      '#size' => 4,
      '#default_value' => date('Y'),
      '#required' => TRUE,
    );

    $form['month'] = array(
      '#title' => t('Month'),
      '#type' => 'textfield',
      '#default_value' => date('m'),
      '#size' => 2,
      '#required' => TRUE,
    );

    return $form;
  }

  public function form_validate($form, &$form_state) {
    $parameters = $form_state['values']['parameters'];

    $year = $parameters['year'];
    $month = $parameters['month'];

    if (!$year || 
      $year != intval($year) ||
      $year <= 1900 || 
      $year >= 2100
    ) {
      form_set_error("parameters][year", 'Please enter a year.');
    }

    if (!$month || 
      $month != intval($month) ||
      $month < 1 || 
      $month > 12
    ) {
      form_set_error("parameters][month", 'Please enter a month.');
    }
  }

  public function render_parameters($parameters) {
    $parameters = $parameters['parameters'];

    $html = sirius_jsonfield_util_render_all_values_new(
      $this->form(), 
      $parameters, 
      array(
        'skip_empty' => TRUE,
        'fieldset_title_prefix' => '<h2>',
        'fieldset_title_suffix' => '</h2>',
        'no_fieldsets' => TRUE,
      )
    );
    return $html;
  }

  public function pks(&$form_state) {
    $parameters = $form_state['values']['parameters'];
    $account_nid = $parameters['account_nid'];

    $sql = "select distinct ledger_participant from sirius_ledger_ar where ledger_account = :account_nid order by ledger_participant ";
    $stmt = sirius_sql_query($sql, array(':account_nid' => $parameters['account_nid']));
    $participant_nids = array();
    while ($hr = $stmt->fetchAssoc()) {
      $participant_nids[] = $hr['ledger_participant'];
    }

    return array_chunk($participant_nids, 5);
  }

  public function batch($pks, $values, &$context) {
    $parameters = $values['parameters'];
    $account_nid = $parameters['account_nid'];
    $year = $parameters['year'];
    $month = $parameters['month'];

    $invoice_instance = Sirius_Ledger_Invoices::getInstance();

    $types = sirius_taxonomy_load('sirius_ledger_type', 'tid', 'full');

    $intervals = array();
    $interval = $invoice_instance->interval_from_ts(strtotime("$year-$month-15"));
    for ($i=0; $i<6; ++$i) {
      $intervals[] = $interval;
      $interval = $invoice_instance->prev_interval($interval);
    }
    $rows = array();
    foreach ($pks as $participant_nid) {
      $row = array();

      $row['participant_nid'] = $participant_nid;
      $row['participant_name'] = sirius_node_title($participant_nid);

      for ($i=0; $i<6; ++$i) {
        $invoice = $invoice_instance->invoice_load($participant_nid, $account_nid, $intervals[$i]);
        $row["balance_$i"] = $invoice['balances']['balance_out'];
        $row["payments_$i"] = $invoice['balances']['payments'];
        $row["charges_$i"] = $invoice['balances']['charges'];
      }

      $params['ledger_participant'] = $participant_nid;
      $params['ledger_account'] = $account_nid;
      $params['ledger_amount_max'] = 0;
      $params['balance'] = FALSE;
      $params['limit'] = 1;
      $result = sirius_ledger_ar_fetch($params);
      if ($result[0]) {
        $ts = $result[0]['ledger_ts'];
        $row['last_payment_date'] = date('Y-m-d', $ts);
      }

      $rows[$participant_nid] = $row;
    }

    return array('is_multiple' => TRUE, 'rows' => $rows);
  }

}