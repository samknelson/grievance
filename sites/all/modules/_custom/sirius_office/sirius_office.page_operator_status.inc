<?php

function sirius_office_node_tab_operator_status_access($operator_node) {
	// Super-admins can access
	if (user_access('sirius admin')) { return TRUE; }

	// Operator admins can access
	if (user_access('sirius office operator admin')) { return TRUE; }

	// Must be an operator user
	if (!user_access('sirius office operator user')) { return FALSE; }

	// Must be one of the users assigned to this office
	global $user;
	$valid_uids = sirius_fieldvals($operator_node, 'field_sirius_users', 'target_id');
	if ($valid_uids && in_array($user->uid, $valid_uids)) { return TRUE; }

	// Oh well.
	return FALSE;
}
function sirius_office_node_tab_operator_status($operator_node) {
	return drupal_get_form('sirius_office_node_tab_operator_status_form', $operator_node);
}

function sirius_office_node_tab_operator_status_form($form, &$form_state, $operator_node) {
	$form_state['operator_nid'] = $operator_node->nid;
	$json = json_decode($operator_node->field_sirius_json['und'][0]['value'], TRUE);

  $form['status'] = array(
    '#title' => t('Foward to Operator Status'),
    '#type' => 'select',
    '#options' => array(
      'open' => 'Open',
      'closed' => 'Closed',
    ),
    '#default_value' => $json['status']['status'],
  );

  $form['which'] = array(
    '#title' => t('When the office is closed, and a user requests the operator, which message should be played?'),
    '#type' => 'select',
    '#options' => array(
      1 => 'Message #1',
      2 => 'Message #2',
      3 => 'Message #3',
      4 => 'Message #4',
      5 => 'Message #5',
    ),
    '#default_value' => $json['status']['which'],
  );
  for ($i=1; $i<=5; ++$i) {
    $form["msg_$i"] = array(
      '#title' => "Message #$i",
      '#type' => 'textarea',
      '#default_value' => $json['status']["msg_$i"],
    );
  }

  $form['submit'] = array(
  	'#type' => 'submit',
  	'#value' => t('Submit'),
  );

  return $form;
}

function sirius_office_node_tab_operator_status_form_submit($form, &$form_state) {
	$operator_node = node_load($form_state['operator_nid']);
	$json = json_decode($operator_node->field_sirius_json['und'][0]['value'], TRUE);
	$json['status'] = $form_state['values'];
	$operator_node->field_sirius_json['und'][0]['value'] = json_encode($json);
	node_save($operator_node);
}