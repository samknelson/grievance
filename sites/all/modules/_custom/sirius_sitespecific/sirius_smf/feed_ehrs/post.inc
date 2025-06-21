<?php

function sirius_smf_feed_ehrs_post_form($form, &$form_state, $info, $data, $wizard_node, $current_step) {
	$pk = $data['pk'];
	$fields = sirius_feed_wizard_fields($info, $data, $wizard_node, $current_step);

	$total_hrs = 0;
	foreach ($data['records']['rows'] as $data_row) {
		$total_hrs += $data_row['hrs']; 
	}

	$html = '';

	$rows = array();
	foreach ($data['records']['rows'] as $data_row) {
		$row = array();
		$row[] = $data_row[$pk];
		$row[] = $data_row['name_first'];
		$row[] = $data_row['name_last'];
		$row[] = $data_row['hour_type_id'];
		$row[] = $data_row['hrs'];
		$rows[] = $row;
	}

	if ($rows) {
		$total_row = array();
		$total_row[] = '<strong>' . t('Total') . '</strong>';
		$total_row[] = '';
		$total_row[] = '';
		$total_row[] = '';
		$total_row[] = '<strong>' . number_format($total_hrs, 2) . '</strong>';
		$rows[] = $total_row;

		$html .= '<p>';
		$html .= t('The following workers will be posted to the system. The total number of hours that will be posted is <strong>@total_hrs</strong>.', array('@total_hrs' => number_format($total_hrs, 2)));
		$html .= '</p>';


		$header = array();
		$header[] = $fields[$pk]['title'];
		$header[] = t('First Name');
		$header[] = t('Last Name');
		$header[] = t('Status');
		$header[] = t('Hours This Month');

		$html .= sirius_table_render($header, $rows);
		$form['intro'] = array('#markup' => $html);
		$form['confirm'] = array(
			'#type' => 'checkbox',
			'#title' => t('Yes, the data above is correct.'),
			'#required' => TRUE,
		);
	} else {
		$html = '<p>' . t('There are no workers hours to post on this upload.') . '</p>';
		$form['intro'] = array('#markup' => $html);
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Post Hours and Generate Charges',
	);

	return sirius_smf_feed_ehrs_form_prep($form, $form_state, $info, $data, $wizard_node, $current_step);
}

function sirius_smf_feed_ehrs_post_form_validate($form, &$form_state) {
	$data = $form_state['data'];
	if (!$data['records']) { form_set_error('intro', 'No records found.'); }
}

function sirius_smf_feed_ehrs_post_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$wizard_node = $form_state['wizard_node'];
	$current_step = $form_state['current_step'];

	$operations = array();
	foreach ($data['records']['rows'] as $delta => $row) {
		$operations[] = array('sirius_smf_feed_ehrs_post_form_submit_batch', array($wizard_node, $info, $data, $current_step, $delta));
	}
	$operations[] = array('sirius_smf_feed_ehrs_post_form_submit_complete', array($wizard_node, $info, $data, $current_step));
	
	$batch = array('operations' => $operations, 'file' => drupal_get_path('module', 'sirius_smf') . '/feed_ehrs/post.inc');
	batch_set($batch);
}

function sirius_smf_feed_ehrs_post_form_submit_complete($wizard_node, $info, $data, $current_step, &$context) {
	drupal_set_message("Hour upload saved.");
	sirius_wizard_submit($info, $data, $wizard_node, $current_step, "Hours upload has been confirmed, and charges have been generated.", array('nonsequential' => FALSE, 'feed_status' => 'complete'));
}

