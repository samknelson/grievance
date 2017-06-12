<?php

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