<?php

function sirius_node_tab_sirius_domain_components($domain_node) {
	return drupal_get_form('sirius_node_tab_sirius_domain_components_form', $domain_node);
}

function sirius_node_tab_sirius_domain_components_form($form, &$form_state, $domain_node) {
	$form_state['domain_nid'] = $domain_node->nid;
	$json = sirius_jsonfield_util_node_json_get($domain_node);
	$form['#tree'] = TRUE;

	$all_info = sirius_domain_component_info_all();

	foreach ($all_info as $key => $info) {
		$form['component'][$key]['enabled'] = array(
			'#type' => 'select',
			'#options' => array(
				NULL => t('-- Select --'),
				'No' => t('No'),
				'Yes' => t('Yes'),
			),
			'#title' => $info['title'],
			'#description' => $info['description'],
			'#default_value' => $json['component'][$key]['enabled'],
		);
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_node_tab_sirius_domain_components_form_submit($form, &$form_state) {
	$domain_node = node_load($form_state['domain_nid']);
	$json = sirius_jsonfield_util_node_json_get($domain_node);
	$json['component'] = $form_state['values']['component'];
	sirius_jsonfield_util_node_json_set($domain_node, $json);
	node_save($domain_node);
	drupal_set_message("Components saved.");
}