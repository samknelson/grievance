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
  $result .= grievance_node_tab_get_addlink("grievance-shop-contact", array("grievance_company" => $entity_nid), 'Add a company');

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
  $steward_type_tid = variable_get('grievance_contacts_st_type', '');
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
