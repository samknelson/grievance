<?php

function sirius_node_access_feed($node, $op, $account) {
	return user_access('sirius admin');
}

function sirius_node_tab_feed_process($feed_node) {
	return drupal_get_form('sirius_node_tab_feed_process_form', $feed_node);
}

function sirius_node_tab_feed_process_form($form, &$form_state, $feed_node) {
	$form_state['feed_node'] = $feed_node;

	$form['status'] = array('#markup' => '<div class="sirius_form_intro">' . 'Current status: <strong>' . $feed_node->field_sirius_feed_status['und'][0]['value'] . '</div>');

	$form['limit'] = array(
		'#type' => 'textfield',
		'#title' => t('Limit'),
		'#size' => 5,
		'#description' => t('Enter the number of rows to process. Leave blank to process the entire file.'),
	);

	$form['mode'] = array(
		'#type' => 'select',
		'#title' => t('Mode'),
		'#options' => array(
			'test' => t('Test mode'),
			'live' => t('Live mode'),
		),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Process Now',
	);

	return $form;
}

/**
 * @todo: Just hardcoding logic here for the time being.
 * - This function should call feed_process_csv()
 * - - $callback()
 * - - - $teamsters_feed_process_titan()
 */

function sirius_node_tab_feed_process_form_submit($form, &$form_state) {
	$feed_node = $form_state['feed_node'];

	$source = (object) $feed_node->field_sirius_attachments['und'][0];
	$destination = 'temporary://sirius_feed_' . $feed_node->nid;
	file_copy($source, $destination, FILE_EXISTS_REPLACE);

	$path = drupal_realpath($destination);
  $handle = fopen($path, "r");
  if (!$path) {
  	drupal_set_message("Unable to open $path", 'error');
  	return; 
  }

  // Headers
  $headers = fgetcsv($handle, 10000);

  // Load our rows one at a time
  $count = 0;
  $good_count = 0;
  $bad_count = 0;
  $limit = $form_state['values']['limit'];
  $mode = $form_state['values']['mode'];

  while ($row_raw = fgetcsv($handle, 10000, ",")) {
    entity_get_controller('node')->resetCache();
  	if ($limit && $count >= $limit) { break; }

  	++$count;
  	$row['row'] = $count;
  	foreach ($headers as $col => $header) {
  		$row[$header] = trim($row_raw[$col]);
  	}

  	$ssn = sirius_ssn_format($row['MBR_SOC_SEC_NO']);
  	$work_status_id = $row['JOB_PYMT_STATUS'];

  	if (!sirius_ssn_validate($ssn)) {
  		sirius_feeds_log($feed_node, $row, 'error:bad_ssn', 'Bad or misformed SSN');
  		++$bad_count;
  		continue;
  	}

  	if (!is_numeric($work_status_id)) {
  		sirius_feeds_log($feed_node, $row, 'error:bad_work_status_id', 'Bad or misformed work status ID');
  		++$bad_count;
  		continue;
		}

		// Find the work status
	  $query = new EntityFieldQuery();
	  $query->entityCondition('entity_type', 'taxonomy_term');
	  $query->entityCondition('bundle', 'sirius_work_status');
	  $query->fieldCondition('field_sirius_id2', 'value', $work_status_id);
	  $stmt = $query->execute();
	  if (!$stmt['taxonomy_term']) {
  		sirius_feeds_log($feed_node, $row, 'error:work_status_not_found', "Work status with titan ID $work_status_id not found.");
  		++$bad_count;
  		continue;
	  }
	  $work_status_tid = array_keys($stmt['taxonomy_term'])[0];

	  // Worker
	  $query = new EntityFieldQuery();
	  $query->entityCondition('entity_type', 'node');
	  $query->entityCondition('bundle', 'sirius_worker');
	  $query->fieldCondition('field_sirius_ssn', 'value', $ssn);
	  $stmt = $query->execute();
	  if (!$stmt['node']) {
  		sirius_feeds_log($feed_node, $row, 'error:worker_not_found', "The worker with SSN $ssn was not found.");
  		++$bad_count;
  		continue;
	  }
	  $worker_nids = array_keys($stmt['node']);

	  if ($mode == 'live') {
		  foreach ($worker_nids as $worker_nid) {
		  	$worker_node = node_load($worker_nid);
		  	sirius_worker_work_status_set($worker_node, $work_status_tid, 'titan import', array());
		  }
		}

	  ++$good_count;
  }
  fclose($handle);

  drupal_set_message("Processed $count rows; $good_count were successful and $bad_count were errors. See the log for details.");
  if ($mode == 'live') { 
  	$feed_node->field_sirius_feed_status['und'][0]['value'] = 'complete';
  }
}

function sirius_feeds_log($feed_node, $row, $type, $message, $other_handler_nids = array()) {
	static $counts;

	++$counts[$type];
	if ($counts[$type] == 20) { $message = "--- subsequent error meesagees of the same type will be suppressed ---"; }
	if ($counts[$type] > 20) { return; }

	$feed_type_term = taxonomy_term_load($feed_node->field_sirius_feed_type['und'][0]['tid']);

	$category = 'feeds:' . $feed_type_term->name;
	$type = $type;
	$message = 'Row #' . $row['row'] . ': ' . $message;
	$summary = $feed_node->title . ': Row #' . $row['row'];
	$notes = '';
	$handler_nids = array($feed_node->nid);
	$handler_nids += $other_handler_nids;
	$notes = '';
	$json = json_encode($row);
	$files = array();

	sirius_log(
	  $category, 
	  $type, 
	  $message, 
	  $summary, 
	  $notes, 
	  $handler_nids, 
	  $files,
	  $json
	);
}