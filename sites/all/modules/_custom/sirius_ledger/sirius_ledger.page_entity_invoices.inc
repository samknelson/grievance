<?php

function sirius_ledger_node_tab_entity_invoices_access($entity_node) {
	$access = sirius_access_instance('ledger');
	return $access->access('entity_page_view', array('entity_node' => $entity_node));
}

function sirius_ledger_node_tab_entity_invoices($entity_node) {
	$interval_id = $_REQUEST['interval_id'];
	if ($interval_id) {
		return sirius_ledger_node_tab_entity_invoices_single($entity_node, $interval_id);
	} else {
		return sirius_ledger_node_tab_entity_invoices_multiple($entity_node);
	}
}

function sirius_ledger_node_tab_entity_invoices_single($entity_node, $interval_id) {
	$invoice_obj = sirius_ledger_invoice_obj();
	$interval = $invoice_obj->interval_from_id($interval_id);
	$next_interval = $invoice_obj->next_interval($interval);
	$prev_interval = $invoice_obj->prev_interval($interval);
	$first_interval = $invoice_obj->first_interval($entity_node->nid);
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

	$invoice = $invoice_obj->invoice_load($entity_node->nid, $interval);

	$html .= $invoice_obj->invoice_render($invoice);

	return $html;
}

function sirius_ledger_node_tab_entity_invoices_multiple($entity_node) {
	$invoice_obj = sirius_ledger_invoice_obj();

	$participant_nid = $entity_node->nid;
	$intervals = array_reverse($invoice_obj->intervals($participant_nid));
	$accounts = $invoice_obj->accounts($participant_nid, $invoice_obj->now_interval());

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

			$balances = $invoice_obj->invoice_load_balances($entity_node->nid, $interval);

			$row = array();
			$row[] = $invoice_obj->invoice_id($participant_nid, $interval);
			$row[] = $interval['month'] . '/' . $interval['year'];
			foreach ($accounts as $account_nid => $account_title) {
				$row[] = sirius_ledger_ar_format_balance($balances[$account_nid]['balance_out']);
			}
			$row[] = $tools;
			$rows[] = $row;
		}

		$header = array();
		$header[] = array('data' => t('Invoice #'));
		$header[] = array('data' => t('Period'));
		foreach ($accounts as $account_nid => $account_title) {
			$header[] = array('data' => $account_title);
		}
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
