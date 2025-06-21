<?php

sirius_require('sirius_employer', 'sirius_employer.monthly_upload.inc');
sirius_require('sirius_employer', 'sirius_employer.monthly_upload_access.inc');

sirius_require('sirius_smf', 'sirius_smf.ehrs_monthly.inc');
sirius_require('sirius_smf', 'sirius_smf.ehrs_monthly_access.inc');

function sirius_smf_feed_ehrs_info() {
  $items = array();

  $items['sirius_smf_feed_ehrs'] = array(
    'name' => 'sirius_smf_feed_ehrs',
    'title' => 'Employer Hours Upload',
    'description' => 'Import hours and make payments.',
    'callback_step_list' => 'sirius_smf_feed_ehrs_steps',

    'feed' => array(
      'pks' => array(
        'ssn' => 'SSN',
        'ein' => 'EIN',
      ),
      'callback_fields' => 'sirius_smf_feed_ehrs_fields',
      'batch_size' => 250,
      'callback_row_verify' => 'sirius_smf_feed_ehrs_row_verify',
      'callback_row_process' =>'sirius_smf_feed_ehrs_row_process',
    ),
    'callback_launch' => 'sirius_feed_wizard_launch',
    'callback_launch_args' => array('sirius_smf_feed_ehrs'),
  );

  $items['sirius_smf_feed_ehrs_correction'] = array(
    'name' => 'sirius_smf_feed_ehrs_correction',
    'title' => 'Employer Hours Upload Correction',
    'callback_step_list' => 'sirius_smf_feed_ehrs_steps',

    'feed' => array(
      'pks' => array(
        'ssn' => 'SSN',
        'ein' => 'EIN',
      ),
      'callback_fields' => 'sirius_smf_feed_ehrs_fields',
      'batch_size' => 250,
      'callback_row_verify' => 'sirius_smf_feed_ehrs_row_verify',
      'callback_row_process' =>'sirius_smf_feed_ehrs_row_process',
    ),
    'callback_launch' => 'sirius_feed_wizard_launch',
    'callback_launch_args' => array('sirius_smf_feed_ehrs_correction'),
  );

  return $items;
}

function sirius_smf_feed_ehrs_fields() {
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
  return $fields;
}

function sirius_smf_feed_ehrs_load_includes() {
  sirius_require('sirius_smf', 'feed_ehrs/upload.inc');
  sirius_require('sirius_smf', 'feed_ehrs/fmla.inc');
  sirius_require('sirius_smf', 'feed_ehrs/map.inc');
  sirius_require('sirius_smf', 'feed_ehrs/verify.inc');
  sirius_require('sirius_smf', 'feed_ehrs/newworker.inc');
  sirius_require('sirius_smf', 'feed_ehrs/disability_approved.inc');
  sirius_require('sirius_smf', 'feed_ehrs/disability_unapproved.inc');
  sirius_require('sirius_smf', 'feed_ehrs/leaves.inc');
  sirius_require('sirius_smf', 'feed_ehrs/high_hrs.inc');
  sirius_require('sirius_smf', 'feed_ehrs/term_proposed.inc');
  sirius_require('sirius_smf', 'feed_ehrs/term.inc');
  sirius_require('sirius_smf', 'feed_ehrs/post.inc');
  sirius_require('sirius_smf', 'feed_ehrs/statement.inc');
  sirius_require('sirius_smf', 'feed_ehrs/payment.inc');
  sirius_require('sirius_smf', 'feed_ehrs/review.inc');
}

