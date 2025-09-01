<?php

class Sirius_Ledger_Charge_Plugin_Monthly_Benefit extends Sirius_Ledger_Charge_Plugin_Monthly {
	public $type = 'monthly_benefit';
	public $name = 'Monthly Benefit';
	public $description = 'Generate fixed monthly charges only for workers receiving a specific benefit.';


	public function entity_settings_form($form, &$form_state, $entity_node, $plugin_uuid) {
		$form = parent::entity_settings_form($form, $form_state, $entity_node, $plugin_uuid);

		$benefit_options = sirius_trust_benefit_options(FALSE);
		$form['settings']['benefit'] = array(
			'#type' => 'select',
			'#title' => 'Benefit',
			'#options' => array(NULL => t('-- Select --')) + $benefit_options,
		);

		$json = sirius_json_get($entity_node);
		sirius_jsonfield_util_set_defaults($form['settings'], $this->entity_settings($entity_node, $plugin_uuid));

		return $form;
	}

	public function charge(&$context, $mode = 'test') {
		$benefit_nid = $context['policy_settings']['benefit'];
		if (!$benefit_nid) {
			sirius_minilog("No benefit specified.", 'error');
			return array('success' => TRUE);
		}

		$params = array();
		$params['benefit_nid'] = $benefit_nid;
		$params['worker_nid'] = $context['worker_node']->nid;
		$params['ts'] = $context['ts'];
		$params['nids_only'] = TRUE;
		$probe = sirius_trust_worker_benefits_search($params);
		if (!$probe) {
			sirius_minilog("The worker did not have [" . sirius_node_title($benefit_nid) . "] at the required time. No charges will be generated.");
			return array('success' => TRUE);
		}

		return parent::charge($context, $mode);
	}
}
