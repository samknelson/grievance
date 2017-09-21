<?php

function grievance_report_status_summary() {
  return drupal_get_form('grievance_report_status_summary_form');
}

function grievance_report_status_summary_form($form, &$form_state) {
  $report_type = 'grievance-report-status-summary';
  report_cache_form_prepare($report_type, $form, $form_state);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Generate',
  );
  
  return $form;
}
