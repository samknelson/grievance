<?php

function sirius_worker_node_tab_schedule_tor_add_access($worker_node) {
	// @todo: Should do this:
	//        $tor_type_tid = sirius_node_tab_arg(0);
	//        But calling sirius_node_tab_arg() in an _access function results in an infinite loop.

	return TRUE;
}

function sirius_worker_node_tab_schedule_tor_add($worker_node) {
	$tor_type_tid = sirius_node_tab_arg(0);
	$tor_type_lookup = sirius_taxonomy_load('sirius_worker_tor_type', 'tid', 'name');

	if ($tor_type_tid) {
		if (!sirius_worker_schedule_tor_access('add', $worker_node, NULL, $tor_type_tid)) {
			drupal_set_message("You do not have access to time-off request type $tor_type_tid.", 'error');
			return '';
		}
		return drupal_get_form('sirius_worker_schedule_tor_addedit_form', $worker_node, NULL, $tor_type_tid);
	} else {
		return drupal_get_form('sirius_worker_node_tab_schedule_tor_add_type_select_form', $worker_node);
	}
}

function sirius_worker_node_tab_schedule_tor_add_type_select_form($form, &$form_state, $worker_node) {
	$form_state['worker_nid'] = $worker_node->nid;

	// Filter the lookup to include only the types we can add.
	$tor_type_lookup = sirius_taxonomy_load('sirius_worker_tor_type', 'tid', 'name');
	foreach ($tor_type_lookup as $tor_type_tid => $tor_type_name) {
		if (!sirius_worker_schedule_tor_access('add', $worker_node, NULL, $tor_type_tid)) {
			unset($tor_type_lookup[$tor_type_tid]);
		}
	}

	// If we can only add one type, shortcut and just go there now.
	if (count($tor_type_lookup) < 1) {
		drupal_set_message("You do not have access to create any time-off requests.", 'error');
		return array();
	}
	if (count($tor_type_lookup) == 1) {
		$tor_type_tid = reset(array_keys($tor_type_lookup));
		drupal_goto("node/" . $worker_node->nid . "/sirius_worker_schedule/tor/add/$tor_type_tid");
	}

	$form['tor_type_tid'] = array(
		'#type' => 'select',
		'#title' => t('Time-Off Type'),
		'#options' => array(NULL => t('-- Select --')) + $tor_type_lookup,
		'#required' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_worker_node_tab_schedule_tor_add_type_select_form_submit($form, &$form_state) {
	$worker_nid = $form_state['worker_nid'];
	$tor_type_tid = $form_state['values']['tor_type_tid'];
	drupal_goto("node/$worker_nid/sirius_worker_schedule/tor/add/$tor_type_tid");
}
