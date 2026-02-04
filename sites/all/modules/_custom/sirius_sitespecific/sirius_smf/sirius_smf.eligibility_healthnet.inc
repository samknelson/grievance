<?php

class Sirius_Smf_Eligibility_Plugin_Healthnet extends Sirius_Trust_Elgibility_Plugin_Geog {
	public $type = 'smf-healthnet';
	public $name = 'SMF: Start Healthnet';
	public $description = 'In order to meet this requirement, the member must meet ONE of the erquirements: ' .
		'(1) Meet the specified geographic requirements, ' . 
		'(2) Have EVER had HealthNet coverage, ' . 
		'(3) Have had the specified benefit without break for the specified number of months, ' . 
		'(4) The employer is in an immediate eligibility period.';

	public function form($json) {
		$form = parent::form($json);
		$form['linked_benefit_nid'] = array(
			'#type' => 'select',
			'#title' => 'Or you must have this benefit ...',
			'#options' => array(NULL => t('-- Select --')) + sirius_trust_benefit_options(TRUE),
			'#description' => 'If the geography requirement is not met, then select the other benefit which grants eligibility after '
		);
		$form['linked_benefit_months'] = array(
			'#type' => 'textfield',
			'#title' => '... For this many months',
			'#size' => 4,
		);

		sirius_jsonfield_util_set_defaults($form, $json);

		return $form;
	}

	public function check($settings, $params) {
		$result1 = $this->check_linked_benefit($settings, $params);
		if ($result1['success']) { return $result1; }
		
		// (1) Meet the specified geographic requirements, 
		$result2 = parent::check($settings, $params);
		if ($result2['success']) { return $result2; }

		return array('success' => FALSE, 'msg' => $result1['msg'] . ' / ' . $result2['msg']);
	}

	public function check_linked_benefit($settings, $params) {
		// Avoid time zone crap
		$ts = $params['ts'];
		if (!$ts) { $ts = time(); }
		$ts = strtotime(date('Y-m-15', $ts));

		// (4) The employer is in an immediate eligibility period.
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


			if ($start_ts && $end_ts) {
				if ( ($ts >= $start_ts) && ($ts <= $end_ts) ) {
					return array('success' => TRUE, 'msg' => 'Subcriber is eligible because the employer, ' . $params['employer_node']->title . ', had immediate eligibility between ' . date('Y-m-d', $start_ts) . ' and ' . date('Y-m-d', $end_ts));
				}
			}
		}

		$subscriber_node = $params['subscriber_worker_node'];
		if (!$subscriber_node) { return array('success' => FALSE, 'msg' => 'No subscriber specified.'); }

		// (2) Have EVER had HealthNet coverage, 
		$healthnet_benefit_id = 'H';
		$healthnet_benefit_nid = sirius_find_nid_by_id($healthnet_benefit_id, 'sirius_trust_benefit');
		if (!$healthnet_benefit_nid) { return array('success' => FALSE, 'msg' => "Configuration error: there is no benefit with id [$healthnet_benefit_id]."); }
		$search_params = array();
		$search_params['subscriber_nid'] = $subscriber_node->nid;
		$search_params['benefit_nid'] = $healthnet_benefit_nid;
		$search_params['nids_only'] = TRUE;
		$probe = sirius_trust_worker_benefits_search($search_params);
		if ($probe) { return array('success' => TRUE, 'msg' => 'The subscriber met the eligibility requirements for starting Health Net (had a previous instance of Health Net coverage).'); }

		// (3) Have had the specified benefit without break for the specified number of months, 
		$linked_benefit_nid = $settings['linked_benefit_nid'];
		if (!$linked_benefit_nid) { return array('success' => FALSE, 'msg' => "No linked benefit NID configured."); }

		$linked_benefit_months = $settings['linked_benefit_months'];
		if (!($linked_benefit_months > 0) || !($linked_benefit_months < 100)) {
 			return array('success' => FALSE, 'msg' => "Number of consecutive months must be between 1 and 99");
		}

		for ($lookback = 1; $lookback <= $linked_benefit_months; ++$lookback) {
			$ts = strtotime('-1 month', $ts);
			// drupal_set_message("Checking: " . date('Y-m-d', $ts));

			$search_params = array();
			$search_params['subscriber_nid'] = $subscriber_node->nid;
			$search_params['linked_benefit_nid'] = $linked_benefit_nid;
			$search_params['ts'] = $ts;
			$search_params['nids_only'] = TRUE;
			$probe = sirius_trust_worker_benefits_search($search_params);
			if (!$probe) {
				return array('success' => FALSE, 'msg' => 'The subscriber never had Health Net coverage, and did not have a benefit of type ' . sirius_node_title($linked_benefit_nid) . ' as of ' . date('m/Y', $ts));
			}
		}

		return array('success' => TRUE, 'msg' => "The subscriber had $linked_benefit_months months of " . sirius_node_title($linked_benefit_nid));
	}
}
