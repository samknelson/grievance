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
  $sql = "select field_sirius_dispatch_medium_value, delta, count(*) as c ";
  $sql .= "from field_data_field_sirius_dispatch_medium ";
  $sql .= "where entity_type = 'node' ";
  $sql .= "and bundle = 'sirius_worker' ";
  $sql .= "group by field_sirius_dispatch_medium_value, delta ";
  $sql .= "order by field_sirius_dispatch_medium_value, delta ";
  $stmt = db_query($sql, array());

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

  dpm($data);
  foreach ($data as $medium => $ignore) {
    for ($delta = 0; $delta <= 2; ++$delta) {
      if (!$data[$medium]["count$delta"]) { $data[$medium]["count$delta"] = 0; }
      if ($data['total']["count$delta"]) { drupal_set_message("Here with $medium"); $data[$medium]["pct$delta"] = round(100 * $data[$medium]["count$delta"] / $data['total']["count$delta"]) . '%'; }
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

