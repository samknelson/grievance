<?php

function sirius_ledger_monthly_enabled() {
	return sirius_domain_variable_get('sirius_ledger_monthly_enabled') == 'Yes';
}

function sirius_ledger_hourly_enabled() {
	return sirius_domain_variable_get('sirius_ledger_hourly_enabled') == 'Yes';
}

function sirius_ledger_page_monthly_test() {
	return drupal_get_form('sirius_ledger_page_monthly_test_form');
}

function sirius_ledger_page_monthly_test_form($form, &$form_state) {
	$form['intro'] = array(
		'#markup' => '<div class="sirius_form_intro">' . t('Use the form below to test the monthly charges for a specific worker.') . '</div>',
	);

	$html = '<h2>Monthly Dues Batch Status</h2>';
	$batch_status = json_decode(sirius_domain_variable_get('sirius_ledger_monthly_batch_status'), TRUE);
	$date = $batch_status['date'];
	if (!$date) { $date = '[none]'; }
	$html .= sirius_fakerender_field('Date:', $date);
	$complete = $batch_status['complete'];
	if ($complete) { $complete = 'True'; } else { $complete = 'False'; }
	$html .= sirius_fakerender_field('Complete:', $complete);
	$last_worker_nid = $batch_status['last_worker_nid'];
	if (!$last_worker_nid) { $last_worker_nid = '[none]'; }
	$html .= sirius_fakerender_field('Last Worker NID:', $last_worker_nid);
	$form['status'] = array('#markup' => $html);

	$form['mode'] = array(
		'#type' => 'select',
		'#title' => t('Mode'),
		'#required' => TRUE,
		'#options' => array(
			'test' => t('Test mode - execute all the steps but do not save any charges.'),
			'live' => t('Live mode - create or update A/R entries'),
		),
	);

	$form['action'] = array(
		'#type' => 'select',
		'#title' => t('Action'),
		'#required' => TRUE,
		'#options' => array(
			'worker' => t('Compute for a single worker'),
			'batch' => t('Compute the next batch'),
			'batch_reset' => t('Restart batch computations for today'),
		),
	);


	$form['worker_nid'] = array(
		'#type' => 'textfield',
		'#title' => t('Worker NID'),
		'#states' => array(
			'visible' => array(
				":input[name=\"action\"]" => array('value' => 'worker')
			),
		),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

  $log = sirius_minilog_render_html();
  if ($log) { $form['results'] = array('#markup' => '<hr>' . $log); }

	return $form;
}

function sirius_ledger_page_monthly_test_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
	if ($form_state['values']['action'] == 'worker') {
		sirius_ledger_monthly_charge_worker($form_state['values']['worker_nid'], $form_state['values']['mode']);
	} else if ($form_state['values']['action'] == 'batch_reset') {
		sirius_ledger_monthly_charge_batch_reset();
	} else {
		sirius_ledger_monthly_charge_batch($form_state['values']['mode']);
	}
}

function sirius_ledger_monthly_cron() {
	$last_ts = variable_get('sirius_ledger_monthly_cron_last_ts');
	if (time() - $last_ts < 3600) { return; }
	variable_set('sirius_ledger_monthly_cron_last_ts', time());
	sirius_ledger_monthly_charge_batch_all('live');
}

function sirius_ledger_monthly_charge_batch_all($mode) {
	$domains = sirius_domain_tree_render_as_select();
	$original_domain_nid = sirius_domain_nid();
	sirius_minilog("Starting monthly dues batch processing (all domains)");
	foreach ($domains as $domain_nid => $domain_name) {
		sirius_minilog("Switching to domain $domain_name.");
		sirius_domain($domain_nid);
		sirius_ledger_monthly_charge_batch($mode);
	}
	sirius_domain($original_domain_nid);
	sirius_minilog("Monthly dues batch processing complete.");
}

function sirius_ledger_monthly_charge_batch_reset() {
	sirius_domain_variable_set('sirius_ledger_monthly_batch_status', NULL);
}

function sirius_ledger_monthly_charge_batch($mode) {
	if (!sirius_ledger_monthly_enabled()) {
		sirius_minilog('Monthly billing is not enabled.');
		return;
	}

	$batch_size = sirius_domain_variable_get('sirius_ledger_monthly_batch_size', 1000);

	$batch_status = json_decode(sirius_domain_variable_get('sirius_ledger_monthly_batch_status'), TRUE);
	$today = date('Y-m-d');
	if ($batch_status['date'] != $today) {
		$batch_status = array('date' => $today);
		sirius_minilog("Starting batch processing for $today"); 
	} else {
		if ($batch_status['complete']) { 
			sirius_minilog("Batch processing for $today is complete."); 
			return;
		} else {
			sirius_minilog("Continuing batch processing for $today"); 
		}
	}

	$domain_nid = sirius_domain_nid();
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_worker');
  if ($domain_nid) { $query->fieldCondition('field_sirius_domain', 'target_id', $domain_nid); }
  if ($batch_status['last_worker_nid']) { $query->propertyCondition('nid', $batch_status['last_worker_nid'], '>'); }
  $query->propertyOrderBy('nid');
  $query->range(0, $batch_size);
  $stmt = $query->execute();
  $worker_nids = array();
  if ($stmt['node']) { $worker_nids = array_keys($stmt['node']); }

  $count = 0;
  foreach ($worker_nids as $worker_nid) {
  	sirius_ledger_monthly_charge_worker($worker_nid, $mode);
		$batch_status['last_worker_nid'] = $worker_nid;
		++$count;
  }
  if ($count < $batch_size) { 
  	$batch_status['complete'] = TRUE;
  	sirius_minilog("All records for $today processed."); 
  }

	if ($mode == 'live') { 
		sirius_domain_variable_set('sirius_ledger_monthly_batch_status', json_encode($batch_status));
	}
}

