<?php

function grievance_letter_loopback($grievance_node) {
	// Become user 1
	sirius_become_superuser(TRUE);

	// And display the node
	drupal_set_title($node->title . ' - Letters');
	grievance_classes_array('loopback');

	$letter_nids = explode(',', $_GET['letter_nids']);

	$render_uid = $_REQUEST['uid'];
	return grievance_letter_render_html($grievance_node, $letter_nids, array_merge($_GET, $_POST));
}

function grievance_letter_pdf_basename($grievance_node, $letter_nids) {
	$basename = '';

	$letter_node = node_load($letter_nids[0]);

	if ($grievance_node->field_grievance_last_name['und'][0]['value']) {
		$tmp = strtolower($grievance_node->field_grievance_last_name['und'][0]['value']);
		$tmp = preg_replace('/[^a-z]/', '', $tmp);
		if ($tmp) { $basename .= $tmp . '_'; }
	}
	if ($grievance_node->field_grievance_first_name['und'][0]['value']) {
		$tmp = strtolower($grievance_node->field_grievance_first_name['und'][0]['value']);
		$tmp = preg_replace('/[^a-z]/', '', $tmp);
		if ($tmp) { $basename .= $tmp . '_'; }
	}
	if ($grievance_node->field_grievance_id['und'][0]['value']) {
		$tmp = strtolower($grievance_node->field_grievance_id['und'][0]['value']);
		$tmp = preg_replace('/[^a-z0-9]/', '', $tmp);
		if ($tmp) { $basename .= $tmp . '_'; }
	}
	if ($letter_node->field_grievance_shortname['und'][0]['value']) {
		$tmp = strtolower($letter_node->field_grievance_shortname['und'][0]['value']);
		$tmp = preg_replace('/[^a-z0-9]/', '', $tmp);
		if ($tmp) { $basename .= $tmp . '_'; }
	}
	$basename .= date('Ymd');
	if (!$basename) { $basename = $grievance_node->nid; }

	return $basename;
}

function grievance_letter_pdf_contents($node, $letter_nids, $values) {
	$basename = grievance_letter_pdf_basename($node, $letter_nids);

	// Loopback to generate the HTML.
	$html_path = DRUPAL_ROOT . '/sites/default/files/private/grievance_gform/' . $basename . '.html';
	$pdf_path = DRUPAL_ROOT . '/sites/default/files/private/grievance_gform/' . $basename . '.pdf';

	global $is_https;
	if ($is_https ||
		(isset($_SERVER['HTTP_X_FORWARDED_PROTO']) && $_SERVER['HTTP_X_FORWARDED_PROTO'] == 'https')) {
		$url = 'https://';
	} else { 
		$url = 'http://';
	}
	$url .= $_SERVER['SERVER_NAME'] . '/node/' . $node->nid . '/loopback-letter';
	$url .= '?letter_nids=' . join(',', $letter_nids);
	$url .= "&" . 'delivery_tid=' . urlencode($values['delivery_tid']);
	global $user;
	if ($user) { $url .= '&' . 'uid=' . $user->uid; }

	// $html = file_get_contents($url);

	$postfields = array();
	if ($values['letter_body']) {
		$postfields['letter_body'] = $values['letter_body']['value'];
	}

  $ch = curl_init();
  curl_setopt($ch, CURLOPT_URL, $url);
  curl_setopt($ch, CURLOPT_HEADER, 0);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
  if ($postfields) {
		curl_setopt($ch, CURLOPT_POST, 1);
		curl_setopt($ch, CURLOPT_POSTFIELDS, $postfields);
	}
  $html = curl_exec($ch);
  curl_close($ch);

	file_put_contents($html_path, $html);

	// Execute wkhtmltopdf
	if ($values['letterhead'] == 'yes') {
		$wkhtmltopdfargs .= token_replace(
			variable_get('grievance_gform_wkhtmltopdfargs', ''),
			array('node' => $node), 
			array()
		);
	} else if ($values['letterhead'] > 0) {
		$letterhead_node = node_load($values['letterhead']); 
		$json = json_decode($letterhead_node->field_sirius_json['und'][0]['value'], TRUE);
		$margin_args = '';
		foreach (array('left', 'right', 'top', 'bottom') as $which) {
			if ($json['page']["margin_$which"]) {
				$arg = escapeshellarg($json['page']["margin_$which"]);
			} else {
				$arg = "'15mm'";
			}
			$margin_args .= "--margin-$which $arg ";
		}

		$letterhead_pdf_uri = 'temporary://pdfgen_letterhead_' . $letterhead_node->nid . '.pdf';
		$letterhead_pdf_path = drupal_realpath($letterhead_pdf_uri);
		$letterhead_source = (object) $letterhead_node->field_sirius_attachments['und'][0];
		file_copy($letterhead_source, $letterhead_pdf_uri, FILE_EXISTS_REPLACE);
	}

	$cmd = '/usr/bin/xvfb-run -a ';
	$cmd .= '--server-args="-screen 0 1024x768x24" ';
	$cmd .= '/usr/local/bin/wkhtmltopdf ';
	if ($wkhtmltopdfargs) {
		$cmd .= $wkhtmltopdfargs . ' ';
	}
	$cmd .= $margin_args;
	$cmd .= '-s letter ';
	$cmd .= '--print-media-type ';
	$cmd .= '-q ';
	$cmd .= $html_path;
	$cmd .= ' ';
	$cmd .= $pdf_path;

	exec($cmd);

	if ($letterhead_node) {
		// Combine the letterhead with the source
		$cmd = "/usr/bin/pdftk $pdf_path multibackground $letterhead_pdf_path output $pdf_path.combined";
		exec($cmd);
		rename("$pdf_path.combined", "$pdf_path");
	}

	return file_get_contents($pdf_path);
}
