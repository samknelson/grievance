<?php

function sirius_smf_feed_ehrs_verify_form($form, &$form_state, $info, $data, $feed_node, $current_step) {
	$form_state['info'] = $info;
	$form_state['data'] = $data;
	$form_state['feed_nid'] = $feed_node->nid;
	$form_state['current_step'] = $current_step;

  $access = Sirius_Smf_Ehrs_Monthly_Access::getInstance();
  if ($access->access('staff', array())) {
  	$form['ignore_errors'] = array(
  		'#type' => 'select',
  		'#title' => t('ADMIN ONLY: Ignore Errors'),
  		'#options' => array(
  			NULL => t('-- Select --'),
  			'No' => t('No'),
  			'Yes' => t('Yes'),
  		),
  	);
  }

	if ($form_state['errors']) {
		$rows = array();
		foreach ($form_state['errors'] as $key => $record) {
			$row = array();
			$row[] = $record['xxxrowcountxxx'];
			$row[] = $record['ssn'];
			$row[] = $record['name_first'];
			$row[] = $record['name_last'];
			$row[] = $record['xxxmsgxxx'];
			$rows[] = $row;
		}
		$header = array();
		$header[] = t('Row Number');
		$header[] = t('SSN');
		$header[] = t('First Name');
		$header[] = t('Last Name');
		$header[] = t('Error');
		$form['errors'] = array('#markup' => sirius_table_render($header, $rows));
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Parse and Verify Upload',
	);

	return sirius_smf_feed_ehrs_form_prep($form, $form_state, $info, $data, $feed_node, $current_step);
}

function sirius_smf_feed_ehrs_verify_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$feed_node = node_load($form_state['feed_nid']);
	$current_step = $form_state['current_step'];

	// Load the spreadsheet
	$reader = sirius_feed_spout_get_reader($feed_node);
	$sheet = sirius_feed_spout_get_sheet($reader, $data['worksheet']);
	if (!$sheet) { 
		drupal_set_message("Unable to load the spreadsheet", 'error');
		return;
	}
	$header = sirius_feed_spout_get_header($sheet, $data['header_row']);
	$fields = sirius_feed_wizard_fields($info, $data, $feed_node, $current_step);

	// Get our data
	$count = 0;
	$import_data = array();
	$counts = $warning_counts = array();
	$callback = $info['feed']['callback_row_verify'];
	$verify_limit = $form_state['values']['verify_limit'];
	$precompute = TRUE;

	$data['records'] = array('rows' => array());
	unset($data['nodupe']);
	$form_state['errors'] = array();

	$already_seen = array();
	foreach ($sheet->getRowIterator() as $row) {
		++$count;
		if ($count == 1 && $data['header_row'] == 'Yes') { continue; }
		$import_data_row = sirius_feed_spout_get_import_row($fields, $row, $data, $count, $header);

		$result = $callback($feed_node, $info, $data, $import_data_row, $precompute);

		if ($result['success']) {
			$data['records']['rows'][] = $import_data_row;
		} else {
			$import_data_row['xxxmsgxxx'] = $result['msg'];
			$form_state['errors'][] = $import_data_row;
		}
	}
	$data['rows'] = $count + 1;
	unset($data['nodupe']);

	if ($form_state['values']['ignore_errors'] != 'Yes') {
		if ($form_state['errors']) {
			drupal_set_message("The uploaded file could not be parsed. Please correct the errors below before continuing.", 'warning');
			$form_state['rebuild'] = TRUE;
			return;
		}
	}

	sirius_wizard_submit($info, $data, $feed_node, $current_step, "Upload parsed.", array('feed_status' => 'draft'));
}

function sirius_smf_feed_ehrs_worker_lookup($feed_node, $info, $data, $row) {
	$pk = $data['pk'];
  if ($pk == 'ssn') {
	  return sirius_worker_get_by_ssn($row['ssn']);
	} else if ($pk == 'ein') {
		$eup_node = sirius_smf_feed_ehrs_get_eup($feed_node);
		if (!$eup_node) { 
			drupal_set_message("This feed is not associated with an employer upload.", 'error');
			return NULL;
		}

		$eup = Sirius_Smf_Ehrs_Monthly::getInstance();
		$employer_node = $eup->get_employer($eup_node);
		$employer_nid = $employer_node->nid;

		$worker_nid = sirius_worker_ein_lookup($employer_nid, $row['ein']);
		return node_load($worker_nid);
	} else {
		drupal_set_message("Unknown pk: $pk");
		return NULL;
	}
}

