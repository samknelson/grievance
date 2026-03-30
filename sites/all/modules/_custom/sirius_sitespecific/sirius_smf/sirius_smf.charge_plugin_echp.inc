<?php

sirius_require('sirius_smf', 'sirius_smf.echp.inc');

class Sirius_Smf_Charge_Plugin_Echp extends Sirius_Ledger_Charge_Plugin {
	public $type = 'smf-echp';
	public $name = 'SMF Event Center Hours Purchase';
	public $description = 'Generate charges for hours purchased via the ECHP service.';

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
	public function employer_settings_enabled() { return FALSE; }

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
		foreach ($hour_type_tids as $hour_type_tid) { $qty += $payperiod_json['totals']['hours']['by_type'][$hour_type_tid]; }

		$echp = Sirius_Smf_Echp::getInstance();
	  $echp->build_context($context);
	  $threshold = $context['hrs_threshold'];
	  if ($qty <= 0) {
	  	$amt = 0;
	  	$memo = "No hours purchased.";
	  } else {
	  	$imputed_hrs_worked = $threshold - $qty;
			$amt = $echp->price($imputed_hrs_worked);
			$amt_format = '$' . number_format($amt, 2);
		  $memo = "Hourly: $qty hours purchased for $amt_format (threshold $threshold)";
		}

		sirius_minilog("Memo: $memo");

		$context['ts'] = sirius_fieldval($context['payperiod_node'], 'field_sirius_date_end', 'ts');

	  $entry = array();
	  $entry['ledger_amount'] = $amt;
	  $entry['ledger_memo'] = $memo;
	  $entry['ledger_reference'] = $context['payperiod_node']->nid;
		$entry['ledger_key'] = $context['payperiod_node']->nid;

	  $context['entry'] = $entry;
		return parent::charge($context, $mode);
	}
}

