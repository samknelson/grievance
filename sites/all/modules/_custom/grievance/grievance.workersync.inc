<?php

function grievance_workersync_plugin_list() {
	$all = module_invoke_all('grievance_workersync_plugin_info');
	ksort($all);
	foreach ($all as $key => $ignore) { $all[$key]['id'] = $key; }
	return $all;
}

function grievance_workersync_plugin_fetch() {
	$list = grievance_workersync_plugin_list();
	$which = sirius_domain_variable_get('grievance_workersync_source', 'none');
	return $list[$which];
}

function grievance_workersync_lookup_ajax($str) {
	$plugin = grievance_workersync_plugin_fetch();
	if (!$plugin) { exit(); }
	$callback = $plugin['lookup_callback'];
	if (!$callback) { exit(); }
	$result = $callback($str);
  drupal_json_output($result);
  exit;
}

function grievance_workersync_backsync($grievance_node) {
	if ($grievance_node->type != 'grievance') { return; }
	if ($grievance_node->grievance_workersync_backsync != 'Yes') { return; }
	if (!user_access("grievance worker update")) {
		drupal_set_message("You do not have permission to update the worker database.", 'warning');
		return;
	}
	$plugin = grievance_workersync_plugin_fetch();
	if (!$plugin) { 
		drupal_set_message("There is no worker database plugin defined.", 'warning');
		return;
	}
	$callback = $plugin['backsync_callback'];
	if (!$callback) { 
		drupal_set_message("This plugin does not allow the worker database to be updated.", 'warning');
		return;
	}
	$callback($grievance_node);
}

function grievance_workersync_search_form_callback_replace($form, $form_state) {
  $commands = array();
  $commands[] = ajax_command_replace("#grievance_workersync_search_form_callback_replace", $form['grievance_workersync_search_result']['#markup']);

  return array(
    '#type' => 'ajax',
    '#commands' => $commands
  );
}

function grievance_workersync_search_form_callback_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}

function grievance_workersync_search_form(&$form, &$form_state) {
	$plugin = grievance_workersync_plugin_fetch();
	if (!$plugin) { return; }

	if (!user_access('grievance worker lookup')) { return; }

  $form['grievance_workersync_search'] = array(
    '#type' => 'textfield',
    '#title' => $plugin['title'] . ' ' . t('Search'),
    '#description' => $plugin['search_description'],
  );
  $form['#groups']['group_grievance_employee']->children[] = 'grievance_workersync_search';
  $form['#group_children']['grievance_workersync_search'] = 'group_grievance_employee';

  $form['grievance_workersync_search_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
    '#submit' => array('grievance_workersync_search_form_callback_submit'),
    '#ajax' => array(
      'callback' => 'grievance_workersync_search_form_callback_replace',
      'method' => 'replace',
    ),
    '#limit_validation_errors' => array(array('grievance_workersync_search')),
  );
  $form['#groups']['group_grievance_employee']->children[] = 'grievance_workersync_search_submit';
  $form['#group_children']['grievance_workersync_search_submit'] = 'group_grievance_employee';

  $candidates = array();
	$str = $form_state['values']['grievance_workersync_search'];
  if ($str) {
  	$callback = $plugin['search_callback'];
    $candidates = $callback($str);
    $result_body = '';

    if (!$candidates) {
      $result_body .= t('No workers match your search criteria: ') . '<strong>' . $form_state['values']['grievance_workersync_search'] . '</strong>';
    } else {

      usort($candidates, function($a, $b) { return strcasecmp($a['sort'], $b['sort']); });

      $header = array(
        t('ID'),
        t('Name'),
        t('Employer'),
        t('Details'),
      );
      $attributes = array();
      $rows = array();
      foreach ($candidates as $candidate) {
        $row = array();
        $row[] = '<a href="#" class="grievance_workersync_id_for_insert">' . $candidate['id'] . '</a>';
        $row[] = $candidate['title'];
        $row[] = $candidate['employer'];
        $row[] = $candidate['extra'];
        $rows[] = $row;
      }
      $result_body = theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => $attributes));
    }

    $result_body .= '<span style="display: none;"><a href="#" id="grievance-imis-broughtby-tid-for-insert">' . $broughtby_tid . '</a></span>';
  }

  $form['grievance_workersync_search_result'] = array(
    '#type' => 'markup',
    '#markup' => '<div id="grievance_workersync_search_form_callback_replace">' . $result_body . '</div>',
  );
  $form['#groups']['group_grievance_employee']->children[] = 'grievance_workersync_search_result';
  $form['#group_children']['grievance_workersync_search_result'] = 'group_grievance_employee';

  // Add "update a worker" side effect
  $backsync_callback = $plugin['backsync_callback'];
  if ($backsync_callback) {
	  if (user_access('grievance worker edit')) { 
	    $form['#groups']['group_grievance_employee']->children[] = 'grievance_workersync_backsync';
	    $form['#group_children']['grievance_workersync_backsync'] = 'group_grievance_employee';
	    $weight = $form['field_grievance_min']['#weight'] + 0.1;
	    $form['grievance_workersync_backsync'] = array(
	      '#title' => 'Update Worker Database',
	      '#type' => 'select',
	      '#options' => array(
	        'No' => t('No'),
	        'Yes' => t('Yes'),
	      ),
	      '#default_value' => 'No',
	      '#required' => TRUE,
	      '#description' => t('Select "Yes" to save this information to the worker database. Choose this option if you are entering a new worker or wish to change the contact information for the worker.'),
	      '#weight' => $weight,
	    );
	  }
	}

}

