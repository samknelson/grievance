<?php

/***
 * Implements hook_permission
 */

function sirius_permission() {
  return array(
    'sirius' => array(
      'title' => t('Sirius'), 
      'description' => t('Permission to use any aspect of the Sirius system.  ALL users must have this permission.'),
    ),
    'sirius ssn' => array(
      'title' => t('Sirius SSN'), 
      'description' => t('Permission to view full SSNs'),
    ),
    'sirius news admin' => array(
      'title' => t('Sirius News Admin'), 
      'description' => t('Permission to add, edit, or delete news items'),
    ),
    'sirius log view' => array(
      'title' => t('Sirius Log View'), 
      'description' => t('View any log entry'),
    ),
    'sirius log edit' => array(
      'title' => t('Sirius Log Edit'), 
      'description' => t('Create or edit any log entry'),
    ),
    'sirius admin' => array(
      'title' => t('Sirius Admin'), 
      'description' => t('Administrative permission for the Sirius system'),
    ),
    'sirius edit own user' => array(
      'title' => t('Sirius Edit Own User'), 
      'description' => t('Permission to edit your own user account (i.e. set your password)'),
    ),
  );
}

/**
 * Implements hook_node_access
 */

function sirius_node_access($node, $op, $account) {
	if ($node->type == 'sirius_log') { return sirius_node_access_log($node, $op, $account); }
  if ($node->type == 'sirius_news') { return sirius_node_access_news($node, $op, $account); }
  return NODE_ACCESS_IGNORE;
}