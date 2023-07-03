<?php

/**
 * Implement hook_node_tabs
 */
 
function sirius_sched_node_tabs() {
  $node_tabs = array();

  // sirius_node_tab_set_module('sirius_sched', $node_tabs);

  return $node_tabs;
}

function sirius_sched_menu() {
  $items = array();

  return $items;
}
