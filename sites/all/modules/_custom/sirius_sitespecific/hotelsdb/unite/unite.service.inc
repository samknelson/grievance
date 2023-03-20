<?php

/** 
 * Implements hook_sirius_service_func_info_all_alter
 */

function unite_sirius_service_func_info_all_alter(&$info) {
	$info['hotelsdb_service_node_list'] = array(
		'callback' => 'hotelsdb_service_node_list',
		'enabled' => 'Yes',
	);
	$info['hotelsdb_service_node'] = array(
		'callback' => 'hotelsdb_service_node',
		'enabled' => 'Yes',
	);
	$info['hotelsdb_service_term'] = array(
		'callback' => 'hotelsdb_service_term',
		'enabled' => 'Yes',
	);
}

function hotelsdb_service_node_list($page) {
	$limit = 1000;
	$page = (int) $page;
	$offset = $page * $limit;

	$sql = "select nid, type, created, changed from node order by nid limit $limit offset $offset";
	$stmt = sirius_sql_query($sql, array());

	$data = array();
	while ($hr = $stmt->fetchAssoc()) {
		$data[] = $hr;
	}
	return $data;
}

function hotelsdb_service_node($nid) {
	$nid = (int) $nid;
	$node = node_load($nid);
	return $node;
}

function hotelsdb_service_term($tid) {
	$tid = (int) $tid;
	$term = taxonomy_term_load($tid);
	return $term;
}