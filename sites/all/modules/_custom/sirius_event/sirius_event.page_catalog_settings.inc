<?php 

function sirius_event_node_tab_catalog_settings_access($catalog_node) {
	// Only log entries of type certification
	if ($catalog_node->field_sirius_type['und'][0]['value'] != 'sirius_event:catalog') { return FALSE; }
	return TRUE;
}

function sirius_event_node_tab_catalog_settings($catalog_node) {
	return drupal_get_form('sirius_event_node_tab_catalog_settings_form', $catalog_node);
}

function sirius_event_node_tab_catalog_settings_form($form, &$form_state, $catalog_node) {
	$form_state['catalog_nid'] = $catalog_node->nid;

	$catalog_json = sirius_json_get($catalog_node);
	$form['#tree'] = TRUE;

	$settings = sirius_taxonomy_load('sirius_event_type', 'tid', 'name');
	$form['title'] = array(
		'#title' => t('Title'),
		'#type' => 'textfield',
		'#default_value' => $catalog_node->title,
	);


	$form['settings']['courses'] = array(
		'#type' => 'fieldset',
		'#title' => 'Courses',
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
	);

	$event_types = sirius_taxonomy_load('sirius_event_type', 'tid', 'name');
	$form['settings']['courses']['event_types'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Event Types'),
		'#description' => t('Select the event types to include in this catalog.'),
		'#options' => $event_types,
		'#checkall' => TRUE,
	);
	if ($catalog_json['settings']['courses']['event_types']) {
		$form['settings']['courses']['event_types']['#default_value'] = $catalog_json['settings']['courses']['event_types'];
	}

	$form['settings']['markup'] = array(
		'#type' => 'fieldset',
		'#title' => 'Introductory Text',
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
	);
	$form['settings']['markup']['intro'] = array(
		'#title' => t('Introductory Text'),
		'#description' => t('Enter any introductory text that should appear at the top of the catalog page.'),
		'#default_value' => $catalog_json['settings']['markup']['intro'],
    '#type' => 'text_format',
    '#format' => 'sirius_form_editor',
    '#allowed_formats' => array('sirius_form_editor'),
	);

	$form['settings']['markup']['outro'] = array(
		'#title' => t('Footer Text'),
		'#description' => t('Enter any text that should appear at the bottom of the catalog page.'),
		'#default_value' => $catalog_json['settings']['markup']['outro'],
    '#type' => 'text_format',
    '#format' => 'sirius_form_editor',
    '#allowed_formats' => array('sirius_form_editor'),
	);

	// Get the list of all possible custom fields in all possible custom field sets.
	$cfs_list = node_load_multiple(array_keys(sirius_customfield_set_list_all()));
	$cf_options = array();
	foreach ($cfs_list as $cfs_nid => $cfs_node) {
		$cfs_json = sirius_json_get($cfs_node);
		$cfs_form = sirius_jsonfield_util_form_get($cfs_json, 'default');
		$cfs_paths = array_values(sirius_jsonfield_util_form_list_paths($cfs_form));
		foreach ($cfs_paths as $cfs_path) {
			$cf_options["$cfs_nid$cfs_path"] = $cfs_node->title . $cfs_path;
		}
	}

	$form['settings']['course_display'] = array(
		'#type' => 'fieldset',
		'#title' => 'Course Display',
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
	);

	$form['settings']['course_display']['inline_custom_fields'] = array(
		'#type' => 'select',
		'#title' => 'Inline Custom Fields',
		'#multiple' => TRUE,
		'#size' => 10,
		'#description' => 'Select the custom fields to display "inline" in the title of the course listing.',
		'#options' => $cf_options,
	);
	if ($catalog_json['settings']['course_display']['inline_custom_fields']) {
		$form['settings']['course_display']['inline_custom_fields']['#default_value'] = $catalog_json['settings']['course_display']['inline_custom_fields'];
	}

	$form['settings']['course_display']['body_custom_fields'] = array(
		'#type' => 'select',
		'#title' => 'Body Custom Fields',
		'#multiple' => TRUE,
		'#size' => 10,
		'#description' => 'Select the custom fields to display in the main body of the course listing.',
		'#options' => $cf_options,
	);
	if ($catalog_json['settings']['course_display']['body_custom_fields']) {
		$form['settings']['course_display']['body_custom_fields']['#default_value'] = $catalog_json['settings']['course_display']['body_custom_fields'];
	}

	$form['settings']['course_display']['filter_custom_fields'] = array(
		'#type' => 'select',
		'#title' => 'Filter Custom Fields',
		'#multiple' => TRUE,
		'#size' => 10,
		'#description' => 'Select the custom fields that may be used to filter the course listing..',
		'#options' => $cf_options,
	);
	if ($catalog_json['settings']['course_display']['filter_custom_fields']) {
		$form['settings']['course_display']['filter_custom_fields']['#default_value'] = $catalog_json['settings']['course_display']['filter_custom_fields'];
	}

	$form['settings']['filters'] = array(
		'#type' => 'fieldset',
		'#title' => 'Filters',
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
	);

	$form['settings']['filters']['show_filters'] = array(
		'#title' => t('Show Filters?'),
		'#type' => 'select',
		'#options' => array(
			NULL => t('-- Select --'),
			'No' => t('No'),
			'Yes' => t('Yes'),
		),
		'#default_value' => $catalog_json['settings']['filters']['show_filters'],
	);

	$form['settings']['access'] = array(
		'#type' => 'fieldset',
		'#title' => 'Access',
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
	);

	$form['settings']['access']['public'] = array(
		'#title' => t('Public Access?'),
		'#type' => 'select',
		'#options' => array(
			NULL => t('-- Select --'),
			'No' => t('No'),
			'Yes' => t('Yes'),
		),
		'#default_value' => $catalog_json['settings']['access']['public'],
	);

	// $roles = sirius_domain_usermanage_roles();
	// $role_options = $roles['optional'] + $roles['required'];
	$role_options = user_roles();

	$form['settings']['access']['roles'] = array(
		'#title' => t('Roles'),
		'#type' => 'checkboxes',
		'#options' => $role_options,
		'#checkall' => TRUE,
	);
	if ($catalog_json['settings']['access']['roles']) {
		$form['settings']['access']['roles']['#default_value'] = $catalog_json['settings']['access']['roles'];
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_event_node_tab_catalog_settings_form_submit($form, &$form_state) {
	$catalog_node = node_load($form_state['catalog_nid']);
	$catalog_json = sirius_json_get($catalog_node);
	$catalog_json['settings'] = $form_state['values']['settings'];
	$catalog_json['settings']['markup']['intro'] = sirius_richtext_handle_input($form_state['values']['settings']['markup']['intro']);
	$catalog_json['settings']['markup']['outro'] = sirius_richtext_handle_input($form_state['values']['settings']['markup']['outro']);
	sirius_json_set($catalog_node, $catalog_json);
	$catalog_node->title = $form_state['values']['title'];
	$catalog_node->field_sirius_public['und'][0]['value'] = $form_state['values']['settings']['access']['public'];
	node_save($catalog_node);
	drupal_set_message("Settings saved.");
}