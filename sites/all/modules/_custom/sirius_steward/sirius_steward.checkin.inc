<?php

function sirius_steward_checkin_status($checkin_node) {
	return $checkin_node->field_sirius_type['und'][0]['value'];
}

function sirius_steward_checkin_status_list() {
	return array(
		'in' => 'Checked In',
		'out' => 'Checked Out'
	);
}

function sirius_steward_checkin_search($params = array()) {
	$sql_args = array();
	$sql = "select distinct nid ";
	$sql .= "from node ";
	$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_json on field_data_field_sirius_json.entity_type = 'node' and field_data_field_sirius_json.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_type on field_data_field_sirius_type.entity_type = 'node' and field_data_field_sirius_type.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_category on field_data_field_sirius_category.entity_type = 'node' and field_data_field_sirius_category.entity_id = node.nid ";
	$sql .= "where node.type = 'sirius_log' ";
	$sql .= "and field_sirius_category_value = 'steward:checkin' ";
	$domain_nid = sirius_domain_nid();
	if ($domain_nid) {
		$sql .= "and field_sirius_domain_target_id = :domain_nid ";
		$sql_args[':domain_nid'] = $domain_nid;
	} else {
		$sql .= "and field_sirius_domain_target_id is null ";
	}

	foreach (array('worker_nid', 'facility_nid', 'employer_nid') as $key) {
		if (!$params[$key]) { continue; }
		$sql .= "and exists ("; 
		$sql .= "  select 1 ";
		$sql .= "  from field_data_field_sirius_log_handler ";
		$sql .= "  where field_data_field_sirius_log_handler.entity_type = 'node' ";
		$sql .= "  and field_data_field_sirius_log_handler.entity_id = node.nid ";
		$sql .= "  and field_data_field_sirius_log_handler.field_sirius_log_handler_target_id = :$key ";
		$sql .= ")";
		$sql_args[":$key"] = $params[$key];
	}

	if ($params['status']) {
		$sql .= "and field_sirius_type_value = :status ";
		$sql_args[':status'] = $params['status'];
	}

	$sql .= "order by node.nid desc ";
	$sql .= "limit 1000 ";

	$stmt = sirius_sql_query($sql, $sql_args);
	$checkin_nids = array();
	while ($hr = $stmt->fetchAssoc()) {
		$checkin_nids[] = $hr['nid'];
	}

	return $checkin_nids;

}

function sirius_steward_checkin_in_possible($facility_nid, $worker_nid, $json) {
	// Must have a real facility
	if (!$facility_nid) { return array('success' => FALSE, 'msg' => 'No facility specified.'); }
	$facility_node = node_load($facility_nid);
	if (!$facility_node) { return array('success' => FALSE, 'msg' => "Facility #$facility_nid not found."); }
	if ($facility_node->type != 'sirius_dispatch_facility') { return array('success' => FALSE, 'msg' => "Facility #$facility_nid is not of type sirius_dispatch_facility."); }

	// Must have a real worker
	if (!$worker_nid) { return array('success' => FALSE, 'msg' => 'No worker specified.'); }
	$worker_node = node_load($worker_nid);
	if (!$worker_node) { return array('success' => FALSE, 'msg' => "Worker #$worker_nid not found."); }
	if ($worker_node->type != 'sirius_worker') { return array('success' => FALSE, 'msg' => "Worker #$worker_nid is not of type sirius_worker."); }

	// Can't check in twice
	$current_checkins = sirius_steward_checkin_search(array('worker_nid' => $worker_node->nid, 'status' => 'in'));
	if ($current_checkins) { return array('success' => FALSE, 'msg' => "Worker $worker_nid is already checked in."); }

	// Must have a dispatch
	$dispatch_node = sirius_dispatch_get_current($worker_node->nid);
	if (!$dispatch_node) {
		return array('success' => FALSE, 'msg' => 'You do not have a current active dispatch, so check in is not available.');
	}
	$job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);
	if (!$job_node) {
		return array('success' => FALSE, 'msg' => 'Dispatch found, but the dispatch is not linked to a job node.');
	}
	$employer_nid = $job_node->field_grievance_shop['und'][0]['target_id'];
	if (!$employer_nid) {
		return array('success' => FALSE, 'msg' => 'Job found, but the job is not associated with an employer.');
	}

	return array('success' => TRUE);
}

