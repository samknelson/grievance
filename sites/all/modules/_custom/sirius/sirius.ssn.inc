<?php

function sirius_ssn_format($string) {
	$string = preg_replace('/[^\d]/', '', $string);
	if (strlen($string) != 9) { return ''; }
	return substr($string, 0, 3) . '-' . substr($string, 3, 2) . '-' . substr($string, 5, 4);
}

function sirius_ssn_validate($string) {
	$result = sirius_ssn_validate_full($string);
	return $result['success'];
}

function sirius_ssn_format_masked($string) {
  return 'xxx-xx-' . substr($string, -4);
}

function sirius_ssn_format_conditional($string) {
	if (user_access('sirius ssn')) {
		return sirius_ssn_format($string);
	} else {
		return sirius_ssn_format_masked($string);
	}
}

function sirius_ssn_validate_full($string, $options = array()) {
	$formatted = sirius_ssn_format($string);
	if (!preg_match('/^\d\d\d\-\d\d\-\d\d\d\d$/', $formatted)) {
		return array('success' => FALSE, 'msg' => 'Does not have the format ddd-dd-dddd.', 'err' => 'BAD_FORMAT');
	}

	if (preg_match('/^(000|666|999)/', $formatted)) {
		return array('success' => FALSE, 'msg' => 'Bad area.', 'err' => 'AREA_ERR', 'formatted' => $formatted);
	}

	if (preg_match('/\-00\-/', $formatted)) {
		return array('success' => FALSE, 'msg' => 'Bad group.', 'err' => 'GROUP_00', 'formatted' => $formatted);
	}

	if (preg_match('/0000$/', $formatted)) {
		return array('success' => FALSE, 'msg' => 'Bad serial.', 'err' => 'SERIAL_0000', 'formatted' => $formatted);
	}

	$known_fake = array(
		'078-05-1120',
		'219-09-9999',
		'123-45-6789',
		'987-65-4321',
		'000-00-0000',
		'111-11-1111',
   	'222-22-2222',
   	'333-33-3333',
   	'444-44-4444',
   	'555-55-5555',
   	'666-66-6666',
   	'777-77-7777',
		'888-88-8888',
		'999-99-9999',
	);

	if (in_array($formatted, $known_fake)) {
		return array('success' => FALSE, 'msg' => 'Known fake.', 'err' => 'KNOWN_FAKE', 'formatted' => $formatted);
	}

	return array('success' => TRUE, 'formatted' => $formatted);
}