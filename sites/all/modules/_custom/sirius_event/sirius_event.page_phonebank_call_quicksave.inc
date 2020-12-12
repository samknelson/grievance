<?php

/**
 * @todo: Incomplete, under development
 */

function sirius_event_phonebank_quicksave_error($msg) {
	drupal_json_output(array('success' => FALSE, 'msg' => $msg, 'request' => $_REQUEST)); 
	drupal_exit();
}

function sirius_event_phonebank_quicksave() {
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// First we do access control. The menu item access for this function returns TRUE; we're responsible for handling access control here.
	// We assume that, if a user has a lock on a phone number for a given event, they have access to any participant at that event with that phone number.
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Get the user
	global $user;
	if (!$user) { sirius_event_phonebank_quicksave_error("You are not currently logged in to Sirius."); }

	// Get our inputs. We should be passed an event NID, a participant NID, and a phone number
	$key = $_REQUEST['key'];
	list ($event_nid, $participant_nid, $phone) = explode('|', $key);
	if (!$event_nid || !$participant_nid || !$phone) { sirius_event_phonebank_quicksave_error('Need an event_nid, a participant_nid, and a phone number.'); }

	// Verify that we have a lock on this phone number for this event
	$lock_type = sirius_event_phonebank_lock_type($event_nid);
	$lock_check = sirius_lock_check($lock_type, $phone);
	if ($lock_check['uid'] != $user->uid) { sirius_event_phonebank_quicksave_error('Your user does not have a lock on: ' . $phone); }

	// Verify that the phone number is associated with this participant and event. As a side effect, load some additional values that we will need later.
	$sql_args = array();
	$sql = "select field_sirius_phone_value, ";
	$sql .= "participant_json.field_sirius_json_value as participant_json_value ";
	$sql .= "from node ";
	$sql .= "left join field_data_field_sirius_json participant_json on participant_json.entity_type = 'node' and participant_json.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_event on field_data_field_sirius_event.entity_type = 'node' and field_data_field_sirius_event.entity_id = node.nid ";
	$sql .= "left join node event_node on event_node.nid = field_sirius_event_target_id ";
	$sql .= "left join field_data_field_sirius_contact on field_data_field_sirius_contact.entity_type = 'node' and field_data_field_sirius_contact.entity_id = node.nid ";
	$sql .= "left join node contact_node on contact_node.nid = field_sirius_contact_target_id ";
	$sql .= "left join field_data_field_sirius_phone on field_data_field_sirius_phone.entity_type = 'node' and field_data_field_sirius_phone.entity_id = contact_node.nid ";
	$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = contact_node.nid ";
	$sql .= "where node.nid = :participant_nid ";
	$sql_args[':participant_nid'] = $participant_nid;
	$sql .= "and event_node.nid = :event_nid ";
	$sql_args[':event_nid'] = $event_nid;
	$domain_nid = sirius_domain_nid();
	if ($domain_nid) {
		$sql .= "and field_sirius_domain_target_id = :domain_nid ";
		$sql_args[':domain_nid'] = $domain_nid;
	}
	$stmt = sirius_sql_query($sql, $sql_args);
	$hr = $stmt->fetchAssoc();
	$check_phone = $hr['field_sirius_phone_value'];
	if (sirius_phone_format($check_phone) != sirius_phone_format($phone)) { sirius_event_phonebank_quicksave_error("Access denied: The contact's phone number, $check_phone, does not match the requested phone number, $phone"); }
	$participant_json = json_decode($hr['participant_json_value'], TRUE);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// We have access to this participant NID. We are able to set the pstatus and the json values for this participant.
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Get the JSON path and value that we're setting
	$json_value = $_REQUEST['json_value'];
	$json_id = $_REQUEST['json_id'];
	if (!$json_id) { sirius_event_phonebank_quicksave_error("No json_id attribute associated with this element."); }
	if (!preg_match('/^sirius_json_id__/', $json_id)) { sirius_event_phonebank_quicksave_error("Unable to parse json_id $json_id."); }
	$json_path = preg_replace('/^sirius_json_id__/', ':', $json_id);
	$json_path = preg_replace('/__/', ':', $json_path);
	$json_path = ':survey:results' . $json_path;

	// Set the value.
	sirius_jsonfield_util_set_from_path($participant_json, $json_path, $json_value);
	$participant_json['survey']['results_encoded'] = json_encode($participant_json['survey']['results']);

	// Write the new value to the database. We're writing directly for speed.
	$sql_args = array(':json' => json_encode($participant_json), ':participant_nid' => $participant_nid);
	$sqls = array(
		"update field_data_field_sirius_json set field_sirius_json_value = :json where entity_type = 'node' and entity_id = :participant_nid ",
		"update field_revision_field_sirius_json set field_sirius_json_value = :json where entity_type = 'node' and entity_id = :participant_nid "
	);
	foreach ($sqls as $sql) { $stmt = sirius_sql_query($sql, $sql_args); }
	entity_get_controller('node')->resetCache(array($participant_nid));
	cache_clear_all("field:node:$participant_nid", 'cache_field');

	$json = array();
	$json['success'] = TRUE;
	$json['msg'] = "Setting $json_path to $json_value.";
	$json['hr'] = $hr;
	$json['participant_json'] = $participant_json;
	$json['hmm'] = array($sqls);
	$json['hmm2'] = $sql_args;

	drupal_json_output($json);
}

