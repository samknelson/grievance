<?php

function sirius_feed_worker_skill_row_verify($feed_node, $info, $data, &$row, $precompute) {
  $pk = $data['pk'];
  if (!$pk) { $pk = 'id'; }

	// Trim all
	foreach ($row as $key => $value) { if (is_string($value)) { $row[$key] = trim($value); } }

  $row['ssn'] = sirius_ssn_format($row['ssn']);
  if ($pk == 'id') {
	 if (!preg_match('/^\d+$/', $row['id'])) { return array('success' => FALSE, 'msg' => 'Invalid Member ID'); }
  } else if ($pk == 'ssn') {
    if (!sirius_ssn_validate($row['ssn'])) { return array('success' => FALSE, 'msg' => 'Invalid SSN'); }
  }

  // Worker
  if ($precompute) {
    if ($pk == 'id') {
    	$sql = "select max(entity_id) as a, min(entity_id) as b from field_data_field_sirius_id ";
    	$sql .= "where entity_type = 'node' ";
    	$sql .= "and bundle = 'sirius_worker' ";
    	$sql .= "and field_sirius_id_value = :id ";
    	$stmt = db_query($sql, array(':id' => $row['id']));
    	$hr = $stmt->fetchAssoc();
    	$nid = $hr['a'];
    	$other_nid = $hr['b'];
    	if ($nid != $other_nid) {
    		return array('success' => FALSE, 'msg' => 'Multiple workers found with this ID.');
    	}
    	$row['worker_nid'] = $nid;
    } else if ($pk == 'ssn') {
      $sql = "select max(entity_id) as a, min(entity_id) as b from field_data_field_sirius_ssn ";
      $sql .= "where entity_type = 'node' ";
      $sql .= "and bundle = 'sirius_worker' ";
      $sql .= "and field_sirius_ssn_value = :ssn ";
      $stmt = db_query($sql, array(':ssn' => $row['ssn']));
      $hr = $stmt->fetchAssoc();
      $nid = $hr['a'];
      $other_nid = $hr['b'];
      if ($nid != $other_nid) {
        return array('success' => FALSE, 'msg' => 'Multiple workers found with this SSN.');
      }
      $row['worker_nid'] = $nid;
    }
    if (!$row['worker_nid']) { return array('success' => FALSE, 'msg' => 'Worker not found'); }

    $reason = $data['source'];
    if ($row['skill_comment']) {
      if ($reason) { $reason .= ': '; }
      $reason .= $row['skill_comment'];
    }
    if (!$reason) { $reason = 'Feed import'; }
    $row['reason'] = $reason;
  } 

  $skill_tid = sirius_taxonomy_lookup_by_anything('sirius_skill', $row['skill']);
  if (!$skill_tid) { return array('success' => FALSE, 'msg' => 'Skill [' . $row['skill'] . '] does not match the TID, name, short name, or ID of any skill taxonomy term.'); }
  if ($precompute) { $row['skill_tid'] = $skill_tid; }

  $action = $row['skill_action'];
  if (!$action) { $action = 'add'; }
  if ($action != 'add' && $action != 'remove') { return array('success' => FALSE, 'msg' => 'Action must be add or remove.'); }

  return array('success' => TRUE, 'msg' => 'Looks good.');
}

function sirius_feed_worker_skill_row_process($feed_node, $info, $data, $row) {
  $result = sirius_feed_worker_skill_row_verify($feed_node, $info, $data, $row, TRUE);
  if (!$result['success']) { return $result; }

  $worker_node = node_load($row['worker_nid']);
  $skill_tid = $row['skill_tid'];
  $expiration = $row['skill_expiration'];
  $action = $row['skill_action'];
  if (!$action) { $action = 'add'; }
  $reason = $row['reason'];
  return sirius_worker_skill_set($worker_node, array($skill_tid), $expiration, $action, $reason, array());
}
