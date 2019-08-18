<?php

use Box\Spout\Reader\Common\Creator\ReaderEntityFactory;

function sirius_node_tab_feed_wizard($feed_node) {
	return sirius_node_tab_wizard($feed_node, $feed_node->field_sirius_type['und'][0]['value']);
}

function sirius_node_access_feed($feed_node, $op, $account) {
	return user_access('sirius admin');
}

function sirius_feed_default_column_title($col) {
	$letters = array('A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z');
	if ($col > 25) { $prefix = sirius_feed_default_column_title(floor($col/26)-1); }
	return $prefix . $letters[$col % 26];
}

function sirius_feed_lastdata($info, $feed_node) {
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_feed');
	$query->fieldCondition('field_sirius_type', 'value', $feed_node->field_sirius_type['und'][0]['value']);
  $query->fieldCondition('field_sirius_feed_status', 'value', 'complete');
  if ($feed_node->field_grievance_shop['und'][0]['target_id']) {
		$query->fieldCondition('field_grievance_shop', 'target_id', $feed_node->field_grievance_shop['und'][0]['target_id']);
  }
  $query->propertyOrderBy('created', 'desc');
  $query->range(0,1);
  $stmt = $query->execute();
  if (!$stmt['node']) { return NULL; }
  $last_feed_node = node_load(array_keys($stmt['node'])[0]);
  return sirius_wizard_data($info, $last_feed_node);
}

/************************************************************************************************
 * List page
 ************************************************************************************************/

function sirius_feed_page_list_access($wizard_name) {
	return user_access('sirius admin');
}

function sirius_feed_page_list($wizard_name) {
	$info = sirius_wizard_info($wizard_name);
	if (!$info) { return "No such wizard."; }

	drupal_set_title("Feed list: " . $info['title']);

	$html .=  views_embed_view('sirius_feeds', 'block_1', $wizard_name);

	return $html;
}

/************************************************************************************************
 * Landing page
 ************************************************************************************************/

function sirius_feed_page_landing_access() {
	return user_access('sirius admin');
}

function sirius_feed_page_landing() {
	$all_info = sirius_wizard_info_all();
	$html = '<h2>Available Feeds</h2>';
	foreach ($all_info as $key => $info) {
		if (!$info['feed']) { continue; }
		$html .= "<li>";
		$html .= l($info['title'], "sirius/feeds/$key");
		$html .= '</li>';
	}
	$html .= '</ul>';

	return $html;
}

/************************************************************************************************
 * Spout
 ************************************************************************************************/

function sirius_feed_spout_init() {
	require_once DRUPAL_ROOT . '/sites/all/libraries/spout/vendor/autoload.php';
}

function sirius_feed_spout_cleanup($feed_node) {
	$source = (object) $feed_node->field_sirius_attachments['und'][0];
	$dest = 'temporary://sirius_feed_' . $source->filename;
	if (file_exists($dest)) { 
		file_unmanaged_delete($dest);
	}
}

function sirius_feed_spout_get_reader($feed_node) {
	// Copy to a temporary directory, because the upload is in private files and might be S3
	$source = (object) $feed_node->field_sirius_attachments['und'][0];
	$dest = 'temporary://sirius_feed_' . $source->filename;
	if (!file_exists($dest)) {
		file_copy($source, $dest, FILE_EXISTS_REPLACE);
	}
	$path = drupal_realpath($dest);

	sirius_feed_spout_init();
	$reader = ReaderEntityFactory::createReaderFromFile($path);
	$reader->open($path);
	return $reader;
}

function sirius_feed_spout_get_sheet($reader, $sheet_name) {
	foreach ($reader->getSheetIterator() as $sheet) {
		if (!$sheet_name) { break; }
  	if ($sheet->getName() == $sheet_name) { break; }
  }
  return $sheet;
}


