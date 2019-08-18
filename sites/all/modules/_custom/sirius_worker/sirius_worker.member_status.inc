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

function sirius_worker_member_status_set($worker_node, $member_status_term, $tf, $reason, $attachments) {
  if ($tf) {
    if (!sirius_worker_member_status_check($worker_node, $member_status_term)) {
      $worker_node->field_sirius_member_status['und'][]['tid'] = $member_status_term->tid;
      $need_save = TRUE;
    }
  }

  if (!$tf) {
    $to_clear = array();
    foreach ($worker_node->field_sirius_member_status['und'] as $delta => $tmp) {
      if ($tmp['tid'] == $member_status_term->tid) { $to_clear[] = $delta; }
    }
    if ($to_clear) {
      foreach ($to_clear as $delta) {
        unset($worker_node->field_sirius_member_status['und'][$delta]);
      }
      $worker_node->field_sirius_member_status[LANGUAGE_NONE] = array_values($worker_node->field_sirius_member_status[LANGUAGE_NONE]);
      $need_save = TRUE;
    }
  }

  // Fix industries
  if ($need_save) {
    $industry_tids = array();
    foreach ($worker_node->field_sirius_member_status[LANGUAGE_NONE] as $tmp) {
      $member_status_term = taxonomy_term_load($tmp['tid']);
      $industry_tid = $member_status_term->field_sirius_industry['und'][0]['tid'];
      $industry_tids[$industry_tid] = TRUE;
    }

    $worker_node->field_sirius_industry = array();
    foreach ($industry_tids as $industry_tid => $ignore) {
      $worker_node->field_sirius_industry['und'][] = array('tid' => $industry_tid);
    }

    node_save($worker_node);
  }

  $category = 'worker:member_status';
  if ($tf) { 
    $type = 'set';
    $message = $worker_node->title . ' has added the member status ' . $member_status_term->name;
  }
  else {
    $type = 'clear';
    $message = $worker_node->title . ' has removed the member status ' . $member_status_term->name;
  }
  $summary = $reason;
  $notes = $member_status_term->tid;
  $handler_nids = array($worker_node->nid);
  sirius_log($category, $type, $message, $summary, $notes, $handler_nids, $attachments);
}
