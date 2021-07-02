<?php

function sirius_event_interestlist_cancel_impl($interestlist_node, $cancel_json = NULL) {
	$interestlist_node->field_sirius_type['und'][0]['value'] = 'canceled';
	sirius_jsonfield_log($interestlist_node, 'interestlist_cancel', 'Interest List Canceled');
	if ($cancel_json) {
		$json = sirius_jsonfield_util_node_json_get($interestlist_node);
		$json = array_merge_recursive($json, $cancel_json);
		sirius_jsonfield_util_node_json_set($interestlist_node, $json);
	}
	node_save($interestlist_node);
	return array('success' => TRUE, 'msg' => 'Interest list signup canceled.');
}

function sirius_event_interestlist_cancel($event_type_term, $contact_nid, $cancel_json = NULL) {
	$interestlist_node = sirius_event_interestlist_lookup($event_type_term, $contact_nid);
	if (!$interestlist_node) { return array('success' => TRUE, 'msg' => t('No signup found.')); }
	return sirius_event_interestlist_cancel_impl($interestlist_node, $cancel_json);
}

function sirius_event_interestlist_signup($event_type_term, $contact_nid, $json) {
	$lookup = sirius_event_interestlist_lookup($event_type_term, $contact_nid);
	if ($lookup) { return array('success' => TRUE, 'msg' => 'Already signed up for this interest list.'); }
	$event_type_proxy_node = sirius_taxonomy_proxy($event_type_term->tid);

	sirius_log(
	  'event_interestlist', 
	  'active', 
	  'Interest List', 
	  '', 
	  '', 
	  array($event_type_proxy_node->nid, $contact_nid), 
	  NULL,
	  $json);

	$msg = "Participant created.";
	if ($json['notification']['medium']) {
		$contact_node = node_load($contact_nid);
		$contact_email = $contact_node->field_sirius_email['und'][0]['value'];
		$contact_phone = $contact_node->field_sirius_phone['und'][0]['value'];

		if (in_array($json['notification']['medium'], array('sms', 'both')) && $contact_phone) {
			$params = array();
      $params['sms'] = $json['notification']['sms'];
      $params['to'] = $contact_phone;
      $result = sirius_sms_send($params);
      if ($result['success']) { $msg .= " SMS sent to: $contact_phone."; } else { $msg .= " SMS to $contact_phone failed: " . $result['msg']; }
    }

		if (in_array($json['notification']['medium'], array('email', 'both')) && $contact_email) {
			$params = array();
      $params['body'] = $json['notification']['email_body'];
      $params['subject'] = $json['notification']['email_subject'];
      $params['to'] = $contact_email;
      $result = sirius_email_send($params);
      if ($result['success']) { $msg .= " Email sent to: $contact_email."; } else { $msg .= " Email to $contact_email failed: " . $result['msg']; }
    }
	}

	return array('success' => TRUE, 'msg' => 'Interest list signup successful.');
}

function sirius_event_interestlist_lookup($event_type_term, $contact_nid) {
	$domain_nid = sirius_domain_nid();
	$event_type_proxy_node = sirius_taxonomy_proxy($event_type_term->tid);

  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_log');
  $query->fieldCondition('field_sirius_category', 'value', 'event_interestlist');
  $query->fieldCondition('field_sirius_type', 'value', 'active');
  $query->fieldCondition('field_sirius_log_handler', 'target_id', $contact_nid);
  $query->fieldCondition('field_sirius_log_handler', 'target_id', $event_type_proxy_node->nid);
  if ($domain_nid) { $query->fieldCondition('field_sirius_domain', 'target_id', $domain_nid); }
  $query->propertyOrderBy('nid', 'desc');
  $stmt = $query->execute();
  if (!$stmt['node']) { return NULL; }
  return node_load(array_keys($stmt['node'])[0]);
}

function sirius_event_interestlist_enabled() {
	return (sirius_domain_variable_get('sirius_event_interestlist_enabled') == 'Yes');
}

function sirius_event_interestlist_enabled_byterm($event_type_term) {
	$json = sirius_jsonfield_util_term_json_get($event_type_term);
	return ($json['interestlist']['settings']['enabled'] == 'Yes');
}

