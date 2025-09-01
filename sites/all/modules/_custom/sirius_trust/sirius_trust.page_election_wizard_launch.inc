<?php

function sirius_trust_page_election_wizard_launch_access() {
	// Staff members can access
	if (user_access('sirius trust staff')) { return TRUE; }

	// Employer can access
	if (user_access('sirius trust employer')) { return TRUE; }

	return FALSE;
}


function sirius_trust_page_election_wizard_launch() {
	if (!sirius_trust_page_election_wizard_launch_access()) { return 'Access denied.'; }

	return drupal_get_form('sirius_trust_page_election_wizard_launch_form');
}

function sirius_trust_page_election_wizard_launch_form($form, &$form_state) {
	// Get the list of employers;
	$employer_options = sirius_employer_list();

	// Get our current employer.
	if (user_access('sirius trust staff')) {
		$form['employer_nid'] = array(
			'#type' => 'select',
			'#title' => t('Employer'),
			'#options' => array(NULL => t('-- Select --')) + $employer_options,
		);
	} else {
		$employer_node = sirius_employer_get();
		if (!$employer_node) { return sirius_form_error("There is no employer associated with the current user."); }
		$employer_nid = $employer_node->nid;
	}

	// Admins can pick their policy
	if (user_access('sirius trust staff')) {
		$policies = sirius_ledger_policy_list();

		$policy_options = array();
		if ($policies) {
			$policy_options = array();
			foreach ($policies as $policy_nid => $policy_node) {
				$policy_options[$policy_nid] = $policy_node->title;
			}
			$form['policy_nid'] = array(
				'#type' => 'select',
				'#title' => t('Policy'),
				'#options' => array(NULL => t('-- Select --')) + $policy_options,
			);
		}
	}

	$form['ssn'] = array(
		'#type' => 'textfield',
		'#title' => t('Subscriber SSN'),
		'#size' => 11,
		'#required' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Start the Election'),
	);

	return $form;
}

function sirius_trust_page_election_wizard_launch_form_validate($form, &$form_state) {
	$ssn = $form_state['values']['ssn'];
	if (!sirius_ssn_validate($ssn)) { form_set_error('ssn', 'Please enter a valid SSN.'); }
}

function sirius_trust_page_election_wizard_launch_form_submit($form, &$form_state) {
	$ssn = sirius_ssn_format($form_state['values']['ssn']);
	$employer_nid = $form_state['values']['employer_nid'];

	// Fetch the worker
	$worker_node = sirius_worker_get_by_ssn($ssn);
	if (!$worker_node) {
		drupal_set_message('No worker found with that SSN.', 'error');
		return;
	}

	// Verify that the worker is currently employed by the employer
	if ($employer_nid) {
		if ($worker_node->field_grievance_shop['und'][0]['target_id'] != $employer_nid) {
			drupal_set_message('That worker is not a current employee.', 'error'); 
			return;
		}
	}

	// Get the policy
	$policy_nid = $_REQUEST['policy_nid'];
	if (!$policy_nid) {
		$policy_node = sirius_ledger_policy_lookup('trust', $worker_node);
		if (!$policy_node) {
			drupal_set_message("No policy found for worker " . $worker_node->title, 'error');
			return;
		}
		$policy_nid = $policy_node->nid;
	}

	// Check for an existing election
	$query = new EntityFieldQuery;
	$query->entityCondition('entity_type', 'node');
	$query->entityCondition('bundle', 'sirius_log');
	$query->fieldCondition('field_sirius_domain', 'target_id', sirius_domain_nid());
	$query->fieldCondition('field_sirius_category', 'value', 'trust:election_wizard');
	$query->fieldCondition('field_sirius_type', 'value', 'draft');
  $query->fieldCondition('field_sirius_notes', 'value', $ssn);
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
	    $ssn, 
	    array(),
	    NULL,
	    $election_json
	  );
	}

	$wizard_json = sirius_json_get($wizard_node);
	$wizard_json['wizard']['trust_election']['policy']['policy_nid'] = $policy_nid;
	$wizard_json['wizard']['trust_election']['policy']['employer_nid'] = $employer_nid;
	$wizard_json['wizard']['trust_election']['steps']['policy']['complete'] = TRUE;
	sirius_json_set($wizard_node, $wizard_json);
	node_save($wizard_node);

	sirius_require('sirius_trust', 'sirius_trust.election_wizard.inc');
	sirius_trust_election_set_title($wizard_node);

  drupal_goto("node/" . $wizard_node->nid . "/sirius_trust_election_wizard");
}
