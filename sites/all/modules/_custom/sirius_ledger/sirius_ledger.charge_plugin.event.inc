<?php

class Sirius_Ledger_Charge_Plugin_Event extends Sirius_Ledger_Charge_Plugin {
	public $type = 'event';
	public $name = 'Event';
	public $description = 'Generate a charge when a member attends an event';

	public function trigger_enabled($trigger_key) { return ($trigger_key == 'event'); }

	public function charge(&$context, $mode = 'test') {

		if (!$context['event_node']) {
			$context['event_node'] = node_load($context['event_participant_node']->field_sirius_event['und'][0]['target_id']);
		}

		if (!$context['event_type_term']) {
			$context['event_type_term'] = taxonomy_term_load($event_node->field_sirius_event_type['und'][0]['tid']);
		}

		$event_settings = $this->event_settings($context['event_node'], $context['plugin_uuid']);
		$event_type_settings = $this->event_type_settings($context['event_type_term'], $context['plugin_uuid']);

		if (isset($event_settings['rate'])) {
			$amt = $event_settings['rate'];
		} else {
			$amt = $event_type_settings['rate'];
		}

		$ledger_key = $context['event_participant_node']->nid;
		if ($amt >= 0) { $ledger_memo = "Charge for "; } else { $ledger_memo = "Credit for "; }
		$ledger_memo .= $context['event_node']->title;

	  $entry = array();
	  $entry['ledger_reference'] = $context['event_participant_node']->nid;
	  $entry['ledger_key'] = $ledger_key;
	  $entry['ledger_memo'] = $ledger_memo;
	  $entry['ledger_amount'] = $amt;
	  $context['entry'] = $entry;
		return parent::charge($context, $mode);
	}

	public function event_type_settings_form($form, &$form_state, $event_type_term, $plugin_uuid) {
		$form_state['event_type_tid'] = $event_type_term->tid;

		$form['#tree'] = TRUE;

		$event_type_settings = $this->event_type_settings($event_type_term, $plugin_uuid);

		$form_state['event_type_tid'] = $event_type_term->tid;
		$event_type_json = sirius_json_get($event_type_term);

		$form['#tree'] = TRUE;

		$paccess_options = sirius_event_paccess_options($event_type_term);
		$register_as_options = sirius_event_paccess_options($event_type_term, array('exclude_non_participants' => TRUE));
		$form['settings']['register_as'] = array(
			'#title' => t('Apply charge to participants with the following role and status:'),
			'#type' => 'select',
			'#options' => $register_as_options,
		);
		$form['settings']['rate'] = array(
			'#type' => 'textfield',
			'#title' => t('Enter the charge that should be applied:'),
			'#size' => 5,
			'#description' => t('(Enter a positive value for a charge, a negative value for a credit.)'),
		);

		sirius_jsonfield_util_set_defaults($form['settings'], $event_type_settings);

		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
		);

		return $form;
	}

	public function event_settings_form($form, &$form_state, $event_node, $plugin_uuid) {
		$form_state['event_nid'] = $event_node->nid;

		$form['#tree'] = TRUE;

		$event_settings = $this->event_settings($event_node, $plugin_uuid);
		if (!$event_settings) {
			$event_type_term = taxonomy_term_load($event_node->field_sirius_event_type['und'][0]['tid']);
			$event_settings = $this->event_type_settings($event_type_term, $plugin_uuid);
		}

		$event_type_term = taxonomy_term_load($event_node->field_sirius_event_type['und'][0]['tid']);
		$paccess_options = sirius_event_paccess_options($event_type_term);
		$register_as_options = sirius_event_paccess_options($event_type_term, array('exclude_non_participants' => TRUE));
		$form['settings']['register_as'] = array(
			'#title' => t('Apply charge to participants with the following role and status:'),
			'#type' => 'select',
			'#options' => $register_as_options,
		);
		$form['settings']['rate'] = array(
			'#type' => 'textfield',
			'#title' => t('Enter the charge that should be applied:'),
			'#size' => 5,
			'#description' => t('(Enter a positive value for a charge, a negative value for a credit.)'),
		);

		sirius_jsonfield_util_set_defaults($form['settings'], $event_settings);

		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
		);

		return $form;
	}
}