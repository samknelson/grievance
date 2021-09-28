<?php

function grievance_uhmigrate_l665_specialrequest($limit, $source_filename = 'SpecialRequest.csv') {
  // Give ourselves more memory
  ini_set('memory_limit', '4096M');
  
  $source_filename =  '/var/www/private/migrate/' . $source_filename;
  $source_handle = fopen($source_filename, "r");
  if (!$source_handle) {
    die("Unable to open $source_filename");
  }

  // Headers
  $headers = fgetcsv($source_handle, 10000);
  $skiprow = fgetcsv($source_handle, 10000);

  // Load our rows one at a time
  $rows_raw = array();
  while ($row_raw = fgetcsv($source_handle, 10000, ",")) {
    $rows_raw[] = $row_raw;
  }
  fclose($source_handle);
  // $rows_raw = array_reverse($rows_raw);

  $x = 0;
  foreach ($rows_raw as $row_raw) {
    ++$x;
    $row = array();
    foreach ($headers as $col => $header) {
      $row[$header] = $row_raw[$col];
      ++$col;
    }
    $worker_id = $row['WorkerID'];
    $rows[$worker_id][] = $row;
  }
  
  $total_rows = count($rows);

  foreach ($rows as $worker_id => $subrows) {
    ++$rowcount;
    print "Row #$rowcount / $total_rows\n";
    // if (grievance_uhmigrate_skiprow()) { continue; }
    ++$processed_rowcount;
    if ($processed_rowcount > $limit) { break; }

    // Clear the cache...
    entity_get_controller('node')->resetCache();

    // Find the node to create or update
    if (!$worker_id) {
      grievance_uhmigrate_log('ERROR', $row, "No external ID (missing WorkerID)");
      continue;
    }
    $node = grievance_uhmigrate_find_by_sirius_id('sirius_worker', $worker_id);
    if ($node->nid) { 
      grievance_uhmigrate_log('DEBUG', $row, "Found existing node with $worker_id, NID = " . $node->nid . ". It will be updated.");
    } else {
      grievance_uhmigrate_log('DEBUG', $row, "No node found with $external_id. Skipping.");
      continue;
    }

    $node->field_sirius_skills_avail = array();
    foreach ($subrows as $subrow) {
      if ($subrow['TableName'] == 'Skill') {
        $skill_id = $subrow['KeyValue'];
        if (!$skill_id) {
          grievance_uhmigrate_log('ERROR', $row, "No skill ID (missing SkillID)");
          continue;
        }
        $skill_term = grievance_uhmigrate_find_term_by_sirius_id('sirius_skill', $skill_id, 'field_sirius_id2');
        $skill_tid = $skill_term->tid;
        if (!$skill_tid) {
          grievance_uhmigrate_log('ERROR', $row, "No skill found with ID $skill_id");
          continue;
        }
        $node->field_sirius_skills_avail['und'][] = array('tid' => $skill_tid);
        $node->field_sirius_skills_availx['und'][0]['value'] = 'No';
      }

      if ($subrow['TableName'] == 'Employer') {
        $employer_id = $subrow['KeyValue'];
        $employer_node = grievance_uhmigrate_find_by_sirius_id('grievance_shop', $employer_id);
        if (!$employer_node->nid) {
          grievance_uhmigrate_log('ERROR', $row, "No employer found with $employer_id");
          continue;
        }
        $node->field_sirius_dispatch_hfe['und'][0]['target_id'] = $employer_node->nid;
        $node->field_sirius_dispatch_hfe_until['und'][0]['value'] = '2100-1-1';
      }

      if ($subrow['TableName'] == 'RequestType') {
        $node->field_sirius_dispatch_cbn['und'][0]['value'] = 'Yes';
      }
    }

    node_save($node);
  }
}
