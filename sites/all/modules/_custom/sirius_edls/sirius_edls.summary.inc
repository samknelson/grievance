<?php

function sirius_edls_summary($employer_node, $date, $options = array()) {
	$ts = strtotime($date);
	$date = date('Y-m-d', strtotime($date));
	$date_render = date('l, j F, Y', strtotime($date));

	$html .= '<div class="sirius_edls_summary">';

	$ms_list = sirius_edls_ms_list($employer_node);
	$status_options = sirius_edls_sheet_status_options();
	unset($status_options['trash']);

	$search_params = array();
	$search_params['employer_nid'] = $employer_node->nid;
	$search_params['date'] = $date;
	$sheet_nodes = sirius_edls_sheet_search($search_params);

	$html .= '<h2>' . t('Summary for:') . ' ' . $date_render . '</h2>';
	$workers = sirius_edls_worker_list($employer_node->nid, $date, $filters = array());

	$data = array();
	$assigned_by_sheet = array();
	$grand_total = array();
	foreach ($workers as $worker) {
		$sheet_nid = $worker['curr_sheet_nid'];

		// The worker has an assignment today, but we didn't find the sheet today. This is an exception.
		// Maybe they're assigned to a trash sheet? Maybe the date for the sheet was edited manually?
		// Let's just pretend that they're unassigned.
		if ($sheet_nid && !$sheet_nodes[$sheet_nid]) { $sheet_nid = 0; }

		$ms_key = $worker['worker_ms'];
		$data[$ms_key]['total']++;
		$grand_total['total']++;

		if ($sheet_nid) {
			$status_key = $sheet_nodes[$sheet_nid]->field_sirius_edls_sheet_status['und'][0]['value'];
			if (!$status_options[$status_key]) { $status_key = 'unassigned'; }
			$data[$ms_key][$status_key]++;
			$grand_total[$status_key]++;
		} else {
			$data[$ms_key]['unassigned']++;
			$grand_total['unassigned']++;
		}

		$ms_key = $worker['worker_ms'];
	}

	$rows = array();
	foreach ($ms_list as $ms_key => $ms_name) {
		if (!$data[$ms_key]) { continue; }

		$row = array();
		$row[] = array('data' => $ms_name, 'width' => '40%');
		foreach ($status_options as $status_key => $status_name) {
			$value = $data[$ms_key][$status_key];
			if (!$value) { $value = 0; }
			$row[] = array('data' => $value, 'width' => '10%');
		}
		$row[] = array('data' => $data[$ms_key]['unassigned'], 'width' => '10%');
		$row[] = array('data' => $data[$ms_key]['total'], 'width' => '10%');
		$rows[] = $row;
	}

	$row = array();
	$row[] = array('data' => '<strong>' . t('Total') . '</strong>', 'width' => '40%');
	foreach ($status_options as $status_key => $status_name) {
		$value = $grand_total[$status_key];
		if (!$value) { $value = 0; }
		$row[] = array('data' => "<strong>$value</strong>", 'width' => '10%');
	}
	$value = $grand_total['unassigned'];
	if (!$value) { $value = 0; }
	$row[] = array('data' => "<strong>$value</strong>", 'width' => '10%');
	$value = $grand_total['total'];
	if (!$value) { $value = 0; }
	$row[] = array('data' => "<strong>$value</strong>", 'width' => '10%');
	$rows[] = $row;

	// Compute the "total requested" and "grand total requested". We exclude reserve sheets from this count
	$total_requested = array();
	$grand_total_requested = 0;
	foreach ($sheet_nodes as $sheet_nid => $sheet_node) {
		$status_key = $sheet_node->field_sirius_edls_sheet_status['und'][0]['value'];
		if ($status_key == 'reserved') { continue; }
		$value = $sheet_node->field_sirius_count['und'][0]['value'];
		if (!$value) { $value = 0; }
		$total_requested[$status_key] += $value;
		$grand_total_requested += $value;
	}
	$variance = $grand_total['total'] - $grand_total_requested;

	// Add the "total requested" row to the table
	$row = array();
	$row[] = array('data' => '<strong>' . t('Total Demand') . '</strong>', 'width' => '40%');
	foreach ($status_options as $status_key => $status_name) {
		if ($status_key == 'reserved') {
			$value = 'N/A';
		} else {
			$value = $total_requested[$status_key];
		}
		if (!$value) { $value = ''; }
		$row[] = array('data' => "<strong>$value</strong>", 'width' => '10%');
	}
	$row[] = array('data' => "<strong></strong>", 'width' => '10%');
	$row[] = array('data' => "<strong>$grand_total_requested</strong>", 'width' => '10%');
	$rows[] = $row;

	// Add the Variance row to the table
	$row = array();
	$row[] = array('data' => '<strong>' . t('Variance') . '</strong>', 'width' => '40%');
	foreach ($status_options as $status_key => $status_name) {
		$row[] = array('data' => "<strong></strong>", 'width' => '10%');
	}
	$row[] = array('data' => "<strong></strong>", 'width' => '10%');
	$row[] = array('data' => "<strong>$variance</strong>", 'width' => '10%');
	$rows[] = $row;

	$header = array();
	$header[] = array('data' => t('Member Status'), 'width' => '40%');
	foreach ($status_options as $status_key => $status_name) {
		$header[] =  array('data' => $status_name, 'width' => '10%');
	}
	$header[] = array('data' => t('Unassigned'), 'width' => '10%');
	$header[] = array('data' => t('Total'), 'width' => '10%');

	$html .= sirius_table_render($header, $rows, array('class' => array('sirius_table_basic sirius_table_align')));

	$html .= '</div>';

	return $html;
}