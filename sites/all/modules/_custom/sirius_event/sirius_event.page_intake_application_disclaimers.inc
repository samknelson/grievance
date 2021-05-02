<?php

function sirius_node_tab_event_intake_application_disclaimers_access($application_node) {
	if ($application_node->field_sirius_category['und'][0]['value'] != 'event:intake_application') { return FALSE; }
	$event_node = sirius_log_handler($application_node, 'sirius_event');
	if (!$event_node) { return FALSE; }
	if (!sirius_event_disclaimer_enabled($event_node)) { return FALSE; } 
	return TRUE;
}



function sirius_node_tab_event_intake_application_disclaimers($application_node) {
	return drupal_get_form('sirius_node_tab_event_intake_application_disclaimers_form', $application_node);
}

function sirius_node_tab_event_intake_application_disclaimers_form($form, &$form_state, $application_node) {

	$form_state['application_nid'] = $application_node->nid;
	$event_node = sirius_log_handler($application_node, 'sirius_event');
	$form['#tree'] = TRUE;

	$application_json = sirius_jsonfield_util_node_json_get($application_node);

	// Back door to revoke all acceptances. I'm using this basically just for dev purposes and it can go away any time.
	if (user_access('sirius domain admin') && $_REQUEST['clear']) { 
		unset($application_json['disclaimer']);
		sirius_jsonfield_util_node_json_set($application_node, $application_json);
		node_save($application_node);
	}

	$event_disclaimer_json = sirius_event_disclaimer_json($event_node);
	$disclaimer_nodes = sirius_event_disclaimer_nodes($event_disclaimer_json);

	if (!$disclaimer_nodes) {
		drupal_set_message("No disclaimers have been configured for this event.");
		return array();
	}

	$accepted_all = TRUE;
	foreach ($disclaimer_nodes as $disclaimer_nid => $disclaimer_node) {
		$disclaimer_json = sirius_jsonfield_util_node_json_get($disclaimer_node)['disclaimer'];
		$acceptance_json = $application_json['disclaimer'][$disclaimer_nid]['acceptances'][0];
		if (!$acceptance_json) { $accepted_all = FALSE; }
		$form['disclaimer'][$disclaimer_nid] = sirius_disclaimer_render_form($disclaimer_json, $acceptance_json);			
	}

  $esignature_status = sirius_esignature_entity_status($application_node);
  if (!$accepted_all && ($esignature_status['status'] != 'unsigned')) {
    $form['intro'] = array('#markup' => '<p><strong>Because this application has already been signed, no changes can be made. (Signature status: ' . $esignature_status['status_msg'] . '.)</strong></p>', '#weight' => -1);
  }

	if (!$accepted_all && ($esignature_status['status'] == 'unsigned')) {
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit'),
		);
	}

	return $form;
}

function sirius_node_tab_event_intake_application_disclaimers_form_submit($form, &$form_state) {
	$application_node = node_load($form_state['application_nid']);
	if (!$application_node) {
		drupal_set_message("Application node not found.", 'error');
		return;
	}

	$disclaimer_nodes = node_load_multiple(array_keys($form_state['values']['disclaimer']));
	sirius_disclaimer_accept($application_node, $disclaimer_nodes, array());
	
	drupal_set_message("Response saved.");
}