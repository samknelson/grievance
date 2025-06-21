<?php

function sirius_customfield_menu_local_tasks_alter(&$data, $router_item, &$root_path) {
	// Only interested in paths that begin "node/%"
	if (strpos($root_path, 'node/%') !== 0) { return; }

	// Only interested in nodes
	$entity_node = menu_get_object();
	if (!$entity_node || !$entity_node->type) { return; }

	// Only interested in certain content types
	$content_types = sirius_customfield_content_types_all();
	if (!in_array($entity_node->type, array_keys($content_types))) { return; }

	// The path is e.g.
	//   node/%/grievance_customfield_tab/%
	// But we want to throw away that local tab, and instead create new local tabs like
	//   node/12345/grievance_customfield_tab/67890
	// They have the same path, but different hrefs, and they replace the original "hidden" tab.
	// This is messy, but it seems to be the right way to do things.

	// Remove the "hidden" tab
	$path = 'node/%/' . $entity_node->type . '_customfield_tab';
	if (is_array($data['tabs'][0]['output'])) {
		foreach ($data['tabs'][0]['output'] as $idx => $record) {
			if ($record['#link']['path'] == $path) {
				unset($data['tabs'][0]['output'][$idx]);
				$data['tabs'][0]['count']--;
			}
		}
	}

	// Find any custom field sets
	$entity_json = sirius_json_get($entity_node);
	$cf_nids = array_keys(sirius_customfield_set_list($entity_node));
	$cf_nodes = node_load_multiple($cf_nids);
	if (!$cf_nodes) { return; }

	// Walk the custom field sets and generate a tab for any that are tab-enabled.
	foreach ($cf_nodes as $cf_nid => $cf_node) {
		$cf_json = sirius_json_get($cf_node);

		// Must have access to this CF set
		$context = array(
			'set_node' => $cf_node,
			'entity_node' => $entity_node
		);
		if (!sirius_customfield_access('list', $context)) { continue; }

		$title = $cf_json['customfield']['tab']['title'];
		if (!$title) { $title = $cf_node->title; }

		$href = 'node/' . $entity_node->nid . '/' . $entity_node->type . '_customfield_tab/' . $cf_nid;

		if ( ($path == $root_path) && ($cf_nid == arg(3)) ) {
			$active = TRUE; 
		} else {
			$active = FALSE;
		}

		$weight = $cf_json['customfield']['tab']['weight'];
		if (!isset($weight) || ($weight === '')) { $weight = 100; }

	  $data['tabs'][0]['output'][] = array(
	    '#theme' => 'menu_local_task',
	    '#link' => array(
	      'title' => $title,
	      'href' => $href,
	      'path' => $path,
	      'weight' => $weight,
	      'localized_options' => array(
	        'attributes' => array(
	        ),
	      ),
	    ),
	    '#active' => $active,
	    '#weight' => $weight,
	  );

		$data['tabs'][0]['count']++;
	}
}

function sirius_customfield_content_types_all() {
 	$types = &drupal_static(__FUNCTION__);

  if (!isset($types)) {
  	$types['grievance'] = array(
  		'key' => 'grievance',
  		'permission' => 'grievance admin',
  	);

  	$types['sirius_event'] = array(
  		'key' => 'sirius_event',
  		'permission' => 'sirius event admin',
  	);

  	$types['sirius_dispatch_job_group'] = array(
  		'key' => 'sirius_dispatch_job_group',
  		'permission' => 'sirius dispatch admin',
  	);

  	$types['sirius_ledger_account'] = array(
  		'key' => 'sirius_ledger_account',
  		'permission' => 'sirius ledger admin',
  	);

  	$types['sirius_trust_benefit'] = array(
  		'key' => 'sirius_trust_benefit',
  		'permission' => 'sirius trust admin',
  	);

  	$types['sirius_trust_worker_election'] = array(
  		'key' => 'sirius_trust_worker_election',
  		'permission' => 'sirius trust admin',
  	);

  	$types['sirius_contact_relationship'] = array(
  		'key' => 'sirius_contact_relationship',
  		'permission' => 'sirius domain admin',
  	);

  	$types['sirius_log'] = array(
  		'key' => 'sirius_log',
  		'permission' => 'sirius domain admin',
  	);

  	$types['grievance_shop'] = array(
  		'key' => 'grievance_shop',
  		'permission' => 'sirius employer admin',
  	);

  	foreach ($types as $key => $record) { 
  		$types[$key]['title'] = sirius_content_type_name($key); 
  	}

		drupal_alter('sirius_customfield_content_types', $types);
  }

	return $types;
}

