<?php

function grievance_letter_loopback($grievance_node) {
	// Become user 1
	global $user;
	$original_user = $user;
	$old_state = drupal_save_session();
	drupal_save_session(FALSE);
	$user = user_load(1);

	// And display the node
	drupal_set_title($node->title . ' - Letter');
	grievance_classes_array('loopback');

	$letter_nid = $_GET['letter_nid'];
	$letter_node = node_load($letter_nid);
	return grievance_letter_render_html($grievance_node, $letter_node, $_GET);
}

function grievance_node_tab_letter($grievance_node, $is_loopback = FALSE) {
  drupal_set_title($grievance_node->title . ': ' . t('Letter'));
  return drupal_get_form('grievance_letter_form', $grievance_node);
}

function grievance_node_tab_letter_pdf($node, $letter_nid) {
	$basename = grievance_letter_pdf_basename($node, $letter_nid);
	// Spool the PDF
	header('Content-Type: application/pdf');
	header("Content-Disposition: attachment; filename=$basename.pdf");
	print grievance_letter_pdf_contents($node, $letter_nid);
	drupal_exit();
}

function grievance_letter_pdf_basename($grievance_node, $letter_nid) {
	$basename = '';

	$letter_node = node_load($letter_nid);

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


function grievance_letter_pdf_contents($node, $letter_nid) {
	$basename = grievance_letter_pdf_basename($node, $letter_nid);

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
	$url .= $_SERVER['SERVER_NAME'] . '/node/' . $node->nid . '/loopback-letter?letter_nid=' . $letter_nid;
	$html = file_get_contents($url);
	file_put_contents($html_path, $html);

	// Execute wkhtmltopdf
	$wkhtmltopdfargs .= token_replace(
		variable_get('grievance_gform_wkhtmltopdfargs', ''),
		array('node' => $node), 
		array()
	);

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



function grievance_form_grievance_letter_template_node_form_alter(&$form, &$form_state, $form_id) {
  $weight = $form['body']['#weight'] + 0.5;

  $form['token_tree'] = array(
    '#type' => 'fieldset',
    '#title' => t('Replacement patterns'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => theme('token_tree', array('token_types' => array('node'))),
    '#weight' => $weight
  );
}

function grievance_letter_form($form, &$form_state, $grievance_node) {
	$form_state['grievance_node'] = $grievance_node;

  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_letter_template');
  $query->propertyOrderBy('title');
  $stmt = $query->execute();
  $options = array(NULL => t('-- select --'));
  if ($stmt['node']) {
  	$letter_nodes = node_load_multiple(array_keys($stmt['node']));
  	foreach ($letter_nodes as $letter_nid => $letter_node) {
  		$options[$letter_nid] = $letter_node->title;
  	}
  }

  $form['letter_nid'] = array(
    '#type' => 'select',
    '#options' => $options,
    '#title' => t('Select Letter'),
    '#required' => TRUE,
  );

   $form['delivery'] = array(
    '#type' => 'select',
    '#options' => array(
    	'By Fax' => t('By Fax'),
    	'By Mail' => t('By Mail'),
    	'CERTIFIED MAIL, RETURN RECEIPT REQUESTED' => t('By Certified Mail'),
    ),
    '#title' => t('Delivery'),
  );


  $form['output'] = array(
    '#type' => 'select',
    '#options' => array(
    	'preview' => t('Screen Preview'),
    	'pdf' => t('Download PDF'),
    ),
    '#title' => t('Output'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Go',
  );

  if ($form_state['result']) {
  	$markup = '<h2>' . t('Preview') . '</h2>';
  	$markup .= '<hr>';
  	$markup .= $form_state['result'];
  	$markup .= '<hr>';
  	$form['markup'] = array(
  		'#type' => 'markup',
  		'#markup' => $markup,
  	);
  }
	
	return $form;
}

function grievance_letter_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
	$form_state['result'] = NULL;
	$grievance_node = $form_state['grievance_node'];

	// Get the letter nid
	$letter_nid = $form_state['values']['letter_nid'];
	if (!$letter_nid) {
		drupal_set_message(t('No letter selected.'), 'error');
		return;
	}
	$letter_node = node_load($letter_nid);
	if (!$letter_node) {
		drupal_set_message(t('Letter could not be loaded.'), 'error');
		return;
	}

	if ($form_state['values']['output'] == 'pdf') {
		return grievance_node_tab_letter_pdf($grievance_node, $letter_nid);
	}

	$form_state['result'] = grievance_letter_render_html(
		$grievance_node, 
		$letter_node, 
		array(
			'delivery' => $form_state['values']['delivery'],
		)
	);
}

function grievance_letter_render_html($grievance_node, $letter_node, $other_tokens) {
	$body = $letter_node->body['und'][0]['value'];
	$body = token_replace(
		$body, array(
			'node' => $grievance_node,
			'grievance-letter' => $other_tokens,
		)
	);

	$body = '<div id="grievance-letter-body">' . $body . '</div>';

	return $body;
}

/**
 * Implements hook_tokens
 */

function grievance_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();

  // Replacement values for tokens that require additional contextual data.
  if ($type == 'grievance-letter') {
    $record = $data['grievance-letter'];
    foreach ($tokens as $name => $original) {
    	if ($name == 'delivery') {
				$replacements[$original] = $record['delivery'];
			}
			if (preg_match('/^render:/', $name)) {
				list ($ignore, $display_mode) = explode(':', $name);
				$renderable = node_view($data['node'], $display_mode);
				$replacements[$original] = drupal_render($renderable);
			}
    }
  }

  return $replacements;
}