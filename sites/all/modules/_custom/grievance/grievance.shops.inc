<?php

function grievance_shop_bulk_reassign() {
  return drupal_get_form('grievance_shop_bulk_reassign_form');
}

function grievance_shop_bulk_reassign_form($form, &$form_state) {
	$form_state['node'] = $node;

	$form['intro'] = array(
		'#type' => 'markup',
		'#markup' => '<strong>' . 
			t('Please read the following carefully before proceeding') . 
			'</strong><br><br>' .
			t('The tool below will update the organizer (sometimes called "specialist") field in every <em>open</em> grievance.  It will give that field the same value as the "default organizer (specialist)" field in the shop that the grievance is associated with.') .
			'<br><br>' .
			t('This action cannot be undone, and may potentially update every grievance on the system.  Do not proceed unless you are sure that this is what you want to do.'),
	);
	$form['confirm'] = array(
		'#type' => 'checkbox',
		'#title' => t('Yes, I really mean it.'),
		'#description' => t('This tool will permanently and irrevocably update every open grievance on the system.  Do not proceed unless you are sure that this is what you want to do.'),
		'#required' => TRUE,
	);
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Run the bulk re-assignment tool now',
  );
	
	return $form;
}

function grievance_shop_bulk_reassign_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;

	// Get the list of "open" (unresolved) statuses
	$status_vocabulary = taxonomy_vocabulary_machine_name_load('grievance_status');
	$status_vid = $status_vocabulary->vid;
	$query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'taxonomy_term');
  $query->propertyCondition('vid', $status_vid);
  $query->fieldCondition('field_grievance_open', 'value', 1);
	$stmt = $query->execute();
	$open_status_tids = array();
	if ($stmt['taxonomy_term']) { $open_status_tids = array_keys($stmt['taxonomy_term']); }

	// Get the list of open grievances
	$query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance');
  $query->fieldCondition('field_grievance_status', 'tid', $open_status_tids, 'IN');
  $query->propertyOrderBy('nid');
  $stmt = $query->execute();
  $nids = array();
  if ($stmt['node']) {
  	$nids = array_keys($stmt['node']);
  }

	// Init
	$operations = array();
	foreach ($nids as $nid) {
		$operations[] = array(
			'grievance_shop_bulk_reassign_batch', 
			array($nid),
		);
	}

	// Launch the batch processor.
  $batch = array(
    'operations' => $operations,
		'finished' => 'grievance_shop_bulk_reassign_finished'
  );
  batch_set($batch);

	return;
}

function grievance_shop_bulk_reassign_batch($grievance_nid, &$context) {
	$context['results']['count']++;

	// Load the node
	$grievance_node = node_load($grievance_nid);
	if (!$grievance_node) { return; }
	$context['message'] = t('Checking grievance: @title', array('@title' => $grievance_node->title));

	// Load the shop
	$shop_nid = $grievance_node->field_grievance_shop['und'][0]['target_id'];
	if (!$shop_nid) { return; }
	$shop_node = node_load($shop_nid);
	if (!$shop_node) { return; }
	if (!$shop_node->field_grievance_rep_organizer['und'][0]) { return; }

	// check for changes
	$assignments = array();
	foreach ($shop_node->field_grievance_rep_organizer['und'] as $tmp) {
		$assignments[$tmp['target_id']]++;
	}
	foreach ($grievance_node->field_grievance_rep_organizer['und'] as $tmp) {
		$assignments[$tmp['target_id']]++;
	}
	$need_update = FALSE;
	foreach ($assignments as $count) {
		if ($count < 2) { $need_update = TRUE; }
	}
	if (!$need_update) { return; }

	// Ok, update the grievance
	$grievance_node->field_grievance_rep_organizer = $shop_node->field_grievance_rep_organizer;
	node_save($grievance_node);
	$context['results']['changed']++;
}

function grievance_shop_bulk_reassign_finished($success, $results, $operations) {
	$count = $results['count'];
	$changed = $results['changed'];
	if (!$count) { $count = 0; }
	if (!$changed) { $changed = 0; }
	drupal_set_message(t('Checked @count open grievances, updated @changed', array('@count' => $count, '@changed' => $changed)));

}

