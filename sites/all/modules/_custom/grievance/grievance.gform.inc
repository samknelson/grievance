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
	$display_mode = variable_get('grievance_gform_display_mode', 'grievance_gform');

	$result .= "<div class=\"grievance_form_wrapper grievance_form_wrapper_$display_mode\">";

	$result .= "<div class=\"grievance_form_header grievance_form_header_$display_mode\">";
	$result .= token_replace(
		variable_get('grievance_gform_header', ''),
		array('node' => $node), 
		array()
	);
	$result .= "</div>";

	$result .= "<div class=\"grievance_form_body grievance_form_body_$display_mode\">";
	$tmp = node_view($node, $display_mode);
	$result .= drupal_render($tmp);
	$result .= "</div>";

	$result .= "<div class=\"grievance_form_footer grievance_form_footer_$display_mode\">";
	$result .= token_replace(
		variable_get('grievance_gform_footer', ''),
		array('node' => $node), 
		array()
	);
	$result .= "</div>";

	$result .= "</div>";

	/*
	if (!$is_loopback) {
		$result .= $pdf_link;
	}
	*/

	return $result;
}

function grievance_node_tab_gform_pdf($node) {
	$basename = grievance_gform_pdf_basename($node);
	// Spool the PDF
	header('Content-Type: application/pdf');
	header("Content-Disposition: attachment; filename=$basename.pdf");

	print grievance_gform_pdf_contents($node);
	drupal_exit();
}

function grievance_gform_pdf_basename($node) {
	$basename = '';

	if ($node->field_grievance_last_name['und'][0]['value']) {
		$tmp = strtolower($node->field_grievance_last_name['und'][0]['value']);
		$tmp = preg_replace('/[^a-z]/', '', $tmp);
		if ($tmp) { $basename .= $tmp . '_'; }
	}
	if ($node->field_grievance_first_name['und'][0]['value']) {
		$tmp = strtolower($node->field_grievance_first_name['und'][0]['value']);
		$tmp = preg_replace('/[^a-z]/', '', $tmp);
		if ($tmp) { $basename .= $tmp . '_'; }
	}
	if ($node->field_grievance_id['und'][0]['value']) {
		$tmp = strtolower($node->field_grievance_id['und'][0]['value']);
		$tmp = preg_replace('/[^a-z0-9]/', '', $tmp);
		if ($tmp) { $basename .= $tmp; }
	}
	if (!$basename) { $basename = $node->nid; }

	return $basename;
}

