<?php

function grievance_arbitrationform_loopback($node) {
	// Become user 1
	global $user;
	$original_user = $user;
	$old_state = drupal_save_session();
	drupal_save_session(FALSE);
	$user = user_load(1);

	// And display the node
	drupal_set_title($node->title . ' - Grievance Form');
	grievance_classes_array('loopback');
	return grievance_node_tab_arbitrationform($node, TRUE);
}

function grievance_node_tab_arbitrationform($node, $is_loopback = FALSE) {
	if ($_GET['pdf']) {
		return grievance_node_tab_arbitrationform_pdf($node);
	}

	$pdf_link = '<div class="pdf_link">' . '[' . l(t('Download this form as a PDF'), current_path(), array('query' => array('pdf' => 1))) . ']' . '</div>';

	$result = '';
	if (!$is_loopback) {
		$result .= $pdf_link;
	}
	$result .= variable_get('grievance_arbitrationform_header', '');
	$tmp = node_view($node, 'grievance_arbitrationform');
	$result .= drupal_render($tmp);
	$result .= variable_get('grievance_arbitrationform_footer', '');

	if (!$is_loopback) {
		$result .= $pdf_link;
	}

	return $result;
}

function grievance_node_tab_arbitrationform_pdf($node) {
	// Loopback to generate the HTML.
	$html_path = DRUPAL_ROOT . '/sites/default/files/private/grievance_arbitrationform/node_' . $node->nid . '.html';
	$url = 'http://' . $_SERVER['SERVER_NAME'] . '/node/' . $node->nid . '/loopback-arbitrationform';
	$html = file_get_contents($url);
	file_put_contents($html_path, $html);

	// Execute wkhtmltopdf
	$pdf_path = DRUPAL_ROOT . '/sites/default/files/private/grievance_arbitrationform/node_' . $node->nid . '.pdf';
	$cmd = '/usr/bin/xvfb-run -a ';
	$cmd .= '--server-args="-screen 0 1024x768x24" ';
	$cmd .= '/usr/local/bin/wkhtmltopdf ';
	$cmd .= '-q ';
	$cmd .= $url;
	$cmd .= ' ';
	$cmd .= $pdf_path;
	exec($cmd);

	// Spool the PDF
	header('Content-Type: text/csv');
	header("Content-Disposition: attachment; filename=grievance_" . $node->nid . ".pdf");

	$pdf = file_get_contents($pdf_path);
	print $pdf;
	drupal_exit();
}


function grievance_arbitrationform_admin() {
  $form = array();

  $form['grievance_arbitrationform_header'] = array(
		'#default_value' => variable_get('grievance_arbitrationform_header', ''),
    '#title' => t('Grievance Form Header'),
    '#description' => t('This will appear at the top of the grievance form.  May contain HTML.'),
    '#type' => 'textarea',
  );

  $form['grievance_arbitrationform_footer'] = array(
		'#default_value' => variable_get('grievance_arbitrationform_footer', ''),
    '#title' => t('Grievance Form Footer'),
    '#description' => t('This will appear at the bottom of the grievance form.  May contain HTML.'),
    '#type' => 'textarea',
  );

  return system_settings_form($form);
}