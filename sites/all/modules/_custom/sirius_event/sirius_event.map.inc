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

function sirius_event_map_legend_render($event_nid) {
  $event_node = node_load($event_nid);
  $event_json = sirius_jsonfield_util_node_json_get($event_node);
  $type_json = sirius_event_type_json($event_node);
  $prole_tid = $type_json['map']['prole_tid'];
  $pstatus_options = sirius_event_participant_role_status_tids($prole_tid, array('phonebank' => array('during_call' => TRUE, 'current_pstatus' => $pstatus_tid)));
  $pstatus_terms = taxonomy_term_load_multiple(array_keys($pstatus_options));

  $counts = sirius_event_participant_summary($event_nid, $prole_tid);

  $has_tools = FALSE;
  $rows = array();
  foreach ($pstatus_terms as $pstatus_tid => $pstatus_term) {
    $pstatus_json = sirius_json_get($pstatus_term);
    $pin = sirius_map_pin($pstatus_json['map']['pin']);

    $tools = array();
    $path = "taxonomy/term/$pstatus_tid/sirius_event_pstatus";
    if (drupal_valid_path($path)) {
      $has_tools = TRUE;
      $tools[] = l('Settings', $path, array('attributes' => array('class' => array('tool-button')), 'query' => array('destination' => current_path())));
    }

    $row = array();
    $row[] = sirius_map_pin_render($pin);
    $row[] = $pstatus_term->name;
    $row[] = number_format($counts[$pstatus_tid], 0);
    $row[] = $pstatus_term->description;
    if ($has_tools) { $row[] = join('', $tools); }

    $rows[] = $row;
  }

  $group_by_address = ($event_json['map']['group_by_address'] == 'Yes');
  if ($group_by_address) {
    $mixed_pstatus_tid = $event_json['map']['group_by_address_options']['mixed_pstatus'];
    if ($mixed_pstatus_tid) {
      $pstatus_term = taxonomy_term_load($mixed_pstatus_tid);
      $pstatus_json = sirius_json_get($pstatus_term);
      $pin = sirius_map_pin($pstatus_json['map']['pin']);

      $tools = array();
      $path = "taxonomy/term/$mixed_pstatus_tid/sirius_event_pstatus";
      if (drupal_valid_path($path)) {
        $has_tools = TRUE;
        $tools[] = l('Settings', $path, array('attributes' => array('class' => array('tool-button')), 'query' => array('destination' => current_path())));
      }

      $row = array();
      $row[] = sirius_map_pin_render($pin);
      $row[] = 'Multiple Statuses';
      $row[] = '';
      $row[] = '(This pin is shown when there are multiple residents at an address, and they have different statuses.)';
      if ($has_tools) { $row[] = join('', $tools); }
      $rows[] = $row;
    }
  }


  $header = array();
  $header[] = t('Marker');
  $header[] = t('Status');
  $header[] = t('Count');
  $header[] = t('Description');
  if ($has_tools) { $header[] = t('Tools'); }
  $html .= sirius_table_render($header, $rows);
  return $html;
}

