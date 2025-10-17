<?php

function report_cache_cl_run($domain_nid, $report_type, $paramters) {
	sirius_domain($domain_nid);
	sirius_minilog_echo_active(TRUE);

	$form_state = array();
	$form_state['values'] = array();
	$form_state['values']['report_type'] = $report_type;
	$form_state['values']['nobatch'] = TRUE;
	$form_state['values']['noninteractive'] = TRUE;
	$form_state['values']['parameters'] = $paramters;
	$form_state['values']['report_id'] = $instance['title'] . ' - ' . 'Auto';
	$result = report_cache_form_submit(array(), $form_state);

	print_r($result);
}