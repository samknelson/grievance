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

function sirius_jsonfield_util_form_list_paths($json) {
	$result = array();
	sirius_jsonfield_util_form_list_paths_recursive($result, $json);
	return $result;
}

function sirius_jsonfield_util_form_list_paths_recursive(&$result, $json, $path = '') {
	if (!$json) { return; }
  if (!is_array($json)) { return; }
	sirius_jsonfield_util_form_sort_by_weight($json);
	$type = $json['#type'];
	$id = $json['#id'];
	if ($id) { $result[$id] = $path; }
	if ($type && ($type != 'fieldset')) { return; }

  foreach ($json as $key => $value) {
  	if (!is_array($value)) { continue; }
  	if (preg_match('/^#/', $key)) { continue; }
    sirius_jsonfield_util_form_list_paths_recursive($result, $value, $path . ':' . $key, $value_only);
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

function sirius_jsonfield_util_render_value($form_element, $value) {
	if (is_array($value)) {
		if ($form_element['#type'] == 'date_popup') { return $value['date'] . ' ' . $value['time']; }
		return print_r($value, 1);
	}
	return $value;
}


function sirius_jsonfield_util_set_defaults(&$form, $defaults) {
	if (!is_array($form)) { return; }
	foreach ($form as $key => $value) {
		if (!is_array($form[$key])) { continue; }
		$type = $form[$key]['#type'];
		if ($type && $type != 'fieldset') {
			if ($type == 'select' || $type == 'textfield' || $type == 'textarea' || $type == 'date_popup' || $type == 'radios' || $type == 'checkboxes')  {
				if (isset($defaults[$key])) { $form[$key]['#default_value'] = $defaults[$key]; }
			}
		} else {
			sirius_jsonfield_util_set_defaults($form[$key], $defaults[$key]);
		}
	}
}
