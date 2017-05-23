<?php

function grievance_node_tab_bundle_download($grievance_node) {
	// Path to the bundled file
	$bundled_file = grievance_bundle_working_directory($grievance_node) . '/bundled_' . $grievance_node->nid . '.pdf';
	if (!file_exists($bundled_file)) { 
		drupal_set_mesage(t('Bundle does not exist.'), 'error');
		return 'Error.';
	}

	// "basename" is the filename we'll give to the downloaded file.  Following the logic for
	// the "letters" funnctionality.
	$basename = '';
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
	$basename .= 'bundle_';
	$basename .= date('Ymd');

	// Stream the bundled PDF.
	header('Content-Type: application/pdf');
	header("Content-Disposition: attachment; filename=$basename.pdf");
	print file_get_contents($bundled_file);
	drupal_exit();
}

function grievance_node_tab_bundle($grievance_node) {
  drupal_set_title($grievance_node->title . ': ' . t('Bundle'));

	return drupal_get_form('grievance_bundle_form', $grievance_node);
}

function grievance_bundle_form($form, &$form_state, $grievance_node) {
	$form_state['grievance_node'] = $grievance_node;
	$grievance_nid = $grievance_node->nid;

	$bundled_file = grievance_bundle_working_directory($source_node) . '/bundled_' . $grievance_node->nid . '.pdf';
	if (file_exists($bundled_file)) {
		$msg = "The most recently generated bundle for this grievance can be downloaded <a href=\"/node/" . $grievance_node->nid . "/bundle/download\">here</a>.";
	} else {
		$msg = "The bundle for this grievance has not yet been generated.";
	}

	drupal_set_message($msg);

	// Which attachments to include?
	$query = new EntityFieldQuery();
	$query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_attachment');
  $query->fieldCondition('field_grievance', 'target_id', $grievance_nid);
	$stmt = $query->execute();
	if ($stmt['node']) {
		$attachment_nodes = node_load_multiple(array_keys($stmt['node']));
		$options = array();
		foreach ($attachment_nodes as $attachment_node) {
			$options[$attachment_node->nid] = $attachment_node->title;
			if ($attachment_node->field_grievance_label['und'][0]['value']) {
				$options[$attachment_node->nid] .= ' (' . $attachment_node->field_grievance_label['und'][0]['value'] . ')';
			}
		}

	  $form['attachment_nids'] = array(
	    '#type' => 'checkboxes',
	    '#title' => t('Attachments to Include'),
	    '#options' => $options,
	  );
	}

	// Which logged attachments to include?
	$query = new EntityFieldQuery();
	$query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_log');
  $query->fieldCondition('field_grievance', 'target_id', $grievance_nid);
  $query->fieldCondition('field_grievance_attachments', 'fid', 0, '>');
	$stmt = $query->execute();
	if ($stmt['node']) {
		$log_nodes = node_load_multiple(array_keys($stmt['node']));
		$options = array();
		foreach ($log_nodes as $log_node) {
			$log_node_type_tid = $log_node->field_grievance_log_type['und'][0]['tid'];
			if (!$log_node_type_tid) { continue; }
			$log_node_type_term = taxonomy_term_load($log_node_type_tid);
			$options[$log_node->nid] = 
				$log_node->title . 
				' / ' . $log_node_type_term->name . 
				' (' . date('m/d/Y', strtotime($log_node->field_grievance_status_date['und'][0]['value'])) . ')';
		}

	  $form['log_nids'] = array(
	    '#type' => 'checkboxes',
	    '#title' => t('Logged Attachments to Include'),
	    '#options' => $options,
	  );
	}

	// What else to include?
  $form['other_include'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Other components to include'),
    '#options' => array(
    	'cover' => t('Cover Page'),
    	'attachments' => t('Attachment List'),
    	'statuslog' => t('Log'),
    	'settlements' => t('Settlements'),
    ),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Create Bundle',
  );

	return $form;
}

