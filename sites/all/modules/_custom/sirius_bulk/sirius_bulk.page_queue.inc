<?php

function sirius_bulk_node_tab_queue($bulk_node) {
	$sql = "select field_sirius_type_value, count(*) as c ";
	$sql .= "from node ";
	$sql .= "join field_data_field_sirius_type on field_data_field_sirius_type.entity_type = 'node' and field_data_field_sirius_type.entity_id = node.nid ";
	$sql .= "join field_data_field_sirius_category on field_data_field_sirius_category.entity_type = 'node' and field_data_field_sirius_category.entity_id = node.nid ";
	$sql .= "join field_data_field_sirius_log_handler on field_data_field_sirius_log_handler.entity_type = 'node' and field_data_field_sirius_log_handler.entity_id = node.nid ";
	$sql .= "where node.type = 'sirius_log' ";
	$sql .= "and field_sirius_category_value = 'bulk:queue' ";
	$sql .= "and field_sirius_log_handler_target_id = :bulk_nid ";
	$sql .= "group by field_sirius_type_value ";
	$sql .= "order by field_sirius_type_value ";

	$stmt = db_query($sql, array(':bulk_nid' => $bulk_node->nid));
	$rows = array();
	while ($hr = $stmt->fetchAssoc()) {
		$row = array();
		$row[] = $hr['field_sirius_type_value'];
		$row[] = number_format($hr['c']);
		$rows[] = $row;
	}
	$header = array('Status', 'Count');

	$html .= '<h2>' . t('Summary') . '</h2>';
	$html .= theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => array()));

	$html .= '<h2>' . t('Queue') . '</h2>';
	$html .= views_embed_view('sirius_bulk_queue', 'block', $bulk_node->nid);

	return $html;
}