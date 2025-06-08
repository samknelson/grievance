<?php

function sirius_trust_node_tab_worker_election_wizard_access($worker_node) {
  return Sirius_Trust_Access::getInstance()->access('worker_election_wizard', array('worker_node' => $worker_node));
}

function sirius_trust_node_tab_worker_election_wizard_form($form, &$form_state, $worker_node) {
	$form_state['worker_nid'] = $worker_node->nid;
	$form['#tree'] = TRUE;

	$form['confirm'] = array(
		'#type' => 'checkbox',
		'#title' => t('I would like to make changes to my current election.'),
		'#required' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Change My Election'),
	);

	return $form;
}

function sirius_trust_node_tab_worker_election_wizard_form_submit($form, &$form_state) {
	// Fetch the worker
	$worker_node = node_load($form_state['worker_nid']);
	if (!$worker_node) {
		drupal_set_message('No worker found.', 'error');
		return;
	}

	// Check for an existing election
	$query = new EntityFieldQuery;
	$query->entityCondition('entity_type', 'node');
	$query->entityCondition('bundle', 'sirius_log');
	$query->fieldCondition('field_sirius_domain', 'target_id', sirius_domain_nid());
	$query->fieldCondition('field_sirius_category', 'value', 'trust:election_wizard');
	$query->fieldCondition('field_sirius_type', 'value', array('draft', 'submitted'), 'in');
  $query->fieldCondition('field_sirius_notes', 'value', $worker_node->nid);
	$stmt = $query->execute();
	if ($stmt['node']) {
		$wizard_nid = array_keys($stmt['node'])[0];
		$wizard_node = node_load($wizard_nid);
	} else {
		$wizard_json = array();
	  $wizard_node = sirius_log(
	    'trust:election_wizard', 
	    'draft', 
	    $worker_node->title, 
	    'Election',
	    $worker_node->nid, 
	    array($worker_node->nid),
	    NULL,
	    array()
	  );
	}

	$wizard_json = sirius_json_get($wizard_node);
	$wizard_json['wizard']['trust_election']['policy']['worker_nid'] = $worker_node->nid;
	sirius_json_set($wizard_node, $wizard_json);
	node_save($wizard_node);

	sirius_require('sirius_trust', 'sirius_trust.election_wizard.inc');
	sirius_trust_election_set_title($wizard_node);

  drupal_goto("node/" . $wizard_node->nid . "/sirius_trust_election_wizard");
}
