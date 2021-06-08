<?php

function sirius_training_page_level_denorm_resync() {
	return drupal_get_form('sirius_training_page_level_denorm_resync_form');
}

function sirius_training_page_level_denorm_resync_form($form, &$form_state) {
	form_load_include($form_state, 'inc', 'sirius_training', 'sirius_training.page_level_denorm_resync');

	$form['confirm'] = array(
		'#type' => 'checkbox',
		'#title' => t('Yes, run the re-denormalize procedure.'),
		'#required' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

  $log = sirius_minilog_render_html();
  if ($log) { $form['results'] = array('#markup' => '<hr>' . $log); }

	return $form;
}

function sirius_training_page_level_denorm_resync_form_submit($form, &$form_state) {
	$sql_args = array();
	$sql = "select nid ";
	$sql .= "from node ";
	$sql .= "where type = 'sirius_worker' ";
	$sql .= "order by nid desc ";
	$stmt = sirius_sql_query($sql, $sql_args);
	$worker_nids = array();
	while ($hr = $stmt->fetchAssoc()) {
		$worker_nids[] = $hr['nid'];
	}
	$chunks = array_chunk($worker_nids, 100, TRUE);

	foreach ($chunks as $chunk) {
		$operations[] = array('sirius_training_page_level_denorm_resync_batch', array($chunk));
	}
	$batch = array(
		'operations' => $operations,
		'finished' => 'sirius_training_page_level_denorm_resync_finished',
		'file' => drupal_get_path('module', 'sirius_training') . '/' . 'sirius_training.page_level_denorm_resync.inc',
	);
	batch_set($batch);
	return;
}

function sirius_training_page_level_denorm_resync_batch($chunk, &$context) {
  if (!$context['results']['scan']) { $context['results']['scan'] = 0; }
  if (!$context['results']['skip']) { $context['results']['skip'] = 0; }
  if (!$context['results']['compute']) { $context['results']['compute'] = 0; }


  $worker_nodes = node_load_multiple($chunk);

	foreach ($worker_nodes as $worker_nid => $worker_node) {
		++$context['results']['scan'];

		$levels = sirius_training_levels_fetch_worker($worker_node);
		if (!$levels) {
			++$context['results']['skip'];
			continue; 
		}

		$denorm = array();
		foreach ($levels as $ms_tid => $record) {
			$level_code = $record['code'];
			$denorm[] = array('value' => "$ms_tid|$level_code");
		}
		$worker_node->field_sirius_denorm_level['und'] = $denorm;
		$worker_node->no_recurse = TRUE;
		try { node_save($worker_node); } catch (exception $ignore) { }
		++$context['results']['compute'];
	}
}

function sirius_training_page_level_denorm_resync_finished($success, $results, $operations) {
  drupal_set_message(t("Process complete: @scan records scanned, @skip skipped and @compute computed.", array('@scan' => $results['scan'], '@skip' => $results['skip'], '@compute' => $results['compute'])));
}
