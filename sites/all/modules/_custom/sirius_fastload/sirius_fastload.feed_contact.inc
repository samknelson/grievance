<?php

function sirius_fastload_feed_contact_info() {

  $items = array();

  $items['feed_fastload_contact'] = array(
    'name' => 'feed_fastload_contact',
    'title' => 'FastLoad Contacts',
    'description' => 'Create or update contacts using the FastLoad tool. This feed can be used only if fastload is enabled, and only for the fields "Name", "Email", "Phone Number", and "Bulk Message".',
    'feed' => array(
      'pks' => array(
        'id' => 'ID',
      ),
      'batch_size' => 2000,
      'callback_row_verify' => 'sirius_feed_contact_row_verify',
      'callback_row_process' =>'sirius_fastload_feed_contact_row_process',
      'callback_fields' => 'sirius_fastload_feed_contact_fields',
    ),
    'callback_step_list' => 'sirius_fastload_feed_contact_steps',
    'callback_launch' => 'sirius_feed_wizard_launch',
    'callback_launch_args' => array('feed_fastload_contact'),
  );

  return $items;
}

function sirius_fastload_feed_contact_steps($data, $node) {
  $bulk_nids = sirius_feed_contact_upcoming_bulk_nids();
  $steps = array();
  $steps['upload'] = array('label' => 'Upload',     'callback' => 'sirius_feed_wizard_upload');
  $steps['sheet'] = array('label' => 'Worksheet',  'callback' => 'sirius_feed_wizard_sheet');
  $steps['map'] = array('label' => 'Mapping',    'callback' => 'sirius_feed_wizard_map');
  if ($bulk_nids) {
    $steps['bulk'] = array('label' => 'Bulk Message',    'callback' => 'sirius_feed_contact_bulk');
  }
  $steps['verify'] = array('label' => 'Verify', 'callback' => 'sirius_feed_wizard_verify');
  $steps['process'] = array('label' => 'Process', 'callback' => 'sirius_feed_wizard_process');
  $steps['review'] = array('label' => 'Review', 'callback' => 'sirius_feed_wizard_review');

  return $steps;
}

function sirius_fastload_feed_contact_fields($info, $data, $feed_node, $current_step) {
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
    'email' => array(
      'title' => 'Email',
      'required' => FALSE,
    ),
    'phone' => array(
      'title' => 'Phone',
      'required' => FALSE,
    ),
  );

  return $fields;
}

function sirius_fastload_feed_contact_row_process($feed_node, $info, $data, $row) {
	// Verify our data
	$result = sirius_feed_contact_row_verify($feed_node, $info, $data, $row, TRUE);
	if (!$result['success']) { return $result; }

	if ($row['contact_nid']) {
		$is_new = FALSE;
	} else {
		$contact_nid = sirius_fastload_reserve('sirius_contact');
		if (!$contact_nid) { return array('success' => FALSE, 'msg' => 'FastLoad failed to return a contact node.'); }
		$is_new = TRUE;
  }

  $sqls = array();

  if ($row['name_given'] || $row['name_middle'] || $row['name_family']) {
  	foreach (array('field_data_field_sirius_name', 'field_revision_field_sirius_name') as $table) {
	  	$sql = "update $table ";
	  	$sql .= "set field_sirius_name_given = :given, field_sirius_name_middle = :middle, field_sirius_name_family = :family ";
	  	$sql .= "where entity_type = 'node' and entity_id = :contact_nid ";
	  	$sql_args = array();
	  	$sql_args[':contact_nid'] = $contact_nid;
	  	$sql_args[':given'] = $row['name_given'];
	  	$sql_args[':middle'] = $row['name_middle'];
	  	$sql_args[':family'] = $row['name_family'];
	  	$sqls[] = array('sql' => $sql, 'sql_args' => $sql_args);
	  }
  }

  $title = ''; 
  if ($row['name_given'] || $row['name_family']) { $title = $row['name_given'] . ' ' . $row['name_family']; }
  if (!$title) { $title = 'No Name ' . $row['phone']; }
  if (!$title) { $title = 'No Name'; }
	foreach (array('node', 'node_revision') as $table) {
  	$sql = "update $table ";
  	$sql .= "set title = :title ";
  	$sql .= "where nid = :contact_nid ";
  	$sql_args = array();
  	$sql_args[':contact_nid'] = $contact_nid;
  	$sql_args[':title'] = $title;
		$sqls[] = array('sql' => $sql, 'sql_args' => $sql_args);
	}

	foreach (array('field_data_field_sirius_source', 'field_revision_field_sirius_source') as $table) {
  	$sql = "update $table ";
  	$sql .= "set field_sirius_source_value = :source ";
  	$sql .= "where entity_type = 'node' and entity_id = :contact_nid ";
  	$sql_args = array();
  	$sql_args[':contact_nid'] = $contact_nid;
  	$sql_args[':source'] = $data['source'];
  	$sqls[] = array('sql' => $sql, 'sql_args' => $sql_args);
  }

 	foreach (array('field_data_field_sirius_id', 'field_revision_field_sirius_id') as $table) {
  	$sql = "update $table ";
  	$sql .= "set field_sirius_id_value = :id ";
  	$sql .= "where entity_type = 'node' and entity_id = :contact_nid ";
  	$sql_args = array();
  	$sql_args[':contact_nid'] = $contact_nid;
  	$sql_args[':id'] = $row['id'];
  	$sqls[] = array('sql' => $sql, 'sql_args' => $sql_args);
  }

	foreach (array('field_data_field_sirius_phone', 'field_revision_field_sirius_phone') as $table) {
  	$sql = "update $table ";
  	$sql .= "set field_sirius_phone_value = :phone ";
  	$sql .= "where entity_type = 'node' and entity_id = :contact_nid ";
  	$sql_args = array();
  	$sql_args[':contact_nid'] = $contact_nid;
  	$sql_args[':phone'] = $row['phone'];
  	$sqls[] = array('sql' => $sql, 'sql_args' => $sql_args);
  }

	foreach (array('field_data_field_sirius_email', 'field_revision_field_sirius_email') as $table) {
  	$sql = "update $table ";
  	$sql .= "set field_sirius_email_value = :email ";
  	$sql .= "where entity_type = 'node' and entity_id = :contact_nid ";
  	$sql_args = array();
  	$sql_args[':contact_nid'] = $contact_nid;
  	$sql_args[':email'] = $row['email'];
  	$sqls[] = array('sql' => $sql, 'sql_args' => $sql_args);
  }

	foreach (array('field_data_field_sirius_domain', 'field_revision_field_sirius_domain') as $table) {
  	$sql = "update $table ";
  	$sql .= "set field_sirius_domain_target_id = :domain_nid ";
  	$sql .= "where entity_type = 'node' and entity_id = :contact_nid ";
  	$sql_args = array();
  	$sql_args[':contact_nid'] = $contact_nid;
  	$sql_args[':domain_nid'] = $feed_node->field_sirius_domain['und'][0]['target_id'];
  	$sqls[] = array('sql' => $sql, 'sql_args' => $sql_args);
  }

  foreach ($sqls as $sql_record) {
  	sirius_sql_query($sql_record['sql'], $sql_record['sql_args']);
  }

  if ($is_new) {
    $msg = 'Created a new contact from a FastLoad reserved node.';
  } else {
    $msg = 'Updated an existing contact.';
  }
  $success = TRUE;

  /*
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
  */

  return array('success' => $success, 'msg' => $msg);
}