function sirius_feed_spout_get_header($sheet, $header_row) {
	$header = array();
	foreach ($sheet->getRowIterator() as $row) {
		$cells = array();
		foreach ($row->getCells() as $cell) {
			$cells[] = $cell->getValue();
		}
		if ($header_row == 'Yes') {
			$header = $cells;
		} else {
			foreach ($cells as $col => $ignore) {
				$header[] = sirius_feed_default_column_title($col);
			}
		}
		break;
	}
	return $header;
}

function sirius_feed_spout_get_cell($row, $col) {
	$cell = $row->getCellAtIndex($col);
	if (!$cell) { return NULL; }
	return $cell->getValue();
}

function sirius_feed_spout_get_import_row($row, $info, $data, $count) {
	$import_data_row = array('xxxrowcountxxx' => $count);
	foreach ($info['feed']['fields'] as $field_name => $field_info) {
		$col = $data["map_$field_name"];
		if ($col === NULL || $col === '') { continue; }
		$import_data_row[$field_name] = sirius_feed_spout_get_cell($row, $col);
	}
	return $import_data_row;
}

/************************************************************************************************
 * Step 0: Launch
 ************************************************************************************************/

function sirius_feed_page_launch_access($wizard_name) {
	$info = sirius_wizard_info($wizard_name);
	if (!$info) { return FALSE; }

	return user_access('sirius admin');
}

function sirius_feed_page_launch($wizard_name) {
	return drupal_get_form('sirius_feed_page_launch_form', $wizard_name);
}

function sirius_feed_page_launch_form($form, &$form_state, $wizard_name) {
	$info = sirius_wizard_info($wizard_name);
	drupal_set_title('Create an Upload: ' . $info['title']);

	$form_state['wizard_name'] = $wizard_name;
	if ($info['description']) {
		$form['intro'] = array('#markup' => '<div class="sirius_form_intro">' . $info['description'] . '</div>');
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Start a new upload',
	);

	return $form;
}

function sirius_feed_page_launch_form_submit($form, &$form_state) {
	$wizard_name = $form_state['wizard_name'];
	$info = sirius_wizard_info($wizard_name);
	sirius_wizard_launch($info);
}

function sirius_feed_wizard_launch($wizard_name) {
  $feed_node = new stdClass();
  $feed_node->type = 'sirius_feed';
  $feed_node->no_recurse = TRUE;
  $feed_node->language = LANGUAGE_NONE;
  $feed_node->uid = $user->uid;
  $feed_node->status = 1;
  $feed_node->promote = 0;
  $feed_node->comment = 0;
  $feed_node->field_sirius_type['und'][0]['value'] = $wizard_name;
  $feed_node->field_sirius_feed_status['und'][0]['value'] = 'draft';
  $feed_node = node_submit($feed_node);
  node_object_prepare($feed_node);
  node_save($feed_node);

  drupal_goto('node/' . $feed_node->nid . '/sirius_feed_wizard');
}

/************************************************************************************************
 * Step 1: Upload
 ************************************************************************************************/

function sirius_feed_wizard_upload($info, $data, $feed_node, $current_step) { return drupal_get_form('sirius_feed_wizard_upload_form', $info, $data, $feed_node, $current_step); }

function sirius_feed_wizard_upload_form($form, &$form_state, $info, $data, $feed_node, $current_step) {
	$form_state['info'] = $info;
	$form_state['data'] = $data;
	$form_state['feed_node'] = $feed_node;
	$form_state['current_step'] = $current_step;
	$steps = sirius_wizard_steps($info, $data, $feed_node);

	if ($feed_node->field_sirius_attachments['und'][0]) {
		$mode = 'full';
		$renderable = field_view_field('node', $feed_node, 'field_sirius_attachments', 'full');
		$renderable['#title'] = 'Currently Processing';
		$form['current'] = array('#markup' => render($renderable));
		$title = "Replace with";
	} else {
		$title = "Upload";
	}

	$form['file'] = array(
		'#type' => 'file',
		'#title' => $title,
		'#default_value' => $data['steps'][$current_step]['data']['a_random_variable'],
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Submit',
	);

	return $form;
}

