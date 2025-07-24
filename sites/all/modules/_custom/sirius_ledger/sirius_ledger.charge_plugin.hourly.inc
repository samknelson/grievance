<?php

class Sirius_Ledger_Charge_Plugin_Hourly extends Sirius_Ledger_Charge_Plugin {
	public $type = 'hourly';
	public $name = 'Hourly';
	public $description = 'Generate hourly charges from payperiods.';

	public function entity_settings_form($form, &$form_state, $entity_node, $plugin_uuid) {
		$form = parent::entity_settings_form($form, $form_state, $entity_node, $plugin_uuid);

		$hour_types = sirius_taxonomy_load('sirius_hour_type', 'tid', 'name');
		$form['settings']['hour_type_tids'] = array(
			'#type' => 'checkboxes',
			'#title' => 'Hour Types',
			'#options' => $hour_types,
			'#checkall' => TRUE,
		);

		$json = sirius_json_get($entity_node);
		sirius_jsonfield_util_set_defaults($form['settings'], $this->entity_settings($entity_node, $plugin_uuid));

		return $form;
	}

	public function trigger_enabled($trigger_key) { return ($trigger_key == 'hourly'); }

	public function charge(&$context, $mode = 'test') {
		if (!$context['payperiod_node']) {
			sirius_minilog("No payperiod found.", 'error');
			return array('success' => TRUE);
		}

		// If not final, don't write the cache
		if ($context['payperiod_node']->field_sirius_active['und'][0]['value'] != 'Yes') { 
			sirius_minilog('Payperiod is not active, no entries written.');
			return array('success' => TRUE);
		}

		$hour_type_tids = $context['policy_settings']['hour_type_tids'];
		if (!$hour_type_tids) {
			sirius_minilog("The hour types have not been specified for this charge plugin.", 'warning');
			return array('success' => TRUE); 
		}
		$hour_type_tids = array_filter(array_values($hour_type_tids));

		$payperiod_json = sirius_json_get($context['payperiod_node']);
		$qty = 0;
		foreach ($hour_type_tids as $hour_type_tid) { $qty += $payperiod_json['totals']['hours']['by_type'][$hour_type_tid]; }

		$history_item = $this->util_history_lookup($context['employer_settings']['rates']['history'], $context['ts']);
		if (!$history_item) {
			sirius_minilog("No rate record found; no charges generated.");
			return array('success' => TRUE); 
		}

		$rate = $history_item['rate'];
		$rate_format = "\$" . number_format($rate, 2);

		sirius_minilog("Found rate $rate_format effective $history_item[date]");

		$amt = $qty * $rate;

	  $memo = "Hourly: $qty eligible hours at $rate_format";
	  sirius_minilog("Memo: $memo");

	  $entry = array();
	  $entry['ledger_amount'] = $amt;
	  $entry['ledger_memo'] = $memo;
	  $entry['ledger_reference'] = $context['payperiod_node']->nid;
		$entry['ledger_key'] = $context['payperiod_node']->nid;

	  $context['entry'] = $entry;
		return parent::charge($context, $mode);
	}

	public function employer_settings_enabled() { return TRUE; }

	public function employer_settings_form($form, &$form_state, $employer_node, $policy_node, $plugin_uuid) {
		$form_state['employer_nid'] = $employer_node->nid;

		$form['#tree'] = TRUE;

		$employer_settings = $this->employer_settings($employer_node, $plugin_uuid);

		$form['settings']['rates'] = array(
			'#type' => 'fieldset',
			'#title' => 'Rate History',
		);

		$form['settings']['rates']['history'] = $this->util_history_form($employer_settings['rates']['history'], array());

		sirius_jsonfield_util_set_defaults($form['settings'], $employer_settings);

		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
		);

		return $form;
	}

	public function employer_settings_form_submit($form, &$form_state) {
		$this->util_history_cleanup_form($form_state['values']['settings']['rates']['history']);
		return parent::employer_settings_form_submit($form, $form_state);
	}
}
