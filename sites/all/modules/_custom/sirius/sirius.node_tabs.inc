<?php

function sirius_node_tab_all() {
  $all = &drupal_static(__FUNCTION__, array());
  if (!$all) {
    $all = module_invoke_all('node_tabs');
  }
  return $all;
}

function sirius_node_tab_content($node, $in_tab_name) {
  $node_tabs = sirius_node_tab_all();
  $tab_record = $node_tabs[$node->type][$in_tab_name];
  if (!$tab_record) { 
    drupal_set_message(t('Bad tab: ') . $node->type . ', ' . $in_tab_name, 'error'); 
    return FALSE;
  }

  $callback = $tab_record['callback'];
  $args = array($node);
  drupal_set_title($node->title);
  if ($tab_record['callback arguments']) {
    $args = array_merge($args, $tab_record['callback arguments']);
  }
  return call_user_func_array($callback, $args);
  
  return "";
}

function sirius_node_tab_access($node, $in_tab_name) {
  // Check generic permissions
  $node_tabs = sirius_node_tab_all();
  $tab_record = $node_tabs[$node->type][$in_tab_name];
  if (!$tab_record) { return FALSE; }

  // Check basic permission
  foreach ($tab_record['permissions'] as $permission) {
    if (!user_access($permission)) { return false; }
  }

  // Check for a custom access permission
  $access_function_name = $tab_record['callback'] . '_access';
  if (function_exists($access_function_name)) {
    if (!$access_function_name($node)) { return false; }
  }
  
  return true;
}



function sirius_term_tab_all() {
  $all = &drupal_static(__FUNCTION__, array());
  if (!$all) {
    $all = module_invoke_all('term_tabs');
  }
  return $all;
}

function sirius_term_tab_content($term, $in_tab_name) {
  $term_tabs = sirius_term_tab_all();
  $tab_record = $term_tabs[$term->vocabulary_machine_name][$in_tab_name];
  if (!$tab_record) { 
    drupal_set_message(t('Bad tab: ') . $term->vocabulary_machine_name . ', ' . $in_tab_name, 'error'); 
    return FALSE;
  }

  $callback = $tab_record['callback'];
  $args = array($term);
  drupal_set_title($term->title);
  if ($tab_record['callback arguments']) {
    $args = array_merge($args, $tab_record['callback arguments']);
  }
  return call_user_func_array($callback, $args);
  
  return "";
}

function sirius_term_tab_access($term, $in_tab_name) {
  // Check generic permissions
  $term_tabs = sirius_term_tab_all();
  $tab_record = $term_tabs[$term->vocabulary_machine_name][$in_tab_name];

  if (!$tab_record) { return FALSE; }

  // Check basic permission
  foreach ($tab_record['permissions'] as $permission) {
    if (!user_access($permission)) { return false; }
  }

  // Check for a custom access permission
  $access_function_name = $tab_record['callback'] . '_access';
  if (function_exists($access_function_name)) {
    if (!$access_function_name($term)) { return false; }
  }
  
  return true;
}