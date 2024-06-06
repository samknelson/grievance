<?php

function sirius_ledger_page_dev() {
	return drupal_get_form('sirius_ledger_page_dev_form');
}

function sirius_ledger_page_dev_form($form, &$form_state) {
	$form['#tree'] = TRUE;
	$form['action'] = array(
		'#type' => 'select',
		'#title' => t('Action'),
		'#options' => array(
			NULL => t('-- Select --'),
			'create_test_charges' => t('Create Test Charges'),
			'remove_test_charges' => t('Remove Test Charges'),
		),
		'#required' => TRUE,
	);

	$form['create'] = array(
		'#type' => 'fieldset',
		'#title' => t('Create Test Charges'),
		'#states' => array('visible' => array(":input[name=\"action\"]" => array('value' => 'create_test_charges'))),
		'#description' => t('(Remember that charges are positive and payments are negative)'),
	);

	$account_options = sirius_ledger_account_list();
	$form['create']['account'] = array(
		'#type' => 'select',
		'#title' => 'Account',
		'#options' => array(NULL => t('-- Select --')) + $account_options,
	);

	$form['create']['participant_nid'] = array(
		'#type' => 'textfield',
		'#title' => t('Participant NID'),
	);

	$form['create']['start_date'] = array(
		'#type' => 'textfield',
		'#title' => t('Start Date'),
	);

	$form['create']['end_date'] = array(
		'#type' => 'textfield',
		'#title' => t('End Date'),
	);

	$form['create']['min'] = array(
		'#type' => 'textfield',
		'#title' => t('Minimum Value'),
		'#size' => 4,
		'#default_value' => '10',
	);

	$form['create']['max'] = array(
		'#type' => 'textfield',
		'#title' => t('Maximum Value'),
		'#size' => 4,
		'#default_value' => '100',
	);

	$form['create']['count'] = array(
		'#type' => 'textfield',
		'#title' => t('Number of Charges'),
		'#size' => 4,
		'#default_value' => 50,
	);



	$form['remove'] = array(
		'#type' => 'fieldset',
		'#title' => t('Remove Test Charges'),
		'#states' => array('visible' => array(":input[name=\"action\"]" => array('value' => 'remove_test_charges'))),
	);

	$form['remove']['participant_nid'] = array(
		'#type' => 'textfield',
		'#title' => t('Participant NID'),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	$log = sirius_minilog_render_html();
	if ($log) { $form['results'] = array('#markup' => '<hr>' . $log); }

	return $form;
}

function sirius_ledger_page_dev_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;

	$action = $form_state['values']['action'];

	if ($action == 'remove_test_charges') {
		$participant_nid = $form_state['values']['remove']['participant_nid'];
		if (!$participant_nid) {
			drupal_set_message("Please enter a participant nid.", 'error');
			return;
		}

		$params = array();
		$params['ledger_participant'] = $participant_nid;
		$params['ledger_reference'] = sirius_ledger_dev_get_test_reference_nid(TRUE);
		$entries = sirius_ledger_ar_fetch($params);

		foreach ($entries as $entry) {
			sirius_ledger_ar_delete($entry);
			sirius_minilog("Purged: " . $entry['ledger_memo']);
		}
	}

	if ($action == 'create_test_charges') {
		$count = $form_state['values']['create']['count'];
		if ($count > 1000) { $count = 1000; }
		if ($count < 1) {
			drupal_set_message("Make sure that count > 0.", 'error');
			return;
		}
		sirius_minilog("Creating $count test charges.");

		$participant_nid = $form_state['values']['create']['participant_nid'];
		$participant_node = node_load($participant_nid);
		if (!$participant_node) { 
			drupal_set_message("Participant node not found ($participant_nid)", 'error');
			return;
		}
		sirius_minilog("Using particpant: $participant_nid");

		$reference_nid = sirius_ledger_dev_get_test_reference_nid(TRUE);
		sirius_minilog("Using reference NID reference_nid");

		$max = $form_state['values']['create']['max'];
		$min = $form_state['values']['create']['min'];

		$min_ts = strtotime($form_state['values']['create']['start_date']);
		$max_ts = strtotime($form_state['values']['create']['end_date']);
		if ( ($min_ts === NULL) || ($max_ts === NULL) || ($max_ts <= $min_ts) ) {
			drupal_set_message("Make sure start and end date are valid and that that the end date is after the start date.", 'error');
			return;
		}

		if ($max <= $min) {
			drupal_set_message("Make sure that max > min.", 'error');
			return;
		}

		for ($i=1; $i<$count+1; ++$i) {
			$amt = round(rand(100*$min, 100*$max) / 100, 2);
			$ts = rand($min_ts, $max_ts);
			sirius_minilog("Charge #$i: \$$amt on " . date('Y-m-d H:i:s', $ts));
			$uuid = uuid_generate();

			if ($amt < 0) {
				$memo = "Random test payment #$i / $count - $uuid";
			} else {
				$memo = "Random test charge #$i / $count - $uuid";
			}

			$entry = array();
			$entry['ledger_amount'] = $amt;
			$entry['ledger_status'] = 'Cleared';
			$entry['ledger_account'] = $form_state['values']['create']['account'];
			$entry['ledger_participant'] = $participant_nid;
			$entry['ledger_reference'] = $reference_nid;
			$entry['ledger_ts'] = $ts;
			$entry['ledger_memo'] = $memo;
			sirius_ledger_ar_insert($entry);
		}
	}

	drupal_set_message("Action complete.");
}

function sirius_ledger_dev_get_test_reference_nid($create = FALSE) {
	$info = sirius_cron_scan_info($key);
	$type = "ledger:test";

	$domain_nid = sirius_domain_nid();
	if (!$domain_nid) { $domain_nid = 0; }

  $sql = "select nid ";
  $sql .= "from node ";
  $sql .= "left join field_data_field_sirius_type on field_data_field_sirius_type.entity_type = 'node' and field_data_field_sirius_type.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
  $sql .= "where node.type = 'sirius_json_definition' ";
  $sql .= "and field_sirius_type_value = :type ";
  $sql_args[':type'] = $type;
  $sql .= "and field_sirius_domain_target_id = :domain_nid ";
  $sql_args[':domain_nid'] = $domain_nid;
  $sql .= "order by created desc limit 1";
  $stmt = db_query($sql, $sql_args);
  $hr = $stmt->fetchAssoc();
  if ($hr['nid']) { return $hr['nid']; }

	if (!$create) { return NULL; }

	global $user;
  $node = new stdClass();
  $node->type = 'sirius_json_definition';
  $node->title = 'TEST LEDGER PARTICIPANT';
  $node->no_recurse = TRUE;
  $node->language = LANGUAGE_NONE;
  $node->uid = $user->uid;
  $node->status = 1;
  $node->promote = 0;
  $node->comment = 0;
  $node->field_sirius_type['und'][0]['value'] = $type;
  $node->field_sirius_domain['und'][0]['target_id'] = $domain_nid;
  $node = node_submit($node);
  node_object_prepare($node);
  node_save($node);
  return $node->nid;
}