function sirius_feed_wizard_upload_form_validate($form, &$form_state) {
	$info = field_info_instance('node', 'field_sirius_attachments', 'sirius_feed');
	$extensions = $info['settings']['file_extensions'];
  $file = file_save_upload("file", array(
    'file_validate_extensions' => array($extensions),
  ));
  if ($file) {
    $name = rand();
    if ($file = file_move($file, 'private://attach_' . rand())) {
      $form_state['storage']["file"] = $file;
    }
    else {
      form_set_error("file", t("Failed to write the uploaded file to the site's file folder."));
    }
  }

  if (!$file) {
  	form_set_error("file", t("Please upload a file."));
  }
}

function sirius_feed_wizard_upload_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$feed_node = $form_state['feed_node'];
	$current_step = $form_state['current_step'];

	// Save the data for this step
	sirius_feed_spout_cleanup($feed_node);
  $file = $form_state['storage']["file"];
  if ($file) {
		$file = (array) $file;
		$file['display'] = 1;
		unset($form_state['storage']["attach_$delta"]);
		unset($file['destination']);
		unset($file['uri']);
		$feed_node->field_sirius_attachments['und'][0] = $file;
  }

	sirius_wizard_submit($info, $data, $feed_node, $current_step, "File uploaded.");
}

/************************************************************************************************
 * Step 2: Worksheet
 ************************************************************************************************/

function sirius_feed_wizard_sheet($info, $data, $feed_node, $current_step) { return drupal_get_form('sirius_feed_wizard_sheet_form', $info, $data, $feed_node, $current_step); }

function sirius_feed_wizard_sheet_form($form, &$form_state, $info, $data, $feed_node, $current_step) {
	$form_state['info'] = $info;
	$form_state['data'] = $data;
	$form_state['feed_node'] = $feed_node;
	$form_state['current_step'] = $current_step;
	$steps = sirius_wizard_steps($info, $data, $feed_node);
	$last_data = sirius_feed_lastdata($info, $feed_node);

	$reader = sirius_feed_spout_get_reader($feed_node);
	$names = array();
	$i = 0;
	foreach ($reader->getSheetIterator() as $sheet) {
		$name = $sheet->getName();
		$display_name = $name;
		if (!$name) { 
			$name = $i; 
			$display_name = "Sheet" . ($i+1);
		}
		$names[$name] = $display_name;
	}
	$reader->close();

	$default = $data['worksheet'];
	if (!$default) { $default = $last_data['worksheet']; }
	$form['worksheet'] = array(
		'#title' => t('Worksheet'),
		'#type' => 'select',
		'#options' => $names,
		'#default_value' => $default,
		'#required' => TRUE,
	);

	$default = $data['header_row'];
	if (!$default) { $default = $last_data['header_row']; }
	$form['header_row'] = array(
		'#title' => t('First row contains column headers?'),
		'#type' => 'select',
		'#options' => array('Yes' => 'Yes', 'No' => 'No'),
		'#default_value' => $default,
		'#required' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Submit',
	);

	return $form;
}

function sirius_feed_wizard_sheet_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$feed_node = $form_state['feed_node'];
	$current_step = $form_state['current_step'];
	$data['worksheet'] = $form_state['values']['worksheet'];
	$data['header_row'] = $form_state['values']['header_row'];

	sirius_wizard_submit($info, $data, $feed_node, $current_step, "Worksheet selection saved.");
}

/************************************************************************************************
 * Step 3: Mapping
 ************************************************************************************************/

function sirius_feed_wizard_map($info, $data, $feed_node, $current_step) { return drupal_get_form('sirius_feed_wizard_map_form', $info, $data, $feed_node, $current_step); }

