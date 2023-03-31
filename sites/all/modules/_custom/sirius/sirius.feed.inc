<?php

use Box\Spout\Reader\Common\Creator\ReaderEntityFactory;

function sirius_node_tab_feed_wizard_access($feed_node) {
	$domain_nid = sirius_domain_nid();
	if ($domain_nid != $feed_node->field_sirius_domain['und'][0]['target_id']) { return FALSE; }

	if (user_access('sirius admin')) { return TRUE; }
	if (user_access('sirius domain admin')) { return TRUE; }

	$wizard_name = $feed_node->field_sirius_type['und'][0]['value'];
	$info = sirius_wizard_info($wizard_name);
	if (!$info) { return FALSE; }

	if ($info['feed']['employer_access'] && user_access('sirius employer user manager')) { 
		$employer_node = sirius_employer_get();
		if ($employer_node && ($employer_node->nid == $feed_node->field_grievance_shop['und'][0]['target_id'])) { return TRUE; }
	}

	return FALSE;
}

function sirius_node_tab_feed_wizard($feed_node) {
	return sirius_node_tab_wizard($feed_node, $feed_node->field_sirius_type['und'][0]['value']);
}

function sirius_node_access_feed($feed_node, $op, $account) {
	// Admin can edit these nodes directly
	if (user_access('sirius admin')) { return NODE_ACCESS_IGNORE; }

	// Everybody else can only view
	if ($op == 'view' && sirius_node_tab_feed_wizard_access($feed_node)) { return NODE_ACCESS_IGNORE; }
	
	return NODE_ACCESS_DENY;
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

function sirius_feed_node_search($id, $bundle, $fields) {
	foreach ($fields as $field) {
		$sql_args = array(':bundle' => $bundle, ':id' => $id);
		if ($field == 'nid') { 
			$sql = "select max(nid) as a, min(nid) as b from node where type = :bundle and nid = :id";
		} else if ($field == 'title') { 
			$sql = "select max(nid) as a, min(nid) as b from node where type = :bundle and title = :id";
		} else {
			$sql = "select max(entity_id) as a, min(entity_id) as b ";
			$sql .= "from field_data_field_$field ";
			$sql .= "where entity_type = 'node' ";
			$sql .= "and bundle = :bundle ";
			$sql .= "and field_{$field}_value = :id ";
		}
		$stmt = db_query($sql, $sql_args);
		$hr = $stmt->fetchAssoc();
		if (!$hr['a']) { continue; }
		if ($hr['a'] != $hr['b']) { return array('success' => FALSE, 'msg' => "Multiple records of type $bundle found in $field with id $id.", 'multiple' => TRUE); }
		return array('success' => TRUE, 'nid' => $hr['a']);
	}

	return array('success' => FALSE, 'msg' => "No record of type $bundle found with id $id.");
}

function sirius_feed_latest_entry($wizard_name) {
	$domain_nid = sirius_domain_nid();
	$info = sirius_wizard_info($wizard_name);
	if (!$info) { return NULL; }

	// Oh well, no callback. In that case we just hardcode some view.
	if (user_access('sirius admin') || user_access('sirius domain admin')) {
		// admins see the full list
	} else {
		// Non-admins see a filtered list
		$employer_node = sirius_employer_get();
		$employer_nid = $employer_node->nid;
	}

  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_feed');
	$query->fieldCondition('field_sirius_type', 'value', $wizard_name);
  if ($employer_nid) { $query->fieldCondition('field_grievance_shop', 'target_id', $employer_nid); };
  if ($domain_nid) { $query->fieldCondition('field_sirius_domain', 'target_id', $domain_nid); };
  $query->propertyOrderBy('created', 'desc');
  $query->range(0,1);
  $stmt = $query->execute();
  if (!$stmt['node']) { return NULL; }
  $last_feed_node = node_load(array_keys($stmt['node'])[0]);
  return $last_feed_node;
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
	try {
		$reader = ReaderEntityFactory::createReaderFromFile($path);
		$reader->open($path);
		return $reader;
	} catch (exception $e) {
		drupal_set_message($e->getMessage(), 'error');
		return NULL;
	}
}

function sirius_feed_spout_get_sheet($reader, $sheet_name) {
	if (!$reader) { return NULL; }
	foreach ($reader->getSheetIterator() as $sheet) {
		if (!$sheet_name) { break; }
  	if ($sheet->getName() == $sheet_name) { break; }
  }
  return $sheet;
}


function sirius_feed_spout_get_header($sheet, $header_row) {
	if (!$sheet) { return NULL; }
	$header = array();
	foreach ($sheet->getRowIterator() as $row) {
		$cells = array();
		foreach ($row->getCells() as $cell) {
			$cells[] = sirius_feed_spout_get_cell_value($cell);
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

	return sirius_feed_spout_get_cell_value($cell);
}

function sirius_feed_spout_get_cell_value($cell) {
	try {
		// Start with the reaw cell value
		$value = $cell->getValue();

		// If it's an object, try to render it. But don't try too hard.
		if (is_object($value)) {
			$class = get_class($value);
			if ($class = 'DateTime') { $value = $value->format('Y-m-d H:i:s e'); }
			else { $value = '(object)'; }
		}

		// Convert to UTF8 and trim
		$value = mb_convert_encoding($value, 'UTF-8');
		$value = trim($value);

		return $value;
	} catch (exception $e) {
		drupal_set_message($e->getMessage(), 'error');
		return NULL;
	}
}

function sirius_feed_spout_get_import_row($fields, $row, $data, $count, $header) {
	$import_data_row = array('xxxrowcountxxx' => $count);
	foreach ($fields as $field_name => $field_info) {
		$col = $data["map_$field_name"];
		if ($col === NULL || $col === '') { continue; }
		$import_data_row[$field_name] = sirius_feed_spout_get_cell($row, $col);
	}

	$raw_row = array();
	$delta = 0;
	foreach ($row->getCells() as $cell) {
		$title = $header[$delta];
		if (!$title) { $title = $delta; }
		$raw_row[$title] = sirius_feed_spout_get_cell_value($cell);
		++$delta;
	}
	$import_data_row['xxxrawxxx'] = $raw_row;

	return $import_data_row;
}

/************************************************************************************************
 * Step 0: Launch
 ************************************************************************************************/

function sirius_feed_page_launch_access($wizard_name) {
	sirius_require('sirius', 'sirius.page_feed_list.inc');
	return sirius_feed_page_list_access($wizard_name);
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
	$domain = sirius_domain();
  $feed_node = new stdClass();
  $feed_node->type = 'sirius_feed';
  $feed_node->no_recurse = TRUE;
  $feed_node->language = LANGUAGE_NONE;
  $feed_node->uid = $user->uid;
  $feed_node->status = 1;
  $feed_node->promote = 0;
  $feed_node->comment = 0;
  if ($domain) { $feed_node->field_sirius_domain['und'][0]['target_id'] = $domain->nid; }
  $feed_node->field_sirius_type['und'][0]['value'] = $wizard_name;
  $feed_node->field_sirius_feed_status['und'][0]['value'] = 'draft';

  if (!user_access('sirius admin') && !user_access('sirius domain admin')) {
  	$employer_node = sirius_employer_get();
  	if (!$employer_node) {
  		drupal_set_message("No employer.", 'error');
  		return;
  	}
  	$feed_node->field_grievance_shop['und'][0]['target_id'] = $employer_node->nid;
  }

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
	$last_data = sirius_feed_lastdata($info, $feed_node);

	if ($feed_node->field_sirius_attachments['und'][0]) {
		$mode = 'full';
		$renderable = field_view_field('node', $feed_node, 'field_sirius_attachments', 'full');
		$renderable['#title'] = 'Currently Processing';
		$form['current'] = array('#markup' => render($renderable));
		if ($data['origname']) {

			$form['origname'] = array('#markup' => sirius_fakerender_field('Uploaded Filename:', $data['origname']));
		}
		$title = "Replace with";
	} else {
		$title = "Upload";
	}

	$form['file'] = array(
		'#type' => 'file',
		'#title' => $title,
	);

	$default = $data['source'];
	if (!$default) { $default = 'Import ' . date('Y-m-d H:i:s'); }
	$form['source'] = array(
		'#type' => 'textfield',
		'#title' => 'Source',
		'#default_value' => $default,
	);

	$pks = $info['feed']['pks'];
	if ($pks) {
		$form['pk'] = array(
			'#type' => 'select',
			'#title' => 'Primary Key',
			'#options' => array(NULL => t('-- Select --')) + $pks,
			'#default_value' => $data['pk'],
			'#required' => TRUE,
		);

		if ($info['feed']['create_new_records'] == 'optional') {
			$form['create_new_records'] = array(
				'#type' => 'select',
				'#title' => t('Create New Records?'),
				'#options' => array(
					'No' => 'No',
					'Yes' => 'Yes',
				),
				'#description' => t('If the primary key is not found, should a new record be created?'),
				'#default_value' => $data['create_new_records'],
			);
		}
	}

	if ($info['feed']['employer_access']) {
		$employer_node = sirius_employer_get();
		if (!$employer_node) {
			$employers = array(NULL => t('-- Select --')) + sirius_employer_list();
			$form['employer_nid'] = array(
				'#type' => 'select',
				'#options' => $employers,
				'#title' => t('Employer'),
				'#default_value' => $feed_node->field_grievance_shop['und'][0]['target_id'],
			);
		}
	}

	$tag_taxonomy = $info['feed']['tag_taxonomy'];
	if ($tag_taxonomy) { 
		$options = sirius_taxonomy_load($tag_taxonomy, 'tid', 'name', TRUE);
		$form['tag'] = array(
			'#type' => 'select',
			'#title' => 'Tag records with',
			'#options' => $options,
			'#default_value' => $data['tag'],
		);
	}


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
  	$data['origname'] = $file['origname'];
		$file['display'] = 1;
		unset($form_state['storage']["attach_$delta"]);
		unset($file['destination']);
		unset($file['uri']);
		$feed_node->field_sirius_attachments['und'][0] = $file;
  }
  $data['source'] = $form_state['values']['source'];
  $data['pk'] = $form_state['values']['pk'];
  $data['tag'] = $form_state['values']['tag'];
	$data['worksheet'] = 0;
	$data['header_row'] = $form_state['values']['header_row'];
	$data['create_new_records'] = $form_state['values']['create_new_records'];

	if ($info['feed']['employer_access']) {
		$employer_node = sirius_employer_get();
		if (!$employer_node) {
			$feed_node->field_grievance_shop['und'][0]['target_id'] = $form_state['values']['employer_nid'];
		}
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
	if (!$reader) {
		drupal_set_message("Unable to load the spreadsheet.", 'error');
		return array();
	}
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
	if (count($names) == 1) { 
		$form['worksheet']['#type'] = 'hidden';
		$form['worksheet']['#value'] = $names[0];
	}

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

function sirius_feed_wizard_fields($info, $data = NULL, $feed_node = NULL, $current_step = NULL) {
	if ($info['feed']['fields']) { return $info['feed']['fields']; }
	$callback = $info['feed']['callback_fields'];
	if ($callback) {
		$fields = $callback($info, $data, $feed_node, $current_step);
	}
	if (!$fields) { $fields = array(); }
	return $fields;
}

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

	if (!$sheet) { 
		drupal_set_message("Unable to load spreadsheet.", 'error');
		return array();
	}

	foreach ($sheet->getRowIterator() as $row) {
		if ($count == 0 && $data['header_row'] == 'Yes') { ++$count; continue; }
		$html_row = array();
		foreach ($header as $col => $ignore) { $html_row[] = sirius_feed_spout_get_cell($row, $col); }
		$html_rows[] = $html_row;
		++$count; 
		if ($count >= 10) { break; }
	}

	$fields = sirius_feed_wizard_fields($info, $data, $feed_node, $current_step);

	$form['table_open'] = array('#markup' => '<table><tr><th>System Field</th><th>Uploaded Field</th></tr>');
	foreach ($fields as $field_name => $field_info) {
		if ($data) {
			$default = $data["map_$field_name"];
		} else {
			$default = $last_data["map_$field_name"];
		}

		if ($field_info['required']) {
			$options = $header;
		} else {
			$options = array(NULL => '-- Not Mapped --') + $header;
		}

		$form["map_${field_name}_open"] = array('#markup' => '<tr>');

		$form["map_${field_name}_label"] = array('#markup' => '<td>' . $field_info['title'] . '</td>');

		$form["map_${field_name}"] = array(
			'#description' => $field_info['description'],
			'#type' => 'select',
			'#options' => $options,
			'#required' => $field_info['required'],
			'#default_value' => $default,
			'#prefix' => '<td>',
			'#suffix' => '</td>',
		);

		$form["map_${field_name}_close"] = array('#markup' => '</tr>');
	}
	$form['table_close'] = array('#markup' => '</table>');

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Submit',
	);

	$html = '<div style="width: 100%; height: 600px; overflow: scroll; border: 1px solid #333333; padding: 20px; margin-top: 20px;">';
	$html .= "<h2>Preview - First $count Rows</h2>";
	$html .= theme_table(array('rows' => $html_rows, 'header' => $header, 'attributes' => array()));
	$html .= '</div>';
	$form["preview"] = array('#markup' => $html);

	return $form;
}

function sirius_feed_wizard_map_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$feed_node = $form_state['feed_node'];
	$current_step = $form_state['current_step'];
	$fields = sirius_feed_wizard_fields($info, $data, $feed_node, $current_step);
	foreach ($fields as $field_name => $field_info) {
		$data["map_$field_name"] = $form_state['values']["map_$field_name"];
	}

	sirius_wizard_submit($info, $data, $feed_node, $current_step, "Worksheet selection saved.");
}

/************************************************************************************************
 * Step 3A: Tagging
 ************************************************************************************************/

/*
function sirius_feed_wizard_tag($info, $data, $feed_node, $current_step) { return drupal_get_form('sirius_feed_wizard_tag_form', $info, $data, $feed_node, $current_step); }

function sirius_feed_wizard_tag_form($form, &$form_state, $info, $data, $feed_node, $current_step) {
	$form_state['info'] = $info;
	$form_state['data'] = $data;
	$form_state['feed_node'] = $feed_node;
	$form_state['current_step'] = $current_step;
	$steps = sirius_wizard_steps($info, $data, $feed_node);
	$last_data = sirius_feed_lastdata($info, $feed_node);

	sirius_taxonomy_checkboxes($form, $form_state, 'whatever', 'sirius_contact_tags');

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Submit',
	);

	return $form;
}

function sirius_feed_wizard_tag_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$feed_node = $form_state['feed_node'];
	$current_step = $form_state['current_step'];
	$fields = array();
	foreach ($fields as $field_name => $field_info) {
		$data["map_$field_name"] = $form_state['values']["map_$field_name"];
	}

	sirius_wizard_submit($info, $data, $feed_node, $current_step, "Worksheet selection saved.");
}
*/

/************************************************************************************************
 * Step 4: Verify
 ************************************************************************************************/

function sirius_feed_wizard_verify($info, $data, $feed_node, $current_step) { return drupal_get_form('sirius_feed_wizard_verify_form', $info, $data, $feed_node, $current_step); }

function sirius_feed_wizard_verify_form($form, &$form_state, $info, $data, $feed_node, $current_step) {
	drupal_add_js(drupal_get_path('module', 'sirius') . '/js/table.js', 'file');
	$form_state['info'] = $info;
	$form_state['data'] = $data;
	$form_state['feed_node'] = $feed_node;
	$form_state['current_step'] = $current_step;

	if (!$info['steps'][$current_step]['hide_limit']) {
		$form['verify_limit'] = array(
			'#type' => 'textfield',
			'#size' => 5,
			'#title' => t('Records to Verify'),
			'#description' => t('For very large imports, it may take too long to verify all records. Select the maximum number of records which should be verified.'),
			'#required' => TRUE,
			'#default_value' => 1000,
		);
	} else {
		$form['verify_limit'] = array('#type' => 'value', '#value' => 1000);
	}

	if (!$info['steps'][$current_step]['hide_precompute']) {
		$form['precompute'] = array(
			'#type' => 'select',
			'#title' => t('Precompute?'),
			'#options' => array(
				'No' => t('No'),
				'Yes' => t('Yes'),
			),
			'#description' => t('"Precompute" performs a more in-depth verification. For example, it will ensure that referenced taxonomy terms exist or that IDs are unique. However, it is computationally expensive, so for large imports it may be better to omit this option.'),
			'#required' => TRUE,
			'#default_value' => 'Yes',
		);
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Submit',
	);

	if ($data['verify_html']) {
		$form['verify_html'] = array('#markup' => $data['verify_html']);
	}

	return $form;
}

function sirius_feed_wizard_verify_form_validate($form, &$form_state) {
	if (! ($form_state['values']['verify_limit'] > 0)) {
		form_set_error('verify_limit', 'Please make sure that at least one row is verified.');
	}
}

function sirius_feed_wizard_verify_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$feed_node = $form_state['feed_node'];
	$current_step = $form_state['current_step'];

	// Preview
	$reader = sirius_feed_spout_get_reader($feed_node);
	$sheet = sirius_feed_spout_get_sheet($reader, $data['worksheet']);
	if (!$sheet) { 
		drupal_set_message("Unable to load the spreadsheet", 'error');
		return array();
	}
	$header = sirius_feed_spout_get_header($sheet, $data['header_row']);

	$import_header = array('xxxrowcountxxx' => 'Row');
	$fields = sirius_feed_wizard_fields($info, $data, $feed_node, $current_step);
	foreach ($fields as $field_name => $field_info) {
		$col = $data["map_$field_name"];
		if ($col === NULL || $col === '') { continue; }
		$import_header[$field_name] = $field_info['title'] . '<br>(' . $header[$col] . ')';
	}
	$import_header['xxxsuccessxxx'] = 'Will Import';
	$import_header['xxxmsgxxx'] = 'Message';

	// Get our data
	$count = 0;
	$import_data = array();
	$counts = $warning_counts = array();
	$callback = $info['feed']['callback_row_verify'];
	$verify_limit = $form_state['values']['verify_limit'];
	$precompute = ($form_state['values']['precompute'] == 'Yes');
	foreach ($sheet->getRowIterator() as $row) {
		++$count;
		if ($count > $verify_limit+2) { continue; }
		if ($count == 1 && $data['header_row'] == 'Yes') { ++$count; continue; }
		$import_data_row = sirius_feed_spout_get_import_row($fields, $row, $data, $count, $header);

		$preview_data_row = array();
		foreach ($import_header as $field_name => $field_info) {
			$preview_data_row[$field_name] = $import_data_row[$field_name];
		}

		$result = $callback($feed_node, $info, $data, $import_data_row, $precompute);

		if ($result['success']) { $preview_data_row['xxxsuccessxxx'] = 'Yes'; } else { $preview_data_row['xxxsuccessxxx'] = 'No'; }
		$preview_data_row['xxxmsgxxx'] = $result['msg'];

		++$counts[$result['msg']];
		if ($counts[$result['msg']] == 6) { $preview_data_row['xxxmsgxxx'] .= ' (Subsequent messages of this type will be suppresssed.)'; }
		if ($counts[$result['msg']] <= 6) { $import_data[] = $preview_data_row; }

		if ($result['warnings']) {
			foreach ($result['warnings'] as $msg) {
				++$warning_counts[$msg];
			}
		}
	}
	$data['rows'] = $count + 1;

	$attributes = array('class' => array('sirius_table_align'));

	$html = "<h2>Verification Summary</h2>";
	$summary_rows = array();
	foreach ($counts as $key => $value) { $summary_rows[] = array($key, $value); }
	$html .= theme_table(array('rows' => $summary_rows, 'header' => array('Type', 'Count'), 'attributes' => $attributes));

	if ($warning_counts) {
		$html .= "<h2>Warnings</h2>";
		$summary_rows = array();
		foreach ($warning_counts as $key => $value) { $summary_rows[] = array($key, $value); }
		$html .= theme_table(array('rows' => $summary_rows, 'header' => array('Type', 'Count'), 'attributes' => $attributes));
	}

	$html .= "<h2>Records to Import</h2>";
	$html .= theme_table(array('rows' => $import_data, 'header' => $import_header, 'attributes' => array()));
	$data['verify_html'] = $html;
	sirius_wizard_submit($info, $data, $feed_node, $current_step, "Ready to Process.");
}

