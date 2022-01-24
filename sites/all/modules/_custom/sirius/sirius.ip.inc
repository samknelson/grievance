<?php

function sirius_ip_rawrange_helptext($range = NULL) {
	$user_ip = sirius_loopback_remote_ip();
 	$html = '<p>';
 	$html .= t('Enter IP addresses one per line. Each entry may have a comment, e.g. ');
 	$html .= '<pre>';
 	$html .= "11.11.11.11 # A Random IP\n";
 	$html .= "12.12.12.12 # Another Random IP\n";
 	$html .= "13.13.13.13 - 13.13.13.26 # A range of IPs\n";
 	$html .= '</pre>';
 	$html .= t("Your current IP address is <strong>@user_ip</strong>.", array('@user_ip' => $user_ip));
 	if (isset($range)) {
	 	if (sirius_ip_check($user_ip, $range)) {
	 		$html .= t('Your IP address <strong>IS</strong> allowed.');
	 	} else {
	 		$html .= t('Your IP address <strong>IS NOT</strong> allowed.');
	 	}
	}
 	$html .= '</p>';
 	return $html;
}

/**
 * Sanitizes a list of IP addresses.
 *
 * Helper function that takes a string containing IP addresses on separate
 * lines, Strips them of any code comments, trims them, and turns them into a
 * nice array of sanitized elements. Note that the elements may or may not be IP
 * addresses and if validation is necessary, the array returned from this
 * function should be validated.
 *
 * @param string $raw_ip_addresses
 *   A newline separated list of IP addresses. This array may contain comments
 *   as well as IP addresses.
 *
 * @return array
 *   An array of IP addresses (strings), one per element.
 */

function sirius_ip_rawrange_sanitize($raw_ip_addresses) {
  $ip_addresses = trim($raw_ip_addresses);
  $ip_addresses = preg_replace('/(\/\/|#).+/', '', $ip_addresses);
  $ip_addresses = preg_replace('~/\*([^*]|[\r\n]|(\*+([^*/]|[\r\n])))*\*+/~', '', $ip_addresses);

  $addresses = explode(PHP_EOL, $ip_addresses);

  $return = array();
  foreach ($addresses as $ip_address) {
    $trimmed = trim($ip_address);
    if (strlen($trimmed)) {
      $return[] = $trimmed;
    }
  }

  return $return;
}

function sirius_ip_rawrange_validate($raw_ip_addresses) {
  $ip_addresses = sirius_ip_rawrange_sanitize($raw_ip_addresses);

  if (count($ip_addresses)) {
    foreach ($ip_addresses as $ip_address) {
      $ip_address = trim($ip_address);
      if ($ip_address != '::1') {
        // Check if IP address is a valid singular IP address
        // (ie - not a range).
        if (!preg_match('~^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$~', $ip_address) && !preg_match('~^(([0-9a-fA-F]{1,4}:){7,7}[0-9a-fA-F]{1,4}|([0-9a-fA-F]{1,4}:){1,7}:|([0-9a-fA-F]{1,4}:){1,6}:[0-9a-fA-F]{1,4}|([0-9a-fA-F]{1,4}:){1,5}(:[0-9a-fA-F]{1,4}){1,2}|([0-9a-fA-F]{1,4}:){1,4}(:[0-9a-fA-F]{1,4}){1,3}|([0-9a-fA-F]{1,4}:){1,3}(:[0-9a-fA-F]{1,4}){1,4}|([0-9a-fA-F]{1,4}:){1,2}(:[0-9a-fA-F]{1,4}){1,5}|[0-9a-fA-F]{1,4}:((:[0-9a-fA-F]{1,4}){1,6})|:((:[0-9a-fA-F]{1,4}){1,7}|:)|fe80:(:[0-9a-fA-F]{0,4}){0,4}%[0-9a-zA-Z]{1,}|::(ffff(:0{1,4}){0,1}:){0,1}((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])\.){3,3}(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])|([0-9a-fA-F]{1,4}:){1,4}:((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])\.){3,3}(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9]))$~', $ip_address)) {
          // IP address is not a single IP address, scheck if
          // it's a range of addresses.
          $pieces = explode('-', $ip_address);
          // Only continue checking this IP address if .it is a range of
          // addresses.
          if (count($pieces) == 2) {
            $start_ip = trim($pieces[0]);
            if (!preg_match('~^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$~', $start_ip)) {
              return array('success' => FALSE, 'msg' => t('@ip_address is not a valid IP address.', array('@ip_address' => $start_ip)));
            }
            else {
              $start_pieces = explode('.', $start_ip);
              $start_final_chunk = (int) array_pop($start_pieces);
              $end_ip = trim($pieces[1]);
              $end_valid = TRUE;
              if (preg_match('~^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$~', $end_ip)) {
                $end_valid = TRUE;
                $end_pieces = explode('.', $end_ip);
                for ($i = 0; $i < 3; $i++) {
                  if ((int) $start_pieces[$i] != (int) $end_pieces[$i]) {
                    $end_valid = FALSE;
                  }
                }
                if ($end_valid) {
                  $end_final_chunk = (int) array_pop($end_pieces);
                  if ($start_final_chunk > $end_final_chunk) {
                    $end_valid = FALSE;
                  }
                }
              }
              elseif (!is_numeric($end_ip)) {
                $end_valid = FALSE;
              }
              else {
                if ($end_ip > 255) {
                  $end_valid = FALSE;
                }
                else {
                  $start_final_chunk = array_pop($start_pieces);
                  if ($start_final_chunk > $end_ip) {
                    $end_valid = FALSE;
                  }
                }
              }

              if (!$end_valid) { 
              	return array('success' => FALSE, 'msg' => t('@range is not a valid IP address range.', array('@range' => $ip_address)));
              }
            }
          }
          else { 
          	return array('success' => FALSE, 'msg' => t('!ip_address is not a valid IP address or range of addresses.', array('!ip_address' => $ip_address)));
          }
        }
      }
    }
  }

	return array('success' => TRUE);
}