function sirius_feed_wizard_map_form($form, &$form_state, $info, $data, $feed_node, $current_step) {
	$form_state['info'] = $info;
	$form_state['data'] = $data;
	$form_state['feed_node'] = $feed_node;
	$form_state['current_step'] = $current_step;
	$steps = sirius_wizard_steps($info, $data, $feed_node);
	$last_data = sirius_feed_lastdata($info, $feed_node);

	// Preview
	$reader = sirius_feed_spout_get_reader($feed_node);
	$sheet = sirius_feed_spout_get_sheet($reader, $data['worksheet']);
	$header = sirius_feed_spout_get_header($sheet, $data['header_row']);

	$count = 0;
	$html_rows = array();
	foreach ($sheet->getRowIterator() as $row) {
		if ($count == 0 && $data['header_row'] == 'Yes') { ++$count; continue; }
		$html_row = array();
		foreach ($header as $col => $ignore) { $html_row[] = sirius_feed_spout_get_cell($row, $col); }
		$html_rows[] = $html_row;
		++$count; 
		if ($count >= 10) { break; }
	}

	foreach ($info['feed']['fields'] as $field_name => $field_info) {
		$default = $data["map_$field_name"];
		if (!$default) { $default = $last_data["map_$field_name"]; }

		if ($field_info['required']) {
			$options = $header;
		} else {
			$options = array(NULL => '-- Select --') + $header;
		}
		$form["map_$field_name"] = array(
			'#title' => $field_info['title'],
			'#description' => $field_info['description'],
			'#type' => 'select',
			'#options' => $options,
			'#required' => $field_info['required'],
			'#default_value' => $default,
		);
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Submit',
	);

	$html = "<h2>Preview - First $count Rows</h2>";
	$html .= theme_table(array('rows' => $html_rows, 'header' => $header, 'attributes' => array()));
	$form["preview"] = array('#markup' => $html);

	return $form;
}

function sirius_feed_wizard_map_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$feed_node = $form_state['feed_node'];
	$current_step = $form_state['current_step'];
	foreach ($info['feed']['fields'] as $field_name => $field_info) {
		$data["map_$field_name"] = $form_state['values']["map_$field_name"];
	}

	sirius_wizard_submit($info, $data, $feed_node, $current_step, "Worksheet selection saved.");
}

/************************************************************************************************
 * Step 4: Verify
 ************************************************************************************************/

function sirius_feed_wizard_verify($info, $data, $feed_node, $current_step) { return drupal_get_form('sirius_feed_wizard_verify_form', $info, $data, $feed_node, $current_step); }

function sirius_feed_wizard_verify_form($form, &$form_state, $info, $data, $feed_node, $current_step) {
	$form_state['info'] = $info;
	$form_state['feed_node'] = $feed_node;
	$form_state['current_step'] = $current_step;

	// Preview
	$reader = sirius_feed_spout_get_reader($feed_node);
	$sheet = sirius_feed_spout_get_sheet($reader, $data['worksheet']);
	$header = sirius_feed_spout_get_header($sheet, $data['header_row']);

	$import_header = array('xxxrowcountxxx' => 'Row');
	foreach ($info['feed']['fields'] as $field_name => $field_info) {
		$col = $data["map_$field_name"];
		if ($col === NULL || $col === '') { continue; }
		$import_header[$field_name] = $field_info['title'] . '<br>(' . $header[$col] . ')';
	}
	$import_header['xxxsuccessxxx'] = 'Will Import';
	$import_header['xxxmsgxxx'] = 'Message';

	// Get our data
	$count = 0;
	$import_data = array();
	$counts = array();
	$callback = $info['feed']['callback_row_verify'];
	foreach ($sheet->getRowIterator() as $row) {
		if ($count == 0 && $data['header_row'] == 'Yes') { ++$count; continue; }
		$import_data_row = sirius_feed_spout_get_import_row($row, $info, $data, $count);
		$result = $callback($feed_node, $info, $data, $import_data_row, FALSE);
		if ($result['success']) { $import_data_row['xxxsuccsssxxx'] = 'Yes'; } else { $import_data_row['xxxsuccsssxxx'] = 'No'; }
		$import_data_row['xxxmsgxxx'] = $result['msg'];

		++$counts[$result['msg']];
		if ($counts[$result['msg']] == 6) { $import_data_row['xxxmsgxxx'] .= ' (Subsequent messages of this type will be suppresssed.)'; }
		if ($counts[$result['msg']] <= 6) { $import_data[] = $import_data_row; }

		++$count;
	}
	$data['rows'] = $count + 1;
	$form_state['data'] = $data;

	$html = "<h2>Summary</h2>";
	$summary_rows = array();
	foreach ($counts as $key => $value) { $summary_rows[] = array($key, $value); }
	$html .= theme_table(array('rows' => $summary_rows, 'header' => array('Type', 'Count'), 'attributes' => array()));
	$html .= "<h2>Preview</h2>";
	$html .= theme_table(array('rows' => $import_data, 'header' => $import_header, 'attributes' => array()));
	$form["preview"] = array('#markup' => $html);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Submit',
	);

	return $form;
}

