<?php

function sirius_ledger_node_tab_employer_transmittal($employer_node) {
	$domain_nid = sirius_domain_nid();

	$action = sirius_node_tab_arg(0);
	$transmittal_nid = sirius_node_tab_arg(1);

	if ($transmittal_nid) {
		$transmittal_node = node_load($transmittal_nid);
		if (!$transmittal_node) { return 'Transmittal form not found.'; }
		if ($transmittal_node->field_sirius_domain['und'][0]['target_id'] != $domain_nid) { return 'Transmittal form not in our domain.'; }
		if ($transmittal_node->type != 'sirius_log') { return 'Not a log entry.'; }
		if ($transmittal_node->field_sirius_category['und'][0]['value'] != 'ledger:transmittal') { return 'Not a transmittal form.'; }
		if ($transmittal_node->field_sirius_log_handler['und'][0]['target_id'] != $employer_node->nid) { return 'Transmittal form is not associated with this employer.'; }
	}

	if ($action == 'print') { return drupal_get_form('sirius_ledger_node_tab_employer_transmittal_print_form', $employer_node); }
	if ($action == 'add') { return drupal_get_form('sirius_ledger_node_tab_employer_transmittal_addedit_form', $employer_node);}
	if ($action == 'delete' && $transmittal_node) { return drupal_get_form('sirius_ledger_node_tab_employer_transmittal_delete_form', $employer_node, $transmittal_node);}
	if ($action == 'edit' && $transmittal_node) { return drupal_get_form('sirius_ledger_node_tab_employer_transmittal_addedit_form', $employer_node, $transmittal_node);}

	return sirius_ledger_node_tab_employer_transmittal_list($employer_node);
}

function sirius_ledger_node_tab_employer_transmittal_list($employer_node) {
	$domain_nid = sirius_domain_nid();
	// $html = sirius_ledger_node_tab_employer_transmittal_quicknav($employer_node);
	$accounts = sirius_ledger_account_list();

  $domain_nid = sirius_domain_nid();
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_log');
  if ($domain_nid) { $query->fieldCondition('field_sirius_domain', 'target_id', $domain_nid); }
  $query->fieldCondition('field_sirius_category', 'value', 'ledger:transmittal');
  $query->fieldCondition('field_sirius_log_handler', 'target_id', $employer_node->nid);
  $query->propertyOrderBy('nid');
  $stmt = $query->execute();
  if ($stmt['node']) {
  	$html .= '<h2>Transmittal Forms</h2>';
  	$transmittal_nodes = node_load_multiple(array_keys($stmt['node']));

  	// *sigh* Sort by json attributes.
		uksort($transmittal_nodes, function($a, $b) use ($transmittal_nodes) {
			$transmittal_json_a = sirius_jsonfield_util_node_json_get($transmittal_nodes[$a]);
			$transmittal_json_b = sirius_jsonfield_util_node_json_get($transmittal_nodes[$b]);
			$year_a = $transmittal_json_a['transmittal']['year'];
			$year_b = $transmittal_json_b['transmittal']['year'];
			if ($year_a > $year_b) { return -1; }
			if ($year_a < $year_b) { return 1; }

			$month_a = $transmittal_json_a['transmittal']['month'];
			$month_b = $transmittal_json_b['transmittal']['month'];
			if ($month_a > $month_b) { return -1; }
			if ($month_a < $month_b) { return 1; }

			return 0;
		});

		$rows = array();
		foreach ($transmittal_nodes as $transmittal_nid => $transmittal_node) {
			$transmittal_json = sirius_jsonfield_util_node_json_get($transmittal_node);
			$year = $transmittal_json['transmittal']['year'];
			$month = $transmittal_json['transmittal']['month'];
			$tools = '';
			$tools .= l('Edit', current_path() . '/edit/' . $transmittal_nid, array('attributes' => array('class' => array('tool-button')), 'query' => array('destination' => current_path())));

			if ($transmittal_json['transmittal']['hours']['active'] == 'Yes') { 
				$tools .= l('Hours', 'node/' . $employer_node->nid . "/sirius_employer_hours/$year/$month", array('attributes' => array('class' => array('tool-button'))));
			}

			if ($transmittal_json['transmittal']['accounts']) {
				foreach ($transmittal_json['transmittal']['accounts'] as $account_nid => $record) {
					$payment_nid = $record['payment_nid'];
					if (!$payment_nid) { continue; }
					$tools .= l($accounts[$account_nid] . ' (' . $record['dollar_amt'] . ')', "node/$payment_nid/sirius_payment_allocate/transmittal", array('attributes' => array('class' => array('tool-button'))));
				}
			}

			$tools .= l('Delete', current_path() . '/delete/' . $transmittal_nid, array('attributes' => array('class' => array('tool-button')), 'query' => array('destination' => current_path())));

			$row = array();
			$row[] = $transmittal_node->field_sirius_summary['und'][0]['value'];
			$row[] = $transmittal_node->field_sirius_type['und'][0]['value'];
			$row[] = $tools;
			$rows[] = $row;
		}

		$header = array('Date', 'Status', 'Tools');
		$html .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('sirius_materials'))));
  }

	$html .= l('Add', current_path() . '/add', array('attributes' => array('class' => array('tool-button')), 'query' => array('destination' => current_path())));
	$html .= l('Print', current_path() . '/print', array('attributes' => array('class' => 'tool-button')));

  return $html;
}

