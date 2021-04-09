<?php

function sirius_event_intake_enabled_tid($event_type_tid) { return sirius_event_intake_enabled_type(taxonomy_term_load($event_type_tid)); }

function sirius_event_intake_enabled_type($event_type_term) {
  $json = sirius_jsonfield_util_term_json_get($event_type_term);
  return $json['intake']['enabled'] == 'Yes';
}


function sirius_event_intake_enabled_nid($event_nid) { return sirius_event_intake_enabled(node_load($event_nid)); }

function sirius_event_intake_enabled($event_node) {
	$event_json = sirius_jsonfield_util_node_json_get($event_node);
	return $event_json['intake']['enabled'] == 'Yes';
}

function sirius_event_intake_settings_form($form, &$form_state, $entity_type, $entity) {
	$form_state['entity_type'] = $entity_type;
	if ($entity_type == 'node') {
		$form_state['entity_id'] = $entity->nid;
		$json = sirius_jsonfield_util_node_json_get($entity);
		$event_type_term = taxonomy_term_load($entity->field_sirius_event_type['und'][0]['tid']);
	} else if ($entity_type == 'taxonomy_term') {
		$form_state['entity_id'] = $entity->tid;
		$json = sirius_jsonfield_util_term_json_get($entity);
		$event_type_term = $entity;
	}
	$intake_json = $json['intake'];

	$prole_tids = sirius_fieldvals($event_type_term, 'field_sirius_event_proles', 'tid');
	$prole_terms = taxonomy_term_load_multiple($prole_tids);
	$prole_options = array();
	foreach ($prole_terms as $term) {
		$prole_options[$term->tid] = $term->name;
	}
	$prole_tids = array_keys($prole_terms);
	$pstatus_options = array(NULL => t('-- Select --')) + sirius_event_participant_role_status_tids($prole_tids);

	$form['#tree'] = TRUE;

	$form['intake']['enabled'] = array(
		'#type' => 'select',
		'#title' => t('Intake Enabled?'),
		'#options' => array('No' => t('No'), 'Yes' => t('Yes')),
		'#description' => t('Does this event type support intake?'),
		'#default_value' => $intake_json['enabled'],
	);

	$form['intake']['anonymous'] = array(
		'#type' => 'fieldset',
		'#collapsible' => FALSE,
		'#title' => t('Anonymous Intake'),
		'#description' => t('Anonymous intake allows non-user to create new accounts and register for the event.'),
	);

	$form['intake']['anonymous']['enabled'] = array(
		'#type' => 'select',
		'#title' => t('Intake Enabled?'),
		'#options' => array('No' => t('No'), 'Yes' => t('Yes')),
		'#description' => t('Does this event type support intake?'),
		'#default_value' => $intake_json['anonymous']['enabled'],
	);

	if ($entity_type == 'node') {
		$form['intake']['anonymous']['access_code'] = array(
			'#type' => 'textfield',
			'#title' => t('Access Code'),
			'#description' => t('Enter the access code to allow anonymous intake. Leave blank to generate automatically.'),
			'#default_value' => $intake_json['anonymous']['access_code'],
		);
	}

	$form['intake']['anonymous']['title'] = array(
		'#type' => 'textfield',
		'#title' => t('Title'),
		'#description' => t('What page title should be shown to anonymous users?'),
		'#default_value' => $intake_json['anonymous']['title'],
	);
	
	$form['intake']['admin_prole_tids'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Admin Roles'),
		'#options' => $prole_options,
		'#description' => t('These roles have permission to use the intake tools.'),
	);
	if ($intake_json['admin_prole_tids']) { $form['intake']['admin_prole_tids']['#default_value'] = $intake_json['admin_prole_tids']; }

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_event_intake_settings_form_submit($form, &$form_state) {
	$entity_type = $form_state['entity_type'];
	$entity_id = $form_state['entity_id']; 
	if ($entity_type == 'node') { $entity = node_load($entity_id); }
	else if ($entity_type == 'taxonomy_term') { $entity = taxonomy_term_load($entity_id); }

	$json = sirius_jsonfield_util_entity_json_get($entity);
	$json['intake'] = $form_state['values']['intake'];
	// $json['intake']['intro'] = sirius_richtext_handle_input($form_state['values']['intake']['intro']);
	$json['intake']['anonymous']['access_code'] = sirius_event_intake_access_code_anonicalize($form_state['values']['intake']['anonymous']['access_code']);
	if (!$json['intake']['anonymous']['access_code']) {
		$json['intake']['anonymous']['access_code'] = sirius_aat_generate(8, 'letter');
	}

	sirius_jsonfield_util_entity_json_set($entity, $json);

	if ($entity_type == 'node') {
		node_save($entity);
	} 
	else if ($entity_type == 'taxonomy_term') {
		taxonomy_term_save($entity);
	}

	drupal_set_message("Settings saved.");
}

function sirius_event_intake_access_code_anonicalize($code) {
	$code = preg_replace('/[^a-zA-Z0-9_ ]/', '', $code);
	$code = strtoupper($code);
	$code = trim($code);
	return $code;
}

function sirius_event_intake_fetch_by_access_code($access_code) {
	$sql = "select nid ";
	$sql .= "from node ";
	$sql .= "left join field_data_field_sirius_json on field_data_field_sirius_json.entity_type = 'node' and field_data_field_sirius_json.entity_id = node.nid ";
	$sql .= "where node.type = 'sirius_event' ";
	$sql .= "and json_unquote(json_extract(field_sirius_json_value, '\$.intake.anonymous.access_code')) = :access_code ";
	$sql .= "order by nid desc ";
	$stmt = sirius_sql_query($sql, array(':access_code' => $access_code));
	$matches = array();
	while ($hr = $stmt->fetchAssoc()) {
		if (!$fetchall) { return node_load($hr['nid']); }
		$matches[] = $hr['nid'];
	}
	return $matches;
}