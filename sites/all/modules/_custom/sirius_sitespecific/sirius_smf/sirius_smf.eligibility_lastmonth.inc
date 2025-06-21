<?php

class Sirius_Smf_Eligibility_Plugin_Lastmonth extends Sirius_Trust_Elgibility_Plugin {
	public $type = 'smf-lastmonth';
	public $name = 'SMF: Preceding Month';
	public $description = 'To be eligible, the subscriber must have had this benefit in one of the preceding two months.';

	public function check($settings, $params) {
		$subscriber_worker_node = $params['subscriber_worker_node'];
		if (!$subscriber_worker_node) { return array('success' => FALSE, 'msg' => 'No subscriber.'); }


		// Avoid time zone crap
		$ts = strtotime(date('Y-m-15', $params['ts']));

		$search_params = array();
		$search_params['worker_nid'] = $subscriber_worker_node->nid;
		$search_params['benefit_nid'] = $params['benefit_node']->nid;
		$search_params['nids_only'] = TRUE;
		for ($count = 1; $count <= 2; ++$count) {
			$ts = strtotime('-1 month', $ts);
			$search_params['ts'] = $ts;
			$wb_nids = sirius_trust_worker_benefits_search($search_params);
			if ($wb_nids) { return array('success' => TRUE, 'msg' => 'The subscriber had the benefit ' . $params['benefit_node']->title . ' in ' . date('m/Y', $ts)); }
		}

		return array('success' => FALSE, 'msg' => 'The subscriber did not have the benefit ' . $params['benefit_node']->title . ' in either of the the two preceding months.');
	}
}
