<?php

sirius_require('sirius_smf', 'sirius_smf.feed_members.verify_elections.inc');
sirius_require('sirius_smf', 'sirius_smf.feed_members.verify_coverage.inc');
sirius_require('sirius_smf', 'sirius_smf.feed_members.verify_ms.inc');

function sirius_smf_feed_members_row_verify($feed_node, $info, $data, &$row, $precompute) {

	$warnings = array();

	// Load the fields
	$fields = &drupal_static(__FUNCTION__);
  if (!isset($fields)) { $fields = sirius_feed_wizard_fields($info, $data, $feed_node, $current_step); }

  // SSN
  if (!sirius_ssn_validate($row['C'])) { return array('success' => FALSE, 'msg' => 'Invalid SSN.'); }
  $row['ssn'] = sirius_ssn_format($row['C']);
  if ($precompute) {
  	$result = sirius_feed_lookup_by_field('sirius_worker', 'field_data_field_sirius_ssn', 'field_sirius_ssn_value', $row['ssn']);
		if ($result['success']) {
	    // We found exactly one. Use that NID.
	    $row['worker_nid'] = $result['nid'];
	    $msg = "An existing worker will be updated.";
	  } else {
	    if ($result['not_found'] && $data['create_new_records'] == 'Yes') { 
	      // Not found, but we can create new records. Do nothing
	      $msg = "A new worker will be created.";
	    } else {
	      // We can't create new records, or we have a different kind of error
	      return $result;
	    }
	  }
  }

  // Phone
  if ($row['U']) {
  	$row['phone'] = $row['T'] . ' ' . $row['U'];
  	if (!sirius_phone_validate($row['phone'])) {
  		$warnings[] = 'Invalid phone number.';
  		$row['phone'] = '';
  	} else {
  		$row['phone'] = sirius_phone_format($row['phone']);
  	}
  }

  // Email
  if ($row['ACK']) {
  	$row['email'] = $row['ACK'];
  	if (!sirius_email_validate($row['email'])) {
  		$warnings[] = 'Invalid email address.';
  		$row['email'] = '';
  	} else {
  		$row['email'] = sirius_email_format($row['email']);
  	}
  }

  // Gender
  if ($row['W']) {
    $value = sirius_taxonomy_lookup_by_anything('sirius_gender', $row['W']);
    if (!$value) { 
    	$warnings[] = "Gender must be one of the values in the taxonomy: sirius_gender.";
    	$row['gender_tid'] = NULL;
    } else {
    	$row['gender_tid'] = $value;
    }
  }

  // Birth date
  if ($row['V']) {
  	$d = $row['V'];
  	$year = substr($d, 0, 4);
  	$month = substr($d, 4, 2);
  	$day = substr($d, 6, 2);
  	if (!$year || $year < 1900 || !$month || $month < 1 || $month > 12 || !$day || $day < 1 || $day > 31) {
  		$warnings[] = "Invalid birth date: not well formed (i.e. year, month, or day is out of range)";
  		$row['dob'] = NULL;
  	} else if (!checkdate($month, $day, $year)) {
  		$warnings[] = "Invalid birth date: checkdate() fails (i.e. June 31)";
  		$row['dob'] = NULL;
  	} else {
  		$row['dob'] = "$year-$month-$day";
  	}
  }

  // Elections

  if ($precompute) {
  	$result = sirius_smf_feed_members_row_verify_elections($feed_node, $info, $data, $row, $fields);
  	if (!$result['success']) { $warnings[] = $result['msg']; }
	}

	// Coverage
  if ($precompute) {
  	$result = sirius_smf_feed_members_row_verify_coverage($feed_node, $info, $data, $row, $fields);
  	if (!$result['success']) { $warnings[] = $result['msg']; }
	}

	// Member Status
  if ($precompute) {
  	$result = sirius_smf_feed_members_row_verify_ms($feed_node, $info, $data, $row, $fields);
  	if (!$result['success']) { $warnings[] = $result['msg']; }
	}

  // Looks good
  $result = array();
  $result['warnings'] = $warnings;
  $result['success'] = TRUE; 
  if ($precompute) {
	  if ($row['worker_nid']) { $msg = 'An existing worker will be updated.'; } 
		else { $msg = 'A new worker will be created.'; }
	} else { $msg = 'Looks good!'; }
	$result['msg'] = $msg;
	return $result;
}

