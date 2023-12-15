<?php

function sirius_edls_node_tab_sheet_assignmentlog_access($sheet_node) {
  return sirius_access_instance('edls')->access('sheet_assignmentlog', array('sheet_node' => $sheet_node));
}

function sirius_edls_node_tab_sheet_assignmentlog($sheet_node) {
	$logger = sirius_edls_log_instance();
  $entries = $logger->fetch($sheet_node->nid);
  if (!$entries) { return '<p>' . t('There have been no assignments on this sheet.') . '</p>'; }

  $type_lookup = $logger->type_list('edls:assignment');

  $rows = array();
  foreach ($entries as $entry) {
  	$assignment = $entry['log_json']['assignment'];
  	$author = user_load($entry['log_uid']);

  	$row = array();
  	$row[] = date('m/d/Y h:i a', $entry['log_ts']);
  	$row[] = $author->name;
  	$row[] = $type_lookup[$entry['log_type']];
  	$row[] = $assignment['worker_id'];
  	$row[] = ucwords(strtolower($assignment['worker_name']));
  	$rows[] = $row;
  }

  $header = array();
  $header[] = t('Date');
  $header[] = t('User');
  $header[] = t('Action');
  $header[] = t('Worker ID');
  $header[] = t('Worker Name');

  return sirius_table_render($header, $rows);
}