<?php

function sirius_trust_node_tab_benefit_beneficiary($benefit_node) {
	return drupal_get_form('sirius_trust_node_tab_benefit_beneficiary_form', $benefit_node);
}

function sirius_trust_node_tab_benefit_beneficiary_form($form, &$form_state, $benefit_node) {
	$form_state['benefit_nid'] = $benefit_node->nid;

	$benefit_json = sirius_json_get($benefit_node);
	$beneficiary_json = $benefit_json['beneficiary_settings'];

	$form['#tree'] = TRUE;

	$form['beneficiary_settings']['active'] = array(
		'#title' => t('Active?'),
		'#type' => 'select', 
		'#options' => array(
			NULL => t('-- Select --'),
			'No' => t('No'),
			'Yes' => t('Yes'),
		),
		'#description' => t('Does this benefit permit a beneficiary to be specified?'),
		'#id' => 'beneficiary_active',
		'#default_value' => $beneficiary_json['active'],
	);

	$form['beneficiary_settings']['alternates'] = array(
		'#title' => t('Number of Alternates'),
		'#type' => 'textfield',
		'#size' => 4, 
		'#description' => t('Enter the number of alternates, to receive the benefit if the first designee is not available'),
		'#default_value' => $beneficiary_json['alternates'],
		'#states' => array('visible' => array(":input[id=\"beneficiary_active\"]" => array('value' => 'Yes'))),
	);

	$form['beneficiary_settings']['partial'] = array(
		'#title' => t('Allow partial beneficiaries?'),
		'#type' => 'select', 
		'#options' => array(
			NULL => t('-- Select --'),
			'No' => t('No'),
			'Yes' => t('Yes'),
		),
		'#description' => t('Does this benefit permit partial beneficiaries (i.e. 50% to each child) to be entered?'),
		'#default_value' => $beneficiary_json['partial'],
		'#states' => array('visible' => array(":input[id=\"beneficiary_active\"]" => array('value' => 'Yes'))),
	);

	$relationship_type_otions = sirius_taxonomy_load('sirius_contact_relationship_types', 'tid', 'name');
	$form['beneficiary_settings']['relationship_types'] = array(
		'#title' => t('Relationship Types'),
		'#type' => 'checkboxes',
		'#options' => $relationship_type_otions,
		'#checkall' => TRUE,
		'#description' => t('Select the relationship types that are eligible to be designated as a beneficiary.'),
		'#default_value' => sirius_form_array_default_value($beneficiary_json['relationship_types']),
		'#states' => array('visible' => array(":input[id=\"beneficiary_active\"]" => array('value' => 'Yes'))),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_trust_node_tab_benefit_beneficiary_form_submit($form, &$form_state) {
	$benefit_node = node_load($form_state['benefit_nid']);
	$benefit_json = sirius_json_get($benefit_node);
	$benefit_json['beneficiary_settings'] = $form_state['values']['beneficiary_settings'];
	sirius_json_set($benefit_node, $benefit_json);
	node_save($benefit_node);
	drupal_set_message('Settings saved.');
}