<?php

function sirius_hour_node_tab_payperiod_entries($payperiod_node) {
	drupal_goto('node/' . $payperiod_node->nid . '/sirius_payperiod_entries/list');
}

function sirius_hour_node_tab_payperiod_entries_edit($payperiod_node) {
	return drupal_get_form('sirius_hour_node_tab_payperiod_entries_edit_form', $payperiod_node);
}

function sirius_hour_node_tab_payperiod_entries_edit_form($form, &$form_state, $payperiod_node) {
	$form_state['payperiod_nid'] = $payperiod_node->nid;
	$json = json_decode($payperiod_node->field_sirius_json['und'][0]['value'], TRUE);
	$hour_types = sirius_taxonomy_load('sirius_hour_type', 'tid', 'name', TRUE);
	$departments = sirius_taxonomy_load('grievance_department', 'tid', 'name', TRUE);
	$payperiod_type = $payperiod_node->field_sirius_payperiod_type['und'][0]['value'];

	$entry_count = $form_state['entry_count'];
	if (!$entry_count && is_array($json['entries'])) { $entry_count = count($json['entries']) + 1; }
	if (!$entry_count) { $entry_count = 1; }
  if ($form_state['clicked_button']['#value'] == 'Add one') { $entry_count++; }
  if ($form_state['clicked_button']['#value'] == 'Add ten') { $entry_count += 10; }
  $form_state['entry_count'] = $entry_count;

  $form['#tree'] = TRUE;
	$form['wrap_open'] = array('#markup' => '<div class="sirius-form">');

  $form['entries'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<div id="sirius_entries">',
    '#suffix' => '</div>',
  );

	for ($i=0; $i<$entry_count; ++$i) {
		$form['entries']["entry_${i}"] = array(
			'#type' => 'fieldset',
			'#title' => t('Entry #@i', array('@i' => $i+1)),
		);

		$form['entries']["entry_${i}"]["entry_${i}_hour_type"] = array(
			'#type' => 'select',
			'#options' => $hour_types,
			'#title' => t('Hour Type'),
			'#prefix' => '<div class="container-inline">',
			'#suffix' => '</div>',
		);
		if ($json['entries'][$i]['hour_type_tid']) { $form['entries']["entry_${i}"]["entry_${i}_hour_type"]['#default_value'] = $json['entries'][$i]["hour_type_tid"]; }

		$form['entries']["entry_${i}"]["entry_${i}_department_tid"] = array(
			'#type' => 'select',
			'#options' => $departments,
			'#title' => t('Department'),
			'#prefix' => '<div class="container-inline">',
			'#suffix' => '</div>',
		);
		if ($json['entries'][$i]['department_tid']) { $form['entries']["entry_${i}"]["entry_${i}_department_tid"]['#default_value'] = $json['entries'][$i]["department_tid"]; }

		if ($payperiod_type == 'timestamp') {
			$form['entries']["entry_${i}"]["entry_${i}_start"] = array(
				'#type' => 'date',
				'#title' => t('Start'),
				'#type' => 'date_popup',
				'#timepicker' => 'timepicker',
				'#timepicker_options' => array(
					'rows' => 6,
					'minutes' => array(
					  'starts' => 0,
					  'ends' => 56,
					  'interval' => 5,
					),
				),
				'#attributes' => array(
					'autocomplete' => 'off',
				),
				'#prefix' => '<div class="container-inline">',
				'#suffix' => '</div>',
			);
			if ($json['entries'][$i]['start_ts'] > 0) { $form['entries']["entry_${i}"]["entry_${i}_start"]['#default_value'] = date('Y-m-d H:i', $json['entries'][$i]["start_ts"]); }

			$form['entries']["entry_${i}"]["entry_${i}_end"] = array(
				'#type' => 'date',
				'#title' => t('End'),
				'#type' => 'date_popup',
				'#timepicker' => 'timepicker',
				'#timepicker_options' => array(
					'rows' => 6,
					'minutes' => array(
					  'starts' => 0,
					  'ends' => 56,
					  'interval' => 5,
					),
				),
				'#attributes' => array(
					'autocomplete' => 'off',
				),
				'#prefix' => '<div class="container-inline">',
				'#suffix' => '</div>',
			);
			if ($json['entries'][$i]['end_ts'] > 0) { $form['entries']["entry_${i}"]["entry_${i}_end"]['#default_value'] = date('Y-m-d H:i', $json['entries'][$i]["end_ts"]); }
		} else {
			$form['entries']["entry_${i}"]["entry_${i}_qty"] = array(
				'#type' => 'textfield',
				'#size' => 6,
				'#title' => t('Quantity'),
				'#prefix' => '<div class="container-inline">',
				'#suffix' => '</div>',
			);
			if ($json['entries'][$i]['qty'] > 0) { $form['entries']["entry_${i}"]["entry_${i}_qty"]['#default_value'] = $json['entries'][$i]["qty"]; }
		}

		$form['entries']["entry_${i}"]["entry_${i}_rate"] = array(
			'#type' => 'textfield',
			'#size' => 6,
			'#title' => t('Rate'),
			'#prefix' => '<div class="container-inline">',
			'#suffix' => '</div>',
		);
		if ($json['entries'][$i]['rate'] > 0) { $form['entries']["entry_${i}"]["entry_${i}_rate"]['#default_value'] = $json['entries'][$i]["rate"]; }

		$form['entries']["entry_${i}"]["entry_${i}_comment"] = array(
			'#type' => 'textfield',
			'#title' => t('Comment'),
			'#prefix' => '<div class="container-inline">',
			'#suffix' => '</div>',
		);
		if ($json['entries'][$i]['comment']) { $form['entries']["entry_${i}"]["entry_${i}_comment"]['#default_value'] = $json['entries'][$i]["comment"]; }
	}

  $form['entries']['add_one'] = array(
    '#type' => 'button',
    '#value' => t('Add one'),
    '#ajax' => array('callback' => 'sirius_generic_entries_addone'),
  );

  $form['entries']['add_ten'] = array(
    '#type' => 'button',
    '#value' => t('Add ten'),
    '#ajax' => array('callback' => 'sirius_generic_entries_addone'),
  );

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	$form['wrap_close'] = array('#markup' => '</div>');

	return $form;
}