function sirius_event_map_list($event_node, $params = array()) {
  $cutoff_ts = $params['cutoff_ts'];
  $limit = $params['limit'];
  if (!$limit) { $limit = 3000; }
  $offset = $params['offset'];
  if (!$offset) { $offset = 0; }
  $show_pstatus_tids = $params['show_pstatus_tids'];

  // Create settings for the map.
  $type_json = sirius_event_type_json($event_node);
  $prole_tid = $type_json['map']['prole_tid'];
  $event_json = sirius_jsonfield_util_node_json_get($event_node);

  $group_by_address = (
    ($event_json['map']['group_by_address'] == 'Yes') && 
    (!$params['disable_group_by_address'])
  );

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
  $sql = "select group_concat(contact_node.title order by node.nid separator '|SEPARATOR|') as contact_titles, ";
  if ($group_by_address) {
    $sql .= "count(distinct contact_node.nid) as contact_count, ";
  }
  $sql .= "group_concat(contact_node.nid order by node.nid separator '_') as contact_nids, ";
  $sql .= "group_concat(node.nid order by node.nid separator '_') as marker_id, ";
  $sql .= "group_concat(field_sirius_event_pstatus_tid order by node.nid separator '_') as pstatus_tids, ";
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
  if ($show_pstatus_tids) {
    $sql .= "and field_sirius_event_pstatus_tid in (:show_pstatus_tids) ";
    $sql_args[':show_pstatus_tids'] = $show_pstatus_tids;
  }
  if ($group_by_address) {
    $sql .= "group by field_sirius_address_thoroughfare, field_sirius_address_premise, field_sirius_address_locality, field_sirius_address_administrative_area, field_sirius_address_postal_code ";
  } else {
    $sql .= "group by contact_node.nid ";
  }
  $sql .= "limit $limit offset $offset ";

  $stmt = sirius_sql_query($sql, $sql_args);
  while ($hr = $stmt->fetchAssoc()) {
    $pstatus_tids = explode('_', $hr['pstatus_tids']);
    $pstatus_names = array();
    foreach ($pstatus_tids as $pstatus_tid) {
      $pstatus_names[] = $pstatus_terms[$pstatus_tid]->name;
    }

    $distinct_pstatus_tids = array_unique(explode('_', $hr['pstatus_tids']));
    if (count($distinct_pstatus_tids) > 1) {
      $pstatus_tid = $event_json['map']['group_by_address_options']['mixed_pstatus'];
      if (!$pstatus_tid) { $pstatus_tid = $distinct_pstatus_tids[0]; }
    } else {
      $pstatus_tid = $distinct_pstatus_tids[0];
    }

    $pin = $pstatus_pins[$pstatus_tid];

    $titles = explode('|SEPARATOR|', $hr['contact_titles']);
    if ($hr['contact_count'] > 1) {
      $title = $hr['contact_count'] . ' Contacts';
    } else {
      $title = $titles[0];
    }

    $map_feature = array(
      'type' => 'point',
      'lat' => $hr['field_sirius_address_geo_lat'],
      'lon' => $hr['field_sirius_address_geo_lon'],
      'popup' => sirius_map_popup_html(
        $event_node->nid, 
        $titles, 
        $hr['field_sirius_address_thoroughfare'], 
        $hr['field_sirius_address_premise'], 
        $hr['marker_id'], 
        $pstatus_names
      ),
      'marker_id'  => $hr['marker_id'],
      'leaflet_id' => 'marker_' . $hr['marker_id'],
      'sirius' => array(
        'marker' => array('id' => $hr['marker_id']),
        'contact_name' => $title,
        'list_html' => sirius_map_list_html(
          $event_node->nid, 
          $hr['contact_nid'], 
          $title, 
          $hr['field_sirius_address_thoroughfare'], 
          $hr['field_sirius_address_premise'], 
          $hr['field_sirius_address_locality'], 
          $hr['field_sirius_address_administrative_area'], 
          $hr['field_sirius_address_postal_code'], 
          $hr['marker_id'], 
          $pstatus_terms[$pstatus_tid]->name,
          $pstatus_tid,
          $pin['path']
        ),
        'icon_url' => $pin['path'],
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
        'marker_id'  => $hr['participant_nid'],
        'leaflet_id' => 'marker_' . $hr['participant_nid'],
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





























function sirius_event_map_removals($event_node, $params = array()) {
  $cutoff_ts = $params['cutoff_ts'];
  $limit = $params['limit'];
  if (!$limit) { $limit = 3000; }
  $offset = $params['offset'];
  if (!$offset) { $offset = 0; }
  $show_pstatus_tids = $params['show_pstatus_tids'];
  if (!$show_pstatus_tids) { return array(); }

  // Create settings for the map.
  $type_json = sirius_event_type_json($event_node);
  $prole_tid = $type_json['map']['prole_tid'];
  $event_json = sirius_jsonfield_util_node_json_get($event_node);

  $group_by_address = (
    ($event_json['map']['group_by_address'] == 'Yes') && 
    (!$params['disable_group_by_address'])
  );

  $removals = array();

  // Fetch all the relevant data about our participants
  $sql_args = array();
  $sql_args[':event_nid'] = $event_node->nid;
  $sql_args[':prole_tid'] = $prole_tid;
  $sql = "select group_concat(node.nid order by node.nid separator '_') as marker_id ";
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
  if ($show_pstatus_tids) {
    $sql .= "and field_sirius_event_pstatus_tid not in (:show_pstatus_tids) ";
    $sql_args[':show_pstatus_tids'] = $show_pstatus_tids;
  }
  if ($group_by_address) {
    $sql .= "group by field_sirius_address_thoroughfare, field_sirius_address_premise, field_sirius_address_locality, field_sirius_address_administrative_area, field_sirius_address_postal_code ";
  } else {
    $sql .= "group by contact_node.nid ";
  }
  $sql .= "limit $limit offset $offset ";

  $stmt = sirius_sql_query($sql, $sql_args);
  while ($hr = $stmt->fetchAssoc()) {
    $pstatus_tids = explode('_', $hr['pstatus_tids']);
    $pstatus_names = array();
    foreach ($pstatus_tids as $pstatus_tid) {
      $pstatus_names[] = $pstatus_terms[$pstatus_tid]->name;
    }

    $distinct_pstatus_tids = array_unique(explode('_', $hr['pstatus_tids']));
    if (count($distinct_pstatus_tids) > 1) {
      $pstatus_tid = $event_json['map']['group_by_address_options']['mixed_pstatus'];
      if (!$pstatus_tid) { $pstatus_tid = $distinct_pstatus_tids[0]; }
    } else {
      $pstatus_tid = $distinct_pstatus_tids[0];
    }

    $removals[] = $hr['marker_id'];
  }

  return $removals;
}