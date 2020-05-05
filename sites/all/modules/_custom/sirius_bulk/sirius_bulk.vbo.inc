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
    $bulk_nid = $_SESSION['my_bulk_nid'];
  }
  if (!$bulk_nid) { 
    drupal_set_message("Don't know which bulk message!", 'error');
    return;
  }

  $contact_nid = $node->field_sirius_contact['und'][0]['target_id'];
  if (!$contact_nid) { $contact_nid = $node->nid; }
  sirius_bulk_recipient_add($bulk_nid, $contact_nid);
}

