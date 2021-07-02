<?php

function sirius_event_widget_info($rule_type, $widget) {
	if ($rule_type == 'register') {
		return sirius_event_register_widget_info($widget);
	} else {
		return sirius_event_participants_widget_info($widget);
	}
}

function sirius_event_widget_settings($event_node, $rule_type, $delta, $widget) {
	$event_type_term = taxonomy_term_load($event_node->field_sirius_event_type['und'][0]['tid']);
	return sirius_event_type_widget_settings($event_type_term, $rule_type, $delta, $widget);
}

function sirius_event_type_widget_settings($event_type_term, $rule_type, $delta, $widget) {
	$event_type_json = sirius_json_get($event_type_term);
	$widget_settings = $event_type_json['widget_settings']['participants'][$delta]['mightysearch'];
	return $widget_settings;
}