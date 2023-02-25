<?php

function report_cache_instances($report_type) {
	$json = report_cache_settings($report_type);
	$items = $json['report_cache']['instances'];
	if (!$items) { $items = array(); }
	uksort($items, function($a, $b) use ($items) { return $items[$a]['weight'] <=> $items[$b]['weight']; });
	return $items;
}

function report_cache_instance($report_type, $uuid) {
	$instances = report_cache_instances($report_type);
	return $instances[$uuid];
}

function report_cache_instances_set($report_type, $instances) {
	$json = report_cache_settings($report_type);
	$json['report_cache']['instances'] = $instances;
	report_cache_settings($report_type, $json);
}

function report_cache_instances_list($report_type) {
	$instances = report_cache_instances($report_type);
	if (!$instances) { return array(); }
	$options = array();
	foreach ($instances as $uuid => $instance) {
		$options[$uuid] = $instance['title'];
	}

	return $options;
}