function sirius_ledger_monthly_charge_worker($worker_nid, $mode) {
	if (!sirius_ledger_monthly_enabled()) {
		sirius_minilog('Monthly billing is not enabled.');
		return;
	}

	$worker_node = node_load($worker_nid);
	if (!$worker_node) { 
		sirius_minilog('No node found.', 'error');
		return;
	} else if ($worker_node->type != 'sirius_worker') {
		sirius_minilog('That is not a valid worker node.', 'error');
		return;
	} else if ($worker_node->field_sirius_domain['und'][0]['target_id'] != sirius_domain_nid()) {
		sirius_minilog('That is not a worker in the current domain.', 'error');
		return;
	}

	sirius_minilog("Calculating monthly dues for " . $worker_node->title);
	sirius_minilog("Operating mode: $mode");

	$work_status_term = taxonomy_term_load($worker_node->field_sirius_work_status['und'][0]['tid']);
	if (!$work_status_term) {
		sirius_minilog('The member has no work status.');
		return;
	}
	if (!$work_status_term->field_sirius_member_active['und'][0]['value'] == 'Yes') {
		sirius_minilog("The member has work status " . $work_status_term->name . '. Dues are not charged to members with this status.');
		return;
	}

	$policy_node = sirius_ledger_policy_lookup('month', $worker_node);
	if (!$policy_node) {
		sirius_minilog("No ledger policy found for " . $worker_node->title, 'warning');
		return;
	}

	$policy_json = sirius_jsonfield_util_node_json_get($policy_node);
	$monthly_charge_day = $policy_json['ledger']['policy']['settings']['monthly_charge_day'];
	if ($monthly_charge_day > date('j')) {
		sirius_minilog("Monthly charges under this policy will not be made until day $monthly_charge_day day of the month.");
		return;
	}

	$rates = sirius_ledger_policy_lookup_rates($worker_node, $policy_node, 'month');
	$accounts = sirius_ledger_account_list();
	foreach ($rates as $account_nid => $rate_record) {
		$rate = $rate_record['rate'];
		if ($account_nid == 'pay') { continue; }

		// Figure out who to charge
		$account_node = node_load($account_nid);
		$account_json = sirius_jsonfield_util_node_json_get($account_node);
		if ($account_json['settings']['payments']['allocate']['employer']['simple']) { $charge_to = 'employer'; } else { $charge_to = 'worker'; }
		sirius_minilog("- Monthly charge for " .  $accounts[$account_nid] . ": \$$rate charged to $charge_to");

		// Start building the entry
		$entry = array();
		$entry['ledger_status'] = 'Cleared';
		$entry['ledger_account'] = $account_nid;
		if ($charge_to == 'worker') { 
			$entry['ledger_participant'] = $worker_node->field_sirius_contact['und'][0]['target_id'];
		} else {
			$entry['ledger_participant'] = $worker_node->field_grievance_shop['und'][0]['target_id'];
		}
		$entry['ledger_key'] = $worker_node->nid . '|' . date('Y-m');
		$entry['ledger_reference'] = $policy_node->nid;

		// Charged already?
		$previous_entries = sirius_ledger_ar_fetch($entry);
		if ($previous_entries) {
			$entry = $previous_entries[0];
			if ($entry['ledger_amount'] == $rate) {
				sirius_minilog('- Found a previous ledger entry with the same rate. Skipping.');
			} else {
				$entry['ledger_amount'] = $rate;
				$entry['ledger_ts'] = time();
				sirius_minilog('- Found a previous ledger entry with a different rate. Updating.');
				if ($mode == 'live') { sirius_ledger_ar_update($entry); } else { sirius_minilog("(Test mode, skipping insert)"); }
			}
		}

		$entry['ledger_amount'] = $rate;
		$entry['ledger_ts'] = time();

		$memo = "Monthly charge for " . $worker_node->title . ': ';
		$memo .= '$' . number_format($rate, 2);
		$memo .= ' (MS ';
		$memo .= $rate_record['ms_name'];
		if ($rate_record['level_name']) { $memo .= ' ' . $rate_record['level_name']; }
		$memo .= ')';

		$entry['ledger_memo'] = $memo;
		sirius_minilog('- Creating a new ledger entry for ' . $worker_node->title);
		if ($mode == 'live') { sirius_ledger_ar_insert($entry); } else { sirius_minilog("(Test mode, skipping insert)"); }
	}
}

