<?php

function grievance_report_bytype() {
	return drupal_get_form('grievance_report_bytype_form');
}

function grievance_report_bytype_form($form, &$form_state) {

  $form['company'] = array(
    '#type' => 'entityreference',
    '#title' => t('Company'),
    '#era_entity_type' => 'node',
    '#era_bundles' => array('grievance_company'),
    '#era_query_settings' => array(),
  );

  $form['shop'] = array(
    '#type' => 'entityreference',
    '#title' => t('Shop'),
    '#era_entity_type' => 'node',
    '#era_bundles' => array('grievance_shop'),
    '#era_query_settings' => array(),
  );

  $form['start_date'] = array(
    '#title' => t('Start Date'),
    '#date_format' => 'm/d/Y',
    '#type' => 'date_popup',
    '#default_value' => NULL,
	);

  $form['end_date'] = array(
    '#title' => t('End Date'),
    '#date_format' => 'm/d/Y',
    '#type' => 'date_popup',
    '#default_value' => NULL,
	);

  $form['expand_type'] = array(
    '#title' => t('Expand Type?'),
    '#type' => 'select',
    '#options' => array(
    	'0' => t('No, show only general grievance types'),
    	'1' => t('Yes, show detailed breakdown of grievance types')
    )
	);

  $form['expand_status'] = array(
    '#title' => t('Expand Status?'),
    '#type' => 'select',
    '#options' => array(
    	'0' => t('No, show only top-level statuses'),
    	'1' => t('Yes, show detailed breakdown of grievance status')
    )
	);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Generate Report',
  );

  if ($form_state['results']) {
  	$form['results'] = array(
  		'#type' => 'markup',
  		'#markup' => $form_state['results'],
  	);
  }

  return $form;
}

function grievance_report_bytype_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;

	$shop_nid = $form_state['values']['shop']['entity_id'];
	$company_nid = $form_state['values']['company']['entity_id'];

	$consolidate_status = !$form_state['values']['expand_status'];
	$consolidate_type = !$form_state['values']['expand_type'];

	$start_date = $form_state['values']['start_date'];
	if ($start_date) { $start_ts = strtotime($start_date); }

	$end_date = $form_state['values']['end_date'];
	if ($end_date) { $end_ts = strtotime($end_date); }

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

	$args = array();
	$sql = "select field_grievance_status_tid, field_grievance_type_tid, count(*) as c ";
	$sql .= "from node grievance_node ";
	$sql .= "left join field_data_field_grievance_status on (";
	$sql .= "    field_data_field_grievance_status.entity_id = grievance_node.nid ";
	$sql .= "    and field_data_field_grievance_status.entity_type = 'node' ";
	$sql .= "    and field_data_field_grievance_status.deleted = 0 ";
	$sql .= ") ";
	$sql .= "left join field_data_field_grievance_type on (";
	$sql .= "    field_data_field_grievance_type.entity_id = grievance_node.nid ";
	$sql .= "    and field_data_field_grievance_type.entity_type = 'node' ";
	$sql .= "    and field_data_field_grievance_type.deleted = 0 ";
	$sql .= ") ";
	$sql .= "left join field_data_field_grievance_shop on (";
	$sql .= "    field_data_field_grievance_shop.entity_id = grievance_node.nid ";
	$sql .= "    and field_data_field_grievance_shop.entity_type = 'node' ";
	$sql .= "    and field_data_field_grievance_shop.deleted = 0 ";
	$sql .= ") ";
	$sql .= "left join field_data_field_grievance_company on (";
	$sql .= "    field_data_field_grievance_company.entity_id = field_data_field_grievance_shop.field_grievance_shop_target_id ";
	$sql .= "    and field_data_field_grievance_company.entity_type = 'node' ";
	$sql .= "    and field_data_field_grievance_company.deleted = 0 ";
	$sql .= ") ";
	$sql .= "where grievance_node.type = 'grievance' ";
	if ($company_nid) {
		$sql .= "and field_data_field_grievance_company.field_grievance_company_target_id = :company_nid ";
		$args[':company_nid'] = $company_nid;
	}
	if ($shop_nid) {
		$sql .= "and field_data_field_grievance_shop.field_grievance_shop_target_id = :shop_nid ";
		$args[':shop_nid'] = $shop_nid;
	}
	if ($start_ts) {
		$sql .= "and grievance_node.created >= :start_ts ";
		$args[':start_ts'] = $start_ts;
	}
	if ($end_ts) {
		$sql .= "and grievance_node.created <= :end_ts ";
		$args[':end_ts'] = $end_ts;
	}
	$sql .= "group by field_grievance_status_tid, field_grievance_type_tid ";
	$stmt = db_query($sql, $args);
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

	$html = '<hr>';

	$html .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array()));

	$form_state['results'] = $html;

	return $html;
}