<?php

function sirius_trust_page_changequeue($status = NULL) {
	return drupal_get_form('sirius_trust_page_changequeue_form', $status);
}

function sirius_trust_page_changequeue_form($form, &$form_state, $status = NULL) {
	$form['#tree'] = TRUE;

	if (!$status) { $status = 'pending'; }
	$form_state['status'] = $status;
	
	$cq = Sirius_Trust_Changequeue::getInstance();

	$form['search'] = array(
		'#type' => 'textfield',
		'#title' => 'Name',
	);

	$form['submit1'] = array(
		'#type' => 'submit',
		'#value' => t('Search'),
		'#id' => 'search',
	);


	$search_params = array();
	$search_params['status'] = $status;
	$search_params['search'] = $form_state['values']['search'];
	$cq_nodes = $cq->search($search_params);
	$form['list'] = $cq->render_list($cq_nodes);


	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_trust_page_changequeue_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;

	$cq = Sirius_Trust_Changequeue::getInstance();
	$status = $form_state['status'];

	$also_saved = array();
	$count = 0;

	if (!$form_state['values']['list']) { $form_state['values']['list'] = array(); }
	
	foreach ($form_state['values']['list'] as $cq_nid => $tmp) {
		$new_status = $tmp['action'];
		$cq_node = node_load($cq_nid);

		$current_status = $cq->get_status($cq_node);

		// Why? Because one of the side effects of setting the status of node A is that you might also set the status of nodes B, C, and D (if they have the same ID).
		// So we skip records whose status might have changed in earlier iterations of the loop.
		if ($current_status != $status) { continue; }
		if ($current_status == $new_status) { continue; }

		$result = $cq->set_status($cq_node, $new_status);

		if (!$result['success']) {
			drupal_set_message($result['msg'], 'warning');
		} else {
			++$count;
		}
	}
	
	if ($count) {
		drupal_set_message("Updated $count change records.");
	}
}