/************************************************************************************************
 * Step 5: Process
 ************************************************************************************************/

function sirius_feed_wizard_process($info, $data, $feed_node, $current_step) { return drupal_get_form('sirius_feed_wizard_process_form', $info, $data, $feed_node, $current_step); }

function sirius_feed_wizard_process_form($form, &$form_state, $info, $data, $feed_node, $current_step) {
	drupal_add_js(drupal_get_path('module', 'sirius') . '/js/table.js', 'file');

	$form_state['info'] = $info;
	$form_state['data'] = $data;
	$form_state['feed_node'] = $feed_node;
	$form_state['current_step'] = $current_step;

	if (!$info['steps'][$current_step]['hide_limit']) {
		$form['limit'] = array(
			'#type' => 'textfield',
			'#title' => t('Limit'),
			'#size' => 5,
			'#description' => t('Enter a number to process a limited number of batches, i.e. for testing. Leave blank to process the entire file.'),
		);
	}

	$submit_txt = $info['steps'][$current_step]['submit_txt'];
	if (!$submit_txt) { $submit_txt = 'Import Now'; }
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => $submit_txt,
	);

	if ($data['verify_html']) {
		$form['verify_html'] = array('#markup' => $data['verify_html']);
	}

	return $form;
}

function sirius_feed_wizard_process_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$feed_node = $form_state['feed_node'];
	$current_step = $form_state['current_step'];
	$limit = $form_state['values']['limit'];

	$batch_size = sirius_feed_batch_size($info['name']);
	$idx = 0;
	$rows = $data['rows'];
	$batch_count = 0;
	$data['process_ts'] = time();
	$operations = array();

	$callback_preprocess = $info['feed']['callback_preprocess'];
	if ($callback_preprocess) { 
		$operations[] = array($callback_preprocess, array($feed_node, $info, $data));
	}
	while ($idx < $rows) {
		$operations[] = array('sirius_feed_wizard_process_batch', array($feed_node, $info, $data, $idx));
		$idx += $batch_size + 1;
		++$batch_count;
		if ($limit && $batch_count >= $limit) { break; }
	}
	$callback_postprocess = $info['feed']['callback_postprocess'];
	if ($callback_postprocess) { 
		$operations[] = array($callback_postprocess, array($feed_node, $info, $data));
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
	$fields = sirius_feed_wizard_fields($info, $data, $feed_node, $current_step);

	if (!$context['results']['results_path']) {
		$context['results']['results_path'] = 'temporary://sirius_feed_results_' . rand();
		$results_fp = fopen(drupal_realpath($context['results']['results_path']), 'w');
		$cols = array_keys($fields);
		$cols[] = 'Message';
		$cols[] = 'Result';
		if ($info['feed']['results_fields']) {
			foreach ($info['feed']['results_fields'] as $field => $label) {
				$cols[] = $label;
			}
		}
		fputcsv($results_fp, $cols);

		if ($info['feed']['results_details']['active']) {
			$context['results']['details']['header'] = $cols;
			$context['results']['details']['success'] = array();
			$context['results']['details']['error'] = array();
		}
	} else {
		$results_fp = fopen(drupal_realpath($context['results']['results_path']), 'a');
	}

	// Get our data
	$count = 0;
	$import_data = array();
	$counts = $context['results']['counts'];
	$warning_counts = $context['results']['warning_counts'];
	$callback = $info['feed']['callback_row_process'];
	foreach ($sheet->getRowIterator() as $row) {
		if ($count == 0 && $data['header_row'] == 'Yes') { ++$count; continue; }
		++$count;
		if ($count < $idx) { continue; }
		if ($count > $idx+$batch_size) { break; }
		$import_data_row = sirius_feed_spout_get_import_row($fields, $row, $data, $count, $header);
		$result = $callback($feed_node, $info, $data, $import_data_row);

		++$counts[$result['msg']];

		if ($result['warnings']) {
	    $result['warnings'] = array_unique($result['warnings']);
	    foreach ($result['warnings'] as $warning_msg) {
	    	++$warning_counts[$warning_msg];
	    }
  	  $result['msg'] .= " (Warning: " . join(' / ', $result['warnings']) . ')'; 
  	}

		$results_row = array();
		foreach (array_keys($fields) as $field_name) {
			$results_row[] = $import_data_row[$field_name];
		}

		if ($result['success']) { 
			$results_row[] = 'Success';
		} else {
			$results_row[] = 'Error';
		}

		$results_row[] = $result['msg'];

		if ($info['feed']['results_fields']) {
			foreach ($info['feed']['results_fields'] as $field => $label) {
				$results_row[] = $result[$field];
			}
		}

		fputcsv($results_fp, $results_row);

		if ($info['feed']['results_details']['active']) {
			$error_limit = $info['feed']['results_details']['error_limit'];
			if (!$error_limit) { $error_limit = 1000; }
			$success_limit = $info['feed']['results_details']['success_limit'];
			if (!$success_limit) { $success_limit = 1000; }

			if ($result['success'] && count($context['results']['details']['success']) < $success_limit) { 
				$context['results']['details']['success'][] = $results_row;
			} else if (count($context['results']['details']['error']) < $error_limit) {
				$context['results']['details']['error'][] = $results_row;
			}
		}
	}
	$context['results']['counts'] = $counts;
	$context['results']['warning_counts'] = $warning_counts;

	$context['message'] = "Processing rows $idx to " . ($idx+$batch_size-1) . ".";
}

