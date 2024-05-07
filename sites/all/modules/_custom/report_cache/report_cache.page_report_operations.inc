<?php

function report_cache_operations($report_type, $report_id, $info) {
	return drupal_get_form('report_cache_operations_form', $report_type, $report_id, $info);
}

function report_cache_operations_form($form, &$form_state, $report_type, $report_id, $info) {
	$form_state['report']['report_type'] = $report_type;
	$form_state['report']['report_id'] = $report_id;
	$form_state['report']['info'] = $info;

	$form['#tree'] = TRUE;

	$domain_nid = sirius_domain_nid(); if (!$domain_nid) { $domain_nid = 0; }
	$fields = $info['fields'];

	$parameters = report_cache_parameters($report_type, $report_id);

	$parameters_render = $info['generator']->render_parameters($parameters);

	if ($parameters_render) {
		$parameters_html = '<div class="report_cache_parameters">';
		$parameters_html .= '<h2>' . t('Parameters') . '</h2>';
		$parameters_html .= $parameters_render;
		$parameters_html .= sirius_fakerender_field('Instance:', $report_id);
	  $parameters_html .= '</div>';
	  $form['parameters_html'] = array('#markup' => $parameters_html);
	}

	// We only support contact operations at the moment
	$field_contact_nid = $info['operations']['contact']['field_contact_nid'];
	if (!$field_contact_nid) { return sirius_form_error('No contact NID field specified. Only contact operations are supported.'); }

	$rowcount = report_cache_rowcount($report_type, $report_id);
	if (!$rowcount) { return sirius_form_error('No records found in this report.'); }

	// Operation
	$operation_options = array();
	$operation_options[NULL] = t('-- Select --');
	$operation_options['tag'] = t('Tag all contacts');
	if (sirius_domain_component_enabled('bulk')) {
		$operation_options['bulk'] = t('Add all contacts to a bulk message');
	}
	$form['operation'] = array(
		'#title' => t('Perform the following action on <strong>all</strong> the contacts found:'),
		'#type' => 'select',
		'#options' => $operation_options,
		'#required' => TRUE,
	);

	// operation = tag
	$form['tag'] = array(
		'#type' => 'fieldset',
		'#title' => t('Tag'),
		'#states' => array('visible' => array(":input[name=\"operation\"]" => array('value' => 'tag')))
	);
	$tag_options = array(NULL => t('-- Select --')) + array('NEW' => t('NEW TAG')) + sirius_taxonomy_load('sirius_contact_tags', 'tid', 'name', FALSE, TRUE);
	$form['tag']['tid'] = array(
		'#type' => 'select',
		'#options' => $tag_options,
		'#id' => 'tag_tid',
	);
	$form['tag']['name'] = array(
		'#title' => t('New Tag'),
		'#type' => 'textfield',
		'#states' => array('visible' => array(":input[id=\"tag_tid\"]" => array('value' => 'NEW')))
	);

	// operation = bulk
	$form['bulk'] = array(
		'#type' => 'fieldset',
		'#states' => array('visible' => array(":input[name=\"operation\"]" => array('value' => 'bulk')))
	);

	$bulk_options = array(NULL => t('-- Select --'));
	$bulk_options += sirius_bulk_list(array('statuses' => array('draft', 'pending')));
	$form['bulk']['nid'] = array(
		'#title' => 'Bulk Message',
		'#type' => 'select',
		'#options' => $bulk_options,
		'#id' => 'tag_existing',
	);

	$form['confirm'] = array(
		'#type' => 'checkbox',
		'#title' => t('Yes, please perform the selected operation on up to <strong>@rowcount</strong> records. This action cannot be undone.', array('@rowcount' => $rowcount)),
		'#required' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	$form['meta'] = array('#markup' => report_cache_render_meta($parameters));

	return $form;
}

function report_cache_operations_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
	$domain_nid = sirius_domain_nid(); if (!$domain_nid) { $domain_nid = 0; }

	$report_type = $form_state['report']['report_type'];
	$report_id = $form_state['report']['report_id'];
	$info = $form_state['report']['info'];
	$field_contact_nid = $info['operations']['contact']['field_contact_nid'];

	$batch_params = array();
	$operation = $form_state['values']['operation'];
	$batch_params['operation'] = $operation;
	$batch_params['report_type'] = $report_type;
	$batch_params['report_id'] = $report_id;
	$batch_params['report_info'] = $info;

	if ($operation == 'tag') {
		$tid = $form_state['values']['tag']['tid'];
		if (!$tid) {
			drupal_set_message("Please select a tag.", 'error'); return;
		}

		if ($tid == 'NEW') {
			$name = $form_state['values']['tag']['name'];
			if (!$name) { drupal_set_message("Please enter a value for the new tag.", 'error'); return; }

			$check = array_flip(sirius_taxonomy_load('sirius_contact_tags', 'tid', 'name'));
			if ($check[$name]) { drupal_set_message("Tag [$name] already exists.", 'error'); return; }

			$vocab = taxonomy_vocabulary_machine_name_load('sirius_contact_tags');
			$vid = $vocab->vid;
			$term = new stdClass();
		  $term->name = $name;
		  $term->vid = $vid;
		  $term->field_sirius_domain['und'][0]['target_id'] = sirius_domain_nid();
		  taxonomy_term_save($term);
		  $tid = $term->tid;
		  drupal_set_message("Created a new tag.");
		}

		$batch_params['tid'] = $tid;
	}

	if ($operation == 'bulk') {
		$nid = $form_state['values']['bulk']['nid'];
		if (!$nid) { drupal_set_message("Please select a bulk message.", 'error'); return; }
		$batch_params['nid'] = $nid;
	}

	// Fetch all the contact NIDs
	$query = db_select('report_cache');
	$query->fields('report_cache');
	$query->condition('report_id', $report_id);
	$query->condition('report_type', $report_type);
	$query->condition('report_domain_nid', $domain_nid);
	$stmt = $query->execute();

	$contact_nids = array();	
	while ($hr = $stmt->fetchAssoc()) {
		$hr = unserialize($hr['report_data']);
		$contact_nid = $hr[$field_contact_nid];
		if ($contact_nid) { $contact_nids[$contact_nid]++; }
	}
	$contact_nids = array_keys($contact_nids);
	$contact_chunks = array_chunk($contact_nids, 2);

	// Generate the batche operations
	$operations = array();
	foreach ($contact_chunks as $chunk) {
		$operations[] = array(
			'report_cache_operations_batch',
			array($chunk, $batch_params),
		);
	}
  $batch = array(
    'operations' => $operations,
		'finished' => 'report_cache_operations_batch_finished',
  );
  batch_set($batch);
}

