<?php

function grievance_forward_loopback($node) {
	// Become user 1
	global $user;
	$original_user = $user;
	$old_state = drupal_save_session();
	drupal_save_session(FALSE);
	$user = user_load(1);

	// And display the node
	drupal_set_title($node->title);
	return node_view($node);
}

function grievance_forward_loopback_access($node) {
	if ($_SERVER['REMOTE_ADDR'] == $_SERVER['SERVER_ADDR']) { return TRUE; }
	return FALSE;
}

function grievance_custom_theme() {
	if (preg_match('/^node\/\d+\/loopback$/', current_path())) {
		return 'at_tf_admin';
	}
}

function grievance_node_tab_forward($node) {
	return drupal_get_form('grievance_forward_form', $node);
}

function grievance_forward_form($form, &$form_state, $grievance_node) {
	$form_state['grievance_node'] = $grievance_node;

  $form['email'] = array(
    '#title' => t('To'),
    '#type' => 'textfield',
    '#required' => 1,
	);

  $form['subject'] = array(
    '#title' => t('Subject'),
    '#type' => 'textfield',
    '#required' => 1,
   );

  $form['body'] = array(
    '#title' => t('Body'),
    '#type' => 'textarea',
		'#rows' => 20,
    '#required' => 1,
	);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Forward This Grievance',
  );
	
	return $form;
}


function grievance_forward_form_submit($form, &$form_state) {
	$grievance_node = $form_state['grievance_node'];
	$form_state['rebuild'] = 1;

	// Fetch the grievance as HTML
	$html_path = DRUPAL_ROOT . '/sites/default/files/private/grievance_forward/node_' . $grievance_node->nid . '.html';
	$url = 'http://' . $_SERVER['SERVER_NAME'] . '/node/' . $grievance_node->nid . '/loopback';
	$html = file_get_contents($url);
	file_put_contents($html_path, $html);

	// Execute wkhtmltopdf
	$pdf_path = DRUPAL_ROOT . '/sites/default/files/private/grievance_forward/node_' . $grievance_node->nid . '.pdf';
	$cmd = '/usr/bin/xvfb-run -a ';
	$cmd .= '--server-args="-screen 0 1024x768x24" ';
	$cmd .= '/usr/local/bin/wkhtmltopdf ';
	$cmd .= '-q ';
	$cmd .= $url;
	$cmd .= ' ';
	$cmd .= $pdf_path;
	exec($cmd);

	$params = array();
	$params['body'] = $form_state['values']['body'];
	$params['subject'] = $form_state['values']['subject'];
	$params['attachments'] = array();

	// Attach the grievance as a PDF
	$params['attachments'][] = array(
		'filecontent' => file_get_contents($pdf_path),
		'filename' => 'grievance_' . $grievance_node->nid . '.pdf',
		'filemime' => 'grievance/pdf',
	);

	// Attach any other attachments
	foreach (get_object_vars($grievance_node) as $key => $ignore) {
		if (!preg_match('/^field_/', $key)) { continue; }
		if (!isset($grievance_node->{$key}['und'][0]['fid'])) { continue; }

		$uri = $grievance_node->{$key}['und'][0]['uri'];
		$path = drupal_realpath($uri);

		$params['attachments'][] = array(
			'filecontent' => file_get_contents($path),
			'filename' => basename($path),
			'filemime' => $grievance_node->{$key}['und'][0]['filemime'],
		);
	}

	$email = $form_state['values']['email'];

	global $user;

	drupal_mail(
		'grievance', 
		'grievance', 
		$email, 
		LANGUAGE_NONE, 
		$params
	);

	drupal_set_message(t('The grievance has been forwarded to: ') . $email);

}