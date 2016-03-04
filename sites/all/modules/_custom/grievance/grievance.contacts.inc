<?php

function grievance_shop_node_tab_contacts($shop_node) {
  drupal_set_title($shop_node->title . ': ' . t('Contacts'));
  $entity_nid = $shop_node->nid;

  $result = views_embed_view('grievance_shop_contacts', 'block_1', $entity_nid);
  $result .= grievance_node_tab_get_addlink("grievance-shop-contact", array("grievance_shops" => $entity_nid));

  return $result;
}

function grievance_company_node_tab_contacts($company_node) {
  drupal_set_title($company_node->title . ': ' . t('Contacts'));
  $entity_nid = $company_node->nid;

  $result = views_embed_view('grievance_shop_contacts', 'block_2', $entity_nid);
  $result .= grievance_node_tab_get_addlink("grievance-shop-contact", array("grievance_company" => $entity_nid));

  return $result;
}