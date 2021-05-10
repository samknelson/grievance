<?php

function sirius_feed_contact_info() {

  $items = array();

  $items['feed_contact'] = array(
    'name' => 'feed_contact',
    'title' => 'Contacts',
    'description' => 'Create or update contacts. This feed should not be used for members or workers.',
    'feed' => array(
      'pks' => array(
        'id' => 'ID',
        'nid' => 'NID',
      ),
      'batch_size' => 2000,
      'callback_row_verify' => 'sirius_feed_contact_row_verify',
      'callback_row_process' =>'sirius_feed_contact_row_process',
      'callback_fields' => 'sirius_feed_contact_fields',
    ),
    'callback_step_list' => 'sirius_feed_contact_steps',
    'callback_launch' => 'sirius_feed_wizard_launch',
    'callback_launch_args' => array('feed_contact'),
  );

  return $items;
}

function sirius_feed_contact_upcoming_bulk_nids() {
  $sql_args = array();
  $sql_args[':cutoff'] = time() - 60*60*24*90;
  $sql = "select nid, title from node ";
  $sql .= "left join field_data_field_sirius_bulk_status on field_data_field_sirius_bulk_status.entity_type = 'node' and field_data_field_sirius_bulk_status.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
  $sql .= "where field_sirius_bulk_status_value in ('draft', 'pending') ";
  $sql .= "and node.created >= :cutoff ";
  $domain_nid = sirius_domain_nid();
  if ($domain_nid) {
    $sql .= "and field_sirius_domain_target_id = :domain_nid ";
    $sql_args[':domain_nid'] = $domain_nid;
  }
  $sql .= "order by node.title ";
  $stmt = sirius_sql_query($sql, $sql_args);

  $bulk_nids = array();
  while ($hr = $stmt->fetchAssoc()) {
    $bulk_nids[$hr['nid']] = $hr['title'];
  }

  return $bulk_nids;
}

function sirius_feed_contact_steps($data, $node) {
  $bulk_nids = sirius_feed_contact_upcoming_bulk_nids();
  $steps = array();
  $steps['upload'] = array('label' => 'Upload',     'callback' => 'sirius_feed_wizard_upload');
  $steps['sheet'] = array('label' => 'Worksheet',  'callback' => 'sirius_feed_wizard_sheet');
  $steps['map'] = array('label' => 'Mapping',    'callback' => 'sirius_feed_wizard_map');
  if ($bulk_nids) {
    $steps['bulk'] = array('label' => 'Bulk Message',    'callback' => 'sirius_feed_contact_bulk');
  }
  $steps['verify'] = array('label' => 'Verify',     'callback' => 'sirius_feed_wizard_verify');
  $steps['process'] = array('label' => 'Process',    'callback' => 'sirius_feed_wizard_process');
  $steps['review'] = array('label' => 'Review',     'callback' => 'sirius_feed_wizard_review');


  return $steps;
}

function sirius_feed_contact_bulk($info, $data, $application_node, $current_step) { return drupal_get_form('sirius_feed_contact_bulk_form', $info, $data, $application_node, $current_step); }

