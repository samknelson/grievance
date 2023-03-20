<?php

sirius_require('sirius_smf', 'sirius_smf.feed_dependents.fields.inc');
sirius_require('sirius_smf', 'sirius_smf.feed_members.mapping.inc');
sirius_require('sirius_smf', 'sirius_smf.feed_dependents.verify.inc');
sirius_require('sirius_smf', 'sirius_smf.feed_dependents.process.inc');
/*
sirius_require('sirius_smf', 'sirius_smf.feed_members.employer_mapping.inc');
*/

function sirius_smf_feed_dependents_info() {

  $items = array();

  $items['sirius_smf_feed_dependents_rel'] = array(
    'name' => 'sirius_smf_feed_dependents_rel',
    'title' => 'Dependents Import - Workers and Relationships',
    'description' => 'Import new or existing dependent records from the TPA.',
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
      'callback_fields' => 'sirius_smf_feed_dependents_fields',
      'batch_size' => 50,
      'callback_row_verify' => 'sirius_smf_feed_dependents_row_verify_rel',
      'callback_row_process' =>'sirius_smf_feed_dependents_row_process_rel',
      'create_new_records' => 'optional',
    ),
    'callback_launch' => 'sirius_feed_wizard_launch',
    'callback_launch_args' => array('sirius_smf_feed_dependents_rel'),
  );

  $items['sirius_smf_feed_dependents_cov'] = array(
    'name' => 'sirius_smf_feed_dependents_cov',
    'title' => 'Dependents Import - Coverage',
    'description' => 'Create coverage records for dependents',
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
      'callback_fields' => 'sirius_smf_feed_dependents_fields',
      'batch_size' => 50,
      'callback_row_verify' => 'sirius_smf_feed_dependents_row_verify_cov',
      'callback_row_process' =>'sirius_smf_feed_dependents_row_process_cov',
      'create_new_records' => 'optional',
    ),
    'callback_launch' => 'sirius_feed_wizard_launch',
    'callback_launch_args' => array('sirius_smf_feed_dependents_cov'),
  );

  return $items;
}

