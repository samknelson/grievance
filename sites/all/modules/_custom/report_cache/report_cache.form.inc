<?php

function report_cache_parameters_form_callback_empty($report_type) {
	return array();
}

function report_cache_generate_form($form, &$form_state, $report_type) {
  $info = report_cache_info($report_type);

  report_cache_form_prepare($report_type, $form, $form_state);

  // "parameters_form_callback" is legacy. We also support "instance", and call the method "form".
	$form['parameters'] = $info['generator']->form();

  if ($form['parameters']) {
	  $instance_options = report_cache_instances_list($report_type);
	  if ($instance_options) {
	  	$form['instance'] = array(
	  		'#title' => t('Load Instance'),
	  		'#type' => 'select',
	  		'#options' => array(NULL => t('-- Select --')) + $instance_options,
				'#ajax' => array(
			    'callback' => 'report_cache_generate_form_callback_replace',
			    'method' => 'replace',
				),
				'#limit_validation_errors' => array(),
	  	);
	  }
  	if (isset($form_state['input']['instance'])) {
  		$uuid = $form_state['input']['instance'];
  		$instance = report_cache_instance($report_type, $uuid);
  		$form_state['input']['parameters'] = $instance['xxxrawinputxxx']['parameters'];
  	}
  	$form['parameters']['#type'] = 'fieldset';
  	$form['parameters']['#tree'] = TRUE;
  	$form['parameters']['#title'] = t('Parameters');
  	$form['parameters']['#prefix'] = '<div id=\'report_cache_generate_form_parameters\'>';
  	$form['parameters']['#suffix'] = '</div>';
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Generate',
  );

  return $form;
}

function report_cache_generate_form_validate($form, &$form_state) {
	$report_type = $form_state['values']['report_type'];
  $info = report_cache_info($report_type);
	$info['generator']->form_validate($form, $form_state);
}

function report_cache_generate_form_callback_replace($form, $form_state) {
  $commands = array();
  $commands[] = ajax_command_replace("#report_cache_generate_form_parameters", render($form['parameters']));
  return array(
    '#type' => 'ajax',
    '#commands' => $commands
  );
}

function report_cache_generate_form_submit($form, &$form_state) {
  return report_cache_form_submit($form, $form_state);
}

// Add some fields to a cached report generation form
function report_cache_form_prepare($report_type, &$form, &$form_state) {

	$form['report_cache'] = array(
    '#type' => 'fieldset',
		'#title' => t('Report Cache'),
		'#attributes' => array('class' => array('fill-wrapper')),
		'#collapsed' => TRUE, 
		'#collapsible' => TRUE,
  );


	/*
	$form['report_cache']['report_intro'] = array(
		'#type' => 'markup',
		'#markup' => 
			t('Use the form below to re-generate the report.  The results from previous runs may be seen ') .
			'<strong>' . 
			l(t('here'), "sirius/reports/$report_type") .
			'</strong>',
	);
	*/
	
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

	/*
	if (!$form['submit']) {
	  $form['submit'] = array(
	    '#type' => 'submit',
	    '#value' => 'Generate',
	    '#weight' => 10,
	  );
	}
	*/
}

function report_cache_form_submit($form, &$form_state) {
	$form_state['rebuild'] = 1;
	$domain_nid = sirius_domain_nid(); if (!$domain_nid) { $domain_nid = 0; }

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
		$stmt->condition('report_domain_nid', $domain_nid);
		$stmt->execute();
	}

	if ($report_action == 'erase') {
		drupal_set_message(t("The report named @report_id has been erased.", array('@report_id' => $report_id)));
		return;
	}

	$info = report_cache_info($report_type);
	if ($info['file']) { sirius_require($info['module'], $info['file']); }

	$pks = $info['generator']->pks($form_state, $report_type, $report_id);

	$limit = (int) $form_state['values']['report_limit'];
	if ($limit) {
		$pks = array_slice($pks, 0, $limit);
	}

	$parameters = $form_state['values'];
	global $user;
	$parameters['meta'] = array(
		'user' => array(
			'uid' => $user->uid,
			'name' => $user->name,
			'mail' => $user->mail,
		),
		'ts' => array(
			'started' => time(),
		),
		'instance' => array(
			'uuid' => $form_state['values']['instance'],
		),
	);
	report_cache_parameters($report_type, $report_id, $parameters);
	$total = count($pks);
	$count = 0;

	if ($form_state['values']['nobatch']) {
		//
		// Skip batch processing. We still run in chunks, but we simply run each chunk manually. Typically for a command line or auto-run background
		//

		$context = array();
		$context['results']['report_id'] = $report_id;
		foreach ($pks as $pk) {
			++$count;
			report_cache_batch($pk, $count, $total, $form_state['values'], $info, $context);
			entity_get_controller('node')->resetCache();
		}
		report_cache_batch_finished(TRUE, $context['results'], array());

	} else {
		//
		// Use batch processing. Typically for browser-based interactive
		//

		$operations = array();
		$operations[] = array(
			'report_cache_batch_setup',
			array($form_state['values']),
		);
		foreach ($pks as $pk) {
			++$count;
			$operations[] = array(
				'report_cache_batch', 
				array($pk, $count, $total, $form_state['values'], $info),
			);
		}

		// Launch the batch processor.
	  $batch = array(
	    'operations' => $operations,
			'finished' => 'report_cache_batch_finished',
			'url_options' => array('query' => array('a' => 'b')),
	  );
	  if ($info['file']) {
	  	$batch['file'] = drupal_get_path('module', $info['module']) . '/' . $info['file'];
	  }
		
		batch_set($batch);
	}
		
	return;
}

