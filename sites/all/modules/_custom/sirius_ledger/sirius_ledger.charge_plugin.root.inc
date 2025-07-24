<?php

class Sirius_Ledger_Charge_Plugin extends Sirius_Plugin {
	public $type = 'root';
	public $name = 'Root';
	public $description = 'Root charge plugin; must be overridden.';

	public function plugin_class() { return 'ledger_charge'; }

	public function charge(&$context, $mode = 'test') {
		// We expect to be called from a child or client, which will set a partially-constructed entry
		$entry = $context['entry'];
		if (!$entry) { return array('success' => FALSE, 'msg' => 'No entry found.'); }

		// Get the account
		$account_nid = $context['policy_settings']['account'];
		if (!$account_nid) {
			sirius_minilog("No account specified in the policy settings.", 'error');
			return array('success' => TRUE);
		}
		$account_title = sirius_node_title($account_nid);

		// Build up the entry JSON
		if ($context['employer_node'] && !$entry['ledger_json']['employer']) {
		  $entry['ledger_json']['employer']['nid'] = $context['employer_node']->nid;
		  $entry['ledger_json']['employer']['name'] = $context['employer_node']->title;
		}
		if ($context['worker_node'] && !$entry['ledger_json']['worker']) {
		  $entry['ledger_json']['worker']['nid'] = $context['worker_node']->nid;
		  $entry['ledger_json']['worker']['name'] = $context['worker_node']->title;
		  $entry['ledger_json']['worker']['ssn'] = $context['worker_node']->field_sirius_ssn['und'][0]['value'];
		}
		if ($context['contact_node'] && !$entry['ledger_json']['contact']) {
			$entry['ledger_json']['contact']['nid'] = $context['contact_node']->nid;
			$entry['ledger_json']['contact']['name'] = $context['contact_node']->name;
		}
		if (!$entry['ledger_json']['plugin']) {
			$entry['ledger_json']['plugin']['uuid'] = $context['plugin_uuid'];
			$entry['ledger_json']['plugin']['name'] = $context['plugin']['title'];
			$entry['ledger_json']['plugin']['type_id'] = $this->type;
			$entry['ledger_json']['plugin']['type_name'] = $this->name;
		}

		// Set the account
		$account_nid = $context['policy_settings']['account'];
		if (!$account_nid) {
			sirius_minilog("No account specified in the policy settings.", 'error');
			return array('success' => TRUE);
		}
		$account_title = sirius_node_title($account_nid);
		$entry['ledger_account'] = $account_nid;

		// Get the participant
		$participant_type = $context['policy_settings']['participant_type'];
	  if ($participant_type == 'employer') {
	  	if (!$context['employer_node']) {
	  		sirius_minilog("Participant type is employer, but there is no employer specified.", 'error');
	  		return array('success' => TRUE);
	  	}
	  	$entry['ledger_participant'] = $context['employer_node']->nid;
	  } else {
	  	// assume participant_type = 'worker', which means "contact"
	  	if (!$context['contact_node']) {
	  		sirius_minilog("Participant type is worker, but there is no contact specified.", 'error');
	  		return array('success' => TRUE);
	  	}
	  	$entry['ledger_participant'] = $context['contact_node']->nid;
	  }

	  // Set the timestamp
	  $entry['ledger_ts'] = $context['ts'];

		if (!$entry['ledger_participant'] || !$entry['ledger_account'] || !$entry['ledger_key']) {
			sirius_minilog("Cannot write to ledger: missing participant, account, or key.", 'error');
			return;
		}

		$amt = $entry['ledger_amount'];
		sirius_minilog("Writing to ledger: " . number_format($amt, 2) . " (mode=$mode)");

		// Set the ledger status
		if (!$entry['ledger_status']) {
	  	$entry['ledger_status'] = 'Cleared';
	  }

	  // 
		// Check for previous entries
		// 

	  $probe = array();
	  $probe['ledger_participant'] = $entry['ledger_participant'];
	  $probe['ledger_account'] = $entry['ledger_account'];
	  $probe['ledger_key'] = $entry['ledger_key'];

		$previous_entries = sirius_ledger_ar_fetch($probe);
		$previous_total = 0;

		if ($previous_entries) {
			foreach ($previous_entries as $previous_entry) {
				$previous_total += $previous_entry['ledger_amount'];
			}
			$previous_total = round($previous_total, 2);
			$amt = round($amt, 2);
			if ($previous_total != $amt) {
				$diff = $previous_total - $amt;
				sirius_minilog("Found previous total $previous_total. An adjustment will be entered.");
			}
		}

	  // 
		// If the amount has changed, insert a new record
		// 

		if ($amt == $previous_total) {
			sirius_minilog("Previous total was " . number_format($previous_total, 2) . ". The ledger will not be updated.");
			$entry['ledger_amount'] = 0;
		} else {
			if ($previous_total) {
				// Put some tracking into the JSON
				$entry['ledger_json']['adjustment']['prev_id'] = $previous_entry['ledger_id'];
				$entry['ledger_json']['adjustment']['prev_total'] = $previous_total;
				$entry['ledger_json']['adjustment']['new_total'] = $entry['ledger_amount'];
				$entry['ledger_json']['adjustment']['adjustment'] = $entry['ledger_amount'] - $previous_total;
				$entry['ledger_json']['adjustment']['ts'] = $entry['ledger_ts'];

				// Adjustments go in as of the current time.
				$entry['ledger_ts'] = time();

				// Memo that this is an adjustment
				$entry['ledger_memo'] = 'ADJUSTMENT: ' . $entry['ledger_memo'];

				// CHange the amount
				$entry['ledger_amount'] = $entry['ledger_amount'] - $previous_total;
			}

		  if ($mode == 'live') {
				sirius_ledger_ar_insert($entry);
			} else {
				sirius_minilog("(Test mode, skipping insert)");
			}
		}

		$msg = "Wrote to ledger: $account_title / " . date('Y-m-d', $entry['ledger_ts']) . " / billing $participant_type / " . number_format($entry['ledger_amount'], 2) . " total";
		sirius_minilog($msg);

		return array('success' => TRUE, 'entry' => $entry, 'msg' => $msg, 'amt' => $entry['ledger_amount']);
	}

