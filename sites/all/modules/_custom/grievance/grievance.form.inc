<?php

function grievance_form_shop_callback_replace($form, $form_state) {
  return array(
    '#type' => 'ajax',
    '#commands' => array(
      ajax_command_replace("#grievance-form-contract-link", render($form['#groups']['group_grievance_form']->format_settings['instance_settings']['description'])),
      ajax_command_replace("#grievance-form-contacts-link", render($form['field_grievance_contact_selector']['und'][0]['markup']['#markup']))
    )
  );
}


function grievance_form_grievance_node_form_alter(&$form, &$form_state, $form_id) {
  // Add a callback when shop changes
  $form['field_grievance_shop']['und'][0]['target_id']['#ajax'] = array(
    'callback' => 'grievance_form_shop_callback_replace',
    'method' => 'replace',
  );

  // Find the current user
  global $user;
  if (!$user) { return; }
  $user_full = user_load($user->uid);
  if (!$user_full) { return; }

  // Base node
  $grievance_node = $form_state['node'];

  // Get the shop ID
  $shop_nid = $form_state['values']['field_grievance_shop']['und'][0]['target_id'];
  if (!$shop_nid) {
    $shop_nid = $grievance_node->field_grievance_shop['und'][0]['target_id'];
  }
  if (!$shop_nid) {
    $shop_nid = $user_full->field_grievance_shops['und'][0]['target_id'];
  }  
  $shop_node = node_load($shop_nid);

  // Link to contract
  $contract_link_text = '';
  if ($shop_nid) {
    $shop_node = node_load($shop_nid);
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
  $contacts = grievance_notify_conmpany_contacts($shop_nid);
  if ($contacts) {
    $contacts_markup = '';
    $contacts_markup .= t('The contacts below are associated with the shop or company.  Select the contact who should appear on the grievance form.');
    foreach ($contacts as $contact) {
      $contact_nid = $contact['nid'];
      $contact_node = node_load($contact_nid);
      $contact_node_view = node_view($contact_node, 'teaser');

      $contacts_markup .= '<div class="grievance-recipient">';
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
    if ($shop_nid) {
      $form['field_grievance_shop']['und'][0]['target_id']['#default_value'] = "$shop_node->title ($shop_node->nid)";
    }
  }
}
