<?php

function sirius_event_node_tab_participant_comments($participant_node) {
	return drupal_get_form('sirius_event_node_tab_participant_comments_form', $participant_node);
}

function sirius_event_node_tab_participant_comments_form($form, &$form_state, $participant_node) {
	$form_state['participant_nid'] = $participant_node->nid;

	$participant_json = sirius_jsonfield_util_node_json_get($participant_node);
	$prole_tid = $participant_node->field_sirius_event_prole['und'][0]['tid'];

	$form['#tree'] = TRUE;
	$form['participant']['comments'] = array(
		'#type' => 'textarea',
		'#title' => 'Comments',
		'#default_value' => $participant_json['comments'],
	);

  $form['pstatus_tid'] = array(
  	'#title' => t('Status'),
  	'#type' => 'select',
  	'#options' => sirius_event_participant_role_status_tids($prole_tid),
  	'#default_value' => $participant_node->field_sirius_event_pstatus['und'][0]['tid'],
  );

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_event_node_tab_participant_comments_form_submit($form, &$form_state) {
	$participant_node = node_load($form_state['participant_nid']);
	$participant_json = sirius_jsonfield_util_node_json_get($participant_node);
	if ($form_state['values']['participant']['comments'] != $participant_json['comments']) {
		$participant_json['comments'] = $form_state['values']['participant']['comments'];
		sirius_jsonfield_util_node_json_set($participant_node, $participant_json);
		sirius_jsonfield_log($participant_node, 'comment', 'Set comments to: ' . $form_state['values']['participant']['comments']);
		node_save($participant_node);
		drupal_set_message("Comments saved.");
	}

	if ($form_state['values']['pstatus_tid'] != $participant_node->field_sirius_event_pstatus['und'][0]['tid']) {
		$prole_tid = $participant_node->field_sirius_event_prole['und'][0]['tid'];
		$result = sirius_event_participant_pstatus_set($participant_node, $prole_tid, $form_state['values']['pstatus_tid']);
		if ($result['success']) { 
			drupal_set_message("Participant status saved.");
		} else {
			drupal_set_message($result['msg'], 'warning');
		}
	}
}