function grievance_workersync_admin($form, &$form_state) {
	//
	// Which plugin?
	//

	$plugins = grievance_workersync_plugin_list();
	$options = array('none' => t('None'));
	if ($plugins) {
		foreach ($plugins as $key => $info) {
			$options[$key] = $info['title'];
		}
	}
	$intro = t('Please select the worker source. This will be used by admins to select workers and contact information when they are creating a new grievance.');
	if ($plugins) {
		$intro .= '<ul>';
		foreach ($plugins as $key => $plugin) {
			$intro .= '<li>';
			$intro .= $plugin['title'];
			$intro .= ': ';
			$intro .= $plugin['description'];
			$inro .= '</li>';
		}
		$intro .= '</ul>';
	}
	$variable_name = sirius_domain_variable_name('grievance_workersync_source');
	$plugin_source = $form_state['values'][$variable_name];
	if (!$plugin_source) { $plugin_source = variable_get($variable_name, 'local'); }
	$plugin_info = $plugins[$plugin_source];
	$form[$variable_name] = array(
		'#title' => t('Worker Source'),
		'#type' => 'select',
		'#options' => $options,
		'#default_value' => $plugin_source,
		'#prefix' => $intro,
    '#ajax' => array(
      'callback' => 'grievance_workersync_admin_form_callback',
      'method' => 'replace',
    ),
	);

	$form['plugin_settings'] = array(
		'#type' => 'fieldset',
		'#title' => t('Plugin Settings'),
		'#prefix' => '<div id="grievance_workersync_admin_form_callback">',
		'#suffix' => '</div>',
	);

	if (!$plugin_info) {
	} else {
		$settings_callback = $plugin_info['settings_callback'];
		if ($settings_callback) { $settings_callback($form, $form_state); }
	}

	$variable_name = sirius_domain_variable_name('grievance_workersync_lookup_enabled');
	$form[$variable_name] = array(
		'#title' => t('Lookup Enabled?'),
		'#type' => 'select',
		'#options' => array('no' => t('No'), 'yes' => t('Yes')),
		'#default_value' => variable_get($variable_name, 'yes'),
		'#description' => t('Select "Yes" to enable admins to search the worker database when creating a new grievance.'),
	);

	$variable_name = sirius_domain_variable_name('grievance_workersync_sync_grievances_enabled');
	$form[$variable_name] = array(
		'#title' => t('Sync Enabled?'),
		'#type' => 'select',
		'#options' => array('no' => t('No'), 'yes' => t('Yes')),
		'#default_value' => variable_get($variable_name, 'yes'),
		'#description' => t('Select "Yes" to automatically update grievance contact information from the worker source for all open grievances. The sync runs once a night.'),
	);

	$variable_name = sirius_domain_variable_name('grievance_workersync_sync_stewards_enabled');
	$form[$variable_name] = array(
		'#title' => t('Steward Sync Enabled?'),
		'#type' => 'select',
		'#options' => array('no' => t('No'), 'yes' => t('Yes')),
		'#default_value' => variable_get($variable_name, 'yes'),
		'#description' => t('Select "Yes" to automatically update steward contact information from the worker source for all stewards. The sync runs once a night.'),
	);

	return system_settings_form($form);
}

function grievance_workersync_admin_form_callback($form, $form_state) {
  $commands = array();

  $commands[] = ajax_command_replace("#grievance_workersync_admin_form_callback", render($form['plugin_settings']));

  return array(
    '#type' => 'ajax',
    '#commands' => $commands
  );
}

