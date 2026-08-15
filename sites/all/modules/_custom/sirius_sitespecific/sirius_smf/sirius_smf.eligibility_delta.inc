<?php

class Sirius_Smf_Eligibility_Plugin_Delta  extends Sirius_Trust_Elgibility_Plugin_Geog {
	public $type = 'smf-delta';
	public $name = 'SMF: Start Delta';
	public $description = 'In order to meet this requirement, the member must either (1) Live outside the service area (X miles driving distance from [ADDRESS]), or (2) Any prior month of Delta coverage (even one), or (3) 24 consecutive months of coverage under any dental benefit';

	public function check($settings, $params) {
		$result1 = $this->check_delta($settings, $params);
		if ($result1['success']) { return $result1; }
		
		// (1) Meet the specified geographic requirements, 
		$result2 = parent::check($settings, $params);
		if ($result2['success']) { return $result2; }

		return array('success' => FALSE, 'msg' => $result1['msg'] . ' / ' . $result2['msg']);
	}


	public function check_delta($settings, $params) {
		$msgs = array();
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

		/*
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
		*/

		// Get the dental benefit type. We're piggybacking off of COBRA.
		$dental_benefit_tid = sirius_taxonomy_lookup_by_anything('sirius_trust_benefit_type', 'DENTAL');
		if (!$dental_benefit_tid) {
			return array('success' => FALSE, 'msg' => 'There must be a benefit type with the ID "DENTAL".');
		}

		// Have we *ever* had delta
		$delta_benefit_ids = array('D');
		$delta_benefit_nids = array();
		foreach ($delta_benefit_ids as $id) {
			$nid = sirius_find_nid_by_id($id, 'sirius_trust_benefit');
			if (!$nid) { return array('success' => FALSE, 'msg' => "Configuration error: there is no benefit with id [$id]."); }
			$delta_benefit_nids[] = $nid;
		}

		$search_params = array();
		$search_params['subscriber_nid'] = $subscriber_node->nid;
		$search_params['benefit_nids'] = $delta_benefit_nids;
		$search_params['nids_only'] = TRUE;
		$probe = sirius_trust_worker_benefits_search($search_params);
		if ($probe) { return array('success' => TRUE, 'msg' => 'The subscriber had a previous instance of Delta coverage.'); }
		$msgs[] = "Never had delta";

		// Start date. 15th of the month.
		$ts = $params['ts'];
		if (!$ts) { $ts = time(); }
		$ts = strtotime(date('Y-m-15', $ts));

		$all24 = TRUE;
		for ($lookback = 1; $lookback <= 24; ++$lookback) {
			$ts = strtotime('-1 month', $ts);
			// drupal_set_message("Checking: " . date('Y-m-d', $ts));

			$search_params = array();
			$search_params['subscriber_nid'] = $subscriber_node->nid;
			$search_params['benefit_type'] = $dental_benefit_tid;
			$search_params['ts'] = $ts;
			$search_params['nids_only'] = TRUE;
			$probe = sirius_trust_worker_benefits_search($search_params);
			if (!$probe) {
				$msgs[] = "Did not have dental as of " . date('Y-m-d', $ts);
				$all24 = FALSE;
				break;
			}
		}

		if ($all24) {
			return array('success' => TRUE, 'msg' => 'The subscriber had dental coverage for the 24 months preceding.');
		}

		return array('success' => FALSE, 'msg' => join(' / ', $msgs));
	}
}
