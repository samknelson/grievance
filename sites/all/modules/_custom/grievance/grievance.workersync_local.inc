<?php

/**
 * Implements hook_grievance_workersync_plugin_info
 */

function grievance_grievance_workersync_plugin_info() {
	$info = array();
	$info['local'] = array(
		'title' => 'Local',
		'description' => 'Workers will be fetched from the local workers database, within this instance of Sirius.',
		'settings_callback' => 'grievance_workersync_local_settings',
		'search_callback' => 'grievance_workersync_local_search',
		'search_description' => 'Enter the worker\'s ID or SSN, or part of the worker\'s name.',
		'lookup_callback' => 'grievance_workersync_local_lookup',
		'backsync_callback' => 'grievance_workersync_local_backsync'
	);

	return $info;
}

/**
 * Plugin callback: settings
 */

function grievance_workersync_local_settings(&$form, &$form_state) {
	$variable_name = sirius_domain_variable_name('grievance_workersync_local_markup');
	$form['plugin_settings'][$variable_name] = array('#markup' => t('Connection information is not necessary; the local worker source has no settings.'));
}

function grievance_workersync_local_search($str, $verbose = FALSE) {
  // Execute as SQL for speed
  $tables = array(
    'field_data_field_sirius_ssn',
    'field_data_field_sirius_id',
    'field_data_field_sirius_id2',
    'field_data_field_grievance_shop',
    'field_data_field_sirius_domain',
    'field_data_field_sirius_name',
  );

  $domain_nid = sirius_domain_nid();

  // Writing this in SQL for speed
  $sql_args = array();
  if ($domain_nid) { $sql_args[':domain_nid'] = $domain_nid; }

  // Fields
  $sql = "select node.nid, ";
  $sql .= "node.title,  ";
  $sql .= "field_sirius_ssn_value, ";
  $sql .= "field_sirius_id_value, ";
  $sql .= "field_sirius_id2_value, ";
  $sql .= "node_employer.title as employer, ";
  $sql .= "field_sirius_name_family as last_name, ";
  $sql .= "field_sirius_name_given as first_name ";
  $sql .= "from node ";
  foreach ($tables as $table) {
    $sql .= "left join $table on $table.entity_type = 'node' and $table.entity_id = node.nid ";
  }
  $sql .= "left join node node_employer on node_employer.nid = field_data_field_grievance_shop.field_grievance_shop_target_id ";
  $sql .= "where node.type = 'sirius_worker' ";
  $sql .= "and (";
  $sql .= "  node.title like :match_substr ";
  $sql .= "  or field_sirius_ssn_value = :match_exact ";
  $sql .= "  or field_sirius_id_value = :match_exact ";
  $sql .= "  or field_sirius_id2_value = :match_exact ";
  $sql .= ") ";
	if ($domain_nid) {
		$sql .= "and field_sirius_domain_target_id = :domain_nid ";
	}
  // $sql .= "order by node.title ";
  $sql .= "limit 50 ";

  $str = preg_replace('/\%/', '', $str);
	$sql_args[':match_substr'] = '%' . $str . '%';
	$sql_args[':match_exact'] = $str;
  $stmt = db_query($sql, $sql_args);

  $matches = array();
  while ($hr = $stmt->fetchAssoc()) {
  	$match = array();
  	$match['id'] = $hr['nid'];
  	$match['title'] = $hr['title'];
  	$match['employer'] = $hr['employer'];

  	$extra = '';
  	if ($hr['field_sirius_ssn_value']) {
  		if ($extra) { $extra .= ' / '; }
  		$extra .= '###-##-' . substr($hr['field_sirius_ssn_value'], -4, 4);
  	}
  	if ($hr['field_sirius_id_value']) {
  		if ($extra) { $extra .= ' / '; }
  		$extra .= $hr['field_sirius_id_value'];
  	}
  	if ($hr['field_sirius_id2_value']) {
  		if ($extra) { $extra .= ' / '; }
  		$extra .= $hr['field_sirius_id2_value'];
  	}
  	$match['extra'] = $extra;
    $match['sort'] = $hr['last_name'] . ' ' . $hr['first_name'];
    $matches[] = $match;
  }

  return $matches;
}

