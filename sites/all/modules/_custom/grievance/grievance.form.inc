<?php

function grievance_form_grievance_node_callback_replace($form, $form_state) {
  $commands = array();
  $commands[] = ajax_command_replace("#grievance-node-form-replaceme", render($form));
  $commands[] = ajax_command_invoke(NULL, "grievance_bind_classification_term_map", array());
  return array(
    '#type' => 'ajax',
    '#commands' => $commands
  );
}

// Find the current value for a field: look in the form_state['values'], form_state['node'], form['default_value']
// This is a quite a hack.  Don't use it outside of the current context.
function grievance_form_current_value($field, $column, $form, $form_state) {
  if (isset($form_state['values']) && isset($form_state['values'][$field]['und'][0][$column])) {
    $result = $form_state['values'][$field]['und'][0][$column];
    return $result;
  }

  if (isset($form['#node']) && isset($form['#node']->{$field}['und'][0][$column])) {
    $result = $form['#node']->{$field}['und'][0][$column];
    return $result;
  }

  if (isset($form[$field]['und']['#default_value'])) {
    $result = $form[$field]['und']['#default_value'];
    return $result;
  }
}

function grievance_form_grievance_node_form_alter(&$form, &$form_state, $form_id) {
  //
  // Load our custom javascript
  //

  drupal_add_js(drupal_get_path('module', 'grievance') . '/js/grievance.form-grievance.js');

  //
  // Hide the title, we're auto-generating
  //

  $form['title']['#access'] = FALSE;

  // 
  // Base node and user
  // 

  $grievance_node = $form_state['node'];
  global $user;
  if (!$user) { return; }
  $user_full = user_load($user->uid);
  if (!$user_full) { return; }

  // 
  // Shop ID
  // 
  
  $shop_nid = $form_state['values']['field_grievance_shop']['und'][0]['target_id'];
  if (!$shop_nid) {
    $shop_nid = $grievance_node->field_grievance_shop['und'][0]['target_id'];
  }
  if (!$shop_nid) {
    $shop_nid = $user_full->field_grievance_shops['und'][0]['target_id'];
  }
  if ($shop_nid) {
    $shop_node = node_load($shop_nid);
  }

  //
  // Wrapper because we're replacing the entire form on ajax
  // 

  $form['#prefix'] = '<div id="grievance-node-form-replaceme">';
  $form['#suffix'] = '</div>';

  //
  // These are the list of fields which cause the form to be rebuilt.
  // 

  $replace = array(
    'callback' => 'grievance_form_grievance_node_callback_replace',
    'method' => 'replace',
  );
  $form['field_grievance_shop']['und'][0]['target_id']['#ajax'] = $replace;
  $form['field_grievance_contract_tplt']['und']['#ajax'] = $replace;
  $form['field_grievance_status']['und']['#ajax'] = $replace;
  $form['field_grievance_classaction']['und']['#ajax'] = $replace;
  //$form['field_grievance_category']['und']['#ajax'] = $replace;

  //
  // Open the correct accordion group
  //

  if ($form_state['triggering_element']['#field_name'] == 'field_grievance_type' ||
    $form_state['triggering_element']['#field_name'] == 'field_grievance_clauseref' ||
    $form_state['triggering_element']['#field_name'] == 'field_grievance_category') {
    $form['#groups']['group_grievance_status']->format_settings['formatter'] = 'closed';
    $form['#groups']['group_grievance_form']->format_settings['formatter'] = 'open';
  } else if ($form_state['triggering_element']['#field_name']) {
    $form['#groups']['group_grievance_status']->format_settings['formatter'] = 'open';
  }
  
  //////////////////////////////////////////////////////
  // These things happen on rebuild
  //////////////////////////////////////////////////////

  //
  // Set the status date, for status changes
  // 

  $form['field_grievance_status_date']['und'][0]['#default_value']['value'] = date('Y-m-d H:i:s');

  //
  // Remove access to the "shop" field for stewards
  //

  if ($user_full->field_grievance_shops['und'][0]['target_id'] && 
     !$user_full->field_grievance_shops['und'][1]['target_id'] && 
     !user_access('grievance edit')) {
    $form['field_grievance_shop']['#access'] = FALSE;
  }

  //
  // Restrict the list of available statuses based on the user's role.
  //

  if (!user_access('grievance admin')) {
    $status_vid = taxonomy_vocabulary_machine_name_load('grievance_status')->vid;
    $status_terms = taxonomy_get_tree($status_vid);

    $user_roles = array_keys($user_full->roles);

    $status_tids_to_disable = array();
    foreach ($status_terms as $status_term) {
      $status_tid = $status_term->tid;
      $status_term_full = taxonomy_term_load($status_tid);
      $status_term_roles = array();
      if ($status_term_full->field_grievance_roles['und']) {
        foreach ($status_term_full->field_grievance_roles['und'] as $delta => $tmp) {
          $status_term_roles[] = $tmp['value'];
        }
      }

      if (!array_intersect($status_term_roles, $user_roles)) {
        $status_tids_to_disable[] = $status_tid;
      }
    }

    foreach ($status_tids_to_disable as $status_tid) {
      unset($form['field_grievance_status']['und']['#options'][$status_tid]);
    }
  }

  //
  // Change field visibility based on category
  //

  $category_tid = grievance_form_current_value('field_grievance_category', 'tid', $form, $form_state);
  if ($category_tid) {
    $category_term = taxonomy_term_load($category_tid);
    $hidefields = preg_split('/\s+/', $category_term->field_grievance_hidefields['und'][0]['value']);
    foreach ($hidefields as $hidefield) {
      if (preg_match('/^field_/', $hidefield) && $form[$hidefield]) {
        $form[$hidefield]['#access'] = FALSE;
      }
    }

    /*
    if ($category_term){
      if ($category_term->name == 'Contract') {
        $form['field_grievance_remedy']['#access'] = FALSE;
        $form['field_grievance_type']['#access'] = FALSE;
      } else if ($category_term->name == 'Discipline') {
        $form['field_grievance_classaction']['#access'] = FALSE;
        // $form['field_grievance_remedy_other']['#access'] = FALSE;
        // $form['field_grievance_type_other']['#access'] = FALSE;
      }
    }
    */
  }

  //
  // Set the contract template from the shop
  // 

  $contract_tplt_nid = grievance_form_current_value('field_grievance_contract_tplt', 'target_id', $form, $form_state);
  if ($shop_node) {
    if (!$contract_tplt_nid || $form_state['triggering_element']['#field_name'] == 'field_grievance_shop' || $form['field_grievance_contract_tplt']['#disabled']) {
      $contract_tplt_nid = $shop_node->field_grievance_contract_tplt['und'][0]['target_id'];

      if ($form['field_grievance_contract_tplt']['#disabled']) {
        $form['field_grievance_contract_tplt']['und']['#default_value'][0] = $contract_tplt_nid;
        $form['field_grievance_contract_tplt']['und']['#value'] = $form['field_grievance_contract_tplt']['und']['#default_value'];
      } else {
        $form_state['input']['field_grievance_contract_tplt']['und'] = $contract_tplt_nid;
      }
    }
  }
  if ($contract_tplt_nid) {
    $contract_tplt_node = node_load($contract_tplt_nid);
  }

  // 
  // Set the list of clauses
  //

  $clauses_options = array();
  if ($contract_tplt_nid) {
    $clauses_view = views_get_view('grievance_contract_clauses');
    $clauses_view->args[0] = $contract_tplt_nid;
    $clauses_view->execute('entityreference_1');
    $clauses_options['_none'] = t('- None -');
    foreach ($clauses_view->result as $clause) {
      $clauses_options[$clause->nid] = 
        $clause->field_data_field_grievance_label_field_grievance_label_value . 
        ': ' . 
        $clause->node_title;
    }
  }
  $form['field_grievance_clauseref']['und']['#options'] = $clauses_options;
  $idx = 0;
  while (isset($form['field_grievance_clauseref']['und'][$idx]['target_id']['#options'])) {
    $form['field_grievance_clauseref']['und'][$idx]['target_id']['#options'] = $clauses_options;
    ++$idx;
  }

  //
  // Link to contract
  //

  $contract_link_text = '';
  if ($shop_node) {
    $contract_uri = $shop_node->field_grievance_contract['und'][0]['uri'];
    if ($contract_uri) {
      $contract_url = file_create_url($contract_uri);

      $contract_link_text .= '<strong>' . 
        t('The contract for the shop ') .
        $shop_node->title . 
        t(' can be downloaded ') .
        l(t('here'), $contract_url, array('attributes' => array('target'=>'_blank'))) .
        '</strong>.';
    }
  }
  if ($contract_tplt_node) {
    $contract_uri = $contract_tplt_node->field_grievance_contract['und'][0]['uri'];
    if ($contract_uri) {
      $contract_url = file_create_url($contract_uri);

      if ($contract_link_text) { $contract_link_text .= '<br>'; }
      $contract_link_text .= '<strong>' . 
        t('The contract template for ') .
        $contract_tplt_node->title . 
        t(' can be downloaded ') .
        l(t('here'), $contract_url, array('attributes' => array('target'=>'_blank'))) .
        '</strong>.';
    }
  }

  $form['#groups']['group_grievance_form']->format_settings['instance_settings']['description'] = 
    '<div id="grievance-form-contract-link">' . $contract_link_text . '</div>';

  // Add the clause picker as a new field
  if ($contract_tplt_node && $contract_tplt_node->nid) {
    $form['field_grievance_clauseref']['#prefix'] = '<a href="#" onClick="javascript:sirius_popup(\'/node/' . $contract_tplt_node->nid . '/clauses/picker/popup\', \'clause_picker\', 1025, 600); return false;">Clause Picker</a>';
  }

  // 
  // Recipients
  //

  $status_tid = grievance_form_current_value('field_grievance_status', 'tid', $form, $form_state);
  $status_term = taxonomy_term_load($status_tid);

  if ($form['field_grievance_contact_selector']['#access']) {
    $form['field_grievance_contact_selector']['und'][0]['markup']['#markup'] = grievance_contacts_create_selector($shop_nid, $status_tid, 'contact', FALSE);
  }

  if ($form['field_grievance_st_selector']['#access']) {
    $form['field_grievance_st_selector']['und'][0]['markup']['#markup'] = grievance_contacts_create_selector($shop_nid, $status_tid, 'st', TRUE);
  }


  // 
  // Set default values for manager, organizer, etc.
  // (This should be on insert only, not edit.  We never set defaults on edit.)
  //

  if (!$grievance_node->nid) {
    $user_defaults = array(
      'field_grievance_rep_assignee' => array(),
      'field_grievance_rep_filed' => array(),
      'field_grievance_rep_organizer' => array(),
      'field_grievance_rep_lead' => array(),
      'field_grievance_rep_admin' => array(),
      'field_grievance_rep_watching' => array(),
    );

    // We might get these from the shop node
    if ($shop_node) {
      foreach ($user_defaults as $field_name => $ignore) {
        if ($shop_node->{$field_name}['und'][0]) {
          foreach ($shop_node->{$field_name}['und'] as $tmp) {
            $shop_uid = $tmp['target_id'];
            $shop_user = user_load($shop_uid);
            $user_defaults[$field_name][] = $shop_user;
          }
        }
      }
    }

    // If we didn't get the "filed by" from the shop, make it me
    if (!$user_defaults['field_grievance_rep_filed']) {
      $user_defaults['field_grievance_rep_filed'][] = $user;

      // Also set *my* organizer and my organizer's lead, if any
      if (isset($user_full->field_grievance_rep_organizer['und'])) {
        foreach ($user_full->field_grievance_rep_organizer['und'] as $tmp) {
          $organizer_uid = $tmp['target_id'];
          $organizer = user_load($organizer_uid);
          $user_defaults['field_grievance_rep_organizer'][] = $organizer;
          if (isset($organizer->field_grievance_rep_lead['und'])) {
            foreach ($organizer->field_grievance_rep_lead['und'] as $tmp) {
              $lead_uid = $tmp['target_id'];
              $user_defaults['field_grievance_rep_lead'][] = user_load($lead_uid);
            }
          }
        }
      }

      // Also set *my* lead
      if (isset($user_full->field_grievance_rep_lead['und'])) {
        foreach ($user_full->field_grievance_rep_lead['und'] as $tmp) {
          $lead_uid = $tmp['target_id'];
          $user_defaults['field_grievance_rep_lead'][] = user_load($lead_uid);
        }
      }
    }

    // Also add the global watcher (if any)
    if (variable_get('grievance_rep_watching', '')) {
      $user_defaults['field_grievance_rep_watching'][] = user_load(variable_get('grievance_rep_watching', '')['entity_id']);
    }

    // Also add the global manager (if any)
    if (variable_get('grievance_rep_manager', '')) {
      $user_defaults['field_grievance_rep_manager'][] = user_load(variable_get('grievance_rep_manager', '')['entity_id']);
    }

    // Also add the global assignee (if any)
    if (!$user_defaults['field_grievance_rep_assignee']) {
      if (variable_get('grievance_rep_assignee', '')) {
        $user_defaults['field_grievance_rep_assignee'][] = user_load(variable_get('grievance_rep_assignee', '')['entity_id']);
      }
    }

    // Now turn these "default" arrays into default values in the form
    foreach ($user_defaults as $field_name => $values) {
      unset($form_state['input'][$field_name]['und']);
      $delta = 0;
      if ($values) {
        foreach ($values as $value) {
          if ($value) {
            $form[$field_name]['und'][$delta]['target_id']['#default_value'] = "$value->name ($value->uid)";
            $form_state['input'][$field_name]['und'][$delta]['target_id'] = "$value->name ($value->uid)";
            $form_state['input'][$field_name]['und'][$delta]['_weight'] = NULL;
            ++$delta;
          }
        }
      }
      // $form_state['field'][$field_name]['und']['items_count'] = $delta+1;
      // $form_state['field'][$field_name]['und']['field']['cardinality'] = $delta+1;
    }

    // Set the assignee notes
    if ($shop_node->field_grievance_assignee_notes['und']) {
      $form['field_grievance_assignee_notes']['und'][0]['value']['#default_value'] = $shop_node->field_grievance_assignee_notes['und']['0']['value'];
    } else {
      $assignee_notes = variable_get('grievance_assignee_notes', '');
      if ($assignee_notes) {
        $form['field_grievance_assignee_notes']['und'][0]['value']['#default_value'] = $assignee_notes;
      }
    }

    // Shop
    if ($shop_node) {
      $form['field_grievance_shop']['und'][0]['target_id']['#default_value'] = "$shop_node->title ($shop_node->nid)";
    }
  }

  //
  // Constrain the list of possible classifications and departments (set in the contract template)
  //

  if ($contract_tplt_node) { 
    //
    // Disable classifications based on the contract template.
    // (I think this code is not used, because we're doing it by ajax.)
    //

    $map = array('_none' => TRUE);
    $to_disable = array();
    if ($contract_tplt_node->field_grievance_classifications['und']) {
      foreach ($contract_tplt_node->field_grievance_classifications['und'] as $tmp) {
        $map[$tmp["tid"]] = TRUE;
      }
    }

    if ($form['field_grievance_class_tid']['und']['#options']) {
      foreach ($form['field_grievance_class_tid']['und']['#options'] as $tid => $name) {
        if (!$map[$tid]) { $to_disable[] = $tid; }
      }
    }
    foreach ($to_disable as $tid) {
      unset($form['field_grievance_class_tid']['und']['#options'][$tid]);
    }

    //
    // Disable departments based on the contract template.
    //

    $map = array('_none' => TRUE);
    $has_map = FALSE;
    if ($contract_tplt_node->field_grievance_departments['und']) {
      foreach ($contract_tplt_node->field_grievance_departments['und'] as $tmp) {
        $map[$tmp["tid"]] = TRUE;
        $has_map = TRUE;
      }
    }
    if ($has_map) {
      $to_disable = array();
      if ($form['field_grievance_department_tid']['und']['#options']) {
        foreach ($form['field_grievance_department_tid']['und']['#options'] as $tid => $name) {
          if (!$map[$tid]) { $to_disable[] = $tid; }
        }
      }
      foreach ($to_disable as $tid) {
        unset($form['field_grievance_department_tid']['und']['#options'][$tid]);
      }
    }

    // 
    // Disable departments based on the classification.
    // @TODO may be unnecessary to trigger a form rebuild at this point.  need to discuss
    // 

    $grievance_classification_department_map = grievance_classification_department_map($contract_tplt_node);
    drupal_add_js(
      array(
        'grievance' => array(
          'grievance_classification_department_map' => $grievance_classification_department_map,
        )
      ), 
      'setting'
    );
    //$classification_tid = grievance_form_current_value('field_grievance_class_tid', 'tid', $form, $form_state); 
    //drupal_set_message("Here with $classification_tid");
  }

  //
  // Add the "Assignee Notes" dropdown
  //

  if ($form['field_grievance_assignee_notes']['#access']) {
    $assignee_notes_vocabulary = taxonomy_vocabulary_machine_name_load('grievance_assignee_notes');
    $assignee_notes_options = array(NULL => '[select]');
    $tmp = taxonomy_get_tree($assignee_notes_vocabulary->vid);
    $found = FALSE;
    foreach ($tmp as $term) {
      $found = TRUE;
      $assignee_notes_options[$term->name] = $term->name;
    }

    if ($found) {
      $weight = $form['field_grievance_assignee_notes']['#weight'] - 0.5;
      $form['grievance_assignee_notes_dropdown'] = array(
        '#type' => 'select',
        '#weight' => $weight,
        '#tree' => TRUE,
        '#options' => $assignee_notes_options,
        '#title' => t('Select Assignee Notes'),
        '#attributes' => array('class' => array('grievance_assignee_notes_dropdown')),
      );

      $form['#groups']['group_grievance_union_rep']->children[] = 'grievance_assignee_notes_dropdown';
      $form['#group_children']['grievance_assignee_notes_dropdown'] = 'group_grievance_union_rep';
    }
  }
  
  //
  // Show or hide fields based on value of the class action toggle
  // 

  $classaction = grievance_form_current_value('field_grievance_classaction', 'value', $form, $form_state);
  $field_to_hide = array();
  if ($classaction == 'Yes') {

    $form['field_grievance_first_name']['und']['#title'] = 
      $form['field_grievance_first_name']['und'][0]['#title'] = 
      $form['field_grievance_first_name']['und'][0]['value']['#title'] = t('Group or Class Name');
    $form['field_grievance_last_name']['und']['#title'] = 
      $form['field_grievance_last_name']['und'][0]['#title'] = 
      $form['field_grievance_last_name']['und'][0]['value']['#title'] = t('Group or Class Description');

    $fields_to_hide[] = 'field_grievance_phone';
    $fields_to_hide[] = 'field_grievance_email';
    $fields_to_hide[] = 'field_grievance_address';
    $fields_to_hide[] = 'field_grievance_address_2';
    $fields_to_hide[] = 'field_grievance_city';
    $fields_to_hide[] = 'field_grievance_state';
    $fields_to_hide[] = 'field_grievance_zip';
    $fields_to_hide[] = 'field_grievance_hire_date';
    $fields_to_hide[] = 'field_grievance_gender';
    /*
    $fields_to_hide[] = 'field_grievance_class_tid';
    $fields_to_hide[] = 'field_grievance_classification';
    $fields_to_hide[] = 'field_grievance_department_tid';
    $fields_to_hide[] = 'field_grievance_department';
    */
    $fields_to_hide[] = 'field_grievance_work_status';
    $fields_to_hide[] = 'field_grievance_shift';
    $fields_to_hide[] = 'field_grievance_daysoff';
    $fields_to_hide[] = 'field_grievance_ein';
    // $fields_to_hide[] = 'field_grievance_min';
    $fields_to_hide[] = 'field_grievance_corrected';
    $fields_to_hide[] = 'field_grievance_supervisor_name';
    $fields_to_hide[] = 'field_grievance_supervisor_title';
    $fields_to_hide[] = 'field_grievance_supervisor_unit';
    // $fields_to_hide[] = 'field_grievance_timss_search';
    // $fields_to_hide[] = 'field_grievance_timss_search_result';
  } else {
    $form['field_grievance_first_name']['und']['#title'] = 
      $form['field_grievance_first_name']['und'][0]['#title'] = 
      $form['field_grievance_first_name']['und'][0]['value']['#title'] = t('First Name');
    $form['field_grievance_last_name']['und']['#title'] = 
      $form['field_grievance_last_name']['und'][0]['#title'] = 
      $form['field_grievance_last_name']['und'][0]['value']['#title'] = t('Last Name');
    // $fields_to_hide[] = 'field_grievance_broughtby';
  }
  if ($fields_to_hide) {
    foreach ($fields_to_hide as $field_name) {
      $form[$field_name]['#access'] = FALSE;
    }
  }

  //
  // Show or hide fields based on value of the "Resolution Procedure"
  // 

  $resproc_tid = $status_term->field_grievance_resproc['und'][0]['tid'];
  $resproc_term = taxonomy_term_load($resproc_tid);
  if ($resproc_term) {
    $form['#groups']['group_grievance_resproc']->label = $resproc_term->name;
    $form['#groups']['group_grievance_resproc']->format_settings['label'] = $resproc_term->name;
    $form['#groups']['group_grievance_resproc']->format_settings['instance_settings']['description'] = $resproc_term->description;

    // @TODO These should be in admin settings
    $is_adr = $is_expedited_arbitration = $is_formal_arbitration = $is_mediation = FALSE;

    if (preg_match('/Expedited Arbitration/i', $resproc_term->name)) { $is_expedited_arbitration = TRUE; }
    if (preg_match('/Arbitration/i', $resproc_term->name) && !$is_expedited_arbitration) { $is_formal_arbitration = TRUE; }
    if (preg_match('/adr/i', $resproc_term->name)) { $is_adr = TRUE; }
    if (preg_match('/mediation/i', $resproc_term->name)) { $is_mediation = TRUE; }

    $form['field_grievance_resproc_an']['#access'] = ($is_expedited_arbitration || $is_formal_arbitration);
    $form['field_grievance_resproc_ea']['#access'] = ($is_expedited_arbitration || $is_formal_arbitration);
    $form['field_grievance_resproc_ua']['#access'] = ($is_expedited_arbitration || $is_formal_arbitration);
    $form['field_grievance_resproc_hd']['#access'] = ($is_expedited_arbitration || $is_formal_arbitration);
    $form['field_grievance_resproc_cd']['#access'] = ($is_expedited_arbitration || $is_formal_arbitration);
    $form['field_grievance_resproc_lh']['#access'] = $is_formal_arbitration;
    $form['field_grievance_resproc_er']['#access'] = ($is_adr || $is_mediation);
    $form['field_grievance_resproc_ur']['#access'] = ($is_adr || $is_mediation);

  } else {
    foreach ( $form['#groups']['group_grievance_resproc']->children as $field_name) {
      $form[$field_name]['#access'] = FALSE;
    }
  }

  //
  // Add some log timeline dates to the intake form
  //

  if (!$grievance_node->nid) {
    for ($i=1; $i<=5; ++$i) {
      $log_type_tid = variable_get("grievance_log_intake_${i}_type", '');
      if (!$log_type_tid) { continue; }
      $log_type_term = taxonomy_term_load($log_type_tid);

      $weight = $form['field_grievance_date_2']['#weight'] + (0.1 * $i);
      $form["grievance_intake_log_${i}"] = array(
        '#type' => 'date_popup',
        '#weight' => $weight,
        '#tree' => TRUE,
        '#title' => $log_type_term->name,
        '#date_format'    => 'm/d/Y H:i',
        '#date_label_position'  => 'none',
        '#prefix' => '<div class="form-item form-type-date-popup">',
        '#suffix' => '</div>'
      );
      $form['#groups']['group_grievance_form']->children[] = "grievance_intake_log_${i}";
      $form['#group_children']["grievance_intake_log_${i}"] = 'group_grievance_form';
    }
  }

  //
  // Add the information request sets
  //

  if ($form['field_grievance_document_types']['#access']) {
    $query = new EntityFieldQuery;
    $query->entityCondition('entity_type', 'node');
    $query->entityCondition('bundle', 'grievance_irset');
    $query->propertyOrderBy('title');
    $stmt = $query->execute();
    if ($stmt['node']) {
      $weight = $form['field_grievance_document_types']['#weight'] - 0.1;
      $irset_nodes = node_load_multiple(array_keys($stmt['node']));
      $irset_iroptions = array();
      foreach ($irset_nodes as $irset_node) {
        $irset_iroptions[$irset_node->nid] = $irset_node->title;
      }
      $form['irsets'] = array(
        '#type' => 'checkboxes',
        '#tree' => TRUE,
        '#title' => "Sets",
        '#options' => $irset_iroptions,
        '#weight' => $weight
      );
      $form['#groups']['group_inforeq']->children[] = "irsets";
      $form['#group_children']["irsets"] = 'group_inforeq';

      $settings_for_js = array();
      foreach ($irset_nodes as $irset_nid => $irset_node) {
        if ($irset_node->field_grievance_document_types['und'][0]) {
          foreach ($irset_node->field_grievance_document_types['und'] as $tmp) {
            $settings_for_js[$irset_nid][] = $tmp['tid'];
          }
        }
      }
      drupal_add_js(array('grievance_form_irsets' => $settings_for_js), 'setting');
    }
  }
}