function sirius_steward_checkin_in($facility_nid, $worker_nid, $json_in) {
	$result = sirius_steward_checkin_in_possible($facility_nid, $worker_nid, $json);
	if (!$result['success']) { return $result;}
	global $user;

	$facility_node = node_load($facility_nid);
	$worker_node = node_load($worker_nid);

	$dispatch_node = sirius_dispatch_get_current($worker_node->nid);
	$job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);
	$employer_nid = $job_node->field_grievance_shop['und'][0]['target_id'];

	$category = 'steward:checkin';
	$type = 'in';
	$message = $summary = "Checked " . $worker_node->title . " in to " . $facility_node->title;
	$notes = "$worker_nid|$facility_nid|IN";
	$handler_nids = array($worker_nid, $facility_nid, $employer_nid);
	$attachments = NULL;
	$json = array();
	$json['in'] = $json_in;
	$json['in']['ts'] = time();
	$json['in']['uid'] = $user->uid;

	// Set the customfield sets as "Active" in the checkin record. We set both in and out customfields, 
	// so e.g. admins can immediately edit the "out" customfield even if it hasn't been filled out yet.
	$customfield_in = sirius_domain_variable_get('sirius_steward_checkin_in_customfield');
	$customfield_out = sirius_domain_variable_get('sirius_steward_checkin_out_customfield');
	if ($customfield_in) { $json['customfield']['sets'][$customfield_in] = $customfield_in; }
	if ($customfield_out) { $json['customfield']['sets'][$customfield_out] = $customfield_out; }

	// Set the customfield
	$customfield_set_nid = sirius_domain_variable_get('sirius_steward_checkin_in_customfield');
	if ($customfield_set_nid) {
	  $which = 'customfield_' . $customfield_set_nid;
  	sirius_jsonfield_util_results_set($json, $which, $json_in['customfield'][$which]);
  }

	$checkin_node = sirius_log($category, $type, $message, $summary, $notes, $handler_nids, $attachments, $json);

	// Set the disclaimer
	if ($json_in['disclaimer']) {
		$disclaimer_nodes = node_load_multiple(array_keys($json_in['disclaimer']));
		sirius_disclaimer_accept($checkin_node, $disclaimer_nodes, array());
		node_save($checkin_node);
	}

	return array('success' => TRUE, 'checkin_node' => $checkin_node);
}

function sirius_steward_checkin_out_possible($checkin_node, $json_out) {
	if ($checkin_node->type != 'sirius_log') { return array('success' => FALSE, 'msg' => 'Not a checkin node.'); }
	if ($checkin_node->field_sirius_type['und'][0]['value'] != 'in') { return array('success' => FALSE, 'msg' => 'This record is not currently checked in.'); }
	return array('success' => TRUE);
}

function sirius_steward_checkin_out($checkin_node, $json_out) {
	$result = sirius_steward_checkin_out_possible($checkin_node, $json);
	if (!$result['success']) { return $result;}
	global $user;

	$checkin_node->field_sirius_type['und'][0]['value'] = 'out';

	$json = sirius_json_get($checkin_node);
	$json['out'] = $json_out;
	$json['out']['ts'] = time();
	$json['out']['uid'] = $user->uid;

	// Set the customfield
	$customfield_set_nid = sirius_domain_variable_get('sirius_steward_checkin_out_customfield');
	if ($customfield_set_nid) {
	  $which = 'customfield_' . $customfield_set_nid;
  	sirius_jsonfield_util_results_set($json, $which, $json_out['customfield'][$which]);
  }

	sirius_json_set($checkin_node, $json);
	node_save($checkin_node);

	return array('success' => TRUE);
}

