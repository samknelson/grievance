<?php

function grievance_gform_loopback($node) {
	// Become user 1
	global $user;
	$original_user = $user;
	$old_state = drupal_save_session();
	drupal_save_session(FALSE);
	$user = user_load(1);

	// And display the node
	drupal_set_title($node->title . ' - Grievance Form');
	grievance_classes_array('loopback');
	return grievance_node_tab_gform($node, TRUE);
}

function grievance_node_tab_gform($node, $is_loopback = FALSE) {
	if ($_GET['pdf']) {
		return grievance_node_tab_gform_pdf($node);
	}

	$pdf_link = '<div class="pdf_link">' . '[' . l(t('Download this form as a PDF'), current_path(), array('query' => array('pdf' => 1))) . ']' . '</div>';

	$result = '';
	if (!$is_loopback) {
		$result .= $pdf_link;
	}
	$result .= variable_get('grievance_gform_header', '');
	$display_mode = variable_get('grievance_gform_display_mode', 'grievance_gform');
	$tmp = node_view($node, $display_mode);
	$result .= drupal_render($tmp);
	$result .= variable_get('grievance_gform_footer', '');

	if (!$is_loopback) {
		$result .= $pdf_link;
	}

	return $result;
}

function grievance_node_tab_gform_pdf($node) {
	print grievance_gform_pdf_contents($node);
	drupal_exit();
}

function grievance_gform_pdf_contents($node) {
	// Loopback to generate the HTML.
	$html_path = DRUPAL_ROOT . '/sites/default/files/private/grievance_gform/node_' . $node->nid . '.html';
	$url = 'http://' . $_SERVER['SERVER_NAME'] . '/node/' . $node->nid . '/loopback-gform';
	$html = file_get_contents($url);
	file_put_contents($html_path, $html);

	// Execute wkhtmltopdf
	$pdf_path = DRUPAL_ROOT . '/sites/default/files/private/grievance_gform/node_' . $node->nid . '.pdf';
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

	return file_get_contents($pdf_path);
}


function grievance_gform_admin() {
  $form = array();

  $form['grievance_gform_header'] = array(
		'#default_value' => variable_get('grievance_gform_header', ''),
    '#title' => t('Grievance Form Header'),
    '#description' => t('This will appear at the top of the grievance form.  May contain HTML.'),
    '#type' => 'textarea',
  );

  $form['grievance_gform_footer'] = array(
		'#default_value' => variable_get('grievance_gform_footer', ''),
    '#title' => t('Grievance Form Footer'),
    '#description' => t('This will appear at the bottom of the grievance form.  May contain HTML.'),
    '#type' => 'textarea',
  );

  $form['grievance_gform_display_mode'] = array(
		'#default_value' => variable_get('grievance_gform_display_mode', 'grievance_gform'),
    '#title' => t('Grievance Form Display Mode'),
    '#description' => t('Machine name of the display suite mode to use when rendering the grievance form.'),
    '#type' => 'textfield',
  );

  return system_settings_form($form);
}