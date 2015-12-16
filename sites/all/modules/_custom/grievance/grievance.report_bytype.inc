<?php

function grievance_report_bytype() {

	$consolidate_status = !$_REQUEST['expand_status'];
	$consolidate_type = !$_REQUEST['expand_type'];

	$html = '';
	$html .= l(t('default'), current_path());
	$html .= ' // '; 
	$html .= l(t('expand type'), current_path(), array('query' => array('expand_type' => 1)));
	$html .= ' // '; 
	$html .= l(t('expand status'), current_path(), array('query' => array('expand_status' => 1)));
	$html .= ' // '; 
	$html .= l(t('expand both'), current_path(), array('query' => array('expand_type' => 1, 'expand_status' => 1)));


	$grievance_type_vid = taxonomy_vocabulary_machine_name_load('grievance_types')->vid;
	$grievance_type_terms = taxonomy_get_tree($grievance_type_vid);
	$grievance_type_map = array();
	foreach ($grievance_type_terms as &$term) {
		$grievance_type_map[$term->tid] = $term; 
	}

	$grievance_status_vid = taxonomy_vocabulary_machine_name_load('grievance_status')->vid;
	$grievance_status_terms = taxonomy_get_tree($grievance_status_vid);
	$grievance_status_map = array();
	foreach ($grievance_status_terms as &$term) {
		$grievance_status_map[$term->tid] = $term; 
	}

	$sql = "select field_grievance_status_tid, field_grievance_type_tid, count(*) as c ";
	$sql .= "from field_data_field_grievance_status, field_data_field_grievance_type ";
	$sql .= "where field_data_field_grievance_status.entity_id = field_data_field_grievance_type.entity_id ";
	$sql .= "and field_data_field_grievance_status.entity_type = 'node' ";
	$sql .= "and field_data_field_grievance_status.bundle = 'grievance' ";
	$sql .= "and field_data_field_grievance_status.deleted = 0 ";
	$sql .= "and field_data_field_grievance_type.entity_type = 'node' ";
	$sql .= "and field_data_field_grievance_type.bundle = 'grievance' ";
	$sql .= "and field_data_field_grievance_type.deleted = 0 ";
	$sql .= "group by field_grievance_status_tid, field_grievance_type_tid ";
	$stmt = db_query($sql);
	$stmt->execute();
	$counts = array();
	while ($hr = $stmt->fetchAssoc()) {
		$status_tid = $hr['field_grievance_status_tid'];
		$type_tid = $hr['field_grievance_type_tid'];

		$status_term = $grievance_status_map[$status_tid];
		$type_term = $grievance_type_map[$type_tid];

		if ($consolidate_status) {
			if ($status_term->parents[0]) {
				$status_tid = $status_term->parents[0];
			}
		}

		if ($consolidate_type) {
			if ($type_term->parents[0]) {
				$type_tid = $type_term->parents[0];
			}
		}

		$counts[$status_tid][$type_tid] += $hr['c'];
		$counts[0][$type_tid] += $hr['c'];
		$counts[$status_tid][0] += $hr['c'];
		$counts[0][0] += $hr['c'];
	}

	$header = array();
	$header[] = t('Type');
	foreach ($grievance_status_terms as $status_term) {
		if ($consolidate_status && $status_term->parents[0]) {  continue; }
		$header[] = $status_term->name;
	}
	$header[] = "Total";

	$rows = array();
	foreach ($grievance_type_terms as $type_term) {
		if ($consolidate_type && $type_term->parents[0]) {  continue; }
		$type_tid = $type_term->tid;
		$row = array();

		$row[] = $type_term->name;
		foreach ($grievance_status_terms as $status_term) {
			if ($consolidate_status && $status_term->parents[0]) {  continue; }

			$status_tid = $status_term->tid;
			$row[] = number_format($counts[$status_tid][$type_tid]);
		}
		$row[] = '<strong>' . number_format($counts[0][$type_tid]) . '</strong>';

		$rows[] = $row;
	}

	$row = array();
	$row[] = 'Total';
	foreach ($grievance_status_terms as $status_term) {
		if ($consolidate_status && $status_term->parents[0]) {  continue; }
		$status_tid = $status_term->tid;
		$row[] = '<strong>' . number_format($counts[$status_tid][0]) . '</strong>';
	}
	$row[] = '<strong>' . number_format($counts[0][0]) . '</strong>';
	$rows[] = $row;

	$html .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array()));

	return $html;
}