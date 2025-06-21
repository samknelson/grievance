<?php

function sirius_smf_node_tab_ehrs_wizard_access($eup_node) {
	if ($eup_node->field_sirius_category['und'][0]['value'] != 'smf:ehrs') { return FALSE; }
	sirius_require('sirius_smf', 'sirius_smf.ehrs.inc');
	$ehrs_monthly = Sirius_Smf_Ehrs_Monthly::getInstance();
	return $ehrs_monthly->access('eup_view', array('eup_node' => $eup_node));
}

function sirius_smf_node_tab_ehrs_wizard($eup_node) {
	sirius_require('sirius_smf', 'sirius_smf.ehrs.inc');
	$ehrs_monthly = Sirius_Smf_Ehrs_Monthly::getInstance();
	$feed_node = $ehrs_monthly->get_feed_node($eup_node);

	return sirius_node_tab_feed_wizard($feed_node);
}