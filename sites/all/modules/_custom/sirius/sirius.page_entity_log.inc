<?php

function sirius_node_tab_entity_log($node) {

	return drupal_get_form('sirius_node_tab_entity_log_form', $node);
}

function sirius_node_tab_entity_log_form($form, &$form_state, $node) {
	$form_state['node'] = $node;

	$form['intro'] = array(
		'#type' => 'markup',
		'#markup' => '<div class="sirius_form_intro">' . 
			t('Use the form below to add a new comment to the log for this entity.') . 
			'</div>',

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

	$form += sirius_log_render_form($node->nid);

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
	$category = 'comment';

	$category = $node->type;
	$category = preg_replace('/sirius_/', '', $category);
	$category .= ':manual';
	$type = 'comment';
	sirius_log($category, 'comment', $comment, $comment, NULL, array($node->nid), $files);
	drupal_set_message(t('Comment added.'));
}