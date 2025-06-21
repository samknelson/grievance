<?php

function grievance_contacts_admin() {
  $form = array();

  $form['grievance_contacts_st_type'] = array(
    '#default_value' => variable_get('grievance_contacts_st_type', ''),
    '#title' => t('Shop Steward Contact Type'),
    '#type' => 'select',
    '#options' => taxonomy_allowed_values(field_info_field('field_grievance_contact_types')),
  );

  return system_settings_form($form);
}

function grievance_shop_node_tab_contacts_access($employer_node) {
  // "Grievance Contact" gets you in.
  if (user_access('grievance contact')) { return TRUE; }

  // Employers can see this tab for their own shop.
  if (user_access('sirius employer contact manager')) { 
    if (!$employer_node->nid) { return FALSE; }
    global $user;
    $user_full = user_load($user->uid);
    if ($user_full->field_grievance_shops['und'][0]['target_id'] == $employer_node->nid) { return TRUE; }
  }

  return FALSE;
}

function grievance_shop_node_tab_contacts($shop_node) {
  drupal_set_title($shop_node->title . ': ' . t('Contacts'));
  $entity_nid = $shop_node->nid;

  $result = '';
  $result .= views_embed_view('grievance_shop_contacts', 'block_1', $entity_nid);
  $result .= grievance_node_tab_get_addlink("grievance-shop-contact", array("grievance_shops" => $entity_nid), 'Add a contact');

  return $result;
}

function grievance_company_node_tab_contacts($company_node) {
  drupal_set_title($company_node->title . ': ' . t('Contacts'));
  $entity_nid = $company_node->nid;

  $result = '';
  $result .= views_embed_view('grievance_shop_contacts', 'block_2', $entity_nid);
  $result .= grievance_node_tab_get_addlink("grievance-shop-contact", array("grievance_company" => $entity_nid), 'Add a company contact');

  return $result;
}


function grievance_views_post_execute_shop_contacts(&$view) {
  if (!isset($view->result)) { return; }

  foreach ($view->result as &$row) {
    if (!isset($row->field_field_grievance_shops_1)) { continue; }
    if (!is_array($row->field_field_grievance_shops_1)) { continue; }
    foreach ($row->field_field_grievance_shops_1 as &$item) {
      $item['rendered']['#markup'] = 'Title for ' . $item['raw']['target_id'];
      $sql = 'select field_grievance_external_id_value from field_data_field_grievance_external_id ';
      $sql .= 'where deleted = 0 ';
      $sql .= 'and entity_type = \'node\' ';
      $sql .= 'and bundle = \'grievance_shop\' ';
      $sql .= 'and entity_id = :nid ';
      $stmt = db_query($sql, array(':nid' => $item['raw']['target_id']));
      $hr = $stmt->fetchAssoc();
      if ($hr) {
        $item['rendered']['#markup'] = $hr['field_grievance_external_id_value'];
      } else {
        $item['rendered']['#markup'] = '';
      }
    }
  }
}

function grievance_contacts_create_selector($shop_nid, $status_tid, $which, $is_stewards) {
  $params = array(
    'shop_nid' => $shop_nid,
    'status_tid' => $status_tid
  );
  $steward_type_tid = sirius_get_special_tid('grievance_contact_types|steward');
  if (!$steward_type_tid) { $steward_type_tid = variable_get('grievance_contacts_st_type', ''); } // legacy

  if ($is_stewards) {
    $params['include_contact_type_tid'] = $steward_type_tid;
  } else {
    $params['exclude_contact_type_tid'] = $steward_type_tid;
  }
  $contacts = grievance_notify_company_contacts($params);

  if ($contacts) {
    $contacts_markup = '';
    $contacts_markup .= t('The contacts below are associated with the shop or company.  Select the contact who should appear on the grievance form.');

    $contact_count_company = 0;
    $contact_count_shop = 0;
    foreach ($contacts as $contact) {
      if ($contact['shop_contact']) { ++$contact_count_shop; }
      if ($contact['company_contact']) { ++$contact_count_company; }
    }


    $contacts_markup .= '<div style="clear: both" id="grievance_contact_controls">';

    if ($contact_count_shop) {
      $contacts_markup .= '<a href="#" id="grievance-' . $which . '-hide-shop">[' . t('Hide shop contacts') . ']</a> ';
      $contacts_markup .= '<a href="#" id="grievance-' . $which . '-show-shop" style="display: none;">[' . t('Show shop contacts') . ']</a> ';
    }

    if ($contact_count_company) {
      $contacts_markup .= '<a href="#" id="grievance-' . $which . '-hide-company">[' . t('Hide company contacts') . ']</a> ';
      $contacts_markup .= '<a href="#" id="grievance-' . $which . '-show-company" style="display: none;">[' . t('Show company contacts') . ']</a> ';
    }

    $contacts_markup .= '</div>';

    foreach ($contacts as $contact) {
      $contact_nid = $contact['nid'];
      $contact_node = node_load($contact_nid);
      $contact_node_view = node_view($contact_node, 'teaser');

      $classes = 'grievance-recipient';
      if ($contact['shop_contact']) {
        $classes .= ' grievance-recipient-shop';
      }
      if ($contact['company_contact']) {
        $classes .= ' grievance-recipient-company';
      }

      $style = '';

      $contacts_markup .= '<div class="' . $classes . '" style="' . $style . '">';
      $contacts_markup .= drupal_render($contact_node_view);
      $contacts_markup .= '</div>';
    }
    $contacts_markup .= '<div style="clear: both">&nbsp;</div>';
  } else {
    $contacts_markup = t('This shop has no contacts configured.  Please enter the company contact manually.');
  }
  return '<div id="grievance-form-' . $which . 's-link">' . $contacts_markup . '</div>';
}

