<?php

function grievance_contract_template_node_tab_rules($contract_template_node) {
  drupal_set_title($contract_template_node->title . ': ' . t('Rules'));
  $entity_nid = $contract_template_node->nid;

  $result .= grievance_node_tab_get_addlink(
    "grievance-contract-rule", 
    array(
      "grievance_contract_tplt" => $entity_nid,
      "grievance_classaction" => 'No',
    )
  );

  $result = views_embed_view('grievance_contract_rules', 'block', $entity_nid);

  $result .= grievance_node_tab_get_addlink(
    "grievance-contract-rule", 
    array(
      "grievance_contract_tplt" => $entity_nid,
      "grievance_classaction" => 'No',
    )
  );

  return $result;
}

function grievance_contract_template_node_tab_notifications($contract_template_node) {
  drupal_set_title($contract_template_node->title . ': ' . t('Notifications'));
  $entity_nid = $contract_template_node->nid;

  $result .= grievance_node_tab_get_addlink("grievance-contract-rule", array("grievance_contract_tplt" => $entity_nid));

  $result = views_embed_view('grievance_contract_rules', 'block_1', $entity_nid);

  $result .= grievance_node_tab_get_addlink("grievance-contract-rule", array("grievance_contract_tplt" => $entity_nid));

  return $result;
}

function grievance_contract_template_node_tab_clauses($contract_template_node) {
  drupal_set_title($contract_template_node->title . ': ' . t('Sections'));
  $entity_nid = $contract_template_node->nid;

  $result = '';

  $result .= grievance_node_tab_get_addlink("grievance-contract-clause", array("grievance_contract_tplt" => $entity_nid));

  $result .= views_embed_view('grievance_contract_clauses', 'block', $entity_nid);

  $result .= grievance_node_tab_get_addlink("grievance-contract-clause", array("grievance_contract_tplt" => $entity_nid));

  return $result;
}

function grievance_contract_section_node_tab_clauses($contract_section_node) {
  drupal_set_title($contract_section_node->title . ': ' . t('Sections'));
  $contract_section_nid = $contract_section_node->nid;

  $contract_template_nid = $contract_section_node->field_grievance_contract_tplt['und'][0]['target_id'];
  $contract_template_node = node_load($contract_template_nid);

  $result = '';

  $result .= '[' . l(t('Back to @title', array('@title' => $contract_template_node->title)), 'node/' . $contract_template_nid . '/sections') . '] ';

  $result .= grievance_node_tab_get_addlink(
    "grievance-contract-clause", 
    array(
      "grievance_contract_tplt" => $contract_template_nid,
      "grievance_contract_section" => $contract_section_nid,
    )
  );

  $result .= views_embed_view('grievance_contract_clauses', 'block_2', $contract_template_nid, $contract_section_nid);
  $result .= grievance_node_tab_get_addlink(
    "grievance-contract-clause", 
    array(
      "grievance_contract_tplt" => $contract_template_nid,
      "grievance_contract_section" => $contract_section_nid,
    )
  );

  return $result;
}