function sirius_event_interestlist_settings_form($form, &$form_state) {
  $name = sirius_domain_variable_name('sirius_event_interestlist_enabled');
  $form[$name] = array(
    '#type' => 'select',
    '#title' => t('Enabled?'),
    '#options' => array(
    	NULL => t('-- Select --'),
      'No' => t('No'),
      'Yes' => t('Yes'),
    ),
    '#description' => t('Select "Yes" to enable interest lists for this domain. Interest lists must then be enabled individually for each event type.'),
    '#default_value' => variable_get($name, ''),
  );

	$medium_options = array(
    NULL => t('No notification'),
    'email' => t('Notify by Email'),
    'sms' => t('Notify by SMS'),
    'both' => t('Notify by both Email and SMS'),
	);

  $name = sirius_domain_variable_name('sirius_event_interestlist_notification_medium');
	$form[$name] = array(
		'#title' => t('Default Medium'),
		'#type' => 'select',
		'#options' => $medium_options,
    '#default_value' => variable_get($name, 'both'),
	);

  $name = sirius_domain_variable_name('sirius_event_interestlist_notification_email_subject');
	$form[$name] = array(
		'#type' => 'textfield',
		'#title' => t('Email Subject'),
    '#default_value' => variable_get($name, 'You have been added to the interest list for [term.name]'),
	);

  $name = sirius_domain_variable_name('sirius_event_interestlist_notification_email_body');
  $val = variable_get($name, array());
  if ($val['value']) { $def = $val['value']; } else { $def = 'You have been added to the interest list for [term.name].'; }
	$form[$name] = array(
		'#type' => 'text_format',
		'#title' => 'Email Body',
		'#default_value' => $json['notification']['register']['email_body'],
		'#format' => 'sirius_form_editor',
		'#allowed_formats' => array('sirius_form_editor'),
    '#default_value' => $def,
	);

  $name = sirius_domain_variable_name('sirius_event_interestlist_notification_sms');
	$form[$name] = array(
		'#type' => 'textarea',
		'#title' => t('SMS'),
    '#default_value' => variable_get($name, 'You have been added to the interest list for [term.name].'),
	);

  return system_settings_form($form);
}


