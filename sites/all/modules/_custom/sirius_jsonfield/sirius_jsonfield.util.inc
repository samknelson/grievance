<?php

function sirius_jsonfield_util_set_from_path(&$json, $path, $value) {
	$path = preg_replace('/^:/', '', $path);
	$parts = explode(':', $path);
	$current = array_shift($parts);
	$path = join(':', $parts);
	if ($path) { sirius_jsonfield_util_set_from_path($json[$current], $path, $value); }
	else { $json[$current] = $value; }
}

function sirius_jsonfield_util_unset_from_path(&$json, $path) {
	$path = preg_replace('/^:/', '', $path);
	$parts = explode(':', $path);
	$current = array_shift($parts);
	$path = join(':', $parts);
	if ($path) { sirius_jsonfield_util_unset_from_path($json[$current], $path); }
	else { unset($json[$current]); }
}

function sirius_jsonfield_util_get_from_path($json, $path) {
	$path = preg_replace('/^:/', '', $path);
	$parts = explode(':', $path);
	$current = array_shift($parts);
	$path = join(':', $parts);
	if ($path) { return sirius_jsonfield_util_get_from_path($json[$current], $path); }
	else { return $json[$current]; }
}

function sirius_jsonfield_util_type_has_data($type) {
	return in_array($type, array('select', 'textfield', 'textarea', 'date_popup', 'radios', 'checkboxes'));
}

function sirius_jsonfield_util_form_list_paths($json, $data_fields_only = FALSE) {
	$result = array();
	sirius_jsonfield_util_form_list_paths_recursive($result, $json, NULL, $data_fields_only);
	return $result;
}

function sirius_jsonfield_util_form_list_paths_recursive(&$result, $json, $path = '', $data_fields_only = NULL) {
	if (!$json) { return; }
  if (!is_array($json)) { return; }
	sirius_jsonfield_util_form_sort_by_weight($json);
	$type = $json['#type'];
	$id = $json['#id'];
	if ($id) { 
		if (!$data_fields_only || sirius_jsonfield_util_type_has_data($type)) { $result[$id] = $path; }
	}
	if ($type && ($type != 'fieldset')) { return; }

  foreach ($json as $key => $value) {
  	if (!is_array($value)) { continue; }
  	if (preg_match('/^#/', $key)) { continue; }
    sirius_jsonfield_util_form_list_paths_recursive($result, $value, $path . ':' . $key, $data_fields_only);
  }
}

function sirius_jsonfield_util_form_sort_by_weight(&$json) {
	if (!is_array($json)) { return; }
	uksort($json, function($a, $b) use ($json) { 
		if (isset($json[$a]['#weight'])) { $wa = $json[$a]['#weight']; } else { $wa = 0; }
		if (isset($json[$b]['#weight'])) { $wb = $json[$b]['#weight']; } else { $wb = 0; }
		if ($wa < $wb) { return -1; }
		if ($wa > $wb) { return 1; }
		return 0;
	});
}

function sirius_jsonfield_util_quote($str) {
	$conn = Database::getConnection();
	$safe = $conn->quote($str);
	$safe = preg_replace('/^\'/', '\'"', $safe);
	$safe = preg_replace('/\'$/', '"\'', $safe);
	return $safe;
}

function sirius_jsonfield_util_value_list_paths($json) {
	$result = array();
	sirius_jsonfield_util_value_list_paths_recursive($result, $json);
	return $result;
}

function sirius_jsonfield_util_value_list_paths_recursive(&$result, $json, $path = '') {
  if (!is_array($json)) { return; }

  foreach ($json as $key => $value) {
  	if (is_array($value)) {
  		sirius_jsonfield_util_value_list_paths_recursive($result, $value, $path . ':' . $key);
  	} else {
  		$result[] = "$path:$key";
  	}
  }
}

