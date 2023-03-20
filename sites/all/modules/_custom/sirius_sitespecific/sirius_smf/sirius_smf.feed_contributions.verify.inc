<?php

function sirius_smf_feed_contributions_row_verify($feed_node, $info, $data, &$row, $precompute) {
	$warnings = array();

	// Load the fields
	$fields = &drupal_static(__FUNCTION__);
  if (!isset($fields)) { $fields = sirius_feed_wizard_fields($info, $data, $feed_node, $current_step); }

  // We use a default department and hours type
  static $hour_type_tid;
  if (!isset($hour_type_tid)) { $hour_type_tid = sirius_taxonomy_lookup_by_anything('sirius_hour_type', 'default'); }
  if (!$hour_type_tid) { return array('success' => FALSE, 'msg' => "No hour type found with ID='default'."); }

  static $dept_tid;
  if (!isset($dept_tid)) { $dept_tid = sirius_taxonomy_lookup_by_anything('grievance_department', 'default'); }
  if (!$dept_tid) { return array('success' => FALSE, 'msg' => "No department found with ID='default'."); }

  // SSN
  $ssn = $row['C'];
  if (!sirius_ssn_validate($ssn)) { return array('success' => FALSE, 'msg' => 'Invalid SSN (column C).'); }
  $ssn = sirius_ssn_format($ssn);
  if ($precompute) {
    $result = sirius_feed_lookup_by_field('sirius_worker', 'field_data_field_sirius_ssn', 'field_sirius_ssn_value', $ssn);
 		if (!$result['success']) { return $result; }
    $worker_nid = $row['worker_nid'] = $result['nid'];
  }

  // Get the employer
  $employer_nid = NULL;
  $employer_id_from = $row['G'];
  if ($employer_id_from) {
    $employer_record = sirius_smf_feed_members_employer_mapping($employer_id_from);
    if (!isset($employer_record)) {
      return array('success' => FALSE, 'msg' => "For coverage month $coverage_month, don't know which employer ID corresponds to the incoming ID $employer_id_from");
    }
    $employer_id_to = $employer_record['employer_id'];
    $employer_nid = sirius_find_nid_by_id($employer_id_to, 'grievance_shop');
    if (!$employer_nid) {
      return array('success' => FALSE, 'msg' => "Employer not found: $employer_id_to");
    }

    $employer_note = $employer_record['note'];
    $row['employer_nid'] = $employer_nid;
  }

  // Date
  $raw = $row['H'];
  $year = substr($raw, 0, 4);
  $month = substr($raw, 4, 2);
  $day = '01';
  if (!$year || $year <= 1900 || !$month || $month < 1 || $month > 12) {
  	return array('success' => FALSE, 'msg' => "Invalid or missing date in column H", 'warnings' => $warnings);
  }
  if (strlen($month) < 2) { $month = "0$month"; }
	$date = "$year-$month-$day 00:00:00";

  if ($precompute && $employer_nid) {
 		$payperiod_node = sirius_hour_payperiod_load(sirius_domain_nid(), $worker_nid, $employer_nid, $year, $month, $day, 'Yes');

 		$payperiod_node->field_sirius_active['und'][0]['value'] = 'Yes';

 		$hrs = $row['M'] / 100;
 		if ($hrs === 0) { return array('success' => FALSE, 'msg' => 'Hours is zero.'); }
 		if (!($hrs > 0)) { return array('success' => FALSE, 'msg' => 'Hours is negative or invalid.'); }

 		$n = array();
 		foreach (array('I', 'J', 'K') as $col) {
 			if ($row[$col]) { $n[] = $row[$col]; }
 		}
 		$n[] = "[SEQ: " . $row['E'] . ']';
 		$comment = join("\n", $n);

 		$json = sirius_json_get($payperiod_node);
		$json['entries']['import']['source_name'] = 'Import';
		$json['entries']['import']['status'] = 'live';

		$entry = array();
		$entry['day'] = 1;
		$entry['department'] = $dept_tid;
		$entry['hour_type'] = $hour_type_tid;
		$entry['qty'] = $hrs;
		$entry['note'] = '';
		$entry['comment'] = $comment;

		// Find by sequence. It would be nice if "sequence" was just the record number.
		// But we don't trust "sequence" to be actually sequential. And we don't have an ID
		// field. So we search by patterns in the comments
		$rows = $json['entries']['import']['rows'];
		$delta = 0;
		if (!$rows) {
			$delta = 0;
		} else {
			$pattern = '/\[SEQ\: ' . $row['E'] . '\]/';
			foreach ($rows as $delta => $row) {
				if (preg_match($pattern, $row['comment'])) {
					$found = TRUE;
					break;
				}
			}
			if (!$found) { $delta++; }
		}

		$json['entries']['import']['rows'][$delta] = $entry;
		sirius_hour_payperiod_compute($json);
		sirius_json_set($payperiod_node, $json);
	  $row['payperiod_node'] = $payperiod_node;
 	}

  // Looks good
  $result = array();
  $result['warnings'] = $warnings;
  $result['success'] = TRUE;
  $msg = 'Looks good!';
	$result['msg'] = $msg;
	return $result;
}


