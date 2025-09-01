<?php

function sirius_trust_election_wizard_dependents_form($form, &$form_state, $info, $data, $wizard_node, $current_step) {
	$policy_node = sirius_trust_election_wizard_get_policy($data);
	$policy_json = sirius_json_get($policy_node);

	$benefits = sirius_trust_policy_benefits_offered($policy_node);

	$worker_node = sirius_trust_election_wizard_get_worker($wizard_node);
	$worker_contact_nid = $worker_node->field_sirius_contact['und'][0]['target_id'];
	if (!$worker_contact_nid) { return sirius_form_error("No contact record found for this worker."); }

	/*
	$election_type = sirius_trust_election_wizard_get_election_type($data);
	$election_type_json = sirius_json_get($election_type);
	$relationship_criteria = sirius_trust_relationship_criteria_get($election_type_json);
	$ok_relationship_types = array();
	foreach ($relationship_criteria as $uuid => $criterion) {
		$ok_relationship_types = array_merge($ok_relationship_types, array_filter(array_values($criterion['relationship_types'])));
	}
	$relationship_type_options = array();
	foreach ($ok_relationship_types as $relationship_type_tid) {
		$relationship_type_options[$relationship_type_tid] = sirius_term_title($relationship_type_tid);
	}

	$form['intro'] = array('#markup' => sirius_trust_relationship_criteria_render($relationship_criteria, array('title' => 'Please select the following dependents')));

	$ok_relationship_types = sirius_taxonomy_load('sirius_contact_relationship_types', 'tid', 'name');
	*/

	// All my relationships
  $params = array();
  $params['active'] = 'Yes';
	$params['contact_nid'] = $worker_contact_nid;
	$all_relationship_nodes = sirius_contact_relationships($params);

	// Current coverage
	$prior_election_node = sirius_trust_worker_election($worker_node);
	$prior_relationship_nodes = array();
	$relationship_nids = sirius_fieldvals($prior_election_node, 'field_sirius_contact_relations', 'target_id');
	foreach ($relationship_nids as $relationship_nid) {
		$prior_relationship_nodes[$relationship_nid] = node_load($relationship_nid);
	}
	if ($prior_relationship_nodes) {
		$options = array();
		foreach ($prior_relationship_nodes as $relationship_nid => $relationship_node) {
			$options[$relationship_nid] = sirius_trust_relationship_title($relationship_node);
		}
		if (isset($data['dependents']['general']['prior'])) {
			$default = $data['dependents']['general']['prior'];
		} else {
			$default = array_filter(array_keys($prior_relationship_nodes));
		}

		$form['dependents']['general']['prior'] = array(
			'#type' => 'checkboxes',
			'#title' => 'Continue Coverage For:',
			'#options' => $options,
			'#prefix' => '<p>' . t('You are <strong>currently providing coverage</strong> for the following dependents. If you would like to drop a dependent, uncheck the box next to their name. You will be required to upload a document providing backup for the life event that permits the coverage to be dropped.') . '</p>',
			'#default_value' => $default,
		);
	}

	// New coverage
	$start_relationship_nodes = array();
	foreach ($all_relationship_nodes as $relationship_nid => $relationship_node) {
		if ($prior_relationship_nodes[$relationship_nid]) { continue; }
		$start_relationship_nodes[$relationship_nid] = $relationship_node;
	}
	if ($start_relationship_nodes) {
		$options = array();
		foreach ($start_relationship_nodes as $relationship_nid => $relationship_node) {
			$options[$relationship_nid] = sirius_trust_relationship_title($relationship_node);
		}

		if (isset($data['dependents']['general']['start'])) {
			$default = $data['dependents']['general']['start'];
		} else {
			$default = array();
		}

		$form['dependents']['general']['start'] = array(
			'#type' => 'checkboxes',
			'#title' => 'Start Coverage For:',
			'#options' => $options,
			'#prefix' => '<p>' . t('You are <strong>not currently providing coverage</strong> for the following family members. Check the ones that you would like to add.') . '</p>',
			'#default_value' => $default,
		);
	}

	$form['dependents']['general']['count_new'] = array(
		'#title' => t('Add and Cover New Family Members:'),
		'#type' => 'select',
		'#default_value' => $data['dependents']['general']['count_new'],
		'#required' => TRUE,
		'#options' => array(
			0 => 0,
			1 => 1,
			2 => 2,
			3 => 3,
			4 => 4,
			5 => 5,
			6 => 6,
		),
		'#prefix' => '<p>' . t('If there are family members that are not listed above, you can add them now. Select the number of <strong>new family members</strong> that you would like to create and cover.') . '</p>',
	);

	$form['suffix'] = array('#markup' => $policy_json['elections']['dependents']['existing']['suffix']);

	// The default value for "prior coverage" should be everybody
	return sirius_trust_election_wizard_form_prep($form, $form_state, $info, $data, $wizard_node, $current_step);
}

function sirius_trust_election_wizard_dependents_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$current_step = $form_state['current_step'];
	$wizard_node = $form_state['wizard_node'];
	$data['dependents']['general'] = $form_state['values']['dependents']['general'];
	sirius_trust_election_set_title($wizard_node);
	sirius_wizard_submit($info, $data, $wizard_node, $current_step, "Response saved.", array('nonsequential' => FALSE));
}

function sirius_trust_election_wizard_dependents_post($info, &$data, $wizard_node, $current_step) {
	// 
	// Store existing relationships
	//

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

	return array('success' => TRUE);
}


function sirius_trust_election_wizard_dependents_render($info, &$data, $wizard_node, $current_step) {
	if ($data['dependents']['general']['prior']) {
		$relationship_nids = array_filter(array_values($data['dependents']['general']['prior']));
		foreach ($relationship_nids as $relationship_nid) {
			$html .= sirius_fakerender_field("Continue Coverage:", sirius_trust_relationship_title(node_load($relationship_nid)));
		}
	}

	if ($data['dependents']['general']['start']) {
		$relationship_nids = array_filter(array_values($data['dependents']['general']['start']));
		foreach ($relationship_nids as $relationship_nid) {
			$html .= sirius_fakerender_field("Start Coverage:", sirius_trust_relationship_title(node_load($relationship_nid)));
		}
	}

	$html .= sirius_fakerender_field("New Family Members:", $data['dependents']['general']['count_new']);

	return $html;
}
