<?php

/**
 * Currently unused. At the moment notifications are hardcoded.
 */

function sirius_worker_term_tab_tor_worker_notifications($tor_type_term) {
	return drupal_get_form('sirius_worker_term_tab_tor_worker_notifications_form', $tor_type_term);
}

function sirius_worker_term_tab_tor_worker_notifications_form($form, &$form_state, $tor_type_term) {
	$form_state['tor_type_tid'] = $tor_type_term->tid;
	$tor_type_json = sirius_json_get($tor_type_term);

  $form['#tree'] = TRUE;
	$form['prefix'] = array('#markup' => '<div class="sirius-form">');

	$statuses = sirius_worker_schedule_tor_statuses($tor_type_term->tid);

	$form['statuses']['prefix'] = array('#markup' => '<div id="statuses">');
	foreach ($statuses as $status_uuid => $status) {
		$title = sirius_event_section_title($section);
    $title .= "<span class=\"sirius_event_weight_indicator\">Sort Order: $default_weight</span>";

    $form['worker_notifications'][$status_uuid] = array(
    	'#type' => 'fieldset',
    	'#title' => $status['title'],
    	'#collapsible' => TRUE,
    	'#collapsed' => FALSE,
    );

    $form['worker_notifications'][$status_uuid]['notify'] = array(
    	'#type' => 'select',
    	'#title' => t('Notify?'),
    	'#options' => array(
    		NULL => t('-- Select --'),
    		'No' => t('No'),
    		'Yes' => t('Yes'),
    	),
    	'#default_value' => $tor_type_json['tor']['worker_notifications'][$status_uuid],
			'#prefix' => '<div class="container-inline">',
			'#suffix' => '</div>',
    );

		$form['worker_notifications'][$status_uuid]['email_subject'] = array(
			'#type' => 'textfield',
			'#title' => t('Email Subject'),
			'#default_value' => $tor_type_json['tor']['worker_notifications'][$status_uuid]['email_subject'],
			'#states' => array('visible' => array(":input[name=\"worker_notifications[$status_uuid][notify]\"]" => array('value' => 'Yes'))),
		);

		$form['worker_notifications'][$status_uuid]['email_body_wrap'] = array(
			'#type' => 'item',
			'#states' => array('visible' => array(":input[name=\"worker_notifications[$status_uuid][notify]\"]" => array('value' => 'Yes'))),
		);

		$form['worker_notifications'][$status_uuid]['email_body_wrap']['email_body'] = array(
			'#type' => 'text_format',
			'#title' => 'Email Body',
			'#default_value' => $tor_type_json['tor']['worker_notifications'][$status_uuid]['email_body'],
			'#format' => 'sirius_form_editor',
			'#allowed_formats' => array('sirius_form_editor'),
		);

		$form['worker_notifications'][$status_uuid]['sms'] = array(
			'#type' => 'textarea',
			'#title' => t('SMS'),
			'#default_value' => $tor_type_json['tor']['worker_notifications'][$status_uuid]['sms'],
			'#states' => array('visible' => array(":input[name=\"worker_notifications[$status_uuid][notify]\"]" => array('value' => 'Yes'))),
		);

  }

	$form['statuses']['suffix'] = array('#markup' => '</div>');

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);
	$form['suffix'] = array('#markup' => '</div>');

	return $form;
}

function sirius_worker_term_tab_tor_worker_notifications_form_submit($form, &$form_state) {
	$tor_type_tid = $form_state['tor_type_tid'];
	$tor_type_term = taxonomy_term_load($tor_type_tid);
	$tor_type_json = sirius_json_get($tor_type_term);
	$tor_type_json['tor']['worker_notifications'] = $form_state['values']['worker_notifications'];
	if ($tor_type_json['tor']['worker_notifications']) {
		foreach (array_keys($tor_type_json['tor']['worker_notifications']) as $status_uuid) {
			$tor_type_json['tor']['worker_notifications'][$status_uuid]['email_body'] = sirius_richtext_handle_input($form_state['values']['worker_notifications'][$status_uuid]['email_body_wrap']['email_body']);
			unset($tor_type_json['tor']['worker_notifications'][$status_uuid]['email_body_wrap']);
		}
	}
	sirius_json_set($tor_type_term, $tor_type_json);
	taxonomy_term_save($tor_type_term);
	drupal_set_message("Worker notifications saved.");
}

