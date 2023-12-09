<?php

function sirius_edls_summary($employer_node, $date, $options = array()) {
	$html .= '<div class="sirius_edls_summary">';

	$ms_list = sirius_edls_ms_list($employer_node);
	$status_options = sirius_edls_sheet_status_options();
	$sheet_nodes = array();

	foreach (array('today' => t('Today'), '+1 day' => t('Tomorrow')) as $d => $d_name) {
		$html .= '<h2>' . $d_name . '</h2>';
		$date = date('Y-m-d', strtotime($d));
		$workers = sirius_edls_worker_list($employer_node->nid, $date, $filters = array());

		$data = array();
		foreach ($workers as $worker) {
			$ms_key = $worker['worker_ms'];
			$data[$ms_key]['total']++;

			$sheet_nid = $worker['curr_sheet_nid'];
			if ($sheet_nid) {
				if (!$sheet_nodes[$sheet_nid]) { $sheet_nodes[$sheet_nid] = node_load($sheet_nid); }
				$status = $sheet_nodes[$sheet_nid]->field_sirius_edls_sheet_status['und'][0]['value'];
				$data[$ms_key][$status]++;
			} else {
				$data[$ms_key]['unassigned']++;
			}

			$ms_key = $worker['worker_ms'];
		}

		$rows = array();
		foreach ($ms_list as $ms_key => $ms_name) {
			if (!$data[$ms_key]) { continue; }

			// Just in case
			$data[$ms_key]['unassigned'] += $data[$ms_key]['trash'];

			$row = array();
			$row[] = array('data' => $ms_name, 'width' => '40%');
			foreach ($status_options as $status_key => $status_name) {
				if ($status_key == 'trash') { continue; }
				$value = $data[$ms_key][$status_key];
				if (!$value) { $value = 0; }
				$row[] = array('data' => $value, 'width' => '10%');
			}
			$row[] = array('data' => $data[$ms_key]['unassigned'], 'width' => '10%');
			$row[] = array('data' => $data[$ms_key]['total'], 'width' => '10%');
			$rows[] = $row;
		}

		$header = array();
		$header[] = array('data' => t('Member Status'), 'width' => '40%');
		foreach ($status_options as $status_key => $status_name) {
			if ($status_key == 'trash') { continue; }
			$header[] =  array('data' => $status_name, 'width' => '10%');
		}
		$header[] = array('data' => t('Unassigned'), 'width' => '10%');
		$header[] = array('data' => t('Total'), 'width' => '10%');

		$html .= sirius_table_render($header, $rows, array('class' => array('sirius_table_basic sirius_table_align')));
	}

	$html .= '</div>';

	return $html;
}