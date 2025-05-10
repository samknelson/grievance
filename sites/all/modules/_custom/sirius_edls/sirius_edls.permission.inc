<?php

/***
 * Implements hook_permission
 */

function sirius_edls_permission() {
  return array(
    'sirius edls admin' => array(
      'title' => t('Sirius EDLS Admin'), 
      'description' => t('Administer all aspects of the EDLS system'),
    ),
    'sirius edls employer labor manager' => array(
      'title' => t('Sirius EDLS Employer Labor Manager'), 
      'description' => t(''),
    ),
    'sirius edls employer labor coordinator' => array(
      'title' => t('Sirius EDLS Employer Labor Coordinator'), 
      'description' => t(''),
    ),
    'sirius edls employer labor supervisor' => array(
      'title' => t('Sirius EDLS Employer Labor Supervisor'), 
      'description' => t(''),
    ),
    'sirius edls employer reader' => array(
      'title' => t('Sirius EDLS Employer Reader'), 
      'description' => t(''),
    ),
    'sirius edls worker labor advisor' => array(
      'title' => t('Sirius EDLS Worker Labor Advisor'), 
      'description' => t(''),
    ),
  );
}

/**
 * Implements hook_node_access
 */

function sirius_edls_node_access($node, $op, $account) {
  // Get the node type
  if (is_object($node)) { $type = $node->type; } else { $type = $node; }

  if ($type == 'sirius_edls_sheet') {
    // Must be trust admin to do see the raw node
    if (!user_access('sirius admin')) { return NODE_ACCESS_DENY; }
  }

  // We know nothing.
  return NODE_ACCESS_IGNORE;
}
