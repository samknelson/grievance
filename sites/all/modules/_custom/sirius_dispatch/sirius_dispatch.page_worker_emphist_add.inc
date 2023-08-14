<?php

function sirius_dispatch_node_tab_worker_emphist_add_form($form, &$form_state, $worker_node) {
	$form_state['worker_nid'] = $worker_node->nid;

	// Get a list of stub jobs. Use SQL to avoid a very large number of node_loads.
	$industry_lookup = sirius_taxonomy_load('sirius_industry', 'tid', 'name');
	$employer_nodes = node_load_multiple(array_keys(sirius_employer_list()));
	$jobs = array();
	foreach ($employer_nodes as $employer_nid => $employer_node) {
		$industry_tids = sirius_fieldvals($employer_node, 'field_sirius_industry', 'tid');
		foreach ($industry_tids as $industry_tid) {
			$label = $employer_node->title . ': ' . $industry_lookup[$industry_tid];
			$stub_nid = sirius_dispatch_job_stub_fetch($employer_nid, $industry_tid, TRUE);
			if (!$stub_nid) { continue; }
			$jobs[$stub_nid] = $label;
		}
	}

	if (!$jobs) {
		return sirius_form_error("No stub jobs found. You may need to create stub jobs for each employer / industry. Manage stub jobs " . l('here', 'sirius/dispatch/job/stub'));
	}

	$form['#tree'] = TRUE;

	$form['emphist']['job_nid'] = array(
		'#type' => 'select',
		'#title' => 'Employer / Industry',
		'#options' => array(NULL => t('-- Select --')) + $jobs,
		'#required' => TRUE,
	);

	$form['emphist']['type'] = array(
		'#type' => 'select',
		'#title' => 'Type',
		'#options' => array(
			'Primary' => t('Primary'),
			'Secondary' => t('Secondary'),
			'None' => t('None -- Remove this record'),
		),
	);

	$form['emphist']['start'] = array(
    '#title' => t('Start Date'),
    '#type' => 'date_popup',
    '#attributes' => array(
      'autocomplete' => 'off',
    ),
    '#date_format' => 'Y-m-d',
  );

	$form['emphist']['end'] = array(
    '#title' => t('End Date'),
    '#type' => 'date_popup',
    '#attributes' => array(
      'autocomplete' => 'off',
    ),
    '#date_format' => 'Y-m-d',
  );

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_dispatch_node_tab_worker_emphist_add_form_submit($form, &$form_state) {
	$worker_nid = $form_state['worker_nid'];

	$record = $form_state['values']['emphist'];
	$record['worker_nid'] = $worker_nid;

	$result = sirius_dispatch_emphist_save($record);
	if (!$result['success']) {
		drupal_set_message("Failed to add record: " . $result['msg'], 'error');
		$form_state['rebuild'] = TRUE;
	} else {
		drupal_set_message("Record added.");
		drupal_goto("node/$worker_nid/sirius_worker_emphist");
	}
}