function grievance_gform_pdf_contents($node) {
	$basename = grievance_gform_pdf_basename($node);

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
	$url .= $_SERVER['SERVER_NAME'] . '/node/' . $node->nid . '/loopback-gform';
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

  $form['grievance_gform_wkhtmltopdfargs'] = array(
		'#default_value' => variable_get('grievance_gform_wkhtmltopdfargs', ''),
    '#title' => t('WKHTMLTOPDF Additional Args'),
    '#description' => t('Additional arguments to pass to wkhtmltopdf.  May contain tokens.'),
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

/**
 * Implements hook_node_view
 *
 * @todo: should be in the .module file, calling a sub-function just for grievance-type nodes.
 */

function  grievance_node_view($node, $view_mode, $langcode) {
	if ($node->type == 'grievance') {

		// Hide all our hide fields
    $hidefields = preg_split('/\s+/', variable_get('grievance_hidefields', ''));
    foreach ($hidefields as $hidefield) {
      if (preg_match('/^field_/', $hidefield) && isset($node->content[$hidefield])) {
        unset($node->content[$hidefield]);
      }
    }

		// Clausref field should show the clause number as well as the clause name.
		if ($node->content['field_grievance_clauseref']['#items']) {
			$delta = 0;
			foreach ($node->content['field_grievance_clauseref']['#items'] as $item) {
				$clause_node = $item['entity'];
				// $article_nid = $clause_node->field_grievance_contract_section['und'][0]['target_id'];
				// $article_node = node_load($article_nid);
				$title = $clause_node->field_grievance_label['und'][0]['value'] . ': ' . $clause_node->title;
				$node->content['field_grievance_clauseref'][$delta]['#markup'] = $title;
				++$delta;
			}
		}

		// "Organizer" should show the phone number as well as the name
		/*
		if ($view_mode == 'grievance_form_226') {
			$organizer_uid = $node->field_grievance_rep_organizer['und'][0]['target_id'];
			if ($organizer_uid) {
				$organizer_user = user_load($organizer_uid);
				if ($organizer_user) {
					$phone = $organizer_user->field_grievance_phone['und'][0]['value'];
					if ($phone) {
						$node->content['field_grievance_rep_organizer'][0]['#markup'] .= '<br>' . $phone;
					}
					$node->content['field_grievance_rep_organizer'][0]['#markup'] .= '<br>' . $organizer_user->mail;
				}
			}
		}
		*/

		// Show the longer information requests.
		if ($node->content['field_grievance_document_types']['#items']) {
			$delta = 0;
			foreach ($node->content['field_grievance_document_types']['#items'] as $item) {
				$term = $item['taxonomy_term'];
				if ($term->field_grievance_emp_name['und'][0]['value']) {
					$name = $term->field_grievance_emp_name['und'][0]['value'];
				} else {
					$name = $term->name;
				}
				$node->content['field_grievance_document_types'][$delta]['#markup'] = $name;
				++$delta;
			}
		}

		if ($node->content['grievance_timeline_short'][0]) {
			$node->content['grievance_timeline_short'][0]['#markup'] = grievance_timeline_short($node);
		}
	}
}

function grievance_timeline_short($grievance_node) {
	// Get a list of log types to show on the timeline
  $log_type_vocabulary = taxonomy_vocabulary_machine_name_load('grievance_log_types');
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'taxonomy_term');
  $query->propertyCondition('vid', $log_type_vocabulary->vid);
 	$query->fieldCondition('field_grievance_timeline_show', $value, 'Yes');
  $stmt = $query->execute();
  $log_type_tids = array();
  if ($stmt['taxonomy_term']) {
  	$log_type_tids = array_keys($stmt['taxonomy_term']);
  }
  $log_type_terms = taxonomy_term_load_multiple($log_type_tids);

	// Get a list of statuses to show on the timeline
  $status_vocabulary = taxonomy_vocabulary_machine_name_load('grievance_status');
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'taxonomy_term');
  $query->propertyCondition('vid', $status_vocabulary->vid);
 	$query->fieldCondition('field_grievance_timeline_show', $value, 'Yes');
  $stmt = $query->execute();
  $status_tids = array();
  if ($stmt['taxonomy_term']) {
  	$status_tids = array_keys($stmt['taxonomy_term']);
  }
  $status_terms = taxonomy_term_load_multiple($status_tids);

	// This will be our list of dates
	$dates = array();

	// Add the intake date
	$dates[] = array(
		'key' => 'Intake', 
		'ts' => (integer) $grievance_node->created,
		'extra' => 'Record created.',
	);

	// Add the last modified date
	$dates[] = array(
		'key' => 'Last Modified', 
		'ts' => (integer) $grievance_node->changed,
		'extra' => 'Record last modified.',
	);

	// Add the incident date and discipline date
	if ($grievance_node->field_grievance_date_1['und'][0]['value']) {
		$dates[] = array(
			'key' => 'Incident',
			'ts' => strtotime($grievance_node->field_grievance_date_1['und'][0]['value']),
			'extra' => '"Incident" date entered in the record.'
		);
	}
	if ($grievance_node->field_grievance_date_2['und'][0]['value']) {
		$dates[] = array(
			'key' => 'Discipline',
			'ts' => strtotime($grievance_node->field_grievance_date_2['und'][0]['value']),
			'extra' => '"Discipline" date entered in the record.'
		);
	}

	// Add any log entries of the right type
	drupal_set_message("Here...");
	if ($log_type_terms) {
		$already_seen = array();
	  $query = new EntityFieldQuery();
	  $query->entityCondition('entity_type', 'node');
	  $query->entityCondition('bundle', 'grievance_log');
	  $query->fieldCondition('field_grievance_log_type', 'tid', $log_type_tids, 'in');
	  $query->fieldCondition('field_grievance', 'target_id', $grievance_node->nid);
	  $query->fieldOrderBy('field_grievance_date', 'value', 'desc');
	  $stmt = $query->execute();
	  if ($stmt['node']) {
	  	$log_nodes = node_load_multiple(array_keys($stmt['node']));
	  	foreach ($log_nodes as $log_nid => $log_node) {
	  		if ($already_seen[$log_node->field_grievance_log_type['und'][0]['tid']]) { continue; }
	  		$already_seen[$log_node->field_grievance_log_type['und'][0]['tid']] = TRUE;
	  		dpm($already_seen);
				$dates[] = array(
					'key' => $log_type_terms[$log_node->field_grievance_log_type['und'][0]['tid']]->name,
					'ts' => strtotime($log_node->field_grievance_status_date['und'][0]['value']),
					'extra' => 'Log entry',
				);
	  	}
	  }
	}
	

	// Add any "log status changed" entries of the right type.
  $log_update_tids = array(
    variable_get('grievance_log_insert_type', ''), 
    variable_get('grievance_log_update_type_with_status_change', ''), 
    variable_get('grievance_log_incident_type', '')
  );
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_log');
  $query->fieldCondition('field_grievance_log_type', 'tid', $log_update_tids, 'in');
  $query->fieldCondition('field_grievance', 'target_id', $grievance_node->nid);
  $stmt = $query->execute();
  if ($stmt['node']) {
  	$log_nodes = node_load_multiple(array_keys($stmt['node']));
  	foreach ($log_nodes as $log_nid => $log_node) {
  		if (!$status_terms[$log_node->field_grievance_status['und'][0]['tid']]) { continue; }
			$dates[] = array(
				'key' => $status_terms[$log_node->field_grievance_status['und'][0]['tid']]->name,
				'ts' => strtotime($log_node->field_grievance_status_date['und'][0]['value']),
				'extra' => 'Status change'
			);
  	}
  }

  // Sort
  usort(
  	$dates, 
  	function($a, $b) { 
  		if ($a['ts'] < $b['ts']) { return -1; }
  		if ($a['ts'] > $b['ts']) { return 1; }
  		return 0;
  	}
  );

  // dedupe
  $dates_clean = array();
  $last_key = $last_datestr = $last_extra = NULL;
  foreach ($dates as $record) {
  	$datestr = date('m/d/Y', $record['ts']);
  	$key = $record['key'];
  	$extra = $record['extra'];

  	if ( ($datestr != $last_datestr) || ($key != $last_key) || ($extra != $last_extra) ) {
  		$dates_clean[] = $record;
  	}

  	$last_datestr = $datestr;
  	$last_key = $key;
  	$last_extra = $extra;
  }

	// Build our output
	$result = '';
	foreach ($dates_clean as $record) {
		$result .= '<section class="field field-name-field-grievance-timeline-short field-label-inline clearfix view-mode-full">';
		$result .= '<h2 class="field-label">';
		$result .= $record['key'];
		$result .= ':&nbsp;</h2>';

		$result .= '<div class="field-items"><div class="field-item even">';
		$result .= date('m/d/Y', $record['ts']);
		if ($record['extra']) {
			// $result .= ' (' . $record['extra'] . ')';
		}
		$result .= '</span></div></div></section>';
	}

	return $result;
}