function sirius_feed_wizard_process_finish($feed_node, $info, $data, $idx, &$context) {
	$context['message'] = "Cleaning up.";
	$data['feed']['counts'] = $context['results']['counts'];
	if ($data['feed']['counts']) { ksort($data['feed']['counts']); }

	$data['feed']['warning_counts'] = $context['results']['warning_counts'];
	if ($data['feed']['warning_counts']) { ksort($data['feed']['warning_counts']); }

  $feed_node->field_sirius_feed_status['und'][0]['value'] = 'complete';

	$file = new stdClass;
	$file->uid = $user->uid;
	$file->uri = $context['results']['results_path'];
	$file->filemime = 'text/csv';
	$file->status = 1;
	$file->display = 1;
	file_save($file);
  $file = file_move($file, 'private://attach_' . rand() . '.csv');
  $feed_node->field_sirius_attachments['und'][1] = (array) $file;

  $ts = time();
  $elapsed = $ts - $data['process_ts'];

  $data['feed']['results_details'] = $context['results']['details'];

	sirius_wizard_submit($info, $data, $feed_node, 'process', "Import complete. Elapsed time: " .  format_interval($elapsed));
}

/************************************************************************************************
 * Step 6: Review
 ************************************************************************************************/

function sirius_feed_wizard_review($info, $data, $feed_node, $current_step) { return drupal_get_form('sirius_feed_wizard_review_form', $info, $data, $feed_node, $current_step); }

