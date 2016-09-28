<?php

function grievance_form_grievance_node_callback_replace($form, $form_state) {

  $commands = array();
  $commands[] = ajax_command_replace("#grievance-form-contract-link", render($form['#groups']['group_grievance_form']->format_settings['instance_settings']['description']));
  $commands[] = ajax_command_replace("#grievance-form-contacts-link", render($form['field_grievance_contact_selector']['und'][0]['markup']['#markup']));
  $commands[] = ajax_command_replace("#edit-field-grievance-clauseref", render($form['field_grievance_clauseref']));
  $commands[] = ajax_command_replace("#field-grievance-alert-date-wrapper", render($form['field_grievance_alert_date']));
  $commands[] = ajax_command_replace("#field-grievance-alert-tid-wrapper", render($form['field_grievance_alert_tid']));
  $commands[] = ajax_command_invoke(NULL, "grievance_select_set", array('edit-field-grievance-contract-tplt-und', $form_state['grievance_contract_tplt_nid']));

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
  // Base node
  $grievance_node = $form_state['node'];

  // Changing various fields causes the form to recompute
  $form['field_grievance_shop']['und'][0]['target_id']['#ajax'] = array(
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

  // Reset the status update date
  $form['field_grievance_status_date']['und'][0]['#default_value']['value'] = date('Y-m-d');

  // Find the current user
  global $user;
  if (!$user) { return; }
  $user_full = user_load($user->uid);
  if (!$user_full) { return; }

  // Get the shop ID
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

  // Set the contract template from the contract
  if ($shop_node) {
    $form_state['grievance_contract_tplt_nid'] = $shop_node->field_grievance_contract_tplt['und'][0]['target_id'];
    if (!$form['field_grievance_contract_tplt']['und']['#default_value']) {
      $form['field_grievance_contract_tplt']['und']['#default_value'] = $shop_node->field_grievance_contract_tplt['und'][0]['target_id'];
    }
  }

  /*
  // Set the list of clauses
  $contract_tplt_id = grievance_form_current_value('field_grievance_contract_tplt', 'target_id', $form, $form_state);
  if ($contract_tplt_id) {
    $clauses_view = views_get_view('grievance_contract_clauses');
    $clauses_view->args[0] = $contract_tplt_id;
    $clauses_view->execute('entityreference_1');
    $clauses_options = array();
    $clauses_options['_none'] = t('- None -');
    foreach ($clauses_view->result as $clause) {
      $clauses_options[$clause->nid] = 
        'Article ' . 
        $clause->field_field_grievance_section_number[0]['rendered']['#markup'] . 
        ': ' . 
        $clause->field_field_grievance_contract_section[0]['rendered']['#markup'] . 
        ' - Clause ' .
        $clause->field_data_field_grievance_label_field_grievance_label_value .
        ': ' . $clause->node_title;
    }
    $form['field_grievance_clauseref']['und']['#options'] = $clauses_options;
    $idx = 0;
    while (isset($form['field_grievance_clauseref']['und'][$idx]['target_id']['#options'])) {
      $form['field_grievance_clauseref']['und'][$idx]['target_id']['#options'] = $clauses_options;
      ++$idx;
    }
  }
  */

  // Link to contract
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

  // Recipients
  $status_tid = grievance_form_current_value('field_grievance_status', 'tid', $form, $form_state);

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

  // Set default values for manager, organizer, etc.
  // This should be on insert only, not edit.  We never set defaults on edit.
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

  $form['field_grievance_alert_date']['#prefix'] = '<div id="field-grievance-alert-date-wrapper">';
  $form['field_grievance_alert_date']['#suffix'] = '</div>';
  $contract_tplt_nid = grievance_form_current_value('field_grievance_contract_tplt', 'target_id', $form, $form_state);
  $status_tid = grievance_form_current_value('field_grievance_status', 'tid', $form, $form_state);
  $alert_date = grievance_form_current_value('field_grievance_alert_date', 'value', $form, $form_state);

  if (!$alert_date || $form_state['triggering_element']['#field_name'] == 'field_grievance_status') {
    if ($contract_tplt_nid && $status_tid) {
      $next_deadline = grievance_contract_next_deadline($contract_tplt_nid, $status_tid);
      // $form['field_grievance_alert_date']['und'][0]['#default_value'] = $next_deadline;
      $form_state['input']['field_grievance_alert_date']['und'][0]['value']['date'] = date('m/d/Y', strtotime($next_deadline));
    }
  }

  //
  // Compute the next alert tid
  //

  $form['field_grievance_alert_tid']['#prefix'] = '<div id="field-grievance-alert-tid-wrapper">';
  $form['field_grievance_alert_tid']['#suffix'] = '</div>';
  $alert_tid = grievance_form_current_value('field_grievance_alert_tid', 'tid', $form, $form_state);

  if (!$alert_tid || $form_state['triggering_element']['#field_name'] == 'field_grievance_status') {
    if ($contract_tplt_nid && $status_tid) {
      $next_alert_tid = grievance_contract_next_alert_tid($contract_tplt_nid, $status_tid);
      $form['field_grievance_alert_tid']['und'][0]['#default_value'] = $next_alert_tid;
      $form_state['input']['field_grievance_alert_tid']['und'] = $next_alert_tid;
    }
  }
}
