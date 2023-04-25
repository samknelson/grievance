<?php

function sirius_trust_report_eligibility_compare_info() {
  $result = array(
    'name' => t('Eligibility - Compare'),
    'description' => '',
    'access' => 'sirius trust staff',
    'fields' => sirius_trust_report_eligibility_compare_fields(),
    'parameters_form_callback' => 'sirius_trust_report_eligibility_compare_form',
    'pk_list_callback' => 'sirius_trust_report_eligibility_compare_pks',
    'batch_callback' => 'sirius_trust_report_eligibility_compare_batch',
    'render_callbacks' => array(
      'Summary' => array(
        'callback' => 'sirius_trust_report_eligibility_compare_summary',
        'csv' => TRUE,
      ),
    ),
    'render_parameters_callback' => 'sirius_trust_report_eligibility_compare_render_parameters',
    'module' => 'sirius_trust',
    'file' => 'sirius_trust.report_eligibility_compare.inc',
    'dashboard' => TRUE,
  );

  return $result;
}

function sirius_trust_report_eligibility_compare_fields() {
  return array(
    'benefit_nid' => 'Benefit NID',
    'benefit_name' => 'Benefit Name',
    'policy_nid' => 'Policy NID',
    'policy_name' => 'Policy Name',
    'subscriber_nid' => 'Subscriber NID',
    'subscriber_name' => 'Subscriber Name',
    'dependent_nid' => 'Dependent NID',
    'dependent_name' => 'Dependent Name',
    'reltype_tid' => 'Relationship TID',
    'reltype_name' => 'Relationship Type Name',
  );
}

function sirius_trust_report_eligibility_compare_form($report_type) {
  $form['asof'] = array(
    '#title' => t('As Of'),
    '#type' => 'date_popup',
    '#attributes' => array(
      'autocomplete' => 'off',
    ),
    '#required' => TRUE,
    '#date_format' => 'Y-m-d',
    '#default_value' => date('Y-m-d'),
  );

  $benefit_options = sirius_trust_benefit_options(FALSE);
  if (!$benefit_options) { return sirius_form_error('No benefits have been defined.'); }
  $form['benefit_nid'] = array(
    '#title' => t('Benefit'),
    '#type' => 'select',
    '#options' => array(NULL => t('-- Select --')) + $benefit_options,
    '#required' => TRUE,
  );

  $policy_options = sirius_ledger_policy_options();
  if (!$policy_options) { return sirius_form_error('No policies have been defined.'); }
  for ($i=0; $i<5; ++$i) {
    $form["policy_{$i}_nid"] = array(
      '#title' => t('Policy #:i', array(':i' => ($i+1))),
      '#type' => 'select',
      '#options' => array(NULL => t('-- Select --')) + $policy_options,
    );
  }

  $ms_options = sirius_taxonomy_load('sirius_member_status', 'tid', 'name');
  $ws_options = sirius_taxonomy_load('sirius_work_status', 'tid', 'name');

  $form['limits'] = array(
    '#type' => 'fieldset',
    '#title' => t('Limits'),
    '#description' => '<p>' . t('Only check workers who match the following criteria.') . '</p>' . 
      '<p><em>' . t('Note: These limits apply only to the workers\' <strong>current</strong> status.') . '</em></p>',
  );

  $form['limits']['ms'] = array(
    '#type' => 'checkboxes',
    '#options' => $ms_options,
    '#title' => t('Member Status'),
    '#checkall' => TRUE,
  );

  $form['limits']['ws'] = array(
    '#type' => 'checkboxes',
    '#options' => $ws_options,
    '#title' => t('Work Status'),
    '#checkall' => TRUE,
  );
  return $form;
}

