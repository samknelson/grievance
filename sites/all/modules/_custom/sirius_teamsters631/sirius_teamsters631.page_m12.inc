<?php

function sirius_teamsters631_page_m12() {
	return drupal_get_form('sirius_teamsters631_page_m12_form');
}

function sirius_teamsters631_page_m12_form($form, &$form_state) {
	$year_now = date('Y');
	$year_options = array();
	for ($i=$year_now-10; $i<=$year_now; ++$i) { $year_options[$i] = $i; }
	$form['year'] = array(
		'#title' => t('Year'),
		'#type' => 'select',
		'#options' => $year_options,
		'#default_value' => $year_now,
	);

	$month_now = date('n');
	$month_options = array();
	for ($i=1; $i<=12; ++$i) { $month_options[$i] = $i; }
	$form['month'] = array(
		'#title' => t('Month'),
		'#type' => 'select',
		'#options' => $month_options,
		'#default_value' => $month_now,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}


function sirius_teamsters631_page_m12_form_submit($form, &$form_state) {
	$year = $form_state['values']['year'];
	$month = $form_state['values']['month'];
	drupal_goto(current_path() . '/' . "$year-$month");
}