function sirius_feed_wizard_verify_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$feed_node = $form_state['feed_node'];
	$current_step = $form_state['current_step'];
	sirius_wizard_submit($info, $data, $feed_node, $current_step, "Ready to Process.");
}

/************************************************************************************************
 * Step 5: Process
 ************************************************************************************************/

function sirius_feed_wizard_process($info, $data, $feed_node, $current_step) { return drupal_get_form('sirius_feed_wizard_process_form', $info, $data, $feed_node, $current_step); }

function sirius_feed_wizard_process_form($form, &$form_state, $info, $data, $feed_node, $current_step) {
	$form_state['info'] = $info;
	$form_state['data'] = $data;
	$form_state['feed_node'] = $feed_node;
	$form_state['current_step'] = $current_step;

	$form['limit'] = array(
		'#type' => 'textfield',
		'#title' => t('Limit'),
		'#size' => 5,
		'#description' => t('Enter a number to process a limited number of batches, i.e. for testing. Leave blank to process the entire file.'),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Import Now',
	);

	return $form;
}

function sirius_feed_wizard_process_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$feed_node = $form_state['feed_node'];
	$current_step = $form_state['current_step'];
	$limit = $form_state['values']['limit'];

	$batch_size = $info['feed']['batch_size'];
	if (!$batch_size) { $batch_size = 10; }
	$idx = 0;
	$rows = $data['rows'];
	$batch_count = 0;
	while ($idx < $rows) {
		$operations[] = array('sirius_feed_wizard_process_batch', array($feed_node, $info, $data, $idx));
		$idx += $batch_size;
		++$batch_count;
		if ($limit && $batch_count >= $limit) { break; }
	}
	$operations[] = array('sirius_feed_wizard_process_finish', array($feed_node, $info, $data, $idx));
	$batch = array('operations' => $operations);
	batch_set($batch);
}

function sirius_feed_wizard_process_batch($feed_node, $info, $data, $idx, &$context) {
	$batch_size = $info['feed']['batch_size'];
	if (!$batch_size) { $batch_size = 10; }

	$reader = sirius_feed_spout_get_reader($feed_node);
	$sheet = sirius_feed_spout_get_sheet($reader, $data['worksheet']);
	$header = sirius_feed_spout_get_header($sheet, $data['header_row']);

	// Get our data
	$count = 0;
	$import_data = array();
	$counts = $context['results']['counts'];
	$callback = $info['feed']['callback_row_process'];
	foreach ($sheet->getRowIterator() as $row) {
		if ($count == 0 && $data['header_row'] == 'Yes') { ++$count; continue; }
		++$count;
		if ($count < $idx) { continue; }
		if ($count > $idx+$batch_size) { break; }
		$import_data_row = sirius_feed_spout_get_import_row($row, $info, $data, $count);
		$result = $callback($feed_node, $info, $data, $import_data_row);
		++$counts[$result['msg']];
	}
	$context['results']['counts'] = $counts;

	$context['message'] = "Processing rows $idx to " . ($idx+$batch_size-1) . ".";
}

function sirius_feed_wizard_process_finish($feed_node, $info, $data, $idx, &$context) {
	$context['message'] = "Cleaning up.";
	$data['feed']['counts'] = $context['results']['counts'];
  $feed_node->field_sirius_feed_status['und'][0]['value'] = 'complete';
	sirius_wizard_submit($info, $data, $feed_node, 'process', "Import complete");
}

