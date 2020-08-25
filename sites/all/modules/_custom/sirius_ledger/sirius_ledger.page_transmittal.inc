<?php

function sirius_ledger_page_transmittal() {
	return drupal_get_form('sirius_ledger_page_transmittal_form');
}

function sirius_ledger_page_transmittal_form($form, &$form_state) {
	$employers = sirius_employer_list();

	$form['employers'] = array(
		'#type' => 'checkboxes',
		'#options' => $employers,
		'#title' => 'Generate Transmittal Sheets For',
		'#checkall' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	if ($form_state['results']) {
		$form['results'] = array('#markup' => $form_state['results']);
	}

	return $form;
}

function sirius_ledger_page_transmittal_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
	$employer_nids = array_filter(array_values($form_state['values']['employers']));
	$employer_nodes = node_load_multiple($employer_nids);
	if (!$employer_nodes) { 
		drupal_set_message("No employer selected", 'error');
		return;
	}
	$domain_nid = sirius_domain_nid();

	$html = '';
	$is_first = TRUE;
	foreach ($employer_nodes as $employer_nid => $employer_node) {
		if ($employer_node->field_sirius_domain['und'][0]['target_id'] != $domain_nid) {
			return 'You do not have access to one of the specified employers (wrong domain).';
		}
		if ($is_first) {
			$is_first = FALSE;
			$html .= '<div style="margin: 0; padding: 0;" class="sirius_ledger_transmittal_wrapper">';
		} else {
			$html .= '<div style="page-break-before: always; margin: 0; padding: 0;" class="sirius_ledger_transmittal_wrapper">';
		}
		$html .= sirius_ledger_node_tab_employer_transmittal_print($employer_node);
		$html .= '</div>';
	}
	// $form_state['results'] = $html;
	// return;

	$params = array();
	$params['post']['body'] = $html;
	$params['post']['css_paths'] = 'sites/all/modules/_custom/sirius_ledger/css/transmittal.css';
	$params['letterhead'] = sirius_domain_variable_get('sirius_ledger_transmittal_letterhead', '');
	$params['filename'] = 'transmittal_batch_' . date('Ymd') . '.pdf';
	sirius_pdf_stream(sirius_loopback_url() . '/sirius/loopback/echo', $params);
}
