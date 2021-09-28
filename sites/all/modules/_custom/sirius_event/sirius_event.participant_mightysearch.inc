<?php

function sirius_event_page_participant_mightysearch($event_node, $rule, $lite = FALSE) {
	$prole_tid = $_REQUEST["prole"];
	if (!$prole_tid) {
		$html .= "To register new participants for this event, please select a role from the list above.";
		return $html;
	}
	$delta = $rule['delta'];
	return drupal_get_form('sirius_event_page_participant_mightysearch_form', $event_node, $prole_tid, $lite, $delta);
}

function sirius_event_page_participant_mightysearch_form($form, &$form_state, $event_node, $prole_tid, $lite, $delta) {
	$form_state['event_nid'] = $event_node->nid;
	$form_state['prole_tid'] = $prole_tid;
	$form_state['lite'] = $lite;
	$participant_role_term = taxonomy_term_load($prole_tid);
	$statuses = sirius_event_participant_role_status_tids($prole_tid);
	$domain_nid = sirius_domain_nid();

	$widget_settings = sirius_event_widget_settings($event_node, $rule_type, $delta, $widget);

	if ($widget_settings['actions']) {
		$actions_all = sirius_event_participant_mightsearch_actions_all($event_node, $prole_tid);
		$actions = array();
		foreach (array_filter(array_values($widget_settings['actions'])) as $key) {
			$actions[$key] = $actions_all[$key];
		}
	} else {
		if ($lite) { 
			$actions = array();
		} else {
			$actions = sirius_event_participant_mightsearch_actions_all($event_node, $prole_tid);
		}
	}

	$contact_definition_json = sirius_contact_definition();
	$contact_definition_form = sirius_jsonfield_util_form_get($contact_definition_json, 'default');

	$participant_definition_node = sirius_event_survey_get($event_node);
	if ($participant_definition_node) { 
		$participant_definition_json = sirius_jsonfield_util_node_json_get($participant_definition_node);
		$participant_definition_form = sirius_jsonfield_util_form_get($participant_definition_json, 'default');
	}

	$json_forms = array(
		'sirius_contact' => array('default' => $contact_definition_form),
		'sirius_event_participant' => array(
			'survey' => $participant_definition_form, 
			'event_nid' => $event_node->nid,
			'prole_tid' => $prole_tid,
		),
		'widget_settings' => $widget_settings,
	);

	sirius_mightysearch_form_prepare(
		$form, 
		$form_state, 
		$event_node->nid,
		$json_forms,
		$actions
	);

	return $form;
}

function sirius_event_page_participant_mightysearch_form_submit($form, &$form_state) {
	sirius_mightysearch_form_submit($form, $form_state);
}

