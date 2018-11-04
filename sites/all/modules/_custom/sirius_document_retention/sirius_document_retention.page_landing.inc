<?php

function sirius_document_retention_page_landing() {
	$html = '<div class="sirius-form-intro sirius-document-retention-intro">';
	$html .= t('The following document retention policies are configured.');
	$html .= '</div>';

	$list = sirius_document_retention_policy_list();
	$rows = array();
	foreach ($list as $key => $policy) {
		$row = array();

		$enabled = variable_get("sirius_document_retention_policy_$key");

		$tools = l('Configure', 'sirius/document-retention/configure/' . $key);

		$row[] = $key;

		if ($enabled) {
			$row[] = t('Enabled');
			$tools .= ' / ' . l('Execute', 'sirius/document-retention/execute/' . $key);
		} else {
			$row[] = t('Disabled');
		}
		$row[] = $policy['name'];
		$row[] = $policy['description'];
		$row[] = $tools;

		$rows[] = $row;
	}
	$header = array('Key', 'Enabled', 'Name', 'Description', 'Tools');

	$html .= theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => array()));

	return $html;
}