	/**
	 * Override to enable triggers (hourly, monthly, etc.)
	 */

	public function trigger_enabled($trigger_key) {
		return FALSE;
	}

	///////////////////////////////////////
	// Policy settings
	///////////////////////////////////////

	public function entity_settings_form($form, &$form_state, $entity_node, $plugin_uuid) {
		$form = parent::entity_settings_form($form, $form_state, $entity_node, $plugin_uuid);

		$form['settings'] = array();
		$accounts = sirius_ledger_account_list();
		$form['settings']['account'] = array(
			'#type' => 'select',
			'#title' => t('Account'),
			'#options' => array(NULL => t('-- Select --')) + $accounts,
		);

		$form['settings']['participant_type'] = array(
			'#type' => 'select',
			'#title' => t('Participant'),
			'#options' => array(
				NULL => t('-- Select --'),
				'employer' => t('EMPLOYER'),
				'worker' => t('WORKER'),
			),
		);

		sirius_jsonfield_util_set_defaults($form['settings'], $this->entity_settings($entity_node, $plugin_uuid));

		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit'),
			'#weight' => 10,
		);

		return $form;
	}

	///////////////////////////////////////
	// Employer settings
	///////////////////////////////////////

	public function employer_settings_enabled() { return FALSE; }

	public function employer_settings($employer_node, $plugin_uuid) {
		$employer_json = sirius_json_get($employer_node);
		return $employer_json['charge_plugins']['settings'][$plugin_uuid];
	}

	public function employer_settings_set($employer_node, $plugin_uuid, $settings) {
		$employer_json = sirius_json_get($employer_node);
		$employer_json['charge_plugins']['settings'][$plugin_uuid] = $settings;
		sirius_json_set($employer_node, $employer_json);
		node_save($employer_node);
	}

	public function employer_settings_form($form, &$form_state, $employer_node, $policy_node, $plugin_uuid) {
		$form_state['employer_nid'] = $employer_node->nid;

		$form['#tree'] = TRUE;

		$form['settings']['settings_go_here'] = array(
			'#type' => 'textfield',
			'#title' => t('Sample Settings Form'),
		);

		sirius_jsonfield_util_set_defaults($form['settings'], $this->employer_settings($employer_node, $plugin_uuid));

		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
		);

		return $form;
	}

	public function employer_settings_form_validate($form, &$form_state) {
		return;
	}

	public function employer_settings_form_submit($form, &$form_state) {
		$employer_node = node_load($form_state['employer_nid']);
		$plugin_uuid = $form_state['plugin_uuid'];
		$this->employer_settings_set($employer_node, $plugin_uuid, $form_state['values']['settings']);
		drupal_set_message("Settings saved.");
		return;
	}

	///////////////////////////////////////
	// Event Type Settings
	///////////////////////////////////////

	public function event_type_settings_enabled() { return FALSE; }

	public function event_type_settings($event_type_term, $plugin_uuid) {
		$event_type_json = sirius_json_get($event_type_term);
		return $event_type_json['charge_plugins']['settings'][$plugin_uuid];
	}

	public function event_type_settings_set($event_type_term, $plugin_uuid, $settings) {
		$event_type_json = sirius_json_get($event_type_term);
		$event_type_json['charge_plugins']['settings'][$plugin_uuid] = $settings;
		sirius_json_set($event_type_term, $event_type_json);
		taxonomy_term_save($event_type_term);
	}

	public function event_type_settings_form($form, &$form_state, $event_type_term, $plugin_uuid) {
		return sirius_form_error("No settings form defined.");
	}

	public function event_type_settings_form_validate($form, &$form_state) {
		return;
	}

	public function event_type_settings_form_submit($form, &$form_state) {
		$event_type_term = taxonomy_term_load($form_state['event_type_tid']);
		$plugin_uuid = $form_state['plugin_uuid'];
		$this->event_type_settings_set($event_type_term, $plugin_uuid, $form_state['values']['settings']);
		drupal_set_message("Settings saved.");
		return;
	}

	///////////////////////////////////////
	// Event Settings
	///////////////////////////////////////

	public function event_settings_enabled() { return FALSE; }

	public function event_settings($event_node, $plugin_uuid) {
		$event_json = sirius_json_get($event_node);
		return $event_json['charge_plugins']['settings'][$plugin_uuid];
	}

	public function event_settings_set($event_node, $plugin_uuid, $settings) {
		$event_json = sirius_json_get($event_node);
		$event_json['charge_plugins']['settings'][$plugin_uuid] = $settings;
		sirius_json_set($event_node, $event_json);
		node_save($event_node);
	}

	public function event_settings_form($form, &$form_state, $event_node, $plugin_uuid) {
		return sirius_form_error("No settings form defined.");
	}

	public function event_settings_form_validate($form, &$form_state) {
		return;
	}

	public function event_settings_form_submit($form, &$form_state) {
		$event_node = node_load($form_state['event_nid']);
		$plugin_uuid = $form_state['plugin_uuid'];
		$this->event_settings_set($event_node, $plugin_uuid, $form_state['values']['settings']);
		drupal_set_message("Settings saved.");
		return;
	}





	public function util_history_cleanup_form(&$history_items) {
		foreach ($history_items as $delta => $value) {
			if (!$history_items[$delta]['date']) { unset($history_items[$delta]); continue; }
			$history_items[$delta]['ts'] = strtotime($history_items[$delta]['date']);
		}

		usort($history_items, function($a, $b) { return $b['ts'] <=> $a['ts']; });
	}

	public function util_history_lookup($history_items, $ts) {
		foreach ($history_items as $delta => $value) {
			if ($value['ts'] <= $ts) { return $value; }
		}
	}

	public function util_history_form($history_items, $fields) {
		if ($history_items) {
			$count = count($history_items) + 1;
		} else {
			$count = 1;
		}

		$form['intro'] = array('#markup' => '<div id="sirius_item_list_wrapper"><table><tr><th>Rate</th><th>Effective From</th></tr>');

		for ($delta=0; $delta < $count; ++$delta) {
			$form[$delta]['tr'] = array('#markup' => '<tr>');
			$form[$delta]['rate'] = array(
				'#type' => 'textfield',
				'#size' => 5,
				'#prefix' => '<td>',
				'#suffix' => '</td>',
			);
			$form[$delta]['date'] = array(
				'#type' => 'date_popup',
				'#prefix' => '<td>',
				'#suffix' => '</td>',
				'#date_format' => 'Y-m-d',
				'#attributes' => array('autocomplete' => 'off'),
			);
			$form[$delta]['tr2'] = array('#markup' => '</tr>');
		}
		$form['outro'] = array('#markup' => '</table></div>');
		return $form;
	}
}