function sirius_smf_feed_ehrs_row_verify($feed_node, $info, &$data, &$row, $precompute) {
  static $rowcount;
  $rowcount++;

  // Get the employer node
	$eup_node = sirius_smf_feed_ehrs_get_eup($feed_node);
	if (!$eup_node) { return array('success' => FALSE, 'msg' => "This feed is not associated with an employer upload."); }
	$eup = Sirius_Smf_Ehrs_Monthly::getInstance();
	$employer_node = $eup->get_employer($eup_node);

  // Trim all
  foreach ($row as $key => $value) {
    if (!is_array($value)) { $row[$key] = trim($value); }
  }

  if (isset($row['ssn']) && isset($row['ein'])) {
  	return array('success' => FALSE, 'msg' => 'Uploads cannot contain both an SSN and an EIN.');
  }

  if (isset($row['ssn'])) {
  	while (strlen($row['ssn']) < 9) { $row['ssn'] = '0' . $row['ssn']; }
  }

  $errors = array();

  $pk = $data['pk'];

  // Required fields
  if ($pk == 'ssn') {
	  if (!$row['ssn']) { $errors[] = 'Please enter an SSN.'; }
	  else if (!sirius_ssn_validate($row['ssn'])) { $errors[] = 'Please enter a valid SSN.'; }
	}

	if ($pk == 'ein') {
	  if (!$row['ein']) { $errors[] = 'Please enter an EIN.'; }
	}

  if (!$row['name_first']) { $errors[] = 'Please enter a first name.'; }
  if (!$row['name_last']) { $errors[] = 'Please enter a last name.'; }

  // Validate fields
	$fields = $info['feed']['callback_fields']($info, $info, $data, $feed_node, $current_step);

  foreach ($fields as $field_name => $field_info) {
    if (!$row[$field_name]) { continue; }
   	$result = sirius_feed_handle_field($field_name, $field_info, $row[$field_name], $precompute, $employer_node->nid);
    if (!$result['success']) { $errors[] = $result['msg']; }
  }

  // Validate the hours
  $hrs = $row['hrs'];
  if (!is_numeric($hrs)) { $errors[] = 'Please enter a numeric for hours.'; }
  if ($hrs < 0) { $errors[] = 'Please enter a positive value for hours.'; }

  // Look up the worker
  $worker_node = sirius_smf_feed_ehrs_worker_lookup($feed_node, $info, $data, $row);
  if ($worker_node) { $row['worker_nid'] = $worker_node->nid; }

  if ($errors) { return array('success' => FALSE, 'msg' => join('<br>', $errors)); }

  // Get the type ID
  $hour_type_term = taxonomy_term_load($row['hour_type']);
  if (!$hour_type_term) { $errors[] = 'Please enter a value for employment status.'; }
  $row['hour_type_id'] = $hour_type_term->field_sirius_id['und'][0]['value'];

  // Terminated workers must have 0 hours
  /*
  if ($row['hour_type_id'] == 'terminated' && $row['hrs'] > 0) {
  	$errors[] = 'For terminated workers, hours must be 0.';
	}
	*/

	/*
	// FMLA and disability workers have the number of hours automatically set to their threshold
	if (in_array($row['hour_type_id'], array('fmla', 'disability'))) {
		//
		// @todo: The threshold logic is really ugly. Technically, we ought to:
		// 		1. Find their election.
		// 		2. Find the policy from their election.
		// 		3. Find all their benefits from the election.
		// 		4. Figure out which of the benefits has the highest threshold configured in the poilcy
		// 		5. Compare it with the worker's member status for "worst threshold"
		// 		But it's too silly. So:

		$plugin_info = sirius_trust_eligibility_plugin_info('smf-buildup');
		$threshold = $plugin_info['instance']->threshold_from_worker_and_employer($worker_node, $employer_node);
		if ($threshold > $row['hrs']) { $row['hrs'] = $threshold; }
	}
	*/

  // For disability, we need to look back in time and find out when they started.
  if ($row['hour_type_id'] == 'disability') {
  	if ($worker_node) {
  		// We're going to look up FMLA and disability. For now this is kind of hardcoded, looking at the previous month's autotags.

			// Get the EUP
			$eup_node = sirius_smf_feed_ehrs_get_eup($feed_node);

			// Get the month and year
			$monthly = Sirius_Smf_Ehrs_Monthly::getInstance();
			$year = $monthly->get_year($eup_node);
			$month = $monthly->get_month($eup_node);

			--$month;
			if ($month < 1) { $month = 12; --$year; }

			// Get FM
			$fmla_tid = sirius_taxonomy_lookup_by_anything('sirius_contact_tags', 'smf_autotag_status_fmla');
			if (!$fmla_tid) { return array('success' => FALSE, 'msg' => 'System error: please define an FMLA autotag.'); }
			$disability_tid = sirius_taxonomy_lookup_by_anything('sirius_contact_tags', 'smf_autotag_status_disability');
			if (!$disability_tid) { return array('success' => FALSE, 'msg' => 'System error: please define a disability autotag.'); }

			// Fetch the WYM node
			$wym_node = sirius_smf_wym_fetch($worker_node->nid, $year, $month, FALSE);
			if ($wym_node) {
				$all_tids = sirius_fieldvals($wym_node, 'field_sirius_contact_tags', 'tid');

				if (in_array($disability_tid, $all_tids)) {
					$row['disability_approved'] = array(
						'approved' => TRUE,
						'msg' => 'Emmployee was on disability in the preceding month.',
					);
				}

				if (in_array($fmla_tid, $all_tids)) {
					$row['disability_approved'] = array(
						'approved' => TRUE,
						'msg' => 'Emmployee was on fmla in the preceding month.',
					);
				}
			}
		}
  }

  // Check for "already seen"
  if (!$data['nodupe']) { $data['nodupe'] = array(); }
  foreach (array('ssn', 'ein') as $key) {
	  if ($row[$key]) {
	  	if ($data['nodupe'][$key][$row[$key]]) {
	  		$errors[] = "Multiple records found in the upload file with this $key.";
	  	} else {
	  		$data['nodupe'][$key][$row[$key]] = TRUE;
	  	}
	  }
  }

  // If there is no EIN, then all fields are required.
  if (!$row['worker_nid']) {
  	if (!$row['dob']) { $errors[] = "For new workers, DoB is required."; }
  	if (!$row['phone']) { $errors[] = "For new workers, phone number is required."; }
  	if (!$row['ssn']) { $errors[] = "For new workers, SSN is required."; }

		foreach (array('addr_thoroughfare', 'addr_locality', 'addr_administrative_area', 'addr_postal_code') as $key) {
			if (!$row[$key]) { $errors[] = "For new workers, a complete address is required."; break; }
		}
		foreach (array('name_first', 'name_last') as $key) {
			if (!$row[$key]) { $errors[] = "For new workers, a full name is required."; break; }
		}
  }

  // Validate Zip
  if ($row['addr_postal_code'] && !sirius_addr_zip_validate($row['addr_postal_code'])) {
  	$errors[] = "Zip code must be a valid five or nine digit zip code.";
  }

  // Validate state
  if ($row['addr_administrative_area'] && !sirius_addr_state_code_validate($row['addr_administrative_area'])) {
  	$errors[] = "State must be a valid two-letter code (e.g., CA, PA, MD).";
  }

  // Validate city
  if ($row['addr_locality'] && !sirius_addr_city_validate($row['addr_locality'])) {
  	$errors[] = "City cannot contain numbers or non-ASCII characters.";
  }

  // No bad characters
	foreach (array('addr_thoroughfare', 'addr_locality', 'addr_administrative_area', 'addr_postal_code') as $key) {
		if (!$row[$key]) { continue; }
		if (!preg_match('/^[\x20-\x7E]*$/u', $row[$key])) { $errors[] = "Please ensure that the address has only printable ASCII characters."; }
		if (preg_match('/\?/', $row[$key])) { $errors[] = "Please ensure that the address has only printable ASCII characters."; }
	}

  if ($errors) { return array('success' => FALSE, 'msg' => join('<br>', $errors)); }

  return array('success' => TRUE);
}