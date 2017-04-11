<?php

function grievance_uhmigrate_l226_import_additional_violations($limit = 100) {
  // Give ourselves more memory
  ini_set('memory_limit', '4096M');
  
  // Source file
  // $source_filename = DRUPAL_ROOT . '/' . drupal_get_path('module', 'grievance_uhmigrate') . "/data/import.csv";
  $source_filename =  grievance_uhmigrate_base_dir() . '/uh_226/uh_226_additional_violations.csv';
  $source_handle = fopen($source_filename, "r");
  if (!$source_handle) {
    die("Unable to open $source_filename");
  }

  // Headers
  $headers = fgetcsv($source_handle, 1000);

  // Load our rows one at a time
  $rows_raw = array();
  while ($row_raw = fgetcsv($source_handle, 1000, ",")) {
    $rows_raw[] = $row_raw;
  }
  fclose($source_handle);
  $rows_raw = array_reverse($rows_raw);
  
  $total_rowcount = count($rows_raw);

  $rowcount = 0;
  foreach ($rows_raw as $row_raw) {
    // Clear the cache...
    entity_get_controller('node')->resetCache();
    
    ++$rowcount;
    if ($rowcount > $limit) { break; }

    // Generate an associative array from the input
    $row = array("Row" => $total_rowcount - $rowcount);
    // if ($row['Row'] != 20494) { continue; }
    $col = 0;
    foreach ($headers as $header) {
      $row[$header] = $row_raw[$col];
      ++$col;
    }

    // Find the grievance_id
    $grievance_id = $row['TGrev#'];
    $row['Griev#'] = $row['TGrev#'];
    if (!$grievance_id) {
      grievance_uhmigrate_log('ERROR - NO GRIEVANCE ID', $row, "No grievance_id");
      continue;
    }

    // Find the grievance
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'node');
    $query->entityCondition('bundle', 'grievance');
    $query->fieldCondition('field_grievance_id', 'value', $grievance_id);
    $stmt = $query->execute();
    if (!$stmt['node']) {
      grievance_uhmigrate_log('ERROR - NO GRIEVANCE ID', $row, "Unable to find grievance with id [$grievance_id]");
      continue;
    }
    $grievance_nids = array_keys($stmt['node']);
    $grievance_nid = $grievance_nids[0];
    $grievance_node = node_load($grievance_nid);
    if (!$grievance_node) {
      grievance_uhmigrate_log('ERROR - GRIEVANCE ID MISSING', $row, "Unable to find grievance with id [$grievance_id]");
      continue;
    }

    // Find the shop
    $shop_nid = $grievance_node->field_grievance_shop['und'][0]['target_id'];
    if (!$shop_nid) {
      grievance_uhmigrate_log('ERROR - NO SHOP', $row, "No shop in the node.");
      continue;
    }
    $shop_node = node_load($shop_nid);
    if (!$shop_node) {
      grievance_uhmigrate_log('ERROR - SHOP NOT FOUND', $row, "Shop node not found.");
      continue;
    }

    // Find the clauses
    $row['Violation'] = $row['TViolation'];
    $clause_nids = grievance_uhmigrate_find_violation_nid($row, $shop_node);
    $need_save = FALSE;
    foreach ($clause_nids as $clause_nid) {
      $found = FALSE;
      if ($grievance_node->field_grievance_clauseref['und']) {
        foreach ($grievance_node->field_grievance_clauseref['und'] as $tmp) {
          if ($tmp['target_id'] == $clause_nid) {
            $found = TRUE;
          }
        }
      }
      if (!$found) {
        $grievance_node->field_grievance_clauseref['und'][] = array('target_id' => $clause_nid);
        $tmp = print_r($row, 1);
        $tmp = preg_replace('/[\x00-\x08\x0B\x0C\x0E-\x1F\x7F-\x9F]/u', '', $tmp);
        $grievance_node->field_grievance_comments['und'][0]['value'] .= $tmp;
        $need_save = TRUE;
      }
    }
    if ($need_save) {
      grievance_uhmigrate_log('SUCCESS', $row, "Added clauses to " . $grievance_node->nid);
      node_save($grievance_node);
    } else {
      grievance_uhmigrate_log('SUCCESS', $row, "Clauses already set for " . $grievance_node->nid);
    }
  }

  grievance_uhmigrate_log_summary();
}




