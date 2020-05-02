<?php

function grievance_node_tab_merge($grievance_node) {
	return drupal_get_form('grievance_merge_form', $grievance_node);
}

function grievance_merge_form($form, &$form_state, $grievance_node) {
	$form_state['grievance_node'] = $grievance_node;

	$form['intro'] = array(
		'#type' => 'markup',
		'#markup' => t('Use the form below to merge other grievnaces into this one.  The other grievances will cease to exist, and their log entrie and attachments will be moved to this grievance.')
	);

	$form['merge_ids'] = array(
		'#type' => 'textarea',
		'#title' => t('Grievances to merge'),
		'#description' => t('Enter the Grievance IDs of the grievances you want to merge, one per line.'),
		'#required' => TRUE,
	);

	$form['confirm'] = array(
		'#type' => 'select',
		'#title' => t('Confirm'),
		'#options' => array('Yes' => 'Yes, permanently and irrevocably merge these grievances'),
		'#required' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit', 
		'#value' => 'Merge grievances now',
	);

	return $form;
}

function grievance_merge_form_submit($form, &$form_state) {
	// Make sure we have our main node
	$grievance_node = $form_state['grievance_node'];
	$grievance_nid = $grievance_node->nid;
	$grievance_id = $grievance_node->field_grievance_id['und'][0]['value'];

	global $user;

	// Need to have configured a "merge type" for the log
	$log_type_tid = sirius_get_special_tid("grievance_log_types|merge");
	if (!$log_type_tid) {
		drupal_set_message('Please configure a log entry "Merge" type on the "Special log types" screen.', 'error');
		$form_state['rebuild'] = TRUE;
		return;
	}

	// Load the nodes to merge
	$merge_ids = preg_split('/[,\s]+/', $form_state['values']['merge_ids']);
	if (count($merge_ids) > 10) {
		drupal_set_message("Only 10 grievances can be merged at a time.", 'error');
		$form_state['rebuild'] = TRUE;
		return;
	}
	$merge_nodes = array();
	foreach ($merge_ids as $merge_id) {
		if ($merge_id == $grievance_id) {
			drupal_set_message("You can't merge a grievance into itself.", 'error');
			$form_state['rebuild'] = TRUE;
			return;
		}
		$merge_node = grievance_fetch_by_id($merge_id);
		if (!$merge_node) {
			drupal_set_message("Unable to load grievance #$merge_id.", 'error');
			$form_state['rebuild'] = TRUE;
			return;
		}
		$merge_nodes[$merge_id] = $merge_node;
	}

	// Merge each.
	foreach ($merge_nodes as $merge_id => $merge_node) {
		$merge_nid = $merge_node->nid;

		// Render
		$renderable = node_view($merge_node);
		unset($renderable['#contextual_links']);
		$render_html = drupal_render($renderable);
		$render_text = "On " . date('Y-m-d H:i:s') . " the grievance below was merged.\n\n";
		$render_text .= "+++ Merged $merge_id (internal ID $merge_nid) +++ \n+++ ==> INTO ==> +++\n+++ $grievance_id / $grievance_nid +++\n\n";
		$render_text .= drupal_html_to_text($render_html);

		// Log the merge node as a simple print_r
	  $log_node = new stdClass();
    $log_node->title = t('Merged: Grievance #' . $merge_id);
	  $log_node->type = 'grievance_log';
	  node_object_prepare($log_node);
	  $log_node->language = LANGUAGE_NONE;
	  $log_node->uid = $user->uid; 
	  $log_node->status = 1;
	  $log_node->promote = 0;
	  $log_node->comment = 0;
	  $log_node->field_grievance['und'][0]['target_id'] = $grievance_nid;
	  $log_node->field_grievance_log_type['und'][0]['tid'] = $log_type_tid;
    $log_node->field_grievance_status_date['und'][0]['value'] = date('Y-m-d H:i:s');
	  $log_node = node_submit($log_node);
	  $log_node->body['und'][0]['value'] = $render_text;
	  node_save($log_node);

	  // Update the log entries
	  $sql = "update field_data_field_grievance set field_grievance_target_id = :grievance_nid where field_grievance_target_id = :merge_nid";
	  $stmt = db_query($sql, array(':grievance_nid' => $grievance_nid, ':merge_nid' => $merge_nid));
	  $sql = "update field_revision_field_grievance set field_grievance_target_id = :grievance_nid where field_grievance_target_id = :merge_nid";
	  $stmt = db_query($sql, array(':grievance_nid' => $grievance_nid, ':merge_nid' => $merge_nid));

	  // Delete the merge
	  node_delete($merge_nid);

		drupal_set_message("Success!  Merged $merge_id into $grievance_id.");
	}
}