function sirius_hour_node_tab_payperiod_entries_edit_form_submit($form, &$form_state) {
	$payperiod_nid = $form_state['payperiod_nid'];
	$payperiod_node = node_load($payperiod_nid);
	$payperiod_type = $payperiod_node->field_sirius_payperiod_type['und'][0]['value'];

	$json = json_decode($payperiod_node->field_sirius_json['und'][0]['value'], TRUE);

	$entries = array();

	if ($form_state['values']['entries']) {
		foreach ($form_state['values']['entries'] as $prefix => $entry) {
			if ($prefix == 'add_one' || $prefix == 'add_ten') { continue; }

			$hour_type_tid = $entry["${prefix}_hour_type"];
			if (!$hour_type_tid) { continue; }
			$hour_type_name = $hour_types[$hour_type_tid];

			if ($payperiod_type == 'timestamp') {
				$start_ts = strtotime($entry["${prefix}_start"]);
				if ($start_ts <= 0) { continue; }
				$end_ts = strtotime($entry["${prefix}_end"]);
				if ($end_ts <= 0) { continue; }

				$qty = ($end_ts - $start_ts) / (60*60);

			} else {
				$start_ts = sirius_date_ts($payperiod_node, 'field_sirius_datetime');
				$qty = round($entry["${prefix}_qty"], 2);
				$end_ts = $start_ts + ($qty*60*60);
			}
			$rate = round($entry["${prefix}_rate"], 2);
			$department_tid = $entry["${prefix}_department_tid"];
			$comment = $entry["${prefix}_comment"];
			$entry = array(
				'hour_type_tid' => $hour_type_tid,
				'start_ts' => $start_ts,
				'end_ts' => $end_ts,
				'rate' => $rate,
				'department_tid' => $department_tid,
				'qty' => $qty,
				'comment' => $comment,
			);
			$entries[] = $entry;
		}
	}

	$json['entries'] = $entries;

	sirius_hour_payperiod_compute($json);

	$payperiod_node->field_sirius_json['und'][0]['value'] = json_encode($json);
	node_save($payperiod_node);
}

function sirius_generic_entries_addone($form, $form_state) {
  $commands = array();
  $commands[] = ajax_command_replace("#sirius_entries", render($form['entries']));
  return array(
    '#type' => 'ajax',
    '#commands' => $commands
  );
}

