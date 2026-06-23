<?php

function sirius_smf_page_ehrs_status() {
	return drupal_get_form('sirius_smf_page_ehrs_status_form');
}

function sirius_smf_page_ehrs_status_data() {
	sirius_require('sirius_smf', 'sirius_smf.ehrs.inc');
	$ehrs_monthly = Sirius_Smf_Ehrs_Monthly::getInstance();

	$status_options = $ehrs_monthly->status_options();

	$accounts = array();
	$accounts['EMPLOYER_CONTRIBUTIONS'] = array();
	$accounts['HWP'] = array();

	foreach ($accounts as $id => $record) {
		$nid = sirius_find_nid_by_id($id, 'sirius_ledger_account');
		if (!$nid) {
			drupal_set_message("Unable to find account with ID $id", 'error');
			return array();
		}
		$accounts[$id]['nid'] = $nid;
		$accounts[$id]['node'] = node_load($nid);
		$accounts[$id]['title'] = $accounts[$id]['node']->field_sirius_name_short['und'][0]['value'];
		if (!$accounts[$id]['title']) {
			$accounts[$id]['title'] = $accounts[$id]['node']->title;
		}
	}

	$csv = FALSE;

	$ea = Sirius_Ledger_EA::getInstance();
	$currency = sirius_ledger_currency_from_account_nid(NULL);

	$ts = time();
	$months = array();
	for ($i=0; $i<4; ++$i) {
		$month = array();
		$month['name'] = date('F', $ts);
		$month['y'] = date('Y', $ts);
		$month['m'] = date('n', $ts);
		$months[] = $month;
		$ts = strtotime('-1 month', $ts);
	}

	$employers = sirius_employer_list();

	$header = array();
	$header[] = t('Employer');
	foreach ($months as $month) { $header[] = $month['name']; }
	foreach ($accounts as $account_id => $account_record) {
		$header[] = $account_record['title'];
	}

	$rows = array();
	foreach ($employers as $employer_nid => $employer_name) {
		// if ($employer_nid != 7603259) { continue; }
		$employer_node = node_load($employer_nid);


		$row = array();
		$row[] = l($employer_name, "node/$employer_nid/contacts");

		$ts = $ts_now;
		foreach ($months as $month) {
			$eup_node = $ehrs_monthly->fetch($employer_nid, $month['y'], $month['m'], FALSE);
			if ($eup_node) {
	      $status = $ehrs_monthly->get_status($eup_node);
	      $color = $ehrs_monthly->get_color($status);
	      $status_render = $status_options[$status];
	    } else {
	    	$status = 'missing';
	    	$color = 'white';
	    	$status_render = '-';
	    }
      if (!$status_render) { $status_render = $status; }
      if ($eup_node) { $status_render = l($status_render, 'node/' . $eup_node->nid . '/sirius_smf_ehrs_wizard'); }
      $row[] = $status_render;
		}

		$balance = 0;


		$context = array();
		$context['entity_node'] = $employer_node;
		foreach ($accounts as $account_id => $account_record) {
			$context['account_node'] = $account_record['node'];
			$ea_node = $ea->fetch($employer_nid, $account_record['nid'], TRUE);
			$balance = $ea->balance($ea_node);
			$row[] = l($currency->render_amt(-1 * $balance), "node/$employer_nid/grievance_shop_ledger", array('html' => TRUE));
		}

		$rows[] = $row;
	}

	return array('header' => $header, 'rows' => $rows);
}

function sirius_smf_page_ehrs_status_form($form, &$form_state) {
	$data = sirius_smf_page_ehrs_status_data();
	$html = sirius_table_render($data['header'], $data['rows']);

	$form['action'] = array(
		'#type' => 'select',
		'#title' => t('Action'),
		'#options' => array(
			NULL => t('-- Select --'),
			'export' => t('Export'),
		),
		'#required' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	$form['intro'] = array('#markup' => $html);

	return $form;
}

function sirius_smf_page_ehrs_status_form_submit($form, &$form_state) {
	$action = $form_state['values']['action'];
	if ($action == 'export') {
		$data = sirius_smf_page_ehrs_status_data();
	}

  $filename = "Employer Upload Status " . date('c') . '.csv';

  drupal_add_http_header('Content-Type', 'text/csv');
  drupal_add_http_header('Content-Disposition', 'attachment;filename=' . $filename);
  $fp = fopen('php://output', 'w');
  fputcsv($fp, $data['header']);
  foreach ($data['rows'] as $row) {
  	if ($row['data']) { $row = $row['data']; }
    foreach ($row as $delta => $val) { $row[$delta] = strip_tags($val); }
    fputcsv($fp, $row);
  }
  drupal_exit();
}
