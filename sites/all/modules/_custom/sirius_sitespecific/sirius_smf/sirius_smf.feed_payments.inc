<?php

function sirius_smf_feed_payments_info() {
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
        'callback' => 'sirius_smf_feed_members_map'
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
        'ssn' => 'SSN',
      ),
      'callback_fields' => 'sirius_smf_feed_members_fields',
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
  $fields['ssn'] = array(
    'title' => 'SSN',
    'type' => 'ssn',
  );
  $fields['ein'] = array(
    'title' => 'EIN',
  );
  $fields['name_first'] = array('title' => 'Name: First');
  $fields['name_middle'] = array('title' => 'Name: Middle');
  $fields['name_last'] = array('title' => 'Name: Last');
  $fields['phone'] = array(
    'title' => 'Phone',
    'type' => 'phone',
  );
  $fields['email'] = array(
    'title' => 'Email',
    'type' => 'email',
  );
  $fields['dob'] = array(
    'title' => 'DoB',
    'type' => 'dob',
  );
  $fields['addr_thoroughfare'] = array('title' => 'Address: Street');
  $fields['addr_premise'] = array('title' => 'Address: Street 2');
  $fields['addr_locality'] = array('title' => 'Address: City');
  $fields['addr_administrative_area'] = array('title' => 'Address: State');
  $fields['addr_postal_code'] = array('title' => 'Address: Zip');
  $fields['hour_type'] = array(
    'title' => 'Employment Status',
    'type' => 'term_reference',
    'taxonomy' => 'sirius_hour_type',
  );
  $fields['hrs'] = array('title' => 'Hours');
  $fields['withheld'] = array('title' => 'Employee Contribution Amount Witheld');
  return $fields;
}

function sirius_smf_feed_payments_load_includes() {
  // sirius_require('sirius_smf', 'feed_payments/review.inc');
}

function sirius_smf_feed_payments_steps($data, $wizard_node) {
  sirius_smf_feed_payments_load_includes();

  $type = $wizard_node->field_sirius_type['und'][0]['value'];

  $steps = &drupal_static(__FUNCTION__);
  if (isset($steps)) { return $steps; }

  $steps = array();
  $steps['upload'] = array(
    'label' => 'Upload',
    'callback' => 'sirius_smf_feed_payments_upload_form'
  );
  $steps['map'] = array(
    'label' => 'Mapping',
    'callback' => 'sirius_smf_feed_payments_map_form'
  );
  $steps['verify'] = array(
    'label' => 'Parse', 
    'callback' => 'sirius_smf_feed_payments_verify_form',
  );

  return $steps;
}

