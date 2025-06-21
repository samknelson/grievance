<?php

function sirius_ledger_event_participant_set($event_participant_node, $mode, $options = array()) {
	if (!sirius_domain_component_enabled('ledger:charge:event')) { return; }
	sirius_minilog("Participant " . l($event_participant_node->title, 'node/' . $event_participant_node->nid) . ' (' . $event_participant_node->nid . '): ' . "Calculating ledger (mode = $mode)");

	// Build the context
	$context = array();

	$context['policy_node'] = sirius_ledger_policy_lookup(NULL);
	if (!$context['policy_node']) { sirius_minilog("No default policy node set.", 'warning'); return; }

	$context['event_participant_node'] = $event_participant_node;
	$context['trigger'] = 'event';

	$contact_nid = $event_participant_node->field_sirius_contact['und'][0]['target_id'];
	if (!$contact_nid) { sirius_minilog("No contact found for the participant record.", 'warning'); return; }
	$context['contact_node'] = node_load($contact_nid);

	$worker_node = sirius_worker_get_by_contact_nid($contact_nid);
	if (!$worker_node) { sirius_minilog("No worker found for this contact.", 'warning'); return; }
	$context['worker_node'] = $worker_node;

	sirius_minilog_indent();
	$result = sirius_ledger_charge_plugin_charge_all($context, $mode);
	sirius_minilog_outdent();

	return $result;
}
