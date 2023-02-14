<?php

function sirius_node_tab_set_module($module_name, &$node_tabs) {
  foreach ($node_tabs as $content_type => $ignore) {
    foreach ($node_tabs[$content_type] as $path => $ignore) {
      if (!$node_tabs[$content_type][$path]['module']) {
        $node_tabs[$content_type][$path]['module'] = $module_name;
      }
    }
  }
}

function sirius_node_tab_handle_requires($tab_record) {
  if ($tab_record['file']) { 
    if (is_array($tab_record['file'])) { 
      foreach ($tab_record['file'] as $file) {
        sirius_require($tab_record['module'], $file);
      }
    } else {
      sirius_require($tab_record['module'], $tab_record['file']);
    }
  }
}

function sirius_node_tab_all() {
  $all = &drupal_static(__FUNCTION__, array());
  if (!$all) { $all = module_invoke_all('node_tabs'); }
  return $all;
}

function sirius_node_tab_content($node, $in_tab_name) {
  $node_tabs = sirius_node_tab_all();
  $tab_record = $node_tabs[$node->type][$in_tab_name];
  if (!$tab_record) { 
    drupal_set_message(t('Bad tab: ') . $node->type . ', ' . $in_tab_name, 'error'); 
    return '';
  }

  sirius_node_tab_handle_requires($tab_record);

  $callback = $tab_record['callback'];
  $args = array($node);

  if (!function_exists($callback)) {
    $callback_form = "$callback" . '_form';
    if (!function_exists($callback_form)) {
      drupal_set_message("Callback doesn't exist: $callback", 'error');
      return '';
    }
    $callback = 'drupal_get_form';
    $args = array($callback_form, $node);
  }

  drupal_set_title($node->title);
  if ($tab_record['callback arguments']) {
    $args = array_merge($args, $tab_record['callback arguments']);
  }

  /*
  // Make landing page redirect to the main view page. Not sure why we bother, though. 
  if ($tab_record['landing']) {
    $pattern = preg_replace('/[^a-zA-Z0-9_\/]/', '', $in_tab_name);
    if (preg_match("/$pattern/", current_path())) {
      drupal_goto('node/' . $node->nid);
    }
  }
  */
  
  // Allow other modules to modify our output
  $result = call_user_func_array($callback, $args);
  $context = array('node' => $node, 'in_tab_name' => $in_tab_name, 'tab_record' => $tab_record);
  drupal_alter('sirius_node_tab_content', $result, $context);

  return $result;
}

function sirius_node_tab_access($node, $in_tab_name, $options = array()) {
  // Deny access to all nodes that are not in your domain
  if (!user_access('sirius admin') && $node->type != 'sirius_domain' && !$options['skip_domain_check']) {
    $node_domain_nid = $node->field_sirius_domain['und'][0]['target_id'];
    $my_domain_nid = sirius_domain_nid();
    if ($node_domain_nid && ($node_domain_nid != $my_domain_nid)) { return FALSE; }
  }

  // Check generic permissions
  $node_tabs = sirius_node_tab_all();
  $tab_record = $node_tabs[$node->type][$in_tab_name];
  if (!$tab_record) { return FALSE; }

  // Check basic permission
  foreach ($tab_record['permissions'] as $permission) {
    if (!user_access($permission)) { return FALSE; }
  }

  // Check if the component is enabled
  if ($tab_record['component'] && !sirius_domain_component_enabled($tab_record['component'])) {
    return FALSE;
  }

  // In general, you must have access to the underlying node. 
  // (Some tabs are accessible even if the user can't see the underlying node)
  if (!node_access('view', $node) && !$tab_record['skip_node_view_check']) {
    return FALSE;
  }

  // Check for a custom access permission
  sirius_node_tab_handle_requires($tab_record);
  $access_function_name = $tab_record['callback'] . '_access';
  if (function_exists($access_function_name)) {
    $args = array($node);
    if ($tab_record['callback arguments']) {
      $args = array_merge($args, $tab_record['callback arguments']);
    }
    if (!call_user_func_array($access_function_name, $args)) { return FALSE; }
  }
  
  return TRUE;
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

  if (!function_exists($callback)) {
    $callback_form = "$callback" . '_form';
    if (!function_exists($callback_form)) {
      drupal_set_message("Callback doesn't exist: $callback", 'error');
      return '';
    }
    $callback = 'drupal_get_form';
    $args = array($callback_form, $term);
  }

  drupal_set_title($term->name);
  if ($tab_record['callback arguments']) {
    $args = array_merge($args, $tab_record['callback arguments']);
  }
  sirius_node_tab_handle_requires($tab_record);
  $result = call_user_func_array($callback, $args);

  return $result;
}

function sirius_term_tab_access($term, $in_tab_name) {
  // Check generic permissions
  $term_tabs = sirius_term_tab_all();
  $tab_record = $term_tabs[$term->vocabulary_machine_name][$in_tab_name];

  // Deny access to all nodes that are not in your domain
  if (!user_access('sirius admin')) {
    $term_domain_nid = $term->field_sirius_domain['und'][0]['target_id'];
    $my_domain_nid = sirius_domain_nid();
    if ($term_domain_nid != $my_domain_nid) { return FALSE; }
  }

  if (!$tab_record) { return FALSE; }

  // Check basic permission
  foreach ($tab_record['permissions'] as $permission) {
    if (!user_access($permission)) { return false; }
  }

  // Check for a custom access permission
  $access_function_name = $tab_record['callback'] . '_access';
  sirius_node_tab_handle_requires($tab_record);
  if (function_exists($access_function_name)) {
    if (!$access_function_name($term)) { return false; }
  }
  
  return true;
}

/**
 * Get extra arguments to a node tab. For instance, if tab name is
 *     sirius_worker_tc/classes
 * and the path is
 *     node/12345/sirius_worker_tc/classes/6890
 * then sirius_node_tab_arg(0) returns "6890"
 */

function sirius_node_tab_arg($idx) {
  $item = &drupal_static(__FUNCTION__, NULL);
  if (!$item) { $item = menu_get_item(); } 
  if (!$item) { return NULL; }
  $page_arguments = $item['page_arguments'];
  if (!$page_arguments) { return NULL; }

  // argument 0 = %node
  // argument 1 = tab_name
  return $page_arguments[$idx+2];
}
