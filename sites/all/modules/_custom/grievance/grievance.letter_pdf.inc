<?php

function grievance_letter_loopback($grievance_node) {
	// Become user 1
	global $user;
	$original_user = $user;
	$old_state = drupal_save_session();
	drupal_save_session(FALSE);
	$user = user_load(1);

	// And display the node
	drupal_set_title($node->title . ' - Letters');
	grievance_classes_array('loopback');

	$letter_nids = explode(',', $_GET['letter_nids']);
	return grievance_letter_render_html($grievance_node, $letter_nids, $_GET);
}

function grievance_node_tab_letter_pdf($node, $letter_nids, $values) {
	$basename = grievance_letter_pdf_basename($node, $letter_nids);
	// Spool the PDF
	header('Content-Type: application/pdf');
	header("Content-Disposition: attachment; filename=$basename.pdf");
	print grievance_letter_pdf_contents($node, $letter_nids, $values);
	drupal_exit();
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
	$url .= "\&" . 'delivery_tid=' . urlencode($values['delivery_tid']);
	$html = file_get_contents($url);
	file_put_contents($html_path, $html);

	// Execute wkhtmltopdf
	if ($values['letterhead'] == 'yes') {
		$wkhtmltopdfargs .= token_replace(
			variable_get('grievance_gform_wkhtmltopdfargs', ''),
			array('node' => $node), 
			array()
		);
	}

	$cmd = '/usr/bin/xvfb-run -a ';
	$cmd .= '--server-args="-screen 0 1024x768x24" ';
	$cmd .= '/usr/local/bin/wkhtmltopdf ';
	if ($wkhtmltopdfargs) {
		$cmd .= $wkhtmltopdfargs . ' ';
	}
	$cmd .= '--print-media-type ';
	$cmd .= '-q ';
	$cmd .= $url;
	$cmd .= ' ';
	$cmd .= $pdf_path;
	exec($cmd);

	return file_get_contents($pdf_path);
}