function sirius_smf_feed_ehrs_post_form_submit_batch($wizard_node, $info, $data, $current_step, $delta, &$context) {
	// Load all our include files
	sirius_require('sirius_smf', 'sirius_smf.ehrs.inc');
	sirius_smf_feed_ehrs_load_includes();

	// Don't denormalize in realtime
	sirius_denorm_realtime(FALSE);


	$row = $data['records']['rows'][$delta];

	$eup_node = sirius_smf_feed_ehrs_get_eup($wizard_node);
	if (!$eup_node) {
		drupal_set_message("This feed is not associated with an employer upload.", 'error');
		return;
	}

	$eup = Sirius_Smf_Ehrs_Monthly::getInstance();
	$year = $eup->get_year($eup_node);
	$month = $eup->get_month($eup_node);
	$day = 1;
	$employer_node = $eup->get_employer($eup_node);
	$employer_nid = $employer_node->nid;
	if (!$year || !$month || !$employer_nid) {
		drupal_set_message("Unable to find the year, the month, or the employer. Please check the employer upload node.", 'error');
		return;
	}
	$domain_nid = sirius_domain_nid();

	// 
	// First create any new workers
	//

	$worker_node = sirius_smf_feed_ehrs_worker_lookup($wizard_node, $info, $data, $row);

	$is_new = FALSE;
	if (!$worker_node) {
		$is_new = TRUE;
		
		// Create the worker node
	  global $user;
	  $worker_node = new stdClass();
	  $worker_node->type = 'sirius_worker';
	  $worker_node->language = LANGUAGE_NONE;
	  $worker_node->uid = $user->uid;
	  $worker_node->status = 1;
	  $worker_node->promote = 0;
	  $worker_node->comment = 0;

	  // Set the domain
	  $worker_node->field_sirius_domain['und'][0]['target_id'] = $domain_nid;
	  $worker_node->field_sirius_ssn['und'][0]['value'] = $row['ssn'];
	  $worker_node->field_sirius_name['und'][0]['given'] = $row['name_first'];
	  $worker_node->field_sirius_name['und'][0]['family'] = $row['name_last'];
	  if (isset($row['middle_name'])) {
	  	$worker_node->field_sirius_name['und'][0]['middle'] = $row['name_middle'];
	  }
	  if ($row['phone']) { $worker_node->field_sirius_phone['und'][0]['value'] = sirius_phone_format($row['phone']); }
		if ($row['email']) { $worker_node->field_sirius_email['und'][0]['value'] = sirius_email_format($row['email']); }
		if ($row['dob']) { $worker_node->field_sirius_dob['und'][0]['value'] = $row['dob'] . " 00:00:00"; }

	  if ($row['addr_thoroughfare'] && $row['addr_locality'] && $row['addr_administrative_area'] && $row['addr_postal_code']) {
	  	$worker_node->field_sirius_address['und'][0] = array(
	  		'thoroughfare' => $row['addr_thoroughfare'],
	  		'locality' => $row['addr_locality'],
	  		'administrative_area' => $row['addr_administrative_area'],
	  		'postal_code' => $row['addr_postal_code'],
	  		'premise' => $row['addr_premise'],
	  		'country' => 'US',
	  	);
	  }

	  node_object_prepare($worker_node);
		node_save($worker_node);
	}

	$worker_nid =  $worker_node->nid;

	$context['message'] = "Processing: " . $worker_node->title;

	// 
	// Queue any name, address, or phone changes
	// 

	sirius_require('sirius_trust', 'sirius_trust.changequeue.inc');
	$cq = Sirius_Trust_Changequeue::getInstance();
	$item = array();
	$item['type'] = 'node_field';
	$item['msg'] = $eup_node->title;
	$item['handler_nids'] = array($eup_node->nid);
	$item['node_field'] = array();
	$item['node_field']['nid'] = $worker_node->nid;
	$item['node_field']['field_name'] = 'field_sirius_name';
	$item['node_field']['val'] = array(
		'given' => $row['name_first'],
		'family' => $row['name_last'],
		'middle' => $row['name_middle'],
	);
	$result = SiriusResult::create($cq->queue($item));
	if (!$result->success()) { $result->drupalSetMessage(); }

	if ($row['phone']) {
		$item = array();
		$item['type'] = 'node_field';
		$item['msg'] = $eup_node->title;
		$item['handler_nids'] = array($eup_node->nid);
		$item['node_field'] = array();
		$item['node_field']['nid'] = $worker_node->nid;
		$item['node_field']['field_name'] = 'field_sirius_phone';
		$item['node_field']['val'] = array('value' => $row['phone']);
		$result = SiriusResult::create($cq->queue($item));
		if (!$result->success()) { $result->drupalSetMessage(); }
	}

	if ($row['email']) {
		$item = array();
		$item['type'] = 'node_field';
		$item['msg'] = $eup_node->title;
		$item['handler_nids'] = array($eup_node->nid);
		$item['node_field'] = array();
		$item['node_field']['nid'] = $worker_node->nid;
		$item['node_field']['field_name'] = 'field_sirius_email';
		$item['node_field']['val'] = array('value' => $row['email']);
		$result = SiriusResult::create($cq->queue($item));
		if (!$result->success()) { $result->drupalSetMessage(); }
	}

  if ($row['addr_thoroughfare'] && $row['addr_locality'] && $row['addr_administrative_area'] && $row['addr_postal_code']) {
		$item = array();
		$item['type'] = 'node_field';
		$item['msg'] = $eup_node->title;
		$item['handler_nids'] = array($eup_node->nid);
		$item['node_field'] = array();
		$item['node_field']['nid'] = $worker_node->nid;
		$item['node_field']['field_name'] = 'field_sirius_address';
		$item['node_field']['val'] = array(
  		'thoroughfare' => $row['addr_thoroughfare'],
  		'locality' => $row['addr_locality'],
  		'administrative_area' => $row['addr_administrative_area'],
  		'postal_code' => $row['addr_postal_code'],
  		'premise' => $row['addr_premise'],
  		'country' => 'US',
  	);
		$result = SiriusResult::create($cq->queue($item));
		if (!$result->success()) { $result->drupalSetMessage(); }
	}

  if ($row['dob']) {
		$item = array();
		$item['type'] = 'node_field';
		$item['msg'] = $eup_node->title;
		$item['handler_nids'] = array($eup_node->nid);
		$item['node_field'] = array();
		$item['node_field']['nid'] = $worker_node->nid;
		$item['node_field']['field_name'] = 'field_sirius_dob';
		$item['node_field']['val'] = $worker_node->field_sirius_dob['und'][0];
		$item['node_field']['val']['value'] = $row['dob'] . " 00:00:00";
		$result = SiriusResult::create($cq->queue($item));
		if (!$result->success()) { $result->drupalSetMessage(); }
	}

	// 
	// Generate hours entries
	// 

	$dept_tid = sirius_taxonomy_lookup_by_anything('grievance_department', 'default');
  if (!$dept_tid) {
  	drupal_set_mesasge("No department found with ID='default'.", 'error');
  	return;
  }

	$payperiod_node = sirius_hour_payperiod_load($domain_nid, $worker_nid, $employer_nid, $year, $month, $day, TRUE);
	$payperiod_json = sirius_json_get($payperiod_node);

	$payperiod_json['entries'] = array();
	$payperiod_json['entries']['upload']['source_name'] = 'Employer Upload';
	$payperiod_json['entries']['upload']['status'] = 'live';

	$entry = array();
	$entry['day'] = 1;
	$entry['department'] = $dept_tid;
	$entry['hour_type'] = $row['hour_type'];
	$entry['qty'] = $row['hrs'];
	$entry['note'] = '';
	$entry['comment'] = '';

	$payperiod_json['entries']['upload']['rows'] = array($entry);

	$payperiod_node->field_sirius_active['und'][0]['value'] = 'Yes';
	sirius_hour_payperiod_compute($payperiod_json);
	sirius_json_set($payperiod_node, $payperiod_json);
	node_save($payperiod_node);
}
