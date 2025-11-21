<?php

class Sirius_Ledger_Report_Aged extends Report_Cache_Report_Generator {
  public $type = 'sirius-ledger-aged';

  public function info($result = array()) {
    $result = array(
      'name' => t('Aged'),
      'description' => '',
      'access' => 'sirius ledger admin',
      'preview' => array(
        'preview_links' => TRUE,
      ),
      'dashboard' => TRUE,
      'module' => 'sirius_ledger',
      'file' => 'reports/aged.inc',
      'row_encoding' => 'json',
    );

    return parent::info($result);
  }

  public function fields() {
    return array(
      'participant_nid' => t('Participant NID'),
      'participant_name' => t('Participant Name'),
      'balance_0' => t('Balance'),
      'balance_30' => t('Bal. 30'),
      'pay_30' => t('Pay. 30'),
      'outstanding_30' => t('Outstanding 30 days'),
      'balance_60' => t('Bal. 60'),
      'pay_60' => t('Pay. 60'),
      'outstanding_60' => t('Outstanding 60 days'),
      'balance_90' => t('Bal. 90'),
      'pay_90' => t('Pay. 90'),
      'outstanding_90' => t('Outstanding 90 days'),
      'balance_120' => t('Bal. 120'),
      'pay_120' => t('Pay. 120'),
      'outstanding_120' => t('Outstanding 120 days'),
    );
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

    return $form;
  }

  public function form_validate($form, &$form_state) {
    $parameters = $form_state['values']['parameters'];
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

    return $participant_nids;
  }

  public function batch($participant_nid, $values, &$context) {
    $parameters = $values['parameters'];
    $account_nid = $parameters['account_nid'];

    $row = array();
    foreach (array(120, 90, 60, 30, 0) as $days_ago) {
      $row['account_nid'] = $account_nid;
      $row['account_name'] = sirius_node_title($account_nid);
      $row['participant_nid'] = $participant_nid;
      $row['participant_name'] = sirius_node_title($participant_nid);

      $sql_args = array();
      $sql_args[':participant_nid'] = $participant_nid;
      $sql_args[':account_nid'] = $account_nid;
      $sql_args[':ago'] = strtotime("-$days_ago days");

      $sql = "select sum(ledger_amount) as a from sirius_ledger_ar ";
      $sql .= "where ledger_ts <= :ago ";
      $sql .= "and ledger_account = :account_nid ";
      $sql .= "and ledger_participant = :participant_nid ";
      $stmt = sirius_sql_query($sql, $sql_args);
      $hr = $stmt->fetchAssoc();
      $row["balance_$days_ago"] = round($hr['a'], 2);

      $sql = "select sum(ledger_amount) as a from sirius_ledger_ar ";
      $sql .= "where ledger_ts > :ago ";
      $sql .= "and ledger_account = :account_nid ";
      $sql .= "and ledger_participant = :participant_nid ";
      $sql .= "and ledger_amount < 0 ";
      $stmt = sirius_sql_query($sql, $sql_args);

      $hr = $stmt->fetchAssoc();
      $row["pay_$days_ago"] = round($hr['a'], 2);

      $row["outstanding_$days_ago"] = $row["balance_$days_ago"] + $row["pay_$days_ago"];

      if ($row["outstanding_$days_ago"] < 0) { $row["outstanding_$days_ago"] = 0; }
      $row["outstanding_$days_ago"] = '$' . number_format($row["outstanding_$days_ago"], 2);

      $row["pay_$days_ago"] = '$' . number_format($row["pay_$days_ago"], 2);

      if ($row["balance_$days_ago"] < 0) { $row["balance_$days_ago"] = 0; }
      $row["balance_$days_ago"] = '$' . number_format($row["balance_$days_ago"], 2);

    }

    return $row;
  }

}