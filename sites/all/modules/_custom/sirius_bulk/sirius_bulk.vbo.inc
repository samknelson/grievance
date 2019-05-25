<?php

function sirius_bulk_action_info() {
  return array(
    'sirius_bulk_queue' => array(
      'type' => 'node',
      'label' => t('Sirius Bulk Queue'),
      'configurable' => FALSE,
      'vbo_configurable' => FALSE,
      'triggers' => array(),
    ),
  );
}

function sirius_bulk_queue(&$node, $context) {
	// @todo: this is a little sketchy. I assume that the bulk nid is the first argument passed into the view (even if the view has no contextual filters defined.)
	$bulk_nid = $context['view_info']['arguments'][0];
	if (!$bulk_nid) { 
		drupal_set_message("Bulk message not found.");
		return; 
	}

	// Don't add to the queue twice.
	$query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_log');
  $query->fieldCondition('field_sirius_log_handler', 'target_id', $bulk_nid);
  // @todo: should be checking handler, but EntityFieldQuery can't check multiple values at the same time.
  $query->fieldCondition('field_sirius_notes', 'value', $node->nid); 
  $query->fieldCondition('field_sirius_category', 'value', 'bulk:queue');
  $stmt = $query->execute();
  if ($stmt['node']) { return; }

  // Add to the queue.
	sirius_log('bulk:queue', 'add', $node->title, '', $node->nid, array($bulk_nid, $node->nid));
}

