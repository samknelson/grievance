<?php

function sirius_event_register_user_select_form($form, &$form_state, $event_node, $rule) {
	$form_state['event_nid'] = $event_node->nid;
	$form_state['rule'] = $rule;
	$form_state['participant_role_tid'] = $participant_role_tid;

	$domain_nid = sirius_domain_nid();

	$rids = array_keys($rule['participant_user_role']);
	$args = array();
	$sql = "select node.nid, node.title ";
	$sql .= "from users ";
	$sql .= "join users_roles on users_roles.uid = users.uid and users_roles.rid in (:rids) ";
	$args[':rids'] = $rids;
	$sql .= "join field_data_field_sirius_contact on field_data_field_sirius_contact.entity_type = 'user' and field_data_field_sirius_contact.entity_id = users.uid ";
	$sql .= "join node on field_sirius_contact_target_id = node.nid ";
	if ($domain_nid) {
		$sql .= "join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'user' and field_data_field_sirius_domain.entity_id = users.uid and field_sirius_domain_target_id = :domain_nid ";
		$args[':domain_nid'] = $domain_nid;
	}
	$sql .= "where users.status = 1 ";
	$sql .= "group by users.uid ";
	$sql .= "order by users.name ";
	$sql .= "limit 1000 ";
	$stmt = db_query($sql, $args);
	$users = array();
	while ($hr = $stmt->fetchAssoc()) { $users[$hr['nid']] = $hr['title']; }
	if (count($users) > 999) {
		drupal_set_message(t('The user select widget is limited to 1000 records. Consider using the by-name widget instead.'), 'warning');
	}

	$pstatus_tids = sirius_event_participant_role_status_tids($prole_tid);

	$form['contact_nid'] = array(
		'#title' => 'Register this user',
		'#type' => 'select',
		'#options' => $users,
		'#required' => TRUE,
	);

	foreach ($rule['register_as'] as $key) {
		list ($prole_tid, $pstatus_tid) = explode('|', $key);
		$prole_term = taxonomy_term_load($prole_tid);
		$pstatus_term = taxonomy_term_load($pstatus_tid);
		$register_as_options[$key] = $prole_term->name . ': ' . $pstatus_term->name;
	}

	$form['register_as'] = array(
		'#title' => 'Register as',
		'#type' => 'select',
		'#options' => $register_as_options,
	);

	$form['notify'] = array(
		'#title' => 'Notify by',
		'#type' => 'select',
		'#options' => array(
			'NULL' => t('No notification'),
			'sms' => t('Text message'),
			'email' => t('Email message'),
			'both' => t('Both text and email messages'),
		),
	);

	$form['notify_body'] = array(
		'#type' => 'textarea',
		'#title' => t('Add a message'),
		'#states' => array(
			'visible' => array(
				':input[name="notify"]' => array(
					array('value' => 'sms'),
					array('value' => 'email'),
					array('value' => 'both')
				),
			),
		),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Register',
	);

	return $form;
}

function sirius_event_register_user_select_form_submit($form, &$form_state) {
	$event_nid = $form_state['event_nid'];
	$event_node = node_load($event_nid);

	$contact_nid = $form_state['values']['contact_nid'];
	$contact_node = node_load($contact_nid);

	$register_as = $form_state['values']['register_as'];
	list ($prole_tid, $pstatus_tid) = explode('|', $register_as);
	$prole_term = taxonomy_term_load($prole_tid);
	$pstatus_term = taxonomy_term_load($pstatus_tid);

  $event_type_tid = $event_node->field_sirius_event_type['und'][0]['tid'];
  $event_type_term = taxonomy_term_load($event_type_tid);
  $json = json_decode($event_type_term->field_sirius_json['und'][0]['value'], TRUE);
  global $base_url;
  $url = $base_url . '/node/' . $event_nid;
  if (sirius_event_bullpen_enabled_type($event_type_term)) { $url .= "/sirius_event_bullpen/view"; }

	$result = sirius_event_participant_register($event_nid, $contact_nid, $prole_tid, $pstatus_tid);
	if ($result['success']) { 
		drupal_set_message($result['msg']);
	} else { 
		drupal_set_message($result['msg'], 'error'); 
		return;
	}

  if ($form_state['values']['notify'] == 'sms' || $form_state['values']['notify'] == 'both') {
		$phone = $contact_node->field_sirius_phone['und'][0]['value'];
		if (!$phone) { 
			drupal_set_message("Unable to notify by SMS: the contact has no phone number listed.");
		} else {
	    $params = array();
	    $params['sms'] = "You have been registered for " . $event_node->title . " as a " . $prole_term->name . " with a status of " . $pstatus_term->name;
	    $params['sms'] .= "\n";
	    $params['sms'] .= $url;
	    if ($form_state['values']['notify_body']) {
	    	$params['sms'] .= "\n" . $form_state['values']['notify_body'];
	    }

      $params['to'] = $phone;
      $result = sirius_sms_send($params);
			if ($result['success']) { drupal_set_message("Notified by SMS: $phone"); } else { drupal_set_message($result['msg'], 'warning'); }
    }
  }

  if ($form_state['values']['notify'] == 'email' || $form_state['values']['notify'] == 'both') {
		$email = $contact_node->field_sirius_email['und'][0]['value'];
		if (!$email) { 
			drupal_set_message("Unable to notify by Email: the contact has no email address listed.");
		} else {
	    $params = array();
	    $params['subject'] = "You have been registered for " . $event_node->title;
	    $params['body'] = "You have been registered for " . $event_node->title . " as a " . $prole_term->name . " with a status of " . $pstatus_term->name;
	    $params['body'] .= "<br /><br />";
	    $params['body'] .= "<a href=\"$url\">$url</a>";
	    if ($form_state['values']['notify_body']) {
	    	$params['body'] .= "<br /><br />" . $form_state['values']['notify_body'];
	    }

      $params['to'] = $email;
      $result = sirius_email_send($params);
			if ($result['success']) { drupal_set_message("Notified by email: $email"); } else { drupal_set_message($result['msg'], 'warning'); }
    }
  }
}
