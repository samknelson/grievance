<?php

function sirius_edls_page_worker_outsource() {
	$result = sirius_edls_employer_switch();
	$html = $result['markup'];
	$employer_node = $result['employer_node'];
	if (!$employer_node) {
		$html .= sirius_page_error('Please select an employer.');
		return $html;
	}

	if (!sirius_access_instance('edls')->access('employer_config_outsource_workers', array('employer_nid' => $employer_node->nid))) {
		return sirius_page_error('Access denied.');
	}

	$html .= drupal_render(drupal_get_form('sirius_edls_page_worker_outsource_form', $employer_node));

	return $html;
}


function sirius_edls_page_worker_outsource_form($form, &$form_state, $employer_node) {


	$form['#tree'] = TRUE;
	$form_state['employer_nid'] = $employer_node->nid;

	$workers = sirius_edls_outsource_list($employer_node->nid);
	$workers_encode = sirius_edls_outsource_encode($workers);
	$form['workers'] = array(
		'#type' => 'textarea',
		'#rows' => 20,
		'#title' => t('Outsource Workers'),
		'#description' => t("Enter the outsource workers one per line. Each line should include the ID, the member status, and the name, separated by a pipe, i.e. \"12345|Apprentice|Oswald Outsource\""),
		'#default_value' => $workers_encode,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_edls_page_worker_outsource_form_submit($form, &$form_state) {
	$employer_node = node_load($form_state['employer_nid']);

	$workers_encode = $form_state['values']['workers'];
	$result = sirius_edls_outsource_decode($workers_encode);
	if (!$result['success']) {
		drupal_set_message("Error: " . $result['msg']);
		$form_state['rebuild'] = TRUE;
		return;
	}

	$workers = $result['workers'];

	/*
	for ($i=1; $i<1000; ++$i) {
		$worker_name = ucwords(strtolower(sirius_aat_generate(rand(5,9), 'letter') . ', ' . sirius_aat_generate(rand(5,9), 'letter')));
		$id = $i;
		$workers[$id] = array('worker_name' => $worker_name);
	}
	*/

	$result = sirius_edls_outsource_set_all($employer_node, $workers);
	if (!$result['success']) {
		drupal_set_message("Error: " . $result['msg']);
		$form_state['rebuild'] = TRUE;
		return;
	}

	drupal_set_message("Outsource workers set.");
}