/************************************************************************************************
 * Step 6: Review
 ************************************************************************************************/

function sirius_feed_wizard_review($info, $data, $feed_node, $current_step) { return drupal_get_form('sirius_feed_wizard_review_form', $info, $data, $feed_node, $current_step); }

function sirius_feed_wizard_review_form($form, &$form_state, $info, $data, $feed_node, $current_step) {
	$form_state['info'] = $info;
	$form_state['data'] = $data;
	$form_state['feed_node'] = $feed_node;
	$form_state['current_step'] = $current_step;

	$summary_rows = array();
	foreach ($data['feed']['counts'] as $key => $value) { $summary_rows[] = array($key, $value); }
	$html .= theme_table(array('rows' => $summary_rows, 'header' => array('Type', 'Count'), 'attributes' => array()));
	$form['review'] = array('#markup' => $html);

	return $form;
}


/************************************************************************************************
 * WWS Upload
 ************************************************************************************************/

function sirius_feed_wws_row_verify($feed_node, $info, $data, &$row, $precompute) {
	static $work_status_map;
	if (!$work_status_map) {
		$work_status_map = sirius_taxonomy_load('sirius_work_status', 'field_sirius_id2', 'full');
	}

	$ssn = $row['ssn'];
	if (!sirius_ssn_validate($ssn)) { return array('success' => FALSE, 'msg' => 'Invalid SSN'); }

  // Worker
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_worker');
  $query->fieldCondition('field_sirius_ssn', 'value', $ssn);
  $stmt = $query->execute();
  if (!$stmt['node']) { return array('success' => FALSE, 'msg' => 'No worker found.'); }
  if (count($stmt['node']) > 1) { return array('success' => FALSE, 'msg' => 'Multiple workers found.'); }
  if ($precompute) { $row['worker_nid'] = array_keys($stmt['node'])[0]; }

	// Find the work status
	$work_status_id = $row['work_status_id'];
	if ($work_status_id === NULL || $work_status_id === '') { return array('success' => FALSE, 'msg' => 'No work status ID'); }
	if (!$work_status_map[$work_status_id]) { return array('success' => FALSE, 'msg' => "Work status $work_status_id not found."); }
	if ($precompute) { $row['work_status_tid'] = $work_status_map[$work_status_id]->tid; }

  return array('success' => TRUE, 'msg' => 'Looks good.');
}

function sirius_feed_wws_row_process($feed_node, $info, $data, $row) {
	$result = sirius_feed_wws_row_verify($feed_node, $info, $data, $row, TRUE);
	if (!$result['success']) { return $result; }
	return sirius_worker_work_status_set(node_load($row['worker_nid']), $row['work_status_tid'], 'import', array());
}

/************************************************************************************************
 * Worker Upload
 ************************************************************************************************/

