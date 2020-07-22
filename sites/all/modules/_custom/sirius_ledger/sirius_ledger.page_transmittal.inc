<?php

function sirius_ledger_page_transmittal() {
	return drupal_get_form('sirius_ledger_page_transmittal_form');
}

function sirius_ledger_page_transmittal_form($form, &$form_state) {
	$employers = sirius_employer_list();

	$form['#attributes'] = array('target' => '_blank');

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
	drupal_goto('sirius/ledger/transmittal/render', array('query' => array('employer_nids' => $employer_nids)));
}

function sirius_ledger_page_transmittal_render() {
	$employer_nodes = node_load_multiple($_GET['employer_nids']);
	if (!$employer_nodes) { return 'No employer selected.'; }
	$domain_nid = sirius_domain_nid();

	$html = '';
	foreach ($employer_nodes as $employer_nid => $employer_node) {
		if ($employer_node->field_sirius_domain['und'][0]['target_id'] != $domain_nid) {
			return 'You do not have access to one of the specified employers (wrong domain).';
		}
		$html .= '<div style="page-break-after: always; class="sirius_ledger_transmittal_wrapper">';
		$html .= drupal_render(drupal_get_form('sirius_ledger_node_tab_employer_transmittal_form', $employer_node));
		$html .= '</div>';
	}
	return $html;
}