function sirius_feed_wizard_review_form($form, &$form_state, $info, $data, $feed_node, $current_step) {
	drupal_add_js(drupal_get_path('module', 'sirius') . '/js/table.js', 'file');

	$form_state['info'] = $info;
	$form_state['data'] = $data;
	$form_state['feed_node'] = $feed_node;
	$form_state['current_step'] = $current_step;

	$attributes = array('class' => array('sirius_table_align'));

	$summary_rows = array();
	foreach ($data['feed']['counts'] as $key => $value) { $summary_rows[] = array($key, $value); }
	$html = '<h2>' . t('Results Summary') . '</h2>';
	$html .= theme_table(array('rows' => $summary_rows, 'header' => array('Type', 'Count'), 'attributes' => $attributes));
	$form['review'] = array('#markup' => $html);

	if ($data['feed']['warning_counts']) {
		$summary_rows = array();
		foreach ($data['feed']['warning_counts'] as $key => $value) { $summary_rows[] = array($key, $value); }
		$html = '<h2>' . t('Warnings') . '</h2>';
		$html .= theme_table(array('rows' => $summary_rows, 'header' => array('Type', 'Count'), 'attributes' => $attributes));
		$form['review_warning'] = array('#markup' => $html);
	}

	if ($info['feed']['results_details']['active']) {
		$error_limit = $info['feed']['results_details']['error_limit'];
		if (!$error_limit) { $error_limit = 1000; }
		$success_limit = $info['feed']['results_details']['success_limit'];
		if (!$success_limit) { $success_limit = 1000; }

		if ($data['feed']['results_details']['error']) {
			$html = '<h2>' . t('Error Rows') . '</h2>';
			$html .= theme_table(array('header' => $data['feed']['results_details']['header'], 'rows' => $data['feed']['results_details']['error'], 'attributes' => $attributes));
			if (count($data['feed']['results_details']['error']) >= $error_limit - 1) {
				$html .= '<p><em>' . t('Only the first @limit rows are shown. Donwload the results file to see all rows.', array('@limit' => $error_limit)) . '</em></p>';
			}
			$form['error'] = array('#markup' => $html);
		}

		if ($data['feed']['results_details']['success']) {
			$html = '<h2>' . t('Success Rows') . '</h2>';
			$html .= theme_table(array('header' => $data['feed']['results_details']['header'], 'rows' => $data['feed']['results_details']['success'], 'attributes' => $attributes));
			if (count($data['feed']['results_details']['success']) >= $success_limit - 1) {
				$html .= '<p><em>' . t('Only the first @limit rows are shown. Donwload the results file to see all rows.', array('@limit' => $success_limit)) . '</em></p>';
			}
			$form['success'] = array('#markup' => $html);
		}
	}

	$html = '<h2>Download Results</h2>';
	$html .= '<p>' . t('The first file below contains the original upload data. The second contains the full set of results.') . '</p>';
	$renderable = field_view_field('node', $feed_node, 'field_sirius_attachments', 'full');
	$html .= drupal_render($renderable);
	$form['attachments'] = array('#markup' => $html);

	return $form;
}

