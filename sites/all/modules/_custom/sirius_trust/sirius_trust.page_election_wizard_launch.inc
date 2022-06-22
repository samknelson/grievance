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
	$result = sirius_proxy_call('sirius_employer_list', array());
	if (!$result['success']) { return sirius_form_error($result['msg']); }
	$employer_options = $result['data'];

	// Get our current employer.
	if (user_access('sirius trust staff')) {
		$form['employer_rnid'] = array(
			'#type' => 'select',
			'#title' => t('Employer'),
			'#options' => array(NULL => t('-- Select --')) + $employer_options,
		);
	} else {
		$employer_rnid = sirius_proxy_get_user_employer_nid();
		if (!$employer_rnid) { return sirius_form_error("There is no employer associated with the current user."); }
		$form['employer_rnid'] = array(
			'#type' => 'value',
			'#value' => $employer_rnid,
		);
	}

	// Admins can pick their policy
	if (user_access('sirius trust staff')) {
		$result = sirius_proxy_call('sirius_ledger_policy_list');
		if (!$result['success']) { return sirius_form_error($result['msg']); }
		$policies = $result['data'];

		$policy_options = array();
		if ($policies) {
			$policy_options = array();
			foreach ($policies as $policy_rnid => $policy_rnode) {
				$policy_options[$policy_rnid] = $policy_rnode->title;
			}
			$form['policy_rnid'] = array(
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
	$employer_rnid = $form_state['values']['employer_rnid'];

	// Fetch the worker
	$result = sirius_proxy_call('sirius_worker_get_by_ssn', $ssn);
	if (!$result['success']) { drupal_set_message($result['msg'], 'error'); return; }
	if (!$result['data']) { drupal_set_message('No worker found with that SSN.', 'error'); return; }
	$worker_rnode = $result['data'];

	// Verify that the worker is currently employed by the employer
	if ($employer_rnid) {
		if ($worker_rnode->field_grievance_shop['und'][0]['target_id'] != $employer_rnid) {
			drupal_set_message('That worker is not a current employee.', 'error'); 
			return;
		}
	}

	// Get the policy
	$policy_rnid = $_REQUEST['policy_rnid'];
	if (!$policy_rnid) {
		$result = sirius_proxy_call('sirius_ledger_policy_lookup', 'trust', $worker_rnode);
		if (!$result['success']) { drupal_set_message($result['msg'], 'error'); return; }
		$policy_rnode = $result['data'];
		if (!$policy_rnode) {
			drupal_set_message("No policy found for worker " . $worker_rnode->title, 'error');
			return;
		}
		$policy_rnid = $policy_rnode->nid;
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
	    $worker_rnode->title, 
	    'Election',
	    $ssn, 
	    array(),
	    NULL,
	    $election_json
	  );
	}

	$wizard_json = sirius_json_get($wizard_node);
	$wizard_json['wizard']['trust_election']['policy']['policy_rnid'] = $policy_rnid;
	$wizard_json['wizard']['trust_election']['policy']['employer_rnid'] = $employer_rnid;
	$wizard_json['wizard']['trust_election']['steps']['policy']['complete'] = TRUE;
	sirius_json_set($wizard_node, $wizard_json);
	node_save($wizard_node);

	sirius_require('sirius_trust', 'sirius_trust.election_wizard.inc');
	sirius_trust_election_set_title($wizard_node);

  drupal_goto("node/" . $wizard_node->nid . "/sirius_trust_election_wizard");
}