/**
 * Determines whether or not the a given IP address matches a raw range string.
 */

function sirius_ip_check($user_ip, $raw_ip_addresses) {
	// Get the value saved to the system, and turn it into an array of IP
	// addresses.
	$ip_addresses = sirius_ip_rawrange_sanitize($raw_ip_addresses);

  foreach ($ip_addresses as $ip_address) {
    $ip_address = trim($ip_address);
    if (strlen($ip_address)) {
      // Check if the given IP address matches the current user.
      if ($ip_address == $user_ip) {
        // The given IP is allowed - so don't deny access (aka allow it)
        return TRUE;
      }

      $pieces = explode('-', $ip_address);
      // We only need to continue checking this IP address if it is a
      // range of addresses.
      if (count($pieces) == 2) {
        $start_ip = $pieces[0];
        $end_ip = $pieces[1];
        $start_pieces = explode('.', $start_ip);
        // If there are not 4 sections to the IP then its an invalid
        // IPv4 address, and we don't need to continue checking.
        if (count($start_pieces) === 4) {
          $user_pieces = explode('.', $user_ip);
          $continue = TRUE;
          // We compare the first three chunks of the first IP address
          // With the first three chunks of the user's IP address
          // If they are not the same, then the IP address is not within
          // the range of IPs.
          for ($i = 0; $i < 3; $i++) {
            if ((int) $user_pieces[$i] !== (int) $start_pieces[$i]) {
              // One of the chunks has failed, so we can stop
              // checking this range.
              $continue = FALSE;
              break;
            }
          }
          // The first three chunks have past testing, so now check the
          // range given to see if the final chunk is in this range.
          if ($continue) {
            // First we get the start of the range.
            $start_final_chunk = (int) array_pop($start_pieces);
            $end_pieces = explode('.', $end_ip);
            // Then we get the end of the range. This will work
            // whether the user has entered
            // XXX.XXX.XXX.XXX - XXX.XXX.XXX.XXX or XXX.XXX.XXX.XXX-XXX.
            $end_final_chunk = (int) array_pop($end_pieces);
            // Now we get the user's final chunk.
            $user_final_chunk = (int) array_pop($user_pieces);
            // Finally check to see if the user's chunk lies in that
            // range.
            if ($user_final_chunk >= $start_final_chunk && $user_final_chunk <= $end_final_chunk) {
              // The user's IP lies in the range, so access is not
              // denied (ie - granted).
              return TRUE; 
            }
          }
        }
      }
    }
  }

  return FALSE; 
}