function report_cache_batch_setup($values, &$context) {
	$report_type = $values['report_type'];
	$report_id = $values['report_id'];
	$report_action = $values['report_action'];

	$context['results']['report_type'] = $report_type;
	$context['results']['report_id'] = $report_id;
	$context['results']['report_action'] = $report_action;
	$context['results']['count'] = 0;
	$context['results']['noninteractive'] = $values['noninteractive'];
	$context['results']['values'] = $values;
}

function report_cache_batch($pk, $count, $total, $values, $info, &$context) {
	$domain_nid = sirius_domain_nid(); if (!$domain_nid) { $domain_nid = 0; }

	$report_type = $values['report_type'];
	$report_id = $values['report_id'];
	$report_action = $values['report_action'];

	$context['results']['count']++;

	if ($report_action == 'continue') {
		$stmt = db_query(
			'select count(*) as c from report_cache where report_id = :report_id and report_row_pk = :pk and report_domain_nid = :domain_nid',
			array(':report_id' => $report_id, ':pk' => $pk, ':domain_nid' => $domain_nid)
		);
		$hr = $stmt->fetchAssoc();
		$count = $hr['c'];
		
		if ($count > 0) {
			$context['message'] = t('Skipping node @pk because its already been computed', array('@pk' => $pk));			
			return;
		}
	}
	
	$msg = t('Generating statement for batch @count / @total', array('@count' => $count, '@total' => $total));
	if (!is_array($pk)) {
		$msg .= ': ' . $pk;
	}
	$context['message'] = $msg;

	$row = $info['generator']->batch($pk, $values, $context);
	if (!$row) { return; }

	$instance_uuid = $values['instance'];
	
	// A "row" could be multiple.
	if (is_array($row) && $row['is_multiple']) {
		foreach ($row['rows'] as $sub_row_pk => $sub_row) {
		  $stmt = db_insert('report_cache');
			$stmt->fields(
				array(
					'report_id' => $report_id,
					'report_row_pk' => $sub_row_pk,
					'report_type' => $report_type,
					'report_data' => $info['generator']->row_encode($sub_row),
					'report_domain_nid' => $domain_nid,
					'report_instance' => $instance_uuid,
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
				'report_data' => $info['generator']->row_encode($row),
				'report_domain_nid' => $domain_nid,
				'report_instance' => $instance_uuid,
			)
		);
		$stmt->execute();
	}
}

function report_cache_batch_finished($success, $results, $operations) {
	$report_type = $results['report_type'];
	$report_id = $results['report_id'];
	if ($report_id) {
		$parameters = report_cache_parameters($report_type, $report_id);
		$parameters['meta']['ts']['completed'] = time();
		$parameters['meta']['ts']['duration'] = $parameters['meta']['ts']['completed'] - $parameters['meta']['ts']['started'];
		$parameters['meta']['dashboard'] = $results['dashboard'];
		report_cache_parameters($report_type, $report_id, $parameters);
	}

	if ($results['count']) {
		$report_type = $results['report_type'];
		$count = $results['count'];

		if (!$results['noninteractive']) {
			$msg = $results['values']['success_msg'];
			if (!$msg) { 
				$msg = t('The report has been generated and may be viewed ') . l(t('here.'), "sirius/reports/$report_type");
			}
			drupal_set_message($msg);
		}
	} else {
		if (!$results['noninteractive']) {
			drupal_set_message(t('No records found.'));
		}
	}

	if ($results['values']['redirect']) {
		drupal_goto($results['values']['redirect']['path'], array('query' => $results['values']['redirect']['query']));
	}
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

	$infos = module_invoke_all('report_cache_info');
	$info = $infos[$report_type];

	$pk_list_callback = $info['pk_list_callback'];

	$form_state = array('values' => $values);
	$pks = $pk_list_callback($form_state, $report_type, $report_id);
	$limit = (int) $values['report_limit'];
	if ($limit) {
		$pks = array_slice($pks, 0, $limit);
	}

	$operations = array();
	foreach ($pks as $pk) {
		$context = array();
		report_cache_batch($pk, $values, $info, $context);
		entity_get_controller('node')->resetCache();		
		watchdog('report-cache', $context['message']);
	}
	
	$user = $original_user;
	drupal_save_session($old_state);
	return;
}