function sirius_smf_feed_ehrs_steps($data, $wizard_node) {
  sirius_smf_feed_ehrs_load_includes();

  $type = $wizard_node->field_sirius_type['und'][0]['value'];

  $access = Sirius_Smf_Ehrs_Monthly_Access::getInstance();
  $access_context = array('wizard_node' => $wizard_node);

  $steps = &drupal_static(__FUNCTION__);
  if (isset($steps)) { return $steps; }

  $steps = array();
  $steps['upload'] = array(
    'label' => 'Upload',
    'callback' => 'sirius_smf_feed_ehrs_upload_form'
  );
  $steps['map'] = array(
    'label' => 'Mapping',
    'callback' => 'sirius_smf_feed_ehrs_map_form'
  );
  $steps['verify'] = array(
    'label' => 'Parse', 
    'callback' => 'sirius_smf_feed_ehrs_verify_form',
  );

  // Multiple "New Workers"
  $count = 0;
  if ($data['records']['rows']) {
    foreach ($data['records']['rows'] as $row) {
      ++$count;
      if ($row['worker_nid']) { continue; }
      $steps['new_worker_' . $count] = array(
        'label' => 'New Worker: ' . $row['name_last'] . ', ' . $row['name_first'],
        'callback' => 'sirius_smf_feed_ehrs_newworker_form',
        'idx' => $count-1,
      );
    }
  }

  $steps['fmla'] = array(
    'label' => 'FMLA', 
    'callback' => 'sirius_smf_feed_ehrs_fmla_form',
  );

  $steps['disability_approved'] = array(
    'label' => 'Approved Disability', 
    'callback' => 'sirius_smf_feed_ehrs_disability_approved_form',
  );

  // Multiple "Unapproved Disability"
  $count = 0;
  if ($data['records']['rows']) {
    foreach ($data['records']['rows'] as $row) {
      ++$count;
      if ($row['hour_type_id'] != 'disability') { continue; }
      if ($row['disability_approved']['approved']) { continue; }
      $steps['disability_unapproved_' . $count] = array(
        'label' => 'Un-Approved Disability: ' . $row['name_last'] . ', ' . $row['name_first'],
        'callback' => 'sirius_smf_feed_ehrs_disability_unapproved_form',
        'idx' => $count-1,
      );
    }
  }

  $steps['leaves'] = array(
    'label' => 'Other Leaves', 
    'callback' => 'sirius_smf_feed_ehrs_leaves_form',
  );

  $steps['high_hrs'] = array(
    'label' => 'High Hours', 
    'callback' => 'sirius_smf_feed_ehrs_high_hrs_form',
  );

  if ($type != 'sirius_smf_feed_ehrs_correction') {
    $steps['term_proposed'] = array(
      'label' => 'Proposed Terminations', 
      'callback' => 'sirius_smf_feed_ehrs_term_proposed_form',
    );
  }

  $steps['term'] = array(
    'label' => 'Terminations', 
    'callback' => 'sirius_smf_feed_ehrs_term_form',
  );

  $steps['post'] = array(
    'label' => 'Post', 
    'callback' => 'sirius_smf_feed_ehrs_post_form', 
  );

  $steps['statement'] = array(
    'label' => 'Statement', 
    'callback' => 'sirius_smf_feed_ehrs_statement_form'
  );

  if ($data['paymentchoice'] == 'online') {
    $steps['payment'] = array(
      'label' => 'Payment', 
      'callback' => 'sirius_smf_feed_ehrs_payment_form'
    );
  }

  $steps['review'] = array(
    'label' => 'Review', 
    'callback' => 'sirius_smf_feed_ehrs_review_form'
  );

  return $steps;
}

function sirius_smf_feed_ehrs_get_eup($feed_node) {
  $lookup = &drupal_static(__FUNCTION__);
  if (isset($lookup[$feed_node->nid])) { return $lookup[$feed_node->nid]; }

  $sql = "select nid from node ";
  $sql .= "left join field_data_field_sirius_type on field_data_field_sirius_type.entity_type = 'node' and field_data_field_sirius_type.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_category on field_data_field_sirius_category.entity_type = 'node' and field_data_field_sirius_category.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_log_handler on field_data_field_sirius_log_handler.entity_type = 'node' and field_data_field_sirius_log_handler.entity_id = node.nid ";
  $sql .= "where node.type = 'sirius_log' ";
  $sql .= "and field_sirius_category_value = 'smf:ehrs' ";
  $sql .= "and field_sirius_log_handler_target_id = :feed_nid ";
  $sql_args[':feed_nid'] = $feed_node->nid;

  $stmt = sirius_sql_query($sql, $sql_args);
  $hr = $stmt->fetchAssoc();
  return ($lookup[$feed_node->nid] = node_load($hr['nid']));
}

