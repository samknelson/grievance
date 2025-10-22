<?php

function sirius_btu_page_worker_export() {
	return drupal_get_form('sirius_btu_page_worker_export_form');
}

function sirius_btu_page_worker_export_form($form, &$form_state) {
	$form['ids'] = array(
		'#type' => 'textarea',
		'#title' => t('Worker IDs'),
		'#description' => t('Enter a list of worker IDs or SSNs, one per line.'),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_btu_page_worker_export_form_submit($form, &$form_state) {
	$ids = preg_split('/\s*\n\s*/', $form_state['values']['ids']);

	$addr_cols = sirius_addr_cols();

	$header = array();
	$header[] = 'Input';
	$header[] = 'First Name';
	$header[] = 'Middle Name';
	$header[] = 'Last Name';
	$header[] = 'ID';
	$header[] = 'Phone';
	$header[] = 'Email';
	foreach ($addr_cols as $key => $label) { $header[] = $label; }

	$filename = "btu_contact_export_" . date('c') . '.csv';
	drupal_add_http_header('Content-Type', 'text/csv');
	drupal_add_http_header('Content-Disposition', 'attachment;filename=' . $filename);
	$fp = fopen('php://output', 'w');
	fputcsv($fp, $header);

	foreach ($ids as $id) {
		$worker_node = NULL;

		if (sirius_ssn_validate($id)) {
			$worker_node = sirius_worker_get_by_ssn(sirius_ssn_format($id));
		}

		if (!$worker_node) {
			$worker_node = sirius_worker_get_by_id($id);
		}

		$row = array();
		$row[] = $id;
		if ($worker_node) {
			$row[] = $worker_node->field_sirius_name['und'][0]['given'];
			$row[] = $worker_node->field_sirius_name['und'][0]['middle'];
			$row[] = $worker_node->field_sirius_name['und'][0]['family'];
			$row[] = $worker_node->field_sirius_id['und'][0]['value'];
			$row[] = $worker_node->field_sirius_phone['und'][0]['value'];
			$row[] = $worker_node->field_sirius_email['und'][0]['value'];
			foreach ($addr_cols as $key => $label) { $row[] = $worker_node->field_sirius_address['und'][0][$key]; }
		}

		fputcsv($fp, $row);
	}
	fclose($fp);
	drupal_exit();
}