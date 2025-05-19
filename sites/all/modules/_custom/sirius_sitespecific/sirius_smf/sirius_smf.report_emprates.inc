<?php

class Sirius_Smf_Report_Emprates extends Report_Cache_Report_Generator {
  public $type = 'sirius-smf-emprates';

  public function info($result = array()) {
    $result = array(
      'name' => t('Employer Rates'),
      'description' => '',
      'access' => 'sirius trust staff',
      'preview' => array(
        'default_display_fields' => array(
          'employer_title',
          'policy_title',
          'rate',
          'plugin_0_title',
          'plugin_0_rate',
          'plugin_0_date',
          'plugin_1_title',
          'plugin_1_rate',
          'plugin_1_date',
          'plugin_2_title',
          'plugin_2_rate',
          'plugin_2_date',
        ),
        'preview_links' => TRUE,
      ),
      'dashboard' => TRUE,
      'module' => 'sirius_smf',
      'file' => 'sirius_smf.report_emprates.inc',
      'row_encoding' => 'json',
    );

    return parent::info($result);
  }

  public function fields() {
    $fields = array();
    $fields['employer_nid'] = 'Employer NID';
    $fields['employer_title'] = 'Employer Name';
    $fields['policy_nid'] = 'Policy NID';
    $fields['policy_title'] = 'Policy Name';
    for ($i=0; $i<4; ++$i) {
      $fields['plugin_' . $i . '_title'] = "Plugin #" . ($i+1);
      $fields['plugin_' . $i . '_rate'] = "Plugin #" . ($i+1) . " Rate";
      $fields['plugin_' . $i . '_date'] = "Plugin #" . ($i+1) . " Effective Date";
      $fields['plugin_' . $i . '_uuid'] = "Plugin #" . ($i+1) . " UUID";
    }
    return $fields;
  }

  public function preview_links($row) {
    $links = array();
    for ($i=0; $i<4; ++$i) {
      $plugin_uuid = $row['plugin_' . $i . '_uuid'];

      if (!$plugin_uuid) { continue; }

      $path = 'node/' . $row['employer_nid'] . '/sirius_employer_ledger/charge_plugin_settings/' . $plugin_uuid;
      $links[$path] = 'Settings #' . ($i+1);
    }
    return $links;
  }

  public function form() {
    $form['asof_date'] = array(
      '#type' => 'date_popup',
      '#title' => t('As Of'),
      '#attributes' => array(
        'autocomplete' => 'off',
      ),
      '#date_format' => 'Y-m-d',
      '#required' => TRUE,
      '#default_value' => date('Y-m-d'),
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
        'no_fieldsets' => TRUE,
      )
    );

    return $html;
  }

  public function pks(&$form_state) {
    return array_keys(sirius_employer_list());
  }

  public function batch($employer_nid, $values, &$context) {
    $employer_node = node_load($employer_nid);

    $row = array();
    $row['employer_nid'] = $employer_nid;
    $row['employer_title'] = $employer_node->title;

    $asof_ts = strtotime($values['parameters']['asof_date']);

    // Build the context
    $context = array();
    $context['employer_node'] = $employer_node;
    $context['ts'] = $asof_ts;
    // $context['trigger'] = 'hourly';

    // Get the policy
    $context['policy_node'] = sirius_ledger_charge_plugin_policy_lookup($context);
    $row['policy_nid'] = $context['policy_node']->nid;
    $row['policy_title'] = $context['policy_node']->title;

    // Get the plugins
    $delta = 0;
    $plugins = sirius_plugin_entity_plugins($context['policy_node'], 'ledger_charge');
    foreach ($plugins as $plugin_uuid => $plugin) {
      $plugin_key = $plugin['plugin'];
      if (!$plugin_key) { continue; }

      $plugin_info = sirius_plugin_info('ledger_charge', $plugin_key);
      if (!$plugin_info) { continue; }

      $context['plugin_uuid'] = $plugin_uuid;

      $row['plugin_' . $delta . '_title'] = $plugin['title'];
      $row['plugin_' . $delta . '_uuid'] = $plugin_uuid;

      $context['employer_settings'] = $plugin_info['instance']->employer_settings($context['employer_node'], $context['plugin_uuid']);

      $history_item = $plugin_info['instance']->util_history_lookup($context['employer_settings']['rates']['history'], $context['ts']);

      if ($history_item) { 
        $row['plugin_' . $delta . '_rate'] = $history_item['rate'];
        $row['plugin_' . $delta . '_date'] = $history_item['date'];
      }

      ++$delta;
    }

    return array('is_multiple' => TRUE, 'rows' => array($employer_nid => $row));
  }
}