function sirius_trust_report_eligibility_compare_pks(&$form_state, $report_type, $report_id) {
  $domain_nid = sirius_domain_nid();
  $sql_args = array();
  // $sql_args[':asof'] = $asof;

  if ($form_state['values']['parameters']['limits']['ms']) { $ms = array_filter(array_values($form_state['values']['parameters']['limits']['ms'])); }
  if ($form_state['values']['parameters']['limits']['ws']) { $ws = array_filter(array_values($form_state['values']['parameters']['limits']['ws'])); }

  $sql = "select distinct nid from node ";
  $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_member_status on field_data_field_sirius_member_status.entity_type = 'node' and field_data_field_sirius_member_status.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_work_status on field_data_field_sirius_work_status.entity_type = 'node' and field_data_field_sirius_work_status.entity_id = node.nid ";
  $sql .= "where type = 'sirius_worker' ";
  if ($domain_nid) { 
    $sql .= "and field_sirius_domain_target_id = :domain_nid ";
    $sql_args[':domain_nid'] = $domain_nid;
  } else {
    $sql .= "and field_sirius_domain_target_id is NULL ";
  }
  if ($ms) {
    $sql .= "and field_sirius_member_status_tid in (:ms) ";
    $sql_args[':ms'] = $ms;
  }
  if ($ws) {
    $sql .= "and field_sirius_work_status_tid in (:ws) ";
    $sql_args[':ws'] = $ws;
  }
  $sql .= "order by nid desc ";
  // $sql .= "limit 10 ";

  $stmt = sirius_sql_query($sql, $sql_args);
  $worker_nids = array();
  while ($hr = $stmt->fetchAssoc()) {
    $worker_nids[] = $hr['nid'];
  }
  return array_chunk($worker_nids, 100, TRUE);
}

function sirius_trust_report_eligibility_compare_batch($worker_nids, $values, $report_id, $report_action, &$context) {
  $rows = array('is_multiple' => TRUE, 'rows' => array());

  $policy_nids = array();
  for ($i=0; $i<10; ++$i) {
    $policy_nid = $values['parameters']["policy_{$i}_nid"];
    if ($policy_nid) { $policy_nids[] = $policy_nid; }
  }
  $policy_nodes = node_load_multiple($policy_nids);

  $benefit_nid = $values['parameters']['benefit_nid'];
  $benefit_node = node_load($benefit_nid);

  $asof = $values['parameters']['asof'];
  if (!$asof) { $asof = date('Y-m-d'); }
  $ts = strtotime($asof);

  $reltype_names = sirius_taxonomy_load('sirius_contact_relationship_types', 'tid', 'name');

  $subscriber_nodes = node_load_multiple($worker_nids);

  foreach ($subscriber_nodes as $subscriber_nid => $subscriber_node) {
    $params = array();
    $params['contact_nid'] = $subscriber_node->field_sirius_contact['und'][0]['target_id'];
    if (!$params['contact_nid']) { continue; }
    $relationship_nodes = sirius_contact_relationships($params);
    $relationship_nodes = array(NULL => NULL) + $relationship_nodes;

    foreach ($policy_nodes as $policy_nid => $policy_node) {
      $result = sirius_trust_eligibility_plugin_check_all($policy_node, $benefit_node, $subscriber_node, $relationship_node, $ts, TRUE);
      if (!$result['success']) { continue; }

      if ($relationship_node) {
        $dependent_contact_nid = $relationship_node->field_sirius_contact_alt['und'][0]['target_id'];
        $dependent_node = sirius_worker_get_by_contact_nid($dependent_contact_nid);
        $reltype_tid = $relationship_node->field_sirius_contact_reltype['und'][0]['tid'];
        $reltype_name = $reltype_names[$reltype_tid];
      } else {
        $dependent_node = $subscriber_node;
        $reltype_tid = NULL;
        $reltype_name = '[self]';
      }

      $row = array();
      $pk = $subscriber_node->nid;
      $pk .= '|';
      $pk .= $policy_node->nid;
      $pk .= '|';
      if ($relationship_node) {
        $pk .= $relationship_node->nid;
      } else {
        $pk .= 'SELF';
      }

      $row['benefit_nid'] = $benefit_node->nid;
      $row['benefit_name'] = $benefit_node->title;
      $row['policy_nid'] = $policy_node->nid;
      $row['policy_name'] = $policy_node->title;
      $row['subscriber_nid'] = $subscriber_node->nid;
      $row['subscriber_name'] = $subscriber_node->title;
      $row['dependent_nid'] = $dependent_node->nid;
      $row['dependent_name'] = $dependent_node->nid;
      $row['reltype_tid'] = $reltype_tid;
      $row['reltype_name'] = $reltype_name;
      $rows['rows'][$pk] = $row;
    }
  }

  return $rows;
}