function grievance_workersync_test_form($form, &$form_state) {
  $form['intro'] = array(
    '#type' => 'markup',
    '#markup' => '<div class="sirius_form_intro">' . t('Use the form below to test your connection settings.') . '</div>',
  );

   $form['which'] = array(
    '#type' => 'select',
    '#title' => t('Function'),
    '#options' => array('search' => 'Search', 'lookup' => 'Lookup', 'backsync' => 'Backsync'),
    '#required' => TRUE,
  );

  $form['str'] = array(
    '#type' => 'textfield',
    '#title' => t('Arg'),
    '#description' => t('For search, enter the search string. For lookup, enter the member ID. For backsync, enter the grievnace NID.'),
    '#required' => TRUE,
  );

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	if (sirius_minilog()) {
		$form['results'] = array('#markup' => '<hr><h2>' . t('Results') . '</h2>' . sirius_minilog_render_html());
	}

	return $form;
}

function grievance_workersync_test_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
	$plugin = grievance_workersync_plugin_fetch();
	if (!$plugin) {
		drupal_set_message("No plugin has been selected.", 'error');
		return;
	}
	sirius_minilog("Using plugin: " . $plugin['id'] . " - " . $plugin['title']);

	$str = $form_state['values']['str'];
	sirius_minilog("Using query string: [$str]");

	$which = $form_state['values']['which'];
	if ($which == 'search') {
		$callback = $plugin['search_callback'];
		sirius_minilog("Testing: Search");
		$arg = $str;
	} else if ($which == 'lookup') {
		$callback = $plugin['lookup_callback'];
		sirius_minilog("Testing: Lookup");
		$arg = $str;
	} else {
		$callback = $plugin['backsync_callback'];
		sirius_minilog("Testing: Backsync");
		$arg = node_load($str);
		if (!$arg) { return sirius_minilog("$str is not a grievance node.");	}
	}

	if (!$callback) { return sirius_minilog("Callback for $which not found.", 'error'); }

	$result = $callback($arg, TRUE);

	if ($which == 'search') {
		if (!$result) {
			sirius_minilog("No results found.");
		} else {
      usort($result, function($a, $b) { return strcasecmp($a['sort'], $b['sort']); });
			$count = 0;
			foreach ($result as $hr) {
				++$count;
				sirius_minilog("Result #$count: $hr[id] -- $hr[title] -- $hr[employer] -- $hr[extra]");
			}
		}
	} else if ($which == 'lookup') {
		if (!$result) { 
			sirius_minilog("Worker not found.");
		} else {
			foreach ($result as $key => $value) {
				sirius_minilog("Found: $key = $value");
			}
		}
	} else {
		// Nothing returned
	}
}

function grievance_workersync_sync_form($form, &$form_state) {
  $form['intro'] = array(
    '#type' => 'markup',
    '#markup' => '<div class="sirius_form_intro">' . t('Use the form below to update all open grievances, and all shop stewards, with the latest information from the worker source.') . '</div>',
  );

  $form['confirm'] = array(
    '#type' => 'checkbox',
    '#title' => t('Yes, run the sync operation now.'),
    '#required' => TRUE,
  );

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	if (sirius_minilog()) {
		$form['results'] = array('#markup' => '<hr><h2>' . t('Results') . '</h2>' . sirius_minilog_render_html());
	}

	return $form;
}

function grievance_workersync_sync_form_submit($form, &$form_state) {
	grievance_workersync_sync();
	$form_state['rebuild'] = TRUE;
}

function grievance_workersync_cron($force = FALSE) {
	// Run once a day
	$today = date('Y-m-d');
	$last_run = variable_get('grievance_workersync_last_date', 0);
	if ($last_run == $today) { if (!$force) { return; } }
	variable_set('grievance_workersync_last_date', $today);

	// Don't run before a specific time
	$hour = variable_get('grievance_workersync_start_hr', 1);
	$min = variable_get('grievance_workersync_start_min', 30);
	if (time() < strtotime("$today $hour:$min")) { if (!$force) { return;} }

	// Run the scan for all domains
	$domains = sirius_domain_tree_render_as_select();
	$domains[NULL] = '(no domain)';
	foreach ($domains as $domain_nid => $domain_name) {
		sirius_minilog("Using domain $domain_name ($domain_nid)");
		sirius_domain($domain_nid);
		grievance_workersync_sync();
		sirius_minilog_watchdog('grievance_workersync_cron');
	}
}

function grievance_workersync_sync() {
	grievance_workersync_sync_grievances();
	grievance_workersync_sync_stewards();
}

