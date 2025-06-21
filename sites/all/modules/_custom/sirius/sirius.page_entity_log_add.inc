<?php

function sirius_node_tab_entity_log_add_form($form, &$form_state, $node, $log_bucket_id) {
  $form_state['nid'] = $node->nid;
	$form_state['log_bucket_id'] = $log_bucket_id;

	$log_bucket = sirius_log_bucket($log_bucket_id);
  $form['quicknav'] = array('#markup' => sirius_node_tab_entity_log_quicknav($log_bucket));

  $ct_options = sirius_log_quicklog_category_type_options($node->type);
  $ct_tree = array();
  foreach ($ct_options as $ct => $title) {
  	list ($category, $type) = explode('|', $ct);
  	$ct_tree[$category][$type] = $title;
  }

	// PHP version crap
	if (!function_exists('array_key_first')) {
    function array_key_first(array $arr) {
      foreach ($arr as $key => $unused) {
        return $key;
      }
      return null;
    }
	}		

	$category_options = array_combine(array_keys($ct_tree), array_keys($ct_tree));
	$form['category'] = array(
		'#type' => 'select',
		'#title' => t('Category'),
		'#required' => TRUE,
		'#options' => $category_options,
		'#default_value' => $category_options[array_key_first($category_options)],
	);


	foreach ($ct_tree as $category => $type_options) {

		$form["type_$category"] = array(
			'#type' => 'select',
			'#title' => t('Type'),
			'#required' => TRUE,
			'#options' => $type_options,
			'#default_value' => $type_options[array_key_first($type_options)],
			'#states' => array('visible' => array(array(":input[name=\"category\"]" => array('value' => $category)))),
		);
	}

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

	return $form;
}

function sirius_node_tab_entity_log_add_form_validate($form, &$form_state) {
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

function sirius_node_tab_entity_log_add_form_submit($form, &$form_state) {
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

  $node = node_load($form_state['nid']);
	$comment = $form_state['values']['reason'];
	$category = $form_state['values']['category'];
	$type = $form_state['values']["type_$category"];
	sirius_log($category, $type, $comment, $comment, NULL, array($node->nid), $files);
  drupal_set_message("Log entry created: $category, $type.");
}