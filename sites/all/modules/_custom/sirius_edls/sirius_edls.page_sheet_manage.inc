<?php

function sirius_edls_node_tab_sheet_manage_access($sheet_node) {
  return sirius_access_instance('edls')->access('sheet_manage', array('sheet_node' => $sheet_node));
}

function sirius_edls_node_tab_sheet_manage_form($form, &$form_state, $sheet_node) {
	$access = sirius_access_instance('edls');
	$form_state['sheet_nid'] = $sheet_node->nid;
	$form['#tree'] = TRUE;
	$sheet_json = sirius_json_get($sheet_node);

	$form['intro'] = array('#markup' => sirius_edls_sheet_render($sheet_node, array('hide_crews' => TRUE)));

	$actions = array();
	if ($access->access('sheet_clone', array('sheet_node' => $sheet_node))) {
		$actions['sheet_clone'] = 'Make a Copy';
	}

	$status_options = sirius_edls_sheet_status_options();
	foreach ($status_options as $status_key => $status_name) {
		if ($access->access("sheet_status_$status_key", array('sheet_node' => $sheet_node))) {
			$actions["sheet_status_$status_key"] = "Set status: " . $status_name;
		}
	}

	if ($access->access('sheet_notify_enable', array('sheet_node' => $sheet_node))) {
		$actions['sheet_notify_enable'] = 'Enable Notifications';
	}

	if ($access->access('sheet_export', array('sheet_node' => $sheet_node))) {
		$actions['sheet_export'] = 'Export';
	}

	if (!$actions) { return sirius_form_error('You do not have permission to perform any actions on this sheet.'); }

	$form['action'] = array(
		'#type' => 'select',
		'#title' => t('Action'),
		'#options' => array(NULL => t('-- Select --')) + $actions,
		'#required' => TRUE,
	);

	$form['sheet_notify_enable'] = array(
		'#type' => 'fieldset',
		'#title' => 'Notifications',
		'#states' => array('visible' => array(":input[name=\"action\"]" => array('value' => 'sheet_notify_enable'))),
	);

	$form['sheet_notify_enable']['enable'] = array(
		'#type' => 'select',
		'#title' => t('Are worker notifications enabled for this sheet?'),
		'#options' => array(
			'No' => t('No'),
			'Yes' => t('Yes'),
		),
		'#default_value' => $sheet_json['edls']['notify']['enable'],
	);

	$form['sheet_clone'] = array(
		'#type' => 'fieldset',
		'#title' => 'Make a Copy',
		'#states' => array('visible' => array(":input[name=\"action\"]" => array('value' => 'sheet_clone'))),
	);

	// Clone Form
	$date_options = array();
	foreach (array("-1 days", "today", "+1 day", "+2 day", "+3 days", "+4 days", "+5 days", "+6 days") as $d) {
		$t = strtotime($d);

		if ($d == '-1 days') { $name = t('Yesterday'); }
		else if ($d == 'today') { $name = t('Today'); }
		else if ($d == '+1 day') { $name = t('Tomorrow'); }
		else { $name = date('l (j F)', $t); }

		$date_options[date('Y-m-d', $t)] = $name;
	}
	$date_options['other'] = t('Other Date');

  $form['sheet_clone']['date'] = array(
	  '#type' => 'select',
	  '#title' => t('Date'),
	  '#options' => $date_options,
		'#default_value' => $date,
  );

  $form['sheet_clone']['date_other'] = array(
	  '#type' => 'date_popup',
	  '#title' => t('Date'),
		'#attributes' => array(
			'autocomplete' => 'off',
		),
		'#date_format' => 'm/d/Y',
		'#default_value' => $date,
		'#states' => array('visible' => array(":input[name=\"sheet_clone[date]\"]" => array('value' => 'other'))),
  );

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_edls_node_tab_sheet_manage_form_submit($form, &$form_state) {
	$sheet_node = node_load($form_state['sheet_nid']);

	$action = $form_state['values']['action'];
	$status_options = sirius_edls_sheet_status_options();

	if ($action == 'sheet_clone') {
		$date_raw = $form_state['values']['sheet_clone']['date'];
		if ($date_raw == 'other') { $date_raw = $form_state['values']['sheet_clone']['date_other']; }
		if (!$date_raw) {
			drupal_set_message("Please select a date", 'error');
			$form_state['rebuild'] = TRUE;
			return;
		}

		$date = date('Y-m-d', strtotime($date_raw));
		if ($date == date('Y-m-d', strtotime($sheet_node->field_sirius_date_start['und'][0]['value']))) {
			drupal_set_message("The new sheet cannot have the same date", 'error');
			$form_state['rebuild'] = TRUE;
			return;
		}

		$result = sirius_edls_sheet_clone($sheet_node, array('date' => $date));
		$msg = "Sheet copied successfully.";
	}

	if ($action == 'sheet_status_draft') {
		$result = sirius_edls_sheet_status_set($sheet_node, 'draft');
		$msg = "Sheet status set to " . $status_options['draft'];
	}

	if ($action == 'sheet_status_request') {
		$result = sirius_edls_sheet_status_set($sheet_node, 'request');
		$msg = "Sheet status set to " . $status_options['request'];
	}

	if ($action == 'sheet_status_lock') {
		$result = sirius_edls_sheet_status_set($sheet_node, 'lock');
		$msg = "Sheet status set to " . $status_options['lock'];
	}

	if ($action == 'sheet_status_trash') {
		$result = sirius_edls_sheet_status_set($sheet_node, 'trash');
		$msg = "Sheet status set to " . $status_options['trash'];
	}

	if ($action == 'sheet_status_reserved') {
		$result = sirius_edls_sheet_status_set($sheet_node, 'reserved');
		$msg = "Sheet status set to " . $status_options['reserved'];
	}

	if ($action == 'sheet_notify_enable') {
		$result = sirius_edls_sheet_notify_enable($sheet_node, $form_state['values']['sheet_notify_enable']['enable']);
		$msg = "Notification status set to " . $form_state['values']['sheet_notify_enable']['enable'];
	}

	if ($action == 'sheet_export') {
		sirius_edls_sheet_export($sheet_node);
	}

	if (!$result['success']) {
		drupal_set_message($result['msg'], 'error');
		$form_state['rebuild'] = TRUE;
		return;
	}

	drupal_set_message($msg);
	if ($result['warnings']) {
		foreach ($result['warnings'] as $warning) {
			drupal_set_message("Warning: $warning", 'warning');
		}
	}

	if ($result['sheet_node']) {
		drupal_goto("node/" . $result['sheet_node']->nid . "/sirius_edls_sheet_view");
	}
}
