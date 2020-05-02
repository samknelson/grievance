<?php

function sirius_bulk_oneclick_all() {
  $all = &drupal_static(__FUNCTION__, array());
  if (!$all) {
    $all = module_invoke_all('sirius_bulk_oneclick');
  }
  return $all;
}

/**
 * Implements hook_sirius_bulk_oneclick
 */

function sirius_bulk_sirius_bulk_oneclick() {
	$items = array();
	$items['worker_member_status_notify'] = array(
		'title' => t('Worker Notification by Member Status'),
		'form' => 'sirius_bulk_oneclick_member_status_notify_form',
	);
	return $items;
}

function sirius_bulk_oneclick_member_status_notify_form($form, &$form_state) {
	$member_status_options = sirius_taxonomy_load('sirius_member_status', 'tid', 'name', TRUE);
	$form['member_status'] = array(
		'#title' => t('Member Status'),
		'#type' => 'select',
		'#options' => $member_status_options,
		'#required' => TRUE,
	);

	$medium_options = list_allowed_values(field_info_field('field_sirius_bulk_medium'));
	$form['medium'] = array(
		'#title' => t('Medium'),
		'#type' => 'select',
		'#options' => $medium_options,
		'#default_value' => 'choice',
		'#required' => TRUE,
	);

	$status_options = list_allowed_values(field_info_field('field_sirius_bulk_status'));
	$form['status'] = array(
		'#title' => t('Status'),
		'#type' => 'select',
		'#options' => $status_options,
		'#default_value' => 'pending',
		'#required' => TRUE,
	);

	$form['subject'] = array(
		'#title' => t('Subject'),
		'#type' => 'textfield',
		'#required' => TRUE,
	);

	$form['email_body'] = array(
		'#title' => t('Email Body'),
		'#type' => 'text_format',
		'#states' => array(
			'visible' => array(
	      array(':input[name="medium"]' => array('value' => 'email')),
	      'or',
	      array(':input[name="medium"]' => array('value' => 'choice')),
	    ),
		),
	);

	$form['sms_body'] = array(
		'#title' => t('SMS Body'),
		'#type' => 'textarea',
		'#states' => array(
			'visible' => array(
	      array(':input[name="medium"]' => array('value' => 'sms')),
	      'or',
	      array(':input[name="medium"]' => array('value' => 'choice')),
	    ),
		),
	);

	$form['voice_body'] = array(
		'#title' => t('Voice Body'),
		'#type' => 'textarea',
		'#states' => array(
			'visible' => array(
	      array(':input[name="medium"]' => array('value' => 'voice')),
	    ),
		),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	// These shouldn't be hardcoded, but what the fuck.
	$form['status']['#default_value'] = 'pending';
	$form['member_status']['#default_value'] = 617;
	$form['subject']['#default_value'] = 'The cross-industry 360 list is now open.';
	global $base_url;
	$url = $base_url;
	$form['email_body']['#default_value'] = "Please sign in to the dispatch system at <a href=\"$url\"> and indicate your EBA dates.";
	$form['sms_body']['#default_value'] = "Please sign in to the dispatch system at\n\n$url\n\nand indicate your EBA dates.";

	return $form;
}

function sirius_bulk_oneclick_member_status_notify_form_submit($form, &$form_state) {
	// Member status
	$member_status_tid = $form_state['values']['member_status'];

	// Work statuses
	$work_status_terms = sirius_taxonomy_load('sirius_work_status', 'tid', 'full');
	$active_work_status_tids = array();
	foreach ($work_status_terms as $work_status_tid => $work_status_term) {
		if ($work_status_term->field_sirius_member_active['und'][0]['value'] == 'Yes') {
			$active_work_status_tids[] = $work_status_tid;
		}
	}
	if (!$active_work_status_tids) {
		drupal_set_message(t('No work statuses are active.'), 'error');
		$form_state['rebuild'] = TRUE;
		return;
	}

	// Workers
	$query = new EntityFieldQuery();
	$query->entityCondition('entity_type', 'node');
	$query->entityCondition('bundle', 'sirius_worker');
	$query->fieldCondition('field_sirius_member_status', 'tid', $member_status_tid);
	$query->fieldCondition('field_sirius_work_status', 'tid', $active_work_status_tids, 'in');
	$query->propertyOrderBy('nid');
	$stmt = $query->execute();
	if (!$stmt['node']) {
		drupal_set_message(t('No workers match your criteria (active work status and the member status selected).'), 'warning');
		$form_state['rebuild'] = TRUE;
		return;
	}
	$worker_nids = array_keys($stmt['node']);
	$worker_nid_chunks = array_chunk($worker_nids, 100);

	// Create the node
  global $user;
  $bulk_node = new stdClass();
  $bulk_node->type = 'sirius_bulk';
  $bulk_node->no_recurse = TRUE;
  node_object_prepare($bulk_node);
  $bulk_node->language = LANGUAGE_NONE;
  $bulk_node->uid = $user->uid;
  $bulk_node->status = 1;
  $bulk_node->promote = 0;
  $bulk_node->comment = 0;
  $bulk_node->title = t('One-Click Message: ') . $form_state['values']['subject'];
  $bulk_node->field_sirius_bulk_medium['und'][0]['value'] = $form_state['values']['medium'];
  $bulk_node->field_sirius_bulk_status['und'][0]['value'] = 'draft';
  $bulk_node->body['und'][0]['value'] = $form_state['values']['email_body']['value'];
  $bulk_node->body['und'][0]['format'] = $form_state['values']['email_body']['format'];
  $bulk_node->field_sirius_sms['und'][0]['value'] = $form_state['values']['sms_body'];
  $bulk_node->field_sirius_voice['und'][0]['value'] = $form_state['values']['voice_body'];
  $bulk_node->field_sirius_datetime['und'][0]['value'] = gmdate('Y-m-d H:i:s');
  $bulk_node = node_submit($bulk_node);
  node_object_prepare($bulk_node);
  node_save($bulk_node);

	$operations = array();
	$count = 0;
	foreach ($worker_nid_chunks as $chunk) {
		++$count;
		$operations[] = array(
			'sirius_bulk_oneclick_member_status_notify_batch',
			array($chunk, $bulk_node->nid, $form_state['values']),
		);
	}
  $batch = array(
    'operations' => $operations,
		'finished' => 'sirius_bulk_oneclick_member_status_notify_batch_finished',
  );
  batch_set($batch);
}

function sirius_bulk_oneclick_member_status_notify_batch($worker_nids, $bulk_nid, $values, &$context) {
	$context['results']['bulk_nid'] = $bulk_nid;
	$context['results']['values'] = $values;
	foreach ($worker_nids as $worker_nid) {
		$sql = "select title from node where nid = :worker_nid";
		$stmt = db_query($sql, array(':worker_nid' => $worker_nid));
		$hr = $stmt->fetchAssoc();
		sirius_log('bulk:queue', 'add', $hr['title'], '', $worker_nid, array($bulk_nid, $worker_nid));
		++$context['results']['count'];
	}
}

function sirius_bulk_oneclick_member_status_notify_batch_finished($success, $results, $operations) {
	drupal_set_message(t('Queued @count workers.', array('@count' => number_format($results['count']))));

	$bulk_nid = $results['bulk_nid'];
	$values= $results['values'];
	$bulk_node = node_load($bulk_nid);
	if ($bulk_node->field_sirius_bulk_status['und'][0]['value'] != $values['status']) {
		$bulk_node->field_sirius_bulk_status['und'][0]['value'] = $values['status'];
		node_save($bulk_node);
		drupal_set_message(t('Set status to @status.', array('@status' => $values['status'])));
	}
	drupal_goto("node/$bulk_nid/sirius_bulk_queue");
}