function sirius_feed_batch_size($key) {
	$size = variable_get('sirius_feed_batch_size_' . $key);
	if (!$size) { 
		$info = sirius_wizard_info($key);
		$size = $info['feed']['batch_size'];
	}
	if (!$size) { $size = 10; }
	return $size;
}

function sirius_feed_admin() {
	$info_all = sirius_wizard_info_all();
	foreach ($info_all as $key => $info) {
		if (!$info['feed']) { continue; }
		$variable_name = 'sirius_feed_batch_size_' . $key;
		$form[$variable_name] = array(
			'#type' => 'textfield',
			'#default_value' => sirius_feed_batch_size($key),
			'#title' => $info['title'],
			'#description' => $info['description'],
		);
	}

  return system_settings_form($form);
}

function sirius_feed_lookup_by_field($content_type, $table, $field_name, $value) {
	$sql_args = array();
  $sql = "select max(nid) as a, min(nid) as b from node ";
  $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
  $sql .= "left join $table on $table.entity_type = 'node' and $table.entity_id = node.nid ";
  $sql .= "where node.type = :content_type ";
  $sql_args[':content_type'] = $content_type;
  $sql .= "and $field_name = :value ";

  $domain_nid = sirius_domain_nid(); 
  if ($domain_nid) {
    $sql .= "and field_sirius_domain_target_id = :domain_nid ";
    $sql_args[':domain_nid'] = $domain_nid;
  } else {
    $sql .= "and field_sirius_domain_target_id IS NULL ";
  }
  $sql_args[':value'] = $value;
  $stmt = sirius_sql_query($sql, $sql_args);
  $hr = $stmt->fetchAssoc();
  $nid = $hr['a'];
  $other_nid = $hr['b'];
  if (!$nid) { return array('success' => FALSE, 'not_found' => TRUE, 'msg' => "Record not found with this ID ($table, $field_name)."); }
  if ($nid != $other_nid) { return array('success' => FALSE, 'msg' => "Multiple entities found with this ID ($table, $field_name)."); }
  return array('success' => TRUE, 'nid' => $nid);
}

