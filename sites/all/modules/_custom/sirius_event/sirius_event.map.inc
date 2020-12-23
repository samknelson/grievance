<?php

function sirius_event_map_enabled_nid($event_nid) { return sirius_event_map_enabled(node_load($event_nid)); }

function sirius_event_map_enabled($event_node) { return sirius_event_map_enabled_tid($event_node->field_sirius_event_type['und'][0]['tid']); }

function sirius_event_map_enabled_tid($event_type_tid) { return sirius_event_map_enabled_type(taxonomy_term_load($event_type_tid)); }

function sirius_event_map_enabled_type($event_type_term) {
  $json = json_decode($event_type_term->field_sirius_json['und'][0]['value'], TRUE);
  return $json['map']['map_enabled'] == 'Yes';
}

function sirius_map_opens_in($event_node) {
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

function sirius_map_active($event_node) {
  $result = NULL;

  $json = sirius_jsonfield_util_node_json_get($event_node);

  if ($json['map']['active'] == 'Yes') { 
    $result = array('success' => TRUE);
  } else if ($json['map']['active'] == 'No') { 
    $result = array('success' => FALSE, 'msg' => "The phone bank has been closed by an administrator.");
  }
  else if ($json['map']['active'] == 'Auto') { 
    $open_grace = $json['map']['active_grace_open'];
    if ($open_grace > 0) { $open_grace *= 60; } else { $open_grace = 0; }

    $close_grace = $json['map']['active_grace_close'];
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
      $result = array('success' => FALSE, 'msg' => "The phone bank is currently closed. " . sirius_map_opens_in($event_node));
    }
  } else { 
    $result = array('success' => FALSE);
  }

  if (!$result['success']) { return $result; }

  $do_not_disturb_id = $json['map']['do_not_disturb']['id'];
  if ($do_not_disturb_id) { return sirius_do_not_disturb_check($do_not_disturb_id); }

  return $result;
}