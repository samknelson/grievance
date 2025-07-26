<?php

function sirius_smf_node_tab_worker_oneoff_dentwell_election_form($form, &$form_state, $worker_node) {
	$form_state['worker_nid'] = $worker_node->nid;

	$form['#tree'] = TRUE;
	$form['mode'] = array(
		'#type' => 'select',
		'#title' => t('Mode'),
		'#options' => array(
			'test' => t('Test'),
			'live' => t('Live'),
		),
		'#require' => TRUE,
	);
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

  $log = sirius_minilog_render_html();
  if ($log) { $form['results'] = array('#markup' => '<hr>' . $log); }

	return $form;
}

function sirius_smf_node_tab_worker_oneoff_dentwell_election_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
	$worker_node = node_load($form_state['worker_nid']);
	return sirius_smf_oneoff_dentwell_election($form_state['values']['mode'], $worker_node);
}










// drush --user=1 php-eval "sirius_require('sirius_smf', 'sirius_smf.oneoff_dentwell_election.inc'); sirius_smf_worker_oneoff_dentwell_election_all('test');"
function sirius_smf_worker_oneoff_dentwell_election_all($mode) {
	sirius_minilog_echo_active(TRUE);
	sirius_denorm_realtime(FALSE);

	$batch_size = 1000;

	$last_worker_nid = variable_get('sirius_smf_worker_oneoff_dentwell_election_all_last_worker_nid');
	if (!$last_worker_nid) { $last_worker_nid = 0; }

	$sql = "select count(*) as c from node where type = 'sirius_worker' and nid > $last_worker_nid";
	$stmt = sirius_sql_query($sql, array());
	$hr = $stmt->fetchAssoc();
	$total_grand = $hr['c'];
	sirius_minilog("There are " . number_format($grand_total) . " records remaining.");

	$sql = "select nid from node where type = 'sirius_worker' and nid > $last_worker_nid order by nid limit $batch_size";
	$stmt = sirius_sql_query($sql, array());

	$worker_nids = array();
	while ($hr = $stmt->fetchAssoc()) { $worker_nids[] = $hr['nid']; }
	$total = count($worker_nids);

  $ts_start = time();
	foreach ($worker_nids as $worker_nid) {
		++$count;

		$ts_elapsed = time() - $ts_start;
		$ts_per_record = $ts_elapsed / $count;
		$estimate = format_interval($ts_per_record * $total);

		$worker_node = node_load($worker_nid);

		sirius_minilog("Processing $count / $total / $total_grand: $worker_nid: " . $worker_node->title . ": $estimate");
		sirius_minilog_indent();
		sirius_domain($worker_node->field_sirius_domain['und'][0]['target_id']);
		sirius_smf_oneoff_dentwell_election($mode, $worker_node);
		sirius_minilog_outdent();

		variable_set('sirius_smf_worker_oneoff_dentwell_election_all_last_worker_nid', $worker_nid);
	}
}











function sirius_smf_oneoff_dentwell_election($mode, $worker_node) {
	sirius_minilog("Scanning: " . $worker_node->title . " (mode = $mode)");

	$lookups = &drupal_static(__FUNCTION__);
	sirius_minilog("... COUNT SO FAR: " . $lookups['count']);
	if (!isset($lookups['delta'])) {
		$lookups['delta'] = sirius_find_nid_by_id('D', 'sirius_trust_benefit');
		$lookups['dentwell'] = sirius_find_nid_by_id('LADC', 'sirius_trust_benefit');
	}
	if (!$lookups['delta'] || !$lookups['dentwell']) {
		sirius_minilog("Didn't find benefit NID for dentwell or delta.", 'error');
		return;
	}

	$prior_election_node = sirius_trust_worker_election($worker_node);
	if (!$prior_election_node) {
		sirius_minilog("... No currently active election found. Skipping.");
		return array('success' => TRUE);
	}

	if (!in_array($lookups['delta'], sirius_fieldvals($prior_election_node, 'field_sirius_trust_benefits', 'target_id'))) {
		sirius_minilog("... Worker did not have delta previously.");
		return array('success' => TRUE);
	}

	global $user;
  $new_election_node = new stdClass();
  $new_election_node->type = 'sirius_trust_worker_election';
  $new_election_node->language = LANGUAGE_NONE;
  $new_election_node->uid = $user->uid;
  $new_election_node->status = 1;
  $new_election_node->promote = 0;
  $new_election_node->comment = 0;

  foreach ($prior_election_node as $key => $val) {
  	if (!preg_match('/^field/', $key)) { continue; }
  	$new_election_node->{$key} = $val;
  }

	$new_election_node->field_sirius_date_start['und'][0]['value'] = date('Y-m-d') . ' 00:00:00';
	$prior_election_node->field_sirius_date_end['und'][0]['value'] = date('Y-m-d', strtotime('-1 day')) . ' 00:00:00';
	foreach ($new_election_node->field_sirius_trust_benefits['und'] as $delta => $ignore) {
		if ($new_election_node->field_sirius_trust_benefits['und'][$delta]['target_id'] == $lookups['delta']) {
			$new_election_node->field_sirius_trust_benefits['und'][$delta]['target_id'] = $lookups['dentwell'];
		}
	}

	if ($mode != 'live') {
		sirius_minilog("... (Skipping save because mode = $mode)");
		return array('success' => TRUE);
	}

	node_save($prior_election_node);
	node_save($new_election_node);
	sirius_minilog("... *** Election saved: " . $new_election_node->title . ' (' . $new_election_node->nid . ')');
	++$lookups['count'];
}