<?php

/**
 * A stupid little library that currently only handles US phone numbers
 */


/**
 * This is copied from the "phone" module, format_ca_phone_number().  I 
 * don't like any of the "real" phone fields, so I'm just hardcoding this for
 * now.  @todo handle phone numbers appropriately, with validation and a real
 * phone number field.
 */

function sirius_phone_format($phonenumber) {
  // define regular expression
  $regex = '/
    \D*            # ignore non-digits
    (\d*)          # an optional 1
    \D*            # optional separator
    ([2-9][0-8]\d) # area code (Allowed range of [2-9] for the first digit, [0-8] for the second, and [0-9] for the third digit)
    \D*            # optional separator
    ([2-9]\d{2})   # 3-digit prefix (cannot start with 0 or 1)
    \D*            # optional separator
    (\d{4})        # 4-digit line number
    \D*            # optional separator
    (\d*)          # optional extension
    \D*            # ignore trailing non-digits
    /x';

  // get digits of phone number
  preg_match($regex, $phonenumber, $matches);

  if (!$matches) { return $phonenumber; }

  $separator = '-';

  // construct ten-digit phone number
  $phonenumber = '(' . $matches[2] . ') ' . $matches[3] . $separator . $matches[4];

  // Optional extension
  if ($matches[5] != '') {
      $phonenumber .= ' x' . $matches[5];
  }

  return $phonenumber;
}

function sirius_phone_validate($phonenumber) {
	$phonenumber = sirius_phone_format($phonenumber);
	if (!preg_match('/\(\d\d\d\)\ \d\d\d\-\d\d\d\d/', $phonenumber)) {
		return FALSE;
	}

	return TRUE;
}

function sirius_phone_format_e164($phonenumber) {
	$phonenumber = preg_replace('/[^\d]/', '', $phonenumber);
  if (strlen($phonenumber) < 11) { $phonenumber = '1' . $phonenumber; }
  $phonenunmber = '+' . $phonenumber;
	return $phonenumber;
}