function grievance_workersync_sync_grievances() {
	if (sirius_domain_variable_get('grievance_workersync_sync_grievances_enabled', 'yes') != 'yes') { return sirius_minilog("Grievance sync is not enabled."); }
	$plugin = grievance_workersync_plugin_fetch();
	if (!$plugin) { return sirius_minilog("No plugin has been selected.", 'error'); }
	$callback = $plugin['lookup_callback'];
	if (!$callback) { return sirius_minilog("This plugin has no lookup callback.", 'error'); }
	sirius_minilog("Starting grievance sync.");

	// Get the domain
	$domain_nid = sirius_domain_nid();

  // Load all "open" TIDs
  $all_status_terms = sirius_taxonomy_load('grievance_status', 'tid', 'full');
  $status_tids = array();
  foreach ($all_status_terms as $status_tid => $status_term) {
  	if ($status_term->field_grievance_open['und'][0]['value'] == 1) { 
  		$status_tids[] = $status_tid;
  		sirius_minilog("Including status " . $status_term->name . " ($status_tid)");
  	}
  }
  if (!$status_tids) {
  	sirius_minilog("There were no status terms flagged as 'open - unresolved'. No grievances will be sync'd.");
  	return;
  }

  $examined_count = 0;
  $updated_count = 0;

  // Find all open grievances where the data hasn't been changed manually.
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance');
  $query->fieldCondition('field_grievance_status', 'tid', $status_tids, 'in');
  if ($domain_nid) { $query->fieldCondition('field_sirius_domain', 'target_id', $domain_nid); }
  // The following would be nice, but doesn't handle nulls.  So we check manually later.
  // $query->fieldCondition('field_grievance_corrected', 'value', 'Pending', '!=');
  $stmt = $query->execute();
  if (!$stmt['node']) { 
  	sirius_minilog("No open grievances found.");
  	return;
  }

	$grievance_nids = array_keys($stmt['node']);

  foreach ($grievance_nids as $grievance_nid) {
    ++$examined_count;
    sirius_minilog("Grievance NID $grievance_nid: Starting");

    // Load the grievance
    $grievance_node = node_load($grievance_nid);

    // Skip "Pending" (meaning that the data has been corrected manually)
    if ($grievance_node->field_grievance_corrected['und'][0]['value'] == 'Pending') {
    	sirius_minilog("Grievance NID $grievance_nid: Data marked as manually corrected. Skipping.");
      continue;
    }

    // Find the member ID
    $member_id = $grievance_node->field_grievance_min['und'][0]['value'];
    if (!$member_id) {
    	sirius_minilog("Grievance NID $grievance_nid: No member ID. Skipping.");
      continue;
    }

    // Load the candidate from imis
    $candidate = $callback($member_id);
    if (!$candidate || !is_array($candidate)) {
    	sirius_minilog("Grievance NID $grievance_nid: No record found after querying worker source. Skipping.");
      continue;
    }

    // Update the member
    $fields = array();
    $fields['field_grievance_first_name']     = 'first_Name';
    $fields['field_grievance_last_name']      = 'last_Name';
    $fields['field_grievance_city']           = 'city';
    $fields['field_grievance_state']          = 'state';
    $fields['field_grievance_address']        = 'street';
    $fields['field_grievance_zip']           	= 'zip';
    $fields['field_grievance_phone']          = 'phone';
    $fields['field_grievance_email']          = 'email';
    $fields['field_grievance_department']     = 'department';
    $fields['field_grievance_classification'] = 'classification';

    $columns = array();
    $columns['field_grievance_first_name']     = 'value';
    $columns['field_grievance_last_name']      = 'value';
    $columns['field_grievance_city']           = 'value';
    $columns['field_grievance_state']          = 'value';
    $columns['field_grievance_address']        = 'value';
    $columns['field_grievance_zip']            = 'value';
    $columns['field_grievance_phone']          = 'value';
    $columns['field_grievance_email']          = 'email';
    $columns['field_grievance_department']     = 'value';
    $columns['field_grievance_classification'] = 'value';

    $is_changed = FALSE;
    foreach ($fields as $drupal_field => $json_field) {
      $column = $columns[$drupal_field];
      $subsection = $subsections[$drupal_field];

      $old_value = $grievance_node->{$drupal_field}['und'][0][$column];
      $new_value = $candidate[$json_field];
      $new_value = preg_replace("/(?:\r\n|\r|\n)/", ', ', $new_value);

      if (!$new_value) { continue; }
      if ($new_value == $old_value) { continue; }
      if ($json_field == 'phone' && sirius_phone_format($old_value) == sirius_phone_format($new_value)) { continue; }
      $grievance_node->{$drupal_field}['und'][0][$column] = $new_value;
      sirius_minilog("Grievance NID $grievance_nid: Changing $json_field from $old_value to $new_value.");
      $is_changed = TRUE;
    }

    if (!$is_changed) {
      sirius_minilog("Grievance NID $grievance_nid: No changes.");
      continue;
    }

    sirius_minilog("Grievance NID $grievance_nid: Saving updated grievance record.");
    node_save($grievance_node);
    ++$updated_count;
  }
  
  sirius_minilog("Grievance sync complete. Examined $examined_count nodes, updated $updated_count");
}

