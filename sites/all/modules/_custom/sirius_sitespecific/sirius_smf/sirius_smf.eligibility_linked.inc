<?php

class Sirius_Smf_Eligibility_Plugin_Linked extends Sirius_Trust_Elgibility_Plugin {
	public $type = 'smf-linked';
	public $name = 'SMF: Linked';
	public $description = 'To be eligible, the subscriber must currently have another benefit (as specified).';

	public function form($json) {
		$benefits = sirius_trust_benefit_options();
		if (!$benefits) {
			return sirius_form_error("There are no benefits configured.");
		}
		$form['benefits'] = array(
			'#type' => 'checkboxes',
			'#title' => 'Linked Benefits',
			'#options' => $benefits,
			'#description' => 'If multiple benefits are selected, the member will be eligible if they have *any* of the checked benefits. Make sure that you do not link a benefit to itself!',
		);

		sirius_jsonfield_util_set_defaults($form, $json);

		return $form;
	}

	public function check($settings, $params) {
		if (!$settings['benefits']) {
			return array('success' => FALSE, 'msg' => 'The "Linked" eligibility requirement is enabled, but no linked benefits are configured.');
		}
		$linked_benefit_nids = array_filter(array_values($settings['benefits']));
		if (!$linked_benefit_nids) {
			return array('success' => FALSE, 'msg' => 'The "Linked" eligibility requirement is enabled, but no linked benefits have been selected.');
		}

		$subscriber_worker_node = $params['subscriber_worker_node'];
		if (!$subscriber_worker_node) { return array('success' => FALSE, 'msg' => 'No subscriber.'); }

		// Avoid time zone crap
		$ts = strtotime(date('Y-m-15', $params['ts']));

		foreach ($linked_benefit_nids as $linked_benefit_nid) {
			$search_params = array();
			$search_params['worker_nid'] = $subscriber_worker_node->nid;
			$search_params['benefit_nid'] = $linked_benefit_nid;
			$search_params['nids_only'] = TRUE;
			$search_params['ts'] = $ts;
			$wb_nids = sirius_trust_worker_benefits_search($search_params);
			if ($wb_nids) { return array('success' => TRUE, 'msg' => 'The subscriber had the benefit ' . sirius_node_title($linked_benefit_nid) . ' in ' . date('m/Y', $ts)); }
		}

		return array('success' => FALSE, 'msg' => 'The subscriber did not have any of the linked benefits in ' . date('m/Y', $ts));
	}
}
