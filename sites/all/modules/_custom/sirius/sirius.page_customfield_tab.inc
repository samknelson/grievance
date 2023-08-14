<?php

function sirius_node_tab_customfield_tab_access($cf_node) {
	// Only JSON Definitions of type "customfield"
	if ($cf_node->field_sirius_type['und'][0]['value'] != 'customfield') { return FALSE; }

	return TRUE;
}

function sirius_node_tab_customfield_tab_form($form, &$form_state, $cf_node) {
	$form_state['cf_nid'] = $cf_node->nid;

	$cf_json = sirius_json_get($cf_node);
	$form['#tree'] = TRUE;

	$access_all = sirius_access_info_all();
	$options = array(NULL => t('-- Select --'));
	foreach ($access_all as $key => $record) {
		$options[$key] = $record['name'];
	}
	$form['customfield']['access']['type'] = array(
		'#title' => 'Access Policy',
		'#type' => 'select',
		'#default_value' => sirius_form_array_default_value($cf_json['customfield']['access']['type']),
		'#options' => $options,
		'#required' => TRUE,
		'#description' => t('<strong>IMPORTANT:</strong> After selecting an access policy, ensure that the "Access Settings" tab is completed.'),
	);

	$title_default = $cf_json['customfield']['tab']['title'];
	if (!$title_default) { $title_default = $cf_node->title; }
	$form['customfield']['tab']['title'] = array(
		'#title' => 'Title',
		'#type' => 'textfield',
		'#default_value' => $title_default,
	);

	$form['customfield']['tab']['weight'] = array(
		'#title' => 'Weight',
		'#type' => 'textfield',
		'#default_value' => $cf_json['customfield']['tab']['weight'],
		'#size' => 4,
	);

	$form['customfield']['tab']['show_by_default'] = array(
		'#title' => 'Show by Default?',
		'#type' => 'select',
		'#default_value' => sirius_form_array_default_value($cf_json['customfield']['tab']['show_by_default']),
		'#options' => array(
			'No' => t('No'),
			'Yes' => t('Yes'),
		),
	);

	$form['customfield']['tab']['multiple'] = array(
		'#title' => 'Multi-value?',
		'#type' => 'select',
		'#default_value' => sirius_form_array_default_value($cf_json['customfield']['tab']['multiple']),
		'#options' => array(
			'No' => t('No'),
			'Yes' => t('Yes'),
		),
	);

	$form['customfield']['tab']['headfoot'] = array(
		'#type' => 'fieldset',
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
		'#title' => t('Header and Footer'),
		'#description' => t('Optionally, enter a header and footer that should appear at the top and bottom of the tab.'),
	);

	$form['customfield']['tab']['headfoot']['head'] = array(
		'#title' => 'Header',
		'#type' => 'text_format',
		'#default_value' => $cf_json['customfield']['tab']['headfoot']['head'],
	);

	$form['customfield']['tab']['headfoot']['foot'] = array(
		'#title' => 'Footer',
		'#type' => 'text_format',
		'#default_value' => $cf_json['customfield']['tab']['headfoot']['foot'],
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;	
}

function sirius_node_tab_customfield_tab_form_submit($form, &$form_state) {
	$cf_node = node_load($form_state['cf_nid']);
	$cf_json = sirius_json_get($cf_node);
	$cf_json['customfield']['tab'] = $form_state['values']['customfield']['tab'];
	$cf_json['customfield']['tab']['headfoot']['head'] = sirius_richtext_handle_input($form_state['values']['customfield']['tab']['headfoot']['head']);
	$cf_json['customfield']['tab']['headfoot']['foot'] = sirius_richtext_handle_input($form_state['values']['customfield']['tab']['headfoot']['foot']);
	$cf_json['customfield']['access']['type'] = $form_state['values']['customfield']['access']['type'];
	sirius_json_set($cf_node, $cf_json);
	node_save($cf_node);
	drupal_set_message('Settings saved.');
}