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
  drupal_goto('/node/' . $contract_template_node->nid . '/clauses/list');
}

function grievance_contract_template_node_tab_clauses_list($contract_template_node) {
  drupal_set_title($contract_template_node->title . ': ' . t('Sections'));
  $entity_nid = $contract_template_node->nid;

  $result = '';

  $result .= grievance_node_tab_get_addlink("grievance-contract-clause", array("grievance_contract_tplt" => $entity_nid));

  $result .= views_embed_view('grievance_contract_clauses', 'block', $entity_nid);

  $result .= grievance_node_tab_get_addlink("grievance-contract-clause", array("grievance_contract_tplt" => $entity_nid));

  return $result;
}

function grievance_contract_section_node_tab_clauses($contract_section_node) {
  drupal_goto('node/' . $contract_section_node->nid . '/clauses/list');
}

function grievance_contract_section_node_tab_clauses_list($contract_section_node) {
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

function grievance_contract_section_node_tab_clauses_bulk($contract_section_node) {
  return drupal_get_form('grievance_contract_section_node_tab_clauses_bulk_form', $contract_section_node);
}
         
function grievance_contract_section_node_tab_clauses_bulk_form($form, &$form_state, $contract_section_node) {
  $form_state['contract_section_node'] = $contract_section_node;
  $contract_template_nid = $contract_section_node->field_grievance_contract_tplt['und'][0]['target_id'];
  $contract_template_node = node_load($contract_template_nid);
  $form['intro'] = array('#markup' => '<div class="sirius_form_intro">' . '[' . l(t('Back to @title', array('@title' => $contract_template_node->title)), 'node/' . $contract_template_nid . '/sections') . '] ' . '</div>');

  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_contract_clause');
  $query->fieldCondition('field_grievance_contract_section', 'target_id', $contract_section_node->nid);
  $query->fieldOrderBy('field_grievance_weight', 'value');
  $query->fieldOrderBy('field_grievance_label', 'value');
  $stmt = $query->execute();
  $section_nids = array();
  if ($stmt['node']) {
    $section_nids = array_keys($stmt['node']);
  }
  $section_nodes = node_load_multiple($section_nids);

  $form['#tree'] = TRUE;

  $max_weight = 0;
  foreach ($section_nodes as $section_nid => $section_node) {
    if ($section_node->field_grievance_weight['und'][0]['value'] > $max_weight) {
      $max_weight = $section_node->field_grievance_weight['und'][0]['value'];
    }
    $form["section_${section_nid}"] = array(
      '#type' => 'fieldset',
      '#attributes' => array('class' => array('container-inline')), 
    );
    $form["section_${section_nid}"]["nid"] = array(
      '#type' => 'value',
      '#value' => $section_nid,
    );
    $form["section_${section_nid}"]["weight"] = array(
      '#title' => t('Weight'),
      '#type' => 'textfield',
      '#default_value' => $section_node->field_grievance_weight['und'][0]['value'],
      '#size' => 5,
    );
    $form["section_${section_nid}"]["number"] = array(
      '#title' => t('Section #'),
      '#type' => 'textfield',
      '#default_value' => $section_node->field_grievance_label['und'][0]['value'],
      '#size' => 10,
    );
    $form["section_${section_nid}"]["title"] = array(
      '#title' => t('Title'),
      '#type' => 'textfield',
      '#default_value' => $section_node->title,
    );
    $form["section_${section_nid}"]["body"] = array(
      '#type' => 'textarea',
      '#default_value' => $section_node->field_grievance_clause['und'][0]['value'],
      '#prefix' => '<div style="display: block;">',
      '#suffix' => '</div>',
    );
  }

  $blanks = $form_state['blanks'];
  if (!$blanks) { $blanks = 3; }
  if ($form_state['clicked_button']['#value'] == 'Add one') { $blanks++; }
  if ($form_state['clicked_button']['#value'] == 'Add ten') { $blanks += 10; }
  $form_state['blanks'] = $blanks;

  $form['newsections'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<div id="newsections">',
    '#suffix' => '</div>',
  );
  for ($i=0; $i<$blanks; ++$i) {
    $form['newsections']["newsection_${i}"] = array(
      '#type' => 'fieldset',
      '#attributes' => array('class' => array('container-inline')), 
    );
    $form['newsections']["newsection_${i}"]["weight"] = array(
      '#title' => t('Weight'),
      '#type' => 'textfield',
      '#size' => 5,
    );

    if ($max_weight) {
      $form['newsections']["newsection_${i}"]["weight"]['#default_value'] = $max_weight + $i + 1;
    }

    $form['newsections']["newsection_${i}"]["number"] = array(
      '#title' => t('Clause #'),
      '#type' => 'textfield',
      '#size' => 10,
    );
    $form['newsections']["newsection_${i}"]["title"] = array(
      '#title' => t('Title'),
      '#type' => 'textfield',
    );
    $form['newsections']["newsection_${i}"]["body"] = array(
      '#type' => 'textarea',
      '#prefix' => '<div style="display: block;">',
      '#suffix' => '</div>',
    );
  }

  $form['newsections']['add_one'] = array(
    '#type' => 'button',
    '#value' => t('Add one'),
    '#ajax' => array(
      'callback' => 'grievance_contract_template_node_tab_sections_bulk_form_addone',
      'wrapper' => 'newsections',
    ),
  );

  $form['newsections']['add_ten'] = array(
    '#type' => 'button',
    '#value' => t('Add ten'),
    '#ajax' => array(
      'callback' => 'grievance_contract_template_node_tab_sections_bulk_form_addone',
      'wrapper' => 'newsections',
    ),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

function grievance_contract_section_node_tab_clauses_bulk_form_submit($form, &$form_state) {
  $contract_section_node = $form_state['contract_section_node'];
  $contract_template_nid = $contract_section_node->field_grievance_contract_tplt['und'][0]['target_id'];
  $contract_template_node = node_load($contract_template_nid);

  // Update existing clauses
  foreach ($form_state['values'] as $name => $value) {
    if (!preg_match('/^section_\d+$/', $name)) { continue; }
    $nid = $value['nid'];
    $title = grievance_contract_template_clean_text($value['title']);
    $weight = grievance_contract_template_clean_text($value['weight']);
    $number = grievance_contract_template_clean_text($value['number']);
    $body = grievance_contract_template_clean_text($value['body']);
    if (!$weight) { $weight = 0; }

    if (!$title && !$body) {
      node_delete($nid);
    } else {
      $clause_node = node_load($nid);
      $clause_node->title = $title;
      $clause_node->field_grievance_weight['und'][0]['value'] = $weight;
      $clause_node->field_grievance_label['und'][0]['value'] = $number;
      $clause_node->field_grievance_clause['und'][0]['value'] = $body;
      node_save($clause_node);
    }
  }

  // Create new clauses
  global $user;
  foreach ($form_state['values']['newsections'] as $name => $value) {
    if (!preg_match('/^newsection_\d+$/', $name)) { continue; }
    $nid = $value['nid'];
    $title = grievance_contract_template_clean_text($value['title']);
    $weight = grievance_contract_template_clean_text($value['weight']);
    $number = grievance_contract_template_clean_text($value['number']);
    $body = grievance_contract_template_clean_text($value['body']);
    if (!$weight) { $weight = 0; }
    if (!$title && !$body) { continue; }

    $clause_node = new stdClass();
    $clause_node->title = $title;
    $clause_node->type = 'grievance_contract_clause';
    node_object_prepare($clause_node);
    $clause_node->language = LANGUAGE_NONE;
    $clause_node->uid = $user->uid; 
    $clause_node->status = 1;
    $clause_node->promote = 0;
    $clause_node->comment = 0;
    $clause_node->field_grievance_contract_tplt = array('und' => array(array('target_id' => $contract_template_node->nid)));
    $clause_node->field_grievance_contract_section = array('und' => array(array('target_id' => $contract_section_node->nid)));
    $clause_node->field_grievance_weight['und'][0]['value'] = $weight;
    $clause_node->field_grievance_label['und'][0]['value'] = $number;
    $clause_node->field_grievance_clause['und'][0]['value'] = $body;
    node_save($clause_node);

  }
}

function grievance_contract_template_node_tab_sections($contract_template_node) {
  drupal_goto('node/' . $contract_template_node->nid . '/sections/list');
}

function grievance_contract_template_node_tab_sections_list($contract_template_node) {
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

function grievance_contract_template_node_tab_picker($contract_template_node) {
  drupal_set_title($contract_template_node->title . ': ' . t('Section Picker'));
  $entity_nid = $contract_template_node->nid;

  drupal_add_js(drupal_get_path('module', 'grievance') . '/js/grievance.clause-picker.js');
  $result = views_embed_view('grievance_contract_clauses', 'block_3', $entity_nid);

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
    variable_get('grievance_log_insert_type', ''), 
    variable_get('grievance_log_update_type_with_status_change', ''), 
    variable_get('grievance_log_update_type_without_status_change', ''), 
    variable_get('grievance_log_incident_type', ''),
    variable_get('grievance_log_timelines_extended_type', ''),
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
      }
    }
  }

  // Walk the log again and extend any timelines
  foreach ($log_nodes as $log_node) {
    if ($log_node->field_grievance_log_type['und'][0]['tid'] != variable_get('grievance_log_timelines_extended_type', '')) { continue; }
    $days = $log_node->field_grievance_days['und'][0]['value'];
    $status_ts = strtotime($log_node->field_grievance_status_date['und'][0]['value']);
    foreach ($rule_nodes as $rule_node) {
      $rule_nid = $rule_node->nid;
      if (isset($timeline[$rule_nid]['entries'])) {
        foreach ($timeline[$rule_nid]['entries'] as $delta => $entry) {
          // Only applies to timeline entries that were active when the log entry was made.
          if ($entry['entered_on'] > $status_ts) { continue; }
          if ($entry['deadline'] < $status_ts) { continue; }

          // Extend the deadline by $days
          // $entry['deadline'] = strtotime("+$days days", $entry['deadline']);
          $timeline[$rule_nid]['entries'][$delta]['deadline'] = strtotime("+$days days", $entry['deadline']);
        }
      }
    }
  }

  // Find the "current" rule -- i.e. first rule without an end time
  $found = FALSE;
  foreach ($rule_nodes as $rule_node) {
    $rule_nid = $rule_node->nid;
    if (isset($timeline[$rule_nid]['entries'])) {
      foreach ($timeline[$rule_nid]['entries'] as $entry) {
        if (!$entry['left_on']) {
          $timeline['current_rule'] = $rule_nid;
          $found = TRUE;
          break;
        }
      }
      if ($found) { break; }
    }
  }

  return $timeline;
}

function grievance_contract_update_dummies($trigger_node) {
  // Find the contract template
  $contract_tplt_node = NULL;
  if ($trigger_node->type == 'grievance_contract_template') {
    $contract_tplt_node = $trigger_node;
    $contract_tplt_nid = $contract_tplt_node->nid;
  } else {
    $contract_tplt_nid = $trigger_node->field_grievance_contract_tplt['und'][0]['target_id'];
    $contract_tplt_node = node_load($contract_tplt_nid);
  }
  if (!$contract_tplt_node) { return; }

  // Do we want dummies? "Yes" or "No"; NULL means we don't know so just go away.
  $want_dummies = $contract_tplt_node->field_grievance_artsel['und'][0]['value'];
  if (!$want_dummies) { return; }

  // Find the list of sections
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_contract_section');
  $query->fieldCondition('field_grievance_contract_tplt', 'target_id', $contract_tplt_nid);
  $stmt = $query->execute();
  if (!$stmt['node']) { return; }
  $contract_section_nids = array_keys($stmt['node']);
  $contract_section_nodes = node_load_multiple($contract_section_nids);

  // For each section ...
  foreach ($contract_section_nodes as $contract_section_node) {
    // Find any current dummies
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'node');
    $query->entityCondition('bundle', 'grievance_contract_clause');
    $query->fieldCondition('field_grievance_contract_section', 'target_id', $contract_section_node->nid);
    $query->fieldCondition('field_grievance_dummy', 'value', 'Yes');
    $stmt = $query->execute();
    $dummy_clause_nids = array();
    if ($stmt['node']) { $dummy_clause_nids = array_keys($stmt['node']); }

    //
    // Do we want dummies?
    //

    if ($want_dummies == 'Yes') {

      // 
      // Yes we want dummies.  Do we have any?
      //

      if (!$dummy_clause_nids) {

        // 
        // No we don't have any.  Create one.
        //

        global $user;
        $dummy_node = new stdClass();
        $dummy_node->title = $contract_section_node->title;
        $dummy_node->type = 'grievance_contract_clause';
        $dummy_node->no_recurse = TRUE;
        node_object_prepare($dummy_node);
        $dummy_node->language = LANGUAGE_NONE;
        $dummy_node->uid = $user->uid; 
        $dummy_node->status = 1;
        $dummy_node->promote = 0;
        $dummy_node->comment = 0;
        $dummy_node->field_grievance_contract_tplt = array('und' => array(array('target_id' => $contract_tplt_nid)));
        $dummy_node->field_grievance_contract_section = array('und' => array(array('target_id' => $contract_section_node->nid)));
        $dummy_node->field_grievance_label = array('und' => array(array('value' => $contract_section_node->field_grievance_section_number['und'][0]['value'])));
        $dummy_node->field_grievance_dummy = array('und' => array(array('value' => 'Yes')));
        $dummy_node->grievance_no_recurse = TRUE;
        node_save($dummy_node);
      } else {

        //
        // Yes we have one.  Update it.
        //

        $dummy_node = node_load($dummy_clause_nids[0]);
        $dummy_node->field_grievance_label = array('und' => array(array('value' => $contract_section_node->field_grievance_section_number['und'][0]['value'])));
        $dummy_node->title = $contract_section_node->title;
        $dummy_node->grievance_no_recurse = TRUE;
        node_save($dummy_node);

      }
    } else {

      // 
      // No we don't want dummies.  Delete them.
      //

      if ($dummy_clause_nids) {
        node_delete_multiple($dummy_clause_nids);
      }
    }
  }
}

