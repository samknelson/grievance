<?php

function sirius_ledger_page_transmittal_byemployer() {
	return drupal_get_form('sirius_ledger_page_transmittal_byemployer_form');
}

function sirius_ledger_page_transmittal_byemployer_form($form, &$form_state) {
  $this_month = date('n');
  $months = array(
    '1' => '1  January',
    '2' => '2  February',
    '3' => '3  March',
    '4' => '4  April',
    '5' => '5  May',
    '6' => '6  June',
    '7' => '7  July',
    '8' => '8  August',
    '9' => '9  September',
    '10' => '10 October',
    '11' => '11 November',
    '12' => '12 December',
  );
	$form['month'] = array(
		'#type' => 'select',
		'#options' => $months,
		'#default_value' => $this_month,
		'#title' => t('End Month'),
		'#required' => TRUE,
	);

	$this_year = date('Y');
	$years = array();
	for ($year = $this_year-10; $year<=$this_year; ++$year) { $years[$year] = $year; }
	$form['year'] = array(
		'#type' => 'select',
		'#options' => $years,
		'#default_value' => $this_year,
		'#title' => t('End Year'),
		'#required' => TRUE,
	);

	$form['month_count'] = array(
		'#type' => 'select',
		'#options' => array(
			1 => t('Show one month'),
			3 => t('Show three months'),
			6 => t('Show six months'),
			12 => t('Show twelve months'),
		),
		'#default_value' => 6,
		'#title' => 'Months to Show',
		'#required' => TRUE,
	);

	$accounts = sirius_ledger_account_list();
	$form['account'] = array(
		'#type' => 'select',
		'#options' => array(NULL => t('-- Select --')) + $accounts,
		'#default_value' => 6,
		'#title' => 'Account',
		'#required' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	if ($form_state['results']) {
		$form['results'] = array('#markup' => '<hr>' . $form_state['results']);
	}

	return $form;
}

function sirius_ledger_page_transmittal_byemployer_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;

	$year = $form_state['values']['year'];
	$month = $form_state['values']['month'];
	$month_count = $form_state['values']['month_count'];
	$account_nid = $form_state['values']['account'];

	$params = array();
	$params['account_nid'] = $account_nid;
	$transmittal_data = array();
	for ($i=0; $i<$month_count; ++$i) {
		$params['year'] = $year;
		$params['month'] = $month;

		$records = sirius_ledger_transmittal_list($params);
		foreach ($records as $record) {
			$transmittal_data[$record['employer_nid']][$year][$month] = $record;
		}
		--$month; if ($month < 1) { $month = 12; --$year; }
	}

	$employers = sirius_employer_list();
	$rows = array();
	foreach ($employers as $employer_nid => $employer_name) {
		$row = array();
		$row[] = $employer_name;

		$year = $form_state['values']['year'];
		$month = $form_state['values']['month'];
		for ($i=0; $i<$month_count; ++$i) {
			$status = $transmittal_data[$employer_nid][$year][$month]['status'];

			if ($status) {
				$transmittal_nid = $transmittal_data[$employer_nid][$year][$month]['transmittal_nid'];
				$payment_nid = $transmittal_data[$employer_nid][$year][$month]['payment_nid'];
				$dollar_amt = sirius_ledger_ar_format_balance($transmittal_data[$employer_nid][$year][$month]['dollar_amt']);
				$status = l($status, "node/$employer_nid/sirius_employer_ledger/transmittal/edit/$transmittal_nid", array('attributes' => array('class' => array('tool-button'))));
				if ($payment_nid) {
					$status .= l($dollar_amt, "node/$payment_nid/sirius_payment_allocate/transmittal", array('html' => TRUE, 'attributes' => array('class' => array('tool-button'))));
				}
			} else {
				$status = '';
			}
			$row[] = $status;
			--$month; if ($month < 1) { $month = 12; --$year; }
		}

		$tools = l('View All', "node/$employer_nid/sirius_employer_ledger/transmittal", array('attributes' => array('class' => array('tool-button'))));
		$row[] = $tools;
		$rows[] = $row;
	}

	$header = array();
	$header[] = t('Employer');
	$year = $form_state['values']['year'];
	$month = $form_state['values']['month'];
	for ($i=0; $i<$month_count; ++$i) {
		$header[] = "$month/$year";
		--$month; if ($month < 1) { $month = 12; --$year; }
	}
	$header[] = t('Tools');

	$form_state['results'] = theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array()));
}