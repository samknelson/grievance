<?php

function sirius_smf_node_tab_worker_oneoff_election_relationships_form($form, &$form_state, $worker_node) {
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

function sirius_smf_node_tab_worker_oneoff_election_relationships_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
	$worker_node = node_load($form_state['worker_nid']);
	return sirius_smf_worker_oneoff_election_relationships($form_state['values']['mode'], $worker_node);
}

// drush --user=1 php-eval "sirius_require('sirius_smf', 'sirius_smf.oneoff_election_relationships.inc'); sirius_smf_worker_oneoff_election_relationships_all('test');"
function sirius_smf_worker_oneoff_election_relationships_all($mode) {
	sirius_minilog_echo_active(TRUE);
	sirius_denorm_realtime(FALSE);

	$batch_size = 1000;

	$last_worker_nid = variable_get('sirius_smf_worker_oneoff_election_relationships_all_last_worker_nid');
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
		sirius_smf_worker_oneoff_election_relationships($mode, $worker_node);
		sirius_domain($worker_node->field_sirius_domain['und'][0]['target_id']);
		if ($mode == 'live') {
			sirius_minilog("Saving the worker record.");
			node_save($worker_node);
		}
		sirius_minilog_outdent();

		variable_set('sirius_smf_worker_oneoff_election_relationships_all_last_worker_nid', $worker_nid);
	}
}

function sirius_smf_worker_oneoff_election_relationships($mode, $subscriber_worker_node) {
	sirius_minilog("Scanning: " . $subscriber_worker_node->title . " (mode = $mode)");

	// Get the election node
	$election_node = sirius_trust_worker_election($subscriber_worker_node);
	if (!$election_node) {
		sirius_minilog("... No currently active election found. Skipping.");
		return array('success' => TRUE);
	}

	// Get the relationships
  $relationship_params = array();
	$relationship_params['contact_nid'] = $subscriber_worker_node->field_sirius_contact['und'][0]['target_id'];
	$relationship_params['nids_only'] = TRUE;
	$all_relationship_nids = sirius_contact_relationships($relationship_params);
	if (!$all_relationship_nids) {
		sirius_minilog("No relationships found.");
		return array('success' => TRUE);
	}

	$current_relationship_nids = sirius_fieldvals($election_node, 'field_sirius_contact_relations', 'target_id');

	// Get the benefit type
  $benefit_type = &drupal_static(__FUNCTION__);
  if (!isset($benefit_type)) {
    $benefit_type = Sirius_Trust_Cobra::getInstance()->cobra_benefit_type();
    if (!$benefit_type) {
      sirius_minilog("No COBRA benefit type has been set. (I'm assuming that the COBRA benefit type is the medical benefit.)", 'error');
    }
  }
  if (!$benefit_type) { return ''; }

	$params = array();
	$params['worker_nid'] = $subscriber_worker_node->nid;
	$params['relationship_nid'] = NULL;
	$params['benefit_type'] = $benefit_type;
	$probe = sirius_trust_worker_benefits_search($params);
	$probe = sirius_trust_worker_benefits_sort_by_date($probe);
	$subscriber_wb_node = reset($probe);

  // Scan for relationships to add
  $need_save = FALSE;
  foreach ($all_relationship_nids as $relationship_nid) {
  	if (in_array($relationship_nid, $current_relationship_nids)) { continue; }

  	$relationship_node = node_load($relationship_nid);
  	$dependent_contact_nid = $relationship_node->field_sirius_contact_alt['und'][0]['target_id'];
  	$dependent_worker_node = sirius_worker_get_by_contact_nid($relationship_node->field_sirius_contact_alt['und'][0]['target_id']);
  	sirius_minilog("... Scanning dependent: " . $dependent_worker_node->title);

		$params = array();
		$params['worker_nid'] = $dependent_worker_node->nid;
		$params['subscriber_nid'] = $subscriber_worker_node->nid;
		$params['benefit_type'] = $benefit_type;
		if ($subscriber_wb_node->field_sirius_active['und'][0]['value'] == 'Yes') {
			$params['active'] = 'Yes';
		} else {
			$params['end_date'] = $subscriber_wb_node->field_sirius_date_end['und'][0]['value'];
		}
		sirius_minilog("... ... Searching for: " . preg_replace('/\s+/', ' ', print_r($params, 1)));
		$params['nids_only'] = TRUE;
		$probe = sirius_trust_worker_benefits_search($params);
		if (!$probe) {
			sirius_minilog("... ... No benefits found. Skipping.");
			continue;
		}

		sirius_minilog("... ... The dependent has a medical benefit with this subscriber. Adding them to the election.");
		$election_node->field_sirius_contact_relations['und'][] = array('target_id' => $relationship_nid);
		$need_save = TRUE;
  }

  if (!$need_save) {
  	sirius_minilog("... No changes found. The election is unchanged.");
  	return;
  }

  sirius_minilog("... Election has been updated.");
  if ($mode == 'live') {
  	node_save($election_node);
  } else {
  	sirius_minilog("... (Skipping save because mode = $mode)");
  }
  
  return array('success' => TRUE);
}