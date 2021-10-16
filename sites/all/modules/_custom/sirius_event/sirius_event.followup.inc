<?php

function sirius_event_followup_enabled($event_node) {
	$survey_node = sirius_event_survey_get($event_node);
	return sirius_event_followup_enabled_survey($survey_node);
}

function sirius_event_followup_enabled_survey($survey_node) {
	$json = sirius_json_get($survey_node);
	return $json['followup']['enabled'] == 'Yes';
}

function sirius_event_followup_enabled_sms_survey($survey_node) {
	if (!sirius_event_followup_enabled_survey($survey_node)) { return FALSE; }
	$json = sirius_json_get($survey_node);
	return $json['followup']['enabled_sms'] == 'Yes';
}

function sirius_event_followup_enabled_email_survey($survey_node) {
	if (!sirius_event_followup_enabled_survey($survey_node)) { return FALSE; }
	$json = sirius_json_get($survey_node);
	return $json['followup']['enabled_email'] == 'Yes';
}

function sirius_event_followup_title($followup, $empty_title = '(new followup)') {
	if ($followup['title']) { $title = $followup['title']; }
	if (!$title) { $title = $followup['sms']; }
	if (!$title) { $title = $followup['uuid']; }
	if (!$title) { $title = $empty_title; }

	return $title;
}

function sirius_event_follwup_options_survey($survey_node, $item_type) {
	$json = sirius_json_get($survey_node);
	$followup_list = $json['followup_list'][$item_type];
	if (!$followup_list) { $followup_list = array(); }
	uksort($followup_list, function($a, $b) use ($followup_list) { return $followup_list[$a]['weight'] <=> $followup_list[$b]['weight']; });
	$options = array();
	foreach ($followup_list as $followup_uuid => $followup) {
		$options[$followup_uuid] = sirius_event_followup_title($followup);
	}

	return $options;
}

function sirius_event_followup_modal_access($event_nid, $participant_nid, $item_type) {
	// We must be able to access the "call" page. 
	// @todo: Might be ok if we can access the "map" page instead.
	$event_node = node_load($event_nid);
	if (!sirius_node_tab_event_phonebank_call_access($event_node)) { return FALSE; }

	// The participant (if any) must be associated with this event.
	if ($participant_nid) {
		$participant_node = node_load($participant_nid);
		if ($participant_node->field_sirius_event['und'][0]['target_id'] != $event_nid) { return FALSE; }
	}

	// The survey must have followups (of the correct type) enabled
	$survey_node = sirius_event_survey_get($event_node);
	if (!sirius_event_followup_enabled_survey($survey_node)) { return FALSE; }
	if ($item_type == 'sms' && !sirius_event_followup_enabled_sms_survey($survey_node)) { return FALSE; }
	if ($item_type == 'email' && !sirius_event_followup_enabled_email_survey($survey_node)) { return FALSE; }

	return TRUE;
}

function sirius_event_followup_modal($event_nid, $participant_nid, $item_type) {
	return drupal_get_form('sirius_event_followup_modal_form', $event_nid, $participant_nid, $item_type);
}

