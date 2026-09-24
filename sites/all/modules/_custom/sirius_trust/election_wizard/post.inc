<?php

function sirius_trust_election_wizard_post($info, $data, $wizard_node, $current_step) { return drupal_get_form('sirius_trust_election_wizard_post_form', $info, $data, $wizard_node, $current_step); }

function sirius_trust_election_wizard_post_form($form, &$form_state, $info, $data, $wizard_node, $current_step) {
	$form_state['info'] = $info;
	$form_state['data'] = $data;
	$form_state['wizard_node'] = $wizard_node;
	$form_state['current_step'] = $current_step;

	$form['#tree'] = TRUE;

	$status = sirius_trust_election_wizard_get_status($wizard_node);
	$form['status'] = array('#markup' => sirius_trust_election_wizard_status_render($status, $data));

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
		$effective_ym_options = sirius_trust_election_wizard_effective_ym_options();
		$effective_ym_default = $data['post']['effective_ym'];
		if (!$effective_ym_default || !isset($effective_ym_options[$effective_ym_default])) { $effective_ym_default = date('Y-m'); }

		$form['effective_ym'] = array(
			'#type' => 'select',
			'#title' => t('Effective Month'),
			'#options' => $effective_ym_options,
			'#default_value' => $effective_ym_default,
			'#required' => TRUE,
			'#description' => t('The election will take effect on the first day of this month, and any prior election will end on the last day of the preceding month. If you select a future month, the election will be created now but will not become active until that date.'),
		);

		$form['action'] = array(
			'#type' => 'select',
			'#title' => t('Action'),
			'#options' => array(
				NULL => t('-- Select --'),
				'approve' => t('Approve the election but do not post'),
				'post' => t('Post the election'),
			),
			'#required' => TRUE,
			'#suffix' => '<p><strong>Posting will create or update any dependent and relationship records, and will create an election for the worker as of the effective month selected above.</strong></p>',
		);
	}

	return sirius_trust_election_wizard_form_prep($form, $form_state, $info, $data, $wizard_node, $current_step);
}

function sirius_trust_election_wizard_post_form_validate($form, &$form_state) {
	if ($form_state['values']['action'] != 'post') { return; }

	$effective_ym = $form_state['values']['effective_ym'];
	$effective_ym_options = sirius_trust_election_wizard_effective_ym_options();
	if (!$effective_ym || !isset($effective_ym_options[$effective_ym]) || !strtotime("$effective_ym-01")) {
		form_set_error('effective_ym', t('Please select a valid effective month.'));
	}
}

function sirius_trust_election_wizard_post_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$wizard_node = $form_state['wizard_node'];
	$current_step = $form_state['current_step'];

	$action = $form_state['values']['action'];
	if ($action == 'approve') {
		$wizard_node->field_sirius_type['und'][0]['value'] = 'approved';
	} else if ($action == 'post') {
		// Record the effective month so that the post callbacks (and the saved wizard data) can see it.
		$data['post']['effective_ym'] = $form_state['values']['effective_ym'];
		drupal_set_message("Effective date: " . date('m/d/Y', sirius_trust_election_wizard_effective_ts($data)));

		// Post all steps
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
	} else {
		drupal_set_message("Error: Unknown action $action");
	}
	sirius_wizard_submit($info, $data, $wizard_node, $current_step, "Response saved.", array('nonsequential' => FALSE));
}

function sirius_trust_election_wizard_post_post($info, &$data, $wizard_node, $current_step) {
	$worker_node = sirius_trust_election_wizard_get_worker($wizard_node);
	$policy_node = sirius_trust_election_wizard_get_policy($data);

	// The election takes effect on the first of the selected month (default: the current month).
	$effective_ts = sirius_trust_election_wizard_effective_ts($data);
	$effective_date = date('Y-m-d 00:00:00', $effective_ts);
	$prior_end_date = date('Y-m-d 00:00:00', strtotime('-1 day', $effective_ts));

	// New relationships start no later than this month, so that they are visible as active relationships right away
	// even if the election itself is future-dated. A backdated election backdates the relationship along with it.
	$this_month_ts = strtotime(date('Y-m-01'));
	$relationship_start_date = date('Y-m-d 00:00:00', min($effective_ts, $this_month_ts));

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
	  $dependent_worker_node->field_sirius_gender['und'][0]['tid'] = $dependent['gender'];
	  if ($dependent['gender_nota_val']) {
		  $dependent_worker_node->field_sirius_gender_nota_val['und'][0]['value'] = $dependent['gender_nota_val'];
	  }

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
		  $relationship_node->field_sirius_date_start['und'][0]['value'] = $relationship_start_date;
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

	// Cancel any existing election that is still in effect on the effective date. That includes the currently
	// active election, any previously posted future election (which this one supersedes), and, if we are
	// backdating, any past election that overlaps the effective date. They all end the day before we start.
	$prior_election_nodes = sirius_trust_worker_elections_search(array('worker_nid' => $worker_node->nid, 'limit' => 1000));
	foreach ($prior_election_nodes as $election_node) {
		$prior_end_value = $election_node->field_sirius_date_end['und'][0]['value'];
		if ($prior_end_value && strtotime($prior_end_value) < $effective_ts) { continue; }
		$election_node->field_sirius_date_end['und'][0]['value'] = $prior_end_date;
		node_save($election_node);
		drupal_set_message("Canceling prior election: " . $election_node->title . " as of " . date('m/d/Y', strtotime($prior_end_date)));
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
  $benefit_nids = $data['benefits']['all'];
  if (!$benefit_nids) { $benefit_nids = array(); }
  $benefit_nids = array_filter($benefit_nids);

  // Add the benefits to our election.
  $benefit_nids = array_unique(array_filter($benefit_nids));
  foreach ($benefit_nids as $benefit_nid) {
	  $election_node->field_sirius_trust_benefits['und'][] = array('target_id' => $benefit_nid);
	}

  // $election_node->field_sirius_trust_election_type['und'][0]['tid'] = $data['election_type']['tid'];

  $election_node->field_sirius_trust_policy['und'][0]['target_id'] = $policy_node->nid;

	$election_node->field_sirius_date_start['und'][0]['value'] = $effective_date;

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

	if ($effective_ts > time()) {
		drupal_set_message("The election is scheduled to take effect on " . date('m/d/Y', $effective_ts) . ". It will become the active election on that date.");
	} else {
		drupal_set_message("The election is effective as of " . date('m/d/Y', $effective_ts) . ".");
	}

	sirius_log_handler_set($wizard_node, $election_node->nid);

	drupal_set_message("Starting benefits");
	sirius_trust_wb_scan_worker('live', $worker_node);

	return array('success' => TRUE);
}
