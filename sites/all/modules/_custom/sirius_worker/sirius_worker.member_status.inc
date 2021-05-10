<?php

function sirius_worker_member_status_get_open_statuses($industry_tid = NULL, $include_closed = FALSE, $job_type_tid = NULL) {
  global $user;
  $rids = array(-1);
  if ($user && $user->roles) {
    $rids = array_keys($user->roles);
  }

  $domain_nid = sirius_domain_nid();
	$query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'taxonomy_term');
  $query->entityCondition('bundle', 'sirius_member_status');

  if (!$include_closed)   { $query->fieldCondition('field_sirius_dispatch_roles', 'value', $rids, 'IN'); }
  if ($industry_tid)      { $query->fieldCondition('field_sirius_industry', 'tid', $industry_tid); }
  if ($job_type_tid)      { $query->fieldCondition('field_sirius_dispatch_job_types', 'tid', $job_type_tid); }
  if ($domain_nid)        { $query->fieldCondition('field_sirius_domain', 'target_id', $domain_nid); }

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
  if ($tf) { $type = 'set'; } else { $type = 'clear'; }
  $summary = $reason;
  $notes = $new_ms_tid;
  $handler_nids = array($worker_node->nid);
  sirius_log($category, $type, $msg, $summary, $notes, $handler_nids, $attachments);

  // Send notifications
  $notify_ms_tids = $clear_ms_tids;
  $notify_ms_tids[] = $new_ms_tid;
  $notify_ms_tids = array_unique($notify_ms_tids);
  $notify_ms_terms = taxonomy_term_load_multiple($notify_ms_tids);
  $notify_emails = array();
  foreach ($notify_ms_terms as $notify_ms_tid => $notify_ms_term) {
    $notify_json = sirius_jsonfield_util_term_json_get($notify_ms_term);
    if ($notify_json['subscribe']['notify']) {
      foreach ($notify_json['subscribe']['notify'] as $email) { $notify_emails[] = $email; }
    }
  }
  $notify_emails = array_unique($notify_emails);
  if ($notify_emails) {
    $params = array();
    $params['subject'] = 'Member status change for ' . $worker_node->title;
    $params['body'] = 'There has been a member status change for ' . $worker_node->title . '.';
    $params['body'] .= '<br /><br />';
    $params['body'] .= $msg;
    $params['body'] .= '<br /><br />';
    $params['body'] .= $reason;
    $params['body'] .= '<br /><br />';
    global $base_url;
    $url = $base_url . '/node/' . $worker_node->nid . '/sirius_worker_industry';
    $params['body'] .= "Link: <a href=\"$url\">$url</a>";
    foreach ($notify_emails as $email) {
      $params['to'] = $email;
      sirius_email_send($params);
    }
  }

  return array('success' => TRUE, 'msg' => $msg);
}

function sirius_worker_render_industries($worker_node, $options = array()) {
  $industry_terms = sirius_taxonomy_load('sirius_industry', 'tid', 'full');
  $member_status_terms = sirius_taxonomy_load('sirius_member_status', 'tid', 'full');
  $industry_member_status_map = array();
  foreach ($member_status_terms as $member_status_tid => $member_status_term) {
    $industry_tid = sirius_fieldval($member_status_term, 'field_sirius_industry', 'tid');
    $industry_member_status_map[$industry_tid][] = $member_status_tid;
    $industry_member_status_map[$member_status_tid] = $industry_tid;
  }

  $my_member_status_tids = sirius_fieldvals($worker_node, 'field_sirius_member_status', 'tid');
  $rows = array();
  foreach ($industry_terms as $industry_tid => $industry_term) {
    $row = array();
    $row[] = $industry_term->name;

    $str = '';
    $str2 = '';
    if ($my_member_status_tids) {

      $job_node = new stdClass();
      $job_node->title = "Dummy";
      $job_node->type = 'sirius_dispatch_job';
      $job_node->language = LANGUAGE_NONE;
      $job_node->uid = $user->uid;
      $job_node->status = 1;
      $job_node->promote = 0;
      $job_node->comment = 0;
      $job_node->field_grievance_shop['und'][0]['target_id'] = 0;
      $job_node->field_sirius_dispatch_job_type['und'][0]['tid'] = sirius_get_special_tid('sirius_dispatch_job_type|OPEN');
      $job_node->field_sirius_datetime['und'][0]['value'] = date('Y-m-d H:i:s');
      $job_node->field_sirius_datetime_created['und'][0]['value'] = date('Y-m-d H:i:s');
      $job_node->field_sirius_dispatch_job_status['und'][0]['value'] = 'Pending';

      foreach ($my_member_status_tids as $member_status_tid) {
        if ($industry_tid == $industry_member_status_map[$member_status_tid]) {
          if ($str) { $str .= ' / '; }
          if ($str2) { $str2 .= ' / '; }
          $str .= $member_status_terms[$member_status_tid]->name;

          $job_node->field_sirius_industry['und'][0]['tid'] = $industry_tid;
          $open_member_status_tids = array_keys(sirius_worker_member_status_get_open_statuses($industry_tid, TRUE, sirius_get_special_tid('sirius_dispatch_job_type|OPEN')));
          foreach ($open_member_status_tids as $tid) {
            if ($tid) {
              $job_node->field_sirius_member_status['und'][] = array('tid' => $tid);
            }
          }
          $dispatchable = sirius_dispatch_job_get_dispatchable_workers_explain(
            $job_node, 
            $worker_node->nid,
            array(
              'ignore_skill' => TRUE,
              'ignore_department' => TRUE,
            )
          );

          if ($dispatchable['success']) {
            $str2 = $dispatchable['msg'];
          } else {
            $str2 = 'Dispatch not available: ' . $dispatchable['msg'];
          }
        }
      }
    }
    if (!$str) { continue; }
    if (!$str2) { $str2 = t('Dispatch not available.'); }
    $row[] = $str;
    $row[] = $str2;

    $rows[] = $row;
  }

  if ($rows) {
    $header = array(
      t('Industry'),
      t('Member Status'),
      t('Dispatch Position'),
    );

    $attributes = $options['table_attributes'];
    if (!$attributes) { $attributes = array(); }
    $html = theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => $attributes));
  } else {
    $html = '<p>' . t('This worker does not currently have any industries.') . '</p>';
  }

  return $html;
}
