<?php

function sirius_jsonfield_util_set_from_path(&$json, $path, $value) {
	$path = preg_replace('/^:/', '', $path);
	$parts = explode(':', $path);
	$current = array_shift($parts);
	$path = join(':', $parts);
	if ($path) { sirius_jsonfield_util_set_from_path($json[$current], $path, $value); }
	else { $json[$current] = $value; }
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
    sirius_jsonfield_util_form_list_paths_recursive($result, $value, $path . ':' . $key);
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