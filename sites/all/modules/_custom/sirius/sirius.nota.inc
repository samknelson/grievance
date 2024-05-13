<?php

function sirius_nota_info_all() {
	return array(
		'content_types' => array(
			'grievance' => array(
				'field_sirius_gender'
			),
			'sirius_worker' => array(
				'field_sirius_gender'
			),
		)
	);
}

function sirius_nota_form_alter(&$form, &$form_state, $form_id) {
	$info_all = sirius_nota_info_all();
	foreach ($info_all['content_types'] as $content_type => $fields) {
		if ($form_id == $content_type . '_node_form') {
			foreach ($fields as $field) {
				if ($form[$field]) {
					sirius_nota_form_alter_field($form, $form_state, $field);
				}
			}
		}
	}
}

function sirius_nota_form_alter_field(&$form, &$form_state, $field) {
	// A "none of the above" form requires three fields: 
	// - The taxonomy field for selecting an option
	// - The "none of the above" value
	// - The calculated value, either the taxonomy or the NOTA value
	$field_val = $field . '_nota_val';
	$field_calc = $field . '_nota_calc';
	if (!$form[$field_val]) { return; }
	if (!$form[$field_calc]) { return; }

	// Find the taxonomy. This assumes some things about the main field
	$field_info = field_info_field($field);
	$vocabulary_name = $field_info['settings']['allowed_values'][0]['vocabulary'];
	if (!$vocabulary_name) { return; }
	$terms = sirius_taxonomy_load($vocabulary_name, 'tid', 'full');

	// The "calc" field is always disabled
	$form[$field_calc]['#access'] = FALSE;

	// Figure out which tids are "none of the above"
	$nota_tids = array();
	foreach ($terms as $tid => $term) { 
		if ($term->field_sirius_nota['und'][0]['value'] == 'Yes') {
			$nota_tids[] = $tid;
		}
	}

	if ($nota_tids) {
		// Use #states to make $field_val conditional on $field
		$tmp = array();
		foreach ($nota_tids as $nota_tid) {
			$tmp[] = array(":input[name=\"${field}[und]\"]" => array('value' => $nota_tid));
		}
		$form[$field_val]['#states'] = array('visible' => $tmp);
	} else {
		// We don't have any NOTA options. The "val" field is always disabled
		$form[$field_val]['#access'] = FALSE;
	}
}

function sirius_nota_node_presave($node) {
	$info_all = sirius_nota_info_all();
	$fields = $info_all['content_types'][$node->type];
	if (!$fields) { return; }
	foreach ($fields as $field) {
		sirius_nota_node_presave_field($node, $field);
	}
}

function sirius_nota_node_presave_field($node, $field) {
	$field_val = $field . '_nota_val';
	$field_calc = $field . '_nota_calc';

	$calc = '';

	$term = taxonomy_term_load($node->{$field}['und'][0]['tid']);
	if (!$term) {
		// No taxonomy term. Calculated value is empty.
		$calc = ''; 
	} else if ($term->field_sirius_nota['und'][0]['value'] == 'Yes') {
		// Taxonomy term is a NOTA term. Use the NOTA value.
		$calc = $node->{$field_val}['und'][0]['value'];
	} else {
		// Taxonomy term is not a NOTA term. Use the term name.
		$calc = $term->name;
	}

	$node->{$field_calc}['und'][0]['value'] = $calc;
}