function sirius_customfield_content_types($json) {
	$types_all = sirius_customfield_content_types_all();
	if (!$json['customfield']['content_types']['types_enabled']) { return $types_all; }
	$keys = array_filter(array_values($json['customfield']['content_types']['types_enabled']));
	foreach ($keys as $key) { $types[$key] = $types_all[$key]; }
	return $types;
}

function sirius_customfield_content_type_options($content_types) {
	if (!$content_types) { return array(); }
	$result = array();
	foreach ($content_types as $key => $record) { $result[$key] = $record['title']; }
	return $result;
}

function sirius_customfield_set_list_all($content_type = NULL, $category = NULL, $params = array()) {
	$domain_nid = sirius_domain_nid();

	$sql_args = array();
	$sql = "select nid, title ";
	$sql .= "from node ";
	$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_type on field_data_field_sirius_type.entity_type = 'node' and field_data_field_sirius_type.entity_id = node.nid ";
	$sql .= "where type = 'sirius_json_definition' ";
	$sql .= "and field_sirius_type_value = 'customfield' ";
	$sql_args = array();
	if ($domain_nid) {
		$sql .= "and field_sirius_domain_target_id = :domain_nid ";
		$sql_args[':domain_nid'] = $domain_nid;
	} else {
		$sql .= "and field_sirius_domain_target_id is null ";
	}
	$stmt = sirius_sql_query($sql, $sql_args);
	$options_all = array();
	while ($hr = $stmt->fetchAssoc()) {
		$options_all[$hr['nid']] = $hr['title'];
	}

	if (!$content_type) { return $options_all; }

	$options = array();
	foreach ($options_all as $cf_nid => $ignore) {
		$cf_node = node_load($cf_nid);
		$cf_json = sirius_json_get($cf_node);

		if (!$cf_json['customfield']['tab']['show_by_default']) {
			$cf_json['customfield']['tab']['show_by_default'] = 'No';
		}
		if ($params['tab']['show_by_default']) {
			if ($cf_json['customfield']['tab']['show_by_default'] != $params['tab']['show_by_default']) {
				continue;
			}
		}


		$content_types = sirius_customfield_content_types($cf_json);
		if (!$content_types) { $content_types = array(); }
		if (!in_array($content_type, array_keys($content_types))) { continue; }

		if ($content_type == 'sirius_log' && $category && isset($cf_json['customfield']['content_types']['log_categories'])) {
			if (!in_array($category, array_filter(array_values($cf_json['customfield']['content_types']['log_categories'])))) {
				continue; 
			}
		}
		$options[$cf_nid] = $cf_node->title;
	}
	return $options;
}

function sirius_customfield_set_list($entity_node) {
	$lookup = sirius_customfield_set_list_all($entity_node->type);

	$entity_json = sirius_json_get($entity_node);
	$options = array();

	// We may have set this explicitly.
	if ($entity_json && $entity_json['customfield']['sets']) { 
		$cf_nids = array_filter(array_values($entity_json['customfield']['sets']));
		foreach ($cf_nids as $cf_nid) {
			$options[$cf_nid] = $lookup[$cf_nid];
		}
		return $options;
	}

	// Any custom field sets which are "show by default"
	foreach ($lookup as $cf_nid => $cf_title) {
		$cf_node = node_load($cf_nid);
		$cf_json = sirius_json_get($cf_node);
		if ($cf_json['customfield']['tab']['show_by_default'] == 'Yes') {
			$options[$cf_nid] = $cf_title;
		}
	}

	// This should be in an _alter hook, but for now
	if ($entity_node->type == 'grievance') {
		$status_tid = $entity_node->field_grievance_status['und'][0]['tid'];
		$status_term = taxonomy_term_load($status_tid);
		if ($status_term) {
			$resproc_tid = $status_term->field_grievance_resproc['und'][0]['tid'];
			$resproc_term = taxonomy_term_load($resproc_tid);
			if ($resproc_term) {
				$resproc_json = sirius_json_get($resproc_term);
				if ($resproc_json && $resproc_json['customfield']['sets']) { 
					$cf_nids = array_filter(array_values($resproc_json['customfield']['sets']));
					foreach ($cf_nids as $cf_nid) {
						$options[$cf_nid] = $lookup[$cf_nid];
					}
				}
			}
		}
	}

	return $options;
}