function sirius_event_partcipant_mightysearch_config_form($form, &$form_state, $event_type_term, $rule_type, $delta, $widget) {
	$form_state['event_type_tid'] = $event_type_term->tid;
	$form_state['rule_type'] = $rule_type;
	$form_state['delta'] = $delta;
	$form_state['widget'] = $widget;

	$widget_settings = sirius_event_type_widget_settings($event_type_term, $rule_type, $delta, $widget);

	$form['#tree'] = TRUE;

	$form['settings']['actions'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Actions'),
		'#options' => sirius_event_participant_mightsearch_action_options_all(),
		'#checkall' => TRUE,
		'#description' => t('Users of this widget have permission to execute the following actions on selected participants.'),
	);
	if ($widget_settings['actions']) { $form['settings']['actions']['#default_value'] = $widget_settings['actions']; }



	$contact_definition_json = sirius_contact_definition();
	$contact_definition_form = sirius_jsonfield_util_form_get($contact_definition_json, 'default');

	$json_forms = array(
		'sirius_contact' => array('default' => $contact_definition_form),
		'sirius_event_participant' => array('event_type_tid' => $event_type_term->tid),
	);
	$paths = sirius_mightysearch_paths($json_forms, FALSE);

	// Add-one crap
	form_load_include($form_state, 'inc', 'sirius_event', 'sirius_event.participant_mightysearch');
	$ajax_params = $form_state['triggering_element']['#sirius_ajax'];
	if ($form_state['field_count']) {
		$field_count = $form_state['field_count'];
	} else {
		if ($widget_settings['fields']['field_list']) {
			$field_count = count($widget_settings['fields']['field_list']);
		} else {
			$field_count = 1;
		}
	}
  $field_count += $ajax_params['qty'];
  if ($field_count < 1) { $field_count = 1; }
  $form_state['field_count'] = $field_count;

	$form['settings']['form'] = array(
		'#type' => 'fieldset',
		'#title' => 'Form Settings',
	);

	$form['settings']['form']['lock_clauses'] = array(
		'#title' => t('Lock Clauses?'),
		'#type' => 'select',
		'#options' => array(
			NULL => t('-- Select --'),
			'No' => t('No'),
			'Yes' => t('Yes'),
		),
		'#default_value' => $widget_settings['form']['lock_clauses'],
		'#description' => t('Should the "clauses" area of the search be hidden? If this value is set to "Yes", then the only fields available for searching will be those listed below.'),
	);
	$form['settings']['form']['lock_display'] = array(
		'#title' => t('Lock Display Fields?'),
		'#type' => 'select',
		'#options' => array(
			NULL => t('-- Select --'),
			'No' => t('No'),
			'Yes' => t('Yes'),
		),
		'#default_value' => $widget_settings['form']['lock_display'],
		'#description' => t('Should the "display fields" area of the search be hidden? If this value is set to "Yes", then the only fields displayed will be the ones listed below.'),
	);

	$tools_info_all = sirius_mightysearch_tools_info_all();
	$tools_options = array();
	foreach ($tools_info_all as $tool_key => $record) { $tools_options[$tool_key] = $record['title']; }
	$form['settings']['form']['tools'] = array(
		'#title' => t('Show links to:'),
		'#type' => 'checkboxes',
		'#options' => $tools_options,
		'#description' => t('In the results, include the checked "tool" links.'),
	);
	if ($widget_settings['form']['tools']) { $form['settings']['form']['tools']['#default_value'] = $widget_settings['form']['tools']; }

	$form['settings']['fields'] = array(
		'#type' => 'fieldset',
		'#title' => t('Fields'),
		'#collapsible' => TRUE,
		'#description' => '<p>' . t('The search interface can be optionally restricted to a specific set of fields. The contact name need not be included, because it is always the first field displayed.') . '</p>',
	);

	$form['settings']['fields']['field_list']['prefix'] = array('#markup' => '<div id="field_list">');
	for ($field_delta = 0; $field_delta < $field_count; ++$field_delta) {
		if ($widget_settings['fields']['field_list'][$field_delta]['path']) {
			$title = $widget_settings['fields']['field_list'][$field_delta]['label'] . ' (' . $widget_settings['fields']['field_list'][$field_delta]['path'] . ')';
		} else {
			$title = 'New Field';
		}
		$form['settings']['fields']['field_list'][$field_delta] = array(
			'#type' => 'fieldset',
			'#title' => $title,
		);
		$form['settings']['fields']['field_list'][$field_delta]['path'] = array(
			'#title' => t('Field'),
			'#type' => 'select',
			'#options' => array(NULL => t('-- Select --')) + $paths,
			'#default_value' => $widget_settings['fields']['field_list'][$field_delta]['path'],
		);

		$form['settings']['fields']['field_list'][$field_delta]['label'] = array(
			'#title' => t('Label'),
			'#type' => 'textfield',
			'#default_value' => $widget_settings['fields']['field_list'][$field_delta]['label'],
		);

		$form['settings']['fields']['field_list'][$field_delta]['criterion'] = array(
			'#title' => t('Criterion'),
			'#type' => 'select',
			'#default_value' => $widget_settings['fields']['field_list'][$field_delta]['criterion'],
			'#options' => array(NULL => t('-- Select --')) + sirius_mightysearch_criterion_options(),
			'#states' => array('visible' => array(":input[name=\"settings[form][lock_clauses]\"]" => array('value' => 'Yes'))),
		);

		$form['settings']['fields']['field_list'][$field_delta]['description'] = array(
			'#title' => t('Form Description'),
			'#type' => 'textfield',
			'#default_value' => $widget_settings['fields']['field_list'][$field_delta]['description'],
			'#states' => array('visible' => array(":input[name=\"settings[form][lock_clauses]\"]" => array('value' => 'Yes'))),
		);


		if ($field_delta == 0) {
			$form['settings']['fields']['field_list'][$field_delta]['path']['#description'] = t('Select the field to display.');
			$form['settings']['fields']['field_list'][$field_delta]['label']['#description'] = t('Enter the field label as it should be displayed to users.');
			$form['settings']['fields']['field_list'][$field_delta]['criterion']['#description'] = t('Select the criterion that should be used for this field. This option only applies to locked fields. If no criterion is selected, the field will not be searchable.');
			$form['settings']['fields']['field_list'][$field_delta]['description']['#description'] = t('Enter the description of the field as it should appear on the form. For example, if the criterion is "contains", a suitable description might be "substring ok".');
		}
	}
	$form['settings']['fields']['field_list']['suffix'] = array('#markup' => '</div>');
  $form['settings']['fields']['add_one'] = array(
    '#type' => 'button',
    '#value' => t('Add one'),
    '#ajax' => array(
      'callback' => 'sirius_ajax_generic_replace',
    ),
    '#sirius_ajax' => array(
    	'qty' => 1,
    	'wrapper' => '#field_list',
    	'element' => 'settings:fields:field_list',
    ),
  );

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_event_partcipant_mightysearch_config_form_submit($form, &$form_state) {
	$event_type_term = taxonomy_term_load($form_state['event_type_tid']);
	$rule_type = $form_state['rule_type'];
	$delta = $form_state['delta'];
	$widget = $form_state['widget'];

	$settings = $form_state['values']['settings'];

	$field_list = $settings['fields']['field_list'];
	if ($field_list) {
		foreach ($field_list as $field_delta => $value) {
			if (!$value['path']) {
				unset($field_list[$field_delta]);
			}
		}
	}
	$field_list = array_values($field_list);
	$settings['fields']['field_list'] = $field_list;

	$event_type_json = sirius_json_get($event_type_term);
	$event_type_json['widget_settings'][$rule_type][$delta][$widget] = $settings;
	sirius_json_set($event_type_term, $event_type_json);
	taxonomy_term_save($event_type_term);
}

function sirius_event_participant_mightsearch_action_options_all($event_node = NULL, $prole_tid = NULL) {
	$options = array();
	$actions_all = sirius_event_participant_mightsearch_actions_all($event_node, $prole_tid);
	foreach ($actions_all as $key => $value) {
		$options[$key] = $value['title'];
	}
	return $options;
}

function sirius_event_participant_mightsearch_actions_all($event_node = NULL, $prole_tid = NULL) {
	$actions = array();
	$actions['tag_add'] = array('title' => 'Add a Tag');
	$actions['tag_remove'] = array('title' => 'Remove a Tag');
	$actions['export'] = array('title' => 'Export');
	$actions['report'] = array('title' => 'Report');
	$actions['change_pstatus'] = array(
		'title' => 'Change Participant Status',
		'event_nid' => $event_node->nid,
		'prole_tid' => $prole_tid,
		'pstatus_options' => sirius_event_participant_role_status_tids($prole_tid),
	);
	$actions['delete_participant'] = array(
		'title' => 'Delete Participant Record',
		'event_nid' => $event_node->nid,
	);

	return $actions;
}