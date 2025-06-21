<?php

function sirius_dashboard_items_by_role($rid) {
	return json_decode(sirius_domain_variable_get("sirius_dashboard_items_by_rid_$rid"), TRUE);
}

function sirius_dashboard_item_data($json, $uuid, $options = array()) {
	$plugin_key = $json['item_list'][$uuid]['plugin'];
	if (!$plugin_key) { return '[no plugin for this dashboard item has been assigned.]'; }
	$plugin_info = sirius_dashboard_plugin_info($plugin_key);
	$callback = $plugin_info['callbacks']['data'];
	if (!$callback) { return '[no callback for this dashboard item has been defined.]'; }

	sirius_require($plugin_info['module'], $plugin_info['file']);

	$result = $callback($json['item'][$uuid]['preferences'], $options);

	// Set the priority
	$priority_cutoffs = $json['item_list'][$uuid]['priority_cutoffs'];
	if ($priority_cutoffs && $result) {
		foreach ($result as $key => $row) {
			foreach ($priority_cutoffs as $priority => $cutoff) {
				if (!is_numeric($cutoff)) { continue; }
				if ($row['count'] >= $cutoff) {
					$result[$key]['priority'] = $priority;
				}
			}
		}
	}

	if ($result) {
		foreach ($result as $key => $row) {
			if ($row['priority'] == -1) {
				unset($result[$key]);
			}
		}
	}

	return $result;
}