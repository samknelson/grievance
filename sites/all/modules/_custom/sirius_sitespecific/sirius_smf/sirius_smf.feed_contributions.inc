<?php

sirius_require('sirius_smf', 'sirius_smf.feed_contributions.fields.inc');
sirius_require('sirius_smf', 'sirius_smf.feed_members.mapping.inc');
sirius_require('sirius_smf', 'sirius_smf.feed_contributions.verify.inc');
sirius_require('sirius_smf', 'sirius_smf.feed_contributions.process.inc');
/*
sirius_require('sirius_smf', 'sirius_smf.feed_members.employer_mapping.inc');
*/

function sirius_smf_feed_contributions_info() {

  $items = array();

  $items['sirius_smf_feed_contributions'] = array(
    'name' => 'sirius_smf_feed_contributions',
    'title' => 'Contributions Import',
    'description' => 'Import new or existing contribution records from the TPA.',
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
        'ssn' => '[Multipl field generated]',
      ),
      'callback_fields' => 'sirius_smf_feed_contributions_fields',
      'batch_size' => 50,
      'callback_row_verify' => 'sirius_smf_feed_contributions_row_verify',
      'callback_row_process' =>'sirius_smf_feed_contributions_row_process',
      'create_new_records' => 'optional',
    ),
    'callback_launch' => 'sirius_feed_wizard_launch',
    'callback_launch_args' => array('sirius_smf_feed_contributions'),
  );

  return $items;
}