function sirius_jsonfield_util_form_flatten($json) {
	$result = array();
	$paths = sirius_jsonfield_util_form_list_paths($json);
	foreach ($paths as $id => $path) {
		// We don't trust the paths. Sigh.
		$new_id = preg_replace('/:/', '__', 'sirius_json_id' . $path);
		$result[$new_id] = sirius_jsonfield_util_get_from_path($json, $path);
		foreach ($result[$new_id] as $key => $value) {
			if (!preg_match('/^#/', $key)) { unset($result[$new_id][$key]); }
		}
		$result[$new_id]['#path'] = $path;
	}
	return $result;
}

function sirius_jsonfield_util_render_value($form_element, $value, $options = array()) {
	$id = $form_element['#id'];
	$title = $form_element['#title'] . ':';

	if ($form_element['#type'] == 'fieldset') {
		if ($options['raw']) { return NULL; }
		else { return '<hr><h2>' . $form_element['#title'] . '</h2>'; }
	}

	if (is_array($value)) {
		if ($form_element['#type'] == 'date_popup') { $value_rendered = $value['date'] . ' ' . $value['time']; }
		else if ($form_element['#type'] == 'checkboxes') { 
			foreach ($value as $a => $b) {
				if ($b) {
					if ($value_rendered) { $value_rendered .= ' / '; }
					$value_rendered .= $b;
				}
			}
		}
		else { $value_rendered = '<pre>' . print_r($value, 1) . '</pre>'; }
	} else { $value_rendered = $value; }

	if ($options['raw']) { return $value_rendered; }

	return sirius_fakerender_field($title, $value_rendered, $options);
}


function sirius_jsonfield_util_set_defaults(&$form, $defaults) {
	if (!is_array($form)) { return; }
	foreach ($form as $key => $value) {
		if (!is_array($form[$key])) { continue; }
		$type = $form[$key]['#type'];
		if ($type && $type != 'fieldset') {
			if ($type == 'select' || $type == 'textfield' || $type == 'textarea' || $type == 'date_popup' || $type == 'radios' || $type == 'checkboxes')  {
				if (isset($defaults[$key])) {
					if ($type == 'date_popup' && is_array($defaults[$key])) { $form[$key]['#default_value'] =  date('Y-m-d H:i:s', strtotime($defaults[$key]['date'] . ' ' . $defaults[$key]['time'])); } 
					else { $form[$key]['#default_value'] = $defaults[$key]; }
				}
			}
		} else {
			sirius_jsonfield_util_set_defaults($form[$key], $defaults[$key]);
		}
	}
}

function sirius_jsonfield_util_render_values($form, $values, $field_list, $options = array()) {
	$html = '';
	if ($field_list) {
		foreach ($field_list as $path) {
			$form_element = sirius_jsonfield_util_get_from_path($form, $path);
			$value = sirius_jsonfield_util_get_from_path($values, $path);
			$html .= sirius_jsonfield_util_render_value($form_element, $value, $options);
		}
	}

	return $html;
}

function sirius_jsonfield_util_add_node_fields($node, &$form, &$values) {
	$node_fields = sirius_jsonfield_util_list_node_fields($node->type);
	foreach ($node_fields as $path => $title) {
		list ($empty, $node_type, $field_name, $col) = explode(':', $path);
		$form[$node_type][$field_name][$col] = array('#type' => 'textfield', '#title' => $title);
		$value = '';
		if ($field_name == 'title') { $value = $node->title; }
		else if ($field_name == 'nid') { $value = $node->nid; }
		else { $value = sirius_fieldval($node, $field_name, $col); }
		$values[$node_type][$field_name][$col] = $value;
	}
}

