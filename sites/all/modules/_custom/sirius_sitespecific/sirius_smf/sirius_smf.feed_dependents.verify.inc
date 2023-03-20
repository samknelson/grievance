<?php

sirius_require('sirius_smf', 'sirius_smf.feed_dependents.verify_relationship.inc');
sirius_require('sirius_smf', 'sirius_smf.feed_dependents.verify_coverage.inc');

function sirius_smf_feed_dependents_row_verify_rel($feed_node, $info, $data, &$row, $precompute) {
	$warnings = array();

	// Load the fields
	$fields = &drupal_static(__FUNCTION__);
  if (!isset($fields)) { $fields = sirius_feed_wizard_fields($info, $data, $feed_node, $current_step); }

  // "Pending"
  if ($row['AD'] == 'P') {
    return array('success' => FALSE, 'msg' => 'Since BPA-DEP2-ENROLL = "P", skipping this record.');
  }

  // 0-pad our SSN's, buggerit
  foreach (array('C', 'F') as $key) {
  	while (strlen($row[$key]) < 9) { $row[$key] = "0" . $row[$key]; }
  }

  // ID2 for the worker
  $row['id2'] =  $row['BC'];
  if (!$row['id2']) { return array('success' => FALSE, 'msg' => 'No dependent IDs.'); }

  // SSN (if it's valid)
  $ssn_candidate = $row['F'];
  if (sirius_ssn_validate($ssn_candidate)) {
    $row['ssn'] = sirius_ssn_format($ssn_candidate);
  }

  if ($precompute) {

    // If we have an SSN, always look up by that.
    if ($row['ssn']) {
      $result = sirius_feed_lookup_by_field('sirius_worker', 'field_data_field_sirius_ssn', 'field_sirius_ssn_value', $row['ssn']);
      $msg = "(lookup by SSN)";
    }

    // If we don't have an SSN, look up by ID2.
    if (!$row['ssn']) {
      $result = sirius_feed_lookup_by_field('sirius_worker', 'field_data_field_sirius_id2', 'field_sirius_id2_value', $row['id2']);
      $msg = "(lookup by ID2)";
    }

		if ($result['success']) {
	    // We found exactly one by SSN
	    $row['worker_nid'] = $result['nid'];
	    $msg = "An existing worker will be updated $msg";
	  } else {
      if ($result['not_found'] && $data['create_new_records'] == 'Yes') { 
        // Not found, but we can create new records.
        $msg = "A new worker will be created.";
      } else {
        // We can't create new records, or we have a different kind of error
        return $result;
      }
	  }
  }

  // SSN for the member
  if (!sirius_ssn_validate($row['C'])) { return array('success' => FALSE, 'msg' => 'Invalid subscriber SSN.');  }
  $row['subscriber_ssn'] = sirius_ssn_format($row['C']);
  if ($precompute) {
  	$result = sirius_feed_lookup_by_field('sirius_worker', 'field_data_field_sirius_ssn', 'field_sirius_ssn_value', $row['subscriber_ssn']);

    if (!$result['success']) {
      $result['msg'] = "[subscriber] " . $result['msg'];
      return $result;
    }

    $row['subscriber_worker_nid'] = $result['nid'];
  }

  // Phone
  if ($row['X']) {
  	$row['phone'] = $row['X'] . ' ' . $row['Y'];
  	if (!sirius_phone_validate($row['phone'])) {
  		$warnings[] = 'Invalid phone number.';
  		$row['phone'] = '';
  	} else {
  		$row['phone'] = sirius_phone_format($row['phone']);
  	}
  }

  // Email
  /*
  if ($row['ACK']) {
  	$row['email'] = $row['ACK'];
  	if (!sirius_email_validate($row['email'])) {
  		$warnings[] = 'Invalid email address.';
  		$row['email'] = '';
  	} else {
  		$row['email'] = sirius_email_format($row['email']);
  	}
  }
  */

  // Gender
  if ($row['AA']) {
    $value = sirius_taxonomy_lookup_by_anything('sirius_gender', $row['AA']);
    if (!$value) { 
    	$warnings[] = "Gender must be one of the values in the taxonomy: sirius_gender.";
    	$row['gender_tid'] = NULL;
    } else {
    	$row['gender_tid'] = $value;
    }
  }

  // Birth date
  if ($row['Z']) {
  	$d = $row['Z'];
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

  // Relationship 
  if ($precompute) {
  	$result = sirius_smf_feed_dependents_row_verify_relationship($feed_node, $info, $data, $row, $fields);
  	if (!$result['success']) { $warnings[] = $result['msg']; }
	}

	// Coverage
  /*
  if ($precompute) {
  	$result = sirius_smf_feed_dependents_row_verify_coverage($feed_node, $info, $data, $row, $fields);
  	if (!$result['success']) { $warnings[] = $result['msg']; }
	}
  */

  // Looks good
  $result = array();
  $result['warnings'] = $warnings;
  $result['success'] = TRUE; 
  if (!$precompute) { $msg = 'Looks good!'; }
	$result['msg'] = $msg;
	return $result;
}












function sirius_smf_feed_dependents_row_verify_cov($feed_node, $info, $data, &$row, $precompute) {
  // Load the fields
  $fields = &drupal_static(__FUNCTION__);
  if (!isset($fields)) { $fields = sirius_feed_wizard_fields($info, $data, $feed_node, $current_step); }

 // Only "Enroll" records
  // "Pending"
  if ($row['AD'] != 'E') {
    return array('success' => FALSE, 'msg' => 'Since BPA-DEP2-ENROLL != "E", skipping this record.');
  }

  // Run the relationship verifier
  $result = sirius_smf_feed_dependents_row_verify_rel($feed_node, $info, $data, $row, $precompute);
  $warnings = $result['warnings'];
  if (!$warnings) { $warnings = array(); }
  if (!$result['success']) { return $result; }

  // Coverage
  if ($precompute) {
    $result = sirius_smf_feed_dependents_row_verify_coverage($feed_node, $info, $data, $row, $fields);
    if (!$result['success']) {
      $result['warnings'] = $warnings;
      return $result;
    }
  }

  // Looks good
  $result = array();
  $result['warnings'] = $warnings;
  $result['success'] = TRUE; 
  $result['msg'] = 'Looks good!';
  return $result;
}



function sirius_smf_feed_dependents_date($fields, $row, $field_name) {
  $col = sirius_smf_feed_dependents_field_name($fields, $field_name);
  if (!$col) { return array('success' => FALSE, 'msg' => "Field name not found: $field_name"); }

  $raw = $row[$col];
  $year = substr($raw, 0, 4);
  $month = substr($raw, 4, 2);
  $day = '01';

  if (!$year || $year <= 1900 || !$month || $month < 1 || $month > 12) { return array('success' => FALSE, 'msg' => "Invalid or missing date in $field_name / $col", 'warnings' => $warnings); }
  if (strlen($month) < 2) { $month = "0$month"; }

  return array('success' => TRUE, 'date' => "$year-$month-$day 00:00:00");
}