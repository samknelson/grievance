<?php

function sirius_page_taxonomy_clone_access($vocabulary_machine_name) {
	if (!sirius_domain_taxonomy_is_multidomain($vocabulary_machine_name)) { return FALSE; }
	return sirius_page_taxonomy_access($vocabulary_machine_name);
}

function sirius_page_taxonomy_clone($vocabulary_machine_name) {
	return drupal_get_form('sirius_page_taxonomy_clone_form', $vocabulary_machine_name);
}

function sirius_page_taxonomy_clone_form($form, &$form_state, $vocabulary_machine_name) {
	$form_state['vocabulary_machine_name'] = $vocabulary_machine_name;

	$domain_node = sirius_domain();
	if ($domain_node) {
		$parent_domain_node = node_load($domain_node->field_sirius_domain['und'][0]['target_id']);
	}

	$form['action'] = array(
		'#title' => t('Action'),
		'#type' => 'select',
		'#options' => array(
			NULL => t('-- Select --'),
		),
		'#required' => TRUE,
	);

	if ($parent_domain_node) {
		$form['action']['#options']['from_parent'] = t('FROM PARENT: Copy the set of terms FROM the domain @title', array('@title' => $parent_domain_node->title));
	}


	$form['action']['#options']['to_domain'] = t('TO DOMAIN: Copy the set of terms TO a specific domain.');
	$options = array(NULL => t('-- Select --'));
	if (user_access('sirius admin')) {
		$options += sirius_domain_tree_render_as_select();
	} else {
		$options += sirius_domain_tree_render_as_select($domain_node->nid);
	}
	$form['target_domain_nid'] = array(
		'#title' => 'Domain',
		'#type' => 'select',
		'#options' => $options,
    '#states' => array(
      'visible' => array(
        array(':input[name="action"]' => array('value' => 'to_domain')),
      )
    ),
	);

	$form['from_parent'] = array(
		'#title' => 'Confirmation - Yes,  perform the action above. This action cannot be undone.',
		'#type' => 'checkbox',
		'#required' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Submit',
	);

  $log = sirius_minilog_render_html();
  if ($log) { $form['results'] = array('#markup' => '<hr>' . $log); }

	return $form;
}

function sirius_page_taxonomy_clone_form_submit($form, &$form_state) {
	$vocabulary_machine_name = $form_state['vocabulary_machine_name'];
	$action = $form_state['values']['action'];

	if ($action == 'from_parent') {
		// Target domain
		$target_domain_node = sirius_domain();
		$target_domain_nid = $target_domain_node->nid;
		if (!$target_domain_nid) {
			drupal_set_message('Unable to clone from parent: no domain.', 'error');
			return;
		}

		// Source domain
		$source_domain_nid = $target_domain_node->field_sirius_domain['und'][0]['target_id'];
		if (!$source_domain_nid) { 
			drupal_set_message('Unable to clone from parent: root-level domain (no parent).', 'error');
			return;
		}
	}

	if ($action == 'to_domain') {
		// Source domain
		$source_domain_node = sirius_domain();
		$source_domain_nid = $source_domain_node->nid;
		if (!$source_domain_nid) {
			drupal_set_message("Unable to clone from this tree: no domain.");
			return;
		}

		// Target domain
		$target_domain_nid = $form_state['values']['target_domain_nid'];
		if (!$target_domain_nid) { 
			drupal_set_message('No target domain selected.', 'error');
			return; 
		}
		if ($target_domain_nid == $source_domain_nid) {
			drupal_set_message('Cannot copy a domain to itself.', 'error');
			return; 
		}
		$target_domain_node = node_load($target_domain_nid);
	}

	$result = sirius_domain_clone_taxonomy($vocabulary_machine_name, $source_domain_nid, $target_domain_nid);

	if ($result['success']) {
		drupal_set_message($result['msg']);
	} else {
		drupal_set_message($result['msg'], 'error');
	}
	drupal_set_message("Cloned $vocabulary_machine_name FROM " . $source_domain_node->title . " TO " . $target_domain_node->title);
  $form_state['rebuild'] = TRUE;
}