//
// Not using these functions at the moment, @todo probably safe to remove
//

function grievance_contract_clause_updateinsert($node) { }
function grievance_contract_section_updateinsert($node) { }
function grievance_contract_template_updateinsert($node) { }


function grievance_contract_views_pre_build(&$view) {
  // When an admin is editing a grievance clause, they select the contract section from a view.  The view should be restricted
  // to just the contract template that they're working i.
  if ($view->name == 'grievance_contract_sections' && $view->current_display == 'entityreference_1') { 
    $template_nid = 0;
    $clause_node = menu_get_object();
    if ($clause_node) {
      $template_nid = $clause_node->field_grievance_contract_tplt['und'][0]['target_id'];
    }
    if (!$template_nid && isset($_REQUEST['grievance_contract_tplt'])) {
      $template_nid = $_REQUEST['grievance_contract_tplt'];
    }
    if ($template_nid) {
      $view->args[0] = $template_nid;
    }
  }

  if ( ($view->name == 'grievance_contract_clauses' && $view->current_display == 'views_data_export_1') ||
       ($view->name == 'grievance_contract_clauses' && $view->current_display == 'views_data_export_2') ||
       ($view->name == 'grievance_contract_sections' && $view->current_display == 'views_data_export_1') ) {
    $contract_tplt_nid = $view->args[0];
    $contract_tplt_node = node_load($contract_tplt_nid);

    $contract_section_nid = $view->args[1];

    $filename = $contract_tplt_node->title;
    if ($contract_section_nid) {
      $contract_section_node = node_load($contract_section_nid);
      $filename .= ' - ' . $contract_section_node->title;      
    }

    if ($view->name == 'grievance_contract_sections') {
      $filename .= ' - ' . "Articles";
    }

    $filename .= '.csv';

    $view->display_handler->options['style_options']['filename'] = $filename;
  }
}

