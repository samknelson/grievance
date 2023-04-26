<?php

function sirius_smf_feed_notes_row_verify($feed_node, $info, $data, &$row, $precompute) {
	$warnings = array();

	// Load the fields
	$fields = &drupal_static(__FUNCTION__);
  if (!isset($fields)) { $fields = sirius_feed_wizard_fields($info, $data, $feed_node, $current_step); }

  if (!$precompute) { return array('success' => TRUE, 'msg' => 'Skipping validation because no precompute.'); }

  $note_type = $row['D'];

  if ($note_type == 1) {
    $id = $row['E'];
    if (!$id) { return array('success' => FALSE, 'msg' => 'No employer ID (column D)'); }
    $result = sirius_feed_lookup_by_field('grievance_shop', 'field_data_field_sirius_id', 'field_sirius_id_value', $id);
  } else if (in_array($note_type, array(2,3,4))) {
    $ssn = $row['E'];
    if (!sirius_ssn_validate($ssn)) { return array('success' => FALSE, 'msg' => 'Invalid SSN (column E).'); }
    $ssn = sirius_ssn_format($ssn);
    $result = sirius_feed_lookup_by_field('sirius_worker', 'field_data_field_sirius_ssn', 'field_sirius_ssn_value', $ssn);
  } else {
    return array('success' => FALSE, 'msg' => "Invalid note type: $note_type");
  }

  if (!$result['success']) { return $result; }
  $entity_nid = $row['entity_nid'] = $result['nid'];

  // Date
  $year = $row['K'];
  if ($year > 24) { $year += 1900; } else { $year += 2000; }
  $month = $row['L'];
  $day = $row['M'];
  $hour = $row['G'];
  $minute = $row['H'];
  $second = $row['I'];
  $date = $row['date'] = "$year-$month-$day $hour:$minute:$second";
  $ts = strtotime($date);
  if (! ($ts > 0)) { return array('success' => FALSE, 'msg' => 'Invalid date stamp.'); }

  $pk = $row['pk'] = 'smf:notes:' . join('|', array($row['B'], $row['C'], $row['D'], $row['E'], $row['F'], $date));
  $result = sirius_feed_lookup_by_field(
    'sirius_log', 
    'field_data_field_sirius_notes', 
    'field_sirius_notes_value', 
    $pk
  );

  if ($result['success']) {
    // We found exactly one by SSN
    $row['log_nid'] = $result['nid'];
    $msg = "An existing log entry will be updated.";
  } else {
    if ($result['not_found'] && $data['create_new_records'] == 'Yes') { 
      // Not found, but we can create new records.
      $msg = "A new log entry will be created.";
    } else {
      // We can't create new records, or we have a different kind of error
      return $result;
    }
  }

  // Looks good
  $result = array();
  $result['warnings'] = $warnings;
  $result['success'] = TRUE;
	$result['msg'] = $msg;
	return $result;
}