function sirius_hour_node_tab_payperiod_entries_list($payperiod_node) {
	$json = json_decode($payperiod_node->field_sirius_json['und'][0]['value'], TRUE);
	$hour_types = sirius_taxonomy_load('sirius_hour_type', 'tid', 'name');
	$departments = sirius_taxonomy_load('grievance_department', 'tid', 'name');

	if (!$json['entries']) { return t('There are no entries in this payperiod.'); }

	$rows = array();
	foreach ($json['entries'] as $entry) {
		$hour_type_tid = $entry["hour_type_tid"];
		$hour_type_name = $hour_types[$hour_type_tid];

		$start_date = date('Y-m-d', $entry['start_ts']);
		$end_date =  date('Y-m-d', $entry['end_ts']);
		$start_time = date('h:i a', $entry['start_ts']);
		$end_time = date('h:i a', $entry['end_ts']);
		$date = $start_date;
		if ($end_date != $start_date) { $date .= " - $end_date"; }
		$time = "$start_time - $end_time";
		$rate = $entry['rate'];
		$department = $departments[$entry['department_tid']];

		$row = array();
		$row[] = $hour_type_name;
		$row[] = $department;
		$row[] = $date;
		$row[] = $time;
		$row[] = '$' . number_format($rate, 2);
		$rows[] = $row;
	}

	$header = array(
		t('Hour Type'),
		t('Department'),
		t('Date'),
		t('Time'),
		t('Rate'),
	);
	return theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array()));
}

function sirius_hour_node_tab_payperiod_entries_totals($payperiod_node) {
	$json = json_decode($payperiod_node->field_sirius_json['und'][0]['value'], TRUE);
	$hour_types = sirius_taxonomy_load('sirius_hour_type', 'tid', 'name');
	$departments = sirius_taxonomy_load('grievance_department', 'tid', 'name');
	if (!$json['hours']['payperiod']) { return t('There are no entries in this payperiod.'); }

	$rows = array();
	foreach ($json['hours']['payperiod'] as $hour_type_tid => $entries) {
		$hour_type_name = $hour_types[$hour_type_tid];
		foreach ($entries as $department_tid => $hours) {
			$department_name = $departments[$department_tid];
			$row = array();
			$row[] = $hour_type_name;
			$row[] = $department_name;
			$row[] = number_format($hours, 2);
			$row[] = '$' . number_format($json['totals']['payperiod'][$hour_type_tid][$department_tid], 2);
			$rows[] = $row;
		}
	}

	$header = array(
		t('Hour Type'),
		t('Department'),
		t('Hours'),
		t('Amount'),
	);
	return theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array()));
}

function sirius_hour_node_tab_payperiod_entries_daily($payperiod_node) {
	$json = json_decode($payperiod_node->field_sirius_json['und'][0]['value'], TRUE);
	$hour_types = sirius_taxonomy_load('sirius_hour_type', 'tid', 'name');
	$departments = sirius_taxonomy_load('grievance_department', 'tid', 'name');
	if (!$json['hours']['daily']) { return t('There are no entries in this payperiod.'); }

	$rows = array();
	foreach ($json['hours']['daily'] as $date => $entries) {
		foreach ($entries as $hour_type_tid => $sub_entries) {
			foreach ($sub_entries as $department_tid => $qty) {
				$department_name = $departments[$department_tid];
				$hour_type_name = $hour_types[$hour_type_tid];
				$row = array();
				$row[] = $date;
				$row[] = $hour_type_name;
				$row[] = $department_name;
				$row[] = number_format($qty, 2);
				$row[] = '$' . number_format($json['totals']['daily'][$date][$hour_type_tid][$department_tid], 2);
				$rows[] = $row;
			}
		}
	}

	$header = array(
		t('Date'),
		t('Hour Type'),
		t('Department'),
		t('Hours'),
		t('Amount'),
	);
	return theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array()));
}

function sirius_hour_node_tab_payperiod_entries_monthly($payperiod_node) {
	$json = json_decode($payperiod_node->field_sirius_json['und'][0]['value'], TRUE);
	$hour_types = sirius_taxonomy_load('sirius_hour_type', 'tid', 'name');
	$departments = sirius_taxonomy_load('grievance_department', 'tid', 'name');
	if (!$json['hours']['monthly']) { return t('There are no entries in this payperiod.'); }

	$rows = array();
	foreach ($json['hours']['monthly'] as $date => $entries) {
		foreach ($entries as $hour_type_tid => $sub_entries) {
			foreach ($sub_entries as $department_tid => $qty) {
				$department_name = $departments[$department_tid];
				$hour_type_name = $hour_types[$hour_type_tid];
				$row = array();
				$row[] = $date;
				$row[] = $hour_type_name;
				$row[] = $department_name;
				$row[] = number_format($qty, 2);
				$row[] = '$' . number_format($json['totals']['monthly'][$date][$hour_type_tid][$department_tid], 2);
				$rows[] = $row;
			}
		}
	}

	$header = array(
		t('Date'),
		t('Hour Type'),
		t('Department'),
		t('Hours'),
		t('Amount'),
	);
	return theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array()));
}

function sirius_hour_node_tab_payperiod_entries_accrual($payperiod_node) {
	return "... under development: vacation and sick leave accrual ...";
}