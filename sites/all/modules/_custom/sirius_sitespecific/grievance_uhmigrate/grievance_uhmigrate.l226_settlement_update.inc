<?php

function grievance_uhmigrate_l226_settlement_update($limit = 100) {
  // Give ourselves more memory
  ini_set('memory_limit', '4096M');

  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_settlement');
  $query->propertyOrderBy('nid');
  $stmt = $query->execute();
  if (!$stmt['node']) {
  	print "No settlements found.";
  	exit();
  }

  $settlement_nids = array_keys($stmt['node']);

  foreach ($settlement_nids as $settlement_nid) {
    // Clear the cache...
    entity_get_controller('node')->resetCache();
    
    ++$rowcount;
    if ($rowcount > $limit) { break; }

    $settlement_node = node_load($settlement_nid);
    if (!$settlement_node) { continue; }
    if ($settlement_node->field_grievance_date_1['und'][0]['value']) { 
    	print "Skipping $settlement_nid: date already set - " . $settlement_node->title . "\n";
    	continue;
    }

    $grievance_nid = $settlement_node->field_grievance['und'][0]['target_id'];
    if (!$grievance_nid) {
    	print "Skipping $settlement_nid: no grievance nid - " . $settlement_node->title . "\n";
    	continue;
    }

    // Load the log
	  $log_update_tids = array(
	    variable_get('grievance_log_insert_type', ''), 
	    variable_get('grievance_log_update_type_with_status_change', ''), 
	    variable_get('grievance_log_update_type_without_status_change', ''), 
	    variable_get('grievance_log_incident_type', '')
	  );
	  $log_nodes = grievance_log_list($grievance_nid, $log_update_tids);

	  $last_open = NULL;
	  $last_closed = NULL;
	  foreach ($log_nodes as $log_node) {
	  	$status_tid = $log_node->field_grievance_status['und'][0]['tid'];
	  	if (!$status_tid) { continue; }
	  	$status_term = taxonomy_term_load($status_tid);
	  	$open = $status_term->field_grievance_open['und'][0]['value'];

	  	if ($open) {
				$last_open = $log_node->field_grievance_status_date['und'][0]['value'];
	  	} else  {
				$last_closed = $log_node->field_grievance_status_date['und'][0]['value'];
	  	}
	  }

	  $settlement_date = NULL;
	  if ($last_closed) {
	  	$settlement_date = $last_closed;
	  }
	  else if ($last_open) {
	  	$settlement_date = $last_open;
	  } else {
	  	$settlement_date = date('Y-m-d', $grievance_node->created);
	  }

	  $settlement_node->field_grievance_date_1['und'][0]['value'] = $settlement_date;
	  node_save($settlement_node);

    $settlement_ts = strtotime($settlement_date);
    $sql = "update node set created = :settlement_ts where nid = :nid";
    $stmt = db_query($sql, array(':settlement_ts' => $settlement_ts, ':nid' => $settlement_nid));

    print "Updating $settlement_nid to $settlement_date: " . $settlement_node->title . "\n";
  }
}