<?php

function grievance_form_grievance_node_callback_replace($form, $form_state) {
  $commands = array();
  $commands[] = ajax_command_replace("#grievance-node-form-replaceme", render($form));
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

  $form['field_grievance_shop']['und'][0]['target_id']['#ajax'] = array(
    'callback' => 'grievance_form_grievance_node_callback_replace',
    'method' => 'replace',
  );
  $form['field_grievance_type']['und'][0]['tid']['#ajax'] = array(
    'callback' => 'grievance_form_grievance_node_callback_replace',
    'method' => 'replace',
  );
  $form['field_grievance_contract_tplt']['und']['#ajax'] = array(
    'callback' => 'grievance_form_grievance_node_callback_replace',
    'method' => 'replace',
  );
  $form['field_grievance_status']['und']['#ajax'] = array(
    'callback' => 'grievance_form_grievance_node_callback_replace',
    'method' => 'replace',
  );
  $form['field_grievance_classaction']['und']['#ajax'] = array(
    'callback' => 'grievance_form_grievance_node_callback_replace',
    'method' => 'replace',
  );

  //
  // Override the "Add More" buttons, and then simulate their operation.
  //

  $form['field_grievance_clauseref']['und']['add_more']['#ajax'] = array(
    'callback' => 'grievance_form_grievance_node_callback_replace',
    'method' => 'replace',
  );

  //
  // Open the correct accordion group
  //

  if ($form_state['triggering_element']['#field_name'] == 'field_grievance_type') {
    $form['#groups']['group_grievance_status']->format_settings['formatter'] = 'closed';
    $form['#groups']['group_grievance_form']->format_settings['formatter'] = 'open';
  }
  if ($form_state['triggering_element']['#field_name'] == 'field_grievance_clauseref') {
    $form['#groups']['group_grievance_status']->format_settings['formatter'] = 'closed';
    $form['#groups']['group_grievance_form']->format_settings['formatter'] = 'open';
  }
  //////////////////////////////////////////////////////
  // These things happen on rebuild
  //////////////////////////////////////////////////////

  //
  // Set the status date, for status changes
  // 

  $form['field_grievance_status_date']['und'][0]['#default_value']['value'] = date('Y-m-d');

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
  // Set the contract template from the shop
  // 

  $contract_tplt_id = grievance_form_current_value('field_grievance_contract_tplt', 'target_id', $form, $form_state);
  if ($shop_node) {
    if (!$contract_tplt_id || $form_state['triggering_element']['#field_name'] == 'field_grievance_shop' || $form['field_grievance_contract_tplt']['#disabled']) {
      $contract_tplt_id = $shop_node->field_grievance_contract_tplt['und'][0]['target_id'];

      if ($form['field_grievance_contract_tplt']['#disabled']) {
        $form['field_grievance_contract_tplt']['und']['#default_value'][0] = $contract_tplt_id;
        $form['field_grievance_contract_tplt']['und']['#value'] = $form['field_grievance_contract_tplt']['und']['#default_value'];
      } else {
        $form_state['input']['field_grievance_contract_tplt']['und'] = $contract_tplt_id;
      }
    }
  }


  // 
  // Set the list of clauses
  //

  $clauses_options = array();
  if ($contract_tplt_id) {
    $clauses_view = views_get_view('grievance_contract_clauses');
    $clauses_view->args[0] = $contract_tplt_id;
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

      $contract_link_text = '<strong>' . 
        t('The contract for ') .
        $shop_node->title . 
        t(' can be downloaded ') .
        l(t('here'), $contract_url, array('attributes' => array('target'=>'_blank'))) .
        '</strong>';
    }
  }
  $form['#groups']['group_grievance_form']->format_settings['instance_settings']['description'] = 
    '<div id="grievance-form-contract-link">' . $contract_link_text . '</div>';

  // 
  // Recipients
  //

  $status_tid = grievance_form_current_value('field_grievance_status', 'tid', $form, $form_state);
  $status_term = taxonomy_term_load($status_tid);

  $contacts = grievance_notify_company_contacts($shop_nid, $status_tid);
  if ($contacts) {
    $contacts_markup = '';
    $contacts_markup .= t('The contacts below are associated with the shop or company.  Select the contact who should appear on the grievance form.');

    $contact_count_company = 0;
    $contact_count_shop = 0;
    $contact_count_statusmatch = 0;
    $contact_count_nostatusmatch = 0;
    foreach ($contacts as $contact) {
      if ($contact['statusmatch']) { ++$contact_count_statusmatch; } else { ++$contact_count_nostatusmatch; }
      if ($contact['shop_contact']) { ++$contact_count_shop; }
      if ($contact['company_contact']) { ++$contact_count_company; }
    }


    $contacts_markup .= '<div style="clear: both" id="grievance_contact_controls">';

    if ($contact_count_shop) {
      $contacts_markup .= '<a href="#" id="grievance-contact-hide-shop">[' . t('Hide shop contacts') . ']</a> ';
      $contacts_markup .= '<a href="#" id="grievance-contact-show-shop" style="display: none;">[' . t('Show shop contacts') . ']</a> ';
    }

    if ($contact_count_company) {
      $contacts_markup .= '<a href="#" id="grievance-contact-hide-company">[' . t('Hide company contacts') . ']</a> ';
      $contacts_markup .= '<a href="#" id="grievance-contact-show-company" style="display: none;">[' . t('Show company contacts') . ']</a> ';
    }

    $grievance_contacts_hide_nostatusmatch = variable_get('grievance_contacts_hide_nostatusmatch', '');
    if ($grievance_contacts_hide_nostatusmatch == "Yes") {
      if ($contact_count_nostatusmatch) {
        $contacts_markup .= '<a href="#" id="grievance-contact-hide-nostatusmatch" style="display: none;">[' . t('Hide non-matching statuses') . ']</a> ';
        $contacts_markup .= '<a href="#" id="grievance-contact-show-nostatusmatch">[' . t('Show non-matching statuses') . ']</a> ';
      }
    } else {
      if ($contact_count_statusmatch) {
        $contacts_markup .= '<a href="#" id="grievance-contact-hide-nostatusmatch">[' . t('Hide non-matching statuses') . ']</a> ';
        $contacts_markup .= '<a href="#" id="grievance-contact-show-nostatusmatch" style="display: none;">[' . t('Show non-matching statuses') . ']</a> ';
      }
    }

    $contacts_markup .= '</div>';

    foreach ($contacts as $contact) {
      $contact_nid = $contact['nid'];
      $contact_node = node_load($contact_nid);
      $contact_node_view = node_view($contact_node, 'teaser');

      $classes = 'grievance-recipient';
      if ($contact['statusmatch']) {
        $classes .= ' grievance-recipient-statusmatch';
      } else {
        $classes .= ' grievance-recipient-nostatusmatch';
      }

      if ($contact['shop_contact']) {
        $classes .= ' grievance-recipient-shop';
      }
      if ($contact['company_contact']) {
        $classes .= ' grievance-recipient-company';
      }

      $style = '';
      if ($grievance_contacts_hide_nostatusmatch == "Yes" && !$contact['statusmatch']) {
        $style .= 'display: none;';
      }

      $contacts_markup .= '<div class="' . $classes . '" style="' . $style . '">';
      $contacts_markup .= drupal_render($contact_node_view);
      $contacts_markup .= '</div>';
    }
    $contacts_markup .= '<div style="clear: both">&nbsp;</div>';
  } else {
    $contacts_markup = t('This shop has no contacts configured.  Please enter the company contact manually.');
  }
  $form['field_grievance_contact_selector']['und'][0]['markup']['#markup'] = '<div id="grievance-form-contacts-link">' . $contacts_markup . '</div>';

  // 
  // Set default values for manager, organizer, etc.
  // (This should be on insert only, not edit.  We never set defaults on edit.)
  //

  if (!$grievance_node->nid) {
    // Filed by
    $form['field_grievance_rep_filed']['und'][0]['target_id']['#default_value'] = "$user_full->name ($user_full->uid)";

    // leads
    $leads = array();
    if (isset($user_full->field_grievance_rep_lead['und'])) {
      foreach ($user_full->field_grievance_rep_lead['und'] as $tmp) {
        $lead_uid = $tmp['target_id'];
        $leads[] = user_load($lead_uid);
      }
    }

    // Organizers (and leads from organizers)
    $organizers = array();
    if (isset($user_full->field_grievance_rep_organizer['und'])) {
      foreach ($user_full->field_grievance_rep_organizer['und'] as $tmp) {
        $organizer_uid = $tmp['target_id'];
        $organizer = user_load($organizer_uid);
        $organizers[] = $organizer;
        if (isset($organizer->field_grievance_rep_lead['und'])) {
          foreach ($organizer->field_grievance_rep_lead['und'] as $tmp) {
            $lead_uid = $tmp['target_id'];
            $leads[] = user_load($lead_uid);
          }
        }
      }
    }

    $delta = 0;
    foreach ($organizers as $organizer) {
      if ($organizer) {
        $form['field_grievance_rep_organizer']['und'][$delta]['target_id']['#default_value'] = "$organizer->name ($organizer->uid)";
        ++$delta;
      }
    }

    $delta = 0;
    foreach ($leads as $lead) {
      if ($lead) {
        $form['field_grievance_rep_lead']['und'][$delta]['target_id']['#default_value'] = "$lead->name ($lead->uid)";
        ++$delta;
      }
    }

    // Manager
    $manager = variable_get('grievance_rep_manager', '');
    if ($manager) {
      if ($manager) {
        $form['field_grievance_rep_manager']['und'][0]['target_id']['#default_value'] = "$manager[entity_label] ($manager[entity_id])";
      }
    }

    // Obeserver
    $watching = variable_get('grievance_rep_watching', '');
    if ($watching) {
      $form['field_grievance_rep_watching']['und'][0]['target_id']['#default_value'] = "$watching[entity_label] ($watching[entity_id])";
    }

    // Shop
    if ($shop_node) {
      $form['field_grievance_shop']['und'][0]['target_id']['#default_value'] = "$shop_node->title ($shop_node->nid)";
    }
  }

  //
  // Compute the next deadline date
  //

  $contract_tplt_nid = grievance_form_current_value('field_grievance_contract_tplt', 'target_id', $form, $form_state);
  if ($contract_tplt_nid) { 
    $contract_tplt_node = node_load($contract_tplt_nid);
  }
  
  $status_tid = grievance_form_current_value('field_grievance_status', 'tid', $form, $form_state);
  $alert_date = grievance_form_current_value('field_grievance_alert_date', 'value', $form, $form_state);


  if (!$alert_date || $form_state['triggering_element']['#field_name'] == 'field_grievance_status') {
    if ($contract_tplt_nid && $status_tid) {
      $next_deadline = grievance_contract_next_deadline($contract_tplt_nid, $status_tid);
    
      if ($next_deadline) {
        if ($form['field_grievance_alert_date']['#disabled']) {
          $form['field_grievance_alert_date']["und"][0]['#default_value']['value'] = date('m/d/Y', strtotime($next_deadline));
          $form['field_grievance_alert_date']["und"][0]['#value'] = $form['field_grievance_alert_date']["und"][0]['#default_value'];
          $form['field_grievance_alert_date']['und'][0]['#date_items']['value'] =  date('Y-m-d', strtotime($next_deadline));
        } else {
          $form_state['input']['field_grievance_alert_date']['und'][0]['value']['date'] = date('m/d/Y', strtotime($next_deadline));
        }
      }
    }
  }

  //
  // Compute the default information requests
  //

  $document_types = grievance_form_current_value('field_grievance_document_types', 'value', $form, $form_state);
  if (!$document_types || $form_state['triggering_element']['#field_name'] == 'field_grievance_type') {
    $type_tid = grievance_form_current_value('field_grievance_type', 'tid', $form, $form_state);
    $type_term = taxonomy_term_load($type_tid);
    $val = array();
    if ($type_term && $type_term->field_grievance_document_types['und']) {
      foreach ($type_term->field_grievance_document_types['und'] as $tmp) {
        $val[$tmp['tid']] = $tmp['tid'];
      }
    }
    $form_state['input']['field_grievance_document_types']['und'] = $val;
  }

  //
  // Compute the next alert tid
  //

  $alert_tid = grievance_form_current_value('field_grievance_alert_tid', 'tid', $form, $form_state);

  if (!$alert_tid || $form_state['triggering_element']['#field_name'] == 'field_grievance_status') {
    if ($contract_tplt_nid && $status_tid) {
      $next_alert_tid = grievance_contract_next_alert_tid($contract_tplt_nid, $status_tid);      
      if ($form['field_grievance_alert_tid']['#disabled']) {
        $form['field_grievance_alert_tid']['und']['#default_value'][0] = $next_alert_tid;
        $form['field_grievance_alert_tid']['und']['#value'] = $form['field_grievance_alert_tid']['und']['#default_value'];
      } else {
        $form_state['input']['field_grievance_alert_tid']['und'] = $next_alert_tid;
      }
    }
  }

  //
  // Show or hide fields based on value of the class action toggle
  // 

  $classaction = grievance_form_current_value('field_grievance_classaction', 'value', $form, $form_state);
  $field_to_hide = array();
  if ($classaction == 'Yes') {
    $fields_to_hide[] = 'field_grievance_first_name';
    $fields_to_hide[] = 'field_grievance_last_name';
    $fields_to_hide[] = 'field_grievance_phone';
    $fields_to_hide[] = 'field_grievance_email';
    $fields_to_hide[] = 'field_grievance_address';
    $fields_to_hide[] = 'field_grievance_address_2';
    $fields_to_hide[] = 'field_grievance_city';
    $fields_to_hide[] = 'field_grievance_state';
    $fields_to_hide[] = 'field_grievance_zip';
    $fields_to_hide[] = 'field_grievance_hire_date';
    $fields_to_hide[] = 'field_grievance_class_tid';
    $fields_to_hide[] = 'field_grievance_classification';
    $fields_to_hide[] = 'field_grievance_department_tid';
    $fields_to_hide[] = 'field_grievance_department';
    $fields_to_hide[] = 'field_grievance_work_status';
    $fields_to_hide[] = 'field_grievance_shift';
    $fields_to_hide[] = 'field_grievance_daysoff';
    $fields_to_hide[] = 'field_grievance_ein';
    $fields_to_hide[] = 'field_grievance_min';
    $fields_to_hide[] = 'field_grievance_corrected';
    $fields_to_hide[] = 'field_grievance_supervisor_name';
    $fields_to_hide[] = 'field_grievance_supervisor_title';
    $fields_to_hide[] = 'field_grievance_supervisor_unit';
    $fields_to_hide[] = 'field_grievance_timss_search';
    $fields_to_hide[] = 'field_grievance_timss_search_result';
  } else {
    $fields_to_hide[] = 'field_grievance_broughtby';
  }
  foreach ($fields_to_hide as $field_name) {
    $form[$field_name]['#access'] = FALSE;
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

    // * Length of Hearing (full arbitration)
    // * Arbitrator Name (full & expedited arbitration)
    // * Employer Representative (adr & mediation)
    // * Employer Attorney (full & expedited arbitration)
    // * Union Representative (adr & mediation)
    // Outcomes (all four) (list)

    // @TODO These should be in admin settings
    $is_adr = $is_expedited_arbitration = $is_formal_arbitration = $is_mediation = FALSE;

    if (preg_match('/Expedited Arbitration/i', $resproc_term->name)) { $is_expedited_arbitration = TRUE; }
    if (preg_match('/Arbitration/i', $resproc_term->name) && !$is_expedited_arbitration) { $is_formal_arbitration = TRUE; }
    if (preg_match('/adr/i', $resproc_term->name)) { $is_adr = TRUE; }
    if (preg_match('/mediation/i', $resproc_term->name)) { $is_mediation = TRUE; }

    $form['field_grievance_resproc_an']['#access'] = ($is_expedited_arbitration || $is_formal_arbitration);
    $form['field_grievance_resproc_ea']['#access'] = ($is_expedited_arbitration || $is_formal_arbitration);
    $form['field_grievance_resproc_lh']['#access'] = $is_formal_arbitration;
    $form['field_grievance_resproc_er']['#access'] = ($is_adr || $is_mediation);
    $form['field_grievance_resproc_ur']['#access'] = ($is_adr || $is_mediation);

  } else {
    foreach ( $form['#groups']['group_grievance_resproc']->children as $field_name) {
      $form[$field_name]['#access'] = FALSE;
    }
  }
}