function sirius_steward_checkin_render_list($checkin_nids, $options = array()) {
	$checkin_nodes = node_load_multiple($checkin_nids);
	if (!$checkin_nodes) { return t('No records found.'); }

	$status_list = sirius_steward_checkin_status_list();

	$rows = array();
	foreach ($checkin_nodes as $checkin_nid => $checkin_node) {
		$facility_node = sirius_log_handler($checkin_node, 'sirius_dispatch_facility');
		$worker_node = sirius_log_handler($checkin_node, 'sirius_worker');
		$employer_node = sirius_log_handler($checkin_node, 'grievance_shop');
		$checkin_json = sirius_json_get($checkin_node);
		if ($checkin_json['in']['ts']) { $in_date = date('Y-m-d h:i A', $checkin_json['in']['ts']); } else { $in_date = ''; }
		if ($checkin_json['out']['ts']) { $out_date = date('Y-m-d h:i A', $checkin_json['out']['ts']); } else { $out_date = ''; }

		$status = sirius_steward_checkin_status($checkin_node);

		$tools = array();
		$paths = array();
		$paths["node/$checkin_nid/sirius_steward_log_checkin"] = t('View');
		foreach ($paths as $path => $title) {
			if (drupal_valid_path($path)) {
				$tools[] = l($title, $path, array('attributes' => array('class' => array('tool-button'))));
			}
		}

		$row = array();
		if (!$options['hide_facility']) { $row[] = $facility_node->title; }
		if (!$options['hide_worker']) { $row[] = $worker_node->title; }
		if (!$options['hide_phone']) { $row[] = sirius_phone_render($worker_node->field_sirius_phone['und'][0]['value']); }
		if (!$options['hide_employer']) { $row[] = $employer_node->title; }
		if (!$options['hide_status']) { $row[] = $status_list[$status]; }
		if (!$options['hide_in']) { $row[] = $in_date; }
		if (!$options['hide_out']) { $row[] = $out_date; }
		if (!$options['hide_tools']) { $row[] = join(' ', $tools); }

		if ($status == 'in') { $class = 'green'; } else { $class = 'grey'; }
		$rows[] = array('data' => $row, 'class' => array($class));
	}

	$header = array();
	if (!$options['hide_facility']) { $header[] = t('Facility'); }
	if (!$options['hide_worker']) { $header[] = t('Steward'); }
	if (!$options['hide_phone']) { $header[] = t('Phone'); }
	if (!$options['hide_employer']) { $header[] = t('Employer'); }
	if (!$options['hide_status']) { $header[] = t('Status'); }
	if (!$options['hide_in']) { $header[] = t('In'); }
	if (!$options['hide_out']) { $header[] = t('out'); }
	if (!$options['hide_tools']) { $header[] = t('Tools'); }

	return sirius_table_render($header, $rows, array('class' => array('sirius_table_align')));
}

