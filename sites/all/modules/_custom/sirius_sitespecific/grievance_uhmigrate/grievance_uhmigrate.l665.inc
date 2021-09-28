<?php

function grievance_uhmigrate_665cleanup($node, $row, $subkey = 'main') {
  if ($subkey == 'main') {
  	// Clear the log
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'node');
    $query->entityCondition('bundle', array('sirius_log'));
    $query->fieldCondition('field_sirius_log_handler', 'target_id', $node->nid);
    $stmt = $query->execute();
    if ($stmt['node']) {
      node_delete_multiple(array_keys($stmt['node']));
    }

    // Set the creation date
    $intake_ts = strtotime($row['DateAdd']);
    if ($intake_ts) {
      $sql = "update node set created = :intake_ts where nid = :nid";
      $stmt = db_query($sql, array(':intake_ts' => $intake_ts, ':nid' => $node->nid));
    }
  }

  // Dump everything into "Comments"
  $comments = print_r($row, 1);
  $comments = mb_convert_encoding($comments, 'UTF-8', 'UTF-8');
  sirius_log(
    'import', 
    "raw:$subkey", 
    'Raw record import', 
    '', 
    '<pre>' . $comments . '</pre>', 
    array($node->nid)
  );

}