function grievance_bundle_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;

	$grievance_node = $form_state['grievance_node'];
	$grievance_nid = $grievance_node->nid;

	// Make a list of the nids and paths we're going to bundle
	$paths_to_bundle = array();

	// Start with the grievance itself.
	$paths_to_bundle[$grievance_nid] = array();

	if ($form_state['values']['other_include']['cover']) {
		$paths_to_bundle[$grievance_nid][] = "node/$grievance_nid";
	}
	if ($form_state['values']['other_include']['attachments']) {
		$paths_to_bundle[$grievance_nid][] = "node/$grievance_nid/attachments";
	}
	if ($form_state['values']['other_include']['statuslog']) {
		$paths_to_bundle[$grievance_nid][] = "node/$grievance_nid/statuslog";
	}

	// Add all the nodes that reference the grievance
	$entity_bundles = array('grievance_attachment', 'grievance_log');
	$entity_bundles[] = 'grievance_log';
	if ($form_state['values']['other_include']['settlements']) {
		$entity_bundles[] = 'grievance_settlement';
	}
	foreach ($entity_bundles as $entity_bundle) {
		$query = new EntityFieldQuery();
		$query->entityCondition('entity_type', 'node');
	  $query->entityCondition('bundle', $entity_bundle);
	  $query->fieldCondition('field_grievance', 'target_id', $grievance_nid);
		$stmt = $query->execute();
		if ($stmt['node']) {
			$tmp = array_keys($stmt['node']);
			foreach ($tmp as $sub_nid) {
				// Only include the checked attachments
				if ($entity_bundle == 'grievance_attachment' && !$form_state['values']['attachment_nids'][$sub_nid]) {
					continue;
				}

				// Only include the checked log entries
				if ($entity_bundle == 'grievance_log' &&  !$form_state['values']['log_nids'][$sub_nid]) {
					continue;
				}

				// Don't bundle the node itself, just the attachments.  (Nodes appear on the list.)
				if ($entity_bundle == 'grievance_attachment' || $entity_bundle == 'grievance_log') {
					$paths_to_bundle[$sub_nid] = array();
				} else {
					$paths_to_bundle[$sub_nid] = array("node/$sub_nid");
				}
			}
		}
	}

	// Init
	$operations = array();
	$operations[] = array(
		'grievance_bundle_init', 
		array($grievance_node),
	);

	// Fetch each path as an HTML page
	foreach ($paths_to_bundle as $target_nid => $target_paths) {
		foreach ($target_paths as $target_path) {
			$operations[] = array('grievance_bundle_batch_generate', array($grievance_node, $target_path));
		}
	}

	// Convert all the HTML paths into PDF documents
	foreach ($paths_to_bundle as $target_nid => $target_paths) {
		foreach ($target_paths as $target_path) {
			$operations[] = array('grievance_bundle_batch_pdf', array($grievance_node, $target_path));
		}
	}

	// Append each PDF document, including attachments, into the large bundle.
	foreach ($paths_to_bundle as $target_nid => $target_paths) {
		foreach ($target_paths as $target_path) {
			$operations[] = array('grievance_bundle_batch_add', array($grievance_node, $target_path));
		}
		$operations[] = array('grievance_bundle_batch_add_attachments', array($grievance_node, $target_nid));
	}

	// Clean up the generated HTML and PDF documents
	foreach ($paths_to_bundle as $target_nid => $target_paths) {
		foreach ($target_paths as $target_path) {
			$operations[] = array('grievance_bundle_batch_cleanup', array($grievance_node, $target_path));
		}
	}

	// Launch the batch processor.
  $batch = array(
    'operations' => $operations,
		'finished' => 'grievance_bundle_finished'
  );
  batch_set($batch);

  drupal_set_message("Bundling for this grievance is complete.");
	return;
}

function grievance_bundle_init($grievance_node, &$context) {
	$context['message'] = t('Using working directory: @working_directory', array('@working_directory' => $working_directory)); 

	$working_directory = grievance_bundle_working_directory($grievance_node);
	if (!file_exists($working_directory)) {
		mkdir($working_directory);
	}

	$old_bundle = grievance_bundle_working_directory($grievance_node) . '/bundled_' . $grievance_node->nid . '.pdf';
	if (file_exists($old_bundle)) {
		unlink($old_bundle);
	}
}

function grievance_bundle_finished($success, $results, $operations) {
	// drupal_set_message("The grievance bundle has been generated.");
}

function grievance_bundle_batch_cleanup($grievance_node, $drupal_path, &$context) {
	$context['message'] = t('Cleaning up: @path', array('@path' => $drupal_path)); 
	unlink(grievance_bundle_output_path($grievance_node, $drupal_path) . '.html');
	unlink(grievance_bundle_output_path($grievance_node, $drupal_path) . '.pdf');
}

function grievance_bundle_batch_add($grievance_node, $drupal_path, &$context) {
	$context['message'] = t('Adding to bundle: @path', array('@path' => $drupal_path)); 
	drupal_set_message($context['message']);

	$bundled_file = grievance_bundle_working_directory($source_node) . '/bundled_' . $grievance_node->nid . '.pdf';
	$tmp_file = grievance_bundle_working_directory($source_node) . '/bundled_' . $grievance_node->nid . '_tmp.pdf';

	if (file_exists($bundled_file)) {
		$cmd = '/usr/bin/pdfunite ';
		$cmd .= $bundled_file . ' ';
		$cmd .= grievance_bundle_output_path($grievance_node, $drupal_path) . '.pdf ';
		$cmd .= $tmp_file . ' ';
	} else {
		$cmd = '/bin/cp ';
		$cmd .= grievance_bundle_output_path($grievance_node, $drupal_path) . '.pdf ';
		$cmd .= $tmp_file . ' ';
	}
	watchdog('grievance_bundle', 'Executing: ' . $cmd);
	exec($cmd);

	unlink($bundled_file);

	$cmd = "mv $tmp_file $bundled_file";
	watchdog('grievance_bundle', 'Executing: ' . $cmd);
	exec($cmd);
}

