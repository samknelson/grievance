<?php

function sirius_event_term_tab_widgets($event_type_term) {
	$event_type_json = sirius_jsonfield_util_term_json_get($event_type_term);

	$html .= sirius_event_term_tab_widgets_quicknav($event_type_json, 'taxonomy/term/' . $event_type_term->tid . '/sirius_event_type_widgets');

	$rule_type = sirius_node_tab_arg(0);
	$delta = sirius_node_tab_arg(1);
	$widget = sirius_node_tab_arg(2);

	if ($rule_type && isset($delta) && $widget) {
		$info = sirius_event_widget_info($rule_type, $widget);
		$html .= drupal_render(drupal_get_form($info['config_callback'], $event_type_term, $rule_type, $delta, $widget));
	} else {
		$html .= "Some widgets can have additional configuration options. Please select the widget from the list above.";
	}

	return $html;
}

function sirius_event_term_tab_widgets_quicknav($event_type_json, $base_path) {
	$links = array();
	$link = array();
	$link['title'] = 'All Widgets';
	$link['path'] = $base_path;
	$links[] = $link;

	foreach (array('register' => 'Register', 'participants' => 'Participant') as $rule_type => $rule_type_name) {
		foreach ($event_type_json[$rule_type] as $rules) {
			foreach ($rules as $delta => $rule) {
				foreach ($rule['widget'] as $widget) {
					$info = sirius_event_widget_info($rule_type, $widget);
					if ($info['config_callback']) {
						$link = array();
						$link['title'] = $rule_type_name . ' / ' . $rule['name'] . ' / ' . $widget;
						$link['path'] = $base_path . "/$rule_type/$delta/$widget";
						$links[] = $link;
					}
				}
			}
		}
	}

	return sirius_quicknav($links);
}
