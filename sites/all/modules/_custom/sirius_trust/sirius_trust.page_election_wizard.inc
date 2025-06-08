<?php

function sirius_trust_node_tab_election_wizard_access($wizard_node) {
  return Sirius_Trust_Access::getInstance()->access('election_wizard_view', array('election_wizard_node' => $wizard_node));
}

function sirius_trust_node_tab_election_wizard_denied($msg) {
	drupal_set_message($msg, 'error');
	return 'Access denied.';
}

function sirius_trust_node_tab_election_wizard($wizard_node) {
	return sirius_node_tab_wizard($wizard_node, 'trust_election');
}

