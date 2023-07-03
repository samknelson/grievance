<?php

/**
 * @todo: I don't know if we need this report or not.
 *        But ... we certainly need the function sirius_trust_report_worker_benefit_query().
 *        So don't delete the file until we're sure. 
 */

function sirius_trust_report_worker_benefit_detail_info() {
  $result = array(
    'name' => t('Coverage - Detail'),
    'description' => '',
    'access' => 'sirius trust staff',
    'fields' => sirius_trust_report_worker_benefit_detail_fields(),
    'form_callback' => 'sirius_trust_report_worker_benefit_detail_form',
    'pk_list_callback' => 'sirius_trust_report_worker_benefit_detail_pks',
    'batch_callback' => 'sirius_trust_report_worker_benefit_detail_batch',
    'render_callbacks' => array(
      'View' => 'sirius_trust_report_worker_benefit_detail_view',
    ),
    'module' => 'sirius_trust',
    'file' => 'sirius_trust.report_worker_benefit_detail.inc',
    'dashboard' => TRUE,
  );

  return $result;
}

function sirius_trust_report_worker_benefit_detail_fields() {
  return array(
    'wb_nid' => 'Coverage NID',
    'subscriber_nid' => 'Subscriber NID',
  );
}

function sirius_trust_report_worker_benefit_detail_form($form, &$form_state, $report_type) {
  report_cache_form_prepare($report_type, $form, $form_state);

  $form['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('Options'),
    '#tree' => TRUE,
  );

  $form['general']['asof'] = array(
    '#title' => t('As Of'),
    '#type' => 'date_popup',
    '#attributes' => array(
      'autocomplete' => 'off',
    ),
    '#date_format' => 'Y-m-d',
  );

  //////////////////////////////////////////////////
  // Current / Add / Remove
  //////////////////////////////////////////////////

  $form['breakout_car'] = array(
    '#type' => 'fieldset',
    '#title' => t('Filter: Current / Add / Remove'),
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['breakout_car']['days'] = array(
    '#type' => 'textfield',
    '#size' => 4,
    '#title' => t('Days Back'),
    '#default_value' => 7,
  );

  $form['breakout_car']['filter'] = array(
    '#title' => t('Filter'),
    '#type' => 'select',
    '#options' => array(
      'current' => t('Records active as of the "as of" date'),
      'add' => t('Records added within "days back" days before the "as of" date'),
      'remove' => t('Records removed within "days back" days before the "as of" date'),
    ),
  );

  //////////////////////////////////////////////////
  // Relationship Type
  //////////////////////////////////////////////////

  $form['breakout_reltype'] = array(
    '#title' => t('Filter - Relationship Type'),
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['breakout_reltype']['filter'] = array(
    '#title' => t('Filter'),
    '#type' => 'checkboxes',
    '#options' => sirius_taxonomy_load('sirius_contact_relationship_types', 'tid', 'name'),
    '#checkall' => TRUE,
  );

  //////////////////////////////////////////////////
  // Employer
  //////////////////////////////////////////////////

  $form['breakout_employer'] = array(
    '#title' => t('Filter - Employer'),
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['breakout_employer']['filter'] = array(
    '#title' => t('Filter'),
    '#type' => 'checkboxes',
    '#options' => sirius_employer_list(),
    '#checkall' => TRUE,
  );

  //////////////////////////////////////////////////
  // Benefit
  //////////////////////////////////////////////////

  $form['breakout_benefit'] = array(
    '#title' => t('Filter - Benefit'),
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['breakout_benefit']['filter'] = array(
    '#title' => t('Filter'),
    '#type' => 'checkboxes',
    '#options' => sirius_trust_benefit_options(),
    '#checkall' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Generate',
  );

  return $form;
}

function sirius_trust_report_worker_benefit_detail_form_validate($form, &$form_state) { 
}

function sirius_trust_report_worker_benefit_detail_form_submit($form, &$form_state) {
  if (!$form_state['values']['general']['asof']) { $form_state['values']['general']['asof'] = date('Y-m-d'); }
  return report_cache_form_submit($form, $form_state);
}

function sirius_trust_report_worker_benefit_detail_pks(&$form_state) {
  $days = $form_state['values']['breakout_car']['days'];
  $asof = $form_state['values']['general']['asof'];
  $keys = array();

  if ($form_state['values']['breakout_car']['filter']) {
    $car = $form_state['values']['breakout_car']['filter'];
    $keys['car'] = $car;
  }

  if ($form_state['values']['breakout_reltype']['filter']) {
    $reltypes = array_filter(array_values($form_state['values']['breakout_reltype']['filter']));
    if ($reltypes) { $keys['reltypes'] = $reltypes; }
  }

  if ($form_state['values']['breakout_employer']['filter']) {
    $employers = array_filter(array_values($form_state['values']['breakout_employer']['filter']));
    if ($employers) { $keys['employers'] = $employers; }
  }

  if ($form_state['values']['breakout_benefit']['filter']) {
    $benefits = array_filter(array_values($form_state['values']['breakout_benefit']['filter']));
    if ($benefits) { $keys['benefits'] = $benefits; }
  }

  $stmt = sirius_trust_report_worker_benefit_query($asof, $days, $keys, FALSE);
  $wb_nids = array();
  while ($hr = $stmt->fetchAssoc()) { $wb_nids[] = $hr['nid']; }
  return array_chunk($wb_nids, 100, TRUE);
}

function sirius_trust_report_worker_benefit_detail_batch($wb_nids, $values, $report_id, $report_action, &$context) {
  foreach ($wb_nids as $wb_nid) {
    $wb_node = node_load($wb_nid);
    $dependent_nid = node_load($wb_node->field_sirius_worker['und'][0]['target_id']);
    if ($wb_node->field_sirius_contact_relation['und'][0]['target_id']) {
      // $wb_node = 
    }
  }
  dpm($wb_nids);
  return array();

  $row = array();
  $row['key'] = $key;

  $breakout_options = array_keys(sirius_trust_report_worker_benefit_detail_breakout_options());
  foreach ($breakout_options as $breakout_option) { $row[$breakout_option] = $keys[$breakout_option]; }
  if ($keys['employer']) { $row['employer_name'] = sirius_node_title($keys['employer']); }
  if ($keys['benefit']) { $row['benefit_name'] = sirius_node_title($keys['benefit']); }
  if ($keys['reltype']) { $term = taxonomy_term_load($keys['reltype']); $row['reltype_name'] = $term->name; }
  if ($keys['car']) { $row['car_name'] = $keys['car']; }

  $days = $values['breakout_car']['days'];
  $asof = $values['general']['asof'];

  $stmt = sirius_trust_report_worker_benefit_query($asof, $days, $keys);
  $hr = $stmt->fetchAssoc();

  if (!$hr['record_count']) { return NULL; }
  $row['record_count'] = $hr['record_count'];
  $row['subscriber_count'] = $hr['subscriber_count'];
  $row['dependent_count'] = $hr['dependent_count'];

  return $row;
}

function sirius_trust_report_worker_benefit_detail_view($report_type, $report_id) {
  $info = report_cache_info($report_type);
  $parameters = report_cache_parameters($report_type, $report_id);
  $breakouts = sirius_trust_report_worker_benefit_detail_breakouts($parameters);

  $query = db_select('report_cache');
  $query->fields('report_cache');
  $query->condition('report_id', $report_id);
  $query->condition('report_type', $report_type);
  $stmt = $query->execute();
  $data = array();
  while ($hr = $stmt->fetchAssoc()) {
    $data[] = unserialize($hr['report_data']);
  }

  usort($data, function($a, $b) use ($breakouts) {
    foreach ($breakouts as $breakout) {
      $a_name = $a[$breakout . "_name"];
      $b_name = $b[$breakout . "_name"];
      if ($a_name == $b_name) { continue; }
      if (!$a_name) { return 1; }
      if (!$b_name) { return -1; }
      return strcmp($a_name, $b_name);
    }
  });

  foreach ($data as $hr) {
    $row = array();
    foreach ($breakouts as $breakout) {
      $name = $hr[$breakout . "_name"];
      if (!$name) {
        if ($breakout == 'car') { $name = '[CURRENT]'; } else { $name = 'TOTAL'; }
      }
      $row[] = $name;
    }
    $row[] = $hr['record_count'];
    $row[] = $hr['subscriber_count'];
    $row[] = $hr['dependent_count'];
    $rows[] = $row;
  }

  $breakout_options = sirius_trust_report_worker_benefit_detail_breakout_options();
  foreach ($breakouts as $breakout) {
    $header[] = $breakout_options[$breakout];
  }
  $header[] = 'Record Count';
  $header[] = 'Subscriber Count';
  $header[] = 'Dependent Count';

  if ($_REQUEST['output'] == 'csv') {
    $filename = "${report_type}_";
    $r = $report_id;
    $r = preg_replace('/[^a-zA-Z0-9_ ]/', '', $r);
    $r = preg_replace('/ +/', '_', $r);
    $filename .= $r;
    $filename .= ".csv";

    drupal_add_http_header('Content-Type', 'text/csv');
    drupal_add_http_header('Content-Disposition', 'attachment;filename=' . $filename);
    $fp = fopen('php://output', 'w');
    fputcsv($fp, $header);
    foreach ($rows as $row) {
      foreach ($row as $delta => $val) { $row[$delta] = strip_tags($val); }
      fputcsv($fp, $row);
    }
    drupal_exit();
  }

  if ($parameters['general']['asof']) {
    $html .= '<h2>' . t('Data as of: ') . $parameters['general']['asof'] . '</h2>';
  }
  $attributes = array();

  $html .= theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => $attributes));
  return $html;
}
