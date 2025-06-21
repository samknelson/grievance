<?php

/**
 * We're not doing this any more. If you want to see a worker's buildup, just run the eligibility tester.
 */

function sirius_smf_node_tab_worker_buildup_access($worker_node) {
	return FALSE;
}

function sirius_smf_node_tab_worker_buildup_form($form, &$form_state, $worker_node) {
	$form_state['worker_nid'] = $worker_node->nid;

	$form['#tree'] = TRUE;

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	if ($form_state['submitted']) {
		$log = sirius_minilog_render_html();

		$html = '<hr><h2>' . t('Results') . '</h2>';
		$html .= sirius_minilog_render_html();
		$form['results'] = array('#markup' => $html);
	}

	return $form;
}

function sirius_smf_node_tab_worker_buildup_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;

	$worker_node = node_load($form_state['worker_nid']);

	// Which nodes are we tagging?
	$entity_nodes = array();

	// Get the contact node
	$contact_node = sirius_worker_contact_get($worker_node);
	if (!$contact_node) { 
		drupal_set_message('The worker does not have a connected contact record.', 'error');
		return;
	}

	// Are tagging as of "right now"? This means that we tag the contact node
	if (isset($asof_ts)) {
		$is_historical = TRUE;
	}
	if (!isset($asof_ts)) {
		$is_historical = FALSE;
		$entity_nodes[] = $contact_node;
		$asof_ts = time();
	}

	// Tag the worker and generate a minilog
	$result = SiriusResult::create(sirius_smf_autotag_worker_buildup($worker_node, $entity_nodes, $asof_ts, $mode));
	$result->drupalSetMessage();
}