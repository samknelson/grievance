<?php

function sirius_page_contact_resync_all() {
	return drupal_get_form('sirius_page_contact_resync_all_form');
}

function sirius_page_contact_resync_all_form($form, &$form_state) {
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Resync all workers and users'),
	);

	return $form;
}

function sirius_page_contact_resync_all_form_submit($form, &$form_state) {
	$sql = "select nid from node where type = 'sirius_worker' order by nid ";
	$stmt = db_query($sql, array());
	$all_workers = array();
	while ($hr = $stmt->fetchAssoc()) {
		$all_workers[] = $hr['nid'];
	}

	$sql = "select uid from users order by uid";
	$stmt = db_query($sql, array());
	$all_users = array();
	while ($hr = $stmt->fetchAssoc()) {
		$all_users[] = $hr['uid'];
	}

	$operations = array();
	$chunks = array_chunk($all_workers, 100, TRUE);
	foreach ($chunks as $chunk) {
		$operations[] = array(
	    'sirius_page_contact_resync_all_batch_workers',
	    array($chunk),
		);
	}

	$chunks = array_chunk($all_users, 100, TRUE);
	foreach ($chunks as $chunk) {
		$operations[] = array(
	    'sirius_page_contact_resync_all_batch_users',
	    array($chunk),
		);
	}

  $batch = array(
    'operations' => $operations,
    'finished' => 'sirius_page_contact_resync_all_batch_finished',
  );

  batch_set($batch);
}

function sirius_page_contact_resync_all_batch_workers($batch, &$context) {
	$count = 0;
	foreach ($batch as $worker_nid) {
		$worker_node = node_load($worker_nid);
		if (!$worker_node->field_sirius_contact['und'][0]['target_id']) {
			node_save($worker_node);
			$context['results']['workers']++;
			++$count;
		}
	}
	$context['message'] = "Updated $count workers (total " . number_format($context['results']['workers'], 0) . ")";
}

function sirius_page_contact_resync_all_batch_users($batch, &$context) {
	foreach ($batch as $uid) {
		if (!$uid) { continue; }
		$user = user_load($uid);
		if (!$user) { continue; }
		if ($id && $user->uid) {}
		if (!$user->field_sirius_contact['und'][0]['target_id']) {
			user_save($user);
		}
		$context['results']['users']++;
	}
	$context['message'] = 'Processing users.';
}

function sirius_page_contact_resync_all_batch_finished($success, $results, $operations) {
	drupal_set_message("Resync'd $results[workers] workers and $results[users] users.");
}