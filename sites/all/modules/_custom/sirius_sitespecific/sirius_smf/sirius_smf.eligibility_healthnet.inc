<?php

class Sirius_Smf_Eligibility_Plugin_Healthnet extends Sirius_Trust_Elgibility_Plugin_Geog {
	public $type = 'smf-healthnet';
	public $name = 'SMF: Start Healthnet';
	public $description = 'In order to meet this requirement, the member must EITHER live outside of the MLK coverage area, OR have had MLK for the last 12 months.';

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
		
		$result2 = parent::check($settings, $params);
		if ($result2['success']) { return $result2; }

		return array('success' => FALSE, 'msg' => $result1['msg'] . ' / ' . $result2['msg']);
	}

	public function check_linked_benefit($settings, $params) {
		// Find out if they've had MLK for the 12 months preceding
		$linked_benefit_nid = $settings['linked_benefit_nid'];
		if (!$linked_benefit_nid) { return array('success' => FALSE, 'msg' => "No linked benefit NID configured."); }

		$linked_benefit_months = $settings['linked_benefit_months'];
		if (!($linked_benefit_months > 0) || !($linked_benefit_months < 100)) {
 			return array('success' => FALSE, 'msg' => "Number of consecutive months must be between 1 and 99");
		}

		$subscriber_node = $params['subscriber_worker_node'];
		if (!$subscriber_node) { return array('success' => FALSE, 'msg' => 'No subscriber specified.'); }

		// Start date. 15th of the month.
		$ts = $params['ts'];
		if (!$ts) { $ts = time(); }
		$ts = strtotime(date('Y-m-15', $ts));

		for ($lookback = 1; $lookback <= $linked_benefit_months; ++$lookback) {
			$ts = strtotime('-1 month', $ts);
			// drupal_set_message("Checking: " . date('Y-m-d', $ts));

			$search_params = array();
			$search_params['subscriber_nid'] = $subscriber_node->nid;
			$search_params['linked_benefit_nid'] = $linked_benefit_nid;
			$search_params['ts'] = $ts;
			$search_params['nids_only'] = TRUE;
			$benefit_nids = sirius_trust_worker_benefits_search($search_params);
			if (!$benefit_nids) {
				$break;
				return array('success' => FALSE, 'msg' => 'The subscriber did not have a benefit of type ' . sirius_node_title($linked_benefit_nid) . ' as of ' . date('m/Y', $ts));
			}
		}

		return array('success' => TRUE, 'msg' => "The subscriber had $linked_benefit_months months of " . sirius_node_title($linked_benefit_nid));
	}
}
