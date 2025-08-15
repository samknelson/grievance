<?php

function sirius_trust_page_wb_scan_changelog() {
	return drupal_get_form('sirius_trust_page_wb_scan_changelog_form');
}

function sirius_trust_page_wb_scan_changelog_form($form, &$form_state) {
	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/ux.css');

	$form['#tree'] = TRUE;

	$instance = Sirius_Trust_Wb_Scan_Changelog::getInstance();

	$count = $instance->count();
	$html = '<div class="sirius_callout_fullwidth">';
	$html .= sirius_fakerender_field('Total Entries:', number_format($count, 0));

	$html .= '<h2>Latest Entry</h2>';
	$latest_record = $instance->latest_record();
	$html .= $instance->render_record($latest_record);

	$html .= '</div>';
	$form['intro'] = array('#markup' => $html);

	$form['action'] = array(
		'#type' => 'select',
		'#title' => t('Action'),
		'#options' => array(
			NULL => t('-- Select --'),
			'download' => t('Download'),
			'purge' => t('Purge'),
		),
		'#required' => TRUE,
	);

	$form['purge_date'] = array(
		'#type' => 'textfield',
		'#title' => t('Purge entries older than'),
		'#states' => array('visible' => array(":input[name=\"action\"]" => array('value' => 'purge'))),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_trust_page_wb_scan_changelog_form_submit($form, &$form_state) {
	$instance = Sirius_Trust_Wb_Scan_Changelog::getInstance();

	$action = $form_state['values']['action'];
	if ($action == 'download') {
		drupal_add_http_header('Content-Type', 'text/csv');
		drupal_add_http_header('Content-Disposition', 'attachment;filename=wb_scan_changelog.csv');
		$fp = fopen('php://output', 'w');
		$instance->output_stream($fp);
		exit();
	}

	if ($action == 'purge') {
		$result = $instance->purge(strtotime($form_state['values']['purge_date']));
		drupal_set_message($result['msg']);
	}
}