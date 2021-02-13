<?php

function sirius_page_contact_definition_display_types() {
	return array(
		'phonebank-default' => array(
			'title' => 'Phone Bank: Default',
			'description' => 'Select the default set of fields that should be displayed to somebody using the phone bank; for example, if a volunteer is calling a voter, they might display "Age" and "Party Affiliation".',
		),
		'dispatch-default' => array(
			'title' => 'Dispatch: Default',
			'description' => 'Select the default set of fields that should be displayed on dispatches.',
		),
	);
}

function sirius_page_contact_definition_display() {
	return drupal_get_form('sirius_page_contact_definition_display_form');
}

function sirius_page_contact_definition_display_form($form, &$form_state) {
	$contact_definition_json = sirius_contact_definition();
	$contact_definition_form = sirius_jsonfield_util_form_get($contact_definition_json, 'default');
	$paths = array_values(sirius_jsonfield_util_form_list_paths($contact_definition_form, TRUE));

	$core_paths = array_reverse(array_keys(sirius_jsonfield_util_list_node_fields('sirius_contact')));
	foreach ($core_paths as $core_path) { array_unshift($paths, $core_path); }

	$weight = 0;
	foreach ($paths as $path) {
		$options["$weight$path"] = $path;
		++$weight;
	}

	$form['#tree'] = TRUE;

	$types = sirius_page_contact_definition_display_types();

	foreach ($types as $type_id => $type_info) {
		$form['display'][$type_id] = array(
			'#type' => 'select',
			'#multiple' => TRUE,
			'#title' => $type_info['title'],
			'#description' => $type_info['description'],
			'#options' => $options,
			'#default_value' => $contact_definition_json['display'][$type_id],
			'#size' => 10,
		);
	}


	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_page_contact_definition_display_form_submit($form, &$form_state) {
	$nid = sirius_contact_definition_get_nid(NULL, TRUE);
	$node = node_load($nid);
	$json = json_decode($node->field_sirius_json['und'][0]['value'], TRUE);
	$json['display'] = $form_state['values']['display'];
	$node->field_sirius_json['und'][0]['value'] = json_encode($json);
	node_save($node);
}

function sirius_contact_definition_display_get($contact_definition, $which) {
	$unsorted_display_fields = $contact_definition['display'][$which];
	if (!$unsorted_display_fields) { $unsorted_display_fields = array(); }
	$display_fields = array();
	foreach ($unsorted_display_fields as $path_with_weight) {
		$parts = explode(':', $path_with_weight);
		$weight = array_shift($parts);
		$path = ':' . join(':', $parts);
		$display_fields[$weight] = $path;
	}
	ksort($display_fields);
	$display_fields = array_values($display_fields);
	return $display_fields;
}