function sirius_smf_feed_ehrs_form_prep(&$form, &$form_state, $info, $data, $wizard_node, $current_step) {
  drupal_add_css(drupal_get_path('module', 'sirius') . '/css/ux.css', 'file');

  $form_state['info'] = $info;
  $form_state['data'] = $data;
  $form_state['wizard_node'] = $wizard_node;
  $form_state['current_step'] = $current_step;

  $form['#tree'] = TRUE;

  if ($current_step != 'map') {
    $form['survey_wrap_open'] = array('#markup' => '<div id="survey_wrap" class="survey_wrap_inpage sirius-form">', '#weight' => -100);
    $form['survey_wrap_close'] = array('#markup' => '</div>', '#weight' => 100);
  }

  // If
  // - The wizard is "complete"
  // - We're not admin
  // - We're not on the "statement" step
  // then disable the form

  $status = $wizard_node->field_sirius_feed_status['und'][0]['value'];
  if ( ($status == 'complete') && 
       !in_array($current_step, array('statement', 'payment', 'review')) &&
       !user_access("sirius trust admin")) {
    $form['#disabled'] = TRUE;
    unset($form['submit']);
  }

  return sirius_form_prep($form);
}

function sirius_smf_feed_ehrs_row_process($feed_node, $info, $data, $row) {
  return array('success' => FALSE, 'msg' => 'Disabled');

  $result = sirius_smf_feed_ehrs_row_verify($feed_node, $info, $data, $row, TRUE);
  if (!$result['success']) { return $result; }

  if ($row['nid']) {
    $is_new = FALSE;
    $node = node_load($row['nid']);
  } else {
    $is_new = TRUE;
    global $user;
    $node = new stdClass();
    $node->type = $info['feed']['content_type'];
    $node->language = LANGUAGE_NONE;
    $node->uid = $user->uid;
    $node->status = 1;
    $node->promote = 0;
    $node->comment = 0;
    $node->field_sirius_domain = $feed_node->field_sirius_domain;
  }

  if ($row['title']) { $node->title = $row['title']; }
  if ($row['sirius_id']) {
    $node->field_sirius_id['und'][0]['value'] = $row['sirius_id'];
  }

  foreach ($info['feed']['fields'] as $field_name => $field_info) {
    if ($row[$field_name]) {
      if (in_array($field_info['type'], array('integer', 'list', 'text', 'long', 'date', 'phone', 'ssn'))) {
        $node->{"field_$field_name"}['und'][0]['value'] = $row[$field_name];
      }
      if ($field_info['type'] == 'term_reference') {
        $node->{"field_$field_name"}['und'][0]['tid'] = $row[$field_name];
      }
      if ($field_info['type'] == 'entity_reference') {
        $node->{"field_$field_name"}['und'][0]['target_id'] = $row[$field_name];
      }
    }
  }

  foreach (array('thoroughfare', 'premise', 'locality', 'administrative_area', 'postal_code', 'country') as $col) {
    $value = $row["sirius_address_$col"];
    if (!$value) { continue; }
    if ($col == 'postal_code') { $value = sirius_addr_zip_zeropad($value); }
    $node->field_sirius_address['und'][0][$col] = $value;
  }

  $node = node_submit($node);
  node_save($node);

  if ($is_new) {
    return array('success' => TRUE, 'msg' => 'Created a new record.');
  } else {
    return array('success' => TRUE, 'msg' => 'Updated an existing record.');
  }
}
