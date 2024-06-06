<?php

class Sirius_Ledger_Charge_Plugin_Hourly extends Sirius_Ledger_Charge_Plugin {
	public $type = 'hourly';
	public $name = 'Hourly';
	public $description = 'Generate hourly charges from payperiods.';

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

		$payperiod_json = sirius_json_get($context['payperiod_node']);
		$qty = $payperiod_json['totals']['hours']['total'];
		if (!$qty) { $qty	= 0; }

		$history_item = $this->util_history_lookup($context['employer_settings']['rates']['history'], $context['ts']);
		if (!$history_item) {
			sirius_minilog("No rate record found; no charges generated.");
			return array('success' => TRUE); 
		}

		$rate = $history_item['rate'];
		sirius_minilog("Found rate \$" . number_format($rate, 2) . " effective $history_item[date]");

		$amt = $qty * $rate;

	  $memo = "Hourly: $qty hours at $rate";
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