function sirius_steward_checkin_render_form($checkin_node, $options = array()) {
	$facility_node = sirius_log_handler($checkin_node, 'sirius_dispatch_facility');
	$worker_node = sirius_log_handler($checkin_node, 'sirius_worker');
	$employer_node = sirius_log_handler($checkin_node, 'grievance_shop');
	$checkin_json = sirius_json_get($checkin_node);

	$intro_html = '';

	if ($checkin_node->field_sirius_type['und'][0]['value'] == 'in') { $status_text = t('Checked in'); } else { $status_text = t('Checked out'); }
	$intro_html .= sirius_fakerender_field('Steward:', $worker_node->title);
	$intro_html .= sirius_fakerender_field('Facility:', $facility_node->title);
	$intro_html .= sirius_fakerender_field('Employer:', $employer_node->title);
	$intro_html .= sirius_fakerender_field('Status:', $status_text);

	foreach (array('in', 'out') as $key) {
		if ($checkin_json[$key]['ts']) {
			$intro_html .= sirius_fakerender_field("Check$key at:", date('l, F d h:i a', $checkin_json[$key]['ts']));

			// If we're showing the map, show the distance.
			// @todo: I think probably we should show/hide map and distance based on permissions, not on display settings.
			//        We probably want permission 'sirius steward checkin view map'
			if (!$options['hide_map']) {
				$result = sirius_steward_checkin_distance($checkin_node, $facility_node, $key);
				if ($result['success']) { $msg = number_format($result['distance'], 0) . ' feet'; } else { $msg = '<em>' . $result['msg'] . '</em>'; }
				$intro_html .= sirius_fakerender_field("Check$key Distance:", $msg);
			}
		}
	}

	$path = 'node/' . $checkin_node->nid . '/sirius_steward_log_checkin';
	if ($path != current_path()) {
		if (drupal_valid_path($path)) {
			$intro_html .= l('View Checkin Record', $path, array('attributes' => array('class' => array('tool-button'))));
		}
	}

	$status = sirius_steward_checkin_status($checkin_node);

	$form['intro'] = array('#type' => 'fieldset', '#title' => t('Status'));
	$form['intro']['status'] = array('#markup' => $intro_html);

	/////////////////////////////
	// Disclaimers
	/////////////////////////////
	
	if ($checkin_json['in']['disclaimer'] && !$options['hide_disclaimer']) {
		$form['disclaimer'] = array(
			'#type' => 'fieldset',
			'#title' => t('Disclaimer'),
			'#collapsible' => TRUE,
			'#collapsed' => TRUE,
		);
		$disclaimer_nodes = node_load_multiple(array_keys($checkin_json['in']['disclaimer']));
		$accepted_all = TRUE;
		foreach ($disclaimer_nodes as $disclaimer_nid => $disclaimer_node) {
			$disclaimer_json = sirius_json_get($disclaimer_node)['disclaimer'];
			$acceptance_json = $checkin_json['disclaimer'][$disclaimer_nid]['acceptances'][0];
			if (!$acceptance_json) { $accepted_all = FALSE; }
			$form['disclaimer'][$disclaimer_nid] = sirius_disclaimer_render_form($disclaimer_json, $acceptance_json);			
		}
	}

	/////////////////////////////
	// Custom Fields
	/////////////////////////////
	
	if (!$options['hide_customfield']) {
		$cf_sets = sirius_customfield_set_list($checkin_node);
		$cf_nodes = node_load_multiple(array_keys($cf_sets));

		foreach ($cf_nodes as $cf_nid => $cf_node) {
			$cf_json = sirius_json_get($cf_node);
			$cf_form = sirius_jsonfield_util_form_get($cf_json, 'default');
			$entity_results = sirius_jsonfield_util_results_get($checkin_json, "customfield_$cf_nid");
			if ($entity_results) {
			  sirius_jsonfield_util_set_defaults($cf_form, $entity_results);
			  /*
		  	$form['customfield'][$cf_nid] = array(
		  		'#type' => 'fieldset', 
		  		'#title' => $cf_node->title,
		  		'#collapsible' => TRUE,
		  		'#collapsed' => FALSE,
		  	);
		  	*/
		  	$options = array();
		  	$options['skip_empty'] = TRUE;
				$options['fieldsets'] = TRUE;
		  	$form['customfield'][$cf_nid]['results'] = array('#markup' => sirius_jsonfield_util_render_all_values_new($cf_form, $entity_results, $options));
			}
		}
	}

	/////////////////////////////
	// Map
	/////////////////////////////
	
	if ($checkin_json['in']['geolocation']['latlon'] && sirius_domain_component_enabled('mapping') && !$options['hide_map']) {
		$form['map'] = array(
			'#type' => 'fieldset',
			'#title' => t('Map'),
			'#collapsible' => TRUE,
			'#collapsed' => TRUE,
		);

		drupal_add_js(drupal_get_path('module', 'sirius') . '/js/map.js', array('type' => 'file', 'weight' => -5));
		$js_settings = array();
		drupal_add_js(array('sirius_map' => $js_settings), 'setting');

		$popup_html = 'Checkin Location';

		$pins = $pin_text = $pin_names = array();
		$pins['facility'] = sirius_map_pin('bulb-blue');
		$pins['in'] = sirius_map_pin('bulb-green');
		$pins['out'] = sirius_map_pin('bulb-red');

		$pin_names['facility'] = t('Facility');
		$pin_names['in'] = t('Checkin'); 
		$pin_names['out'] = t('Checkout');

		$map_features = array();

		// Render facility
		$key = 'facility';
		if ($facility_node->field_sirius_address_geo['und'][0]['lat']) {
			$pin_text[$key] = $facility_node->title;
			$map_feature = array(
			  'type' => 'point',
			  'lat' => $facility_node->field_sirius_address_geo['und'][0]['lat'],
			  'lon' => $facility_node->field_sirius_address_geo['und'][0]['lon'],
			  'popup' => $pin_text[$key],
		    'icon' => sirius_map_pin_render_leaflet($pins[$key]),
				'icon_name' => $pin_names[$key],
			);
			$map_features[] = $map_feature;
		} else {
			$pin_text[$key] = t("Facility location is not shown, because the facility has not been geocoded.");
		}

		// Render checkin and out
		$tmp = array('in');
		if ($status == 'out') { $tmp[] = 'out'; }
		foreach ($tmp as $key) {
			if ($checkin_json[$key]['geolocation']['latlon']['lat']) {
				$pin_text[$key] = $worker_node->title;
				if ($checkin_json[$key]['ts']) { $pin_text[$key] .= ' at ' . date('l, F d h:i a', $checkin_json[$key]['ts']); }
				$map_feature = array(
				  'type' => 'point',
				  'lat' => $checkin_json[$key]['geolocation']['latlon']['lat'],
				  'lon' => $checkin_json[$key]['geolocation']['latlon']['lon'],
				  'popup' => $pin_text[$key],
			    'icon' => sirius_map_pin_render_leaflet($pins[$key]),
					'icon_name' => $pin_names[$key],
				);
				$map_features[] = $map_feature;
			} else {
				if ($checkin_json[$key]['geolocation']['latlon']['err']) {
					$pin_text[$key] = '<em>' . t('Not shown') . ': ' . $checkjson_in['geolocation_in']['latlon']['err'] . '</em>';
				} else {
					$pin_text[$key] = '<em>' . t('Not shown.') . '</em>';
				}
			}
		}


		if ($map_features) {
			$map = sirius_map_leaflet_create();
		  $map_build = leaflet_build_map($map, $map_features, sirius_domain_variable_get('sirius_map_height', '400px'));
			$html = render($map_build);
			$form['map']['render'] = array('#markup' => $html);
		}

		foreach ($pins as $key => $pin) {
			$row = array();
			$row[] = sirius_map_pin_render($pin);
			$row[] = $pin_names[$key];
			$row[] = $pin_text[$key];
			$rows[] = $row;
		}

		$header = array();
  	$html = theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array()));
		$form['map']['legend'] = array('#markup' => $html);
	}

	return $form;
}

function sirius_steward_checkin_distance($checkin_node, $facility_node, $which) {
	$facility_lat = $facility_node->field_sirius_address_geo['und'][0]['lat'];
	$facility_lon = $facility_node->field_sirius_address_geo['und'][0]['lon'];
	if (!$facility_lat && !$facility_lon) { return array('success' => FALSE, 'msg' => 'Facility is not geocoded.'); }

	$checkin_json = sirius_json_get($checkin_node);
	$checkin_lat = $checkin_json[$which]['geolocation']['latlon']['lat'];
	$checkin_lon = $checkin_json[$which]['geolocation']['latlon']['lon'];
	if (!$checkin_lat && !$checkin_lon) { return array('success' => FALSE, 'msg' => 'Steward\'s location was not recorded.'); }

	$distance = sirius_addr_latlon_distance($facility_lat, $facility_lon, $checkin_lat, $checkin_lon, 'F');
	return array(
		'success' => TRUE, 
		'distance' => $distance,
	);
}