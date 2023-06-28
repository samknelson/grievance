<?php

function sirius_node_tab_dispatch_job_payrate($job_node) {
	return drupal_get_form('sirius_node_tab_dispatch_job_payrate_form', $job_node);
}

function sirius_node_tab_dispatch_job_payrate_form($form, &$form_state, $job_node) {
	$form_state['job_nid'] = $job_node->nid;

	if (isset($job_node->field_sirius_payrate['und'][0]['value'])) {
		$intro = t('This job currently has a payrate of <strong>@payrate</strong>.', array('@payrate' => number_format($job_node->field_sirius_payrate['und'][0]['value'], 2)));
	} else {
		$intro = t('This job does not have a payrate set, and is using the default payrate for the member status.');
	}

	$form['intro'] = array(
		'#markup' => '<div class="sirius_form_intro">' . $intro . '</div>',
	);

	$form['payrate'] = array(
		'#type' => 'textfield', 
		'#title' => t('New Payrate'),
		'#description' => t('Enter -1 to clear the payrate for the job, and revert to the default.'),
		'#size' => 5,
		'#required' => TRUE,
	);

	$form['payrate'] = array(
		'#type' => 'textfield', 
		'#title' => t('New Payrate'),
		'#description' => t('Enter -1 to clear the payrate for the job, and revert to the default.'),
		'#size' => 5,
		'#required' => TRUE,
	);

	$form['reason'] = array(
		'#type' => 'textarea',
		'#title' => t('Reason'),
		'#required' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit', 
		'#value' => t('Submit'),
	);

	$log = sirius_minilog_render_html();
	if ($log) { $form['results'] = array('#markup' => '<hr>' . $log); }

	return $form;
}

function sirius_node_tab_dispatch_job_payrate_form_validate($form, &$form_state) {
	$payrate = $form_state['values']['payrate'];
	if (!is_numeric($payrate)) {
		form_set_error('payrate', t('Please enter a numeric value.'));
	}
}

function sirius_node_tab_dispatch_job_payrate_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
	$job_nid = $form_state['job_nid'];
	$job_node = node_load($job_nid);
	$payrate = $form_state['values']['payrate'];
	$reason = $form_state['values']['reason'];

	// Set the payrate for the job
	if ($payrate < 0) {
		unset($job_node->field_sirius_payrate['und']);
		$msg = "Cleared the payrate.";
	} else {
		$job_node->field_sirius_payrate['und'][0]['value'] = $payrate;
		$msg = "Set the payrate to $payrate.";
	}
	node_save($job_node);

	// Log
	sirius_log('job:payrate', 'Set', $msg, $reason, '', array($job_nid));

	// Set payrate for all dispatches
	$query = new EntityFieldQuery;
	$query->entityCondition('entity_type', 'node');
	$query->entityCondition('bundle', 'sirius_dispatch');
	$query->fieldCondition('field_sirius_dispatch_job', 'target_id', $job_nid);
	$query->propertyOrderBy('nid');
	$stmt = $query->execute();
	$dispatch_nids = array();
	if ($stmt['node']) { $dispatch_nids = array_keys($stmt['node']); }
	$count = 0;
	$count_changed = 0;
	foreach ($dispatch_nids as $dispatch_nid) {
		$dispatch_node = node_load($dispatch_nid);
		$old_payrate = $dispatch_node->field_sirius_payrate['und'][0]['value'];
		$result = sirius_dispatch_payrate_compute($dispatch_node, TRUE);
		sirius_minilog($result['msg']);

		++$count;
		if ($old_payrate != $dispatch_node->field_sirius_payrate['und'][0]['value']) {
			node_save($dispatch_node);
			++$count_changed;
		}
	}
	drupal_set_message(t('Updated @count dispatches and changed the payrate for @count_changed', array('@count' => $count, '@count_changed' => $count_changed)));
}
