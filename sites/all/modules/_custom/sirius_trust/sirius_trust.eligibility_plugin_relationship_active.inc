<?php

class Sirius_Trust_Elgibility_Plugin_Relationship_Active extends Sirius_Trust_Elgibility_Plugin {
	public $type = 'relationship_active';
	public $name = 'Relationship Active';
	public $description = 'Eligibility for this benefit is restricted to only active relationships.';

	public function subscriber_only() { return FALSE; }

	public function check($settings, $params) {
		$benefit_node = $params['benefit_node'];
		$subscriber_worker_node = $params['subscriber_worker_node'];

		$relationship_node = $params['relationship_node'];
		if (!$relationship_node) { 
			return array(
				'success' => TRUE, 
				'msg' => 'Relationship is [self].'
			);
		}

		$ts = $params['ts'];
		$start_ts = sirius_fieldval($relationship_node, 'field_sirius_date_start', 'ts');
		$end_ts = sirius_fieldval($relationship_node, 'field_sirius_date_end', 'ts');

		if ($start_ts === NULL) {
			$result = array(
				'success' => FALSE, 
				'msg' => $msg . 'There is no start date for the relationship.'
			);
		} else if ($start_ts > $ts) {
			$result = array(
				'success' => FALSE, 
				'msg' => $msg . 'The start date for the relationship is ' . date('m/d/Y', $start_ts) . ', which is after the eligibility date, ' . date('m/d/Y', $ts),
			);
		} else if ($end_ts === NULL) {
			$result = array(
				'success' => TRUE, 
				'msg' => 'The relationship started on ' . date('m/d/Y', $start_ts) . ', before the eligibility date, ' . date('m/d/Y', $ts) . '. There is no end date.',
			);
		} else if ($end_ts < $ts) {
			$result = array(
				'success' => FALSE, 
				'msg' => 'The relationship ended on ' . date('m/d/Y', $end_ts) . ', before the eligibility date, ' . date('m/d/Y', $ts) . '.',
			);
		} else {
			$result = array(
				'success' => TRUE, 
				'msg' => 'The relationship started on ' . date('m/d/Y', $start_ts) . ', before the eligibility date, ' . date('m/d/Y', $ts) . ', and ended on ' . date('m/d/Y', $end_ts) . '.',
			);
		}

		$paths = array(
			"node/" . $relationship_node->nid => 'Relationship Record',
		);
		$tools = array();
		foreach ($paths as $path => $label) {
			if (drupal_valid_path($path)) {
				$tools[] = l($label, $path, array('attributes' => array('class' => array('tool-button'))));
			}
		}
		$result['tools'] = $tools;
		return $result;
	}
}