function sirius_feed_handle_list($field_name, $value) {
  $structure_info = field_info_field("field_$field_name");
  $allowed_values = list_allowed_values($structure_info);
  $allowed_values_flip = array_flip($allowed_values);

  // Match case insensitive
  foreach ($allowed_values as $a => $b) {
    if (trim(strtolower($a)) == trim(strtolower($value))) { return $a; }
    if (trim(strtolower($b)) == trim(strtolower($value))) { return $a; }

    if (trim(strtolower($value)) == 'yes') {
      if (trim(strtolower($a)) == 'y') { return $a; }
    }

    if (trim(strtolower($value)) == 'no') {
      if (trim(strtolower($a)) == 'n') { return $a; }
    }
  }

  return NULL;
}

//
// @todo: Under development. Make sure these are domain-specific. Get sirius_employer working with them properly.
// 

function sirius_feed_handle_date($field_name, $value) {
  $ts = strtotime($value);
  if (!$ts) { return NULL; }
  return gmdate('Y-m-d', $ts);
}

function sirius_feed_handle_entity_reference($entity_type, $value) {
  $sql = "select min(nid) as mn, max(nid) as mx from node ";
  $sql .= "left join field_data_field_sirius_id on field_data_field_sirius_id.entity_type = 'node' and field_data_field_sirius_id.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_name_short on field_data_field_sirius_name_short.entity_type = 'node' and field_data_field_sirius_name_short.entity_id = node.nid ";
  $sql .= "where nid = :v1 ";
  $sql .= "or title = :v2 ";
  $sql .= "or field_sirius_id_value = :v3 ";
  $sql .= "or field_sirius_name_short_value = :v4 ";
  $stmt = sirius_sql_query($sql, array(':v1' => $value, ':v2' => $value, ':v3' => $value, ':v4' => $value));
  $hr = $stmt->fetchAssoc();
  if (!$hr['mn']) { return NULL; }
  if ($hr['mn'] != $hr['mx']) { return NULL; }
  return $hr['mn'];
}

