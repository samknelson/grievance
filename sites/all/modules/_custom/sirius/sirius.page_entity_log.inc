<?php

function sirius_node_tab_entity_log($node) {
	return drupal_get_form('sirius_node_tab_entity_log_form', $node);
}

function sirius_node_tab_entity_log_form($form, &$form_state, $node) {
	$form_state['node'] = $node;

  $category_type_options = sirius_log_quicklog_category_type_options($node->type);

  $form['category_type'] = array(
    '#type' => 'select',
    '#options' => $category_type_options,
    '#required' => TRUE,
    '#title' => t('Type of Log Entry'),
  );

	$form['reason'] = array(
		'#type' => 'textarea',
		'#title' => t('Comment'),
		'#required' => TRUE,
	);

	for ($delta=0; $delta<3; ++$delta) {
		$form["attach_$delta"] = array(
			'#type' => 'file',
			'#title' => t('Attachment #') . ($delta + 1),
		);
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Submit',
	);

	$nids = array($node->nid);
	if ($node->type == 'sirius_worker') {
		if ($node->field_sirius_contact['und'][0]['target_id']) { $nids[] = $node->field_sirius_contact['und'][0]['target_id']; }
	}

	$form += sirius_log_render_form(join('+', $nids));

	return $form;
}

function sirius_node_tab_entity_log_form_validate($form, &$form_state) {
	$info = field_info_instance('node', 'field_sirius_attachments', 'sirius_log');
	$extensions = $info['settings']['file_extensions'];
	for ($delta = 0; $delta < 3; ++$delta) {
	  $file = file_save_upload("attach_$delta", array(
	    'file_validate_extensions' => array($extensions),
	  ));
	  if ($file) {
	    $name = rand();
	    if ($file = file_move($file, 'private://attach_' . rand())) {
	      $form_state['storage']["attach_$delta"] = $file;
	    }
	    else {
	      form_set_error("attach_$delta", t("Failed to write the uploaded file to the site's file folder."));
	    }
	  }
	}
}

function sirius_node_tab_entity_log_form_submit($form, &$form_state) {
	$files = array();
	for ($delta = 0; $delta < 3; ++$delta) {
	  $file = $form_state['storage']["attach_$delta"];
	  if ($file) {
  		$file = (array) $file;
  		$file['display'] = 1;
			unset($form_state['storage']["attach_$delta"]);
  		unset($file['destination']);
  		unset($file['uri']);
  		$files[] = $file;
  	}
  }

	$node = $form_state['node'];

	$comment = $form_state['values']['reason'];

  list ($category, $type) = explode('|', $form_state['values']['category_type']);
	sirius_log($category, 'comment', $comment, $comment, NULL, array($node->nid), $files);
	drupal_set_message(t('Comment added.'));
}