function grievance_workersync_local_lookup($worker_nid, $verbose = FALSE) {
	$domain_nid = sirius_domain_nid();
  $worker_node = node_load($worker_nid);
  if ($worker_node->field_sirius_domain['und'][0]['target_id'] != $domain_nid) { 
  	sirius_minilog("Worker $worker_nid is not in domain $domain_nid.", 'error');
  	return array();
  }
  $result = array();
  $result['first_name']   = $worker_node->field_sirius_name['und'][0]['given'];
  $result['last_name']    = $worker_node->field_sirius_name['und'][0]['family'];
  $result['city']         = $worker_node->field_sirius_address['und'][0]['locality'];
  $result['state']        = $worker_node->field_sirius_address['und'][0]['administrative_area'];
  $result['zip']          = $worker_node->field_sirius_address['und'][0]['postal_code'];
  $result['street']       = $worker_node->field_sirius_address['und'][0]['thoroughfare'];
  $result['gender']       = $worker_node->field_sirius_gender['und'][0]['value'];
  $result['phone']        = $worker_node->field_sirius_phone['und'][0]['value'];
  $result['email']        = $worker_node->field_sirius_email['und'][0]['value'];
  $result['id']           = $worker_node->field_sirius_id['und'][0]['value'];
  return $result;
}

function grievance_workersync_local_backsync($grievance_node, $verbose = FALSE) {
	if ($grievance_node->type != 'grievance') { return sirius_minilog('That is not a grievance node.', 'error'); }
	$domain_nid = sirius_domain_nid();
	if ($domain_nid != $grievance_node->field_sirius_domain['und'][0]['target_id']) { return sirius_minilog('That grievance is not in the current domain', 'error'); }

  $worker_id = trim($grievance_node->field_grievance_min['und'][0]['value']);
  if (!$worker_id) { return sirius_minilog("Worker database update failed: no member id.", 'warning'); }
  $worker_node = sirius_worker_get_by_id($worker_id);
  if (!$worker_node) {
    if (!user_access('grievance worker create')) {
      drupal_set_message("Worker database update failed: The worker was not found in the database, and you do not have permission to create a new worker. ", 'warning');
      return;
    }

    global $user;
    $worker_node = new stdClass();
    $worker_node->type = 'sirius_worker';
    $worker_node->language = LANGUAGE_NONE;
    $worker_node->uid = $user->uid;
    $worker_node->status = 1;
    $worker_node->promote = 0;
    $worker_node->comment = 0;
    $worker_node->title = substr(date('c') . " - " . $category . " - " . $type . " - " . $message, 0, 254);
    $worker_node->field_sirius_id['und'][0]['value'] = $worker_id;
    if ($domain_nid) {
    	$worker_node->field_sirius_domain['und'][0]['target_id'] = $domain_nid;
    }
  }


  if ($value = $grievance_node->field_grievance_first_name['und'][0]['value']) { $worker_node->field_sirius_name['und'][0]['given'] = $value; }
  if ($value = $grievance_node->field_grievance_last_name['und'][0]['value'])  { $worker_node->field_sirius_name['und'][0]['family'] = $value; }
  if ($value = $grievance_node->field_grievance_phone['und'][0]['value'])      { $worker_node->field_sirius_phone['und'][0]['value'] = $value; }
  if ($value = $grievance_node->field_grievance_email['und'][0]['email'])      { $worker_node->field_sirius_email['und'][0]['value'] = $value; }

  if ($value = $grievance_node->field_grievance_address['und'][0]['value'])    {
    $worker_node->field_sirius_address['und'][0]['thoroughfare'] = $value; 
    $worker_node->field_sirius_address['und'][0]['country'] = 'US';
  }

  if ($value = $grievance_node->field_grievance_address_2['und'][0]['value'])  { $worker_node->field_sirius_address['und'][0]['premise'] = $value; }
  if ($value = $grievance_node->field_grievance_city['und'][0]['value'])       { $worker_node->field_sirius_address['und'][0]['locality'] = $value; }
  if ($value = $grievance_node->field_grievance_state['und'][0]['value'])      { $worker_node->field_sirius_address['und'][0]['administrative_area'] = $value; }
  if ($value = $grievance_node->field_grievance_zip['und'][0]['value'])        { $worker_node->field_sirius_address['und'][0]['postal_code'] = $value; }

  if ($grievance_node->field_grievance_gender['und'][0]['value'] == 'M' && sirius_get_special_tid('sirius_gender|M')) { 
    $worker_node->field_sirius_gender['und'][0]['tid'] = sirius_get_special_tid('sirius_gender|M');
  }

  if ($grievance_node->field_grievance_gender['und'][0]['value'] == 'F' && sirius_get_special_tid('sirius_gender|F')) { 
    $worker_node->field_sirius_gender['und'][0]['tid'] = sirius_get_special_tid('sirius_gender|F');
  }


  $worker_node = node_submit($worker_node);
  node_object_prepare($worker_node);
  node_save($worker_node);
  drupal_set_message("Worker database update: saved worker " . $worker_node->title . " ($worker_id)");
}