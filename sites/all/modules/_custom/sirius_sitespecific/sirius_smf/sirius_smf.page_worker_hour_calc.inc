<?php

function sirius_smf_node_tab_worker_hour_calc_form($form, &$form_state, $worker_node) {
	$form_state['worker_nid'] = $worker_node->nid;

	$form['#tree'] = TRUE;

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

function sirius_smf_node_tab_worker_hour_calc_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
	$worker_nid = $form_state['worker_nid'];

	sirius_smf_emphist_from_hours($worker_nid);
}
