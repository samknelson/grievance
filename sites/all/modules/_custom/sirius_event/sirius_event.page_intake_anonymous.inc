<?php

function sirius_event_page_intake_anonymous($access_code = NULL) {
	if (!$access_code) {
		drupal_set_message(t("No access code."), 'error');
		drupal_goto('intake');
	}

	$event_node = sirius_event_intake_fetch_by_access_code($access_code);
	if (!$event_node) {
		drupal_set_message(t("That does not appear to be a valid access code."), 'error');
		drupal_goto('intake');
	}
	$json = sirius_jsonfield_util_node_json_get($event_node);
	if ($json['intake']['enabled'] != 'Yes') {
		drupal_set_message(t('Intake is not enabled for that event.'), 'error');
		drupal_goto('intake');
	}
	if ($json['intake']['anonymous']['enabled'] != 'Yes') {
		drupal_set_message(t('Anonymous intake is not enabled for that event.'), 'error');
		drupal_goto('intake');
	}

	$title = $json['intake']['anonymous']['title'];
	if (!$title) { $title = $event_node->title; }
	drupal_set_title($title);
	sirius_backlink_add('intake', array('name' => 'New Access Code'));
	$renderable = node_view($event_node, 'full');
  $html .= drupal_render(field_view_field('node', $event_node, 'body', 'full'));

  $active = sirius_event_intake_active($event_node);
  if (!$active['success']) {
  	drupal_set_message($active['msg'], 'warning');
  } else {
	  $html .= drupal_render(drupal_get_form('sirius_event_page_intake_anonymous_form', $event_node));
	}
	return $html;
}

function sirius_event_page_intake_anonymous_form($form, &$form_state, $event_node) {
	$form_state['event_nid'] = $event_node->nid;

	$application_node = sirius_event_intake_mine($event_node->nid);

	if ($application_node) {
		$form['action'] = array(
			'#title' => t('Action'),
			'#type' => 'select',
			'#options' => array(
				'continue' => t('Continue my application'),
				'new' => t('Start a new application'),
			),
		);

		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit'),
		);
	} else {
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Start an Application'),
		);
	}

	return $form;
}

function sirius_event_page_intake_anonymous_form_submit($form, &$form_state) {
	$event_node = node_load($form_state['event_nid']);
	$action = $form_state['values']['action'];
	if ($action == 'continue') {
		$application_node = sirius_event_intake_mine($event_node->nid);
	}

	if (!$application_node) {
		$application_node = sirius_event_intake_mine_create($event_node->nid);	
	}

	drupal_goto(current_path() . '/wizard');
}
