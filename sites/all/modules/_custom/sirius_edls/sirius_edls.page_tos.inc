<?php

function sirius_edls_page_tos() {
	$result = sirius_edls_employer_switch();
	$html = $result['markup'];
	$employer_node = $result['employer_node'];
	if (!$employer_node) {
		$html .= sirius_page_error('Please select an employer.');
		return $html;
	}

	// Should use the TOS object, but components and classes and fuck it.
	$sql_args = array();
	$sql = "select distinct node.nid from node ";
	$sql .= "left join field_data_field_sirius_category on field_data_field_sirius_category.entity_type = 'node' and field_data_field_sirius_category.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_type on field_data_field_sirius_type.entity_type = 'node' and field_data_field_sirius_type.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
	$sql .= "where node.type = 'sirius_log' ";
	$sql .= "and field_sirius_category_value = 'tos' ";
	$sql .= "and field_sirius_type_value = 'active' ";
	$sql .= "order by node.created desc, node.nid desc ";
	$sql .= "limit 1000 ";
	$stmt = sirius_sql_query($sql, $sql_args);

	$tos_nids = array();
	while ($hr = $stmt->fetchAssoc()) {
		$tos_nids[] = $hr['nid'];
	}

	$tos_nodes = node_load_multiple($tos_nids);

	$rows = array();
	foreach ($tos_nodes as $tos_nid => $tos_node) {
		$worker_node = sirius_log_handler($tos_node, 'sirius_worker');

		$row = array();
		$row[] = $worker_node->title;
		$row[] = date('Y-m-d h:i a', $tos_node->created);

		$assignments_render = array();
		$assignments_render_today = '';
		$params = array();
		$params['grievance_shop'] = $employer_node->nid;
		$params['date_min'] = date('Y-m-d');
		$params['worker_id'] = 'I' . $worker_node->field_sirius_id['und'][0]['value'];
		$assignments = sirius_edls_assignment_search($params);
		$today_d = date('Y-m-d');
		foreach ($assignments as $assignment) {
			$sheet_node = node_load($assignment['sheet_nid']);
			$sheet_json = sirius_json_get($sheet_node);
			$ts = strtotime($assignment['field_sirius_date_start_value']);
			$d = date('Y-m-d', $ts);

			if ($d == $today_d) {
				$start_time = $assignment['assignment_extra']['time'];
				if (!$start_time) {
					$crews = sirius_edls_crews($sheet_node, 'uuid');
					$crew = $crews[$assignment['crew_uuid']];
					$start_time = $crew['start_time'];
				}

				$assignments_render_today = l(
					date('D m/d', $ts) . ' ' . $start_time . '<br />' . sirius_edls_supervisor_render($sheet_node, array('name_only' => TRUE)) . '<br /> ' . sirius_node_title($sheet_node->field_sirius_dispatch_job_group['und'][0]['target_id']),
					'node/' . $assignment['sheet_nid'] . '/sirius_edls_sheet_view',
					array('html' => TRUE, 'attributes' => array('class' => array('tool-button')))
				);
			} else {
				$assignments_render[] = l(
					date('D m/d', $ts) . '<br />' . sirius_edls_supervisor_render($sheet_node, array('name_only' => TRUE)), 
					'node/' . $assignment['sheet_nid'] . '/sirius_edls_sheet_view',
					array('html' => TRUE, 'attributes' => array('class' => array('tool-button')))
				);
			}
		}
		// $row[] = $tos_node->field_sirius_summary['und'][0]['value'];
		$row[] = $assignments_render_today;
		$row[] = join('', $assignments_render);

		$rows[] = $row;
	}

	$header = array();
	$header[] = array('data' => 'Name', 'x-width' => '15%');
	$header[] = array('data' => 'Out Since', 'x-width' => '15%');
	// $header[] = array('data' => 'Message', 'x-width' => '40%');
	$header[] = array('data' => 'Today\'s Assignment', 'x-width' => '30%');
	$header[] = array('data' => 'Future Assignments', 'x-width' => '30%');

	return sirius_table_render($header, $rows);
}
