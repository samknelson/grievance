<?php

class Sirius_Ledger_Charge_Plugin_Monthly extends Sirius_Ledger_Charge_Plugin {
	public $type = 'monthly';
	public $name = 'Monthly';
	public $description = 'Generate fixed monthly charges at all participating employers.';

	public function trigger_enabled($trigger_key) { return ($trigger_key == 'monthly'); }

	public function entity_settings_form($form, &$form_state, $entity_node, $plugin_uuid) {
		$form = parent::entity_settings_form($form, $form_state, $entity_node, $plugin_uuid);

		$ws_options = sirius_taxonomy_load('sirius_work_status', 'tid', 'name');
		$form['settings']['ws'] = array(
			'#type' => 'checkboxes',
			'#title' => t('Work Statuses'),
			'#options' => $ws_options,
		);

		$form['settings']['monthly_charge_day'] = array(
			'#type' => 'textfield',
			'#title' => t('Monthly Charge Day'),
			'#size' => 2,
			'#description' => t('On which day of the month should the charge be entered?')
		);
		sirius_jsonfield_util_set_defaults($form['settings'], $this->entity_settings($entity_node, $plugin_uuid));

		return $form;
	}

	public function charge(&$context, $mode = 'test') {
		$day = date('j', $context['ts']);
		$charge_day = $context['policy_settings']['monthly_charge_day'];
		if ($charge_day) {
			if ($charge_day > $day) {
				sirius_minilog("Charges can't be made until day $charge_day of each month. It is only day $day.");
				return array('success' => TRUE);
			}
		}

		$history_item = $this->util_history_lookup($context['employer_settings']['rates']['history'], $context['ts']);
		if (!$history_item) {
			sirius_minilog("No rate record found; no charges generated.");
			return array('success' => TRUE); 
		}

		$amt = $history_item['rate'];
		sirius_minilog("Found rate \$" . number_format($amt, 2) . " effective $history_item[date]");

		$ws_restriction = $context['policy_settings']['ws'];
		if (!$ws_restriction) { $ws_restriction = array(); }
		$ws_restriction = array_filter(array_values($ws_restriction));
		if ($ws_restriction) {
			$ws_asof = sirius_worker_history_asof($context['worker_node'], 'worker:work_status', $context['ts']);
			if (!in_array($ws_asof, $ws_restriction)) {
				sirius_minilog("The worker had the work status " . sirius_term_title($ws_asof) . ", which is not one of the specified statuses. Setting rate = 0."); 
				$amt = 0;
			}
		}

	  $entry = array();
	  $entry['ledger_reference'] = $context['policy_node']->nid;
	  $entry['ledger_key'] = $context['worker_node']->nid . '|' . date('Y-m', $context['ts']);
	  $entry['ledger_memo'] = "Monthly: $amt";
	  $entry['ledger_amount'] = $amt;

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
