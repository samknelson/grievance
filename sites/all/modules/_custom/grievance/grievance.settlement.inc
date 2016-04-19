<?php

function grievance_node_tab_settlements($node) {
  drupal_set_title($node->title . ': ' . t('Settlements'));
  $entity_nid = $node->nid;

  $result = views_embed_view('grievance_settlements', 'block', $entity_nid);
  $result .= grievance_node_tab_get_addlink("grievance-settlement", array("grievance" => $entity_nid));

  return $result;
}

