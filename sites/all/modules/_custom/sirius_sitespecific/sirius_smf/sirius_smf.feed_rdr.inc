<?php

function sirius_smf_feed_rdr_info() {
  sirius_smf_feed_rdr_load_includes();

  $items = array();

  $items['sirius_smf_rdr_payments'] = array(
    'name' => 'sirius_smf_feed_rdr',
    'title' => 'RDR Validator',
    'description' => 'Generate the RDR validation data',

    'steps' => array(
      'upload' => array(
        'label' => 'Upload',
        'callback' => 'sirius_feed_wizard_upload'
      ),
      'map' => array(
        'label' => 'Mapping',
        'callback' => 'sirius_feed_wizard_map'
      ),
      'verify' => array(
        'label' => 'Verify', 
        'callback' => 'sirius_feed_wizard_verify',
        'hide_limit' => TRUE
      ),
      'process' => array(
        'label' => 'Process', 
        'callback' => 'sirius_feed_wizard_process', 
        'hide_limit' => TRUE
      ),
      'review' => array(
        'label' => 'Review', 
        'callback' => 'sirius_feed_wizard_review'
      ),
    ),

    'feed' => array(
      'pks' => array(
        'field_sirius_id' => 'SSN + Date',
      ),
      'callback_fields' => 'sirius_smf_feed_rdr_fields',
      'batch_size' => 50,
      'callback_row_verify' => 'sirius_smf_feed_rdr_row_verify',
      'callback_row_process' =>'sirius_smf_feed_rdr_row_process',
      'create_new_records' => FALSE,
      'results_echo_input' => TRUE,
      'results_fields' => array(
        'covered_in_month' => 'Covered in Month',
        'tier' => 'Tier',
        'status' => 'Status',
      ),
    ),
    'callback_launch' => 'sirius_feed_wizard_launch',
    'callback_launch_args' => array('sirius_smf_rdr_payments'),
  );

  return $items;
}

function sirius_smf_feed_rdr_fields() {
  $fields = array();

  $fields['ssn'] = array(
    'title' => 'SSN',
    'type' => 'text',
  );

  $fields['date'] = array(
    'title' => 'Date',
  );

  $fields['discrepancy_category'] = array(
    'title' => 'Discrepancy Category',
  );

  return $fields;
}

function sirius_smf_feed_rdr_load_includes() {
  sirius_require('sirius_smf', 'feed_rdr/verify.inc');
  sirius_require('sirius_smf', 'feed_rdr/process.inc');
}
