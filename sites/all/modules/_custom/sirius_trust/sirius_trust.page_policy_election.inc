<?php

function sirius_trust_node_tab_policy_election_access($policy_node) {
	return $policy_node->field_sirius_type['und'][0]['value'] == 'sirius_ledger:policy';
}

function sirius_trust_node_tab_policy_election($policy_node) {
	return drupal_get_form('sirius_trust_node_tab_policy_election_form', $policy_node);
}

function sirius_trust_node_tab_policy_election_form($form, &$form_state, $policy_node) {
	$policy_json = sirius_json_get($policy_node);
	$form_state['policy_nid'] = $policy_node->nid;

	$form['#tree'] = TRUE;

	// ////////////////////////////////////
	// ELECTION TYPES
	// ////////////////////////////////////

	$form['elections']['types'] = array(
		'#title' => 'Election Types',
		'#type' => 'fieldset',
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
	);
	$election_options = sirius_taxonomy_load('sirius_trust_election_type', 'tid', 'name');
	$form['elections']['types']['electable'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Election Types'),
		'#options' => $election_options,
		'#description' => t('Select the election types that are available under this policy.'),
		'#checkall' => TRUE,
		'#default_value' => sirius_form_array_default_value($policy_json['elections']['types']['electable']),
	);

	// ////////////////////////////////////
	// INTRO
	// ////////////////////////////////////

	$form['elections']['intro'] = array(
		'#title' => 'Intro',
		'#type' => 'fieldset',
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
	);

	$form['elections']['intro']['body'] = array(
	  '#type' => 'text_format',
	  '#title' => t('Introductory Text'),
	  '#description' => t('Enter any text that should appear on the introductory page of the election wizard.'),
	  '#default_value' => $policy_json['elections']['intro']['body'],
	  '#format' => 'sirius_form_editor',
	);

	// ////////////////////////////////////
	// DISCLAIMERS
	// ////////////////////////////////////

	$form['elections']['disclaimer'] = array(
		'#title' => 'Disclaimers',
		'#type' => 'fieldset',
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
	);

	$form['elections']['disclaimer']['nids'] = array(
		'#title' => 'Disclaimers',
		'#options' => sirius_disclaimer_list(),
		'#type' => 'checkboxes',
		'#default_value' => sirius_form_array_default_value($policy_json['elections']['disclaimer']['nids']),
	);

	// ////////////////////////////////////
	// SIGNATURES
	// ////////////////////////////////////

	$form['elections']['esignature'] = array(
		'#title' => 'Signatures',
		'#type' => 'fieldset',
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
	);

	$defs = sirius_esignature_defs();
	if (!$defs) {
		$form['elections']['esignature']['def'] = array('#markup' => '<p>' . t('There are no signatures configured. Go to the ') . l('configuration page', 'sirius/esignature/admin') . t(' to create them.') . '</p>');
	}	else {
		$options = array(NULL => t('-- Select --'));
		foreach ($defs as $def) {
			$options[$def['code']] = $def['title'];
		}
		$form['elections']['esignature']['def'] = array(
			'#title' => 'Signature',
			'#type' => 'select',
			'#options' => $options,
			'#default_value' => $policy_json['elections']['esignature']['def'],
	 	);
	 	if (drupal_valid_path('sirius/esignature/admin')) {
	 		$formm['elections']['esignature']['def']['#title'] .= ' ' . l('(Manage)', 'sirius/esignature/admin');
	 	}
		$form['elections']['esignature']['statement'] = array(
			'#title' => 'Statment',
			'#type' => 'textarea',
			'#default_value' => $policy_json['elections']['esignature']['statement'],
			'#description' => t('E.g. "By signing below, I assert that all the information that I have provided in this application is true and accurate."'),
	 	);
	}

	// ////////////////////////////////////
	// Benefits
	// ////////////////////////////////////

	$benefits = sirius_trust_policy_benefits_offered($policy_node);
	$benefit_options = array();
	foreach ($benefits as $benefit_nid => $benefit_node) {
		$benefit_options[$benefit_nid] = $benefit_node->title;
	}
	$form['elections']['benefits'] = array(
		'#title' => 'Benefits',
		'#type' => 'fieldset',
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
	);

	$form['elections']['benefits']['electable'] = array(
		'#title' => t('Electable Benefits'),
		'#type' => 'checkboxes',
		'#checkall' => TRUE,
		'#options' => $benefit_options,
		'#default_value' => sirius_form_array_default_value($policy_json['elections']['benefits']['electable']),
	);

	$form['elections']['benefits']['prefix'] = array(
	  '#type' => 'text_format',
	  '#title' => t('Prefix'),
	  '#description' => t('Enter any text that should appear at the top of this page of the wizard'),
	  '#default_value' => $policy_json['elections']['benefits']['prefix'],
	  '#format' => 'sirius_form_editor',
	);

	$form['elections']['benefits']['suffix'] = array(
	  '#type' => 'text_format',
	  '#title' => t('Prefix'),
	  '#description' => t('Enter any text that should appear at the bottom of this page of the wizard'),
	  '#default_value' => $policy_json['elections']['benefits']['suffix'],
	  '#format' => 'sirius_form_editor',
	);

	// ////////////////////////////////////
	// Dependents
	// ////////////////////////////////////

	$form['elections']['dependents'] = array(
		'#title' => 'Dependents',
		'#type' => 'fieldset',
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
	);

	$form['elections']['dependents']['existing']['prefix'] = array(
	  '#type' => 'text_format',
	  '#title' => t('Prefix - Current Dependents Page'),
	  '#default_value' => $policy_json['elections']['dependents']['existing']['prefix'],
	  '#format' => 'sirius_form_editor',
	);

	$form['elections']['dependents']['existing']['suffix'] = array(
	  '#type' => 'text_format',
	  '#title' => t('Suffix = Current Dpeendents Page'),
	  '#default_value' => $policy_json['elections']['dependents']['existing']['suffix'],
	  '#format' => 'sirius_form_editor',
	);

	$form['elections']['dependents']['new']['prefix'] = array(
	  '#type' => 'text_format',
	  '#title' => t('Prefix - New Dependent Page'),
	  '#default_value' => $policy_json['elections']['dependents']['new']['prefix'],
	  '#format' => 'sirius_form_editor',
	);

	$form['elections']['dependents']['new']['suffix'] = array(
	  '#type' => 'text_format',
	  '#title' => t('Suffix - New Dependent Page'),
	  '#default_value' => $policy_json['elections']['dependents']['new']['suffix'],
	  '#format' => 'sirius_form_editor',
	);

	// ////////////////////////////////////
	// PUSH / SUBMIT
	// ////////////////////////////////////

	$form['elections']['push'] = array(
		'#title' => 'Submit',
		'#type' => 'fieldset',
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
	);

	$form['elections']['push']['body'] = array(
	  '#type' => 'text_format',
	  '#title' => t('Introductory Text'),
	  '#description' => t('Enter any text that should appear on the introductory page of the "submit" step of the election wizard. E.g. "Please review your election carefully and click "Submit". After submitting, your election will be locked and you won\'t be able to make any changes.'),
	  '#default_value' => $policy_json['elections']['push']['body'],
	  '#format' => 'sirius_form_editor',
	);
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_trust_node_tab_policy_election_form_submit($form, &$form_state) {
	$policy_node = node_load($form_state['policy_nid']);
	$policy_json = sirius_json_get($policy_node);
	$policy_json['elections'] = $form_state['values']['elections'];
	$policy_json['elections']['intro']['body'] 				= sirius_richtext_handle_input($form_state['values']['elections']['intro']['body']);
	$policy_json['elections']['benefits']['prefix'] 	= sirius_richtext_handle_input($form_state['values']['elections']['benefits']['prefix']);
	$policy_json['elections']['benefits']['suffix'] 	= sirius_richtext_handle_input($form_state['values']['elections']['benefits']['suffix']);
	$policy_json['elections']['dependents']['existing']['prefix'] = sirius_richtext_handle_input($form_state['values']['elections']['dependents']['existing']['prefix']);
	$policy_json['elections']['dependents']['existing']['suffix'] = sirius_richtext_handle_input($form_state['values']['elections']['dependents']['new']['suffix']);
	$policy_json['elections']['dependents']['new']['prefix'] = sirius_richtext_handle_input($form_state['values']['elections']['dependents']['existing']['prefix']);
	$policy_json['elections']['dependents']['new']['suffix'] = sirius_richtext_handle_input($form_state['values']['elections']['dependents']['new']['suffix']);
	$policy_json['elections']['push']['body'] 				= sirius_richtext_handle_input($form_state['values']['elections']['push']['body']);
	sirius_json_set($policy_node, $policy_json);
	node_save($policy_node);
	drupal_set_message("Settings saved.");
}
