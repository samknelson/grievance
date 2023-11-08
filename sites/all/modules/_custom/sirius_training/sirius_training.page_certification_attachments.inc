<?php

function sirius_training_node_tab_certification_attachments_access($certification_node) {
	// Only log entries of type certification
	if ($certification_node->field_sirius_category['und'][0]['value'] != 'worker:certification') { return FALSE; }

	// Admin can access
	if (user_access('sirius training admin')) { return TRUE; }
	if (user_access('sirius training certification edit')) { return TRUE; }

	// Author can access
	global $user;
	$status = $certification_node->field_sirius_type['und'][0]['value'];
	if ($user->uid == $certification_node->uid) { return TRUE; }

	// Oh well
	return FALSE;
}

function sirius_training_node_tab_certification_attachments($certification_node) {
	return drupal_get_form('sirius_training_node_tab_certification_attachments_form', $certification_node);
}

function sirius_training_node_tab_certification_attachments_form($form, &$form_state, $certification_node) {
	$form_state['certification_nid'] = $certification_node->nid;

	if ($certification_node->field_sirius_attachments['und'][0]['fid']) {
		foreach ($certification_node->field_sirius_attachments['und'] as $delta => $tmp) {
			$form["current_$delta"] = array(
				'#type' => 'fieldset',
				'#title' => 'Attachment #' . ($delta+1),
			);


			$renderable = field_view_value('node', $certification_node, 'field_sirius_attachments', $certification_node->field_sirius_attachments['und'][$delta]);
			$form["current_$delta"]["display_$delta"] = array('#markup' => drupal_render($renderable));
			$form["current_$delta"]["delete_$delta"] = array(
				'#type' => 'checkbox',
				'#title' => 'Delete This Attachment',
			);
		}
	}

	$form["new"] = array(
		'#type' => 'fieldset',
		'#title' => t('Upload a New Attachment'),
	);

	$form["new"]["description"] = array(
		'#type' => 'textfield',
		'#title' => t('Description'),
	);

	$form["new"]["attach"] = array(
		'#type' => 'file',
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Submit',
	);

	return $form;
}

function sirius_training_node_tab_certification_attachments_form_validate($form, &$form_state) {
	$info = field_info_instance('node', 'field_sirius_attachments', 'sirius_log');
	$extensions = $info['settings']['file_extensions'];
  $file = file_save_upload("attach", array(
    'file_validate_extensions' => array($extensions),
  ));
  if ($file) {
    $name = rand();
    if ($file = file_move($file, 'private://attach_' . rand())) {
      $form_state['storage']["attach"] = $file;
    }
    else {
      form_set_error("attach", t("Failed to write the uploaded file to the site's file folder."));
    }

    /*
    if (!$form_state['values']["description"]) {
    	form_set_error("description", t("Please enter a description for all uploaded files."));	
    }
    */
  }
}

function sirius_training_node_tab_certification_attachments_form_submit($form, &$form_state) {
	$certification_node = node_load($form_state['certification_nid']);
	$files = $certification_node->field_sirius_attachments['und'];

  $file = $form_state['storage']["attach"];
  if ($file) {
		$file = (array) $file;
		$file['display'] = 1;
		unset($form_state['storage']["attach"]);
		unset($file['destination']);
		unset($file['uri']);
		$file['description'] = $form_state['values']["description"];
		$files[] = $file;
	}

	$to_delete = array();
	foreach ($form_state['values'] as $name => $value) {
		if ($value != 1) { continue; }
		if (!preg_match('/^delete_(\d+)$/', $name, $matches)) { continue; }
		$delta = $matches[1];
		$to_delete[] = $delta;
	}

	if ($to_delete) {
		$new_files = array();
		foreach ($files as $delta => $file) {
			if (!in_array($delta, $to_delete)) {
				$new_files[] = $file; 
			}
		}
		$files = $new_files;
	}

	$certification_node->field_sirius_attachments['und'] = $files;
	node_save($certification_node);
	drupal_set_message(sirius_minilog_render_html());
}