function grievance_workersync_sync_stewards() {
	if (sirius_domain_variable_get('grievance_workersync_sync_stewards_enabled', 'yes') != 'yes') { return sirius_minilog("Steward sync is not enabled."); }
	$plugin = grievance_workersync_plugin_fetch();
	if (!$plugin) { return sirius_minilog("No plugin has been selected.", 'error'); }
	$callback = $plugin['lookup_callback'];
	if (!$callback) { return sirius_minilog("This plugin has no lookup callback.", 'error'); }
	sirius_minilog("Starting steward sync.");

	$steward_type_tid = sirius_get_special_tid('grievance_contact_types|steward');
	if (!$steward_type_tid) { return sirius_minilog("There is no grievance contact type term configured for shop stewards. Go to the 'Sirius Special TIDs' page to select one.", 'error'); }

  // Find all shop stewards
  $domain_nid = sirius_domain_nid();
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_shop_contact');
  $query->fieldCondition('field_grievance_contact_types', 'tid', $steward_type_tid);
  if ($domain_nid) { $query->fieldCondition('field_sirius_domain', 'target_id', $domain_nid); }
  $stmt = $query->execute();
  if (!$stmt['node']) { return sirius_minilog("There are no contacts to sync."); }
  $contact_nids = array_keys($stmt['node']);

  foreach ($contact_nids as $contact_nid) {
    ++$examined_count;
    // Load the steward
    $contact_node = node_load($contact_nid);
    $contact_name = $contact_node->title;
    sirius_minilog("Contact #$contact_nid $contact_name: Starting");

    // Find the member ID
    $member_id = $contact_node->field_grievance_min['und'][0]['value'];
    if (!$member_id) {
    	sirius_minilog("Contact #$contact_nid $contact_name: No member ID. Skipping.");
      continue;
    }

    // Load the candidate from IMIS
    $candidate = $callback($member_id);
    if (!$candidate || !is_array($candidate)) {
      sirius_minilog("Contact #$contact_nid $contact_name: Lookup found no record for member id $member_id.");
      continue;
    }

    // Update the member
    $fields = array();
    $fields['field_grievance_co_city']           = 'city';
    $fields['field_grievance_co_state']          = 'state';
    $fields['field_grievance_co_address']        = 'street';
    $fields['field_grievance_co_phone']          = 'phone';
    $fields['field_grievance_co_email']          = 'email';

    $columns = array();
    $columns['field_grievance_co_city']           = 'value';
    $columns['field_grievance_co_state']          = 'value';
    $columns['field_grievance_co_address']        = 'value';
    $columns['field_grievance_co_phone']          = 'value';
    $columns['field_grievance_co_email']          = 'email';

    $subsections = array();
    $subsections['field_grievance_co_city']           = NULL;
    $subsections['field_grievance_co_state']          = NULL;
    $subsections['field_grievance_co_address']        = NULL;
    $subsections['field_grievance_co_phone']          = NULL;
    $subsections['field_grievance_co_email']          = NULL;

    $is_changed = FALSE;
    foreach ($fields as $drupal_field => $json_field) {
      $column = $columns[$drupal_field];
      $old_value = $contact_node->{$drupal_field}['und'][0][$column];
      $new_value = $candidate[$json_field];
      $new_value = preg_replace("/(?:\r\n|\r|\n)/", ', ', $new_value);

      if (!$new_value) { continue; }
      if ($new_value == $old_value) { continue; }
      if ($json_field == 'phone' && sirius_phone_format($old_value) == sirius_phone_format($new_value)) { continue; }

      $contact_node->{$drupal_field}['und'][0][$column] = $new_value;
      sirius_minilog("Contact #$contact_nid $contact_name: Changing $json_field from $old_value to $new_value.");
      $is_changed = TRUE;
    }

    if (!$is_changed) {
      sirius_minilog("Contact #$contact_nid $contact_name: No changes.");
      continue;
    }

    sirius_minilog("Contact #$contact_nid $contact_name: Saving updated grievance record.");
    node_save($contact_node);
    ++$updated_count;
  }

  sirius_minilog("Steward sync complete. Examined $examined_count nodes, updated $updated_count");
}
