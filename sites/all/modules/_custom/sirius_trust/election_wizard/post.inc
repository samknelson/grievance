<?php

function sirius_trust_election_wizard_post($info, $data, $wizard_node, $current_step) { return drupal_get_form('sirius_trust_election_wizard_post_form', $info, $data, $wizard_node, $current_step); }

function sirius_trust_election_wizard_post_form($form, &$form_state, $info, $data, $wizard_node, $current_step) {
	$form_state['info'] = $info;
	$form_state['data'] = $data;
	$form_state['wizard_node'] = $wizard_node;
	$form_state['current_step'] = $current_step;

	$form['#tree'] = TRUE;

	$form['status'] = array('#markup' => sirius_trust_election_wizard_status_render($info, $data, $wizard_node, $current_step));
	
	$steps = sirius_trust_election_wizard_steps($data, $wizard_node);
	foreach ($steps as $step_id => $step) {
		if ($step['file']) { sirius_require($step['module'], $step['file']); }
		if (!$step['callback_render']) { continue; }
		if (!function_exists($step['callback_render'])) {
			drupal_set_message("Missing callback: " . $step['callback_render'], 'error');
			return;
		}

		$html = $step['callback_render']($info, $data, $wizard_node, $step_id);

		$html_full = '';
		if ($html) {
			$html_full .= '<div class="sirius_wizard_render_section sirius_callout_basic">';
			$html_full .= '<h2>' . $step['label'] . '</h2>';
			$html_full .= $html;
			$html_full .= '</div>';
			$form[$step_id] = array('#markup' => $html_full);
		}
	}

	$access = Sirius_Trust_Access::getInstance();
	$access_context = array('election_wizard_node' => $wizard_node, 'current_step' => $current_step);

	if ($access->access('election_wizard_edit', $access_context)) {
		$form['post'] = array(
			'#markup' => '<p><strong>Click below to post this election wizard. This will create or update any dependent and relationship records, and will create an active election for the worker.</strong></p>',
		);
	}

	return sirius_trust_election_wizard_form_prep($form, $form_state, $info, $data, $wizard_node, $current_step);
}

function sirius_trust_election_wizard_post_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$wizard_node = $form_state['wizard_node'];
	$current_step = $form_state['current_step'];

	// Create any relationships
	$steps = sirius_trust_election_wizard_steps($data, $wizard_node);
	foreach ($steps as $step_id => $step) {
		if (!$step['callback_post']) { continue; }
		drupal_set_message("Posting: " . $step['label']);
		if (!function_exists($step['callback_post'])) {
			drupal_set_message("Missing callback: " . $step['callback_post'], 'error');
			return;
		}

		$result = SiriusResult::create($step['callback_post']($info, $data, $wizard_node, $step_id));
		if (!$result->success()) { return $result->drupalSetMessage(); }
	}

	$wizard_node->field_sirius_type['und'][0]['value'] = 'posted';
	sirius_wizard_submit($info, $data, $wizard_node, $current_step, "Response saved.", array('nonsequential' => FALSE));
}

function sirius_trust_election_wizard_status_render($info, $data, $wizard_node, $current_step) {
	$status = sirius_trust_election_wizard_get_status($wizard_node);
	$html .= '<div class="sirius_wizard_render_section sirius_callout_basic">';
	$html .= '<strong>';
	if ($status == 'draft') {
		$html .= 'This is a DRAFT election. It can still be edited.';
	} else if ($status == 'submitted') {
		$html .= 'This is a SUBMITTED election. It is locked and can only be edited by staff. It has not yet been posted.';
	} else if ($status == 'posted') {
		$html .= 'This is a POSTED election. It has been posted to the member\'s record, and cannot be changed. To make any changes, a new election must be created.';
	}
	$html .= '</strong>';
	$html .= '</div>';
	return $html;
}