function sirius_feed_handle_pk_nid($content_type, $nid) {
  if (!$nid) { 
  	return array('success' => FALSE, 'msg' => 'If the primary key is NID, all rows must have a node ID.');
  }

  $sql = "select title, type from node where nid = :nid";
  $stmt = sirius_sql_query($sql, array(':nid' => $nid));
  $hr = $stmt->fetchAssoc();
  if (!$hr['type']) {
		return array('success' => FALSE, 'msg' => 'No node was found with this NID.');
	}
	if ($hr['type'] != $content_type) {
  	return array('success' => FALSE, 'msg' => 'The node for this NID was found, but it is not an employer.');
  }

  return array(
  	'sucess' => TRUE,
  	'msg' => 'Found node: ' . $hr['title'],
  	'pk' => $nid
  );
}

function sirius_feed_info_set_titles(&$item) {
  foreach ($item['feed']['fields'] as $field_name => $field_info) {
    $group = $field_info['group'];
    $group_name = $item['feed']['groups'][$group];

    if (!$field_info['title']) {
      $structure_info = field_info_instance('node', "field_$field_name", $item['feed']['content_type']);
      if ($structure_info) { $item['feed']['fields'][$field_name]['title'] = $structure_info['label']; }
    }

    $item['feed']['fields'][$field_name]['title'] = 
      $group_name . ': ' . $item['feed']['fields'][$field_name]['title'];
  }
}