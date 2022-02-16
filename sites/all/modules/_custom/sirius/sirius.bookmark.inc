<?php

function sirius_bookmark_view_form_alter(&$form, &$form_state) {
	if ($form['type']['#options']) {
		$form['type']['#options'] = array('All' => '- Any -') + sirius_bookmark_type_options();
	}
}

function sirius_bookmark_view_pre_render(&$view) {
	$type_options = sirius_bookmark_type_options();
	if ($view->result) {
    foreach ($view->result as $delta => $row) {
      if ($row->node_type) {
        if ($type_options[$row->node_type]) {
        	$row->node_type = $type_options[$row->node_type];
        }
      }
    }
  }
}

/**
 * When you change this, don't forget to also modify the flag (admin/structure/flags/manage/bookmarks) to allow the additional node types to be referenced.
 */

function sirius_bookmark_types() {
	return array('sirius_worker', 'grievance_shop', 'sirius_contact', 'sirius_phonenumber');
}

function sirius_bookmark_type_options() {
	$types = sirius_bookmark_types();
	$options = array();
	foreach ($types as $type) {
		$options[$type] = sirius_content_type_name($type);
	}
	return $options;
}

function sirius_bookmark_flag_id() {
	$fid = &drupal_static(__FUNCTION__);
  if (!isset($fid)) {
		$sql = "select fid from flag where name = 'bookmarks'";
		$stmt = sirius_sql_query($sql, array());
		$hr = $stmt->fetchAssoc();
		$fid = $hr['fid'];
	}
	return $fid;
}

function sirius_bookmark_recent($limit = 0) {
	global $user;
	if (!$user || !$user->uid) { return array(); }
	$fid = sirius_bookmark_flag_id();

	if (!$limit) { $limit = 3; }
	$fid = sirius_bookmark_flag_id();
	$sql = "select flagging_id, nid, title from flagging ";
	$sql .= "left join node on flagging.entity_type = 'node' and flagging.entity_id = node.nid ";
	$sql .= "where fid = :fid ";
	$sql .= "and flagging.uid = :user_id ";
	$sql .= "and node.nid is not null ";
	$sql .= "order by flagging.timestamp desc limit $limit ";
	$stmt = sirius_sql_query($sql, array(':fid' => $fid, ':user_id' => $user->uid));

	$result = array();
	while ($hr = $stmt->fetchAssoc()) {
		$result[] = $hr;
	}
	return $result;
}

function sirius_bookmark_count() {
	global $user;
	if (!$user || !$user->uid) { return 0; }
	$fid = sirius_bookmark_flag_id();

	if (!$limit) { $limit = 3; }
	$fid = sirius_bookmark_flag_id();
	$sql = "select count(*) as c from flagging ";
	$sql .= "where fid = :fid ";
	$sql .= "and flagging.uid = :user_id ";
	$stmt = sirius_sql_query($sql, array(':fid' => $fid, ':user_id' => $user->uid));
	$hr = $stmt->fetchAssoc();
	return $hr['c'];
}