function sirius_event_followup_modal_form($form, &$form_state, $event_nid, $participant_nid, $item_type) {
	drupal_add_css(drupal_get_path('module', 'sirius_event') . '/css/survey.css', 'file');

  // dismiss if the form was submitted via a modal
  if($form_state['input']['js'] && $form_state['submitted']) {
    ctools_include('modal');
    ctools_include('ajax');
    $commands = array();

    // Dismiss the modal
    $commands[] = ctools_modal_command_dismiss();

    // Show any messages
    $commands[] = ajax_command_replace(
    	'#sirius_followup_messages', 
      '<div id="sirius_followup_messages">' . theme_status_messages(array()) . '</div>'
    );

    print ajax_render($commands);
    drupal_exit();
  }


	$form_state['event_nid'] = $event_nid;
	$form_state['participant_nid'] = $participant_nid;
	$form_state['item_type'] = $item_type;

	$event_node = node_load($event_nid);

	if ($participant_nid) {
		$participant_node = node_load($participant_nid);
		$contact_nid = $participant_node->field_sirius_contact['und'][0]['target_id'];
		$contact_node = node_load($contact_nid);
		$phone = $contact_node->field_sirius_phone['und'][0]['value'];
		$email = $contact_node->field_sirius_email['und'][0]['value'];
	}

	$form['#tree'] = TRUE;
	$form['prefix'] = array('#markup' => '<div id="survey_wrap">');

	if ($item_type == 'sms') {
		$form['phone'] = array(
			'#type' => 'textfield',
			'#title' => t('Phone'),
			'#default_value' => $phone,
			'#required' => TRUE,
		);
	}

	if ($item_type == 'email') {
		$form['email'] = array(
			'#type' => 'textfield',
			'#title' => t('email'),
			'#default_value' => $email,
			'#required' => TRUE,
		);
	}


	$survey_node = sirius_event_survey_get($event_node);
	$survey_json = sirius_json_get($survey_node);
	$options = sirius_event_follwup_options_survey($survey_node, $item_type);
	$form['followup_uuid'] = array(
		'#type' => 'select',
		'#title' => t('Which Message?'),
		'#options' => $options,
    '#ajax' => array(
      'callback' => 'sirius_ajax_generic_replace',
    ),
    '#sirius_ajax' => array(
      'qty' => 1,
      'wrapper' => '#sirius_ajax_wrapper',
      'element' => 'message',
    ),
	);

	$followup_uuid = $form_state['values']['followup_uuid'];
	if (!$followup_uuid) { $followup_uuid = array_keys($options)[0]; }
	$followup = $survey_json['followup_list'][$item_type][$followup_uuid];

	foreach (array('sms', 'subject', 'body') as $key) {
		unset($form_state['values']['message'][$key]);
		unset($form_state['input']['message'][$key]);
	}

	$form['message'] = array('#type' => 'fieldgroup');

	$form['message']['#prefix'] = '<div id="sirius_ajax_wrapper">';

	if ($item_type == 'email') {
		$form['message']['subject'] = array(
			'#type' => 'textfield',
			'#title' => t('Subject'),
			'#default_value' => $followup['subject'],
			'#required' => TRUE,
		);

		$form['message']['body'] = array(
			'#title' => t('Email Message'),
	    '#type' => 'text_format',
	    '#format' => 'sirius_form_editor',
	    '#allowed_formats' => array('sirius_form_editor'),
			'#default_value' => $followup['body'],
		);

		if ($followup['editable'] == 'No') {
			$form['message']['subject']['#disabled'] = TRUE;
			$form['message']['body']['#disabled'] = TRUE;
		}
	}

	if ($item_type == 'sms') {
		$form['message']['sms'] = array(
			'#type' => 'textarea',
			'#title' => t('Body'),
			'#default_value' => $followup['sms'],
			'#required' => TRUE,
		);
		if ($followup['editable'] == 'No') {
			$form['message']['sms']['#disabled'] = TRUE;
		}
	}

	$form['message']['#suffix'] = '</div>';

	$form['suffix'] = array('#markup' => '</div>');

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Send Message')
	);

	return $form;
}

function sirius_event_followup_modal_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
  $form_state['submitted'] = TRUE;
  $item_type = $form_state['item_type'];

  if ($item_type == 'sms') {
		$params['to'] = $form_state['values']['phone'];
		$params['sms'] = $form_state['values']['message']['sms'];
		$result = sirius_sms_send($params);
	}

  if ($item_type == 'email') {
		$params['to'] = $form_state['values']['email'];
  	$params['subject'] = $form_state['values']['message']['subject']; 
  	$params['body'] = sirius_richtext_handle_input($form_state['values']['message']['body']);
		$result = sirius_email_send($params);
  }
   
  if ($result['success']) {
   	$msg .= "Message Sent";
   } else {
   	$msg .= "Message FAILED: " . $result['msg'];
   }

   drupal_set_message($msg);
}
