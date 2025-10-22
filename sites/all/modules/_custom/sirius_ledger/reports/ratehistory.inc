<?php

class Sirius_Ledger_Report_Ratehistory extends Report_Cache_Report_Generator {
  public $type = 'sirius-ledger-ratehistory';

  public function info($result = array()) {
    $result = array(
      'name' => t('Rate History'),
      'description' => '',
      'access' => 'sirius ledger admin',
      'preview' => array(
        'preview_links' => TRUE,
      ),
      'dashboard' => TRUE,
      'module' => 'sirius_ledger',
      'file' => 'reports/ratehistory.inc',
      'row_encoding' => 'json',
    );

    return parent::info($result);
  }

  public function fields() {
    $fields['employer_nid'] = 'Employer NID';
    $fields['employer_id'] = 'Employer ID';
    $fields['employer_name'] = 'Employer Name';
    $fields['rate_date'] = 'Date';
    $fields['rate'] = 'Rate';

    return $fields;
  }

  public function preview_links($row) {
    $context = func_get_arg(1);
    $parameters = $context['values']['parameters'];

    $links = array();

    $account_nid = $parameters['account_nid'];
    $participant_nid = $row['employer_nid'];
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
    return array_keys(sirius_employer_list());
  }

  public function batch($employer_nid, $values, &$context) {
    static $count;
    if ($count > 5) { return array(); }

    $account_nid = $values['parameters']['account_nid'];
    if (!$account_nid) { return array(); }

    $employer_node = node_load($employer_nid);
    $policy_node = sirius_ledger_policy_lookup(NULL, $employer_node);
    if (!$policy_node) { return array(); }

    $items = sirius_plugin_entity_plugins($policy_node, 'ledger_charge');
    if (!$items) { return array(); }

    $rows = array();
    foreach ($items as $plugin_uuid => $item) {
      $info = sirius_plugin_info('ledger_charge', $item['plugin']);

      if (!$info) { continue; }
      if (!$info['instance']) { continue; }
      if (!$info['instance']->employer_settings_enabled()) { continue; }

      $policy_settings = $info['instance']->entity_settings($policy_node, $plugin_uuid);
      if ($policy_settings['account'] != $account_nid) { continue; }

      $employer_settings = $info['instance']->employer_settings($employer_node, $plugin_uuid);
      if (!$employer_settings) { return array(); }
      if (!$employer_settings['rates']['history']) { continue; }

      foreach ($employer_settings['rates']['history'] as $delta => $rate) {
        $pk = $employer_nid;
        $pk .= '|';
        $pk .= $plugin_uuid;
        $pk .= '|';
        $pk .= $delta;

        $row = array();
        $row['employer_nid'] = $employer_nid;
        $row['employer_name'] = $employer_node->title;
        $row['employer_id'] = $employer_node->field_sirius_id['und'][0]['value'];
        $row['rate_date'] = $rate['date'];
        $row['rate'] = $rate['rate'];
        $rows[$pk] = $row;
      }
    }

    return array('is_multiple' => TRUE, 'rows' => $rows);
  }

}