function sirius_customfield_value($node, $set_machine_name, $path) {
	$set_nid = sirius_customfield_set_lookup_by_machine_name($set_machine_name);
	if (!$set_nid) { return '[custom field set not found]'; }
  $which = 'customfield_' . $set_nid;
	$entity_json = sirius_json_get($node);
  $entity_results = sirius_jsonfield_util_results_get($entity_json, $which);
	return sirius_jsonfield_util_get_from_path($entity_results, $path);
}

function sirius_customfield_set_lookup_by_machine_name($machine_name) {
	// @todo: This is some really cumbersome code.
	$domain_nid = sirius_domain_nid();
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_json_definition');
  $query->fieldCondition('field_sirius_type', 'value', 'customfield');
  if ($domain_nid) { $query->fieldCondition('field_sirius_domain', 'target_id', $domain_nid); }
  $stmt = $query->execute();
  $nids = array();
  if (!$stmt['node']) { return NULL; }
	$nids = array_keys($stmt['node']);
  $nodes = node_load_multiple($nids);
  foreach ($nodes as $nid => $node) {
  	$json = sirius_jsonfield_util_node_json_get($node);
  	if ($json['machine_name'] == $machine_name) { return $nid; }
  }
 return NULL;
}

/**
 * Replace the display suite custom field "custom_fields" with rendered content
 * for all the custom field sets defined for the node.
 */

function sirius_customfield_node_view($entity_node, $view_mode, $langcode) {
	// Only interested in replacing the "custom_fields" field
  if (!$entity_node->content['custom_fields']) { return; }

  $options = array();
  $options['fieldsets'] = TRUE;
	$options['skip_empty'] = TRUE;

	$html = '';

	// Special treatment for sirius_worker. @todo: This is kinda legacy.
	if ($entity_node->type == 'sirius_worker') {

	  $which = 'default';

	  $contact_node = node_load($entity_node->field_sirius_contact['und'][0]['target_id']);
	  if (!$contact_node) { $entity_node->content['custom_fields']['#access'] = FALSE; return; }

	  $contact_definition_json = sirius_contact_definition();
	  $contact_definition_form = sirius_jsonfield_util_form_get($contact_definition_json, $which);
	  $contact_json = sirius_jsonfield_util_node_json_get($contact_node);
	  $contact_results = sirius_jsonfield_util_results_get($contact_json, $which);
	  if ($contact_results) { 
		  sirius_jsonfield_util_set_defaults($contact_definition_form, $contact_results);
	  	$html .= sirius_jsonfield_util_render_all_values_new($contact_definition_form, $contact_results, $options);
	  }
	}

	// Render all CF sets
	$entity_json = sirius_json_get($entity_node);
	$cf_sets = sirius_customfield_set_list($entity_node);
	$cf_nodes = node_load_multiple(array_keys($cf_sets));
	if ($entity_node->type == 'sirius_log' && $entity_node->field_sirius_category['und'][0]['value'] == 'customfield') {
		$cf_nid = $entity_node->field_sirius_type['und'][0]['value'];
		$cf_nodes[$cf_nid] = node_load($cf_nid);
	}

	foreach ($cf_nodes as $cf_nid => $cf_node) {
		$cf_json = sirius_json_get($cf_node);
		$cf_form = sirius_jsonfield_util_form_get($cf_json, 'default');
		$entity_results = sirius_jsonfield_util_results_get($entity_json, "customfield_$cf_nid");
		if ($entity_results) {
		  sirius_jsonfield_util_set_defaults($cf_form, $entity_results);
	  	$html .= sirius_jsonfield_util_render_all_values_new($cf_form, $entity_results, $options);
		}
	}

	// Add materials. This is a kludge.
	if (sirius_materials_access('list', $entity_node)) {
		$html .= sirius_node_tab_entity_materials_list($entity_node, array('hide_add' => TRUE, 'hide_empty' => TRUE, 'hide_header' => TRUE));
	}

  $entity_node->content['custom_fields'][0]['#markup'] = $html;
  if (!$html) { $entity_node->content['custom_fields']['#access'] = FALSE; }
}