function sirius_jsonfield_util_list_node_fields($node_type) {
	$fields = array();
	if ($node_type == 'sirius_contact') {
		$fields = array(
			":$node_type:nid" => 'Internal ID',
			":$node_type:title:value" => 'Name',
			":$node_type:field_sirius_id:value" => 'ID',
			":$node_type:field_sirius_phone:value" => 'Phone',
			":$node_type:field_sirius_email:value" => 'Email',
			":$node_type:field_sirius_address:country" => 'Country',
			":$node_type:field_sirius_address:administrative_area" => 'State',
			":$node_type:field_sirius_address:locality" => 'City',
			":$node_type:field_sirius_address:thoroughfare" => 'Address',
			":$node_type:field_sirius_address:postal_code" => 'Zip',
			":$node_type:field_sirius_source:value" => 'Source',
		);
	} else if ($node_type == 'sirius_worker') {
		$fields = array(
			":$node_type:nid" => 'Internal ID',
			":$node_type:title:value" => 'Name',
			":$node_type:field_sirius_id:value" => 'ID',
			":$node_type:field_sirius_phone:value" => 'Phone',
			":$node_type:field_sirius_email:value" => 'Email',
			":$node_type:field_sirius_address:country" => 'Country',
			":$node_type:field_sirius_address:administrative_area" => 'State',
			":$node_type:field_sirius_address:locality" => 'City',
			":$node_type:field_sirius_address:thoroughfare" => 'Address',
			":$node_type:field_sirius_address:postal_code" => 'Zip',
			":$node_type:field_sirius_work_status" => 'Work Status',
			":$node_type:field_sirius_member_status" => 'Member Status',
		);
	}

	return $fields;
}


function sirius_jsonfield_util_render_all_values($form, $values, $options = array()) {
	$paths = drupal_map_assoc(sirius_jsonfield_util_form_list_paths($form));
	return sirius_jsonfield_util_render_values($form, $values, $paths, $options);
}


///////////////////////////////////////////////////////////////////////////////

function sirius_jsonfield_util_form_get($json, $which) {
	if (!is_array($json) || !is_array($json[$which])) { return NULL; }
	return json_decode($json[$which]['form_encoded'], TRUE);
}

function sirius_jsonfield_util_form_set(&$json, $which, $form) {
	$json[$which]['form_encoded'] = json_encode($form);
}

function sirius_jsonfield_util_results_get($json, $which) {
	if (!is_array($json) || !is_array($json[$which])) { return NULL; }
	return json_decode($json[$which]['results_encoded'], TRUE);
}

function sirius_jsonfield_util_results_set(&$json, $which, $results) {
	$json[$which]['results'] = $results;
	$json[$which]['results_encoded'] = json_encode($results);
}

function sirius_jsonfield_util_display_get($json, $which) {
	if (!is_array($json) || !is_array($json[$which])) { return NULL; }
	return json_decode($json[$which]['display_encoded'], TRUE);
}

function sirius_jsonfield_util_display_set(&$json, $which, $display) {
	$json[$which]['display_encoded'] = json_encode($display);
}

function sirius_jsonfield_util_esignature_get($json, $which) {
	if (!is_array($json) || !is_array($json[$which])) { return NULL; }
	return json_decode($json[$which]['esignature_encoded'], TRUE);
}

function sirius_jsonfield_util_esignature_set(&$json, $which, $esignature) {
	$json[$which]['esignature_encoded'] = json_encode($esignature);
}

function sirius_jsonfield_util_node_json_get($node) {
	return json_decode($node->field_sirius_json['und'][0]['value'], TRUE);
}

function sirius_jsonfield_util_node_json_set(&$node, $json) {
	$node->field_sirius_json['und'][0]['value'] = json_encode($json);
}

function sirius_jsonfield_util_term_json_get($term) {
	return json_decode($term->field_sirius_json['und'][0]['value'], TRUE);
}

function sirius_jsonfield_util_term_json_set(&$node, $json) {
	$node->field_sirius_json['und'][0]['value'] = json_encode($json);
}

function sirius_jsonfield_util_user_json_get($user) {
	return json_decode($user->field_sirius_json['und'][0]['value'], TRUE);
}

function sirius_jsonfield_util_user_json_set(&$user, $json) {
	$user->field_sirius_json['und'][0]['value'] = json_encode($json);
}

