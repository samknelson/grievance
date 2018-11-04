<?php

function sirius_document_retention_page_configure() {
	return drupal_get_form('sirius_document_retention_page_configure_form');
}

function sirius_document_retention_page_configure_form($form, &$form_state) {
	$form['intro'] = array(
		'#type' => 'markup',
		'#markup' => '<div class="sirius-form-intro sirius-document-retention-configure-intro">' . 
			t('Check the document retention policies that you would like to apply to this site.') .
			'</div>',
	);

	$policies = sirius_document_retention_policy_list();
	foreach ($policies as $key => $policy) {
		$form["sirius_document_retention_policy_$key"] = array(
			'#type' => 'checkbox',
			'#title' => $policy['name'],
			'#description' => $policy['description'] . ' (' . l('Configure', 'sirius/document-retention/configure/' . $key) . ')',
	    '#default_value' => variable_get("sirius_document_retention_policy_$key", ''),
		);
	}

	return system_settings_form($form);
}

function sirius_document_retention_page_configure_policy($key) {
	$policy = sirius_document_retention_policy_info($key);
	return drupal_get_form($policy['configure_callback'], $key);
}
