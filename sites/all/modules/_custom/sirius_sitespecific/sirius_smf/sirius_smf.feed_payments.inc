<?php

function sirius_smf_feed_payments_info() {
  sirius_smf_feed_payments_load_includes();

  $items = array();

  $items['sirius_smf_feed_payments'] = array(
    'name' => 'sirius_smf_feed_payments',
    'title' => 'Payments Importer',
    'description' => 'Import offline payments',

    'steps' => array(
      'upload' => array(
        'label' => 'Upload',
        'callback' => 'sirius_feed_wizard_upload'
      ),
      'map' => array(
        'label' => 'Mapping',
        'callback' => 'sirius_feed_wizard_map'
      ),
      'account' => array(
        'label' => 'Account', 
        'callback' => 'sirius_smf_feed_payments_account_form',
        'hide_limit' => TRUE
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
        'field_sirius_id' => 'Transaction ID',
      ),
      'callback_fields' => 'sirius_smf_feed_payments_fields',
      'batch_size' => 50,
      'callback_row_verify' => 'sirius_smf_feed_payments_row_verify',
      'callback_row_process' =>'sirius_smf_feed_payments_row_process',
      'create_new_records' => 'optional',
    ),
    'callback_launch' => 'sirius_feed_wizard_launch',
    'callback_launch_args' => array('sirius_smf_feed_payments'),
  );

  return $items;
}

function sirius_smf_feed_payments_fields() {
  $fields = array();

  $fields['field_sirius_id'] = array(
    'title' => 'Transaction ID',
    'type' => 'text',
  );

  $fields['field_sirius_dollar_amt'] = array(
    'title' => 'Amount',
  );

  $fields['field_sirius_merchant_name'] = array(
    'title' => 'Merchant Name',
  );

  $fields['field_sirius_check_number'] = array(
    'title' => 'Check Number',
  );

  return $fields;
}

function sirius_smf_feed_payments_load_includes() {
  sirius_require('sirius_smf', 'feed_payments/account.inc');
  sirius_require('sirius_smf', 'feed_payments/verify.inc');
  sirius_require('sirius_smf', 'feed_payments/process.inc');
}
