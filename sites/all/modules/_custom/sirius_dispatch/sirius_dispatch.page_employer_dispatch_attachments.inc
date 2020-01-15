<?php

function sirius_node_tab_employer_dispatch_attachments_access($employer_node) {
	return sirius_employer_node_tab_employer_users_access($employer_node);
}

function sirius_node_tab_employer_dispatch_attachments($employer_node) {
	return drupal_get_form('sirius_node_tab_employer_dispatch_attachments_form', $employer_node);
}

function sirius_node_tab_employer_dispatch_attachments_form($form, &$form_state, $employer_node) {
	$form_state['employer_nid'] = $employer_node->nid;

  drupal_add_library('system', 'ui.accordion');
  drupal_add_js('jQuery(document).ready(function () { jQuery("#sirius_accordion").accordion({active: false, collapsible: true, autoehight: true, heightStyle: "content"}); });', 'inline');

	$json = json_decode($employer_node->field_sirius_json['und'][0]['value'], TRUE);
	$attachments_by_fid = array();
	if ($employer_node->field_sirius_attachments['und']) {
		foreach ($employer_node->field_sirius_attachments['und'] as $tmp) {
			$attachments_by_fid[$tmp['fid']] = $tmp;
		}
	}

	$form['#tree'] = TRUE;

  $form['accordion_open'] = array('#markup' => '<div id="sirius_accordion">');

	if ($json['dispatch']['job_files']) {
		foreach ($json['dispatch']['job_files'] as $entry) {
			$fid = $entry['fid'];
			$file = $attachments_by_fid[$fid];
			if (!$file) { continue; } // The file must have been deleted elsewhere. Oh well.

			$title = $file['description'];
			if (!$title) { $title = $file['filename']; }

		  $form['current'][$fid]['accordion_open'] = array('#markup' => '<div class="sirius_accordion_label">' . $title . '</div><div class="sirius_accordion_body">');
			$renderable = field_view_value('node', $employer_node, 'field_sirius_attachments', $file, 'full');
			$form['current'][$fid]['intro'] = array('#markup' => drupal_render($renderable));
			$form['current'][$fid]['description'] = array(
				'#type' => 'textfield',
				'#title' => 'New Description',
				'#default_value' => $file['description'],
			);
			$form['current'][$fid]['action'] = array(
				'#type' => 'select',
				'#title' => 'Action',
				'#options' => array(
					'keep' => t('Keep'),
					'delete' => t('Delete'),
				),
				'#default_value' => 'keep',
			);
		  $form['current'][$fid]['accordion_close'] = array('#markup' => '</div>');
		}
	}

  $form['accordion_open_new'] = array('#markup' => '<div class="sirius_accordion_label">' . t('New Attachment') . '</div><div class="sirius_accordion_body">');

	$form['file'] = array(
		'#type' => 'file',
		'#title' => t('Upload'),
	);

	$form['description'] = array(
		'#type' => 'textfield',
		'#title' => t('Description'),
	);

  $form['accordion_close_new'] = array('#markup' => '</div>');

  $form['accordion_close'] = array('#markup' => '</div>');

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}


function sirius_node_tab_employer_dispatch_attachments_form_validate($form, &$form_state) {
	$info = field_info_instance('node', 'field_sirius_attachments', 'grievance_shop');
	$extensions = $info['settings']['file_extensions'];
  $file = file_save_upload('file', array('file_validate_extensions' => array($extensions)));
  if ($file) {
    $name = rand();
    if ($file = file_move($file, 'private://attach_' . rand())) {
      $form_state['storage']['file'] = $file;
    }
    else {
      form_set_error('file', t("Failed to write the uploaded file to the site's file folder."));
    }
  }
}

function sirius_node_tab_employer_dispatch_attachments_form_submit($form, &$form_state) {
	$employer_node = node_load($form_state['employer_nid']);
	$json = json_decode($employer_node->field_sirius_json['und'][0]['value'], TRUE);
	$attachments_by_fid = array();
	if ($employer_node->field_sirius_attachments['und']) {
		foreach ($employer_node->field_sirius_attachments['und'] as &$tmp) {
			$attachments_by_fid[$tmp['fid']] = &$tmp;
		}
	}

	// Handle any existing files
	$to_delete = array();
	if ($form_state['values']['current']) {
		$json['values']['current'] = array();
		foreach ($form_state['values']['current'] as $fid => $entry) {
			$attachments_by_fid[$fid]['description'] = $entry['description'];
			if ($entry['action'] == 'delete') {
				$to_delete[] = $fid;
			} else {
				$json['values']['current'][$fid] = $entry;
			}
		}
	}
	if ($to_delete) {
		$new_attachments = array();
		foreach ($employer_node->field_sirius_attachments['und'] as &$tmp) {
			if (!in_array($tmp['fid'], $to_delete)) { $new_attachments[] = $tmp; }
		}
		$employer_node->field_sirius_attachments['und'] = $new_attachments;
	}

	// Handle the new file
  $file = $form_state['storage']['file'];
  if ($file) {
		$file = (array) $file;
		$file['display'] = 1;
		$file['description'] = $form_state['values']['description'];
		unset($form_state['storage']['file']);
		unset($file['destination']);
		unset($file['uri']);
		$employer_node->field_sirius_attachments['und'][] = $file;
		$json['dispatch']['job_files'][] = array(
			'fid' => $file['fid'],
		);
	}

	$employer_node->field_sirius_json['und'][0]['value'] = json_encode($json);
	node_save($employer_node);
}