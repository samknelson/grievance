<?php

function grievance_node_tab_settlements($node) {
  $entity_nid = $node->nid;

  $result = views_embed_view('grievance_settlements', 'block', $entity_nid);
  $result .= grievance_node_tab_get_addlink("grievance-settlement", array("grievance" => $entity_nid), 'Add a new settlement');

  return $result;
}

