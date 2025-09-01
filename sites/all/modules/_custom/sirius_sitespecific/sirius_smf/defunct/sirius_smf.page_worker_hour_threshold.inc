<?php

function sirius_smf_node_tab_worker_hour_threshold_form($form, &$form_state, $worker_node) {
	$form_state['worker_nid'] = $worker_node->nid;

	$form['#tree'] = TRUE;

	$form['industry_tid'] = array(
		'#type' => 'select',
		'#title' => t('Industry'),
		'#options' => array(NULL => t('Worker\'s current industry')) + sirius_taxonomy_load('sirius_industry', 'tid', 'name'),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

  $log = sirius_minilog_render_html();
  if ($log) {
		$form['results'] = array('#markup' => '<hr>' . $log);
  }

	return $form;
}

function sirius_smf_node_tab_worker_hour_threshold_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
	$worker_node = node_load($form_state['worker_nid']);

	$ignore_for_class_loading = sirius_trust_eligibility_plugin_info_all();

	$threshold = Sirius_Smf_Eligibility_Plugin_Buildup::getInstance()->hours_threshold_worker($worker_node, $form_state['values']['industry_tid']);

	drupal_set_message("Found hours threshold: $threshold");
}
