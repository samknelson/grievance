<?php

class Sirius_Trust_Cobra extends Sirius_Singleton {
	public function cobra_employer($nid_only = FALSE) {
		$employer_nid = sirius_domain_variable_get('sirius_trust_cobra_employer_nid');
		if ($nid_only) { return $employer_nid; }
		return node_load($employer_nid);
	}

	public function cobra_benefit_type() {
		return sirius_domain_variable_get('sirius_trust_cobra_benefit_type');
	}

	public function cobra_hour_type() {
		return sirius_domain_variable_get('sirius_trust_cobra_hour_type');
	}

	public function worker_can_elect($worker_node) {
		// Component must be enabled
		if (!sirius_domain_component_enabled('trust:cobra')) {
			return array('success' => FALSE, 'msg' => 'The COBRA component is not enabled.');
		}

		// Must have a COBRA employer configured
		$employer_nid = $this->cobra_employer(TRUE);
		if (!$employer_nid) {
			return array('success' => FALSE, 'msg' => 'No COBRA employer has been configured.');
		}

		// Must have a medical benefit type
		$benefit_type_tid = $this->cobra_benefit_type();
		if (!$benefit_type_tid) {
			return array('success' => FALSE, 'msg' => 'No COBRA benefit type has been configured.');
		}

		// Fetch all the worker's medical benefits
		$params = array();
		$params['worker_nid'] = $worker_node->nid;
		$params['benefit_type'] = $benefit_type_tid;
		$benefit_nodes = sirius_trust_worker_benefits_search($params);

		// They must not have a non-COBRA benefit at the moment
		foreach ($benefit_nodes as $benefit_nid => $benefit_node) {
			if ($benefit_node->field_sirius_active['und'][0]['value'] != 'Yes') { continue; }
			if ($benefit_node->field_grievance_shop['und'][0]['target_id'] == $employer_nid) { continue; }
			return array('success' => FALSE, 'msg' => 'The worker cannot elect COBRA because they currently have a medical benefit.');
		}

		// They must have had a non-COBRA benefit in the last 18 months.
		$ts_now = strtotime(date('Y-m-15', time()));
		$ts_18 = strtotime('-18 months', $ts_now);
		$found = FALSE;
		foreach ($benefit_nodes as $benefit_nid => $benefit_node) {
			if ($benefit_node->field_grievance_shop['und'][0]['target_id'] == $employer_nid) { continue; }
			if (!$benefit_node->field_sirius_date_end['und'][0]['value']) { continue; }
			if (strtotime($benefit_node->field_sirius_date_end['und'][0]['value']) < $ts_18) { continue; }
			$found = TRUE;
			$date = $benefit_node->field_sirius_date_end['und'][0]['value'];
		}
		if (!$found) {
			return array('success' => FALSE, 'msg' => 'The worker cannot elect COBRA because they have not had a (non-COBRA) medical benefit in the last 18 months.');
		}

		// We passed all the tests
		return array('success' => TRUE, 'msg' => 'This worker can elect COBRA because their medical benefit ended on ' . date('m/d/Y', strtotime($date)));
	}
}