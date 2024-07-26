<?php

function sirius_ledger_node_tab_event_participant_test_form($form, &$form_state, $event_participant_node) {
	$form_state['event_participant_nid'] = $event_participant_node->nid;
	$form['mode'] = array(
		'#type' => 'select',
		'#title' => t('Mode'),
		'#required' => TRUE,
		'#options' => array(
			'test' => t('Test mode - execute all the steps but do not save any charges.'),
			'live' => t('Live mode - create or update A/R entries'),
		),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

  $log = sirius_minilog_render_html();
  if ($log) { $form['results'] = array('#markup' => '<hr>' . $log); }

	return $form;
}

function sirius_ledger_node_tab_event_participant_test_form_submit($form, &$form_state) {
	$event_participant_node = node_load($form_state['event_participant_nid']);
	$form_state['rebuild'] = TRUE;

	sirius_ledger_event_participant_set($event_participant_node, $form_state['values']['mode']);
	drupal_set_message("Ledger entries updated.");
}
