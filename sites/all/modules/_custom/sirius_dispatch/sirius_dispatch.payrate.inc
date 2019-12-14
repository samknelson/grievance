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

	return $form;
}

function sirius_node_tab_dispatch_job_payrate_form_validate($form, &$form_state) {
	$payrate = $form_state['values']['payrate'];
	if (!is_numeric($payrate)) {
		form_set_error('payrate', t('Please enter a numeric value.'));
	}
}

function sirius_node_tab_dispatch_job_payrate_form_submit($form, &$form_state) {
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
		sirius_dispatch_compute_payrate($dispatch_node, TRUE);

		++$count;
		if ($old_payrate != $dispatch_node->field_sirius_payrate['und'][0]['value']) {
			node_save($dispatch_node);
			++$count_changed;
		}
	}
	drupal_set_message($msg);
	drupal_set_message(t('Updated @count dispatches and changed the payrate for @count_changed', array('@count' => $count, '@count_changed' => $count_changed)));
}

function sirius_dispatch_compute_payrate($dispatch_node, $force_recompute = FALSE) {
  // The pay rate should only be computed once.
  if (!$force_recompute) {
  	if (isset($dispatch_node->field_sirius_payrate['und'][0]['value'])) { return; }
  }
 
  $job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);
  if (!$job_node) { return; }

  // Might get the payrate straight from the job, as an override.
  if (isset($job_node->field_sirius_payrate['und'][0])) {
  	$dispatch_node->field_sirius_payrate = $job_node->field_sirius_payrate;
  	return;
  }

  $industry_tid = $job_node->field_sirius_industry['und'][0]['tid'];
  if (!$industry_tid) { return; }

  $employer_node = node_load($job_node->field_grievance_shop['und'][0]['target_id']);
  if (!$employer_node) { return; }

  $worker_node = node_load($dispatch_node->field_sirius_worker['und'][0]['target_id']);
  if (!$worker_node) { return; }

  // Find the member status for the industry of the employer.
  $member_status_tids = sirius_fieldvals($worker_node, 'field_sirius_member_status', 'tid');
  if (!$member_status_tids) { return; }
  $member_status_terms = taxonomy_term_load_multiple($member_status_tids);
  $found_industry = FALSE; 
  foreach ($member_status_terms as $member_status_tid => $member_status_term) {
    if ($member_status_term->field_sirius_industry['und'][0]['tid'] == $industry_tid) { 
      $found_industry = TRUE;
      break;
    }
  }
  if (!$found_industry) { return; }

  // Need to determine if we should pay at the skilled rate.
  $is_skilled = FALSE;

  // If the job explicitly mentions a skill, then it's a skilled job and we pay at the skilled rate.
  if ($job_node->field_sirius_skill['und'][0]['tid']) { $is_skilled = TRUE; }

  // Some employers always pay the skilled rate for workers with specific skills.
  if (!$is_skilled) {
    $employer_skills = sirius_fieldvals($employer_node, 'field_sirius_skill', 'tid');
    $worker_skills = sirius_fieldvals($worker_node, 'field_sirius_skill', 'tid');
    if (array_intersect($employer_skills, $worker_skills)) { $is_skilled = TRUE; }
  }


  if ($is_skilled) {
    $dispatch_node->field_sirius_payrate = $member_status_term->field_sirius_payrate_skilled; 
  } else {
    $dispatch_node->field_sirius_payrate = $member_status_term->field_sirius_payrate; 
  }
}