function sirius_customfield_form($cf_nid, $entity_node = NULL) {
	$cf_node = node_load($cf_nid);
  $cf_json = sirius_json_get($cf_node);
  $cf_form = sirius_jsonfield_util_form_get($cf_json, 'default');
  if (!$cf_form) { 
  	return sirius_form_error("The form for this custom field set has not been configured. It can be set up " . l('here.', 'sirius/customfield/definition/list'));
  }

  $which = 'customfield_' . $cf_nid;

  if ($entity_node) {
	  $entity_json = sirius_json_get($entity_node);
	  $entity_results = sirius_jsonfield_util_results_get($entity_json, $which);
	  sirius_jsonfield_util_set_defaults($cf_form, $entity_results);
	}

  $cf_form['!intro'] = array(
    '#markup' => '<div class="sirius-form">',
    '#weight' => -9999,
  );
  $cf_form['!outro'] = array(
    '#markup' => '</div>',
    '#weight' => 9999,
  );

  return $cf_form;
}

function sirius_customfield_access_instance($set_node) {
	$set_json = sirius_json_get($set_node);
	$type = $set_json['customfield']['access']['type'];
	if ($type) { $instance = sirius_access_instance($type); }
	if (!$instance) { $instance = sirius_access_instance('denyall'); }
	return $instance;
}

function sirius_customfield_access_instance_settings($set_node) {
	$instance = sirius_customfield_access_instance($set_node);
	$set_json = sirius_json_get($set_node);
	$settings = $set_json['customfield']['access']['settings'][$instance->get_type()];
	if (!$settings) { $settings = array(); }
	return $settings;
}

//
// Actions:
// - list
// - view
// - add
// - edit
//

function sirius_customfield_access($action, $context) {
	$set_node = $context['set_node'];
	$entity_node = $context['entity_node'];
	$data_node = $context['data_node'];

	// LEGACY SUPPORT: We simply have a list of roles.
	$set_json = sirius_json_get($set_node);
	if ($set_json['customfield']['tab']['roles']) {
		global $user;
		$set_rids = array_filter(array_values($set_json['customfield']['tab']['roles'])); 
		$my_rids = array_filter(array_keys($user->roles));
		if (!array_intersect($set_rids, $my_rids)) { return FALSE; }
		if (!node_access('update', $entity_node)) { return FALSE; }
		return TRUE; 
	} 


	// Fetch our settings
	if (!$context['settings']) {
		$settings = sirius_customfield_access_instance_settings($set_node); 
		$context['settings'] = $settings;
	}

	// Call the access plugin
	$instance = sirius_customfield_access_instance($set_node);
	return $instance->access($action, $context);
}

function sirius_customfield_status($context) {
	$json = sirius_json_get($context['data_node']);
	$set_nid = $context['set_node']->nid;
	$which = "customfield_$set_nid";
	return $json[$which]['status'];
}

function sirius_customfield_status_set($context, $new_status) {
	$data_node = $context['data_node'];
	$json = sirius_json_get($data_node);
	$set_nid = $context['set_node']->nid;
	$which = "customfield_$set_nid";
	$json[$which]['status'] = $new_status;
	sirius_json_set($data_node, $json);
	node_save($data_node);
	return $new_status;
}

function sirius_customfield_multiple_context($data_node) {
	$set_nid = $data_node->field_sirius_type['und'][0]['value'];
	$set_node = node_load($set_nid);
	$entity_node = sirius_log_handler($data_node, NULL);
	$settings = sirius_customfield_access_instance_settings($set_node); 
	$context = array(	
		'set_node' => $set_node,
		'entity_node' => $entity_node,
		'data_node' => $data_node,
		'settings' => $settings,
	);
	return $context;
}

function sirius_customfield_multiple($set_node) {
	$set_json = sirius_json_get($set_node);
	return ($set_json['customfield']['tab']['multiple'] == 'Yes');
}