function grievance_contract_template_node_tab_sections_bulk($contract_template_node) { 
  return drupal_get_form('grievance_contract_template_node_tab_sections_bulk_form', $contract_template_node); 
}

function grievance_contract_template_node_tab_sections_bulk_form($form, &$form_state, $contract_template_node) {
  $form_state['contract_template_node'] = $contract_template_node;

  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_contract_section');
  $query->fieldCondition('field_grievance_contract_tplt', 'target_id', $contract_template_node->nid);
  $query->fieldOrderBy('field_grievance_weight', 'value');
  $query->fieldOrderBy('field_grievance_section_number', 'value');
  $stmt = $query->execute();
  $section_nids = array();
  if ($stmt['node']) {
    $section_nids = array_keys($stmt['node']);
  }
  $section_nodes = node_load_multiple($section_nids);

  $form['#tree'] = TRUE;

  $max_weight = 0;
  foreach ($section_nodes as $section_nid => $section_node) {
    if ($section_node->field_grievance_weight['und'][0]['value'] > $max_weight) {
      $max_weight = $section_node->field_grievance_weight['und'][0]['value'];
    }
    $form["section_${section_nid}"] = array(
      '#type' => 'fieldset',
      '#attributes' => array('class' => array('container-inline')), 
    );
    $form["section_${section_nid}"]["nid"] = array(
      '#type' => 'value',
      '#value' => $section_nid,
    );
    $form["section_${section_nid}"]["weight"] = array(
      '#title' => t('Weight'),
      '#type' => 'textfield',
      '#default_value' => $section_node->field_grievance_weight['und'][0]['value'],
      '#size' => 5,
    );
    $form["section_${section_nid}"]["number"] = array(
      '#title' => t('Article #'),
      '#type' => 'textfield',
      '#default_value' => $section_node->field_grievance_section_number['und'][0]['value'],
      '#size' => 10,
    );
    $form["section_${section_nid}"]["title"] = array(
      '#title' => t('Title'),
      '#type' => 'textfield',
      '#default_value' => $section_node->title,
    );
  }

  $blanks = $form_state['blanks'];
  if (!$blanks) { $blanks = 3; }
  if ($form_state['clicked_button']['#value'] == 'Add one') { $blanks++; }
  if ($form_state['clicked_button']['#value'] == 'Add ten') { $blanks += 10; }
  $form_state['blanks'] = $blanks;

  $form['newsections'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<div id="newsections">',
    '#suffix' => '</div>',
  );
  for ($i=0; $i<$blanks; ++$i) {
    $form['newsections']["newsection_${i}"] = array(
      '#type' => 'fieldset',
      '#attributes' => array('class' => array('container-inline')), 
    );
    $form['newsections']["newsection_${i}"]["weight"] = array(
      '#title' => t('Weight'),
      '#type' => 'textfield',
      '#size' => 5,
    );

    if ($max_weight) {
      $form['newsections']["newsection_${i}"]["weight"]['#default_value'] = $max_weight + $i + 1;
    }

    $form['newsections']["newsection_${i}"]["number"] = array(
      '#title' => t('Article #'),
      '#type' => 'textfield',
      '#size' => 10,
    );
    $form['newsections']["newsection_${i}"]["title"] = array(
      '#title' => t('Title'),
      '#type' => 'textfield',
    );
  }

  $form['newsections']['add_one'] = array(
    '#type' => 'button',
    '#value' => t('Add one'),
    '#ajax' => array(
      'callback' => 'grievance_contract_template_node_tab_sections_bulk_form_addone',
      'wrapper' => 'newsections',
    ),
  );

  $form['newsections']['add_ten'] = array(
    '#type' => 'button',
    '#value' => t('Add ten'),
    '#ajax' => array(
      'callback' => 'grievance_contract_template_node_tab_sections_bulk_form_addone',
      'wrapper' => 'newsections',
    ),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

function grievance_contract_template_node_tab_sections_bulk_form_submit($form, &$form_state) {
  $contract_template_node = $form_state['contract_template_node'];

  // Update existing articles
  foreach ($form_state['values'] as $name => $value) {
    if (!preg_match('/^section_\d+$/', $name)) { continue; }
    $nid = $value['nid'];
    $title = grievance_contract_template_clean_text($value['title']);
    $weight = grievance_contract_template_clean_text($value['weight']);
    $number = grievance_contract_template_clean_text($value['number']);

    if (!$title) {
      node_delete($nid);
    } else {
      $section_node = node_load($nid);
      $section_node->title = $title;
      $section_node->field_grievance_weight['und'][0]['value'] = $weight;
      $section_node->field_grievance_section_number['und'][0]['value'] = $number;
      node_save($section_node);
    }
  }

  // Create new articles
  global $user;
  foreach ($form_state['values']['newsections'] as $name => $value) {
    if (!preg_match('/^newsection_\d+$/', $name)) { continue; }
    $nid = $value['nid'];
    $title = grievance_contract_template_clean_text($value['title']);
    $weight = grievance_contract_template_clean_text($value['weight']);
    $number = grievance_contract_template_clean_text($value['number']);
    if (!$title) { continue; }

    $section_node = new stdClass();
    $section_node->title = $title;
    $section_node->type = 'grievance_contract_section';
    node_object_prepare($section_node);
    $section_node->language = LANGUAGE_NONE;
    $section_node->uid = $user->uid; 
    $section_node->status = 1;
    $section_node->promote = 0;
    $section_node->comment = 0;
    $section_node->field_grievance_contract_tplt = array('und' => array(array('target_id' => $contract_template_node->nid)));
    if ($weight) {
      $section_node->field_grievance_weight['und'][0]['value'] = $weight;
    }
    $section_node->field_grievance_section_number['und'][0]['value'] = $number;
    node_save($section_node);

  }
}

function grievance_contract_template_node_tab_sections_bulk_form_addone($form, $form_state) {
  $commands = array();
  $commands[] = ajax_command_replace("#newsections", render($form['newsections']));
  return array(
    '#type' => 'ajax',
    '#commands' => $commands
  );
}

function grievance_contract_template_clean_text($text) {
  // Get rid of leading and trailing.
  $text = trim($text);
  $text = preg_replace('/\r/', '', $text);
  $text = preg_replace('/\s*\n\n\s*/', "{lf}", $text);
  $text = preg_replace('/\s+/', ' ', $text);
  $text = preg_replace('/({lf})+/', "\n\n", $text);

  // Convert 
  return $text;
}