<?php

function sirius_ledger_node_tab_employer_transmittal_access($employer_node) {
	return TRUE;
}

function sirius_ledger_node_tab_employer_transmittal($employer_node) {
	$arg = sirius_node_tab_arg(0);
	if ($arg == 'print') { return drupal_get_form('sirius_ledger_node_tab_employer_transmittal_form', $employer_node); }

	$html .= 'To generate the transmittal form for this employer, ';
	$html .= l('click here.', current_path() . '/print', array('attributes' => array('target' => '_blank')));
	return $html;
}

function sirius_ledger_node_tab_employer_transmittal_form($form, &$form_state, $employer_node) {
	drupal_add_css('sites/all/modules/_custom/sirius_ledger/css/transmittal.css');
	grievance_classes_array('loopback');

	$form_state['employer_nid'] = $employer_node->nid;

	// Fetch the indsutries
	$industries = sirius_taxonomy_load('sirius_industry', 'tid', 'full');
	$ms_terms = sirius_taxonomy_load('sirius_member_status', 'tid', 'full');

	// Get the policy for this employer
	$schedule_key = 'hour';
	$policy_node = sirius_ledger_policy_lookup($schedule_key, NULL, $employer_node);
	if (!$policy_node) {
		drupal_set_message("No charge policy found: " . sirius_minilog_render_html(), 'warning');
		return array();
	}
	$policy_json = sirius_jsonfield_util_node_json_get($policy_node);
	$account_options = sirius_ledger_policy_accounts($policy_json);
	unset($account_options['pay']);
	$rates = $policy_json['ledger']['policy']['rates'][$schedule_key];

	$form['#tree'] = TRUE;
	$form['wrap_open'] = array('#markup' => '<div class="sirius_ledger_transmittal">');

	$contact_node = sirius_ledger_transmittal_contact($employer_node->nid);
	if ($contact_node) {
		$address_html = '<div class="sirius_ledger_transmittal_address">';
		$address_html .= $employer_node->title;
		$address_html .= '<br />' . $contact_node->field_grievance_co_address['und'][0]['value'];
		if ($contact_node->field_grievance_co_address_2) { $address_html .= '<br />' . $contact_node->field_grievance_co_address2['und'][0]['value']; }
		$address_html .= '<br />' . $contact_node->field_grievance_co_city['und'][0]['value'] . ', ' . $contact_node->field_grievance_co_state['und'][0]['value'] . ' ' . $contact_node->field_grievance_co_zip['und'][0]['value'];
		$address_html .= '</div>';
		$form['address'] = array('#markup' => $address_html);
	}

	$intro_html .= '<div class="sirius_form_intro">';
	$intro_html .= '<h1>' . $employer_node->title . '<br />' . 'Transmittal Form ' . date('F Y') . '</h1>';
	$intro_html .= '</div>';
	$form['intro'] = array('#markup' => $intro_html);

	$header = sirius_domain_variable_get('sirius_ledger_transmittal_header', '');
	if ($header) { $form['header'] = array('#markup' => '<div class="sirius_ledger_transmittal_header">' . $header . '</div>'); }

	// Get all current dispatches for this employer. SQL for speed.
	$dispatches = sirius_ledger_transmittal_dispatches($employer_node->nid);

	$table_open_html .= '<table class="sirius_ledger_transmittal">';
	$table_open_html .= '<tr>';
	$table_open_html .= '<th rowspan="2">Worker<br />SSN</th>';
	$table_open_html .= '<th rowspan="2">Member Status</th>';
	$table_open_html .= '<th rowspan="2">Hours</th>';
	foreach ($account_options as $account_nid => $account_name) {
		$table_open_html .= '<th colspan="2">' . $account_name . '</th>';
	}
	$table_open_html .= '</tr>'; 
	$table_open_html .= '<tr>';
	foreach ($account_options as $account_nid => $account_name) {
		$table_open_html .= '<th>Rate<br />Balance</th>';
		$table_open_html .= '<th>Withheld</th>';
	}
	$table_open_html .= '</tr>';
	$form['transmittal']['table_open'] = array('#markup' => $table_open_html);


	$extra_rows = sirius_domain_variable_get('sirius_ledger_transmittal_extra_rows', 5);
	for ($i=0; $i<$extra_rows; ++$i) {
		$dispatches["extra_" . $i] = array();
	}

	foreach ($dispatches as $dispatch_nid => $dispatch) {
		$worker_json = json_decode($dispatch['worker_json'], TRUE);

		// Figure out the member status. Since I might have multiple member statuses, I neede to pick the one that corresponds to the industry of the job.
		$industry_tid = $dispatch['industry_tid'];
		$industry_name = $ms_tid = $ms_name = NULL;
		if ($industry_tid) {
			$industry_name = $industries[$industry_tid]->name;
			$ms_tids = explode(',', $dispatch['ms_tids']);
			foreach ($ms_tids as $probe) {
				if ($ms_terms[$probe]->field_sirius_industry['und'][0]['tid'] == $industry_tid) {
					$ms_tid = $probe;
					$ms_name = $ms_terms[$probe]->name;
				}
			}
		}

		// I might have a level within an MS
		$level_code = $worker_json['training']['levels'][$ms_tid]['code'];
		$level_name = NULL; 
		if ($level_code) { $level_name = sirius_training_levels_fetch_title($ms_tid, $level_code); }
		if ($level_name) { $ms_name .= '<br />' . $level_name; }


		$worker_title = $dispatch['worker_title'];
		if (!$worker_title) { $worker_title = '____________'; }

		$ssn = $dispatch['field_sirius_ssn_value'];
		if (!$ssn) { $ssn = '___-__-____'; }
		if (!$ms_name) { $ms_name = '____________'; }

		$form['transmittal']['dispatches'][$dispatch_nid]['tr_open'] = array('#markup' => '<tr>');

		$form['transmittal']['dispatches'][$dispatch_nid]['title'] = array('#markup' => '<td>' . $worker_title . '<br />' . $ssn . '</td>');
		$form['transmittal']['dispatches'][$dispatch_nid]['ms_name'] = array('#markup' => '<td>' . $ms_name . '</td>');

		$form['transmittal']['dispatches'][$dispatch_nid]['hours'] = array('#markup' => '<td>____________</td>');

		foreach ($account_options as $account_nid => $account_name) {
			// Get the rate
			$rate = $rates[$ms_tid][$account_nid];
			if ($level_code && $rates["$ms_tid|$level_code"][$account_nid]) { $rate = $rates["$ms_tid|$level_code"][$account_nid]; }
			if ($rate) { $rate = '$' . number_format($rate, 2); }
			if (!$rate) { $rate = '____________'; }

			// Get the balance
			$balance = NULL;
			if ($dispatch) {
				$params = array();
				$params['ledger_account'] = $account_nid;
				$params['ledger_participant'] = $dispatch['contact_nid'];
				$params['balance'] = TRUE;
				$balance = sirius_ledger_ar_fetch($params);
			}
			if (isset($balance)) {
				$balance = sirius_ledger_ar_format_balance($balance);
			} else {
				$balance = '';
			}

			$form['transmittal']['dispatches'][$dispatch_nid][$account_nid]['rate'] = array('#markup' => '<td>' . $rate . '<br />' . $balance . '</td>');
			$form['transmittal']['dispatches'][$dispatch_nid][$account_nid]['payments'] = array('#markup' => '<td>____________</td>');
		}

		$form['transmittal']['dispatches'][$dispatch_nid]['tr_close'] = array('#markup' => '</tr>');
	}

	$form['transmittal']['dispatches']['total']['tr_open'] = array('#markup' => '<tr>');
	$form['transmittal']['dispatches']['total']['title'] = array('#markup' => '<th colspan="2">Total</th>');
	$form['transmittal']['dispatches']['total']['hours'] = array('#markup' => '<th>____________</th>');
	foreach ($account_options as $account_nid => $account_name) {
		$form['transmittal']['dispatches']['total'][$account_nid] = array('#markup' => '<th></th><th>____________</th>');
	}
	$form['transmittal']['dispatches']['total']['tr_close'] = array('#markup' => '</tr>');


	$form['transmittal']['table_close'] = array('#markup' => '<table>');

	$footer = sirius_domain_variable_get('sirius_ledger_transmittal_footer', '');
	if ($footer) { $form['footer'] = array('#markup' => '<div class="sirius_ledger_transmittal_footer">' . $footer . '</div>'); }



	$form['wrap_close'] = array('#markup' => '</div>');

  return $form;
}