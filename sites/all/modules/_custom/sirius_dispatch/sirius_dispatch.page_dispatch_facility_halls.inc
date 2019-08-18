<?php

function sirius_node_tab_dispatch_facility_halls($facility_node) {
  drupal_set_title($facility_node->title . ': ' . t('Halls'));
  $entity_nid = $facility_node->nid;

  $result = views_embed_view('sirius_dispatch_facility_halls', 'block', $entity_nid);
  $result .= grievance_node_tab_get_addlink("sirius-dispatch-facility-hall", array("sirius_dispatch_facility" => $entity_nid), 'Add a new hall');

  return $result;
}