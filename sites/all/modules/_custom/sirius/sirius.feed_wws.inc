<?php

function sirius_feed_wws_row_verify($feed_node, $info, $data, &$row, $precompute) {
	static $work_status_map;
	if (!$work_status_map) {
		$work_status_map = sirius_taxonomy_load('sirius_work_status', 'field_sirius_id2', 'full');
	}

	$ssn = $row['ssn'];
	if (!sirius_ssn_validate($ssn)) { return array('success' => FALSE, 'msg' => 'Invalid SSN'); }

  // Worker
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_worker');
  $query->fieldCondition('field_sirius_ssn', 'value', $ssn);
  $stmt = $query->execute();
  if (!$stmt['node']) { return array('success' => FALSE, 'msg' => 'No worker found.'); }
  if (count($stmt['node']) > 1) { return array('success' => FALSE, 'msg' => 'Multiple workers found.'); }
  if ($precompute) { $row['worker_nid'] = array_keys($stmt['node'])[0]; }

	// Find the work status
	$work_status_id = $row['work_status_id'];
	if ($work_status_id === NULL || $work_status_id === '') { return array('success' => FALSE, 'msg' => 'No work status ID'); }
	if (!$work_status_map[$work_status_id]) { return array('success' => FALSE, 'msg' => "Work status $work_status_id not found."); }
	if ($precompute) { $row['work_status_tid'] = $work_status_map[$work_status_id]->tid; }

  return array('success' => TRUE, 'msg' => 'Looks good.');
}

function sirius_feed_wws_row_process($feed_node, $info, $data, $row) {
	$result = sirius_feed_wws_row_verify($feed_node, $info, $data, $row, TRUE);
	if (!$result['success']) { return $result; }
	return sirius_worker_work_status_set(node_load($row['worker_nid']), $row['work_status_tid'], 'import', array());
}