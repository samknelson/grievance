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

function sirius_smf_oneoff_dentwell_election($mode, $worker_node) {
	sirius_minilog("Scanning: " . $worker_node->title . " (mode = $mode)");

	$lookups = &drupal_static(__FUNCTION__);
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
	sirius_minilog("... Election saved: " . $new_election_node->title . ' (' . $new_election_node->nid . ')');
}