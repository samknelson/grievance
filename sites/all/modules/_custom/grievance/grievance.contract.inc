<?php

function grievance_contract_template_node_tab_rules($contract_template_node) {
  drupal_set_title($contract_template_node->title . ': ' . t('Rules'));
  $entity_nid = $contract_template_node->nid;

  $result = views_embed_view('grievance_contract_rules', 'block', $entity_nid);
  $result .= grievance_node_tab_get_addlink("grievance-contract-rule", array("grievance_contract_tplt" => $entity_nid));

  return $result;
}

function grievance_contract_template_node_tab_notifications($contract_template_node) {
  drupal_set_title($contract_template_node->title . ': ' . t('Notifications'));
  $entity_nid = $contract_template_node->nid;

  $result = views_embed_view('grievance_contract_rules', 'block_1', $entity_nid);
  $result .= grievance_node_tab_get_addlink("grievance-contract-rule", array("grievance_contract_tplt" => $entity_nid));

  return $result;
}

function grievance_contract_template_node_tab_clauses($contract_template_node) {
  drupal_set_title($contract_template_node->title . ': ' . t('Clauses'));
  $entity_nid = $contract_template_node->nid;

  $result = views_embed_view('grievance_contract_clauses', 'block', $entity_nid);
  $result .= grievance_node_tab_get_addlink("grievance-contract-clause", array("grievance_contract_tplt" => $entity_nid));

  return $result;
}

function grievance_contract_template_node_tab_sections($contract_template_node) {
  drupal_set_title($contract_template_node->title . ': ' . t('Articles'));
  $entity_nid = $contract_template_node->nid;

  $result = views_embed_view('grievance_contract_sections', 'block', $entity_nid);
  $result .= grievance_node_tab_get_addlink("grievance-contract-section", array("grievance_contract_tplt" => $entity_nid));

  return $result;
}

function grievance_contract_template_node_tab_full($contract_template_node) {
  drupal_set_title($contract_template_node->title . ': ' . t('Full Text'));
  $entity_nid = $contract_template_node->nid;

  $result = views_embed_view('grievance_contract_clauses', 'block_1', $entity_nid);
  $result .= grievance_node_tab_get_addlink("grievance-contract-clause", array("grievance_contract_tplt" => $entity_nid));

  return $result;
}

function grievance_contract_template_node_tab_pdf($contract_template_node) {
  drupal_set_title($contract_template_node->title . ': ' . t('PDF'));
  $entity_nid = $contract_template_node->nid;

  $result = '';

  $uri = $contract_template_node->field_grievance_contract['und'][0]['uri'];
  if (!$uri) {
    $result .= t('No PDF has been uploaded for this contract');
    return $result;
  }

  $url = file_create_url($uri);

  $result .= '<div id="grievance-pdf-url" style="display: none;">' . $url . '</div>';
  $result .= "[<a href=\"$url\" target=\"_blank\">Download or Print</a>]";
  $result .= '<div id="grievance-pdf-container">' . t('(PDF object will appear here)') . '</div>';
  $result .= "[<a href=\"$url\" target=\"_blank\">Download or Print</a>]";

  // drupal_add_css(drupal_get_path('module', 'grievance') . '/css/wv_etl_form.css');
  drupal_add_js(drupal_get_path('module', 'grievance') . '/js/grievance.js');
  drupal_add_js(drupal_get_path('module', 'grievance') . '/js/pdfobject.js');

  return $result;
}

function grievance_contract_find_rule($contract_tplt_nid, $status_tid) {
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_contract_rule');
  $query->fieldCondition('field_grievance_contract_tplt', 'target_id', $contract_tplt_nid);
  $query->fieldCondition('field_grievance_status', 'tid', $status_tid);
  $stmt = $query->execute();
  $rule_nids = $rule_nodes = array();
  if ($stmt['node']) {
    $rule_nids = array_keys($stmt['node']);
    $rule_nodes = node_load_multiple($rule_nids);
  }

  foreach ($rule_nodes as $rule_node) {
    return $rule_node;
  }
}

function grievance_contract_list_rules($contract_tplt_nid) {
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_contract_rule');
  $query->fieldCondition('field_grievance_contract_tplt', 'target_id', $contract_tplt_nid);
  $stmt = $query->execute();
  $rule_nids = $rule_nodes = array();
  if ($stmt['node']) {
    $rule_nids = array_keys($stmt['node']);
    $rule_nodes = node_load_multiple($rule_nids);
  }
  return $rule_nodes;
}

function grievance_contract_next_deadline($contract_tplt_nid, $status_tid, $from_ts = NULL) {
  if (!$from_ts) { $from_ts = time(); }
  $rule_node = grievance_contract_find_rule($contract_tplt_nid, $status_tid);
  if (!$rule_node) { return NULL; }
  $days = $rule_node->field_grievance_days['und'][0]['value'];
  $days_type = $rule_node->field_grievance_days_type['und'][0]['value'];
  return grievance_holidays_date($days, $days_type, $contract_tplt_nid, $from_ts);
}

function grievance_contract_next_alert_tid($contract_tplt_nid, $status_tid) {
  $rule_node = grievance_contract_find_rule($contract_tplt_nid, $status_tid);
  if (!$rule_node) { return NULL; }
  return $rule_node->field_grievance_alert_tid['und'][0]['tid'];
}

function grievance_form_grievance_contract_rule_node_form_alter(&$form, &$form_state, $form_id) {
  $weight = $form['field_grievance_notify_body']['#weight'] + 0.5;

  $form['token_tree'] = array(
    '#type' => 'fieldset',
    '#title' => t('Replacement patterns'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => theme('token_tree', array('token_types' => array('node'))),
    '#weight' => $weight
  );
}