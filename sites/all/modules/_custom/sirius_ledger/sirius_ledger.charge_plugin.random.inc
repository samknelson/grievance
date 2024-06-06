<?php

class Sirius_Ledger_Charge_Plugin_Random extends Sirius_Ledger_Charge_Plugin {
	public $type = 'random';
	public $name = 'Random';
	public $description = 'Generate a random charge each time the plugin is called. Useful for testing purposes';

	public function trigger_enabled($trigger_key) { return ($trigger_key == 'manual'); }

	public function entity_settings_form($form, &$form_state, $entity_node, $plugin_uuid) {
		$form = parent::entity_settings_form($form, $form_state, $entity_node, $plugin_uuid);

		$form['settings']['min'] = array(
			'#type' => 'textfield',
			'#title' => 'Minimum Amount',
			'#size' => 4,
			'#default_value' => 10
		);

		$form['settings']['max'] = array(
			'#type' => 'textfield',
			'#title' => 'Minimum Amount',
			'#size' => 4,
			'#default_value' => 99.99,
		);

		$form['charge'] = array(
			'#type' => 'fieldset',
			'#title' => t('Make a Charge'),
			'#collapsible' => TRUE,
			'#description' => t('Use the form below to generate a random charge. Note that settings changes will not take effect until after the form is saved, so do not change settings and make a charge at the same time.'),
		);

		$form['charge']['worker_nid'] = array(
			'#type' => 'textfield',
			'#size' => 10,
			'#title' => 'Worker NID',
		);

		$form['charge']['date'] = array(
			'#type' => 'textfield',
			'#title' => 'Date',
		);

		$form['charge']['key'] = array(
			'#type' => 'textfield',
			'#title' => 'Key',
			'#description' => 'Enter the ledger "key". Multiple charges to the same key will result in adjustments.',
			'#default_value' => "daily|" . date('Y-m-d'),
		);

		$form['charge']['mode'] = array(
			'#type' => 'select',
			'#title' => t('Mode'),
			'#options' => array(
				'test' => t('Test mode - execute all the steps but do not save any charges.'),
				'live' => t('Live mode - create or update A/R entries'),
			),
		);

		$form['charge']['confirm'] = array(
			'#type' => 'checkbox',
			'#title' => 'Yes, please generate a random charge to the specified participant.',
		);

		sirius_jsonfield_util_set_defaults($form['settings'], $this->entity_settings($entity_node, $entity_uuid));

	  $log = sirius_minilog_render_html();
	  if ($log) { $form['results'] = array('#markup' => $log, '#weight' => 50); }

		return $form;
	}

	public function entity_settings_form_submit($form, &$form_state) {
		if ($form_state['values']['charge']['confirm']) {
			$form_state['rebuild'] = TRUE;

			// Build the context
			$context = array();
			$context['worker_node'] = node_load($form_state['values']['charge']['worker_nid']);
			$context['trigger'] = 'manual';
			$context['plugin_uuid'] = $form_state['plugin_uuid'];
			$context['ledger_key'] = $form_state['values']['charge']['key'];
			if ($form_state['values']['charge']['date']) {
				$context['ts'] = strtotime($form_state['values']['charge']['date']);
			}
			$mode = $form_state['values']['charge']['mode'];
			sirius_minilog("Generating random charge...");
			sirius_minilog_indent();
			$result = sirius_ledger_charge_plugin_charge_all($context, $mode);
			sirius_minilog_outdent();

		}

		return parent::entity_settings_form_submit($form, $form_state);
	}

	public function charge(&$context, $mode = 'test') {
		$min = $context['policy_settings']['min'];
		if (!$min) { $min = 10; }
		$max = $context['policy_settings']['max'];
		if (!$max) { $max = 99.99; }

		$amt = round(rand(100*$min, 100*$max) / 100, 2);

		$ledger_key = $context['ledger_key'];
		if (!$ledger_key) { $ledger_key = 'daily|' . date('Y-m-d'); }

	  $entry = array();
	  $entry['ledger_reference'] = $context['policy_node']->nid;
	  $entry['ledger_key'] = $ledger_key;
	  $entry['ledger_memo'] = "Random charge with key $ledger_key";
	  $entry['ledger_amount'] = $amt;

	  $context['entry'] = $entry;
		return parent::charge($context, $mode);
	}

	public function employer_settings_enabled() { return FALSE; }
}
