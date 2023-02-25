<?php

function report_cache_settings_nid($report_type, $create = FALSE) {
	$domain_nid = sirius_domain_nid();

	$type = 'report_cache:settings:' . $report_type;

  $sql = "select nid ";
  $sql .= "from node ";
  $sql .= "left join field_data_field_sirius_type on field_data_field_sirius_type.entity_type = 'node' and field_data_field_sirius_type.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
  $sql .= "where node.type = 'sirius_json_definition' ";
  $sql .= "and field_sirius_type_value = :type ";
  $sql_args[':type'] = $type;
  if ($domain_nid) {
  	$sql .= "and field_sirius_domain_target_id = :domain_nid ";
  	$sql_args[':domain_nid'] = $domain_nid;
  } else {
  	$sql .= "and field_sirius_domain_target_id is null ";
  }
  $sql .= "order by created desc limit 1";
  $stmt = sirius_sql_query($sql, $sql_args);
  $hr = $stmt->fetchAssoc();
  if ($hr['nid']) { return $hr['nid']; }

	if (!$create) { return NULL; }

	global $user;
  $node = new stdClass();
  $node->type = 'sirius_json_definition';
  $node->title = 'Report - Settings - ' . $report_type;
  $node->no_recurse = TRUE;
  $node->language = LANGUAGE_NONE;
  $node->uid = $user->uid;
  $node->status = 1;
  $node->promote = 0;
  $node->comment = 0;
  $node->field_sirius_type['und'][0]['value'] = $type;
  if ($domain_nid) { $node->field_sirius_domain['und'][0]['target_id'] = $domain_nid; }
  $node = node_submit($node);
  node_object_prepare($node);

  // /////////// Begin legacy support
  $json = json_decode(sirius_domain_variable_get("sirius_report_" . $report_type . "_settings"), TRUE);
  sirius_json_set($node, $json);
  // /////////// End legacy supoprt

  node_save($node);
  return $node->nid;
}

function report_cache_settings_node($report_type, $create = FALSE) {
	return node_load(report_cache_settings_nid($report_type, $create));
}


function report_cache_settings($report_type, $new_settings = array()) {
	$node = report_cache_settings_node($report_type, TRUE);
	if (!$new_settings) { return sirius_json_get($node); }
	sirius_json_set($node, $new_settings);
	node_save($node);
	return $new_settings;
}
