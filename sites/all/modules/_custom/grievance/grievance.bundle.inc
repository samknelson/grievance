<?php

/**
 * @todo: bundle functionality is incomplete at this time.
 */

function grievance_node_tab_bundle($grievance_node) {
  drupal_set_title($grievance_node->title . ': ' . t('Bundle'));

	return drupal_get_form('grievance_bundle_form', $grievance_node);
}

function grievance_bundle_form($form, &$form_state, $grievance_node) {
	$form_state['grievance_node'] = $grievance_node;

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

	$working_directory = grievance_bundle_working_directory($grievance_node);
	$output_path = grievance_bundle_path($grievance_node);
	drupal_set_message($working_directory);

	// Init
	$operations = array();
	$operations[] = array(
		'grievance_bundle_init', 
		array($grievance_node),
	);

	// Add paths
	$operations[] = array('grievance_bundle_output_path', array($grievance_node, "node/$grievance_nid", "view"));
	$operations[] = array('grievance_bundle_output_path', array($grievance_node, "node/$grievance_nid/gform", "form"));
	$operations[] = array('grievance_bundle_output_path', array($grievance_node, "node/$grievance_nid/settlements", "settlements"));
	$operations[] = array('grievance_bundle_output_path', array($grievance_node, "node/$grievance_nid/statuslog", "log"));
	
	// Launch the batch processor.
  $batch = array(
    'operations' => $operations,
		'finished' => 'grievance_bundle_finished'
  );
	// batch_set($batch);
	// /usr/bin/xvfb-run -a --server-args="-screen 0 1024x768x24" /usr/local/bin/wkhtmltopdf -q tmp.html tmp.pdf

  /*
	ob_start();
	menu_execute_active_handler('node/464');
	$output = ob_get_clean();
	ob_end_clean();
	ob_end_clean();

	file_put_contents($output_path, $output);

	drupal_set_message("Returned.");
	*/

	/*
	$renderable = node_view($grievance_node);
	$html = drupal_render($renderable);
	*/

	$renderable = menu_execute_active_handler('node/464', FALSE);
	$html = drupal_render($renderable);

	drupal_set_message($html);
	file_put_contents($output_path, $html);

	return $html;
}

function grievance_bundle_init($grievance_node, &$context) {
	$working_directory = grievance_bundle_working_directory($grievance_node);
	if (!file_exists($working_directory)) {
		mkdir($working_directory);
	}

	$context['message'] = t('Using working directory: @working_directory', array('@working_directory' => $working_directory)); 
}

function grievance_bundle_output_path($grievance_node, $path, $filename) {
	$working_directory = grievance_bundle_working_directory($grievance_node);
	$output_path = $working_directory . '/' . $filename;


	ob_start();
	menu_execute_active_handler('node/464');
	$output = ob_get_clean();
	ob_end_clean();

	file_put_contents($output_path, $output);

	$context['message'] = t('Output @path to @filename', array('@path' => $path, '@filename' => $filename));
}

function grievance_bundle_finished($success, $results, $operations) {
	drupal_set_message("The grievance bundle has been generated.");
}

function grievance_bundle_working_directory($grievance_node) {
	$base_path = grievance_bundle_base_path($grievance_node);
	return $base_path;
}

function grievance_bundle_path($grievance_node) {
	$base_path = grievance_bundle_base_path($grievance_node);
	return $base_path . '.html';
}

function grievance_bundle_base_path($grievance_node) {
	$path = DRUPAL_ROOT;
	$path .= '/' . variable_get('grievance_bundle_base_path', 'sites/default/files/test');

	$id = $grievance_node->field_grievance_id['und'][0]['value'];
	if (!$id) { $id = $grievance_node->nid; }

	$path .= '/' . $id;

	return $path;
}