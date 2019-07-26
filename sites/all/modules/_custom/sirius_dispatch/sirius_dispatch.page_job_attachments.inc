<?php

function sirius_node_tab_dispatch_job_attachments_access($node) {
	// Staff can view any job
	if (user_access('sirius dispatch staff')) { return TRUE; }

	// Employer can view only their own shop's jobs.
	global $user;
	$user_full = user_load($user->uid);
	if ($node->field_grievance_shop['und'][0]['target_id'] && 
		($user_full->field_grievance_shops['und'][0]['target_id'] == $node->field_grievance_shop['und'][0]['target_id'])) {
		return TRUE;
	}

	// Oh well.
	return FALSE;
}

function sirius_node_tab_dispatch_job_attachments($node) {
	return drupal_get_form('sirius_node_tab_dispatch_job_attachments_form', $node);
}

function sirius_node_tab_dispatch_job_attachments_form($form, &$form_state, $node) {
	$form_state['node'] = $node;

	if ($node->field_sirius_attachments['und'][0]['fid']) {
		foreach ($node->field_sirius_attachments['und'] as $delta => $tmp) {
			$form["current_$delta"] = array(
				'#type' => 'fieldset',
				'#title' => 'Attachment #' . ($delta+1),
			);


			$renderable = field_view_value('node', $node, 'field_sirius_attachments', $node->field_sirius_attachments['und'][$delta]);
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

function sirius_node_tab_dispatch_job_attachments_form_validate($form, &$form_state) {
	$info = field_info_instance('node', 'field_sirius_attachments', 'sirius_dispatch_job');
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

    if (!$form_state['values']["description"]) {
    	form_set_error("description", t("Please enter a description for all uploaded files."));	
    }
  }
}

function sirius_node_tab_dispatch_job_attachments_form_submit($form, &$form_state) {
	$node = $form_state['node'];
	$files = $node->field_sirius_attachments['und'];

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

	$node->field_sirius_attachments['und'] = $files;
	node_save($node);
}


function sirius_dispatch_job_attachment_file_download_access($file_item, $job_node) {
	// Don't run if we already have "view" permission on the node.
	if (node_access('view', $job_node)) { return; }

	// We're only interested in files that are in the "Attachments" field. This is dumb, since at the moment that's the only place that files
	// might live. But we might as well be thorough.
	$fids = sirius_fieldvals($job_node, 'field_sirius_attachments', 'fid');
	$fid = $file_item['fid'];
	if (!$fid || !in_array($fid, $fids)) { return; }

	// The employer has access
	if (sirius_node_tab_dispatch_job_manage_access($job_node)) { return TRUE; }

	// Workers have access, but only if they have an active dispatch for this job.
	$worker_node = sirius_worker_get();
	if ($worker_node) {
	  $query = new EntityFieldQuery;
	  $query->entityCondition('entity_type', 'node');
	  $query->entityCondition('bundle', 'sirius_dispatch');
	  $query->fieldCondition('field_sirius_dispatch_job', 'target_id', $job_node->nid);
	  $query->fieldCondition('field_sirius_worker', 'target_id', $worker_node->nid);
	  $query->fieldCondition('field_sirius_dispatch_status', 'value', array('accepted', 'accepted_secondary', 'bullpen', 'resigned', 'terminated'), 'in');
	  $stmt = $query->execute();
	  if ($stmt['node']) { return TRUE; }
	}

	// If you're not signed in, but you have an AAT for an accepted dispatch, you have access. Using some slightly cumbersome code to ensure that 
	// we don't get an empty string into the all_aats array. There's probably a tidier way to do this.
	$all_aats = array();
	if ($_REQUEST['aat']) { $all_aats[] = $_REQUEST['aat']; }
	if ($_SESSION['aat']) { 
		foreach ($_SESSION['aat'] as $key => $value) {
			if ($key && $value) { $all_aats[] = $key; }
		}
	}
	if ($all_aats) {
	  $query = new EntityFieldQuery;
	  $query->entityCondition('entity_type', 'node');
	  $query->entityCondition('bundle', 'sirius_dispatch');
	  $query->fieldCondition('field_sirius_dispatch_job', 'target_id', $job_node->nid);
	  $query->fieldCondition('field_sirius_aat', 'value', $all_aats, 'in');
	  $query->fieldCondition('field_sirius_dispatch_status', 'value', array('accepted', 'accepted_secondary', 'bullpen', 'resigned', 'terminated'), 'in');
	  $stmt = $query->execute();
	  if ($stmt['node']) { return TRUE; }
	}
}
