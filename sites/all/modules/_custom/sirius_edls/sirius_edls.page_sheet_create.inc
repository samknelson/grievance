<?php

function sirius_edls_page_sheet_create() {
	$result = sirius_edls_employer_switch();
	$html = $result['markup'];
	$employer_node = $result['employer_node'];
	if (!$employer_node) {
		$html .= sirius_page_error('Please select an employer.');
		return $html;
	}

	return drupal_get_form('sirius_edls_sheet_form', NULL, $employer_node);
}