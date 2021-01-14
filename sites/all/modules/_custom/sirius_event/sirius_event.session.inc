<?php

function sirius_event_session_enabled_tid($event_type_tid) { return sirius_event_session_enabled_type(taxonomy_term_load($event_type_tid)); }

function sirius_event_session_enabled_type($event_type_term) {
  $json = sirius_jsonfield_util_term_json_get($event_type_term);
  return $json['session']['enabled'] == 'Yes';
}


function sirius_event_session_enabled_nid($event_nid) { return sirius_event_session_enabled(node_load($event_nid)); }

function sirius_event_session_enabled($event_node) {
	$session_json = sirius_event_session_json($event_node); 
	return $session_json['enabled'] == 'Yes';
}

function sirius_event_session_json($event_node) {
	$json = sirius_jsonfield_util_node_json_get($event_node);
	return $json['session'];
}

