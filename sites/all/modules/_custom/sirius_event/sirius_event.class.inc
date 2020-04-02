<?php

function sirius_event_class_enabled_nid($event_nid) { return sirius_event_class_enabled(node_load($event_nid)); }

function sirius_event_class_enabled($event_node) { return sirius_event_class_enabled_tid($event_node->field_sirius_event_type['und'][0]['tid']); }

function sirius_event_class_enabled_tid($event_type_tid) { return sirius_event_class_enabled_type(taxonomy_term_load($event_type_tid)); }

function sirius_event_class_enabled_type($event_type_term) {
  $json = sirius_jsonfield_util_term_json_get($event_type_term);
  return $json['class']['enabled'] == 'Yes';
}

function sirius_event_class_ts($event_node) {
	$ts = sirius_date_ts($event_node, 'field_sirius_daterepeat');
	$signin_opens_self = $ts - variable_get('sirius_dispatch_class_signin_opens', 60*60*24*5);
	$signin_opens_others = $ts - variable_get('sirius_dispatch_class_signin_opens_others', 60*60);
	$signin_closes = $ts + variable_get('sirius_dispatch_class_signin_grace', 60*15);

	return array(
		'job_ts' => $ts,
		'signin_opens_self' => $signin_opens_self,
		'signin_opens_others' => $signin_opens_others,
		'signin_closes' => $signin_closes,
	);
}

function sirius_event_get_class_types() {
	$event_type_terms = sirius_taxonomy_load('sirius_event_type', 'tid', 'full');
	$class_type_terms = array();
	foreach ($event_type_terms as $event_type_tid => $event_type_term) {
		if (sirius_event_class_enabled_type($event_type_term)) { $class_type_terms[$event_type_tid] = $event_type_term; }
	}

	return $class_type_terms;
}