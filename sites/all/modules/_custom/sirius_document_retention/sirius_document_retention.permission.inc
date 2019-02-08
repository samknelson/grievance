<?php

/***
 * Implements hook_permission
 */

function sirius_document_retention_permission() {
  return array(
    'sirius document retention admin' => array(
      'title' => t('sirius document retention admin'), 
      'description' => t('Administer document retention policies'),
    ),
  );
}

/**
 * Implements hook_node_access
 */

function sirius_document_retention_node_access($node, $op, $account) {
	if ($node->type != 'sirius_document_retention_log') { return NODE_ACCESS_IGNORE; }
	if (user_access('sirius document retention admin')) { return NODE_ACCESS_ALLOW; }
	return NODE_ACCESS_DENY;
}
