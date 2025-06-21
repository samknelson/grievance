<?php

function sirius_edls_node_tab_employer_active_access($employer_node) {
	return sirius_access_instance('edls')->access('employer_config', array('employer_nid' => $employer_node->nid));
}

function sirius_edls_node_tab_employer_active_form($form, &$form_state, $employer_node) {
	$form['#tree'] = TRUE;
	$form_state['employer_nid'] = $employer_node->nid;

	$employer_json = sirius_json_get($employer_node);
	$form['edls']['active'] = array(
		'#type' => 'select',
		'#title' => t('EDLS (Employer Day Labor System) enabled for this employer?'),
		'#options' => array(
			NULL => t('-- Select --'),
			'No' => t('No'),
			'Yes' => t('Yes'),
		),
		'#default_value' => $employer_json['edls']['active'],
	);

	if (sirius_access_instance('edls')->access('employer_config_active', array('employer_nid' => $employer_node->nid))) {
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit'),
		);
	} else {
		$form['edls']['active']['#disabled'] = TRUE;
	}

	return $form;
}

function sirius_edls_node_tab_employer_active_form_submit($form, &$form_state) {
	$employer_node = node_load($form_state['employer_nid']);
	$employer_json = sirius_json_get($employer_node);
	$employer_json['edls']['active'] = $form_state['values']['edls']['active'];
	sirius_json_set($employer_node, $employer_json);
	node_save($employer_node);
	drupal_set_message("Settings saved.");
}
