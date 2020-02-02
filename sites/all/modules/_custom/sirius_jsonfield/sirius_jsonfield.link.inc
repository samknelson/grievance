<?php

function sirius_jsonfield_link_set($definition_json, $source_json, &$target_json) {
	if (!is_array($definition_json)) { return 0; }

	$count = 0;
	foreach ($definition_json as $key => $value) {
		if (!is_array($definition_json[$key])) { continue; }
		
		$type = $definition_json[$key]['#type'];
		$linked = $definition_json[$key]['#sirius']['link'];
		if ($type) {
			if ($linked == 'Yes') {
				$path = $definition_json[$key]['#sirius']['link_target'];
				$path = preg_replace('/__/', ':', $path);
				$path = preg_replace('/^sirius_json_id/', '', $path);
				$old_value = sirius_jsonfield_util_get_from_path($target_json, $path);
				$new_value = $source_json[$key];

				if ($old_value != $new_value) {
					sirius_jsonfield_util_set_from_path($target_json, $path, $new_value);
					++$count;
				}
			}
		} else {
			$count += sirius_jsonfield_link_set($definition_json[$key], $source_json[$key], $target_json);
		}
	}

	return $count;
}
