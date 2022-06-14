<?php

function sirius_trust_node_tab_election_wizard_access($wizard_node) {
	// Must be an election-type log entry
	if ($wizard_node->field_sirius_category['und'][0]['value'] != 'trust:election_wizard') { return FALSE; }

	return TRUE;
}

function sirius_trust_node_tab_election_wizard_denied($msg) {
	drupal_set_message($msg, 'error');
	return 'Access denied.';
}

function sirius_trust_node_tab_election_wizard($wizard_node) {
	// 
	// This should probably be in the _access function, but service calls are expensive and _access() is called a lot
	//

	// Must have an SSN
	$ssn = $wizard_node->field_sirius_notes['und'][0]['value'];
	if (!sirius_ssn_validate($ssn)) { return sirius_trust_node_tab_election_wizard_denied("There is no SSN associated with this wizard."); }

	// Staff can access
	if (user_access('sirius trust staff')) { 
		// No further checks required.
	}

	// Employers can access their emlpoyees' elections
	else if (user_access('sirius trust employer')) {
		$employer_rnid = sirius_proxy_get_user_employer_nid();
		if (!$employer_rnid) { return sirius_trust_node_tab_election_wizard_denied("Your user is not associated with a valid employer"); }

		$result = sirius_proxy_call('sirius_worker_get_by_ssn', $ssn);
		if (!$result['success']) { return sirius_trust_node_tab_election_wizard_denied($result['msg']); }
		if (!$result['data']) { return sirius_trust_node_tab_election_wizard_denied('No worker found with the given SSN.'); }
		$worker_rnode = (object) $result['data'];

		// Verify that the worker is currently employed by the employer
		if ($worker_rnode->field_grievance_shop['und'][0]['target_id'] != $employer_rnid) { return sirius_trust_node_tab_election_wizard_denied('That worker is not currently employed by your employer.'); }
		$can_access = TRUE;
	}

	// Oh well
	else {
		 return sirius_trust_node_tab_election_wizard_denied('You do not have one of the permissions required to access this wizard.');
	}

	return sirius_node_tab_wizard($wizard_node, 'trust_election');
}