function sirius_feed_contact_bulk_form($form, &$form_state, $info, $data, $feed_node, $current_step) {
  $form_state['info'] = $info;
  $form_state['data'] = $data;
  $form_state['feed_node'] = $feed_node;
  $form_state['current_step'] = $current_step;

  $form['#tree'] = TRUE;

  $form['bulk']['bulk_nid'] = array(
    '#type' => 'select',
    '#title' => t('Bulk Message'),
    '#options' => array(NULL => t('NONE')) + sirius_feed_contact_upcoming_bulk_nids(),
    '#description' => t('All contacts will be added to the specified bulk message.'),
    '#default_value' => $data['bulk']['bulk_nid'],
  );


  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

function sirius_feed_contact_bulk_form_submit($form, &$form_state) {
  $info = $form_state['info'];
  $data = $form_state['data'];
  $feed_node = $form_state['feed_node'];
  $current_step = $form_state['current_step'];

  $data['bulk'] = $form_state['values']['bulk'];

  sirius_wizard_submit($info, $data, $feed_node, $current_step, "Bulk Message Selection Saved");
}


function sirius_feed_contact_fields($info, $data, $feed_node, $current_step) {
  // Core fields
  $fields = array(
    'id' => array(
      'title' => 'ID',
      'required' => FALSE,
    ),
    'nid' => array(
      'title' => 'NID',
      'required' => FALSE,
    ),
    'name_given' => array(
      'title' => 'First Name',
      'required' => FALSE,
    ),
    'name_family' => array(
      'title' => 'Last Name',
      'required' => FALSE,
    ),
    'name_middle' => array(
      'title' => 'Middle Name',
      'required' => FALSE,
    ),
    'address_thoroughfare' => array(
      'title' => 'Address 1',
      'required' => FALSE,
    ),
    'address_premise' => array(
      'title' => 'Address 2',
      'required' => FALSE,
    ),
    'address_locality' => array(
      'title' => 'City',
      'required' => FALSE,
    ),
    'address_administrative_area' => array(
      'title' => 'State',
      'required' => FALSE,
    ),
    'address_postal_code' => array(
      'title' => 'Zip',
      'required' => FALSE,
    ),
    'email' => array(
      'title' => 'Email',
      'required' => FALSE,
    ),
    'phone' => array(
      'title' => 'Phone',
      'required' => FALSE,
    ),
    'phone_alt' => array(
      'title' => 'Phone (Alt)',
      'required' => FALSE,
    ),
    'phone_mobile' => array(
      'title' => 'Phone (Mobile)',
      'required' => FALSE,
    ),
    'shop_name' => array(
      'title' => 'Current Employer Name',
      'required' => FALSE,
    ),
  );

  // Json fields
  $contact_definition_json = sirius_contact_definition();
  $contact_definition_form = sirius_jsonfield_util_form_get($contact_definition_json, 'default');
  $paths = array_values(sirius_jsonfield_util_form_list_paths($contact_definition_form, TRUE));
  foreach ($paths as $path) {
    $fields["JSON:$path"] = array('title' => $path);
  }

  return $fields;
}

function sirius_feed_contact_row_verify($feed_node, $info, $data, &$row, $precompute) {
	// Trim all
  foreach ($row as $key => $value) { if (is_string($value)) { $row[$key] = trim($value); } }

	// if (!preg_match('/^\d+$/', $row['id'])) { return array('success' => FALSE, 'msg' => 'Invalid Member ID'); }

	if ($row['phone'] && !sirius_phone_validate($row['phone'])) { return array('success' => FALSE, 'msg' => 'Invalid Phone'); } else { $row['phone'] = sirius_phone_format($row['phone']); }
	if ($row['phone_alt'] && !sirius_phone_validate($row['phone_alt'])) { return array('success' => FALSE, 'msg' => 'Invalid Phone (Alt)'); } else { $row['phone_alt'] = sirius_phone_format($row['phone_alt']); }

	if ($row['email'] && !sirius_email_validate($row['email'])) { return array('success' => FALSE, 'msg' => 'Invalid Email'); }

  $domain_nid = $feed_node->field_sirius_domain['und'][0]['target_id'];

  $pk = $data['pk'];
  if (!$pk) { $pk = 'id'; }
  if ($pk == 'nid') {
   if (!preg_match('/^[0-9]+$/', $row['nid'])) { return array('success' => FALSE, 'msg' => 'Invalid NID'); }
  }

  if (!$precompute) {
    return array('success' => TRUE, 'msg' => 'Looks good.');
  }

  // Contact
  if ($pk == 'id') {
    $args = array();
    $args[':id'] = $row['id'];
    if (!$row['id']) { return array('success' => FALSE, 'msg' => 'No ID found.'); }

  	$sql = "select max(field_data_field_sirius_id.entity_id) as a, min(field_data_field_sirius_id.entity_id) as b from field_data_field_sirius_id ";
    if ($domain_nid) {
      $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = field_data_field_sirius_id.entity_id ";
    }
  	$sql .= "where field_data_field_sirius_id.entity_type = 'node' ";
  	$sql .= "and field_data_field_sirius_id.bundle = 'sirius_contact' ";
  	$sql .= "and field_data_field_sirius_id.field_sirius_id_value = :id ";
    if ($domain_nid) {
      $sql .= "and field_sirius_domain_target_id = :domain_nid";
      $args[':domain_nid'] = $domain_nid;
    }

  	$stmt = sirius_sql_query($sql, $args);
  	$hr = $stmt->fetchAssoc();
  	$nid = $hr['a'];
  	$other_nid = $hr['b'];
  	if ($nid != $other_nid) {
  		return array('success' => FALSE, 'msg' => 'Multiple contacts found with this ID.');
  	}
    $row['contact_nid'] = $nid;
  } else {
    $args = array();
    $args[':nid'] = $row['nid'];
    if (!$row['nid']) { return array('success' => FALSE, 'msg' => 'No NID found.'); }
    $sql = "select nid from node ";
    if ($domain_nid) {
      $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
    }
    $sql .= "where node.type = 'sirius_contact' ";
    if ($domain_nid) {
      $sql .= "and field_sirius_domain_target_id = :domain_nid ";
      $args[':domain_nid'] = $domain_nid;
    }
    $sql .= "and node.nid = :nid ";

    $stmt = sirius_sql_query($sql, $args);
    $hr = $stmt->fetchAssoc();
    $nid = $hr['nid'];
    if (!$nid) { return array('success' => FALSE, 'msg' => 'Contact not found in this domain.'); }
    $row['contact_nid'] = $nid;
  }

  if ($row['contact_nid']) { 
    return array('success' => TRUE, 'msg' => 'An existing record will be updated.');
  } else {
    return array('success' => TRUE, 'msg' => 'An new record will be created.');
  }
}

function sirius_feed_contact_row_process($feed_node, $info, $data, $row) {
	$result = sirius_feed_contact_row_verify($feed_node, $info, $data, $row, TRUE);
	if (!$result['success']) { return $result; }

	if ($row['contact_nid']) {
		$is_new = FALSE;
		$contact_node = node_load($row['contact_nid']);
	} else {
		$is_new = TRUE;
    global $user;
    $contact_node = new stdClass();
    $contact_node->type = 'sirius_contact';
    $contact_node->language = LANGUAGE_NONE;
    $contact_node->uid = $user->uid;
    $contact_node->status = 1;
    $contact_node->promote = 0;
    $contact_node->comment = 0;
    $contact_node->field_sirius_id['und'][0]['value'] = $row['id'];
  }

  if ($row['name_given'] || $row['name_middle'] || $row['name_family']) {
    $contact_node->field_sirius_name['und'][0]['given'] = $row['name_given'];
    $contact_node->field_sirius_name['und'][0]['middle'] = $row['name_middle'];
    $contact_node->field_sirius_name['und'][0]['family'] = $row['name_family'];
  }
  $contact_node->field_sirius_source['und'][0]['value'] = $data['source'];

  $phone1 = $row['phone'];
  $phone2 = $row['phone_alt'];
  if ($row['phone_mobile']) {
  	if (!$phone1) { $phone1 = $row['phone_mobile']; } else { $phone2 = $row['phone_mobile']; }
  }
  if ($phone1) { $contact_node->field_sirius_phone['und'][0]['value'] = $phone1; }
  if ($phone2) { $contact_node->field_sirius_phone_alt['und'][0]['value'] = $phone2; }
  if ($row['email']) { $contact_node->field_sirius_email['und'][0]['value'] = $row['email']; }

  foreach (array('thoroughfare', 'premise', 'locality', 'administrative_area', 'postal_code') as $col) {
  	$has_address = FALSE;
  	if ($row["address_$col"]) {
  		$has_address = TRUE;
  		$contact_node->field_sirius_address['und'][0][$col] = $row["address_$col"];
  	}
  	if ($has_address) {
  		$contact_node->field_sirius_address['und'][0]['country'] = 'US';
  	}
  }

  $json = sirius_jsonfield_util_node_json_get($contact_node);
  $results_json = sirius_jsonfield_util_results_get($json, 'default');
  foreach ($row as $key => $value) {
    if (preg_match('/JSON:/', $key)) {
      sirius_jsonfield_util_set_from_path($results_json, preg_replace('/JSON:/', '', $key), $value);
    }
  }
  sirius_jsonfield_util_results_set($json, 'default', $results_json);
  sirius_jsonfield_util_node_json_set($contact_node, $json);
  $contact_node->field_sirius_domain = $feed_node->field_sirius_domain;
  node_save($contact_node);

  if ($is_new) {
    $msg = 'Created a new contact.';
  } else {
    $msg = 'Updated an existing contact.';
  }
  $success = TRUE;

  $bulk_nid = $data['bulk']['bulk_nid'];
  if ($bulk_nid) {
    $result = sirius_bulk_recipient_add($bulk_nid, $contact_node->nid);
    if ($result['success']) {
      $msg .= " Added to the bulk message.";
    } else {
      $msg .= " Failed to add to the bulk message: " . $result['msg'];
      $success = FALSE;
    }
  }

  return array('success' => $success, 'msg' => $msg);
}