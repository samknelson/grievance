<?php

function sirius_ledger_node_tab_ea_employer_workers_access($ea_node) {
	if ($ea_node->field_sirius_category['und'][0]['value'] != 'ledger:ea') { return FALSE; }
	return Sirius_Ledger_Access::getInstance()->access('ea_view_employer_workers', array('ea_node' => $ea_node));
}

function sirius_ledger_node_tab_ea_employer_workers($ea_node) {
	$access = Sirius_Ledger_Access::getInstance();
	$ea = Sirius_Ledger_EA::getInstance();
	$account_node = $ea->account_node($ea_node);
	$entity_node = $ea->entity_node($ea_node);
	$currency_info = sirius_ledger_currency_info_from_account_nid($account_node->nid);

	$params = array();
	$params['employer_nid'] = $entity_node->nid;
	$params['status'] = 'accepted';
	$params['return_workers'] = TRUE;
	$params['nids_only'] = TRUE;
	$params['order'] = 'field_sirius_name_family, field_sirius_name_given, field_sirius_name_middle';
	$worker_nids = sirius_dispatch_search($params);
	if (!$worker_nids) { return sirius_page_error("No employees found."); }

	$is_staff = $access->access('staff');

	$rows = array();
	$total = 0;
	foreach ($worker_nids as $worker_nid) {
		$balance = -1 * sirius_ledger_balance_fetch(sirius_contact_get_nid_from_worker($worker_nid), $account_node->nid);
		$total += $balance;
		$params = array();
		$row = array();
		$row[] = sirius_node_title($worker_nid);
		$row[] = $currency_info['instance']->render_amt($balance);

		if ($is_staff) {
			$tools = array();
			$tools[] = l('Worker', "node/$worker_nid/sirius_worker_ledger", array('attributes' => array('class' => array('tool-button'))));
			$sub_ea_node = $ea->fetch($worker_nid, $account_node->nid, TRUE);
			$tools[] = l('account', "node/" . $sub_ea_node->nid . '/sirius_ea_ar', array('attributes' => array('class' => array('tool-button'))));
			$row[] = join('', $tools);
		}
		$rows[] = $row;
	}

	$header = array();
	$header[] = 'Employee';
	$header[] = 'Balance';
	if ($is_staff) { $header[] = 'Tools'; }

	$html .= sirius_table_render($header, $rows);
	$html .= sirius_fakerender_field('Total balance:', $currency_info['instance']->render_amt($total));

	return $html;
}
