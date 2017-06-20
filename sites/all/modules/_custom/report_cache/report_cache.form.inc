<?php

// Add some fields to a cached report generation form
function report_cache_form_prepare($report_type, &$form, &$form_state) {

	$form['report_cache'] = array(
    '#type' => 'fieldset',
		'#title' => t('Report Cache'),
		'#attributes' => array('class' => array('fill-wrapper')),
		'#collapsed' => TRUE, 
		'#collapsible' => TRUE,
  );

	$form['report_cache']['report_intro'] = array(
		'#type' => 'markup',
		'#markup' => 
			t('Use the form below to re-generate the report.  The results from previous runs may be downloaded ') .
			l(t('here'), "admin/reports/report-cache/$report_type") .
			t(' in csv format.'),
	);
	
	$form['report_cache']['report_id'] = array(
		'#type' => 'textfield',
		'#title' => 'Report Name',
		'#description' => t('Enter a name for this report "run".  If there is an existing run with the same name, it will be replaced or continued.  The date will be prefixed.'),
	);

	$form['report_cache']['report_action'] = array(
		'#type' => 'select',
		'#title' => 'Action',
		'#options' => array(
			'replace' => 'Replace any existing report with the same name',
			'continue' => 'Continue any existing report with the same name',
			'erase' => 'Erase any existing report with the same name (and do not re-run)'
		),
		'#description' => t('What should be done if a prior report with the same name exists?'),
	);

	$form['report_cache']['report_limit'] = array(
		'#type' => 'textfield',
		'#title' => 'Limit',
		'#size' => 10,
		'#description' => t('Limit the report generation to this many records.  (Usually useful for testing or previewing a report.)'),
	);

	$form['report_cache']['report_type'] = array(
		'#type' => 'hidden',
		'#value' => $report_type,
	);
}

function report_cache_form_submit($form, &$form_state) {
	$form_state['rebuild'] = 1;

	$report_type = $form_state['values']['report_type'];
	$report_id = $form_state['values']['report_id'];
	if (preg_match('/^\d\d\d\d\-\d\d\-\d\d/', $report_id)) {
		// Don't prefix the date, it's already there
	} else if ($report_id) {
		$report_id = date('Y-m-d H:i:s') . ' - ' . $report_id;
	} else {
		$report_id = date('Y-m-d H:i:s');
	}

	$form_state['values']['report_id'] = $report_id;

	$report_action = $form_state['values']['report_action'];
	if (!$report_action) { $report_action = 'replace'; }

	if ($report_action == 'replace' || $report_action == 'erase') {
		$stmt = db_delete('report_cache');
		$stmt->condition('report_id', $report_id);
		$stmt->condition('report_type', $report_type);
		$stmt->execute();
	}

	if ($report_action == 'erase') {
		drupal_set_message(t("The report named @report_id has been erased.", array('@report_id' => $report_id)));
		return;
	}

	$report_type_infos = module_invoke_all('report_cache_info');
	$report_type_info = $report_type_infos[$report_type];

	$pk_list_callback = $report_type_info['pk_list_callback'];
	if (!$pk_list_callback) {
		drupal_set_message(t('No pk_list_callback.'), 'error');
		return;
	}

	$pks = $pk_list_callback($form_state, $report_type, $report_id);
	$limit = (int) $form_state['values']['report_limit'];
	if ($limit) {
		$pks = array_slice($pks, 0, $limit);
	}

	$operations = array();
	$count = 0;
	$total = count($pks);
	foreach ($pks as $pk) {
		++$count;
		$operations[] = array(
			'report_cache_batch', 
			array($pk, $count, $total, $form_state['values'], $report_type_info),
		);
	}
	
	// Launch the batch processor.
  $batch = array(
    'operations' => $operations,
		'finished' => 'report_cache_batch_finished',
  );
	
	batch_set($batch);
	
	return;
}

function report_cache_batch($pk, $count, $total, $values, $report_type_info, &$context) {
	$report_type = $values['report_type'];
	$report_id = $values['report_id'];
	$report_action = $values['report_action'];

	if ($report_action == 'continue') {
		$stmt = db_query(
			'select count(*) as c from report_cache where report_id = :report_id and report_row_pk = :pk',
			array(':report_id' => $report_id, ':pk' => $pk)
		);
		$hr = $stmt->fetchAssoc();
		$count = $hr['c'];
		
		if ($count > 0) {
			$context['message'] = t('Skipping node @pk because its already been computed', array('@pk' => $pk));			
			return;
		}
	}
	
	$batch_callback = $report_type_info['batch_callback'];
	if (!$batch_callback) {
		drupal_set_message(t('No batch_callback.'), 'error');
		return;
	}

	$msg = t('Generating statement for batch @count / @total', array('@count' => $count, '@total' => $total));
	if (!is_array($pk)) {
		$msg .= ': ' . $pk;
	}
	$context['message'] = $msg;

	$row = $batch_callback($pk, $values, $report_id, $report_action, $context);

	if (!$row) {
		return;
	}
	
	// A "row" could be multiple.
	if (is_array($row) && $row['is_multiple']) {
		foreach ($row['rows'] as $sub_row_pk => $sub_row) {
		  $stmt = db_insert('report_cache');
			$stmt->fields(
				array(
					'report_id' => $report_id,
					'report_row_pk' => $sub_row_pk,
					'report_type' => $report_type,
					'report_data' => serialize($sub_row)
				)
			);
			$stmt->execute();
		}
	} else {
	  $stmt = db_insert('report_cache');
		$stmt->fields(
			array(
				'report_id' => $report_id,
				'report_row_pk' => $pk,
				'report_type' => $report_type,
				'report_data' => serialize($row)
			)
		);
		$stmt->execute();
	}
}

function report_cache_batch_finished($success, $results, $operations) {
	drupal_set_message("Report generated.");
}




function report_cache_run($report_type, $values = array()) {
	// Become user 1
	global $user;
	$original_user = $user;
	$old_state = drupal_save_session();
	drupal_save_session(FALSE);
	$user = user_load(1);

	watchdog('report-cache', "Running $report_type");

	$report_id = date('Y-m-d H:i:s');
	$report_action = 'replace';

	$values['report_id'] = $report_id;
	$values['report_action'] = $report_action;
	$values['report_type'] = $report_type;

	$report_type_infos = module_invoke_all('report_cache_info');
	$report_type_info = $report_type_infos[$report_type];

	$pk_list_callback = $report_type_info['pk_list_callback'];

	$form_state = array('values' => $values);
	$pks = $pk_list_callback($form_state, $report_type, $report_id);
	$limit = (int) $values['report_limit'];
	if ($limit) {
		$pks = array_slice($pks, 0, $limit);
	}

	$operations = array();
	foreach ($pks as $pk) {
		$context = array();
		report_cache_batch($pk, $values, $report_type_info, $context);
		entity_get_controller('node')->resetCache();		
		watchdog('report-cache', $context['message']);
	}
	
	$user = $original_user;
	drupal_save_session($old_state);
	return;
}