function sirius_trust_report_eligibility_compare_view_form($form, &$form_state) {
  $form['#method'] = 'get';

  foreach (array('report_id', 'action', 'callback-name') as $key) {
    $form[$key] = array(
      '#type' => 'hidden',
      '#value' => $_REQUEST[$key],
    );
  }

  $key_names = array(
    'benefit_name' => 'Benefit',
    'employer_name' => 'Employer',
    'relationship_type_name' => 'Relationship',
  );

  for ($i=1; $i<=3; ++$i) {
    $form["key$i"] = array(
      '#title' => "Breakout Field $i",
      '#type' => 'select',
      '#options' => array(NULL => t('-- Select --')) + $key_names,
      '#default_value' => $_GET["key$i"],
      '#prefix' => '<div class="container-inline">',
      '#suffix' => '</div>',
    );
  }

  $form["output"] = array(
    '#title' => "Output",
    '#type' => 'select',
    '#options' => array(
      NULL => t('-- Select --'),
      'screen' => t('Screen'),
      'csv' => t('CSV')
    ),
    '#default_value' => $_GET['output'],
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

function sirius_trust_report_eligibility_compare_render_parameters($parameters) {
  $parameters = $parameters['parameters'];
  $benefit_nid = $parameters['benefit_nid'];
  $benefit_node = node_load($benefit_nid);
  $html .= sirius_fakerender_field('Benefit:', $benefit_node->title);

  $asof = $parameters['asof'];
  $html .= sirius_fakerender_field('As Of:', $asof);

  for ($i=0; $i<10; ++$i) {
    $policy_nid = $parameters["policy_{$i}_nid"];
    if ($policy_nid) {
      $val = sirius_node_title($policy_nid);
      $val .= ' ';
      $val .= '[' . l('View', "node/$policy_nid/sirius_trust_policy_benefits") . ']';
      $html .= sirius_fakerender_field('Policy #' . ($i+1) . ':', $val);
    }
  }
  return $html;
}

function sirius_trust_report_eligibility_compare_summary($report_type, $report_id, $output = 'page') {
  $info = report_cache_info($report_type);

  $query = db_select('report_cache');
  $query->fields('report_cache');
  $query->condition('report_id', $report_id);
  $query->condition('report_type', $report_type);
  $stmt = $query->execute();

  $data = array();
  while ($hr = $stmt->fetchAssoc()) {
    $hr = unserialize($hr['report_data']);

    $policy_name = $hr['policy_name'];
    $subscriber_nid = $hr['subscriber_nid'];
    $dependent_nid = $hr['dependent_nid'];
    $reltype_name = $hr['reltype_name'];

    if ($reltype_name == '[self]') {
      ++$totals['subscribers'];
      ++$data[$policy_name]['subscribers'];
    } 

    ++$data[$policy_name]['covered'];
    ++$totals['covered'];
  }

  $header = array();
  $header[] = t('Policy');
  $header[] = t('Eligible Subscribers');
  $header[] = t('Eligible Covered Lives');

  $rows = array();
  foreach ($data as $policy_name => $record) {
    $row = array();
    $row[] = $policy_name;
    $row[] = sirius_number_format($record['subscribers'], 0, '.', ',', $output);
    $row[] = sirius_number_format($record['covered'], 0, '.', ',', $output);
    $rows[] = $row;
  }

  // Totals are silly here...
  // $row = array();
  // $row[] = '<strong>' . t('Total') . '</strong>';
  // $row[] = '<strong>' . sirius_number_format($totals['subscribers'], 0, '.', ',', $output) . '</strong>';
  // $row[] = '<strong>' . sirius_number_format($totals['covered'], 0, '.', ',', $output) . '</strong>';
  // $rows[] = $row;

  if ($output == 'csv') { return array('header' => $header, 'rows' => $rows); }
  $attributes = array();
  $html .= theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => $attributes));
  return $html;
}