/**
 * Modify the node form, removing elements that users shouldn't have permission on.
 */

function grievance_form_grievance_shop_contact_node_form_alter(&$form, &$form_state, $form_id) {
  // The "grievnace contact" permission gives you access on anything.
  if (user_access('grievance contact')) { return TRUE; }

  global $user;
  if (!$user || !$user->uid) { $form['#access'] = FALSE; return; }
  $user_full = user_load($user->uid);

  // The shop should have been set in the URL. In any event, there's no access to it.
  $form['field_grievance_shops']['#access'] = FALSE;

  // If we're creating a new node, then the default value for the "shops" field must have been set to a shop that we have access to
  $contact_nid = $form['nid']['#value'];
  if (!$contact_nid) {
    $my_shop_nids = sirius_fieldvals($user_full, 'field_grievance_shops', 'target_id');
    $str = $form['field_grievance_shops']['und'][0]['target_id']['#default_value'];
    if (!$str) { 
      drupal_set_message("Please set a default value for the shops field.", 'error');
      $form['#access'] = FALSE; 
      return;
    }
    if (!preg_match('/.*\((\d+)\)$/', $str, $matches)) {
      drupal_set_message("Default value for the shops fields is [$str]", 'error');
      $form['#access'] = FALSE; 
      return;
    }
    $shop_nid = $matches[1];
    if (!in_array($shop_nid, $my_shop_nids)) {
      drupal_set_message("You do not have permission to create a contact for the shop $shop_nid.", 'error');
      $form['#access'] = FALSE; 
      return;
    }
  }

  // Various other fields are hidden from non-admin users
  $form['field_grievance_min']['#access'] = FALSE;
  $form['field_grievance_company']['#access'] = FALSE;

  // "Contact Type" is restricted to the ones that we have permission on
  $contact_types = sirius_taxonomy_load('grievance_contact_types', 'tid', 'full');
  $user_rids = array_keys($user->roles);
  foreach ($contact_types as $tid => $term) {
    $json = json_decode($term->field_sirius_json['und'][0]['value'], TRUE);
    $can_access = FALSE;
    foreach ($user_rids as $rid) { 
      if ($json['permissions']['employer_update'][$rid]) { $can_access = TRUE; }
    }
    if (!$can_access) { unset($form['field_grievance_contact_types']['und']['#options'][$tid]); }
  }
  $form['field_grievance_contact_types']['und']['#required'] = TRUE;
}

/**
 * Remove from the view any contacts that we don't have access to
 */

function grievance_contacts_views_pre_render(&$view) {
  // If there are no results, ignore.
  if (!$view->result) { return; }

  // If we have "grievance contact" permission, we can do anything
  if (user_access('grievance contact')) { return; }

  // Ok, we need to filter this view.
  $new_result = array();
  foreach ($view->result as $row) {
    if (!node_access('view', $row->_field_data['nid']['entity'])) { continue; }
    $new_result[] = $row;
  }
  $view->result = $new_result;
}

