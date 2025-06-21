<?php

function sirius_teamsters631_node_tab_log_m12_notify_access($m12_node) {
	return ($m12_node->field_sirius_category['und'][0]['value'] == 'teamsters631:m12');
}

function sirius_teamsters631_node_tab_log_m12_notify_form($form, &$form_state, $m12_node) {
	$m12 = Sirius_Teamsters631_M12::getInstance();

	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/table.css');

	$form_state['m12_nid'] = $m12_node->nid;
	$m12_json = sirius_json_get($m12_node);

	$employer_node = $m12->employer($m12_node);
	$employer_json = sirius_json_get($employer_node);

	$form['#tree'] = TRUE;
	$form['email']['to'] = array('#type' => 'fieldset', '#title' => 'To');
	$emails = $employer_json['teamsters631_m12']['emails'];
	if (!$emails) { $emails = array(); }
	$emails[] = '';
	$emails[] = '';
	foreach ($emails as $delta => $email) {
		$form['email']['to'][$delta] = array(
			'#type' => 'textfield',
			'#default_value' => $emails[$delta],
		);
	}

	$form['email']['subject'] = array(
		'#type' => 'textfield', 
		'#title' => 'Subject',
		'#default_value' => $m12_node->title,
	);

	$body = '<p>' . 'Our system indicates that the following currently employed workers are no longer eligible for employment. Please lay them off within 48 hours.' . '</p>';
	$body .= $m12->layoff_render($m12_node, 'email');

	$form['email']['body'] = array(
		'#type' => 'text_format',
		'#rows' => 30,
		'#default_value' => $body,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Send Notification'),
	);

	return $form;
}

function sirius_teamsters631_node_tab_log_m12_notify_form_submit($form, &$form_state) {
	$m12 = Sirius_Teamsters631_M12::getInstance();

	$m12_node = node_load($form_state['m12_nid']);
	$employer_node = $m12->employer($m12_node);

	$emails = array_unique(array_filter($form_state['values']['email']['to']));
	if (!$emails) {
		drupal_set_message("Please enter at least one email address.", 'error');
		return;
	}

	$subject = $form_state['values']['email']['subject'];
	$body = sirius_richtext_handle_input($form_state['values']['email']['body']);

	$params = array();
  $params['body'] = $body;
  $params['subject'] =  $subject;
  $params['handler_nids'] = array($m12_node->nid, $employer_node->nid);

	foreach ($emails as $email) {
    $params['to'] = $email;
    $result = sirius_email_send($params);
    if ($result['success']) {
    	drupal_set_message("Sent to: $email");
    } else {
    	drupal_set_message("Send to $email failed: " . $result['msg'], 'error');
    }
	}

	$m12->notify_set($m12_node);

	return;
}