function sirius_domain_get_target_tid($domain_nid, $source_tid, $reset = FALSE) {
	$data = &drupal_static(__FUNCTION__);
  if (!isset($data)) { $data = array(); }
  if ($reset) { $data = array(); }

  // drupal_set_message("Looking up: $domain_nid, $source_tid");
  if (!isset($data[$domain_nid][$source_tid])) {
  	// drupal_set_message("... Querying.");
		$sql = "select tid from taxonomy_term_data ";
		$sql .= "join field_data_field_sirius_term_source on field_data_field_sirius_term_source.entity_type = 'taxonomy_term' and field_data_field_sirius_term_source.entity_id = taxonomy_term_data.tid ";
		$sql .= "join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'taxonomy_term' and field_data_field_sirius_domain.entity_id = taxonomy_term_data.tid ";
		$sql .= "where field_sirius_domain_target_id = :domain_nid ";
		$sql .= "and field_sirius_term_source_value = :source_tid ";
		$sql_args = array();
		$sql_args[':domain_nid'] = $domain_nid;
		$sql_args[':source_tid'] = $source_tid;
		$stmt = db_query($sql, $sql_args);
		$hr = $stmt->fetchAssoc();
		$data[$domain_nid][$source_tid] = $hr['tid'];
	}
	return $data[$domain_nid][$source_tid];
}

