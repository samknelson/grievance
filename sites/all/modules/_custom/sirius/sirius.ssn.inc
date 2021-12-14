<?php

function sirius_ssn_format($string) {
	$string = preg_replace('/[^\d]/', '', $string);
	if (strlen($string) != 9) { return ''; }
	return substr($string, 0, 3) . '-' . substr($string, 3, 2) . '-' . substr($string, 5, 4);
}

function sirius_ssn_validate($string) {
	$string = sirius_ssn_format($string);

	return preg_match('/^\d\d\d\-\d\d\-\d\d\d\d$/', $string);
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
