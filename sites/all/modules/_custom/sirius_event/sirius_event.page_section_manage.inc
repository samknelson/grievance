<?php

function sirius_node_tab_event_section_manage_access($event_node) {
	return sirius_node_tab_event_section_settings_access($event_node);
}

function sirius_node_tab_event_section_manage($event_node) {
  sirius_event_disclaimer_redirect($event_node);
 	return drupal_get_form('sirius_node_tab_event_section_manage_form', $event_node);
}

function sirius_node_tab_event_section_manage_form($form, &$form_state, $event_node) {
	$form_state['event_nid'] = $event_node->nid; 
	$json = sirius_jsonfield_util_node_json_get($event_node);
	
	// $json['section_list'] = NULL;
	// sirius_jsonfield_util_node_json_set($event_node, $json);
	// node_save($event_node);

  $ajax_params = $form_state['triggering_element']['#sirius_ajax'];

	$section_list = $json['section_list'];
	if (!$section_list) { $section_list = array(); }
	uksort($section_list, function($a, $b) use ($section_list) { return $section_list[$a]['weight'] <=> $section_list[$b]['weight']; });
	$section_list = array_values($section_list);

	if ($form_state['section_count']) {
		$section_count = $form_state['section_count'];
	} else {
		$section_count = count($section_list);
	}
  $section_count += $ajax_params['qty'];
  if ($section_count < 1) { $section_count = 1; }
  $form_state['section_count'] = $section_count;

  for ($i=count($section_list); $i<$section_count; ++$i) {
  	$section_list[] = array();
  }

  $form['#tree'] = TRUE;

	$form['section_list']['prefix'] = array('#markup' => '<div id="section_list">');
	foreach ($section_list as $delta => $section) {
		$uuid = $section['uuid'];

		$title = sirius_event_section_title($section);

    $form['section_list'][$delta] = array(
    	'#type' => 'fieldset',
    	'#title' => $title,
    	'#collapsible' => TRUE,
    	'#collapsed' => TRUE,
    );

    $form['section_list'][$delta]['uuid'] = array(
    	'#type' => 'value',
    	'#value' => $section['uuid'],
    );
    $form['section_list'][$delta]['weight'] = array(
    	'#type' => 'value',
    	'#value' => $delta,
    );
    $form['section_list'][$delta]['title'] = array(
    	'#type' => 'textfield',
    	'#title' => t('Title'),
    	'#default_value' => $section['title'],
    );
    $form['section_list'][$delta]['date'] = array(
      '#title' => t('Date'),
			'#type' => 'date_popup',
			'#timepicker' => 'timepicker',
			'#timepicker_options' => array(
				'rows' => 6,
				'minutes' => array(
				  'starts' => 0,
				  'ends' => 56,
				  'interval' => 5,
				),
			),
			'#attributes' => array(
				'autocomplete' => 'off',
			),
    );
    if ($section['date']) { $form['section_list'][$delta]['date']['#default_value'] = $section['date']; }

    $form['section_list'][$delta]['limit'] = array(
    	'#type' => 'textfield',
    	'#title' => t('Capacity'),
    	'#size' => 4,
    	'#default_value' => $section['limit'],
    	'#description' => t("If omitted, the default limit for the event will be used."),
    );

    $form['section_list'][$delta]['comments'] = array(
    	'#type' => 'textarea',
    	'#title' => t('Comments'),
    	'#default_value' => $section['comments'],
    );
  }

	$form['section_list']['suffix'] = array('#markup' => '</div>');

	form_load_include($form_state, 'inc', 'sirius_event', 'sirius_event.page_section_manage');
	form_load_include($form_state, 'inc', 'sirius_event', 'sirius_event.page_section_settings');

  $form['add_one'] = array(
    '#type' => 'button',
    '#value' => t('Add one'),
    '#ajax' => array(
      'callback' => 'sirius_ajax_generic_replace',
    ),
    '#sirius_ajax' => array(
    	'qty' => 1,
    	'wrapper' => '#section_list',
    	'element' => 'section_list',
    ),
  );

  $form['add_ten'] = array(
    '#type' => 'button',
    '#value' => t('Add ten'),
    '#ajax' => array(
      'callback' => 'sirius_ajax_generic_replace',
    ),
    '#sirius_ajax' => array(
    	'qty' => 10,
    	'wrapper' => '#section_list',
    	'element' => 'section_list',
    ),
    '#suffix' => '<br clear="left" />',
  );

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Save',
	);

	$form['suffix'] = array('#markup' => '</div>');

	return $form;
}

function sirius_node_tab_event_section_manage_form_submit($form, &$form_state) {
	$event_nid = $form_state['event_nid'];
	$event_node = node_load($event_nid);
	$json = sirius_jsonfield_util_node_json_get($event_node);

	$section_list = array();
	$section_list_input = $form_state['values']['section_list'];
	if (!$section_list_input) { $section_list_input = array(); }

	foreach ($section_list_input as $section) {
		if (!$section['date'] && !$section['title']) { continue; }
		$uuid = $section['uuid'];
		if (!$uuid) { 
			$uuid = uuid_generate();
			$section['uuid'] = $uuid;
		}
		$section['ts'] = strtotime($section['date']);
		$section_list[$uuid] = $section;
	}

	$json['section_list'] = $section_list;
	sirius_jsonfield_util_node_json_set($event_node, $json);
	node_save($event_node);
	drupal_set_message("Sections saved.");
}


