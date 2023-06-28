<?php

function sirius_dispatch_job_policy($job_node) {
	return sirius_dispatch_job_policy_from_tid($job_node->field_sirius_dispatch_job_type['und'][0]['tid']);
}

function sirius_dispatch_job_policy_from_tid($job_type_tid) {
	$job_type_term = taxonomy_term_load($job_type_tid);
	$job_policy = json_decode($job_type_term->field_sirius_json['und'][0]['value'], TRUE);
	return $job_policy;
}
