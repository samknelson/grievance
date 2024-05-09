<?php

class Sirius_Trust_Elgibility_Plugin_WS extends Sirius_Trust_Elgibility_Plugin {
	public $type = 'ws';
	public $name = 'Work Status';
	public $description = 'Eligibility for this benefit is restricted to only certain work statuses.';

	public function form($json) {
		$ws_options = sirius_taxonomy_load('sirius_work_status', 'tid', 'name');

		$form = array();
		$form['subscriber']['active'] = array(
			'#title' => 'Subscriber Work Status', 
			'#type' => 'select',
			'#options' => array(
				NULL => t('-- Select --'),
				'Yes' => t('Any Active'),
				'No' => t('Any Inactive'),
				'Specify' => t('Specify'),
			),
			'#default_value' => $json['subscriber']['active'],
			'#id' => 'subscriber_active'
		);

		$form['subscriber']['ws'] = array(
			'#title' => 'Work Status', 
			'#type' => 'checkboxes',
			'#options' => $ws_options,
			'#default_value' => sirius_form_array_default_value($json['subscriber']['ws']),
			'#checkall' => TRUE,
			'#states' => array('visible' => array(":input[id=\"subscriber_active\"]" => array('value' => 'Specify'))),
		);

		return $form;
	}

	public function check($settings, $params) {
		$ts = $params['ts'];
		$date = date('Y-m-d', $ts);

		$subscriber_worker_node = $params['subscriber_worker_node'];

		$history_key = 'worker:work_status';
		$ws_tid_asof = sirius_worker_history_asof($params['subscriber_worker_node'], $history_key, $ts);

		$tools = array(
			l('Work Status History', "node/" . $subscriber_worker_node->nid . "/sirius_worker_work_status/history", array('attributes' => array('class' => array('tool-button')))),
		);

		if (!$ws_tid_asof) { 
			return array(
				'success' => FALSE, 
				'msg' => "No history record was found as of $date.",
				'tools' => $tools,
			);
		}
		$ws_term_asof = taxonomy_term_load($ws_tid_asof);

		if (!$ws_term_asof) {
			return array(
				'success' => FALSE, 
				'msg' => "Internal error: no term found for tid $ws_tid_asof",
			);
		}

		if ($settings['subscriber']['active'] == 'Yes') {
			if ($ws_term_asof->field_sirius_member_active['und'][0]['value'] == 'Yes') {
				$success = TRUE; 
				$msg = "This is an active work status.";
			} else {
				$success = FALSE;
				$msg = "This is an inactive work status.";
			}
		} else if ($settings['subscriber']['active'] == 'No') {
			if ($ws_term_asof->field_sirius_member_active['und'][0]['value'] == 'Yes') {
				$success = FALSE; 
				$msg = "This is an active work status.";
			} else {
				$success = TRUE;
				$msg = "This is an inactive work status.";
			}
		} else if ($settings['subscriber']['active'] == 'Specify') {
			if (!$settings['subscriber']['ws']) {
				$success = FALSE;
				$msg = "This plugin is not configured.";
			} else {
				$ws_tids = array_filter(array_values($settings['subscriber']['ws']));
				if (!$ws_tids) {
					$success = FALSE; 
					$msg = "This plugin is not configured.";
				} else if (in_array($ws_tid_asof, $ws_tids)) {
					$success = TRUE;
					$msg = "This is one of the approved work statuses.";
				} else {
					$ws_names_all = sirius_taxonomy_load('sirius_work_status', 'tid', 'name'); 
					$ws_names = array();
					foreach ($ws_tids as $ws_tid) { $ws_names[] = $ws_names_all[$ws_tid]; }
					$success = FALSE;
					$msg = "This is not one of the approved work statuses (" . join(', ', $ws_names) . ").";
				}
			}
		} else {
			$success = FALSE;
			$msg = "This plugin is not configured.";
		}

		return array(
			'success' => $success, 
			'msg' => "Subscriber had work status " . $ws_term_asof->name . " as of $date. $msg",
			'tools' => $tools,
		);
	}
}
