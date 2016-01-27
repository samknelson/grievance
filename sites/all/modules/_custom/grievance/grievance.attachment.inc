<?php

function grievance_node_tab_attachments($node) {
  drupal_set_title($node->title . ': ' . t('Attachments'));
  $entity_nid = $node->nid;

  $result = views_embed_view('grievance_attachments', 'block', $entity_nid);
  $result .= grievance_node_tab_get_addlink("grievance-attachment", array("grievance" => $entity_nid));

  return $result;
}