function sirius_domain_clone_taxonomy($vocabulary_machine_name, $source_domain_nid, $target_domain_nid) {
	// Get the vocabulary
	$vocabulary = taxonomy_vocabulary_machine_name_load($vocabulary_machine_name);
	if (!$vocabulary) { return array('success' => FALSE, 'msg' => 'Vocabulary not found.'); }
	$vid = $vocabulary->vid;

	// Get the terms in the source vocabulary
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'taxonomy_term');
  $query->entityCondition('bundle', $vocabulary_machine_name);
  $query->fieldCondition('field_sirius_domain', 'target_id', $source_domain_nid);
  $stmt = $query->execute();
  if (!$stmt) {
  	return array('success' => FALSE, 'msg' => t('There are no terms in the source vocabulary.'));
  }
  $source_terms = taxonomy_term_load_multiple(array_keys($stmt['taxonomy_term']));

  // Get the terms in the target vocabulary
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'taxonomy_term');
  $query->entityCondition('bundle', $vocabulary_machine_name);
  $query->fieldCondition('field_sirius_domain', 'target_id', $target_domain_nid);
  $stmt = $query->execute();
  $target_terms = array();
  if ($stmt) {
	  $target_terms = taxonomy_term_load_multiple(array_keys($stmt['taxonomy_term']));
  }

  // Set up the target mapping
  $target_map = array();
  foreach ($target_terms as $target_tid => $target_term) {
  	$parent_tid = $target_term->field_sirius_term_source['und'][0]['value'];
  	if ($parent_tid) {
  		$target_map[$parent_tid] = $target_term;
  	}
  }

  // Clone
  $count = 0;
  $count_update = 0;
  $count_create = 0;
  foreach ($source_terms as $source_tid => $source_term) {

		// The "load" function doesn't fetch the parent, so we do that manually. One at a time because there might bemany
		$sql = "select parent from taxonomy_term_hierarchy where tid = :tid";
		$stmt = db_query($sql, array(':tid' => $source_tid));
		$hr = $stmt->fetchAssoc();
		$source_term->parent = $hr['parent'];

		sirius_minilog("Cloning: " . $source_term->name);
		$vars = get_object_vars($source_term);
  	++$count;
  	if ($target_map[$source_tid]) {
  		++$count_update;
  		$new_term = $target_map[$source_tid];
  		$new_term->name = $source_term->name;
  		$new_term->description = $source_term->description;
  		$new_term->weight = $source_term->weight;
  		$new_term->format = $source_term->format;
  		$new_term->hweight = $source_term->hweight;
  		$new_term->hdepth = $source_term->hdepth;
  		foreach ($vars as $field_name => $var) {
  			if (!preg_match('/^field_/', $field_name)) { continue; }
  			if ($field_name == 'field_sirius_domain') { continue; }
  			if ($field_name == 'field_sirius_term_source') { continue; }
  			$new_term->{$field_name} = $source_term->{$field_name};
  		}
  	} else {
  		++$count_create;
  		$new_term = $source_term;
  		unset($new_term->tid);
  		unset($new_term->uuid);
  		$new_term->field_sirius_term_source['und'][0]['value'] = $source_tid;
  		$new_term->field_sirius_domain['und'][0]['target_id'] = $target_domain_nid;
  	}

  	// Update term reference fields
  	foreach ($vars as $field_name => $var) {
			if (!preg_match('/^field_/', $field_name)) { continue; }
			if ($field_name == 'field_sirius_domain') { continue; }
			if ($field_name == 'field_sirius_term_source') { continue; }

	    $info = field_info_field($field_name);
	    if ($info['type'] != 'taxonomy_term_reference') { continue; }
	    $vocabulary_machine_name = $info['settings']['allowed_values'][0]['vocabulary'];
	    if (!$vocabulary_machine_name) { continue; }
	    $values = sirius_fieldvals($source_term, $field_name, 'tid');
	    foreach ($values as $delta => $source_reference_tid) {
	    	if (!$source_reference_tid) { continue; }
				$new_tid = sirius_domain_get_target_tid($target_domain_nid, $source_reference_tid);
				if ($new_tid) { $new_term->{$field_name}['und'][$delta]['tid'] = $new_tid; }
	    }
  	}

  	// Update that pesky JSON
  	$json = sirius_jsonfield_util_term_json_get($new_term);
  	$paths = sirius_jsonfield_util_value_list_paths($json);
  	$patterns = array(
  		':prole_tid',
			':caller_prole_tids:(\d+)',
			':admin_prole_tids:(\d+)',
			':paccess:([A-Z\d]+)\|([A-Z\d]+)',
			':participants:([A-Z\d]+)\|([A-Z\d]+)',
			':register_as',
  	);
  	foreach ($paths as $path) {
  		$matches = array();
  		foreach ($patterns as $pattern) {
	  		if (preg_match("/$pattern/", $path, $matches)) {
	  			$key_part_1 = $matches[1];
	  			$key_part_2 = $matches[2];
	  			$val = sirius_jsonfield_util_get_from_path($json, $path);
  				list ($val_1, $val_2) = explode('|', $val);

  				$key_part_1_new = sirius_domain_get_target_tid($target_domain_nid, $key_part_1);
  				$key_part_2_new = sirius_domain_get_target_tid($target_domain_nid, $key_part_2);
  				$val_1_new 			= sirius_domain_get_target_tid($target_domain_nid, $val_1);
  				$val_2_new 			= sirius_domain_get_target_tid($target_domain_nid, $val_2);

  				$new_path = $path;
  				if ($key_part_1 && $key_part_1_new && $key_part_1 != $key_part_1_new) { $new_path = preg_replace("/:$key_part_1\|/", ":$key_part_1_new|", $new_path); $new_path = preg_replace("/:$key_part_1\$/", ":$key_part_1_new", $new_path); }
  				if ($key_part_2 && $key_part_2_new && $key_part_2 != $key_part_2_new) { $new_path = preg_replace("/\|$key_part_2\$/", "|$key_part_2_new", $new_path); }

  				$new_val = $val;
  				if ($val_1 && $val_1_new && $val_1 != $val_1_new) { $new_val = preg_replace("/^$val_1/", "$val_1_new", $new_val); }
  				if ($val_2 && $val_2_new && $val_2 != $val_2_new) { $new_val = preg_replace("/$val_2\$/", "$val_2_new", $new_val); }

  				if ($path != $new_path) { sirius_jsonfield_util_unset_from_path($json, $path); }
  				if ($val != $new_val)   { sirius_jsonfield_util_set_from_path($json, $new_path, $new_val); }
	  		}
	  	}
  	}
  	sirius_jsonfield_util_term_json_set($new_term, $json);

		taxonomy_term_save($new_term);
		if (!$target_map[$source_tid]) { $target_map[$source_tid] = $new_term; }
  }

  // Reconstruct the hierarchy
  foreach ($source_terms as $source_tid => $source_term) {
  	$source_parent_tid = $source_term->parent;
  	if (is_array($source_parent_tid)) { $source_parent_tid = $source_parent_tid[0]; }
  	if ($source_parent_tid) {
  		$target_term = $target_map[$source_tid];
  		$target_parent_term = $target_map[$source_parent_tid];
  		$target_parent_tid = $target_parent_term->tid;
  		$target_term->parent = $target_parent_tid;
  		taxonomy_term_save($target_term);
  	}
  }

  // @todo: set hweight, hdepth

  // Set the "special" TIDs
  $all_keys = sirius_specialtid_keys();
  $keys = $all_keys[$vocabulary_machine_name];
  if ($keys) {
	  foreach ($keys as $key => $ignore) {
	  	$source_variable_name = 'sirius_specialtid_' . $source_domain_nid . '/' . $vocabulary_machine_name . '|' . $key;
	  	$target_variable_name = 'sirius_specialtid_' . $target_domain_nid . '/' . $vocabulary_machine_name . '|' . $key;
	  	$source_value = variable_get($source_variable_name);
	  	if ($source_value && $target_map[$source_value]) {
		  	$target_value = $target_map[$source_value]->tid;
		  	variable_set($target_variable_name, $target_value);
		  	sirius_minilog("Copying setting: $source_variable_name = $source_value to $target_variable_name = $target_value");
			}
	  }
	}

  return array('success' => TRUE, 'msg' => t('Taxonomy clone complete. Copied @count terms; created @count_create and updated @count_update.', array('@count' => $count, '@count_create' => $count_create, '@count_update' => $count_update)));
}
