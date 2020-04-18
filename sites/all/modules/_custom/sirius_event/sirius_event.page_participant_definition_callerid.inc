<?php

function sirius_node_tab_event_participant_definition_callerid_access($definition_node) {
	if ($definition_node->field_sirius_type['und'][0]['value'] != 'node:sirius_event_participant:field_sirius_json') { return FALSE; }
	return TRUE;
}

function sirius_node_tab_event_participant_definition_callerid($definition_node) {
	return drupal_get_form('sirius_node_tab_event_participant_definition_callerid_form', $definition_node->nid);
}

function sirius_node_tab_event_participant_definition_callerid_form($form, &$form_state, $definition_nid) {
	$form_state['definition_nid'] = $definition_nid;
	$definition_node = node_load($definition_nid);

	$intro_html = '<div class="sirius_form_intro">';
	$intro_html .= t('Use the form below to select the available caller IDs for this survey.');
	if (drupal_valid_path('sirius/config/phone/callerid')) {
		$intro_html .= ' ';
		$intro_html .= 'To add a new caller ID to the system, ';
		$intro_html .= l('click here.', 'sirius/config/phone/callerid');
	}
	$intro_html .= '<br /><br />';
	$intro_html .= '</div>';
	$form['intro'] = array('#markup' => $intro_html);

	$callerid_nodes = sirius_callerid_list();
	$callerid_options = array(NULL => t('-- Select --'));
	foreach ($callerid_nodes as $callerid_nid => $callerid_node) {
		if ($callerid_node->field_sirius_active['und'][0]['value'] == 'Yes') {
			$callerid_options[$callerid_nid] = $callerid_node->title . ' (' . $callerid_node->field_sirius_phone['und'][0]['value'] . ')';
		}
	}

	$max = 2;
	if (isset($definition_node->field_sirius_callerids['und'])) {
		$max = count($definition_node->field_sirius_callerids['und']) + 2;
	} else {
		$max = 2;
	}
	for ($delta = 0; $delta < $max; ++$delta) {
		$form["callerid_nid_$delta"] = array(
			'#type' => 'select',
			'#options' => $callerid_options,
			'#title' => t('Caller ID #@i', array('@i' => $delta+1)),
			'#default_value' => $definition_node->field_sirius_callerids['und'][$delta]['target_id'],
		);
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_node_tab_event_participant_definition_callerid_form_submit($form, &$form_state) {
	$definition_nid = $form_state['definition_nid'];
	$definition_node = node_load($definition_nid);

	$delta = 0;
	$value = array();
	while (TRUE) {
		if (!$form_state['values']["callerid_nid_$delta"]) { break; }
		$value[] = array('target_id' => $form_state['values']["callerid_nid_$delta"]);
		$delta++;
	}
	$definition_node->field_sirius_callerids['und'] = $value;
  node_submit($definition_node);
  node_save($definition_node);
  drupal_set_message(t('Caller ID options saved.'));
}
