<?php

function sirius_node_tab_event_participant_definition_followup_sms_access($survey_node) {
	if ($survey_node->field_sirius_type['und'][0]['value'] != 'node:sirius_event_participant:field_sirius_json') { return FALSE; }
	return sirius_event_followup_enabled_sms_survey($survey_node);
}

function sirius_node_tab_event_participant_definition_followup_sms($survey_node) {
	return drupal_get_form('sirius_node_tab_event_participant_definition_followup_items_form', $survey_node->nid, 'sms');
}

function sirius_node_tab_event_participant_definition_followup_email_access($survey_node) {
	if ($survey_node->field_sirius_type['und'][0]['value'] != 'node:sirius_event_participant:field_sirius_json') { return FALSE; }
	return sirius_event_followup_enabled_email_survey($survey_node);
}

function sirius_node_tab_event_participant_definition_followup_email($survey_node) {
	return drupal_get_form('sirius_node_tab_event_participant_definition_followup_items_form', $survey_node->nid, 'email');
}


function sirius_node_tab_event_participant_definition_followup_items_form($form, &$form_state, $survey_nid, $item_type) {
  drupal_add_css(drupal_get_path('module', 'sirius_event') . '/css/event.css', 'file');
	form_load_include($form_state, 'inc', 'sirius_event', 'sirius_event.page_participant_definition_followup_items');

	$form_state['survey_nid'] = $survey_nid;
	$form_state['item_type'] = $item_type;
	$survey_node = node_load($survey_nid);
	$survey_json = sirius_jsonfield_util_node_json_get($survey_node);

	$form['#tree'] = TRUE;

  $ajax_params = $form_state['triggering_element']['#sirius_ajax'];

	$followup_list = $survey_json['followup_list'][$item_type];
	if (!$followup_list) { $followup_list = array(); }
	uksort($followup_list, function($a, $b) use ($followup_list) { return $followup_list[$a]['weight'] <=> $followup_list[$b]['weight']; });
	$followup_list = array_values($followup_list);

	if ($form_state['followup_count']) {
		$followup_count = $form_state['followup_count'];
	} else {
		$followup_count = count($followup_list);
	}
  $followup_count += $ajax_params['qty'];
  if ($followup_count < 1) { $followup_count = 1; }
  $form_state['followup_count'] = $followup_count;

  for ($i=count($followup_list); $i<$followup_count; ++$i) {
  	$followup_list[] = array();
  }

	$form['followup_list']['prefix'] = array('#markup' => '<div id="followup_list">');
	foreach ($followup_list as $delta => $followup) {
		$uuid = $followup['uuid'];
    $default_weight = $followup['weight'];
    if (!$default_weight) { $default_weight = 10 * ($delta+1); }

		$title = sirius_event_followup_title($followup);
		if (!$title) { $title = '[new followup]'; }
    $title .= "<span class=\"sirius_event_weight_indicator\">Sort Order: $default_weight</span>";

    $form['followup_list'][$delta] = array(
    	'#type' => 'fieldset',
    	'#title' => $title,
    	'#collapsible' => TRUE,
    	'#collapsed' => TRUE,
    );

    $form['followup_list'][$delta]['uuid'] = array(
    	'#type' => 'value',
    	'#value' => $followup['uuid'],
    );
    $form['followup_list'][$delta]['title'] = array(
    	'#type' => 'textfield',
    	'#title' => t('Title'),
    	'#default_value' => $followup['title'],
    );

    if ($item_type == 'email') {
	    $form['followup_list'][$delta]['subject'] = array(
	    	'#type' => 'textfield',
	    	'#title' => t('Subject'),
	    	'#default_value' => $followup['subject'],
	    );
    }

		$form['followup_list'][$delta]['editable'] = array(
			'#title' => t('Editable?'),
			'#type' => 'select',
			'#options' => array(
				NULL => t('-- Select --'),
				'Yes' => t('Yes'),
				'No' => t('No'),
			),
			'#default_value' => $followup['editable'],
			'#description' => t('Should the caller have the ability to edit or personalize the message?'),
		);

		if ($item_type == 'sms') {
			$form['followup_list'][$delta]['sms'] = array(
				'#title' => t('Text Message'),
				'#type' => 'textarea',
				'#default_value' => $followup['sms'],
				'#description' => t('Enter the text message that will be sent to the person being called.'),
			);
		}

		if ($item_type == 'email') {
			$form['followup_list'][$delta]['body'] = array(
				'#title' => t('Email Message'),
		    '#type' => 'text_format',
		    '#format' => 'sirius_form_editor',
		    '#allowed_formats' => array('sirius_form_editor'),
				'#default_value' => $followup['body'],
				'#description' => t('Enter the email message that will be sent to the person being called.'),
			);
		}

    $form['followup_list'][$delta]['weight'] = array(
      '#title' => t('Sort Order'),
      '#type' => 'textfield',
      '#size' => 4,
    );
    if ($followup['weight']) { $form['followup_list'][$delta]['weight']['#default_value'] = $followup['weight']; }

    $form['followup_list'][$delta]['delete'] = array(
      '#type' => 'select',
      '#title' => t('Delete this followup'),
      '#options' => array(
        NULL => t('No, keep this followup'),
        'Yes' => t('Yes, delete this followup.')
      ),
      '#description' => t('The followup will be removed, and any students assigned to it will be unassigned.'),
    );

    $form['followup_list'][$delta]['weight'] = array(
      '#type' => 'textfield',
      '#title' => 'Sort Order',
      '#default_value' => $default_weight,
      '#size' => 4,
    );
  }

	$form['followup_list']['suffix'] = array('#markup' => '</div>');

  $form['add_one'] = array(
    '#type' => 'button',
    '#value' => t('Add one'),
    '#ajax' => array(
      'callback' => 'sirius_ajax_generic_replace',
    ),
    '#sirius_ajax' => array(
    	'qty' => 1,
    	'wrapper' => '#followup_list',
    	'element' => 'followup_list',
    ),
  );

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_node_tab_event_participant_definition_followup_items_form_submit($form, &$form_state) {
	$survey_nid = $form_state['survey_nid'];
	$survey_node = node_load($survey_nid);
	$survey_json = sirius_json_get($survey_node);
	$item_type = $form_state['item_type'];

	$followup_list = array();
	$followup_list_input = $form_state['values']['followup_list'];
	if (!$followup_list_input) { $followup_list_input = array(); }

	foreach ($followup_list_input as $followup) {
		if (!$followup['sms'] && !$followup['title']) { continue; }
    if ($followup['delete']) { continue; }
		$uuid = $followup['uuid'];
		if (!$uuid) { 
			$uuid = uuid_generate();
			$followup['uuid'] = $uuid;
		}
		// $followup['ts'] = strtotime($followup['date']);

		$followup['body'] = sirius_richtext_handle_input($followup['body']);

		$followup_list[$uuid] = $followup;
	}

	$survey_json['followup_list'][$item_type] = $followup_list;

	sirius_json_set($survey_node, $survey_json);
	node_save($survey_node);
	drupal_set_message("Settings saved.");
}
