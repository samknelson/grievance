<?php

class Sirius_Smf_Report_Arrears extends Report_Cache_Report_Generator {
  public $type = 'sirius-smf-arrears';

  public function info($result = array()) {
    $result = array(
      'name' => t('Worker Arrears'),
      'description' => '',
      'access' => 'sirius trust staff',
      'preview' => array(
        // 'default_display_fields' => array(),
        'preview_links' => TRUE,
        'preview_links' => TRUE,
      ),
      'dashboard' => TRUE,
      'module' => 'sirius_smf',
      'file' => 'reports/arrears.inc',
      'row_encoding' => 'json',
    );

    return parent::info($result);
  }

  public function fields() {
    $fields = array();
    $fields['worker_nid'] = 'NID';
    $fields['worker_name'] = 'Name';

    $cols = sirius_addr_cols();
    foreach ($cols as $col => $title) { $fields["worker_address_$col"] = $title; }

    $fields['status'] = 'Status';
    $fields['msg'] = 'Message';
    $fields['balance'] = 'Balance';
    $fields['date_due'] = 'Payment Due Date';
    $fields['date_cutoff'] = 'Coverage Cutoff Date';

    return $fields;
  }

  public function preview_links($row) {
    return array(
      'node/' . $row['worker_nid'] => 'Worker',
      'node/' . $row['worker_nid'] . '/sirius_worker_ledger' => 'Accounting',
    );
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
        'no_fieldsets' => TRUE,
      )
    );

    if (!$html) { $html = '&nbsp;'; }

    return $html;
  }

  public function pks(&$form_state) {
    $parameters = $form_state['values']['parameters'];

    $search_params = array();
    $search_params['is_worker'] = TRUE;
    $search_params['balance_amount_max'] = -0.01;
    $search_params['limit'] = 100000;
    $records = sirius_ledger_balance_search($search_params);
    $pks = array();
    foreach ($records as $record) {
      $pks[] = $record['worker_nid'];
    }

    return array_chunk($pks, 100);
  }

  public function batch($worker_nids, $values, &$context) {
    $employer_industry_map = &drupal_static(__FUNCTION__);

    $benefit_types = sirius_taxonomy_load('sirius_trust_benefit_type', 'tid', 'full');
    $benefits = sirius_trust_benefits();
    $eligible_benefit_nids = array();
    foreach ($benefits as $benefit_nid => $benefit_node) {
      $type_tid = $benefit_node->field_sirius_trust_benefit_type['und'][0]['tid'];
      $type_term = $benefit_types[$type_tid];
      $type_id = $type_term->field_sirius_id['und'][0]['value'];

      if (in_array($type_id, array('MEDICAL', 'DENTAL'))) {
        $eligible_benefit_nids[] = $benefit_nid;
      }
    }

    $rows = array();
    foreach ($worker_nids as $worker_nid) {
      $row = array();

      $worker_node = node_load($worker_nid);

      // Get the election
      $election_node = sirius_trust_worker_election($worker_node);
      if (!$election_node) { continue; }

      $employer_node = node_load($election_node->field_grievance_shop['und'][0]['target_id']);

      $policy_node = sirius_ledger_policy_lookup(NULL, $employer_node);

      // Which benefit are we checking?
      $benefit_nids = sirius_fieldvals($election_node, 'field_sirius_trust_benefits', 'target_id');
      $benefit_nids = array_intersect($benefit_nids, $eligible_benefit_nids);
      if (!$benefit_nids) { continue; }
      $benefit_nid = $benefit_nids[0];
      if (!$benefit_nid) { continue; }
      $benefit_node = node_load($benefit_nid);

      // Check arrears
      $elig_plugin_info = sirius_trust_eligibility_plugin_info('arrears');

      // $all_plugins = sirius_trust_eligibility_plugins_enabled($policy_node, $benefit_node);
      // dpm($all_plugins);
      // continue;

      $settings = sirius_trust_eligibility_plugin_settings($policy_node, $benefit_node, 'arrears');

      $params = array();
      $params['employer_node'] = $employer_node;
      $params['policy_node'] = $policy_node;
      $params['subscriber_worker_node'] = $worker_node;
      $params['is_election'] = TRUE;
      $result = $elig_plugin_info['instance']->check($settings, $params);
      if (!$result['success']) {
        $status = 'suspended';
        $date = $result['date1'];
      }
      else if ($result['warning']) {
        $status = 'warning';
        $date = $result['date2'];
      }
      else { continue; }

      $msg = $result['msg'];

      $row['worker_nid'] = $worker_nid;
      $row['worker_name'] = $worker_node->title;
      $row['worker_phone'] = $worker_node->field_sirius_phone['und'][0]['value'];
      $row['worker_email'] = $worker_node->field_sirius_email['und'][0]['value'];

      $cols = sirius_addr_cols();
      foreach ($cols as $col => $title) {
        $row["worker_address_$col"] = $worker_node->field_sirius_address['und'][0][$col];
      }

      $balance = sirius_ledger_balance_fetch($worker_node->field_sirius_contact['und'][0]['target_id'], $settings['account']);

      $row['status'] = $status;
      $row['msg'] = $msg;
      $row['balance'] = $balance;
      $row['date_cutoff'] = date('m/d/Y', strtotime($date));
      $row['date_due'] = date('m/d/Y', strtotime('-1 day', strtotime($date)));

      $rows[$worker_nid] = $row;
    }

    return array('is_multiple' => TRUE, 'rows' => $rows);
  }
}
