<?php

function sirius_worker_member_status_get_open_statuses($industry_tid = NULL, $include_closed = FALSE, $job_type_tid = NULL) {
  global $user;
  $rids = array(-1);
  if ($user && $user->roles) {
    $rids = array_keys($user->roles);
  }

	$query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'taxonomy_term');
  $query->entityCondition('bundle', 'sirius_member_status');
  if (!$include_closed) {
    $query->fieldCondition('field_sirius_dispatch_roles', 'value', $rids, 'IN');
  }
  if ($industry_tid) {
    $query->fieldCondition('field_sirius_industry', 'tid', $industry_tid);
  }
  if ($job_type_tid) {
    $query->fieldCondition('field_sirius_dispatch_job_types', 'tid', $job_type_tid);
  }
  $query->propertyOrderBy('weight');
  $stmt = $query->execute();
  if (!$stmt['taxonomy_term']) { return array(); }
  $tids = array_keys($stmt['taxonomy_term']);
  $terms = taxonomy_term_load_multiple($tids);

  return $terms;
}

function sirius_worker_member_status_check($worker_node, $member_status_term) {
  if (!$worker_node->field_sirius_member_status['und']) { return FALSE; }

  foreach ($worker_node->field_sirius_member_status['und'] as $tmp) {
    if ($tmp['tid'] == $member_status_term->tid) { return TRUE; }
  }

  return FALSE;
}

function sirius_worker_member_status_set($worker_node, $new_ms_term, $tf, $reason, $attachments = NULL) {
  // Get my current MS
  $my_ms_tids = sirius_fieldvals($worker_node, 'field_sirius_member_status', 'tid');

  // Don't add duplicates or remove nonexistent
  $new_ms_tid = $new_ms_term->tid;
  if ($tf && in_array($new_ms_tid, $my_ms_tids)) { return array('success' => TRUE, 'msg' => 'The worker already has this member status.'); }
  if (!$tf && !in_array($new_ms_tid, $my_ms_tids)) { return array('success' => TRUE, 'msg' => 'The worker did not have this member status.'); }

  // Build a map fom industry to ms
  static $industry_ms_map;
  if (!$industry_ms_map) {
    $industry_terms = sirius_taxonomy_load('sirius_industry', 'tid', 'full');
    $ms_terms = sirius_taxonomy_load('sirius_member_status', 'tid', 'full');
    $industry_ms_map = array();
    foreach ($ms_terms as $ms_tid => $ms_term) {
      $industry_ms_map[$ms_tid] = sirius_fieldvals($ms_term, 'field_sirius_industry', 'tid');
    }
  }
  $new_industry_tids = $industry_ms_map[$new_ms_tid];
  $clear_ms_tids = array();

  $msg = "";
  if (!$tf) {
    // Just clear this one
    $clear_ms_tids[] = $new_ms_tid;
    $msg .= "Removed: " . $new_ms_term->name . '. ';
  } else {
    // Clear any MS in the same industry
    if ($new_industry_tids) {
      foreach ($my_ms_tids as $ms_tid) {
        if (array_intersect($new_industry_tids, $industry_ms_map[$ms_tid])) {
          $clear_ms_tids[] = $ms_tid;
          $ms_term = taxonomy_term_load($ms_tid);
          $msg .= "Removed: " . $ms_term->name . '. ';
        }
      }
    }

    // Add our new one
    $my_ms_tids[] = $new_ms_tid;
    $msg .= "Added: " . $new_ms_term->name . '. ';
  }

  $tmp = array();
  foreach ($my_ms_tids as $ms_tid) {
    if (!in_array($ms_tid, $clear_ms_tids)) { $tmp[] = $ms_tid; }
  }
  $my_ms_tids = $tmp;

  // Dedupe
  $my_ms_tids = array_unique($my_ms_tids);
  $value = array();
  foreach ($my_ms_tids as $ms_tid) { $value[] = array('tid' => $ms_tid); }
  $worker_node->field_sirius_member_status['und'] = $value;

  // Fix industries
  $industry_tids = array();
  foreach ($my_ms_tids as $ms_tid) {
    foreach ($industry_ms_map[$ms_tid] as $industry_tid) {
      $industry_tids[$industry_tid] = TRUE;
    }
  }
  $worker_node->field_sirius_industry = array();
  foreach ($industry_tids as $industry_tid => $ignore) {
    $worker_node->field_sirius_industry['und'][] = array('tid' => $industry_tid);
  }

  node_save($worker_node);

  $category = 'worker:member_status';
  if ($tf) { 
    $type = 'set';
    $message = $worker_node->title . ' has added the member status ' . $new_ms_term->name;
  }
  else {
    $type = 'clear';
    $message = $worker_node->title . ' has removed the member status ' . $new_ms_term->name;
  }
  $summary = $reason;
  $notes = $new_ms_tid;
  $handler_nids = array($worker_node->nid);
  sirius_log($category, $type, $message, $summary, $notes, $handler_nids, $attachments);
}
