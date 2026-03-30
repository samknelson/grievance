<?php

class Sirius_Smf_Charge_Plugin_Interest extends Sirius_Ledger_Charge_Plugin {
	public $type = 'smf_interest';
	public $name = 'SMF Interest';
	public $description = 'Generate interest chargese';

	public function trigger_enabled($trigger_key) { return ($trigger_key == 'monthly'); }

	public function employer_settings_enabled() { return FALSE; }

	public function entity_settings_form($form, &$form_state, $entity_node, $plugin_uuid) {
		$form = parent::entity_settings_form($form, $form_state, $entity_node, $plugin_uuid);

		$benefit_options = sirius_trust_benefit_options(FALSE);
		$form['settings']['benefit'] = array(
			'#type' => 'select',
			'#title' => 'Benefit Placeholder',
			'#options' => array(NULL => t('-- Select --')) + $benefit_options,
		);

		$json = sirius_json_get($entity_node);
		sirius_jsonfield_util_set_defaults($form['settings'], $this->entity_settings($entity_node, $plugin_uuid));

		return $form;
	}

	public function charge(&$context, $mode = 'test') {
		$ts = $context['ts'];
		if (!$ts) { $ts = time(); }

		$charge_ts = strtotime('-3 months', $ts);
		$charge_date = date('Y-m', $charge_ts);
		$charge_ts_min = strtotime(date('Y-m-1 00:00:00', $charge_ts));
		$charge_ts_max = strtotime(date('Y-m-t 23:59:59', $charge_ts));
		sirius_minilog("Assessing interest for $charge_date");

		// (Non-penalty charges from the month + adjustments made in the month) – 
		// (payments received since the end of the reporting month – balance as of the end of the month) = 
		// balance to assess

		$params = array();
		$params['ledger_participant'] = $context['employer_node']->nid;
		$params['ledger_account'] = $context['account_node']->nid;
		$params['ledger_ts_max'] = $charge_ts_max;
		$params['balance'] = TRUE;
		$balance_end_of_month = sirius_ledger_ar_fetch($params);
		sirius_minilog("balance_end_of_month: " . $balance_end_of_month);

		$params = array();
		$params['ledger_participant'] = $context['employer_node']->nid;
		$params['ledger_account'] = $context['account_node']->nid;
		$params['ledger_ts_min'] = $charge_ts_max;
		$params['ledger_amount_max'] = 0;
		$params['balance'] = TRUE;
		$payments_since_end_of_month = sirius_ledger_ar_fetch($params);
		sirius_minilog("payments_since_end_of_month: " . $payments_since_end_of_month);

		$params = array();
		$params['ledger_participant'] = $context['employer_node']->nid;
		$params['ledger_account'] = $context['account_node']->nid;
		$params['ledger_ts_min'] = $charge_ts_min;
		$params['ledger_ts_max'] = $charge_ts_max;
		$entries = sirius_ledger_ar_fetch($params);
		$adjustments_in_month = $noninterest_charges_in_month = 0;
		foreach ($entries as $entry) {
			if (preg_match('/adjustment/i', $entry['ledger_memo'])) {
				$adjustments_in_month += $entry['ledger_amount'];
			} else if ( ($entry['ledger_amount'] > 0) && (!preg_match('/interest/i', $entry['ledger_memo'])) ) {
				$noninterest_charges_in_month += $entry['ledger_amount'];
			}
		}
		sirius_minilog("adjustments_in_month: " . $adjustments_in_month);
		sirius_minilog("noninterest_charges_in_month: " . $noninterest_charges_in_month);

		$total = ($noninterest_charges_in_month + $adjustments_in_month) - ($payments_since_end_of_month - $balance_end_of_month);
		sirius_minilog("Total: $total");
		if ($total <= 0) {
			sirius_minilog("Total is negative. No interest.");
			return;
		}
		$amt = 0.1 * $total;
		sirius_minilog("interest: $amt");


		$entry = array();
		$entry['ledger_participant'] = $context['employer_node']->nid;
		$entry['ledger_reference'] = $context['policy_node']->nid;
		$entry['ledger_key'] = $context['employer_node']->nid . '|' . $context['account_node']->nid . '|' . date('Ymd', $charge_ts);
		$entry['ledger_memo'] = "Interest: $amt";
		$entry['ledger_amount'] = $amt;
		$context['entry'] = $entry;
		return parent::charge($context, $mode);
	}
}
