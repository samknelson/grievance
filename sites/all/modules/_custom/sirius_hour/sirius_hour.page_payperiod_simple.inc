<?php

function sirius_hour_node_tab_payperiod_simple_access($payperiod_node) {
	$access = Sirius_Hour_Access::getInstance();
	return $access->access('payperiod_view', array('payperiod_node' => $payperiod_node));
}

function sirius_hour_node_tab_payperiod_simple_form($form, &$form_state, $payperiod_node) {
	$payperiod_json = sirius_json_get($payperiod_node);

	// Just take the first row of the first entry.
	if ($payperiod_json['entries']) { $entry = reset($payperiod_json['entries']); }
	if ($entry && $entry['rows']) { $row = reset($entry['rows']); }
	if (!$row) { $row = array(); }

	// If there is legacy data, aggregate the total
	if ($entry && $entry['rows'] && count($entry['rows']) > 0) {
		$qty = 0;
		foreach ($entry['rows'] as $delta => $sub_row) {
			if ($sub_row['qty']) {
				$qty += $sub_row['qty'];
			}
		}
		$row['qty'] = $qty;
	}


	// Always use day = 15, to avoid stupid timezone crap.	
	$row['day'] = 15;

	// Default hour type and department
	if (!$row['department']) { $row['department'] = sirius_domain_variable_get('sirius_hour_default_department'); }
	if (!$row['hour_type']) { $row['hour_type'] = sirius_domain_variable_get('sirius_hour_default_type'); }

	$hour_types = sirius_taxonomy_load('sirius_hour_type', 'tid', 'name');
	$departments = sirius_taxonomy_load('grievance_department', 'tid', 'name');

	$form['#tree'] = TRUE;
	$form_state['payperiod_nid'] = $payperiod_node->nid;

	$form['row'] = array();

	$form['row']['day'] = array(
		'#type' => 'value',
		'#value' => 15,
	);

	$form['row']['qty'] = array(
		'#type' => 'textfield',
		'#title' => t('Number of Hours'),
		'#required' => TRUE,
		'#size' => 4,
	);

	$default_type = $row['hour_type'];
	if (!$default_type) { }
	$form['row']['hour_type'] = array(
		'#type' => 'select',
		'#title' => t('Hour Type'),
		'#options' => array(NULL => t('-- Select --')) + $hour_types,
		'#required' => TRUE,
	);

	$form['row']['department'] = array(
		'#type' => 'select',
		'#title' => t('Department'),
		'#options' => array(NULL => t('-- Select --')) + $departments,
		'#required' => TRUE,
	);

	$form['row']['comment'] = array(
		'#type' => 'textarea',
		'#title' => t('Comment'),
	);

	sirius_jsonfield_util_set_defaults($form['row'], $row);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_hour_node_tab_payperiod_simple_form_validate($form, &$form_state) {
	$payperiod_node = node_load($form_state['payperiod_nid']);

	$qty = $form_state['values']['row']['qty'];
	if (!is_numeric($qty)) {
		form_set_error('row][qty', "Please enter a valid number of hours.");
	}

	// @todo: This should be in another module!
	if (sirius_domain_component_enabled('trust:cobra')) {
		$cobra = Sirius_Trust_Cobra::getInstance();
		if ($form_state['values']['row']['hour_type'] == $cobra->cobra_hour_type()) {
			if ($payperiod_node->field_grievance_shop['und'][0]['target_id'] != $cobra->cobra_employer(TRUE)) {
				form_set_error('row][hour_type', 'COBRA hours can only be applied at the COBRA employer.');
			}
		}

		if ($form_state['values']['row']['hour_type'] != $cobra->cobra_hour_type()) {
			if ($payperiod_node->field_grievance_shop['und'][0]['target_id'] == $cobra->cobra_employer(TRUE)) {
				form_set_error('row][hour_type', 'All hours at the COBRA employer must be of type COBRA.');
			}
		}
	}

	return;
}

function sirius_hour_node_tab_payperiod_simple_form_submit($form, &$form_state) {
	$payperiod_node = node_load($form_state['payperiod_nid']);
	$payperiod_json = sirius_json_get($payperiod_node);

	$source_key = 'manual';
	$entry = array();
	$entry['source_key'] = $source_key;
	$entry['status'] = 'live';
	$entry['source_name'] = 'Manual';
	$entry['rows'] = array($form_state['values']['row']);

	$payperiod_json['entries'] = array('manual' => $entry);
	$payperiod_node->field_sirius_active['und'][0]['value'] = 'Yes';
	sirius_hour_payperiod_compute($payperiod_json);
	sirius_json_set($payperiod_node, $payperiod_json);
	node_save($payperiod_node);
	
	drupal_set_message("Hours Entry Saved");
}

