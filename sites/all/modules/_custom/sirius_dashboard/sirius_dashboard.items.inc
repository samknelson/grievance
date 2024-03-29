<?php

function sirius_dashboard_items_by_role($rid) {
	return json_decode(sirius_domain_variable_get("sirius_dashboard_items_by_rid_$rid"), TRUE);
}

/*
function sirius_dashboard_item_render($json, $uuid, $options = array()) {
	drupal_add_css(drupal_get_path('module', 'sirius_dashboard') . '/css/dashboard.css');

	$html = '<div class="sirius_dashboard_item">';
	$html .= '<div class="sirius_dashboard_item_title">';
	$html .= $json['item_list'][$uuid]['title'];
	$html .= '</div>';
	$html .= sirius_dashboard_item_render_body($json, $uuid, $options);
	$html .= '</div>';

	return $html;
}
*/

function sirius_dashboard_item_data($json, $uuid, $options = array()) {
	$plugin_key = $json['item_list'][$uuid]['plugin'];
	if (!$plugin_key) { return '[no plugin for this dashboard item has been assigned.]'; }
	$plugin_info = sirius_dashboard_plugin_info($plugin_key);
	$callback = $plugin_info['callbacks']['data'];
	if (!$callback) { return '[no callback for this dashboard item has been defined.]'; }

	sirius_require($plugin_info['module'], $plugin_info['file']);

	if (sirius_proxy_active() && $plugin_info['proxy']) {
		$result = sirius_proxy_call('sirius_dashboard_item_data', $json, $uuid, $options);
		if ($result['success']) {
			$result = $result['data'];
		} else {
			$result = array(
				array(
					'count' => 0,
					'tools' => '',
					'details' => '<em><strong>ERROR:</strong> ' . $result['msg'] . '</em>',
				)
			);
		}
	} else {
		$result = $callback($json['item'][$uuid]['preferences'], $options);
	}

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