function report_cache_operations_batch($contact_nids, $batch_params, &$context) {
	$context['results']['batch_params'] = $batch_params;
	$context['results']['whatever'] = 'Hello, Sam';

	foreach ($contact_nids as $contact_nid) {
		++$context['results']['count'];

		if ($batch_params['operation'] == 'bulk') {
			$nid = $batch_params['nid'];
			$result = sirius_bulk_recipient_add($nid, $contact_nid);
		}
		else if ($batch_params['operation'] == 'tag') {
			$tid = $batch_params['tid'];
			$contact_node = node_load($contact_nid);
			$result = sirius_contact_tag_add($contact_node, $tid);
		} else {
			// Unknown operation
			$result = array('success' => FALSE);
		}

		if ($result['success']) {
			++$context['results']['success'];
		} else {
			++$context['results']['error'];
			if (!$context['results']['error_msg']) { $context['results']['error_msg'] = array(); }

			$msg = $result['msg'];
			$count = count($context['results']['error_msg']);

			if ($context['results']['error_msg'][$msg]) {
				$context['results']['error_msg'][$msg]++;
			} else if (count($context['results']['error_msg']) < 100) {
				$context['results']['error_msg'][$msg] = 1;
			} 
		}
	}
}

function report_cache_operations_batch_finished($success, $results, $operations) {
	$count = $results['count'];
	if (!$count) { $count = 0; }

	$success = $results['success'];
	if (!$success) { $success = 0; }

	$error = $results['error'];
	if (!$error) { $error = 0; }

	drupal_set_message("Processed $count contacts, with $success success and $error errors.");

	if ($results['error_msg']) {
		foreach ($results['error_msg'] as $msg => $count) {
			drupal_set_message($msg . " ($count records)", 'warning');
		}
	}

	$report_type = $results['batch_params']['report_type'];
	$report_id = $results['batch_params']['report_id'];
	drupal_goto('sirius/reports/' . $report_type, array('query' => array('report_id' => $report_id, 'action' => 'operations')));
}