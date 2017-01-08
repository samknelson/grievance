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

function grievance_node_tab_letter($grievance_node, $is_loopback = FALSE) {
  drupal_set_title($grievance_node->title . ': ' . t('Letter'));
  return drupal_get_form('grievance_letter_form', $grievance_node);
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

function grievance_form_grievance_letter_template_node_form_alter(&$form, &$form_state, $form_id) {
	// Getting memory errors, I hate token tree.
	/*
  $weight = $form['body']['#weight'] + 0.5;

  $form['token_tree'] = array(
    '#type' => 'fieldset',
    '#title' => t('Replacement patterns'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => theme('token_tree', array('token_types' => array('node'))),
    '#weight' => $weight
  );
  */
}

// @todo pass in $account, instead of assuming current user
function grievance_letter_permission_check($letter_node) {
	global $user;

	// Admin can send any letter
	if (user_access('grievance admin')) { return TRUE; }

	// No permissions set, return false
	if (!isset($letter_node->field_grievance_roles['und'])) { return FALSE; }

	// Does the user have nay of the specified roles?
	foreach ($letter_node->field_grievance_roles['und'] as $tmp) {
		if (user_has_role($tmp['value'], $account)) {
			return TRUE;
		}
	}

	// Apparently not.
	return FALSE;
}

function grievance_letter_form($form, &$form_state, $grievance_node) {
	$form_state['grievance_node'] = $grievance_node;

  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_letter_template');
  $query->propertyOrderBy('title');
  $stmt = $query->execute();
  $options = array();
  $footnote = '';
  if ($stmt['node']) {
  	$letter_nodes = node_load_multiple(array_keys($stmt['node']));
  	foreach ($letter_nodes as $letter_nid => $letter_node) {
  		// Check permissions on the letter
			if (grievance_letter_permission_check($letter_node)) {
	  		$options[$letter_nid] = $letter_node->title;
	  		if ($letter_node->field_grievance_attachments['und'][0]['uri']) {
	  			$options[$letter_nid] .= '*';
	  			$footnote = t('* These letters must be printed individually, they cannot be bundled with other letters.');
	  		}
	  	}
  	}
  }

  $form['letter_nid'] = array(
    '#type' => 'checkboxes',
    '#options' => $options,
    '#title' => t('Select Letter(s)'),
    '#required' => TRUE,
    '#description' => $footnote,
  );

	$delivery_vocabulary = taxonomy_vocabulary_machine_name_load('grievance_delivery');
	$delivery_terms = taxonomy_get_tree($delivery_vocabulary->vid);
	$delivery_options = array(NULL => t('-- select --'));
	foreach ($delivery_terms as $term) {
		$delivery_options[$term->tid] = str_repeat('-', $term->depth) . $term->name;
	}
   $form['delivery_tid'] = array(
    '#type' => 'select',
    '#options' => $delivery_options,
    '#title' => t('Delivery'),
  );

  $form['letterhead'] = array(
    '#type' => 'select',
    '#options' => array(
    	'no' => t('No Letterhead'),
    	'yes' => t('Standard Letterhead'),
    ),
    '#title' => t('Letterhead'),
  );

  $form['output'] = array(
    '#type' => 'select',
    '#options' => array(
    	'pdf' => t('Download PDF'),
    	'preview' => t('Screen Preview'),
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
	$letter_nids = array();
	foreach ($form_state['values']['letter_nid'] as $ignore => $letter_nid) {
		if ($letter_nid) {
			$letter_nids[] = $letter_nid;
		}
	}
	if (!$letter_nids) {
		drupal_set_message(t('No letter selected.'), 'error');
		return;
	}

	$letter_nodes = node_load_multiple($letter_nids);

	if ($form_state['values']['output'] == 'pdf') {
		// Sometimes a "letter" is just an attachment.  If so, we just redirect there.
		foreach ($letter_nodes as $letter_nid => $letter_node) {
			if ($letter_node->field_grievance_attachments['und'][0]['uri']) {
				$url = file_create_url($letter_node->field_grievance_attachments['und'][0]['uri']);
				if ($url) {
					drupal_goto($url);
				}
			}
		}
		return grievance_node_tab_letter_pdf($grievance_node, $letter_nids, $form_state['values']);
	}

	$form_state['result'] = grievance_letter_render_html(
		$grievance_node, 
		$letter_nids, 
		array(
			'delivery_tid' => $form_state['values']['delivery_tid'],
		)
	);
}

function grievance_letter_render_html($grievance_node, $letter_nids, $other_tokens) {
	$letter_nodes = node_load_multiple($letter_nids);
	$body = '';
	foreach ($letter_nodes as $letter_nid => $letter_node) {
		$letter_part = $letter_node->body['und'][0]['value'];
		$letter_part = token_replace(
			$letter_part, 
			array(
				'node' => $grievance_node,
				'grievance-letter' => $other_tokens,
			)
		);

		if ($body) {
			$body .= '<div class="grievance-letter-page-break">&nbsp;</div>';
		}
		$body .= '<div class="grievance-letter-part">' . $letter_part . '</div>';

	}

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
    	// Add "Delivery" as a siml=ple replacement token
    	if ($name == 'delivery') {
    		$delivery_tid = $record['delivery_tid'];
    		$delivery_term = taxonomy_term_load($delivery_tid);
    		$replacement = $delivery_term->description;
    		if (!$replacement) {
    			$replacement = $delivery_term->name;
    		}
				$replacements[$original] = $replacement;
			}

			// Add "render:display_mode" to render the grievance with the specified display mode.
			if (preg_match('/^render:/', $name)) {
				list ($ignore, $display_mode) = explode(':', $name);
				$renderable = node_view($data['node'], $display_mode);
				$replacements[$original] = drupal_render($renderable);
			}

    }
  }

  return $replacements;
}


