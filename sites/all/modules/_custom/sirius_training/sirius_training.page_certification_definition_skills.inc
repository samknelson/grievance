<?php

function sirius_training_node_tab_certification_definition_skills_access($certification_definition_node) {
	if ($certification_definition_node->field_sirius_type['und'][0]['value'] != 'sirius_training:certification') { return FALSE; }

	return TRUE;
}

function sirius_training_node_tab_certification_definition_skills($certification_definition_node) {
	return drupal_get_form('sirius_training_node_tab_certification_definition_skills_form', $certification_definition_node);
}

function sirius_training_node_tab_certification_definition_skills_form($form, &$form_state, $certification_definition_node) {
	$skill_options = sirius_taxonomy_load('sirius_skill', 'tid', 'name');

	$form_state['certification_definition_nid'] = $certification_definition_node->nid;
	$json = sirius_jsonfield_util_node_json_get($certification_definition_node);

	$form['#tree'] = TRUE;
	$form['certification']['skills'] = array(
		'#type' => 'select',
		'#title' => t('This certification may grant or renew the following skills'),
		'#options' => $skill_options,
		'#multiple' => TRUE,
		'#size' => 20,
		'#default_value' => $json['certification']['skills'],
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_training_node_tab_certification_definition_skills_form_submit($form, &$form_state) {
	$certification_definition_node = node_load($form_state['certification_definition_nid']);
	$json = sirius_jsonfield_util_node_json_get($certification_definition_node);
	$json['certification']['skills'] = $form_state['values']['certification']['skills'];
	sirius_jsonfield_util_node_json_set($certification_definition_node, $json);
	node_save($certification_definition_node);
	drupal_set_message("Skills saved.");
}