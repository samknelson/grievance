<?php

function sirius_trust_eligibility_plugin_ms_settings_form($json) {
	$ms_options = sirius_taxonomy_load('sirius_member_status', 'tid', 'name');

	$form = array();
	$form['subscriber']['ms'] = array(
		'#title' => 'Member Status', 
		'#type' => 'checkboxes',
		'#options' => $ms_options,
		'#default_value' => sirius_form_array_default_value($json['subscriber']['ms']),
		'#checkall' => TRUE,
	);

	return $form;
}

function sirius_trust_eligibility_plugin_ms_check($settings, $params) {
	$ts = $params['ts'];
	$date = date('Y-m-d', $ts);

	$subscriber_worker_node = $params['subscriber_worker_node'];

	$history_key = 'worker:member_status';
	$ms_tids_asof = sirius_worker_history_asof($params['subscriber_worker_node'], $history_key, $ts);

	$tools = array(
		l('Member Status History', "node/" . $subscriber_worker_node->nid . "/sirius_worker_industry/history", array('attributes' => array('class' => array('tool-button')))),
	);

	if (!$ms_tids_asof) { 
		return array(
			'success' => FALSE, 
			'msg' => "No history record was found as of $date.",
			'tools' => $tools,
		);
	}

	$ms_names_all = sirius_taxonomy_load('sirius_member_status', 'tid', 'name'); 

	if (!$settings['subscriber']['ms']) {
		$success = FALSE;
		$msg = "This plugin is not configured.";
	} else {
		$ms_tids = array_filter(array_values($settings['subscriber']['ms']));
		if (!$ms_tids) {
			$success = FALSE; 
			$msg = "This plugin is not configured.";
		} else if (array_intersect($ms_tids, $ms_tids_asof)) {
			$success = TRUE;
			$msg = "This is one of the approved member statuses.";
		} else {
			$ms_names = array();
			foreach ($ms_tids as $ms_tid) { $ms_names[] = $ms_names_all[$ms_tid]; }
			$success = FALSE;
			$msg = "This is not one of the approved member statuses [" . join(', ', $ms_names) . "].";
		}
	}

	$ms_names_asof = array();
	foreach ($ms_tids_asof as $ms_tid) { $ms_names_asof[] = $ms_names_all[$ms_tid]; }

	return array(
		'success' => $success, 
		'msg' => "Subscriber had member status [" . join(',', $ms_names_asof) . "] as of $date. $msg",
		'tools' => $tools,
	);
}