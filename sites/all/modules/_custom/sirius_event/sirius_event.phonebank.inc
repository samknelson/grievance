<?php

function sirius_event_phonebank_enabled_nid($event_nid) { return sirius_event_phonebank_enabled(node_load($event_nid)); }

function sirius_event_phonebank_enabled($event_node) { return sirius_event_phonebank_enabled_tid($event_node->field_sirius_event_type['und'][0]['tid']); }

function sirius_event_phonebank_enabled_tid($event_type_tid) { return sirius_event_phonebank_enabled_type(taxonomy_term_load($event_type_tid)); }

function sirius_event_phonebank_enabled_type($event_type_term) {
  $json = json_decode($event_type_term->field_sirius_json['und'][0]['value'], TRUE);
  return $json['phonebank']['phonebank_enabled'] == 'Yes';
}

function sirius_event_phonebank_lock_type($event_node) {
  return 'phonebank_' . $event_node->nid;
}

function sirius_event_phonebank_next_number($event_node, $prole_tid, $pstatus_tid) {
  global $user;
  $lock_type = sirius_event_phonebank_lock_type($event_node);

  // @todo: This really should be surrounded by lock_acquire($name) and lock_release($name).
  //        But I really can't be bothered. So it's theoretically possible to attempt to serve the same participant to multiple users.
  //        In which case, we return an error message and they reload the page. Oh well.

  $sql = "select node.nid, field_sirius_phone_value as phone from node ";
  $sql .= "left join field_data_field_sirius_event on field_data_field_sirius_event.entity_type = 'node' and field_data_field_sirius_event.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_event_prole on field_data_field_sirius_event_prole.entity_type = 'node' and field_data_field_sirius_event_prole.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_event_pstatus on field_data_field_sirius_event_pstatus.entity_type = 'node' and field_data_field_sirius_event_pstatus.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_contact on field_data_field_sirius_contact.entity_type = 'node' and field_data_field_sirius_contact.entity_id = node.nid ";
  $sql .= "left join node contact_node on contact_node.nid = field_sirius_contact_target_id ";
  $sql .= "left join field_data_field_sirius_phone on field_data_field_sirius_phone.entity_type = 'node' and field_data_field_sirius_phone.entity_id = contact_node.nid ";
  $sql .= "where node.type = 'sirius_event_participant' ";
  $sql .= "and field_sirius_event_target_id = :event_nid ";
  $sql .= "and field_sirius_event_prole_tid = :prole_tid ";
  $sql .= "and field_sirius_event_pstatus_tid = :pstatus_tid ";
  $sql .= "and not exists (select 1 from sirius_lock where sirius_lock.type = :lock_type and sirius_lock.id = field_sirius_phone_value and sirius_lock.uid != :uid) ";
  $sql .= "and field_sirius_phone_value is not null ";
  $sql .= "and field_sirius_phone_value != '' ";
  $sql .= "order by node.nid ";
  $sql .= "limit 1 ";
  $stmt = db_query($sql, array(
    ':event_nid' => $event_node->nid, 
    ':prole_tid' => $prole_tid, 
    ':pstatus_tid' => $pstatus_tid,
    ':lock_type' => $lock_type,
    ':uid' => $user->uid,
  ));
  $hr = $stmt->fetchAssoc();
  $nid = $hr['nid'];
  $phone = sirius_phone_format($hr['phone']);
  if (!$nid) { return NULL; }

  $result = sirius_lock_obtain($lock_type, $phone);
  if (!$result['success']) {
    drupal_set_message("Unable to obtain a lock: " . $result['msg']);
    return NULL; 
  }

  return $phone;
}

