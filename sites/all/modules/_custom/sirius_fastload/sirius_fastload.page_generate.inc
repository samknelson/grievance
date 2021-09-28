<?php

function sirius_fastload_page_generate() {
	return drupal_get_form('sirius_fastload_page_generate_form');
}

function sirius_fastload_page_generate_form($form, &$form_state) {
	$form['#tree'] = TRUE;

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

  $log = sirius_minilog_render_html();
	if ($log) { $form['results'] = array('#markup' => '<hr>' . $log); }

	return $form;
}

function sirius_fastload_page_generate_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
	sirius_fastload_generate();
}