function sirius_trust_election_wizard_post_post($info, &$data, $wizard_node, $current_step) {
	$worker_node = sirius_trust_election_wizard_get_worker($wizard_node);
	$policy_node = sirius_trust_election_wizard_get_policy($data);


	// 
	// Create any new workers
	// 

	$count_new = $data['dependents']['general']['count_new'];
	for ($which = 1; $which <= $count_new; ++$which) {
		$dependent = $data['dependents']['new'][$which];
		if (!$dependent) { return array('success' => FALSE, 'msg' => "Missing new dependent #$which"); }

		$ssn = $dependent['ssn'];
		if (!$ssn || !sirius_ssn_validate($ssn)) { return array('success' => FALSE, 'msg' => "Missing or bad SSN for dependent #$which"); }
		$ssn = sirius_ssn_format($ssn);
		$probe = sirius_worker_get_by_ssn($ssn);
		if ($probe) {
			return array('success' => FALSE, 'msg' => "There is already a dependent with the SSN $ssn. The relationship must be created manually.");
		}

		$dependent_worker_node = sirius_worker_get_by_ssn($ssn);
		if ($dependent_worker_node) {
			drupal_set_message("... Relationship #$which: Found existing worker for $ssn.");
			continue; 
		}

		drupal_set_message("... Relationship #$which: Creating a new worker for $ssn.");

		// Create the worker node
	  global $user;
	  $dependent_worker_node = new stdClass();
	  $dependent_worker_node->type = 'sirius_worker';
	  $dependent_worker_node->language = LANGUAGE_NONE;
	  $dependent_worker_node->uid = $user->uid;
	  $dependent_worker_node->status = 1;
	  $dependent_worker_node->promote = 0;
	  $dependent_worker_node->comment = 0;

	  // Set the domain
	  $dependent_worker_node->field_sirius_domain['und'][0]['target_id'] = sirius_domain_nid();
	  $dependent_worker_node->field_sirius_ssn['und'][0]['value'] = $ssn;
	  $dependent_worker_node->field_sirius_name['und'][0]['given'] = $dependent['name_first'];
	  $dependent_worker_node->field_sirius_name['und'][0]['family'] = $dependent['name_last'];
	  if ($dependent['phone']) {
	  	$dependent_worker_node->field_sirius_phone['und'][0]['value'] = sirius_phone_format($dependent['phone']);
	  }
	  $dependent_worker_node->field_sirius_dob['und'][0]['value'] = $dependent['dob'] . ' 00:00:00';

		node_save($dependent_worker_node);
	}

	// 
	// Create or update any relationships
	//

	$count_new = $data['dependents']['general']['count_new'];
	$worker_node = sirius_trust_election_wizard_get_worker($wizard_node);
	$worker_contact_nid = $worker_node->field_sirius_contact['und'][0]['target_id'];

	for ($which = 1; $which <= $count_new; ++$which) {
		$dependent = $data['dependents']['new'][$which];
		$ssn = $dependent['ssn'];
		$dependent_worker_node = sirius_worker_get_by_ssn($ssn);
		$dependent_contact_nid = $dependent_worker_node->field_sirius_contact['und'][0]['target_id'];
		if (!$dependent_contact_nid) { return array('success' => FALSE, 'msg' => 'Missing contact ID for worker ' . $dependent_worker_node->nid); }
		$reltype_tid = $dependent['reltype'];
		if (!$reltype_tid) { return array('success' => FALSE, 'msg' => 'Missing relationship type.'); }

		// Search
		$search_params = array();
		$params['contact_nid'] = $worker_contact_nid;
		$params['contact_alt_nid'] = $dependent_contact_nid;
		$params['reltype_tid'] = $reltype_tid;
		$relationships = sirius_contact_relationships($params);
		$relationship_node = reset($relationships);

		if (!$relationship_node) {
			drupal_set_message("... Relationship #$which: Creating new relationship to $ssn.");
		  global $user;
		  $relationship_node = new stdClass();
		  $relationship_node->type = 'sirius_contact_relationship';
		  $relationship_node->language = LANGUAGE_NONE;
		  $relationship_node->uid = $user->uid;
		  $relationship_node->status = 1;
		  $relationship_node->promote = 0;
		  $relationship_node->comment = 0;

		  // Set the domain
		  $relationship_node->field_sirius_domain['und'][0]['target_id'] = sirius_domain_nid();
		  $relationship_node->field_sirius_contact['und'][0]['target_id'] = $worker_contact_nid;
		  $relationship_node->field_sirius_contact_alt['und'][0]['target_id'] = $dependent_contact_nid;
		  $relationship_node->field_sirius_contact_reltype['und'][0]['tid'] = $reltype_tid;
		  $relationship_node->field_sirius_date_start['und'][0]['value'] = date('Y-m-1 00:00:00');
		} else {
			drupal_set_message("... Relationship #$which: Updating existing relationship to $ssn.");
		}

		unset($relationship_node->field_sirius_date_end['und']);
		node_save($relationship_node);
		$data['dependents']['new'][$which]['relationship_nid'] = $relationship_node->nid;

		$log_node = sirius_log(
  		'election', 
  		'election', 
  		'Created by election', 
  		'Created by election', 
  		'Created by election', 
  		array($relationship_node->nid, $wizard_node->nid),
  	);
	}

	// Cancel any existing election (as of the beginning of the preceding month)
	$no_infinite_loop = 0;
	while ($election_node = sirius_trust_worker_election($worker_node)) {
		++$no_infinite_loop; if ($no_infinite_loop > 10) { break; }
		$election_node->field_sirius_date_end['und'][0]['value'] = date('Y-m-t 00:00:00', strtotime('-1 month'));
		node_save($election_node);
		drupal_set_message("Canceling prior election: " . $election_node->title);
	}

	// Create a new election
  global $user;
  $election_node = new stdClass();
  $election_node->type = 'sirius_trust_worker_election';
  $election_node->language = LANGUAGE_NONE;
  $election_node->uid = $user->uid;
  $election_node->status = 1;
  $election_node->promote = 0;
  $election_node->comment = 0;

  $election_node->field_sirius_domain['und'][0]['target_id'] = sirius_domain_nid();
  $election_node->field_sirius_worker['und'][0]['target_id'] = $worker_node->nid;

  // All of our elected benefits
  $benefit_nids = array();
  if ($data['benefits']['by_type']) {
  	foreach ($data['benefits']['by_type'] as $type_tid => $benefit_nid) { 
  		$benefit_nids[] = $benefit_nid;
  	}
  }

  // Add any "non-electable" benefits
	$benefit_nids = array_merge($benefit_nids, array_keys(sirius_trust_policy_benefits_offered($policy_node, array('electable' => FALSE, 'worker_node' => $worker_node))));

  $benefit_nids = array_unique(array_filter($benefit_nids));
  if ($benefit_nids) {
  	foreach ($benefit_nids as $benefit_nid) {
		  $election_node->field_sirius_trust_benefits['und'][] = array('target_id' => $benefit_nid);
  	}
  }

  // $election_node->field_sirius_trust_election_type['und'][0]['tid'] = $data['election_type']['tid'];

  $election_node->field_sirius_trust_policy['und'][0]['target_id'] = $policy_node->nid;

	$election_node->field_sirius_date_start['und'][0]['value'] = date('Y-m-1 00:00:00');

	$relationship_nids = array();
	if ($data['dependents']['existing']) {
		foreach ($data['dependents']['existing'] as $relationship_nid => $tmp) {
			if ($tmp['active'] != 'Yes') { continue; }
			$relationship_nids[] = $relationship_nid;
		}
	}
	$count_new = $data['dependents']['general']['count_new'];
	for ($which = 1; $which <= $count_new; ++$which) { $relationship_nids[] = $data['dependents']['new'][$which]['relationship_nid']; }
	$relationship_nids = array_filter(array_unique($relationship_nids));
	foreach ($relationship_nids as $relationship_nid) {
		$election_node->field_sirius_contact_relations['und'][] = array('target_id' => $relationship_nid);
	}

	$election_node->field_grievance_shop['und'][0]['target_id'] = $data['employer']['employer_nid'];

	node_save($election_node);

	sirius_log_handler_set($wizard_node, $election_node->nid);

	drupal_set_message("Starting benefits");
	sirius_trust_wb_scan_worker('live', $worker_node);

	return array('success' => TRUE);
}
