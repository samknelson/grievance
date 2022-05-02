<?php

function report_cache_page_auto_test() {
	return drupal_get_form('report_cache_page_auto_test_form');
}

function report_cache_page_auto_test_form($form, &$form_state) {
  $form['force'] = array(
	  '#type' => 'select',
	  '#title' => t('Force?'),
	  '#options' => array(
	    'no' => t('No, poll normally'),
	    'yes' => t('Yes, run all auto jobs now'),
	  ),
	  '#description' => t('Force will disregard time limitations.'),
	  '#default_value' => 'no',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  if ($form_state['results']) {
	  $form['results'] = array(
			'#markup' => '<h2>' . t('Results') . '</h2><hr>' . $form_state['results'],
	  );
  }

  return $form;
}

function report_cache_page_auto_test_form_submit($form, &$form_state) {
  if ($form_state['values']['force'] == 'yes') { $force = TRUE; } else { $force = FALSE; }

  $result = report_cache_auto_all($force);
  drupal_set_message($result['msg']);

  $form_state['rebuild'] = TRUE;
  $form_state['results'] = sirius_minilog_render_html();
}
