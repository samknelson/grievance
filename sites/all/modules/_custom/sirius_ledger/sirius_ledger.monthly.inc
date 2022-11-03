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
	$html = '<h2>Monthly and Semi-Monthly Dues Batch Status</h2>';
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

	$form['trigger'] = array(
		'#type' => 'select',
		'#title' => t('Trigger'),
		'#options' => sirius_ledger_policy_trigger_options('month'),
	);

	$form['date'] = array(
		'#type' => 'date_popup',
		'#title' => t('As Of'),
    '#attributes' => array('autocomplete' => 'off'),
    '#description' => t('Leave blank to make charges "as of" today. If running in batch mode, selecting a date will clear the batch progress for today.'),
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

	$ts = strtotime($form_state['values']['date']);
	if ($ts <= 1) { $ts = NULL; }

	$form_state['rebuild'] = TRUE;
	if ($form_state['values']['action'] == 'worker') {
		$ts = strtotime($form_state['values']['date']);
		if ($ts <= 1) { $ts = NULL; }
		sirius_ledger_monthly_charge_worker($form_state['values']['worker_nid'], $form_state['values']['mode'], $form_state['values']['trigger'], $ts);
	} else if ($form_state['values']['action'] == 'batch_reset') {
		sirius_ledger_monthly_charge_batch_reset();
	} else {
		sirius_ledger_monthly_charge_batch($form_state['values']['mode'], $ts, $form_state['values']['trigger']);
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

function sirius_ledger_monthly_charge_batch($mode, $ts = NULL, $trigger = 'cron') {
	if (!$ts) { $ts = time(); }
	if (!sirius_ledger_monthly_enabled()) {
		return array('success' => TRUE, 'msg' => 'Monthly billing is not enabled for this domain.');
	}

	$batch_size = sirius_domain_variable_get('sirius_ledger_monthly_batch_size', 1000);
	$batch_status = json_decode(sirius_domain_variable_get('sirius_ledger_monthly_batch_status'), TRUE);
	$today = date('Y-m-d', $ts);
	if ($batch_status['date'] != $today) {
		$batch_status = array('date' => $today);
		sirius_minilog("Starting monthly charging for $today"); 
	} else {
		if ($batch_status['complete']) { 
			return array('success' => TRUE, 'msg' => "Batch processing for $today is complete."); 
		} else {
			sirius_minilog("-- Continuing previously started batch"); 
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
  	sirius_ledger_monthly_charge_worker($worker_nid, $mode, $trigger, $ts);
		$batch_status['last_worker_nid'] = $worker_nid;
		++$count;
  }
  if ($count < $batch_size) { 
  	$batch_status['complete'] = TRUE;
  	sirius_minilog("-- All records for $today processed."); 
  }

	if ($mode == 'live') { 
		sirius_domain_variable_set('sirius_ledger_monthly_batch_status', json_encode($batch_status));
	}
	return array('sucecess' => TRUE, 'msg' => "Scanned $count workers.");
}

function sirius_ledger_monthly_charge_worker($worker_nid, $mode, $trigger, $ts = NULL, $action = 'generate', $options = array()) {
	$trigger_options = sirius_ledger_policy_trigger_options('month');
	if (!$ts) { $ts = time(); }

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

	$today_day = date('j', $ts);
	$today_month = date('n', $ts);
	$today_year = date('Y', $ts);
	sirius_minilog("Calculating monthly dues for " . $worker_node->title . " using trigger " .  $trigger_options[$trigger] . " and mode $mode as of $today_year-$today_month-$today_day");


	$work_status_term = taxonomy_term_load($worker_node->field_sirius_work_status['und'][0]['tid']);
	if (!$work_status_term) {
		sirius_minilog('The member has no work status.');
		return;
	}
	if (!$work_status_term->field_sirius_member_active['und'][0]['value'] == 'Yes') {
		sirius_minilog("The member has work status " . $work_status_term->name . '. Dues are not charged to members with this status.');
		return;
	}

	$accounts = sirius_ledger_account_list();
	$schedules = array('month' => 'Monthly', 'semimonth' => 'Semi-Monthly');
	$total_charged = 0;


	if ($options['policy_node']) {
		$policy_node = $options['policy_node'];
		sirius_minilog("Forcing policy " . $policy_node->title . ". This may not be the current policy for this worker.", 'warning');
	} else {
		$policy_node = sirius_ledger_policy_lookup($worker_node);
	}

	if (!$policy_node) {
		sirius_minilog("No ledger policy found for " . $worker_node->title, 'warning');
		return;
	}
	$policy_json = sirius_json_get($policy_node);

	foreach ($schedules as $schedule => $schedule_name) {
		if ($policy_json['ledger']['policy']['monthly']['skip_months'][$today_month]) {
			sirius_minilog("$schedule_name: The ledger policy lists the current month, month #$today_month, as a \"skip month\". Monthly dues are not charged this month.");
			continue;
		}

		$rates = sirius_ledger_policy_lookup_rates_new($worker_node, $policy_node, $schedule, array('ts' => $ts));
		if (!$rates) {
			sirius_minilog("$schedule_name: no rates found.");
			continue; 
		}

		if ($schedule == 'month') { 
			$charge_days = array($policy_json['ledger']['policy']['monthly']['monthly_charge_day']);
		} else {
			$charge_days = array(
				$policy_json['ledger']['policy']['monthly']['semimonthly_charge_day_1'],
				$policy_json['ledger']['policy']['monthly']['semimonthly_charge_day_2']
			);
		}

		foreach ($charge_days as $delta => $charge_day) {
			$next_charge_day = $charge_days[$delta+1];
			if ($schedule == 'semimonth') { $schedule_name = 'Semi-Monthly Charge #' . ($delta+1); }
			else { $schedule_name = 'Monthly charge'; }
			if ($charge_day > $today_day) {
				sirius_minilog("$schedule_name under this policy will not be made until day $charge_day of the month.");
				continue;
			}

			if ($next_charge_day && ($today_day >= $next_charge_day)) {
				sirius_minilog("$schedule_name under this policy will not be made after day $next_charge_day of the month.");
				continue;
			}

			foreach ($rates as $account_nid => $rate_record) {
				if ($account_nid == 'pay') { continue; }
				if ($action == 'cancel') {
					$rate = 0;
				} else {
					$rate = $rate_record['rate'];
				}
				if (!$rate_record['triggers'][$trigger]) {
					sirius_minilog("$schedule_name under this policy cannot be charged via trigger " . $trigger_options[$trigger]);
					continue;
				}

				// Figure out who to charge
				$account_node = node_load($account_nid);
				$account_json = sirius_jsonfield_util_node_json_get($account_node);
				if ($account_json['settings']['payments']['allocate']['employer']['simple']) { $charge_to = 'employer'; } else { $charge_to = 'worker'; }
				sirius_minilog("$schedule_name for " .  $accounts[$account_nid] . ": \$$rate charged to $charge_to");

				// Start building the entry
				$entry = array();
				$entry['ledger_status'] = 'Cleared';
				$entry['ledger_account'] = $account_nid;
				if ($charge_to == 'worker') { 
					$entry['ledger_participant'] = $worker_node->field_sirius_contact['und'][0]['target_id'];
				} else {
					$entry['ledger_participant'] = $worker_node->field_grievance_shop['und'][0]['target_id'];
				}

				if ($schedule == 'month') {
					$entry['ledger_key'] = $worker_node->nid . '|' . date('Y-m', $ts);
				} else {
					$entry['ledger_key'] = $worker_node->nid . '|' . $delta . '|' . date('Y-m', $ts);
				}

				// Charged already?
				$previous_entries = sirius_ledger_ar_fetch($entry);
				if ($previous_entries) {
					$entry = $previous_entries[0];
					if ($rate == 0) {
						sirius_minilog('- Found a previous entry, but the new rate is $0. Deleting.');
						if ($mode == 'live') {
							sirius_ledger_ar_delete($entry);
						} else {
							sirius_minilog("(Test mode, skipping delete)");
						}
						continue;
					} else if ($entry['ledger_amount'] == $rate) {
						sirius_minilog('- Found a previous ledger entry with the same rate. Skipping.');
						continue;
					} else {
						sirius_minilog('- Found a previous ledger entry with a different rate.');
					}
				}

				if ($rate == 0) {
					sirius_minilog('- Rate is $0, skipping.');
					continue;
				}

				$entry['ledger_reference'] = $policy_node->nid;
				$entry['ledger_amount'] = $rate;
				$entry['ledger_ts'] = $ts;

				$memo = $schedule_name;
				$memo .= ' charge ';
				if ($schedule != 'month') { $memo .= '#' . ($delta+1) . ' '; }
				$memo .= "for " . $worker_node->title . ': ';
				$memo .= '$' . number_format($rate, 2);
				$memo .= ' (MS ';
				$memo .= $rate_record['ms_name'];
				if ($rate_record['level_name']) { $memo .= ' ' . $rate_record['level_name']; }
				$memo .= ')';

				$entry['ledger_memo'] = $memo;
				sirius_minilog('- Ledger entry saved for ' . $worker_node->title);
				if ($mode == 'live') { 
					if ($entry['ledger_id']) {
						sirius_ledger_ar_update($entry);
					} else {
						sirius_ledger_ar_insert($entry);
					}
				} else {
					sirius_minilog("(Test mode, skipping insert)");
				}

				$total_charged += $rate;
			}
		}
	}

	return $total_charged;
}

