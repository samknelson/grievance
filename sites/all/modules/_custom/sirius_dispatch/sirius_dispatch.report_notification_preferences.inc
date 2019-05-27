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
  $fields['count0'] = 'Count Dispatch Primary';
  $fields['pct0'] = 'Percent Dispatch Primary';
  $fields['count1'] = 'Count Dispatch Secondary';
  $fields['pct1'] = 'Percent Dispatch Secondary';
  $fields['count2'] = 'Count Non-Dispatch';
  $fields['pct2'] = 'Percent Non-Dispatch';
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

    $data['total']['count0'] += $data[$medium]['count0'];
    $data['total']['count1'] += $data[$medium]['count1'];
    $data['total']['count2'] += $data[$medium]['count2'];

    if (preg_match('/sms/', $medium)) {
      $data['sms_total']['count0'] += $data[$medium]['count0'];
      $data['sms_total']['count1'] += $data[$medium]['count1'];
      $data['sms_total']['count2'] += $data[$medium]['count2'];
    }

    if (preg_match('/voice/', $medium)) {
      $data['voice_total']['count0'] += $data[$medium]['count0'];
      $data['voice_total']['count1'] += $data[$medium]['count1'];
      $data['voice_total']['count2'] += $data[$medium]['count2'];
    }
  }


  foreach ($data as $medium => $ignore) {
    if ($data['total']['count0']) { $data[$medium]['pct0'] = round(100 * $data[$medium]['count0'] / $data['total']['count0']); }
    if ($data['total']['count1']) { $data[$medium]['pct1'] = round(100 * $data[$medium]['count1'] / $data['total']['count1']); }
    if ($data['total']['count2']) { $data[$medium]['pct2'] = round(100 * $data[$medium]['count2'] / $data['total']['count2']); }
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

function sirius_dispatch_report_notification_preferences_view($report_type, $report_id) {
  // Load the parameters
  $query = db_select('report_cache');
  $query->fields('report_cache');
  $query->condition('report_id', $report_id);
  $query->condition('report_type', $report_type . ' :: PARAMETERS');
  $stmt = $query->execute();
  $hr = $stmt->fetchAssoc();
  $parameters = unserialize($hr['report_data']);
  $ratio_1_tid = $parameters['ratio_1'];
  $ratio_2_tid = $parameters['ratio_2'];
  $ratio_1_term = taxonomy_term_load($ratio_1_tid);
  $ratio_2_term = taxonomy_term_load($ratio_2_tid);

  $html = '';
  $html .= '<h2>';
  $html .= 'Ratio of employed workers with the member status <strong>' . $ratio_1_term->name . '</strong> to <strong>' . $ratio_2_term->name . '</strong>';
  $html .= '</h2>';

  $query = db_select('report_cache');
  $query->fields('report_cache');
  $query->condition('report_id', $report_id);
  $query->condition('report_type', $report_type);
  $stmt = $query->execute();
  $data = array();
  $grand_total_1 = 0;
  $grand_total_2 = 0;
  while ($hr = $stmt->fetchAssoc()) {
    $hr = unserialize($hr['report_data']);

    $count_1 = $hr['status_' . $ratio_1_tid];
    $count_2 = $hr['status_' . $ratio_2_tid];

    $grand_total_1 += $count_1;
    $grand_total_2 += $count_2;

    $total = $count_1 + $count_2; 
    if (!$total) { continue; }

    $key = $hr['shop_name'];

    $row = array();
    $row[] = $key;
    $row[] = $total;
    $row[] = $count_1;
    $row[] = (100 *number_format($count_1 / $total, 2)) . ' %';
    $row[] = $count_2;
    $row[] = (100 *number_format($count_2 / $total, 2)) . ' %';



    $data[$key] = $row;
  }

  ksort($data);
  $rows = array_values($data);

  $grand_total = $grand_total_1 + $grand_total_2;
  if ($grand_total) {
  	$row = array();
  	$row[] = '<strong>' . 'Total' . '</strong>';
    $row[] = '<strong>' . $grand_total . '</strong>';
    $row[] = '<strong>' . $grand_total_1 . '</strong>';
    $row[] = '<strong>' . (100 *number_format($grand_total_1 / $grand_total, 2)) . ' %' . '</strong>';
    $row[] = '<strong>' . $grand_total_2;
    $row[] = '<strong>' . (100 *number_format($grand_total_2 / $grand_total, 2)) . ' %' . '</strong>';
    $rows[] = $row;
  }

  $header = array(
  	'Shop Name',
  	'Total Workers',
  	'Count: ' . $ratio_1_term->name,
  	'Fraction: ' . $ratio_1_term->name,
  	'Count: ' . $ratio_2_term->name,
  	'Fraction: ' . $ratio_2_term->name,
  );

  $attributes = array();
  $html .= theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => $attributes));

  return $html;
}