function grievance_contract_template_node_tab_sections($contract_template_node) {
  drupal_set_title($contract_template_node->title . ': ' . t('Articles'));
  $entity_nid = $contract_template_node->nid;

  $result = '';
  
  $result .= grievance_node_tab_get_addlink("grievance-contract-section", array("grievance_contract_tplt" => $entity_nid));
  $result .= views_embed_view('grievance_contract_sections', 'block', $entity_nid);
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

function grievance_contract_find_rule($contract_tplt_nid, $status_tid) {
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_contract_rule');
  $query->fieldCondition('field_grievance_contract_tplt', 'target_id', $contract_tplt_nid);
  $query->fieldCondition('field_grievance_from_status', 'tid', $status_tid);
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

  // We really want to sort by field_grievance_weight, but we can't because it's sometimes null.
  // Which is why the atrocity of a uasort below.
  // $query->fieldOrderBy('field_grievance_weight', 'value');

  $stmt = $query->execute();
  $rule_nids = $rule_nodes = array();
  if ($stmt['node']) {
    $rule_nids = array_keys($stmt['node']);
    $rule_nodes = node_load_multiple($rule_nids);
  }

  uasort($rule_nodes, function($a, $b) { 
    if ($a->field_grievance_weight['und'][0]['value'] > $b->field_grievance_weight['und'][0]['value']) { return 1; }
    else if ($a->field_grievance_weight['und'][0]['value'] < $b->field_grievance_weight['und'][0]['value']) { return -1; }
    else if ($a->nid > $b->nid) { return 1; }
    else if ($a->nid < $b->nid) { return -1; }
    else { return 0; }
  });

  return $rule_nodes;
}

function grievance_contract_next_alert($grievance_node) {
  // Closed grievances have no alerts or deadlines
  $status_tid = $grievance_node->field_grievance_status['und'][0]['tid'];
  if (!$status_tid) { return array(NULL, NULL); }
  $status_term = taxonomy_term_load($status_tid);
  if (!$status_term) { return array(NULL, NULL); }
  $open = $status_term->field_grievance_open['und'][0]['value'];
  if (!$open) { return array(NULL, NULL); }

  $timeline = grievance_contract_compute_timeline($grievance_node);
  if (!$timeline) { return array(NULL, NULL); }

  $rule_nid = $timeline['current_rule'];
  if (!$rule_nid) { return array(NULL, NULL); }
  
  $rule_node = $timeline['rule_nodes'][$rule_nid];

  $delta = $timeline[$rule_nid]['current_delta'];
  if ($delta == -1) { return array(NULL, NULL); }

  $entry = $timeline[$rule_nid]['entries'][$delta];
  $next_alert_tid = $rule_node->field_grievance_alert_tid['und'][0]['tid'];
  $next_alert_ts = $entry['deadline'];

  return array($next_alert_tid, $next_alert_ts);
}


function grievance_contract_compute_timeline($grievance_node) {
  // Our big data structure. 
  $timeline = array();

  // Get the contract template
  $contract_tplt_nid = $grievance_node->field_grievance_contract_tplt['und'][0]['target_id'];
  if (!$contract_tplt_nid) {
    drupal_set_message(t('This grievance does not have a contract configured.'), 'warning');
    return '';
  }

  // Get all the contract rules
  $rule_nodes = grievance_contract_list_rules($contract_tplt_nid);
  if (!$rule_nodes) {
    drupal_set_message(t('This contract has no rules configured.'), 'warning');
    return '';
  }
  $timeline['rule_nodes'] = $rule_nodes;

  // Load all the statuses and alerts
  $status_vocabulary = taxonomy_vocabulary_machine_name_load('grievance_status');
  $status_terms = array();
  $tmp = taxonomy_get_tree($status_vocabulary->vid);
  foreach ($tmp as $status_term) {
    $status_terms[$status_term->tid] = $status_term;
  }
  $timeline['status_terms'] = $status_terms;

  $alert_vocabulary = taxonomy_vocabulary_machine_name_load('grievance_alert_types');
  $alert_terms = array();
  $tmp = taxonomy_get_tree($alert_vocabulary->vid);
  foreach ($tmp as $alert_term) {
    $alert_terms[$alert_term->tid] = taxonomy_term_load($alert_term->tid);
  }
  $timeline['alert_terms'] = $alert_terms;

  // Load the log dates that the grievance status changed
  $log_update_tids = array(
    variable_get('grievance_log_update_type', ''), 
    variable_get('grievance_log_incident_type', '')
  );
  $log_nodes = grievance_log_list($grievance_node, $log_update_tids);

  // Generate an open timeline
  $timeline_lookup_from_status = array();
  $timeline_lookup_next_status = array();
  foreach ($rule_nodes as $rule_node) {
    $rule_nid = $rule_node->nid;
    $timeline[$rule_nid] = array(
      'current_delta' => -1,
      'entries' => array(),
      'rule_node' => $rule_node,
    );
    $from_status_display = '';
    foreach ($rule_node->field_grievance_from_status['und'] as $tmp) {
      $tid = $tmp['tid'];
      $timeline_lookup_from_status[$tid][] = $rule_node->nid;
      if ($from_status_display) { $from_status_display .= ', '; }
      $from_status_display .= $status_terms[$tid]->name;
    }

    $next_status_display = '';
    foreach ($rule_node->field_grievance_next_status['und'] as $tmp) {
      $tid = $tmp['tid'];
      $timeline_lookup_next_status[$tid][] = $rule_node->nid;
      if ($next_status_display) { $next_status_display .= ', '; }
      $next_status_display .= $status_terms[$tid]->name;
    }
    $timeline[$rule_nid]['from_status_display'] = $from_status_display;
    $timeline[$rule_nid]['next_status_display'] = $next_status_display;
  }

  // Walk the log and generate the timeline.
  foreach ($log_nodes as $log_node) {
    $status_tid = $log_node->field_grievance_status['und'][0]['tid'];
    if (!$status_tid) { continue; }
    $status_ts = strtotime($log_node->field_grievance_status_date['und'][0]['value']);

    // Does this status start any timeline entries?
    if ($timeline_lookup_from_status[$status_tid]) {
      foreach ($timeline_lookup_from_status[$status_tid] as $rule_nid) {
        // Make sure we don't have an open entry at the moment (can't open it twice)
        $rule_node = $timeline['rule_nodes'][$rule_nid];
        $current_delta = $timeline[$rule_nid]['current_delta'];
        if ($current_delta > -1) { continue; }

        $days = $rule_node->field_grievance_days['und'][0]['value'];
        $days_type = $rule_node->field_grievance_days_type['und'][0]['value'];
        $deadline = grievance_holidays_date($days, $days_type, $contract_tplt_nid, $status_ts);
        if ($deadline) { $deadline = strtotime($deadline); }

        // Start a new section
        $timeline[$rule_nid]['entries'][] = array(
          'entered_on' => $status_ts,
          'entered_because' => $status_tid,
          'deadline' => $deadline,
        );
        $timeline[$rule_nid]['current_delta'] = count($timeline[$rule_nid]['entries'])-1;
        $timeline['current_rule'] = $rule_nid;
      }
    }

    // Does this status end any timeline entries?
    if ($timeline_lookup_next_status[$status_tid]) {
      foreach ($timeline_lookup_next_status[$status_tid] as $rule_nid) {
        // Only relevant if we're currently in an entry
        $current_delta = $timeline[$rule_nid]['current_delta'];
        if ($current_delta == -1) { continue; }

        // End the existing section
        $timeline[$rule_nid]['entries'][$current_delta]['left_on'] = $status_ts;
        $timeline[$rule_nid]['entries'][$current_delta]['left_because'] = $status_tid;
        $timeline[$rule_nid]['current_delta'] = -1;
        $timeline['current_rule'] = NULL;
      }
    }
  }

  return $timeline;
}