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
		sirius_minilog("... ... Searching for: " . print_r($params, 1));
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