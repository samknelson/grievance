<?php

function sirius_dispatch_report_notification_preferences_form($form, &$form_state, $report_type) {
  report_cache_form_prepare($report_type, $form, $form_state);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Generate',
  );
  
  return $form;
}

function sirius_dispatch_report_notification_preferences_form_submit($form, &$form_state) { return report_cache_form_submit($form, $form_state); }

function sirius_dispatch_report_notification_preferences_fields() {
	$fields = array();
	$fields['medium'] = 'Medium';
  $fields['count0'] = '# Dispatch';
  $fields['pct0'] = '% Dispatch';
  $fields['count1'] = '# Secondary Dispatch';
  $fields['pct1'] = '% Secondary Dispatch';
  $fields['count2'] = '# Other Notifications';
  $fields['pct2'] = '% Other Notification';
	return $fields;
}

function sirius_dispatch_report_notification_preferences_pks(&$form_state) {
  return array('onekey');
}

function sirius_dispatch_report_notification_preferences_batch($medium, $values, $report_id, $report_action, &$context) {
  // Get the "active" work statuses
  $all_ws_terms = sirius_taxonomy_load('sirius_work_status', 'tid', 'full');
  $active_ws_tids = array();
  foreach ($all_ws_terms as $tid => $term) {
    if ($term->field_sirius_dispatch_available['und'][0]['value'] == 'Yes') {
      $active_ws_tids[] = $tid;
    }
  }

  $sql = "select field_sirius_dispatch_medium_value, field_data_field_sirius_dispatch_medium.delta, count(*) as c ";
  $sql .= "from field_data_field_sirius_dispatch_medium ";
  $sql .= "join field_data_field_sirius_work_status ";
  $sql .= "  on field_data_field_sirius_work_status.entity_type = 'node' ";
  $sql .= "  and field_data_field_sirius_work_status.entity_id = field_data_field_sirius_dispatch_medium.entity_id ";
  $sql .= "  and field_data_field_sirius_work_status.field_sirius_work_status_tid in (:active_ws_tids) ";
  $sql .= "where field_data_field_sirius_dispatch_medium.entity_type = 'node' ";
  $sql .= "and field_data_field_sirius_dispatch_medium.bundle = 'sirius_worker' ";
  $sql .= "group by field_sirius_dispatch_medium_value, field_data_field_sirius_dispatch_medium.delta ";
  $sql .= "order by field_sirius_dispatch_medium_value, field_data_field_sirius_dispatch_medium.delta ";
  $stmt = db_query($sql, array(':active_ws_tids' => $active_ws_tids));

  $data = array();
  while ($hr = $stmt->fetchAssoc()) {
    $medium = $hr['field_sirius_dispatch_medium_value'];
    $delta = $hr['delta'];
    $count = $hr['c'];

    $data[$medium]['medium'] = $medium;
    $data[$medium]["count$delta"] = $count;
  }

  $data['total'] = array('medium' => 'Total');
  $data['sms_total'] = array('medium' => 'SMS Total');
  $data['voice_total'] = array('medium' => 'Voice Total');

  foreach ($data as $medium => $ignore) {
    if (preg_match('/total/', $medium)) { continue; }
    for ($delta = 0; $delta <= 2; ++$delta) {
      $data['total']["count$delta"] += $data[$medium]["count$delta"];
      if (preg_match('/sms/', $medium)) { $data['sms_total']["count$delta"] += $data[$medium]["count$delta"]; }
      if (preg_match('/voice/', $medium)) { $data['voice_total']["count$delta"] += $data[$medium]["count$delta"]; }
    }
  }

  foreach ($data as $medium => $ignore) {
    for ($delta = 0; $delta <= 2; ++$delta) {
      if (!$data[$medium]["count$delta"]) { $data[$medium]["count$delta"] = 0; }
      if ($data['total']["count$delta"]) { $data[$medium]["pct$delta"] = round(100 * $data[$medium]["count$delta"] / $data['total']["count$delta"]) . '%'; }
    }
  }

  foreach ($data as $medium => $ignore) {
    for ($delta = 0; $delta <= 2; ++$delta) {
      $data[$medium]["count$delta"] = number_format($data[$medium]["count$delta"], 0);
    }
  }

  $media_in_order = array(
    'none',
    'sms', 
    'sms_alt', 
    'sms_total',
    'voice',
    'voice_alt',
    'voice_total',
    'email',
    'total'
  );

  $rows = array();
  $rows['is_multiple'] = TRUE;
  $rows['rows'] = array();
  foreach ($media_in_order as $medium) {
    if ($data[$medium]) { $rows['rows'][] = $data[$medium]; }
  }

  return $rows;
}

