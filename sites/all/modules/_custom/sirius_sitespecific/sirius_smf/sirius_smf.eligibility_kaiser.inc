<?php

class Sirius_Smf_Eligibility_Plugin_Kaiser extends Sirius_Trust_Elgibility_Plugin {
	public $type = 'smf-kaiser';
	public $name = 'SMF: Start Kaiser';
	public $description = 'In order to meet this requirement, the member must have had any medical benefit for all of the preceding 24 months, OR be in the immediate enrollment period for their employer.';

	public function check($settings, $params) {
		$subscriber_node = $params['subscriber_worker_node'];
		if (!$subscriber_node) { return array('success' => FALSE, 'msg' => 'No subscriber specified.'); }

		// Find the home employer. @todo this should be abstracted out
		if (!$params['employer_node']) {
			$search_params = array();
			$search_params['worker_nid'] = $subscriber_node->nid;
			$search_params['ts'] = $params['ts'];
			$election_nodes = sirius_trust_worker_elections_search($search_params);
			$election_node = reset($election_nodes);
			if ($election_node) {
				$params['employer_node'] = node_load($election_node->field_grievance_shop['und'][0]['target_id']);
			}
		}

		if ($params['employer_node']) {
			$employer_json = sirius_json_get($params['employer_node']);
			$start_ts = strtotime($employer_json['smf']['kaiser']['immediate_start_date']);
			$end_ts = strtotime($employer_json['smf']['kaiser']['immediate_end_date']);

			// Avoid time zone crap
			$ts = strtotime(date('Y-m-15', $params['ts']));

			if ($start_ts && $end_ts) {
				if ( ($ts >= $start_ts) && ($ts <= $end_ts) ) {
					return array('success' => TRUE, 'msg' => 'Subcriber is eligible because the employer, ' . $params['employer_node']->title . ', had immediate eligibility between ' . date('Y-m-d', $start_ts) . ' and ' . date('Y-m-d', $end_ts));
				}
			}
		}

		// Are we in an "immediate eligibility" period?

		// Get the medical benefit type. We're piggybacking off of COBRA.
		$medical_benefit_type = taxonomy_term_load(Sirius_Trust_Cobra::getInstance()->cobra_benefit_type());
		if (!$medical_benefit_type) {
			return array('success' => FALSE, 'msg' => 'Don\'t know which benefit type is "medical". This is configured on the COBRA configuration page.');
		}

		// Start date. 15th of the month.
		$ts = $params['ts'];
		if (!$ts) { $ts = time(); }
		$ts = strtotime(date('Y-m-15', $ts));

		for ($lookback = 1; $lookback <= 24; ++$lookback) {
			$ts = strtotime('-1 month', $ts);
			// drupal_set_message("Checking: " . date('Y-m-d', $ts));

			$search_params = array();
			$search_params['subscriber_nid'] = $subscriber_node->nid;
			$search_params['benefit_type'] = $medical_benefit_type->tid;
			$search_params['ts'] = $ts;
			$search_params['nids_only'] = TRUE;
			$benefit_nids = sirius_trust_worker_benefits_search($search_params);
			if (!$benefit_nids) {
				return array('success' => FALSE, 'msg' => 'The subscriber did not have a benefit of type ' . $medical_benefit_type->name . ' as of ' . date('m/Y', $ts));
			}
		}

		return array('success' => TRUE, 'msg' => 'The subscriber met the eligibility requirements for starting Kaiser (24 consecutive months of medical coverage).');
	}
}
