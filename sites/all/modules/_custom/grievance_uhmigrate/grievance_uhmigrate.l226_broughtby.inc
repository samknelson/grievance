<?php

function grievance_uhmigrate_l226_broughtby() {
  // Give ourselves more memory
  ini_set('memory_limit', '4096M');

  $sql = "select nid from node ";
  $sql .= "left join field_data_field_grievance_broughtby on (field_data_field_grievance_broughtby.entity_type = 'node' and bundle = 'grievance' and entity_id = nid) ";
  $sql .= "where field_grievance_broughtby_tid is null ";
  $sql .= "and node.type = 'grievance' ";

  $nids = array();
  $stmt = db_query($sql, array());
  while ($hr = $stmt->fetchAssoc()) {
  	$node = node_load($hr['nid']);
  	$node->field_grievance_broughtby = array('und' => array(array('tid' => 227)));
  	$node->grievance_no_recurse = TRUE;
  	node_save($node);

  	print "Saved: $node->nid ($node->title)\n";
  }

}