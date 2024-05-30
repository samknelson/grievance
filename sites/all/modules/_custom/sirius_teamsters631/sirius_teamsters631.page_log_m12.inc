<?php

function sirius_teamsters631_node_tab_log_m12_access($m12_node) {
	return TRUE;
	return ($m12_node->category == 'teamsters631:m12');
}

function sirius_teamsters631_node_tab_log_m12_form($form, &$form_state, $m12_node) {
	$form_state['m12_nid'] = $m12_node->nid;
	$m12_json = sirius_json_get($m12_node);

	if (!$form_state['submitted']) {
		$form['action'] = array('#type' => 'value', '#value' => 'search');

		$form['names'] = array(
			'#type' => 'textarea',
			'#size' => 6,
			'#title' => t('Past Some Names'),
		);

		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Search'),
		);

		return $form;
	}

	$form['action'] = array('#type' => 'value', '#value' => 'add');

	//
	// Clean up our input names
	//

	$names_input = explode("\n", $form_state['values']['names']);
	$data = array();
	foreach ($names_input as $delta => $name) {
		$name_clean = $name;
		$name_clean = strtolower($name_clean);
		$name_clean = trim($name_clean);
		$name_clean = preg_replace('/\s+/', ' ', $name_clean);
		$name_clean = preg_replace('/[^a-z ]/', '', $name_clean);
		if (!$name_clean) {
			continue;
		}

		$data[$delta]['input'] = $name;
		$data[$delta]['parts'] = explode(' ', $name_clean);
		$data[$delta]['candidates'] = array();
	}

	//
	// Worker search on each name
	//

	foreach ($data as $delta => $record) {
		$domain_nid = sirius_domain_nid();

		$sql_args = array();
		$sql = "select nid, title from node ";
		$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
		$sql .= "where type = 'sirius_worker' ";

		if ($domain_nid) {
			$sql .= "and field_sirius_domain_target_id = :domain_nid ";
			$sql_args[':domain_nid'] = $domain_nid;
		} else {
			$sql .= "and field_sirius_domain_target_id is null ";
		}

		foreach ($record['parts'] as $part_delta => $part) {
			$sql .= "and title like :part_$part_delta ";
			$sql_args[":part_$part_delta"] = '%' . $part . '%';
		}

		$sql .= "order by title limit 10 ";
		$stmt = sirius_sql_query($sql, $sql_args);
		while ($hr = $stmt->fetchAssoc()) {
			$data[$delta]['candidates'][$hr['nid']] = $hr;
		}
	}

	foreach ($data as $delta => $record) {
		foreach ($record['candidates'] as $candidate_delta => $candidate) {
			$search_params = array();
			$search_params['employer_nid'] = $m12_json['teamsters631_m12']['employer_nid'];
			$search_params['worker_nid'] = $candidate['nid'];
			$search_params['statuses'] = array('accepted', 'accepted_secondary');
			$search_params['limit'] = 10;
			$dispatches = sirius_dispatch_search($search_params);
			if (!$dispatches) {
				$msg = '<em>Not dispatched</em>';
			} else {
				$msgs = array();
				foreach ($dispatches as $dispatch_nid => $dispatch_node) {
					$job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);
					$start_date = $dispatch_node->field_sirius_date_start['und'][0]['value'];
					$job_type = sirius_term_title($job_node->field_sirius_dispatch_job_type['und'][0]['tid']);
					$dispatch_status = $dispatch_node->field_sirius_dispatch_status['und'][0]['value'];
					$msgs[] = "$dispatch_status $job_type on $start_date";
				}
				$msg = join('<br />', $msgs);
			}
			$data[$delta]['candidates'][$candidate_delta]['msg'] = $msg;
		}
	}
	//
	// Dispatch search on each candidate
	//

	foreach ($data as $delta => $record) {
		foreach ($record['candidates'] as $worker_nid => $candidate) {
			// Work status
			$worker_node = node_load($worker_nid);
			$data[$delta]['candidates'][$worker_nid]['ws'] = sirius_term_title($worker_node->field_sirius_work_status['und'][0]['tid']);

			// Disaptches
			$search_params = array();
			$search_params['employer_nid'] = $m12_json['teamsters631_m12']['employer_nid'];
			$search_params['worker_nid'] = $worker_nid;
			$search_params['statuses'] = array('accepted', 'accepted_secondary');
			$search_params['limit'] = 10;
			$dispatches = sirius_dispatch_search($search_params);
			if (!$dispatches) {
				$msg = '<em>Not dispatched</em>';
				$elig = '';
			} else {
				$msgs = $eligs = array();

				foreach ($dispatches as $dispatch_nid => $dispatch_node) {
					$job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);
					$start_date = $dispatch_node->field_sirius_date_start['und'][0]['value'];
					if ($start_date) { $start_date = " on " . date('m/d/Y', strtotime($start_date)); }
					$job_type = sirius_term_title($job_node->field_sirius_dispatch_job_type['und'][0]['tid']);
					$dispatch_status = $dispatch_node->field_sirius_dispatch_status['und'][0]['value'];
					$msgs[] = "$dispatch_status $job_type$start_date";

					$result = sirius_dispatch_elig_explain($job_node, $worker_nid, array('ignore_redispatch' => TRUE, 'ignore_dispatch_status' => TRUE));
					if ($result['success']) {
						$eligs[] = 'Eligible';
					} else {
						$eligs[] = '<strong>Ineligible: ' . $result['msg'] . '</strong>';
					}
				}

				$msg = join(' / ', $msgs);
				$elig = join(' / ', array_unique($eligs));
			}
			$data[$delta]['candidates'][$worker_nid]['msg'] = $msg;
			$data[$delta]['candidates'][$worker_nid]['elig'] = $elig;
		}
	}

	//
	// Render the tableselect
	//

	$rows = array();
	$notfound = 0;
	foreach ($data as $delta => $record) {
		if (!$record['candidates']) {
			$row = array();
			$row['input'] = $record['input'];
			$row['candidate'] = '<em>No candidates found</em>';
			$row['#disabled'] = TRUE;
			$row['#attributes'] = array('class' => array(''));
			--$notfound;
			$rows[$notfound] = $row;
		}	else {
			$is_first = TRUE;
			foreach ($record['candidates'] as $worker_nid => $candidate) {
				$row = array();
				if ($is_first) {
					$row['input'] = $record['input'];
					$is_first = FALSE;
				} else {
					$row['input'] = '';
				}
				$row['candidate'] = $candidate['title'];
				$row['ws'] = $candidate['ws'];
				$row['msg'] = $msg;
				$row['elig'] = $elig;
				$row['tools'] = l('View', "node/$worker_nid", array('attributes' => array('class' => array('tool-button'))));
				$rows[$worker_nid] = $row;
			}
		}
	}

	$header = array();
	$header['input'] = t('Input');
	$header['candidate'] = t('Candidate');
	$header['ws'] = t('Work Status');
	$header['msg'] = t('Dispatch');
	$header['elig'] = t('Eligible?');
	$header['tools'] = t('Tools');

	$form['rows'] = array(
		'#title' => t('Candidates'),
		'#type' => 'tableselect',
		'#header' => $header,
		'#options' => $rows,
		'#empty' => t('No input found.'),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Add Selected Workers'),
	);

	return $form;
}

function sirius_teamsters631_node_tab_log_m12_form_submit($form, &$form_state) {
	$action = $form_state['values']['action'];

	if ($action == 'search') {
		$form_state['rebuild'] = TRUE;
		return;
	}

	if ($action == 'add') {
		drupal_set_message("Added...");
	}
}