function sirius_event_phonebank_participants_by_phone($event_node, $prole_tid, $phone) {
  $phone = sirius_phone_format($phone);
  $sql = "select node.nid from node ";
  $sql .= "left join field_data_field_sirius_event on field_data_field_sirius_event.entity_type = 'node' and field_data_field_sirius_event.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_event_prole on field_data_field_sirius_event_prole.entity_type = 'node' and field_data_field_sirius_event_prole.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_contact on field_data_field_sirius_contact.entity_type = 'node' and field_data_field_sirius_contact.entity_id = node.nid ";
  $sql .= "left join node contact_node on contact_node.nid = field_sirius_contact_target_id ";
  $sql .= "left join field_data_field_sirius_phone on field_data_field_sirius_phone.entity_type = 'node' and field_data_field_sirius_phone.entity_id = contact_node.nid ";
  $sql .= "where node.type = 'sirius_event_participant' ";
  $sql .= "and field_sirius_event_target_id = :event_nid ";
  $sql .= "and field_sirius_event_prole_tid = :prole_tid ";
  $sql .= "and field_sirius_phone_value = :phone ";
  $sql .= "order by node.nid ";

  $stmt = db_query($sql, array(
    ':event_nid' => $event_node->nid, 
    ':prole_tid' => $prole_tid, 
    ':phone' => $phone,
  ));

  $participant_nids = array();
  while ($hr = $stmt->fetchAssoc()) {
    $participant_nids[] = $hr['nid'];
  }
  return node_load_multiple($participant_nids);
}

function sirius_phonebank_opens_in($event_node) {
  if (!$event_node->field_sirius_daterepeat['und']) { return ''; }
  $ts_now = time();
  $opens_in = 999999999;
  foreach ($event_node->field_sirius_daterepeat['und'] as $tmp) {
    $start_ts = strtotime($tmp['value'] . ' ' . $tmp['timezone_db']);
    $end_ts = strtotime($tmp['value2'] . ' ' . $tmp['timezone_db']);
    if ($start_ts <= 1 || $end_ts <= 1) { continue; }
    if ($start_ts >= $ts_now) {
      $range = $start_ts - $ts_now;
      if ($range && $range < $opens_in) { $opens_in = $range; }
    }
  }
  if ($opens_in > 1 && $opens_in < 999999999) {
    return t('Opening in: @interval', array('@interval' => format_interval($opens_in)));
  }
}

function sirius_phonebank_active($event_node) {
  $result = NULL;

  $json = sirius_jsonfield_util_node_json_get($event_node);

  if ($json['phonebank']['active'] == 'Yes') { 
    $result = array('success' => TRUE);
  } else if ($json['phonebank']['active'] == 'No') { 
    $result = array('success' => FALSE, 'msg' => "The phone bank has been closed by an administrator.");
  }
  else if ($json['phonebank']['active'] == 'Auto') { 
    $open_grace = $json['phonebank']['active_grace_open'];
    if ($open_grace > 0) { $open_grace *= 60; } else { $open_grace = 0; }

    $close_grace = $json['phonebank']['active_grace_close'];
    if ($close_grace) { $close_grace *= 60; } else { $close_grace = 0; }

    if ($event_node->field_sirius_daterepeat['und']) {
      $ts_now = time();
      foreach ($event_node->field_sirius_daterepeat['und'] as $tmp) {
        $start_ts = strtotime($tmp['value'] . ' ' . $tmp['timezone_db']);
        $end_ts = strtotime($tmp['value2'] . ' ' . $tmp['timezone_db']);
        if ($start_ts <= 1 || $end_ts <= 1) { continue; }
        $start_ts -= $open_grace;
        $end_ts += $close_grace;
        if ($ts_now >= $start_ts && $ts_now <= $end_ts) {
          $result = array('success' => TRUE);
        }
      }
    }
    if (!$result['success']) {
      $result = array('success' => FALSE, 'msg' => "The phone bank is currently closed. " . sirius_phonebank_opens_in($event_node));
    }
  } else { 
    $result = array('success' => FALSE);
  }

  if (!$result['success']) { return $result; }

  $do_not_disturb_id = $json['phonebank']['do_not_disturb']['id'];
  if ($do_not_disturb_id) { return sirius_do_not_disturb_check($do_not_disturb_id); }

  return $result;
}