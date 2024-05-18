<?php

function sirius_denorm_page_queue() {
	return drupal_get_form('sirius_denorm_page_queue_form');
}

function sirius_denorm_page_queue_form($form, &$form_state) {

	$form['#tree'] = TRUE;

  $form['filters'] = array(
  	'#type' => 'fieldset',
  	'#title' => 'filters',
  	'#collapsible' => TRUE,
  	'#collapsed' => TRUE,
  );

  $form['filters']['status'] = array(
  	'#type' => 'checkboxes',
  	'#checkall' => TRUE,
  	'#title' => t('Status'),
  	'#options' => array(
  		'pending' => t('Pending'), 
  		'in_progress' => t('In Progress'),
  		'complete' => t('Complete'),
  	),
  	'#default_value' => array('pending', 'in_progress'),
  );

  $form['filters']['submit'] = array(
  	'#type' => 'submit',
  	'#value' => t('Submit'),
  );

  $params = array();
  $params['order'] = 'denorm_id desc';
  $params['limit'] = 100;
  if (isset($form_state['values']['filters']['status'])) {
  	$params['statuses'] = array_filter(array_values($form_state['values']['filters']['status']));
  } else {
  	$params['statuses'] = array('pending', 'in_progress');
  }

	$items = sirius_denorm_queue_search($params);
	$form['results'] = array('#markup' => sirius_denorm_queue_render($items, 1));
	return $form;
}

function sirius_denorm_page_queue_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
}

