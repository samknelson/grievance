<?php

function sirius_bulk_action_info() {
  return array(
    'sirius_bulk_queue' => array(
      'type' => 'node',
      'label' => t('Sirius Bulk Queue'),
      'configurable' => FALSE,
      'vbo_configurable' => FALSE,
      'triggers' => array(),
      'pass rows' => TRUE,
    ),
  );
}

function sirius_bulk_queue(&$node, $context) {
	// We need to set the bulk message in each row, in a views hook. It's a little ornate, but I couldn't figure out any other way to get the data into the VBO.
  if ($context['rows']) {
    $bulk_nid = array_values($context['rows'])[0]->sirius_bulk_nid;
  }
  if (!$bulk_nid) { 
    drupal_set_message("Don't know which bulk message!", 'error');
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