function sirius_feed_worker_row_verify($feed_node, $info, $data, &$row, $precompute) {
	static $employer_map;
	if (!$employer_map) {
		$sql = "select title, nid from node where type = 'grievance_shop'";
		$stmt = db_query($sql, array());
		while ($hr = $stmt->fetchAssoc()) {
			$employer_map[$hr['title']] = $hr['nid'];
		}
	}

	// Trim all
	foreach ($row as $key => $value) {
		$row[$key] = trim($value);
	}

	if (!preg_match('/^\d+$/', $row['id'])) { return array('success' => FALSE, 'msg' => 'Invalid Member ID'); }

	if ($row['phone'] && !sirius_phone_validate($row['phone'])) { return array('success' => FALSE, 'msg' => 'Invalid Phone'); } else { $row['phone'] = sirius_phone_format($row['phone']); }
	if ($row['phone_alt'] && !sirius_phone_validate($row['phone_alt'])) { return array('success' => FALSE, 'msg' => 'Invalid Phone (Alt)'); } else { $row['phone_alt'] = sirius_phone_format($row['phone_alt']); }
	if ($row['phone_mobile'] && !sirius_phone_validate($row['phone_mobile'])) { return array('success' => FALSE, 'msg' => 'Invalid Phone (Mobile)'); } else { $row['phone_mobile'] = sirius_phone_format($row['phone_mobile']); }

	if ($row['email'] && !sirius_email_validate($row['email'])) { return array('success' => FALSE, 'msg' => 'Invalid Email'); }

  // Worker
  if ($precompute) {
  	$sql = "select max(entity_id) as a, min(entity_id) as b from field_data_field_sirius_id ";
  	$sql .= "where entity_type = 'node' ";
  	$sql .= "and bundle = 'sirius_worker' ";
  	$sql .= "and field_sirius_id_value = :id ";
  	$stmt = db_query($sql, array(':id' => $row['id']));
  	$hr = $stmt->fetchAssoc();
  	$nid = $hr['a'];
  	$other_nid = $hr['b'];
  	if ($nid != $other_nid) {
  		return array('success' => FALSE, 'msg' => 'Multiple workers found with this ID.');
  	}
  	$row['worker_nid'] = $nid;
  }

  // Current Employer
  if ($row['shop_name']) {
  	$shop_nid = $employer_map[$row['shop_name']];
  	if (!$shop_nid) { return array('success' => FALSE, 'msg' => 'Employer "' . $row['shop_name'] . '" not found.'); }
  	if ($precompute) { $row['shop_nid'] = $shop_nid; }
  }

  return array('success' => TRUE, 'msg' => 'Looks good.');
}

function sirius_feed_worker_row_process($feed_node, $info, $data, $row) {
	$result = sirius_feed_worker_row_verify($feed_node, $info, $data, $row, TRUE);
	if (!$result['success']) { return $result; }

	if ($row['worker_nid']) {
		$is_new = FALSE;
		$worker_node = node_load($row['worker_nid']);
	} else {
		$is_new = TRUE;
    global $user;
    $worker_node = new stdClass();
    $worker_node->type = 'sirius_worker';
    $worker_node->language = LANGUAGE_NONE;
    $worker_node->uid = $user->uid;
    $worker_node->status = 1;
    $worker_node->promote = 0;
    $worker_node->comment = 0;
    $worker_node->field_sirius_id['und'][0]['value'] = $row['id'];
  }

  $worker_node->field_sirius_name['und'][0]['given'] = $row['name_given'];
  $worker_node->field_sirius_name['und'][0]['middle'] = $row['name_middle'];
  $worker_node->field_sirius_name['und'][0]['family'] = $row['name_family'];

  $phone1 = $row['phone'];
  $phone2 = $row['phone_alt'];
  if ($row['phone_mobile']) {
  	if (!$phone1) { $phone1 = $row['phone_mobile']; } else { $phone2 = $row['phone_mobile']; }
  }
  if ($phone1) { $worker_node->field_sirius_phone['und'][0]['value'] = $phone1; }
  if ($phone2) { $worker_node->field_sirius_phone_alt['und'][0]['value'] = $phone2; }
  if ($row['email']) { $worker_node->field_sirius_email['und'][0]['value'] = $row['email']; }

  if ($row['shop_nid']) { $worker_node->field_grievance_shop['und'][0]['target_id'] = $row['shop_nid']; }


  foreach (array('thoroughfare', 'premise', 'locality', 'administrative_area', 'postal_code') as $col) {
  	$has_address = FALSE;
  	if ($row["address_$col"]) {
  		$has_address = TRUE;
  		$worker_node->field_sirius_address['und'][0][$col] = $row["address_$col"];
  	}
  	if ($has_address) {
  		$worker_node->field_sirius_address['und'][0]['country'] = 'US';
  	}
  }

  node_save($worker_node);

  if ($is_new) {
  	return array('success' => TRUE, 'msg' => 'Created a new worker.');
  } else {
  	return array('success' => TRUE, 'msg' => 'Updated an existing worker.');
  }
}