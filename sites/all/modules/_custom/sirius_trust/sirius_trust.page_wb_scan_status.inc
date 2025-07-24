<?php

function sirius_trust_page_wb_scan_status() {
	return drupal_get_form('sirius_trust_page_wb_scan_status_form');
}

function sirius_trust_page_wb_scan_status_form($form, &$form_state) {
	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/ux.css');
	$form['#tree'] = TRUE;

	$status = sirius_domain_variable_get('sirius_trust_wb_scan_status', array());

	$html = '<div class="sirius_callout_fullwidth">';

	if ($status['total']) {
		$pct = 100 * round($status['complete'] / $status['total'], 2);
 		$html .= theme_progress_bar(
 			array(
				'percent' => $pct, 
				'message' => "$status[complete] / $status[total] records processed ($pct%)",
 			)
 		);
 		$html .= '<br />';
 	}


	$html .= sirius_fakerender_field('Month:', $status['ym']);
	$html .= sirius_fakerender_field('Pending:', number_format($status['pending'], 0));
	$html .= sirius_fakerender_field('Complete:', number_format($status['complete']));
	$html .= sirius_fakerender_field('Total:', number_format($status['total']));
	$html .= sirius_fakerender_field('Batch Size:', $status['limit']);
	$html .= sirius_fakerender_field('Mode:', $status['mode']);

	$last_worker_nid = $status['last_worker_nid'];
	if ($last_worker_nid) {
		$last_worker_node = node_load($last_worker_nid);
		$html .= sirius_fakerender_field('Last Worker:', l($last_worker_node->title . ' (' . $last_worker_nid . ')', "node/$last_worker_nid"));
	}

	$instance = Sirius_Trust_Wb_Scan_Changelog::getInstance();
	$html .= sirius_fakerender_field('Change Log Entries:', number_format($instance->count(), 0));

	$cron_nid = sirius_cron_scan_get_nid('trust_benefit');
	if ($cron_nid) {
		$path = "node/$cron_nid/sirius_cron_scan_info";
		if (drupal_valid_path($path)) {
			$html .= '<br />' . l('Cron Status', $path, array('attributes' => array('class' => array('tool-button'))));
		}
	}

	$html .= '</div>';

	$form['intro'] = array('#markup' => $html);

	$form['settings'] = array(
		'#type' => 'fieldset',
		'#title' => 'Settings',
		'#description' => t('Use the form below to configure the scan settings.'),
	);

	$form['settings']['limit'] = array(
		'#type' => 'textfield',
		'#title' => t('Batch Size'),
		'#default_value' => $status['limit'],
	);

	$form['settings']['mode'] = array(
		'#type' => 'select',
		'#title' => t('Mode'),
		'#default_value' => $status['mode'],
		'#options' => array(
			'test' => t('Test'),
			'live' => t('Live'),
		),
	);

	$form['settings']['reset'] = array(
		'#type' => 'select',
		'#title' => t('Reset?'),
		'#options' => array(
			'No' => t('No, do not reset'),
			'Yes' => t('Yes, force a rescan of all workers this month'),
		),
	);

	$form['settings']['confirm'] = array(
		'#type' => 'checkbox',
		'#title' => 'Click here to confirm',
		'#required' => TRUE,
	);

	$form['settings']['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_trust_page_wb_scan_status_form_validate($form, &$form_state) {
	if (!$form_state['values']['settings']['limit'] || 
		($form_state['values']['settings']['limit'] < 0) ||
		(!is_int($form_state['values']['settings']['limit']+0))) {
		form_set_error('settings][limit', 'Please enter a valid integer.');
	}
}


function sirius_trust_page_wb_scan_status_form_submit($form, &$form_state) {
	$instance = Sirius_Trust_Wb_Scan_Changelog::getInstance();

	$status = sirius_domain_variable_get('sirius_trust_wb_scan_status', array());
	$status['limit'] = $form_state['values']['settings']['limit'];
	$status['mode'] = $form_state['values']['settings']['mode'];
	if ($form_state['values']['settings']['reset'] == 'Yes') {
		unset($status['ym']);
		unset($status['last_worker_nid']);
		unset($status['pending']);
		unset($status['complete']);
	}

	sirius_domain_variable_set('sirius_trust_wb_scan_status', $status);
	drupal_set_message("Settings saved.");
}