<?php

function sirius_smf_node_tab_employer_ehrs_access($employer_node) {
	sirius_require('sirius_smf', 'sirius_smf.ehrs.inc');
	$ehrs_monthly = Sirius_Smf_Ehrs_Monthly::getInstance();
	return $ehrs_monthly->access('employer_landing', array('employer_node' => $employer_node));
}

function sirius_smf_node_tab_employer_ehrs_form($form, &$form_state, $employer_node) {
	sirius_require('sirius_smf', 'sirius_smf.ehrs.inc');
	$ehrs_monthly = Sirius_Smf_Ehrs_Monthly::getInstance();
	return $ehrs_monthly->employer_landing_form($form, $form_state, $employer_node);
}

function sirius_smf_node_tab_employer_ehrs_form_submit($form, &$form_state) {
	sirius_require('sirius_smf', 'sirius_smf.ehrs.inc');
	$ehrs_monthly = Sirius_Smf_Ehrs_Monthly::getInstance();
	return $ehrs_monthly->employer_landing_form_submit($form, $form_state);
}