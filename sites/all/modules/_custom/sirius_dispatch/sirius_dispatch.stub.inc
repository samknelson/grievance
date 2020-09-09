<?php

function sirius_dispatch_page_job_stub() {
	return drupal_get_form('sirius_dispatch_page_job_stub_form');
}

function sirius_dispatch_page_job_stub_form($form, &$form_state) {
	$employers = sirius_employer_list();

	$form['action'] = array(
		'#title' => 'Action',
		'#type' => 'select',
		'#options' => array(
			NULL => t('-- Select --'),
			'create' => t('Create or update stub jobs'),
			'download' => t('Export the list of stub jobs'),
		),
	);

	$form['employers'] = array(
		'#title' => 'Create or update stub jobs for these employers',
 		'#states' => array(
 			'visible' => array(
 				':input[name="action"]' => array('value' => 'create')
 			),
 		),
 		'#type' => 'checkboxes',
 		'#options' => $employers,
 		'#checkall' => TRUE,
 	);


	$job_types = sirius_taxonomy_load('sirius_dispatch_job_type', 'tid', 'field_sirius_name_display');
	$form['job_type'] = array(
		'#title' => 'Job Type',
 		'#states' => array(
 			'visible' => array(
 				':input[name="action"]' => array('value' => 'create')
 			),
 		),
 		'#type' => 'select',
 		'#options' => $job_types,
 	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

  $log = sirius_minilog_render_html();
  if ($log) { $form['results'] = array('#markup' => '<hr>' . $log); }

	return $form;
}

function sirius_dispatch_page_job_stub_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
	$action = $form_state['values']['action'];
	$job_type_tid = $form_state['values']['job_type_tid'];
	global $user;
	$domain_nid = sirius_domain_nid();
	$industries = sirius_taxonomy_load('sirius_industry', 'tid', 'name');

	$ms_options = array();

	if ($action == 'create') {
		$employer_nids = array_filter(array_values($form_state['values']['employers']));
		$employer_nodes = node_load_multiple($employer_nids);
		foreach ($employer_nodes as $employer_nid => $employer_node) {
			sirius_minilog("Employer " . $employer_node->title . ": Scanning");
			$industry_tids = sirius_fieldvals($employer_node, 'field_sirius_industry', 'tid');
			foreach ($industry_tids as $industry_tid) {
				$stub_node = sirius_dispatch_job_stub_fetch($employer_nid, $industry_tid);
				if ($stub_node) {
					sirius_minilog("Employer " . $employer_node->title . " / " . $industries[$industry_tid] . ": Updating existing stub node");
				} else {
					sirius_minilog("Employer " . $employer_node->title . " / " . $industries[$industry_tid] . ": Creating new stub node");
	        $stub_node = new stdClass();
	        $stub_node->type = 'sirius_dispatch_job';
	        $stub_node->no_recurse = TRUE;
	        $stub_node->language = $user->language;
	        $stub_node->uid = $user->uid;
	        $stub_node->status = 1;
	        $stub_node->promote = 0;
	        $stub_node->comment = 0;
					$stub_node->field_sirius_id2['und'][0]['value'] = 'STUB';
					$stub_node->field_grievance_shop['und'][0]['target_id'] = $employer_nid;
					$stub_node->field_sirius_industry['und'][0]['tid'] = $industry_tid;
					$stub_node->field_sirius_domain['und'][0]['target_id'] = $domain_nid;
				}

				$stub_node->field_sirius_dispatch_job_type['und'][0]['tid'] = $form_state['values']['job_type'];
				if (!isset($ms_options[$industry_tid])) {
					$ms_options[$industry_tid] = sirius_worker_member_status_get_open_statuses($industry_tid, TRUE, $job_type_tid);
				}
				unset($stub_node->field_sirius_member_status);
				foreach ($ms_options[$industry_tid] as $ms_tid => $ignore) {
					$stub_node->field_sirius_member_status['und'][] = array('tid' => $ms_tid);
				}
				$stub_node->field_sirius_dispatch_job_status['und'][0]['value'] = 'Filled';
        node_object_prepare($stub_node);
        $stub_node = node_submit($stub_node);
        node_save($stub_node);
				sirius_minilog("Employer " . $employer_node->title . " / " . $industries[$industry_tid] . ": Stub node is " . l($stub_node->nid, 'node/' . $stub_node->nid));
			}
		}
		drupal_set_message("Stub node creation complete. Check the log below for details.");
	}

	if ($action == 'download') {
    $fp = fopen('php://output', 'w');
    $filename = 'stubs.csv';
    drupal_add_http_header('Content-Type', 'text/csv');
    drupal_add_http_header('Content-Disposition', 'attachment;filename=' . $filename);
    fputcsv($fp, array('Employer', 'Industry', 'Stub NID'));

		$query = new EntityFieldQuery;
		$query->entityCondition('entity_type', 'node');
		$query->entityCondition('bundle', 'sirius_dispatch_job');
		if ($domain_nid) { $query->fieldCondition('field_sirius_domain', 'target_id', $domain_nid); }
		$query->fieldCondition('field_sirius_id2', 'value', 'STUB');
		$query->propertyOrderBy('nid');
		$stmt = $query->execute();
		$stub_nodes = node_load_multiple(array_keys($stmt['node']));
		foreach ($stub_nodes as $stub_nid => $stub_node) {
			$employer_node = node_load($stub_node->field_grievance_shop['und'][0]['target_id']);
			$industry_name = $industries[$stub_node->field_sirius_industry['und'][0]['tid']];
			fputcsv($fp, array($employer_node->title, $industry_name, $stub_node->nid));
		}
		fclose($fp);
		exit();
	}
}

function sirius_dispatch_job_stub_fetch($employer_nid, $industry_tid) {
	$domain_nid = sirius_domain_nid();
	$query = new EntityFieldQuery;
	$query->entityCondition('entity_type', 'node');
	$query->entityCondition('bundle', 'sirius_dispatch_job');
	if ($domain_nid) { $query->fieldCondition('field_sirius_domain', 'target_id', $domain_nid); }
	$query->fieldCondition('field_grievance_shop', 'target_id', $employer_nid);
	$query->fieldCondition('field_sirius_industry', 'tid', $industry_tid);
	$query->fieldCondition('field_sirius_id2', 'value', 'STUB');
	$query->propertyOrderBy('nid');
	$query->range(0,1);
	$stmt = $query->execute();
	if (!$stmt['node']) { return NULL; }
	$stub_nid = array_keys($stmt['node'])[0];
	return node_load($stub_nid);
}