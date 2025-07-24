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
	$ts = strtotime($date);
	$t = date('t', $ts);
	$date_end = "$year-$month-$t 23:59:59";
	$ts_end = strtotime($date_end);
	$row['ts'] = $ts;
	$row['date'] = $date;
	$row['date_end'] = $date_end;
	$row['ts_end'] = $ts_end;

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
    if (!$employer_nid) { return array('success' => FALSE, 'msg' => "Employer not found: $employer_id_from"); }
    $employer_note = $employer_record['note'];
	  $row['employer_nid'] = $employer_nid;
  }

  if ($precompute && $employer_nid) {
 		$payperiod_node = sirius_hour_payperiod_load(sirius_domain_nid(), $worker_nid, $employer_nid, $year, $month, $day);

 		// Hours is now a formatted number with a decimal. The only oddity is that it's prefixed with a "+" if positive.
 		$hrs = $row['M'];
 		$hrs = preg_replace('/^\+/', '', $hrs);
 		if (!preg_match('/^\-?[0-9\.]+$/', $hrs)) {
 			return array('success' => FALSE, 'msg' => 'Hours is invalid.');
 		}

 		$n = array();
 		foreach (array('I', 'J', 'K') as $col) {
 			if ($row[$col]) { $n[] = $row[$col]; }
 		}
 		$n[] = "[SEQ: " . $row['E'] . ']';
 		$comment = join("\n", $n);

		$json = sirius_json_get($payperiod_node);
 		$json_orig = $json;
		$json['entries']['import']['source_name'] = 'Import';
		$json['entries']['import']['status'] = 'live';

		$entry = array();
		$entry['day'] = 1;
		$entry['department'] = $dept_tid;
		$entry['hour_type'] = $hour_type_tid;
		$entry['qty'] = $hrs;
		$entry['note'] = '';
		$entry['comment'] = $comment;

		// drupal_set_message("$date: $hrs");

		// Find by sequence. It would be nice if "sequence" was just the record number.
		// But we don't trust "sequence" to be actually sequential. And we don't have an ID
		// field. So we search by patterns in the comments
		$pp_rows = $json['entries']['import']['rows'];
		$delta = 0;
		if (!$pp_rows) {
			$delta = 0;
		} else {
			$pattern = '/\[SEQ\: ' . $row['E'] . '\]/';
			foreach ($pp_rows as $delta => $pp_row) {
				if (preg_match($pattern, $pp_row['comment'])) {
					$found = TRUE;
					break;
				}
			}
			if (!$found) { $delta++; }
		}

		$json['entries']['import']['rows'][$delta] = $entry;

		$new_elig_hrs = preg_replace('/^0+/', '', $row['O']);
		$old_elig_hrs = $json['smf']['elig_hrs'];
		if (!$new_elig_hrs) { $elig_hrs = $old_elig_hrs; }
		else if (!$old_elig_hrs) { $elig_hrs = $new_elig_hrs; }
		else { $elig_hrs = min($new_elig_hrs, $old_elig_hrs); }
		$json['smf']['elig_hrs'] = $elig_hrs;
		$json['smf']['employer_id_to'] = $employer_id_to;
		$json['smf']['employer_id_from'] = $employer_id_from;

	  // Set the pseudo-work status for the month as disability or FMLA.
	  if ($row['N'] == 'FL') {
	  	$json['smf']['status']['fmla'] = TRUE;
	  }
	  if (in_array($row['G'], array('D9999', 'I9999'))) {
	  	$json['smf']['status']['disability'] = TRUE;
	  }

		$change = sirius_revision_changed_impl($json['entries'], $json_orig['entries'], 10);
		if (!$change) { $change = sirius_revision_changed_impl($json['smf'], $json_orig['smf'], 10); }

		if (!$change) {
			$row['payperiod_unchanged'] = TRUE;
		} else {
			if (!$payperiod_node) {
		 		$payperiod_node = sirius_hour_payperiod_load(sirius_domain_nid(), $worker_nid, $employer_nid, $year, $month, $day, 'Yes');
			}
			sirius_hour_payperiod_compute($json);
			sirius_json_set($payperiod_node, $json);
	 		$payperiod_node->field_sirius_active['und'][0]['value'] = 'Yes';
		  $row['payperiod_node'] = $payperiod_node;
		}
 	}

  // Looks good
  $result = array();
  $result['warnings'] = $warnings;
  $result['success'] = TRUE;
  $msg = 'Looks good!';
	$result['msg'] = $msg;
	return $result;
}


