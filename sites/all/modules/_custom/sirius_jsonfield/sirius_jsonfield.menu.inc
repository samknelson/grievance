<?php

/**
 * Tabs: Which node type has which tabs. 
 *
 * Much of the functionality is via local tasks in nodes.  For instance, nodes of type "worker" have a local tab called "housing" which 
 * shows housing-related functions. 
 */
 
function sirius_jsonfield_node_tabs() {
  $node_tabs = array();

  /*
  $node_tabs['sirius_json_definition']['sirius_json_definition_edit'] = array(
    'title' => 'Definition',
    'description' => '',
    'callback' => 'sirius_json_definition_edit',
    'permissions' => array('sirius admin'),
    'weight' => 1,
  );
  */

  return $node_tabs;
}

function sirius_jsonfield_menu() {
  $items = array();

  return $items;
}
