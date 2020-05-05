<?php

function sirius_bulk_node_tab_body($bulk_node) {
	return drupal_get_form('sirius_bulk_node_tab_body_form', $bulk_node);
}

function sirius_bulk_node_tab_body_form($form, &$form_state, $bulk_node) {
	$form_state['bulk_nid'] = $bulk_node->nid;
	$media = sirius_bulk_media();
	$medium = $bulk_node->field_sirius_bulk_medium['und'][0]['value'];
	$form['into'] = array('#markup' => '<div class="sirius_form_intro">' . t('Message type: ') . '<strong>' . $media[$medium] . '</strong>' . '</div>');

	$form['title'] = array(
		'#title' => t('Subject'),
		'#type' => 'textfield',
		'#default_value' => $bulk_node->title,
		'#description' => t('Will be used as the subject for an email, and will be the first part of an SMS or robocall.'),
		'#required' => TRUE,
	);

	if (in_array($medium, array('email', 'choice'))) {
		$form['body'] = array(
			'#title' => t('Email Body'),
			'#type' => 'text_format',
			'#default_value' => $bulk_node->body['und'][0]['value'],
			'#format' => $bulk_node->body['und'][0]['text_format'],
			'#required' => TRUE,
		);
	}

	if (in_array($medium, array('sms', 'choice'))) {
		$form['sms'] = array(
			'#title' => t('SMS Body'),
			'#type' => 'textarea',
			'#default_value' => $bulk_node->field_sirius_sms['und'][0]['value'],
			'#required' => TRUE,
		);
	}

	if (in_array($medium, array('voice', 'choice'))) {
		$form['voice'] = array(
			'#title' => t('Voice Body'),
			'#type' => 'textarea',
			'#default_value' => $bulk_node->field_sirius_voice['und'][0]['value'],
			'#required' => TRUE,
		);
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
		'#required' => TRUE,
	);

	return $form;
}

function sirius_bulk_node_tab_body_form_submit($form, &$form_state) {
	$bulk_node = node_load($form_state['bulk_nid']);

	$bulk_node->title = $form_state['values']['title'];
	$bulk_node->body['und'][0]['value'] = $form_state['values']['body']['value'];
	$bulk_node->body['und'][0]['text_format'] = $form_state['values']['body']['text_format'];
	$bulk_node->field_sirius_sms['und'][0]['value'] = $form_state['values']['sms'];
	$bulk_node->field_sirius_voice['und'][0]['value'] = $form_state['values']['voice'];
	node_save($bulk_node);
}