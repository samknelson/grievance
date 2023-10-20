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
			$item_data = $result['data'];
		} else {
			$item_data = array(
				array(
					'count' => 0,
					'tools' => '',
					'details' => '<em><strong>ERROR:</strong> ' . $result['msg'] . '</em>',
				)
			);
		}
	} else {
		$item_data = $callback($json['item'][$uuid]['preferences'], $options);
	}

	return $item_data;
}