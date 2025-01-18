<?php

function sirius_edls_node_tab_employer_layoff_access($employer_node) {
	return sirius_access_instance('edls')->access('employer_config', array('employer_nid' => $employer_node->nid));
}

function sirius_edls_node_tab_employer_layoff_form($form, &$form_state, $employer_node) {
	$form['#tree'] = TRUE;
	$form_state['employer_nid'] = $employer_node->nid;

	$employer_json = sirius_json_get($employer_node);

	$form['edls']['layoff']['active'] = array(
		'#type' => 'select',
		'#title' => t('Active?'),
		'#options' => array(
			NULL => t('-- Select --'),
			'No' => t('No'),
			'Yes' => t('Yes'),
		),
		'#description' => 'Is the layoff connector active for this employer?',
	);

	$form['edls']['layoff']['url'] = array(
		'#type' => 'textfield',
		'#title' => t('URL'),
		'#description' => 'This will probably be a value like <br><strong>https://dispatch.site.com/node/12345/sirius_employer_dispatch/layoff_connector</strong><br>where <strong>12345</strong> is the NID of the employer.',
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	sirius_jsonfield_util_set_defaults($form['edls']['layoff'], $employer_json['edls']['layoff']);

	return $form;
}

function sirius_edls_node_tab_employer_layoff_form_submit($form, &$form_state) {
	$employer_node = node_load($form_state['employer_nid']);
	$employer_json = sirius_json_get($employer_node);
	$employer_json['edls']['layoff'] = $form_state['values']['edls']['layoff'];
	sirius_json_set($employer_node, $employer_json);
	node_save($employer_node);
	drupal_set_message("Settings saved.");
}
