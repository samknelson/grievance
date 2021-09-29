<?php

function sirius_worker_page_search_by_phone_carrier() {
	return drupal_get_form('sirius_worker_page_search_by_phone_carrier_form');
}

function sirius_worker_page_search_by_phone_carrier_form($form, &$form_state) {

	$form['carrier_name'] = array(
		'#type' => 'textfield',
		'#title' => t('Carrier Name'),
		'#description' => t('Enter all or part of the carrier name, e.g. "T-Mobile". Special characters such as & are not supported.'),
	);

	$form['output'] = array(
		'#type' => 'select',
		'#title' => t('Output'),
		'#options' => array(
			'screen' => t('Screen - Show results on the screen'),
			'csv' => t('CSV - Export results to a spreadsheet'),
		),
		'#required' => TRUE,
	);
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit')
	);

	if ($form_state['results']) {
		$form['results'] = array('#markup' => '<hr>' . $form_state['results']);
	}

	return $form;
}

function sirius_worker_page_search_by_phone_carrier_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;

	$output = $form_state['values']['output'];

	$carrier_name = $form_state['values']['carrier_name'];
	$carrier_name = preg_replace('/[^a-zA-Z0-9_\- ]/', '', $carrier_name);

	$sql_args = array();
	$domain_nid = sirius_domain_nid();

	$sql = "select node.nid, node.title, ";
	$sql .= "field_data_field_sirius_id.field_sirius_id_value, ";
	$sql .= "field_sirius_phone_value, ";
	$sql .= "json_unquote(json_extract(field_sirius_json_value, '\$.result.carrier.name')) as carrier_name, ";
	$sql .= "p_node.nid as p_nid ";
	$sql .= " from node ";
	$sql .= "left join field_data_field_sirius_phone on field_data_field_sirius_phone.entity_type = 'node' and field_data_field_sirius_phone.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_id on field_data_field_sirius_id.entity_type = 'node' and field_data_field_sirius_id.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_id p_id on p_id.entity_type = 'node' and p_id.field_sirius_id_value = field_data_field_sirius_phone.field_sirius_phone_value and p_id.bundle = 'sirius_phonenumber' ";
	$sql .= "left join node p_node on p_node.nid = p_id.entity_id ";
	$sql .= "left join field_data_field_sirius_domain p_domain on p_domain.entity_type = 'node' and p_domain.entity_id = p_node.nid ";
	$sql .= "left join field_data_field_sirius_json p_json on p_json.entity_type = 'node' and p_json.entity_id = p_node.nid ";
	$sql .= "where node.type = 'sirius_worker' ";
	$sql .= "and field_sirius_phone_value is not null ";
	$sql .= "and field_sirius_phone_value != '' ";
	if ($domain_nid) {
		$sql .= "and field_data_field_sirius_domain.field_sirius_domain_target_id = :domain_nid ";
		$sql_args[':domain_nid'] = $domain_nid;
		$sql .= "and p_domain.field_sirius_domain_target_id = :domain_nid2 ";
		$sql_args[':domain_nid2'] = $domain_nid;
	}
	$sql .= "order by node.nid ";
	$stmt = sirius_sql_query($sql, $sql_args);

	$header = array('Worker', 'NID', 'ID', 'Carrier');
	if ($output == 'screen') {
		$header[] = 'Tools';
	}

	if ($output == 'csv') {
    $filename = 'carriers.csv';
    drupal_add_http_header('Content-Type', 'text/csv');
    drupal_add_http_header('Content-Disposition', 'attachment;filename=' . $filename);
    $fp = fopen('php://output', 'w');
    fputcsv($fp, $header);
	}
	$rows = array();
	while ($hr = $stmt->fetchAssoc()) {
		if (!preg_match("/$carrier_name/i", $hr['carrier_name'])) { continue; }

		$tools = l('Worker', 'node/' . $hr['nid'], array('attributes' => array('class' => array('tool-button'))));
		$tools .= l('Phone Number', 'node/' . $hr['p_nid'], array('attributes' => array('class' => array('tool-button'))));

		$row = array();
		$row[] = $hr['title'];
		$row[] = $hr['nid'];
		$row[] = $hr['field_sirius_id_value'];
		$row[] = $hr['carrier_name'];
		if ($output == 'screen') {
			$row[] = $tools;
		}

		if ($output == 'csv') {
			fputcsv($fp, $row); 
		} else {
			$rows[] = $row;
		}
	}

	if ($output == 'csv') { 
		fclose($fp);
		exit();
	}

	$html = '<h2>' . t('Results') . '</h2>';
	if (!$rows) {
		$html .= '<p>' . t('No matching records found.') . '</p>';
	} else {
		$html .= theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => array()));
	}

	$form_state['results'] = $html;

	return;
}