function sirius_ledger_node_tab_employer_transmittal_addedit_form($form, &$form_state, $employer_node, $transmittal_node = NULL) {
	if ($transmittal_node) {
		$form_state['transmittal_nid'] = $transmittal_node->nid;
		drupal_set_title(drupal_get_title() . ' - ' . $transmittal_node->field_sirius_summary['und'][0]['value']); 
		$transmittal_json = sirius_jsonfield_util_node_json_get($transmittal_node);
	} else { 
		drupal_set_title(drupal_get_title() . ' - ' . 'New Transmittal Sheet');
	}
	$form_state['employer_nid'] = $employer_node->nid;

	$form['#tree'] = TRUE;

	$form['transmittal']['type'] = array(
		'#type' => 'select',
		'#title' => t('Status'),
		'#required' => TRUE,
		'#options' => array(
			NULL => t('-- Select --'),
			'Draft' => t('Draft'),
			'Complete' => t('Complete'),
		),
	);
	if ($transmittal_json['transmittal']['type']) { $form['transmittal']['type']['#default_value'] = $transmittal_json['transmittal']['type']; }

	$year_now = date('Y');
	$month_now = date('n');
	$year_options = array();
	for ($year=$year_now-10; $year<$year_now+2; ++$year) { $year_options[$year] = $year; }
	$month_options = array();
	for ($month=1; $month<=12; ++$month) { $month_options[$month] = $month; }

	$payment_types = array(NULL => t('-- Select --')) + sirius_taxonomy_load('sirius_payment_type', 'tid', 'name');

	$form['transmittal']['year'] = array(
		'#type' => 'select',
		'#options' => $year_options,
		'#title' => t('Year'),
	);
	if ($transmittal_json['transmittal']['year']) { $form['transmittal']['year']['#default_value'] = $transmittal_json['transmittal']['year']; } else { $form['transmittal']['year']['#default_value'] = $year_now; }

	$form['transmittal']['month'] = array(
		'#type' => 'select',
		'#options' => $month_options,
		'#title' => t('Month'),
	);
	if ($transmittal_json['transmittal']['month']) { $form['transmittal']['month']['#default_value'] = $transmittal_json['transmittal']['month']; } else { $form['transmittal']['month']['#default_value'] = $month_now; }

	$form['transmittal']['hours']['active'] = array(
		'#type' => 'select',
		'#title' => t('Hours'),
		'#options' => array(
			NULL => t('-- Select --'),
			'No' => t('No'),
			'Yes' => t('Yes'),
		),
	);
	if ($transmittal_json['transmittal']['hours']['active']) { $form['transmittal']['hours']['active']['#default_value'] = $transmittal_json['transmittal']['hours']['active']; }


	$accounts = sirius_ledger_account_list();
	foreach ($accounts as $account_nid => $account_name) {
		$form['transmittal']['accounts'][$account_nid] = array('#type' => 'fieldset', '#title' => $account_name);
		$form['transmittal']['accounts'][$account_nid]['active'] = array(
			'#type' => 'select',
			'#title' => t('Active?'),
			'#options' => array(
				NULL => t('-- Select --'),
				'No' => t('No'),
				'Yes' => t('Yes'),
			),
			'#id' => "transmittal_accounts_${account_nid}_active",
		);
		if ($transmittal_json['transmittal']['accounts'][$account_nid]['active']) { $form['transmittal']['accounts'][$account_nid]['active']['#default_value'] = $transmittal_json['transmittal']['accounts'][$account_nid]['active']; }

		$form['transmittal']['accounts'][$account_nid]['dollar_amt'] = array(
			'#type' => 'textfield',
			'#title' => t('Amount'),
			'#size' => 5, 
			'#states' => array(
				'visible' => array(
					":input[id=\"transmittal_accounts_${account_nid}_active\"]" => array('value' => 'Yes')
				),
			),
		);
		if ($transmittal_json['transmittal']['accounts'][$account_nid]['dollar_amt']) { $form['transmittal']['accounts'][$account_nid]['dollar_amt']['#default_value'] = $transmittal_json['transmittal']['accounts'][$account_nid]['dollar_amt']; }

  	$form['transmittal']['accounts'][$account_nid]['payment_type'] = array(
			'#type' => 'select',
			'#title' => t('Payment Type'),
			'#options' => $payment_types,
			'#states' => array(
				'visible' => array(
					":input[id=\"transmittal_accounts_${account_nid}_active\"]" => array('value' => 'Yes')
				),
			),
		);
		if ($transmittal_json['transmittal']['accounts'][$account_nid]['payment_type']) { $form['transmittal']['accounts'][$account_nid]['payment_type']['#default_value'] = $transmittal_json['transmittal']['accounts'][$account_nid]['payment_type']; }

		$form['transmittal']['accounts'][$account_nid]['payment_nid'] = array(
			'#type' => 'hidden',
			'#value' => $transmittal_json['transmittal']['accounts'][$account_nid]['payment_nid'],
		);
	}

	/*
	$form['transmittal']['accounts'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Accounts'),
		'#checkall' => TRUE,
		'#options' => $accounts,
	);
	if ($transmittal_json['transmittal']['accounts']) { $form['transmittal']['accounts']['#default_value'] = $transmittal_json['transmittal']['accounts']; }
	*/

	// Create ms_level dropdown options
	$industries = sirius_taxonomy_load('sirius_industry', 'tid', 'full');
	$ms_terms = sirius_taxonomy_load('sirius_member_status', 'tid', 'full');
	$employer_industry_tids = sirius_fieldvals($employer_node, 'field_sirius_industry', 'tid');
	$employer_ms_tids = array();
	foreach ($ms_terms as $ms_tid => $ms_term) {
		$industry_tid = $ms_term->field_sirius_industry['und'][0]['tid'];
		if (in_array($industry_tid, $employer_industry_tids)) { $employer_ms_tids[] = $ms_tid; }
	}

	$ms_level_options = array(NULL => t('-- Select --'));
	foreach ($employer_ms_tids as $ms_tid) {
		$levels = sirius_training_levels_fetch($ms_tid);
		$ms_level_options[$ms_tid] = $ms_terms[$ms_tid]->name;
		foreach ($levels as $level_code => $level) {
			$ms_level_options["$ms_tid|$level_code"] = $ms_terms[$ms_tid]->name . ': ' . $level['title'];
		}
	}

	// Get all current dispatches for this employer.
	$dispatches = sirius_ledger_transmittal_dispatches($employer_node->nid);

	$table_open_html .= '<table class="sirius_employer_transmittal">';
	$table_open_html .= '<tr>';
	$table_open_html .= '<th>SSN</th>';
	$table_open_html .= '<th>Name</th>';
	$table_open_html .= '<th>Member Status</th>';
	$table_open_html .= '</tr>';
	$form['transmittal']['table_open'] = array('#markup' => $table_open_html);

	$extra_rows = sirius_domain_variable_get('sirius_ledger_transmittal_extra_rows', 5);
	for ($i=0; $i<$extra_rows; ++$i) {
		$dispatches["extra_" . $i] = array();
	}

	foreach ($dispatches as $dispatch_nid => $dispatch) {
		$worker_json = json_decode($dispatch['worker_json'], TRUE);

		// Figure out the member status. Since I might have multiple member statuses, I neede to pick the one that corresponds to the industry of the job.
		$dispatch_ms_level = $transmittal_json['transmittal']['dispatches'][$dispatch_nid]['ms_level'];
		if (!$dispatch_ms_level && $dispatch_nid > 0) {
			$industry_tid = $dispatch['industry_tid'];
			if ($industry_tid) {
				$ms_tids = explode(',', $dispatch['ms_tids']);
				foreach ($ms_tids as $probe) {
					if ($ms_terms[$probe]->field_sirius_industry['und'][0]['tid'] == $industry_tid) {
						$dispatch_ms_level = $probe;
					}
				}
			}

			// I might have a level within an MS
			$level_code = $worker_json['training']['levels'][$dispatch_ms_level]['code'];
			if ($level_code) { $dispatch_ms_level .= '|' . $level_code; }
		}

		$form['transmittal']['dispatches'][$dispatch_nid]['tr_open'] = array('#markup' => '<tr style="page-break-inside: avoid;">');

		if ($dispatch) {
			$worker_title = $dispatch['worker_title'];
			if (!$worker_title) { $worker_title = ''; }
			$ssn = $dispatch['field_sirius_ssn_value'];
			$form['transmittal']['dispatches'][$dispatch_nid]['ssn'] = array('#markup' => '<td>' . $ssn . '</td>');
			$form['transmittal']['dispatches'][$dispatch_nid]['worker_nid'] = array('#type' => 'hidden', '#value' => $dispatch['worker_nid']);
			$form['transmittal']['dispatches'][$dispatch_nid]['worker_name'] = array('#type' => 'hidden', '#value' => $dispatch['worker_title']);
			$form['transmittal']['dispatches'][$dispatch_nid]['contact_nid'] = array('#type' => 'hidden', '#value' => $dispatch['contact_nid']);
		} else {
			$worker_title = $transmittal_json['transmittal']['dispatches'][$dispatch_nid]['worker_name'];
			$form['transmittal']['dispatches'][$dispatch_nid]['ssn'] = array(
				'#type' => 'textfield',
				'#size' => 11,
				'#prefix' => '<td class="container-inline">SSN',
				'#suffix' => '</td>',
			);
			if (isset($transmittal_json['transmittal']['dispatches'][$dispatch_nid]['ssn'])) { $form['transmittal']['dispatches'][$dispatch_nid]['ssn']['#default_value'] = $transmittal_json['transmittal']['dispatches'][$dispatch_nid]['ssn']; }
		}

		$form['transmittal']['dispatches'][$dispatch_nid]['name'] = array('#markup' => '<td>' . $worker_title . '</td>');
		$form['transmittal']['dispatches'][$dispatch_nid]['ms_level'] = array(
			'#type' => 'select',
			'#options' => $ms_level_options,
			'#prefix' => '<td>',
			'#suffix' => '</td>',
			'#default_value' => $dispatch_ms_level,
		);
	}

	$form['transmittal']['dispatches'][$dispatch_nid]['tr_close'] = array('#markup' => '</tr>');

	$form['transmittal']['table_close'] = array('#markup' => '</table>');

	$form['dispatch_status'] = array(
		'#title' => t('Create Dispatches?'),
		'#type' => 'select',
		'#options' => array(
			NULL => t('Do not create dispatches'),
			'accepted' => t('Create a dispatch for all workers'),
			'accepted_secondary' => t('Create a secondary dispatch for all workers'),
		),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}


function sirius_ledger_node_tab_employer_transmittal_addedit_form_validate($form, &$form_state) {
	$employer_node = node_load($form_state['employer_nid']);
	$year = $form_state['year'];
	$month = $form_state['month'];
	$day = 1;

	foreach ($form_state['values']['transmittal']['dispatches'] as $dispatch_nid => $transmittal_record) {
		if ($dispatch_nid > 0) {
			if (!$transmittal_record['ms_level']) {
				form_set_error("transmittal][dispatches][$dispatch_nid][ms_level", 'Please select a member status');
				continue; 
			}
		} else {
			$ssn = $transmittal_record['ssn'];
			if ($ssn) {
				if (!$transmittal_record['ms_level']) {
					form_set_error("transmittal][dispatches][$dispatch_nid][ms_level", 'Please select a member status');
					continue; 
				}
				if (!$ssn) {
					form_set_error("transmittal][dispatches][$dispatch_nid][ssn", 'Please enter a SSN');
					continue; 
				}
				if (!sirius_ssn_validate($ssn)) {
					form_set_error("transmittal][dispatches][$dispatch_nid][ssn", 'Please enter a valid SSN');
					continue; 
				}
				$ssn = sirius_ssn_format($ssn);
				$worker_node = sirius_worker_get_by_ssn($ssn);
				if (!$worker_node) {
					form_set_error("transmittal][dispatches][$dispatch_nid][ssn", 'No worker found with this SSN');
					continue; 
				}
				$form_state['storage']['dispatches'][$dispatch_nid]['worker_nid'] = $worker_node->nid;
				$form_state['storage']['dispatches'][$dispatch_nid]['worker_name'] = $worker_node->title;
				$form_state['storage']['dispatches'][$dispatch_nid]['contact_nid'] = $worker_node->field_sirius_contact['und'][0]['target_id'];
			}
		}
	}
}

function sirius_ledger_node_tab_employer_transmittal_addedit_form_submit($form, &$form_state) {
	$employer_node = node_load($form_state['employer_nid']);
	$type = $form_state['values']['transmittal']['type'];
	global $user;
	$domain_nid = sirius_domain_nid();

	$transmittal_node = node_load($form_state['transmittal_nid']);
	if ($transmittal_node) { 
		$transmittal_json = sirius_jsonfield_util_node_json_get($transmittal_node);
	} else {
		$transmittal_json = array();
	}
	$transmittal_json['transmittal'] = $form_state['values']['transmittal'];
	if ($form_state['storage']['dispatches']) {
		foreach ($form_state['storage']['dispatches'] as $dispatch_nid => $record) {
			$transmittal_json['transmittal']['dispatches'][$dispatch_nid]['worker_nid'] = $record['worker_nid'];
			$transmittal_json['transmittal']['dispatches'][$dispatch_nid]['worker_name'] = $record['worker_name'];
			$transmittal_json['transmittal']['dispatches'][$dispatch_nid]['contact_nid'] = $record['contact_nid'];
		}
	}

	// Create any missing dispatches
	$dispatch_status = $form_state['values']['dispatch_status'];
	if ($dispatch_status && $transmittal_json['transmittal']['dispatches']) {
		$industry_tid = $employer_node->field_sirius_industry['und'][0]['tid'];
		$employer_nid = $employer_node->nid;
		$existing_dispatches = sirius_ledger_transmittal_dispatches($employer_node->nid);
		$already_dispatched_workers = array();
		foreach ($existing_dispatches as $existing_dispatch) { $already_dispatched_workers[$existing_dispatch['worker_nid']] = TRUE; }

		foreach ($transmittal_json['transmittal']['dispatches'] as $dispatch_nid => $record) {
			// Is the worker already dispatched?
			$worker_nid = $record['worker_nid'];
			if (!$worker_nid) { continue; }
			if ($already_dispatched_workers[$worker_nid]) { continue; }

			// Need the industry. We get this from the member status.
			$ms_level = $record['ms_level'];
			list ($ms_tid, $level_code) = explode('|', $ms_level);
			if (!$ms_tid) { continue; }
			$ms_term = taxonomy_term_load($ms_tid);
			$industry_tid = $ms_term->field_sirius_industry['und'][0]['tid'];
			if (!$industry_tid) { continue; }

			// Need a stub job
			$stub_job_node = sirius_dispatch_job_stub_fetch($employer_nid, $industry_tid);
			if (!$stub_job_node) {
				drupal_set_message("Unable to create a dispatch for $record[worker_name]; there is no stub job for this employer and industry.");
				continue; 
			}

			// Dispatch this worker to this job
			$result = sirius_dispatch_actions_create($stub_job_node, $worker_nid, 'No', $dispatch_status);
			if ($result['success']) {
				drupal_set_message("Dispatched $record[worker_name] with status $dispatch_status");
			} else {
				drupal_set_message("Failed to Dispatched $record[worker_name] with status $dispatch_status: $result[msg]", 'warning');
			}
		}
	}

	// Create the payments
	$accounts = sirius_ledger_account_list();

	foreach ($form_state['values']['transmittal']['accounts'] as $account_nid => $record) {
		if ($record['active'] != 'Yes') { continue; }

		$payment_node = node_load($record['payment_nid']);

		if (!$payment_node) {
			// Create the event node
			$payment_node = new stdClass();
			$payment_node->type = 'sirius_payment';
			$payment_node->no_recurse = TRUE;
			$payment_node->language = $user->language;
			$payment_node->uid = $user->uid;
			$payment_node->status = 1;
			$payment_node->promote = 0;
			$payment_node->comment = 0;
			$payment_node->field_sirius_payer['und'][0]['target_id'] = $employer_node->nid;
			$payment_node->field_sirius_ledger_account['und'][0]['target_id'] = $account_nid;
			$payment_node->field_sirius_datetime_created['und'][0]['value'] = gmdate('Y-m-d H:i:s');
			$payment_node->field_sirius_ledger_allocated['und'][0]['value'] = 'No';
			$payment_node->field_sirius_payment_status['und'][0]['value'] = 'Received';
			if ($domain_nid) { $payment_node->field_sirius_domain['und'][0]['target_id'] = $domain_nid; }
		}

		$payment_node->field_sirius_dollar_amt['und'][0]['value'] = -1 * $record['dollar_amt'];
		if ($record['payment_type']) { $payment_node->field_sirius_payment_type['und'][0]['tid'] = $record['payment_type']; }
		sirius_ledger_payment_set_title($payment_node);
		node_object_prepare($payment_node);
		$payment_node = node_submit($payment_node);
		node_save($payment_node);
		$transmittal_json['transmittal']['accounts'][$account_nid]['payment_nid'] = $form_state['values']['transmittal']['accounts'][$account_nid]['payment_nid'] = $payment_node->nid;
	}

	$summary = $form_state['values']['transmittal']['year'] . ' / ' . $form_state['values']['transmittal']['month'];
	if ($transmittal_node) {
		sirius_jsonfield_util_node_json_set($transmittal_node, $transmittal_json);
		$transmittal_node->field_sirius_type['und'][0]['value'] = $type;
		$transmittal_node->field_sirius_message['und'][0]['value'] = $transmittal_node->field_sirius_summary['und'][0]['value'] = $summary;
		node_save($transmittal_node);
		drupal_set_message("Transmittal edited.");
	} else {
		$transmittal_node = sirius_log('ledger:transmittal', $type, $summary, $summary, NULL, array($employer_node->nid), $files, $transmittal_json);
		drupal_set_message(t('Transmittal added.'));
	}

	// Save the payment nodes again, because we might not have the transmittal NID until this point. Extra work. Oh well.
	foreach ($form_state['values']['transmittal']['accounts'] as $account_nid => $record) {
		$payment_node = node_load($record['payment_nid']);
		if (!$payment_node) { continue; }
		$payment_json = sirius_jsonfield_util_node_json_get($payment_node);
		if ($payment_json['transmittal_nid'] == $transmittal_node->nid) { continue; }
		$payment_json['transmittal_nid'] = $transmittal_node->nid;
		sirius_jsonfield_util_node_json_set($payment_node, $payment_json);
		node_save($payment_node);
		drupal_set_message("Saved: " . $payment_node->title . '(' . $payment_node->nid . ')');
	}

	// Create the epayperiod, if any, and link the transmittal NID
	if ($form_state['values']['transmittal']['hours']['active'] == 'Yes') {
		$year = $form_state['values']['transmittal']['year'];
		$month = $form_state['values']['transmittal']['month'];
		$day = 1;
		$epayperiod_node = sirius_hour_epayperiod_load($domain_nid, $employer_node->nid, $year, $month, $day, 'Yes');
		$epayperiod_json = sirius_jsonfield_util_node_json_get($epayperiod_node);
		if ($transmittal_node->nid != $epayperiod_json['transmittal_nid']) {
			$epayperiod_json['transmittal_nid'] = $transmittal_node->nid;
			sirius_jsonfield_util_node_json_set($epayperiod_node, $epayperiod_json);
			node_save($epayperiod_node);
			drupal_set_message("Saved hours record.");
		} 
	}
}

function sirius_ledger_node_tab_employer_transmittal_delete_form($form, &$form_state, $employer_node, $transmittal_node) {
	$form_state['transmittal_nid'] = $transmittal_node->nid;

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Really Delete?'),
	);

	return $form;
}

function sirius_ledger_node_tab_employer_transmittal_delete_form_submit($form, &$form_state) {
	node_delete($form_state['transmittal_nid']);
	drupal_set_message(t('Transmittal form deleted.'));
}
