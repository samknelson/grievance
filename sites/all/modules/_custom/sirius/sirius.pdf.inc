<?php

function sirius_pdf_admin($form, &$form_state) {
  $name = sirius_domain_variable_name('sirius_pdf_margin_left');
	$form[$name] = array(
		'#type' => 'textfield',
		'#title' => 'Left Margin',
		'#default_value' => sirius_domain_variable_get($name, '15mm'),
	);

  $name = sirius_domain_variable_name('sirius_pdf_margin_right');
	$form[$name] = array(
		'#type' => 'textfield',
		'#title' => 'Right Margin',
		'#default_value' => sirius_domain_variable_get($name, '15mm'),
	);

  $name = sirius_domain_variable_name('sirius_pdf_margin_top');
	$form[$name] = array(
		'#type' => 'textfield',
		'#title' => 'Top Margin',
		'#default_value' => sirius_domain_variable_get($name, '15mm'),
	);

  $name = sirius_domain_variable_name('sirius_pdf_margin_bottom');
	$form[$name] = array(
		'#type' => 'textfield',
		'#title' => 'Bottom Margin',
		'#default_value' => sirius_domain_variable_get($name, '15mm'),
	);

  return system_settings_form($form);
}

function sirius_pdf_loopback($html, $letterhead_node = NULL) {
	sirius_become_superuser(TRUE);

	// And display the node
	grievance_classes_array('loopback');

	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/pdf.css');
	drupal_add_js(drupal_get_path('module', 'sirius') . '/js/pdf.js');

	if ($letterhead_node) {
		$json = json_decode($letterhead_node->field_sirius_json['und'][0]['value'], TRUE);
		$css .= $json['css']['body_first'];
		drupal_add_css($css, 'inline');
	}

	$html = '<div id="sirius-pdf-wrapper">' . $html . '</div>';

	return $html;
}

function sirius_pdf_stream($url, $params) {
	$pdf_contents = sirius_pdf_generate($url, $params);
	
	// Dump to browser and exit
	header('Content-Type: application/pdf');
	if ($params['filename']) { header("Content-Disposition: attachment; filename=$params[filename].pdf"); }
	print $pdf_contents;
	drupal_exit();
}

function sirius_pdf_generate($url, $params) {
	watchdog('sirius_letters', "Called with $url, <pre>" . print_r($params, 1) . '</pre>');
	$letterhead = $params['letterhead'];
	if ($letterhead > 0) {
		$letterhead_node = node_load($letterhead); 
	}
	if ($letterhead_node) {
		$json = json_decode($letterhead_node->field_sirius_json['und'][0]['value'], TRUE);
	}

	$margin_args = '';
	foreach (array('left', 'right', 'top', 'bottom') as $which) {
		if ($json['page']["margin_$which"]) {
			$arg = escapeshellarg($json['page']["margin_$which"]);
		} else {
			$arg = escapeshellarg(sirius_domain_variable_get("sirius_pdf_margin_$which", '15mm'));
		}
		$margin_args .= "--margin-$which $arg ";
	}

	// Loopback to generate the HTML.
	$key = date('YmdHis') . rand(100000, 999999);
	$input_html_uri = 'temporary://pdfgen_input_' . $key . '.html';
	$input_html_path = drupal_realpath($input_html_uri);

	$input_pdf_uri = 'temporary://pdfgen_input_' . $key . '.pdf';
	$input_pdf_path = drupal_realpath($input_pdf_uri);

	$letterhead_pdf_uri = 'temporary://pdfgen_letterhead_' . $key . '.pdf';
	$letterhead_pdf_path = drupal_realpath($letterhead_pdf_uri);

	$output_pdf_uri = 'temporary://pdfgen_output_' . $key . '.pdf';
	$output_pdf_path = drupal_realpath($output_pdf_uri);

	// Post to that URL, fetching our HTML
	watchdog('sirius_letters', "Curl: $url");
  $ch = curl_init();
  curl_setopt($ch, CURLOPT_URL, $url);
  curl_setopt($ch, CURLOPT_HEADER, 0);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
	curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 0);
	curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0);
  if ($params['post']) {
		curl_setopt($ch, CURLOPT_POST, 1);
		curl_setopt($ch, CURLOPT_POSTFIELDS, $params['post']);
	}
  $html = curl_exec($ch);
  if ($html === FALSE) {
  	$error = curl_error($ch);
  	watchdog('sirius_letters', "Curl error: $error");
  	drupal_set_message($errror, 'warning');
  }
  curl_close($ch);
	file_put_contents($input_html_path, $html);
	watchdog('sirius_letters', "Fetched $url to $input_html_path ($input_html_path)");

	// Convert the HTML to a PDF.
	$cmd = sirius_server_paths('xvfb-run') . ' -a ';
	$cmd .= '--server-args="-screen 0 1024x768x24" ';
	$cmd .= sirius_server_paths('wkhtmltopdf') . ' ';
	$cmd .= $margin_args;
	$cmd .= '-s letter ';
	$cmd .= '--print-media-type --disable-smart-shrinking ';
	$cmd .= '-q ';
	$cmd .= $input_html_path;
	$cmd .= ' ';
	$cmd .= $input_pdf_path;
	exec($cmd);
	watchdog('sirius_letters', "Executed $cmd");

	// The PDF might live in S3, so we do a stupid file copy to a temporary directory
	if ($letterhead_node) {
		$letterhead_source = (object) $letterhead_node->field_sirius_attachments['und'][0];
		$letterhead_path = 'temporary://sirius_letterhead_' . $letterhead_source->filename;
		file_copy($letterhead_source, $letterhead_pdf_uri, FILE_EXISTS_REPLACE);

		// Combine the letterhead with the source
		$cmd = sirius_server_paths('pdftk') . " $input_pdf_path multibackground $letterhead_pdf_path output $output_pdf_path";
		exec($cmd);
		watchdog('sirius_letters', "Executed $cmd");
	} else {
		rename($input_pdf_path, $output_pdf_path);
		watchdog('sirius_letters', "Renamed $input_pdf_path to $output_pdf_path");;
	}

	watchdog('sirius_letters', "Output to $output_pdf_path");;
	return file_get_contents($output_pdf_path);
}
