<?php

function sirius_node_tab_dispatch_job_dispatches_access($job_node) {
	$access = Sirius_Dispatch_Access::getInstance();
	// return $access->access('dispatch_list', array('job_node' => $job_node));

	// Staff can view any job
	if (user_access('sirius dispatch staff')) { return TRUE; }

	// Employer can view only their own shop's jobs.
	global $user;
	$user_full = user_load($user->uid);
	if (user_access('sirius dispatch employer')) {
		if ($job_node->field_grievance_shop['und'][0]['target_id'] && 
			($user_full->field_grievance_shops['und'][0]['target_id'] == $job_node->field_grievance_shop['und'][0]['target_id'])) {
			return TRUE;
		}
	}

	// Stewards can manage the dispatches for a job
	if (sirius_rename_this_is_steward($job_node)) { return TRUE; }

	// Oh well.
	return FALSE;
}

function sirius_node_tab_dispatch_job_dispatches_form($form, &$form_state, $job_node) {
	$form_state['job_nid'] = $job_node->nid;
	$form['#tree'] = TRUE;

	$form['counts'] = array('#markup' => sirius_dispatch_job_counts_render($job_node->nid));

	$dispatch_nodes = sirius_dispatch_search(array('job_nid' => $job_node->nid));

	$header = array();
	$header['worker_name'] = t('Worker');
	$header['dispatch_status'] = t('Status');
	$header['dispatch_start'] = t('Start');
	$header['dispatch_end'] = t('End');
	$header['tools'] = t('Tools');

	$rows = array();
	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/table.css');
	foreach ($dispatch_nodes as $dispatch_nid => $dispatch_node) {
		$status = $dispatch_node->field_sirius_dispatch_status['und'][0]['value'];
		$row['worker_name'] = sirius_node_title($dispatch_node->field_sirius_worker['und'][0]['target_id']);
		$row['dispatch_status'] = array(
			'data' => sirius_dispatch_status_title($status),
			'class' => array(sirius_dispatch_status_color($status)),
		);
		$rows[] = $row;
	}
	$form['dispatch_nids'] = array(
		'#type' => 'tableselect',
		'#options' => $rows,
		'#header' => $header,
		'#empty' => t('No dispatches found.'),
		'#attributes' => array('class' => array('sirius_table')),
	);

	if (user_access('sirius dispatch staff')) {
		$html .= views_embed_view('sirius_dispatches', 'block', $job_node->nid);
	} else if (sirius_rename_this_is_steward($job_node)) {
		$html .= views_embed_view('sirius_dispatches', 'block_2', $job_node->nid);
	} else {
		$html .= views_embed_view('sirius_dispatches', 'block_1', $job_node->nid);
	}

	$form['whatever'] = array('#markup' => $html);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_rename_this_is_steward($job_node) {
	$event_nid = $job_node->field_sirius_events['und'][0]['target_id'];
	if ($event_nid) {
		$event_node = node_load($event_nid);
		$event_type_term = taxonomy_term_load($event_node->field_sirius_event_type['und'][0]['tid']);
		$json = json_decode($event_type_term->field_sirius_json['und'][0]['value'], TRUE);
		$rules = $json['bullpen']['manage']['rules'];
		if ($rules) {
			$match = sirius_event_paccess_rule_match($event_nid, $rules);
			if ($match) { return TRUE; }
		}
	}
	return FALSE;
}