<?php

function sirius_dispatch_node_tab_worker_emphist_form($form, &$form_state, $worker_node) {
	$form_state['worker_nid'] = $worker_node->nid;

	$form['#tree'] = TRUE;
	$form['filters'] = array(
		'#type' => 'fieldset',
		'#title' => t('Filters'),
		'#collapsible' => TRUE,
	);
	if (!$form_state['values']['filters']) { $form['filters']['#collapsed'] = TRUE; }

	$form['filters']['types'] = array(
		'#title' => t('Types'),
		'#type' => 'select',
		'#options' => array(
			'Primary' => t("Primary Only"),
			'Secondary' => t("Secondary Only"),
			'Both' => t("Both Primary and Secondary"),
		),
		'#default_value' => 'Both',
	);

	$form['filters']['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	$type_input = $form_state['values']['filters']['types'];
	if ($type_input == 'Both') {
		$types = array('Primary', 'Secondary');
	} else if ($type_input) {
		$types = array($type_input); 
	} else {
		$types = array('Primary', 'Secondary');
	}

	$emphist = sirius_dispatch_emphist_search($worker_node->nid, $types);
	$form['results'] = array('#markup' => sirius_dispatch_emphist_render($emphist));

	return $form;
}

function sirius_dispatch_node_tab_worker_emphist_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
}