function grievance_bundle_batch_add_attachments($grievance_node, $target_nid, &$context) {
	$context['message'] = t('Adding attachments: @target_nid', array('@target_nid' => $target_nid)); 
	drupal_set_message($context['message']);

	$target_node = node_load($target_nid);
	if (!$target_node) { return; }
	if (!isset($target_node->field_grievance_attachments['und'][0])) { return; }

	foreach ($target_node->field_grievance_attachments['und'] as $delta => $tmp) {
		$uri = $tmp['uri'];
		$extension = preg_replace('/[^a-z0-9_]/', '', strtolower(substr(strrchr($uri, '.'), 1)));

		if ($extension != 'pdf' && $extension != 'txt' && $extension != 'htm' && $extension != 'html') {
			drupal_set_message("The node <a href=\"/node/$target_nid\" target=\"_blank\">" . $target_node->title . "</a> has a non-PDF attachment which will not be included in the bundle.", 'warning');
			continue;
		}

		$dest = grievance_bundle_output_path($grievance_node, "node/$target_nid") . "_attachment_" . $delta . '.' . $extension;
		drupal_set_message("Here with $delta, $uri");

		file_unmanaged_copy($uri, $dest);

		drupal_set_message("Copied to $dest.");

		if ($extension == 'txt' || $extension == 'htm' || $extension == 'html') {
			$txt_path = $dest;
			$dest = grievance_bundle_output_path($grievance_node, "node/$target_nid") . "_attachment_" . $delta . '.' . 'pdf';

			// Execute wkhtmltopdf
			$cmd = '/usr/bin/xvfb-run -a ';
			$cmd .= '--server-args="-screen 0 1024x768x24" ';
			$cmd .= '/usr/local/bin/wkhtmltopdf ';
			$cmd .= $wkhtmltopdfargs . ' ';
			$cmd .= '--print-media-type ';
			$cmd .= '-q ';
			$cmd .= $txt_path;
			$cmd .= ' ';
			$cmd .= $dest;

			exec($cmd);
			unlink($txt_path);
		}

		if (!file_exists($dest)) {
			drupal_set_message("Why no $dest?");
			continue;
		}

		$bundled_file = grievance_bundle_working_directory($source_node) . '/bundled_' . $grievance_node->nid . '.pdf';
		$tmp_file = grievance_bundle_working_directory($source_node) . '/bundled_' . $grievance_node->nid . '_tmp.pdf';

		$cmd = '/usr/bin/pdfunite ';
		if (file_exists($bundled_file)) {
			$cmd .= $bundled_file . ' ';		
		}
		$cmd .= $dest . ' ';
		$cmd .= $tmp_file . ' ';
		watchdog('grievance_bundle', 'Executing: ' . $cmd);
		exec($cmd);

		unlink($bundled_file);
		// unlink($dest);

		$cmd = "mv $tmp_file $bundled_file";
		watchdog('grievance_bundle', 'Executing: ' . $cmd);
		exec($cmd);
	}
}

function grievance_bundle_batch_generate($grievance_node, $drupal_path, &$context) {
	$context['message'] = t('Generating path: @path', array('@path' => $drupal_path)); 

	$cmd = '/usr/local/bin/drush ';
	$cmd .= '--user=1 ';
	$cmd .= 'php-eval ';
	$cmd .= '"\$_GET[\'q\'] = \'' . $drupal_path . '\'; menu_execute_active_handler(\'' . $drupal_path . '\');" ';
	$cmd .= '> ' . grievance_bundle_output_path($grievance_node, $drupal_path) . '.html';
	watchdog('grievance_bundle', 'Executing: ' . $cmd);
	exec($cmd);
}

function grievance_bundle_batch_pdf($grievance_node, $drupal_path, &$context) {
	$context['message'] = t('Converting to PDF: @path', array('@path' => $drupal_path)); 

	//$cmd = '/usr/bin/xvfb-run -a ';
	//$cmd .= '--server-args="-screen 0 1024x768x24" ';
	$cmd .= '/usr/local/bin/wkhtmltopdf ';
	$cmd .= '--print-media-type ';
	$cmd .= '--disable-javascript ';
	$cmd .= '-q ';
	$cmd .= grievance_bundle_output_path($grievance_node, $drupal_path) . '.html ';
	$cmd .= ' ';
	$cmd .= grievance_bundle_output_path($grievance_node, $drupal_path) . '.pdf ';
	watchdog('grievance_bundle', 'Executing: ' . $cmd);
	exec($cmd);
}

function grievance_bundle_working_directory($source_node) {
	$path = DRUPAL_ROOT;
	$path .= '/' . variable_get('grievance_bundle_base_path', 'sites/default/files/private/bundle');
	return $path;
}

function grievance_bundle_output_path($source_node, $drupal_path) {
	$output_path = preg_replace('/[^a-zA-Z0-9_]/', '_', $drupal_path);
	return grievance_bundle_working_directory($source_node) . '/' . $source_node->type . '_' . $source_node->nid . '_' . $output_path;
}

