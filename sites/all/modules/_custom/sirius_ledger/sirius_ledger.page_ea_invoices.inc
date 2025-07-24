<?php

function sirius_ledger_node_tab_ea_invoices_access($ea_node) {
	// Must be an EA node
	if ($ea_node->field_sirius_category['und'][0]['value'] != 'ledger:ea') { return FALSE; }

	// Must have access to view invoices
	if (!Sirius_Ledger_Access::getInstance()->access('ea_invoices', array('ea_node' => $ea_node))) { return FALSE; }

	// Must be at least one invoice
	$ea = Sirius_Ledger_EA::getInstance();
	$invoice_obj = sirius_ledger_invoice_obj();
	$participant_node = $ea->participant_node($ea_node);
	$account_node = $ea->account_node($ea_node);
	$first_interval = $invoice_obj->first_interval($participant_node->nid, $account_node->nid);
	if (!$first_interval) { return FALSE; }

	return TRUE;
}

function sirius_ledger_node_tab_ea_invoices($ea_node) {
	$ea = Sirius_Ledger_EA::getInstance();

	$interval_id = $_REQUEST['interval_id'];
	if ($interval_id) {
		return sirius_ledger_node_tab_ea_invoices_single($ea_node, $interval_id);
	} else {
		return sirius_ledger_node_tab_ea_invoices_multiple($ea_node);
	}
}

function sirius_ledger_node_tab_ea_invoices_single($ea_node, $interval_id) {
	$ea = Sirius_Ledger_EA::getInstance();
	$entity_node = $ea->entity_node($ea_node);
	$participant_node = $ea->participant_node($ea_node);
	$account_node = $ea->account_node($ea_node);

	$invoice_obj = sirius_ledger_invoice_obj();
	$interval = $invoice_obj->interval_from_id($interval_id);
	$next_interval = $invoice_obj->next_interval($interval);
	$prev_interval = $invoice_obj->prev_interval($interval);
	$first_interval = $invoice_obj->first_interval($participant_node->nid, $account_node->nid);
	$now_interval = $invoice_obj->now_interval($entity_node);

	if ($prev_interval['id'] < $first_interval['id']) { $prev_interval = array(); }
	if ($next_interval['id'] > $now_interval['id']) { $next_interval = array(); }

	$tools = array();
	if ($prev_interval) {
		$tools[] = l(
			'<i class="fas fa-arrow-left"></i> Prev', 
			current_path(), 
			array(
				'query' => array('interval_id' => $prev_interval['id']), 
				'html' => TRUE, 
				'attributes' => array('class' => array('tool-button'))
			)
		);
	}
	$tools[] = l('List', current_path(), array('attributes' => array('class' => array('tool-button'))));
	if ($next_interval) {
		$tools[] = l(
			'Next <i class="fas fa-arrow-right"></i>', 
			current_path(), 
			array(
				'query' => array('interval_id' => $next_interval['id']), 
				'html' => TRUE, 
				'attributes' => array('class' => array('tool-button'))
			)
		);
	}


	$html .= join('', $tools);

	$invoice = $invoice_obj->invoice_load($participant_node->nid, $account_node->nid, $interval);

	$html .= $invoice_obj->invoice_render($invoice);

	return $html;
}

function sirius_ledger_node_tab_ea_invoices_multiple($ea_node) {
	$ea = Sirius_Ledger_EA::getInstance();
	$entity_node = $ea->entity_node($ea_node);
	$account_node = $ea->account_node($ea_node);
	$participant_node = $ea->participant_node($ea_node);

	$invoice_obj = sirius_ledger_invoice_obj();
	$currency = sirius_ledger_currency_info_from_account_nid($account_node->nid)['instance'];

	$intervals = array_reverse($invoice_obj->intervals($participant_node->nid, $account_node->nid));

	$html .= '<h2>' . t('Invoices') . '</h2>';
	if (!$intervals) {
		$html .= '<p>' . t('No invoices found.') . '</p>';
	} else {
		$rows = array();
		foreach ($intervals as $interval) {
			$path = current_path();
			if (!preg_match('/invoices$/', $path)) { $path .= "/invoices"; } // DEFAULT_LOCAL_TAKS dammit

			$tools = l(
				'View', 
				$path, 
				array(
					'query' => array('interval_id' => $interval['id']),
					'attributes' => array('class' => array('tool-button')),
				)
			);

			$balances = $invoice_obj->invoice_load_balances($participant_node->nid, $account_node->nid, $interval);

			$row = array();
			$row[] = $invoice_obj->invoice_id($participant_node->nid, $interval);
			$row[] = $interval['month'] . '/' . $interval['year'];
			$row[] = $currency->render_amt($balances['balance_out']);
			$row[] = $tools;
			$rows[] = $row;
		}

		$header = array();
		$header[] = array('data' => t('Invoice #'));
		$header[] = array('data' => t('Period'));
		$header[] = array('data' => t('Balance'));
		$header[] = array('data' => t('Tools'));
		$count = count($header);
		$pct = round(100/$count);
		foreach ($header as $delta => $record) { 
			$header[$delta]['width'] = "$pct%";
		}

		$html .= sirius_table_render($header, $rows);
	}
	return $html;
}
