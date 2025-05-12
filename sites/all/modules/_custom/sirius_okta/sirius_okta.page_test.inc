<?php

function sirius_okta_page_test() {
	return drupal_get_form('sirius_okta_page_test_form');
}

function sirius_okta_page_test_form($form, &$form_state) {
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	if ($form_state['submitted']) {
		$form['results'] = array(
			'#markup' => '<h2>' . t('Results') . '</h2>' . '<hr>' . sirius_minilog_render_html(),
		);
	}

	return $form;
}

function sirius_okta_page_test_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
	sirius_minilog("Testing Okta connection...");
	sirius_minilog_indent();
	$okta = Sirius_Okta::getInstance();
	$result = SiriusResult::create($okta->connection_test());
	sirius_minilog_outdent();
	return $result->drupalSetMessage();
}