function grievance_node_access_shop_contact($contact_node, $op, $account) {
  // If we have "grievance contact" permission, we can do anything
  if (user_access('grievance contact')) { return NODE_ACCESS_IGNORE; }

  // Load the contact type taxonomy (once)
  static $contact_types = NULL;
  if (!$contact_types) { $contact_types = sirius_taxonomy_load('grievance_contact_types', 'tid', 'full');  }

  // Get the user roles
  if (!$account || $acount->uid || !$account->roles) { return NODE_ACCESS_DENY; }
  $rids = array_keys($account->roles);
  $account_full = user_load($account->uid);

  // Get the contact type tids
  $tids = sirius_fieldvals($contact_node, 'field_grievance_contact_types', 'tid');

  // Is this my employer?
  $contact_employer_nids = sirius_fieldvals($contact_node, 'field_grievance_shops', 'target_id');
  $user_employer_nids = sirius_fieldvals($account_full, 'field_grievance_shops', 'target_id');
  $my_employer = array_intersect($user_employer_nids, $contact_employer_nids);

  // Is this my industry?
  $worker_node = sirius_worker_get($account->uid);
  // $worker_node = node_load(23732);
  $user_industry_tids = sirius_fieldvals($worker_node, 'field_sirius_industry', 'tid');
  $contact_employer_nodes = node_load_multiple($contact_employer_nids);
  $contact_industry_tids = array();
  foreach ($contact_employer_nodes as $contact_employer_nid => $contact_employer_node) {
    $contact_industry_tids += sirius_fieldvals($contact_employer_node, 'field_sirius_industry', 'tid');
  }
  $my_industry = array_intersect($user_industry_tids, $contact_industry_tids);


  // If the op is "edit", check for "edit "
  foreach ($tids as $tid) {
    $json = json_decode($contact_types[$tid]->field_sirius_json['und'][0]['value'], TRUE);
    foreach ($rids as $rid) {
      if ($op == 'update' && $my_employer && $json['permissions']['employer_update'][$rid])       { return NODE_ACCESS_ALLOW; }
      if ($op == 'view' && $my_employer && $json['permissions']['employer_view'][$rid])       { return NODE_ACCESS_ALLOW; }
      if ($op == 'view' && $my_industry && $json['permissions']['industry_view'][$rid])   { return NODE_ACCESS_ALLOW; }
      if ($op == 'view' && $json['permissions']['all_view'][$rid])                        { return NODE_ACCESS_ALLOW; }
    }
  }
  if ($can_access) { return NODE_ACCESS_ALLOW; }

  return NODE_ACCESS_DENY;

}


function grievance_contact_sirius_jsonfield_form_definitions() {
  $roles = user_roles();

  $form['permissions'] = array(
    '#type' => 'fieldset',
    '#title' => 'Permissions',
  );
  $form['permissions']['employer_view'] = array(
    '#title' => 'Roles that can VIEW their own employer\'s contacts of this type.',
    '#type' => 'checkboxes', 
    '#options' => $roles,
  );
  $form['permissions']['employer_update'] = array(
    '#title' => 'Roles that can EDIT their own employer\'s contacts of this type.',
    '#type' => 'checkboxes', 
    '#options' => $roles,
  );
  $form['permissions']['industry_view'] = array(
    '#title' => 'Roles that can VIEW any contacts of this type in their industry.',
    '#type' => 'checkboxes', 
    '#options' => $roles,
  );
  $form['permissions']['all_view'] = array(
    '#title' => 'Roles that can VIEW any contacts of this type.',
    '#type' => 'checkboxes', 
    '#options' => $roles,
  );

  $definitions['taxonomy_term']['grievance_contact_types']['field_sirius_json'] = $form;

  return $definitions;
}


function grievance_contact_views_exposed_form_alter(&$form, &$form_state) {
  // No changes for admin users.
  if (user_access('grievance contact')) { return; }

  // "Contact Type" is restricted to the ones that we have permission on
  global $user;
  $user_full = user_load($user->uid);
  $user_rids = array_keys($user->roles);
  $contact_types = sirius_taxonomy_load('grievance_contact_types', 'tid', 'full');
  foreach ($contact_types as $tid => $term) {
    $json = json_decode($term->field_sirius_json['und'][0]['value'], TRUE);
    $can_access = FALSE;
    foreach ($user_rids as $rid) { 
      if ($json['permissions']['industry_view'][$rid]) { $can_access = TRUE; }
      if ($json['permissions']['all_view'][$rid]) { $can_access = TRUE; }
    }
    if (!$can_access) { unset($form['field_grievance_contact_types_tid']['#options'][$tid]); }
  }

  // Restrict the list of employers to those within our industry.
  $worker_node = node_load($user_full->field_sirius_worker['und'][0]['target_id']);
  $industry_tids = sirius_fieldvals($worker_node, 'field_sirius_industry', 'tid');
  if ($industry_tids) {
    $sql = "select distinct nid, title ";
    $sql .= "from node ";
    $sql .= "left join field_data_field_sirius_industry on field_data_field_sirius_industry.entity_type = 'node' and field_data_field_sirius_industry.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_active on field_data_field_sirius_active.entity_type = 'node' and field_data_field_sirius_active.entity_id = node.nid ";
    $sql .= "where node.type = 'grievance_shop' ";
    $sql .= "and field_sirius_industry_tid in (:industry_tids) ";
    $sql .= "order by title ";
    $stmt = db_query($sql, array(':industry_tids' => $industry_tids));
    $options = array('All' => t('- Any -'));
    while ($hr = $stmt->fetchAssoc()) {
      $options[$hr['nid']] = $hr['title'];
    }
    $form['field_grievance_shop_target_id']['#options'] = $options;
  }
}