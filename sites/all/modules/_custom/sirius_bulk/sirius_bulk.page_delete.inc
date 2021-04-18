<?php

function sirius_bulk_node_tab_delete($bulk_node) {
	return drupal_get_form('sirius_bulk_node_tab_delete_form', $bulk_node);
}

function sirius_bulk_node_tab_delete_form($form, &$form_state, $bulk_node) {
	$form_state['bulk_nid'] = $bulk_node->nid;

	$inventory = sirius_bulk_delete_inventory($bulk_node);
	$count = count($inventory);

	$form['intro'] = array(
		'#markup' => '<div class="sirius_form_intro">' . t('The form below will permanently completely delete the bulk node <strong>@title</strong>. All other records associated with this message, such as recipients, will also be deleted. This action cannot be undone. A total of <strong>@count</strong> dependent records will also be deleted.', array('@title' => $bulk_node->title, '@count' => $count)) . '</div>',
	);

	$form['mode'] = array(
		'#type' => 'select',
		'#title' => 'Mode',
		'#options' => array(
			NULL => t('-- Select --'),
			'test' => t('Test - nothing will be deleted'),
			'live' => t('Live - Domain will be deleted')
		),
		'#required' => TRUE,
	);

	$form['confirm'] = array(
		'#type' => 'checkbox',
		'#title' => t('Yes, permanently and completely delete this entity. This action cannot be undone.'),
		'#required' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_bulk_node_tab_delete_form_submit($form, &$form_state) {
	$bulk_node = node_load($form_state['bulk_nid']);
	$mode = $form_state['values']['mode'];
	$form_state['rebuild'] = TRUE;

	$inventory = sirius_bulk_delete_inventory($bulk_node);
	$chunks = array_chunk($inventory, 1000, TRUE);
	foreach ($chunks as $chunk) {
		$operations[] = array(
			'sirius_bulk_node_tab_delete_batch', 
			array($mode, $bulk_node->nid, array_values($chunk))
		);
	}

	// Put the bulk node itself at the very end.
	$operations[] = array(
		'sirius_bulk_node_tab_delete_batch', 
		array($mode, $bulk_node->nid, array($bulk_node->nid)),
	);

	// Launch the batch processor.
  $batch = array(
    'operations' => $operations,
		'finished' => 'sirius_bulk_node_tab_delete_batch_finished',
		'file' => drupal_get_path('module', 'sirius_bulk') . '/sirius_bulk.page_delete.inc',
  );

	batch_set($batch);
}

function sirius_bulk_node_tab_delete_batch($mode, $bulk_nid, $entity_ids, &$context) {
	$count = count($entity_ids);
	$first = $entity_ids[0];
	$last = $entity_ids[$count-1];

	$msg = "Deleting: Bulk node #$bulk_nid: ($count entities, ids $first to $last)";

	if ($mode == 'live') {
		node_delete_multiple($entity_ids);
	}

	if ($mode == 'live') { $msg .= " [LIVE MODE]"; } else { $msg .= " [TEST MODE, NO RECORDS DELETED]"; }
	$context['results']['msgs'][] = $msg;
	$context['results']['deleted']['count'] += $count;
	$context['message'] = $msg;
	$context['results']['mode'] = $mode;
}

function sirius_bulk_node_tab_delete_batch_finished($success, $results, $operations) {
	$mode = $results['mode'];
	if ($results['deleted']) {
		$count = $results['deleted']['count'];
		$msg = "Deleted: $count records.";
		if ($mode == 'live') { $msg .= " [LIVE MODE]"; } else { $msg .= " [TEST MODE, NO RECORDS DELETED]"; }
		drupal_set_message($msg);
	}
	if ($mode == 'live') { drupal_goto('sirius/bulk'); }
}

function sirius_bulk_delete_inventory($bulk_node) {
	$query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_log');
  $query->fieldCondition('field_sirius_log_handler', 'target_id', $bulk_node->nid);
  $query->fieldCondition('field_sirius_category', 'value', 'bulk:queue');
  $stmt = $query->execute();
  if (!$stmt['node']) { return array(); }

	return array_keys($stmt['node']);
}