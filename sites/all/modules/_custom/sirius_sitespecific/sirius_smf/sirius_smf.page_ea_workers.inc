<?php

function sirius_smf_node_tab_ea_workers_access($ea_node) {
	if ($ea_node->field_sirius_category['und'][0]['value'] != 'ledger:ea') { return FALSE; }
	return Sirius_Ledger_Access::getInstance()->access('ea_view_employer_workers', array('ea_node' => $ea_node));
}

function sirius_smf_node_tab_ea_workers($ea_node) {
	$access = Sirius_Ledger_Access::getInstance();
	$ea = Sirius_Ledger_EA::getInstance();
	$account_node = $ea->account_node($ea_node);
	$entity_node = $ea->entity_node($ea_node);
	$currency_info = sirius_ledger_currency_info_from_account_nid($account_node->nid);

	$csv = $_REQUEST['csv'];

	$params = array();
	$params['employer_nid'] = $entity_node->nid;
	$params['statuses'] = array('accepted', 'accepted_secondary');
	$params['return_workers'] = TRUE;
	$params['nids_only'] = TRUE;
	$params['order'] = 'field_sirius_name_family, field_sirius_name_given, field_sirius_name_middle';
	$worker_nids = sirius_dispatch_search($params);
	if (!$worker_nids) { return sirius_page_error("No employees found."); }

	$is_staff = $access->access('staff');

	$cardcheck_instance = Sirius_Cardcheck::getInstance();
	$cardcheck_definition_kaiser = $cardcheck_instance->get_definition_by_id('K');
	if (!$cardcheck_definition_kaiser) {
		return sirius_form_error("Please make sure that there is just one card check definition with the ID \"K\".");
	}

	$cardcheck_definition_healthnet = $cardcheck_instance->get_definition_by_id('HN');
	if (!$cardcheck_definition_healthnet) {
		return sirius_form_error("Please make sure that there is just one card check definition with the ID \"HN\".");
	}

	$rows = array();
	$total = 0;
	foreach ($worker_nids as $worker_nid) {
		$balance = -1 * sirius_ledger_balance_fetch(sirius_contact_get_nid_from_worker($worker_nid), $account_node->nid);
		$total += $balance;
		$params = array();

		$worker_node = node_load($worker_nid);

		$ein = sirius_worker_employee_lookup($worker_nid, $entity_node->nid, array('return' => 'ein'));

		if ($is_staff && !$csv) {
			$tools = array();
			$tools[] = l('Worker', "node/$worker_nid/sirius_worker_ledger", array('attributes' => array('class' => array('tool-button'))));
			$sub_ea_node = $ea->fetch($worker_nid, $account_node->nid, TRUE);
			$tools[] = l('account', "node/" . $sub_ea_node->nid . '/sirius_ea_ar', array('attributes' => array('class' => array('tool-button'))));
			$tools = join('', $tools);
		}

		$search_params = array();
		$search_params['worker_nid'] = $worker_nid;
		$search_params['definition_nid'] = $cardcheck_definition_healthnet->nid;
		$search_params['status'] = 'signed';
		$probe = reset($cardcheck_instance->search($search_params));
		if ($probe) { $hn = 'Yes'; } else { $hn = 'No'; }

		$search_params = array();
		$search_params['worker_nid'] = $worker_nid;
		$search_params['definition_nid'] = $cardcheck_definition_kaiser->nid;
		$search_params['status'] = 'signed';
		$probe = reset($cardcheck_instance->search($search_params));
		if ($probe) { $k = 'Yes'; } else { $k = 'No'; }

		if ($csv) {
			$balance_render = $balance; 
		} else {
			$balance_render = $currency_info['instance']->render_amt($balance);
		}

		$row = array();
		$row[] = $worker_node->title;
		$row[] = $worker_nid;
		$row[] = $ein;
		$row[] = $balance_render;
		$row[] = $hn;
		$row[] = $k;
		if ($is_staff && !$csv) { $row[] = $tools; }

		$rows[] = $row;
	}

	$header = array();
	$header[] = 'Employee';
	$header[] = 'NID';
	$header[] = 'EIN';
	$header[] = 'Balance';
	$header[] = 'Cardcheck Healthnet?';
	$header[] = 'Cardcheck Kaiser?';
	if ($is_staff && !$csv) { $header[] = 'Tools'; }

	if ($csv) {
	  $filename = $ea_node->title . ' :: ' . 'Employee Balances' . ' :: ' . date('c');
	  $filename = strtolower($filename);
	  $filename = preg_replace('/[^a-zA-Z0-9_ \:]/', ' ', $filename);
	  $filename = preg_replace('/ +/', ' ', $filename);
	  $filename = preg_replace('/_+/', '_', $filename);
	  $filename = preg_replace('/::/', '-', $filename);
	  $filename .= ".csv";

	  drupal_add_http_header('Content-Type', 'text/csv');
	  drupal_add_http_header('Content-Disposition', 'attachment;filename=' . $filename);
	  $fp = fopen('php://output', 'w');
	  fputcsv($fp, $header);
	  foreach ($rows as $row) {
	    fputcsv($fp, $row);
	  }
	  drupal_exit();
	}

	$html .= sirius_table_render($header, $rows);
	$html .= sirius_fakerender_field('Total balance:', $currency_info['instance']->render_amt($total));
	$html .= l(
		'CSV Export', 
		current_path(), 
		array(
			'query' => array('csv' => '1'),
			'attributes' => array('class' => array('tool-button'))
		)
	);
	return $html;
}