function sirius_event_interestlist_list($event_type_term, $params = array()) {
 	$domain_nid = sirius_domain_nid();
	$event_type_proxy_node = sirius_taxonomy_proxy($event_type_term->tid);

	// SQL for speed
	$sql_args = array();
	$sql_args[':proxy_nid'] = $event_type_proxy_node->nid;
	$sql = "select node.nid as interestlist_nid, ";
	$sql .= "node.created as interestlist_signup_ts, ";
	$sql .= "interestlist_json.field_sirius_json_value as interestlist_json, ";
	$sql .= "contact_node.nid as contact_nid, ";
	$sql .= "contact_node.title as contact_title, ";
	$sql .= "field_sirius_phone_value as contact_phone, ";
	$sql .= "field_sirius_email_value as contact_email, ";
	$sql .= "worker_node.nid as worker_nid, ";
	$sql .= "worker_node.title as worker_title, ";
	$sql .= "group_concat(distinct ms_term.name order by ms_term.name separator ', ') as worker_ms_name, ";
	$sql .= "ws_term.tid as worker_ws_tid, ";
	$sql .= "ws_term.name as worker_ws_name, ";
	$sql .= "field_sirius_id_value as worker_id ";
	$sql .= "from node ";
	$sql .= "left join field_data_field_sirius_category on field_data_field_sirius_category.entity_type = 'node' and field_data_field_sirius_category.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_type on field_data_field_sirius_type.entity_type = 'node' and field_data_field_sirius_type.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_json interestlist_json on interestlist_json.entity_type = 'node' and interestlist_json.entity_id = node.nid ";

	$sql .= "left join field_data_field_sirius_log_handler handler_term on handler_term.entity_type = 'node' and handler_term.entity_id = node.nid ";
	$sql .= "left join node proxy_node on proxy_node.nid = handler_term.field_sirius_log_handler_target_id ";

	$sql .= "left join field_data_field_sirius_log_handler handler_contact on handler_contact.entity_type = 'node' and handler_contact.entity_id = node.nid ";
	$sql .= "left join node contact_node on handler_contact.field_sirius_log_handler_target_id = contact_node.nid ";

	$sql .= "left join field_data_field_sirius_phone on field_data_field_sirius_phone.entity_type = 'node' and field_data_field_sirius_phone.entity_id = contact_node.nid ";
	$sql .= "left join field_data_field_sirius_email on field_data_field_sirius_email.entity_type = 'node' and field_data_field_sirius_email.entity_id = contact_node.nid ";
	$sql .= "left join field_data_field_sirius_contact worker_contact on worker_contact.entity_type = 'node' and worker_contact.bundle = 'sirius_worker' and worker_contact.field_sirius_contact_target_id = contact_node.nid ";
	$sql .= "left join node worker_node on worker_contact.entity_id = worker_node.nid ";

	$sql .= "left join field_data_field_sirius_work_status on field_data_field_sirius_work_status.entity_type = 'node' and field_data_field_sirius_work_status.entity_id = worker_node.nid ";
	$sql .= "left join taxonomy_term_data ws_term on ws_term.tid = field_sirius_work_status_tid ";

	$sql .= "left join field_data_field_sirius_member_status on field_data_field_sirius_member_status.entity_type = 'node' and field_data_field_sirius_member_status.entity_id = worker_node.nid ";
	$sql .= "left join taxonomy_term_data ms_term on ms_term.tid = field_sirius_member_status_tid ";

	$sql .= "left join field_data_field_sirius_id on field_data_field_sirius_id.entity_type = 'node' and field_data_field_sirius_id.entity_id = worker_node.nid ";

	$sql .= "where node.type = 'sirius_log' ";
	$sql .= "and contact_node.type = 'sirius_contact' ";
	$sql .= "and proxy_node.type = 'sirius_term_proxy' ";
	$sql .= "and proxy_node.nid = :proxy_nid ";
	$sql .= "and field_sirius_category_value = 'event_interestlist' ";
	$sql .= "and field_sirius_type_value = 'active' ";
	if ($domain_nid) {
		$sql .= "and field_sirius_domain_target_id = :domain_nid ";
		$sql_args[':domain_nid'] = $domain_nid;
	}
	$sql .= "group by node.nid ";
	$sql .= "order by node.created desc ";
	$sql .= "limit 1000 offset 0 ";
	$stmt = sirius_sql_query($sql, $sql_args);
	$result = array();
	while ($hr = $stmt->fetchAssoc()) {
		$hr['interestlist_json'] = json_decode($hr['interestlist_json'], TRUE);
		$result[] = $hr;
	}

	if ($params['include_event_registrations']) {
		foreach ($result as $delta => $record) {
			$contact_nid = $record['contact_nid'];

			$sql_args = array();
			$sql_args[':contact_nid'] = $contact_nid;
			$sql_args[':event_type_tid'] = $event_type_term->tid;
			$sql = "select field_sirius_event_prole_tid as prole_tid, field_sirius_event_pstatus_tid as pstatus_tid, count(*) as count, max(node.created) as last_ts ";
			$sql .= "from node ";
			$sql .= "left join field_data_field_sirius_contact on field_data_field_sirius_contact.entity_type = 'node' and field_data_field_sirius_contact.entity_id = node.nid ";
			$sql .= "left join field_data_field_sirius_event on field_data_field_sirius_event.entity_type = 'node' and field_data_field_sirius_event.entity_id = node.nid ";
			$sql .= "left join field_data_field_sirius_event_pstatus on field_data_field_sirius_event_pstatus.entity_type = 'node' and field_data_field_sirius_event_pstatus.entity_id = node.nid ";
			$sql .= "left join field_data_field_sirius_event_prole on field_data_field_sirius_event_prole.entity_type = 'node' and field_data_field_sirius_event_prole.entity_id = node.nid ";
			$sql .= "left join node event_node on event_node.nid = field_sirius_event_target_id ";
			$sql .= "left join field_data_field_sirius_event_type on field_data_field_sirius_event_type.entity_type = 'node' and field_data_field_sirius_event_type.entity_id = event_node.nid ";
			$sql .= "where node.type = 'sirius_event_participant' ";
			$sql .= "and field_sirius_contact_target_id = :contact_nid ";
			$sql .= "and field_sirius_event_type_tid = :event_type_tid ";
			$sql .= "group by field_sirius_event_prole_tid, field_sirius_event_pstatus_tid ";
			$stmt = sirius_sql_query($sql, $sql_args);
			while ($hr = $stmt->fetchAssoc()) {
				$result[$delta]["register"][$hr['prole_tid'] . '|' . $hr['pstatus_tid']] = $hr;
			}
		}
	}

	return $result;
}

function sirius_interestlist_auto_cancel($event_type_term, $participant_node) {
	$event_type_json = sirius_jsonfield_util_term_json_get($event_type_term);
	$auto_cancel = $event_type_json['interestlist']['settings']['auto_cancel'];
	if (!$auto_cancel) { return; }
	$auto_cancel = array_filter(array_values($auto_cancel));
	if (!$auto_cancel) { return; }

	$contact_nid = $participant_node->field_sirius_contact['und'][0]['target_id'];
	$interestlist_node = sirius_event_interestlist_lookup($event_type_term, $contact_nid);
	if (!$interestlist_node) { return; }

	$prole_tid = $participant_node->field_sirius_event_prole['und'][0]['tid'];
	$pstatus_tid = $participant_node->field_sirius_event_pstatus['und'][0]['tid'];
	$my_key = "$prole_tid|$pstatus_tid";

	foreach ($auto_cancel as $key) {
		if ($my_key == $key) {
			sirius_event_interestlist_cancel($event_type_term, $contact_nid, array('interestlist' => array('cancel' => array('comments' => 'Signed up for an event, participant nid ' . $participant_node->nid))));
			sirius_jsonfield_log($participant_node, 'interestlist_cancel', 'Interest list entry canceled');
			node_save($participant_node);
			// drupal_set_message("Canceling: $contact_nid because $my_key == $key");
			return;
		}
	}
}
