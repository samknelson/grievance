<?php

function sirius_dispatch_payrate_set($dispatch_node, $force_recompute = FALSE, $mode = 'test') {
}

function sirius_dispatch_payrate_compute(&$dispatch_node, $force_recompute = FALSE) {
  // The pay rate should only be computed once.
  if (!$force_recompute) {
  	if (isset($dispatch_node->field_sirius_payrate['und'][0]['value'])) { return array('success' => TRUE, 'msg' => 'Payrate already set.'); }
  }
  $job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);
  if (!$job_node) { return array('success' => FALSE, 'msg' => 'No job.'); }

  // Might get the payrate straight from the job, as an override.
  if (isset($job_node->field_sirius_payrate['und'][0])) {
  	$dispatch_node->field_sirius_payrate = $job_node->field_sirius_payrate;
  	$payrate_display = $job_node->field_sirius_payrate['und'][0]['value'];
  	return array('success' => TRUE, 'msg' => "Using the pay rate override specified in the job, $payrate_display");
  }

  $industry_tid = $job_node->field_sirius_industry['und'][0]['tid'];
  if (!$industry_tid) { return array('success' => FALSE, 'msg' => 'There is no industry specified for the job.'); }

  $employer_node = node_load($job_node->field_grievance_shop['und'][0]['target_id']);
  if (!$employer_node) { return array('success' => FALSE, 'msg' => 'There is no employer specified for the job.'); }

  $worker_node = node_load($dispatch_node->field_sirius_worker['und'][0]['target_id']);
  if (!$worker_node) { return array('success' => FALSE, 'msg' => 'Unable to find the worker for this job.'); }

  // Find the member status for the industry of the employer.
  $member_status_tids = sirius_fieldvals($worker_node, 'field_sirius_member_status', 'tid');
  if (!$member_status_tids) { return array('success' => FALSE, 'msg' => 'There are no member statuses for this worker.'); }
  $member_status_terms = taxonomy_term_load_multiple($member_status_tids);
  $found_industry = FALSE; 
  foreach ($member_status_terms as $member_status_tid => $member_status_term) {
    if ($member_status_term->field_sirius_industry['und'][0]['tid'] == $industry_tid) { 
      $found_industry = TRUE;
      break;
    }
  }
  if (!$found_industry) { return array('success' => FALSE, 'msg' => 'The worker does not have a member status associated with the industry for this job.'); }

  $msg = $worker_node->title . ': ';

  // Need to determine if we should pay at the skilled rate.
  $is_skilled = FALSE;

  // If the job explicitly mentions a skill, then it's a skilled job and we pay at the skilled rate.
  if ($job_node->field_sirius_skill['und'][0]['tid']) {
  	$msg .= 'Using skilled rate because the job reuires a skill. ';
  	$is_skilled = TRUE;
  }

  // Some employers always pay the skilled rate for workers with specific skills.
  if (!$is_skilled) {
  	$json = json_decode($employer_node->field_sirius_json['und'][0]['value'], TRUE);
    $employer_skills = $json['dispatch']['pay_at_skilled_rate']['skill'];
    $worker_skills = sirius_fieldvals($worker_node, 'field_sirius_skill', 'tid');
    $employer_member_statuses = $json['dispatch']['pay_at_skilled_rate']['member_status'];
    $worker_member_statuses = sirius_fieldvals($worker_node, 'field_sirius_member_status', 'tid');

  	// Is skilled if we have one of the required skills...
  	if (is_array($employer_skills) && is_array($worker_skills)) {
    	if (array_intersect($employer_skills, $worker_skills)) { 
    		$is_skilled = TRUE;
    		$msg .= 'Using skilled rate because the worker has one of the "global" skills for this employer. ';
    	}
    }

    // ... But NOT if we don't have one of the reuired member statuses
    if ($is_skilled && $employer_member_statuses) {
    	if (!array_intersect($employer_member_statuses, $worker_member_statuses)) { 
    		$is_skilled = FALSE;
    		$msg .= '(But using unskilled rate because the user does not have one of the appropriate member statuses.) ';
    	}
    }
  }
  if (!$is_skilled) { $msg .= "Using unskilled payrate. "; }

  $schedule_key = 'hour';
	$policy_node = sirius_ledger_policy_lookup($worker_node);
	$policy_json = sirius_jsonfield_util_node_json_get($policy_node);
	$rates = sirius_ledger_policy_lookup_rates($worker_node, $policy_node, $schedule_key, $member_status_term->tid);

	if ($policy_node) {
		$payrate_unskilled = $rates['pay']['rate'];
		$payrate_skilled = $rates['pay']['rate_skilled'];
		if (!$payrate_skilled) { $payrate_skilled = $payrate_unskilled; }
		$msg .= "Payrates set in the policy are $payrate_unskilled / $payrate_skilled. ";
	} else {
	  //
	  // @todo: All of the following code ought to become obsolete.
	  //        In the bad old days, we deduced the pay rate from values assocated with member statuses and levels. That logic is
	  //        still in palce for Teamsters 631, which is why this code cannot be removed. However, going forward, everybody 
	  //        ought to be setting the pay rate in the policy, rather than in the member status.
	  // 


	  $ms_json = json_decode($member_status_term->field_sirius_json['und'][0]['value'], TRUE);

	  $payrate_skilled = $member_status_term->field_sirius_payrate_skilled['und'][0]['value'];
	  $payrate_unskilled = $member_status_term->field_sirius_payrate['und'][0]['value'];
	  $msg .= "Payrates for the member status are $payrate_unskilled / $payrate_skilled. ";

	  if ($ms_json['training']['levels_enabled']) {
	  	$msg .= "The member status is leveled, so looking at the worker's level ... ";
	  	$worker_json = json_decode($worker_node->field_sirius_json['und'][0]['value'], TRUE);
	  	$worker_level_code = $worker_json['training']['levels'][$member_status_term->tid]['code'];
	  	if ($worker_level_code) { $msg .= "found $worker_level_code. "; }
	  	if ($worker_level_code && $ms_json['training']['levels'][$worker_level_code]) {
	  		if ($ms_json['training']['levels'][$worker_level_code]['payrate_unskilled']) { $payrate_unskilled = $ms_json['training']['levels'][$worker_level_code]['payrate_unskilled']; }
	  		if ($ms_json['training']['levels'][$worker_level_code]['payrate_skilled']) { $payrate_skilled = $ms_json['training']['levels'][$worker_level_code]['payrate_skilled']; }
				$msg .= "Payrates for level $worker_level_code are $payrate_unskilled / $payrate_skilled. ";
	  	}
	  }
	}

  if ($is_skilled) { $payrate = $payrate_skilled; } else { $payrate = $payrate_unskilled; }
  $msg .= "Final payrate is $payrate.";

  $dispatch_node->field_sirius_payrate['und'][0]['value'] = $payrate;
  return array('success' => TRUE, 'msg' => $msg);
}
