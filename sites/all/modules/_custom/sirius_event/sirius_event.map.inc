<?php

function sirius_event_map_enabled_nid($event_nid) { return sirius_event_map_enabled(node_load($event_nid)); }

function sirius_event_map_enabled($event_node) { return sirius_event_map_enabled_tid($event_node->field_sirius_event_type['und'][0]['tid']); }

function sirius_event_map_enabled_tid($event_type_tid) { return sirius_event_map_enabled_type(taxonomy_term_load($event_type_tid)); }

function sirius_event_map_enabled_type($event_type_term) {
  if (!sirius_domain_component_enabled('mapping')) { return FALSE; }

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
    $result = array('success' => FALSE, 'msg' => "The map has been closed by an administrator.");
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
      $result = array('success' => FALSE, 'msg' => "The map is currently closed. " . sirius_map_opens_in($event_node));
    }
  } else { 
    $result = array('success' => FALSE);
  }

  if (!$result['success']) { return $result; }

  $do_not_disturb_id = $json['map']['do_not_disturb']['id'];
  if ($do_not_disturb_id) { return sirius_do_not_disturb_check($do_not_disturb_id); }

  return $result;
}

function sirius_event_map_list($event_node, $cutoff_ts = 0, $limit = 3000, $offset = 0) {
  // Create settings for the map.
  $type_json = sirius_event_type_json($event_node);
  $prole_tid = $type_json['map']['prole_tid'];
  $event_json = sirius_jsonfield_util_node_json_get($event_node);

  $map_features = array();

  $pstatus_terms = sirius_taxonomy_load('sirius_event_participant_status', 'tid', 'full');
  $pstatus_pins = array();
  foreach ($pstatus_terms as $pstatus_tid => $pstatus_term) {
    $pstatus_json = sirius_jsonfield_util_term_json_get($pstatus_term);
    $pstatus_pins[$pstatus_tid] = sirius_map_pin($pstatus_json['map']['pin']);
  }

  // Fetch all the relevant data about our participants
  $sql_args = array();
  $sql_args[':event_nid'] = $event_node->nid;
  $sql_args[':prole_tid'] = $prole_tid;
  $sql = "select contact_node.title, ";
  $sql .= "contact_node.nid as contact_nid, ";
  $sql .= "node.nid as participant_nid, ";
  $sql .= "field_sirius_event_pstatus_tid, ";
  $sql .= "field_sirius_address_thoroughfare, ";
  $sql .= "field_sirius_address_premise, ";
  $sql .= "field_sirius_address_locality, ";
  $sql .= "field_sirius_address_administrative_area, ";
  $sql .= "field_sirius_address_postal_code, ";
  $sql .= "field_sirius_address_geo_lat, ";
  $sql .= "field_sirius_address_geo_lon ";
  $sql .= "from node ";
  $sql .= "left join field_data_field_sirius_event on field_data_field_sirius_event.entity_type = 'node' and field_data_field_sirius_event.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_event_prole on field_data_field_sirius_event_prole.entity_type = 'node' and field_data_field_sirius_event_prole.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_event_pstatus on field_data_field_sirius_event_pstatus.entity_type = 'node' and field_data_field_sirius_event_pstatus.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_contact on field_data_field_sirius_contact.entity_type = 'node' and field_data_field_sirius_contact.entity_id = node.nid ";
  $sql .= "left join node contact_node on contact_node.nid = field_sirius_contact_target_id ";
  $sql .= "left join field_data_field_sirius_address on field_data_field_sirius_address.entity_type = 'node' and field_data_field_sirius_address.entity_id = contact_node.nid ";
  $sql .= "left join field_data_field_sirius_address_geo on field_data_field_sirius_address_geo.entity_type = 'node' and field_data_field_sirius_address_geo.entity_id = contact_node.nid ";
  $sql .= "where field_sirius_event_target_id = :event_nid ";
  $sql .= "and field_sirius_event_prole_tid = :prole_tid ";
  $sql .= "and field_sirius_address_geo_lat is not null ";
  $sql .= "and field_sirius_address_thoroughfare is not null ";
  $sql .= "and field_sirius_address_thoroughfare != '' ";
  if ($cutoff_ts) { 
    $sql .= "and node.changed > :cutoff_ts ";
    $sql_args[':cutoff_ts'] = $cutoff_ts;
  }
  $sql .= "limit $limit offset $offset ";

  $stmt = sirius_sql_query($sql, $sql_args);
  while ($hr = $stmt->fetchAssoc()) {
    $pstatus_tid = $hr['field_sirius_event_pstatus_tid'];
    $pin = $pstatus_pins[$pstatus_tid];

    $map_feature = array(
      'type' => 'point',
      'lat' => $hr['field_sirius_address_geo_lat'],
      'lon' => $hr['field_sirius_address_geo_lon'],
      'popup' => sirius_map_popup_html($event_node->nid, $hr['contact_nid'], $hr['title'], $hr['field_sirius_address_thoroughfare'], $hr['field_sirius_address_premise'], $hr['participant_nid'], $pstatus_terms[$pstatus_tid]->name),
      'participant_nid'  => $hr['participant_nid'],
      'leaflet_id' => 'participant_' . $hr['participant_nid'],
      'sirius' => array(
        'marker' => array('id' => $hr['participant_nid']),
        'contact_name' => $hr['title'],
        'list_html' => sirius_map_list_html(
          $event_node->nid, 
          $hr['contact_nid'], 
          $hr['title'], 
          $hr['field_sirius_address_thoroughfare'], 
          $hr['field_sirius_address_premise'], 
          $hr['field_sirius_address_locality'], 
          $hr['field_sirius_address_administrative_area'], 
          $hr['field_sirius_address_postal_code'], 
          $hr['participant_nid'], 
          $pstatus_terms[$pstatus_tid]->name,
          $pstatus_tid,
          $pin['path']
        ),
        'type' => 'contact',
        'pstatus_tid' => $hr['field_sirius_event_pstatus_tid'],
        'pstatus_name' => $pstatus_terms[$hr['field_sirius_event_pstatus_tid']]->name,
      ),
      'icon' => sirius_map_pin_render_leaflet($pin),
      'pin_key' => $pin['key'],
    );

    $map_features[] = $map_feature;
  }

  // Show the locations of volunteers, if
  // - The current user is an admin (has access to the settings page for the event), AND
  // - The component is enabled, AND
  // - Checkin is enabled for the map, AND 
  // - There is at least one "caller role".

  if (sirius_node_tab_event_map_settings_access($event_node) && 
    sirius_domain_component_enabled('mapping:checkin') && 
    ($event_json['map']['checkin']['active'] == 'Yes') && 
    $type_json['map']['caller_prole_tids']) {
    $caller_prole_tids = array_filter(array_values($type_json['map']['caller_prole_tids']));
    $sql_args = array();
    $sql_args[':event_nid'] = $event_node->nid;
    $sql_args[':caller_prole_tids'] = $caller_prole_tids;
    $sql = "select contact_node.title, ";
    $sql .= "contact_node.nid as contact_nid, ";
    $sql .= "node.nid as participant_nid, ";
    $sql .= "field_sirius_event_pstatus_tid, ";
    $sql .= "users.uid as uid, ";
    $sql .= "users.name as user_name, ";
    $sql .= "users.mail as user_mail, ";
    $sql .= "users_phone.field_grievance_phone_value as user_phone, ";
    $sql .= "users_json.field_sirius_json_value as user_json_value ";
    $sql .= "from node ";
    $sql .= "left join field_data_field_sirius_event on field_data_field_sirius_event.entity_type = 'node' and field_data_field_sirius_event.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_event_prole on field_data_field_sirius_event_prole.entity_type = 'node' and field_data_field_sirius_event_prole.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_event_pstatus on field_data_field_sirius_event_pstatus.entity_type = 'node' and field_data_field_sirius_event_pstatus.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_contact on field_data_field_sirius_contact.entity_type = 'node' and field_data_field_sirius_contact.entity_id = node.nid ";
    $sql .= "left join node contact_node on contact_node.nid = field_sirius_contact_target_id ";
    $sql .= "left join field_data_field_sirius_contact users_contact on users_contact.entity_type = 'user' and users_contact.field_sirius_contact_target_id = contact_node.nid ";
    $sql .= "left join users on users.uid = users_contact.entity_id ";
    $sql .= "left join field_data_field_sirius_json users_json on users_json.entity_type = 'user' and users_json.entity_id = users.uid ";
    $sql .= "left join field_data_field_grievance_phone users_phone on users_phone.entity_type = 'user' and users_phone.entity_id = users.uid ";
    $sql .= "where field_sirius_event_target_id = :event_nid ";
    $sql .= "and users.uid is not null ";
    $sql .= "and field_sirius_event_prole_tid in (:caller_prole_tids) ";
    if ($cutoff_ts) { 
      $sql .= "and users.access > :cutoff_ts ";
      $sql_args[':cutoff_ts'] = $cutoff_ts;
    }
    $sql .= "limit $limit offset $offset ";

    $stmt = sirius_sql_query($sql, $sql_args);
    $ts_cutoff = time() - 60*60*4;
    while ($hr = $stmt->fetchAssoc()) {
      $json_user = json_decode($hr['user_json_value'], TRUE);
      $json_checkin = $json_user['current_location']['checkin'];
      if (!$json_checkin['lat']) { continue; }
      if ($json_checkin['ts'] < $ts_cutoff) { continue; }

      $pstatus_tid = $hr['field_sirius_event_pstatus_tid'];
      $pin = $pstatus_pins[$pstatus_tid];

      $popup_html = '<strong>Name:</strong> ' . $hr['user_name'];
      $popup_html .= "<br /><strong>Checkin:</strong> " . date('g:i:s A l', $json_checkin['ts']);
      $popup_html .= "<br /><strong>Checkin:</strong> " . format_interval(time() - $json_checkin['ts']) . ' ago';
      $popup_html .= "<br /><strong>Email:</strong> <a href=\"mailto:$hr[user_mail]\">$hr[user_mail]</a>";
      if ($hr['user_phone']) { $popup_html .= "<br /><strong>Phone:</strong> <a href=\"tel:$hr[user_phone]\">$hr[user_phone]</a>"; }

      $map_feature = array(
        'type' => 'point',
        'lat' => $json_checkin['lat'],
        'lon' => $json_checkin['lon'],
        'popup' => $popup_html,
        'participant_nid'  => $hr['participant_nid'],
        'leaflet_id' => 'participant_' . $hr['participant_nid'],
        'sirius' => array(
          'type' => 'user',
          'marker' => array('id' => $hr['participant_nid'])
        ),
        'tooltip' => substr($hr['user_name'], 0, 10),
        'tooltip_options' => array(
          'permanent' => TRUE, 
          'direction' => 'right',
          // 'offset' => array($pin['tooltipAnchor']['x'], $pin['tooltipAnchor']['y']),
        ),
        'icon' => sirius_map_pin_render_leaflet($pin),
        'pin_key' => $pin['key'],
      );
      $map_features[] = $map_feature;
    }
  }

  return $map_features;
}