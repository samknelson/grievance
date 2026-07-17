<?php

class Sirius_Ledger_Report_Payments_Without_AR extends Report_Cache_Report_Generator {
  public $type = 'sirius-ledger-payments-without-ar';

  public function info($result = array()) {
    $result = array(
      'name' => t('Payments Without AR'),
      'description' => '',
      'access' => 'sirius ledger admin',
      'preview' => array(
        'preview_links' => TRUE,
      ),
      'dashboard' => TRUE,
      'module' => 'sirius_ledger',
      'file' => 'reports/payments-without-ar.inc',
      'row_encoding' => 'json',
    );

    return parent::info($result);
  }

  public function fields() {

    $fields['payment_nid'] = 'NID';
    $fields['payment_title'] = 'Title';
    $fields['payment_from'] = 'From';
    $fields['payment_to'] = 'To';
    $fields['payment_created'] = 'Date';

    return $fields;
  }

  public function preview_links($row) {
    $context = func_get_arg(1);
    $parameters = $context['values']['parameters'];

    $links = array();

    $payment_nid = $row['payment_nid'];
    $links["node/$payment_nid/sirius_payment_allocate"] = 'View';

    return $links;
  }

  public function form() {
    return $form;
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

    $sql = "select nid from node ";
    $sql .= "left join sirius_ledger_ar on nid = ledger_reference ";
    $sql .= "where type = 'sirius_payment' ";
    $sql .= "and ledger_id is null ";
    $sql .= "group by nid order by nid ";

    $stmt = sirius_sql_query($sql, array());
    $payment_nids = array();
    while ($hr = $stmt->fetchAssoc()) {
      $payment_nids[] = $hr['nid'];
    }

    return array_chunk($payment_nids, 50);
  }

  public function batch($pks, $values, &$context) {
    $parameters = $values['parameters'];

    $rows = array();
    foreach ($pks as $payment_nid) {
      $row = array();

      $payment_node = node_load($payment_nid);

      $row['payment_nid'] = $payment_nid;
      $row['payment_title'] = $payment_node->title;
      $row['payment_from'] = sirius_node_title($payment_node->field_sirius_payer['und'][0]['target_id']);
      $row['payment_to'] = sirius_node_title($payment_node->field_sirius_ledger_account['und'][0]['target_id']);
      $row['payment_created'] = date('Y-m-d h:i a', $payment_node->created);

      $rows[$payment_nid] = $row;
    }

    return array('is_multiple' => TRUE, 'rows' => $rows);
  }

}