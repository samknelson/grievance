<?php
function sirius_training_node_tab_evaluation_edit_access($evaluation_node) {
	if ($evaluation_node->field_sirius_category['und'][0]['value'] != 'worker:evaluation') { return FALSE; }

	return TRUE;
}

function sirius_training_node_tab_evaluation_edit($evaluation_node) {
	return drupal_get_form('sirius_training_node_tab_evaluation_edit_form', $evaluation_node);
}

function sirius_training_node_tab_evaluation_edit_form($form, &$form_state, $evaluation_node) {
	$form_state['evaluation_nid'] = $evaluation_node->nid;

	$json = sirius_jsonfield_util_node_json_get($evaluation_node);
	$survey_form = sirius_jsonfield_util_form_get($json, 'evaluation');
	$results = sirius_jsonfield_util_results_get($json, 'evaluation');
	sirius_jsonfield_util_set_defaults($survey_form, $results);

	$form['survey'] = $survey_form;
	$form['#tree'] = TRUE;

	$form['status'] = array(
		'#type' => 'select',
		'#title' => t('Status'),
		'#options' => sirius_training_evaluation_status_options(),
		'#default_value' => $evaluation_node->field_sirius_type['und'][0]['value'],
		'#required' => TRUE,
		'#weight' => -10000,
	);

	$form['!intro'] = array(
		'#markup' => '<div class="sirius_form_intro">' . '<div class="sirius-form">',
		'#weight' => -9999,
	);
	
	$intro = sirius_domain_variable_get('sirius_training_evaluations_form_intro', '');
	if ($intro['value']) {
		$form['intro'] = array(
			'#markup' => $intro['value'],
			'#weight' => -9998,
		);
	}

	$form['!outro'] = array(
		'#markup' => '</div>',
		'#weight' => 9999,
	);

	$form['!outro'] = array(
		'#markup' => '</div>',
		'#weight' => 9999,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
		'#weight' => 10000,
	);

	if ($form_state['results']) {
		$form['results'] = array('#markup' => $form_state['results']);
	}

	return $form;
}

function sirius_training_node_tab_evaluation_edit_form_submit($form, &$form_state) {
	$evaluation_nid = $form_state['evaluation_nid'];
	$evaluation_node = node_load($evaluation_nid);

	$json = sirius_jsonfield_util_node_json_get($evaluation_node);
	$original_results = sirius_jsonfield_util_results_get($json, 'evaluation');
	sirius_jsonfield_util_results_set($json, 'evaluation', $form_state['values']['survey']);
	sirius_jsonfield_util_node_json_set($evaluation_node, $json);
	sirius_jsonfield_log($evaluation_node, 'edit', 'Edited', array('original' => $original_results));
	$evaluation_node->field_sirius_type['und'][0]['value'] = $form_state['values']['status'];
	node_save($evaluation_node);
	drupal_set_message("Evaluation saved.");
}