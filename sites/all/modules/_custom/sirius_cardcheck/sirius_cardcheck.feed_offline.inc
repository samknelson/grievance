<?php

function sirius_cardcheck_feed_offline_wizard_info() {

  $items = array();

  $items['sirius_cardcheck_feed_offline'] = array(
    'name' => 'sirius_cardcheck_feed_offline',
    'title' => 'Offline Signatures',
    'steps' => array(
      'upload' => array(
        'label' => 'Upload',
        'callback' => 'sirius_feed_wizard_upload'
      ),
      'settings' => array(
        'label' => 'Settings',
        'callback' => 'sirius_cardcheck_feed_offline_wizard_info_settings'
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
      'content_type' => 'sirius_log',
      'pks' => array(
        'id' => 'ID',
      ),
      'fields' => array(
        'sirius_id' => array('title' => 'Worker ID', 'type' => 'text'),
        'bu' => array('title' => 'Bargaining Unit', 'type' => 'text'),
        'filename' => array('title' => 'filename'),
        'signature_date' => array('title' => 'Signature Date', 'type' => 'date'),
      ),
      'batch_size' => 50,
      'callback_row_verify' => 'sirius_cardcheck_feed_offline_row_verify',
      'callback_row_process' =>'sirius_cardcheck_feed_offline_row_process',
      'create_new_records' => 'optional',
    ),
    'callback_launch' => 'sirius_feed_wizard_launch',
    'callback_launch_args' => array('sirius_cardcheck_feed_offline'),
  );

  foreach ($items as $key => $item) { sirius_feed_info_set_titles($items[$key]); }
  return $items;
}

function sirius_cardcheck_feed_offline_row_verify($feed_node, $info, $data, &$row, $precompute) {
  $pk = $data['pk'];

  static $rowcount;
  $rowcount++;

  $instance = Sirius_Cardcheck::getInstance();

  // Trim all
  foreach ($row as $key => $value) {
    if (!is_array($value)) { $row[$key] = trim($value); }
  }

  foreach ($info['feed']['fields'] as $field_name => $field_info) {
    if (!$row[$field_name]) { continue; }

    $result = sirius_feed_handle_field($field_name, $field_info, $row[$field_name], $precompute);
    if (!$result['success']) { return $result; }
  }

  // Find the bargaining unit
  $bu_id = $row['bu'];
  if ($bu_id) {
    $bu_nid = sirius_find_nid_by_id($bu_id, 'sirius_bu');
    if (!$bu_nid) {
      return array('success' => FALSE, 'msg' => "Bargaining unit [$bu_id] not found.");
    }
    $row['bu_nid'] = $bu_nid;
  }

  // Get the date
  $ts = strtotime($row['signature_date']);
  if ($ts === NULL) {
    return array('success' => FALSE, 'msg' => 'Please enter a valid date.');
  }

  $content_type = $info['feed']['content_type'];
  if ($precompute) {
    if ($pk = 'sirius_id') {
      if (!$row['sirius_id']) { return array('success' => FALSE, 'msg' => 'No ID.'); }

      $result = sirius_feed_lookup_by_field(
        'sirius_worker', 
        'field_data_field_sirius_id', 
        'field_sirius_id_value', 
        $row['sirius_id']
      );

      if ($result['success']) {
        // We found exactly one. Use that NID.
        $row['worker_nid'] = $result['nid'];
      } else {
        return $result;
      }
    } else {
      return array('success' => FALSE, 'msg' => 'Missing or unknown pk $pk.');
    }

    // Already signed?
    $options = array();
    $options['worker_nid'] = $row['worker_nid'];
    $options['definition_nid'] = $data['cardcheck']['definition_nid'];
    $options['status'] = 'signed';
    $probe = $instance->search($options);
    if ($probe) {
      return array('success' => FALSE, 'msg' => 'There is a signed cardcheck on file for that worker.');
    }
  }

  if (!$msg) { $msg = "Looks good."; }
  return array('success' => TRUE, 'msg' => $msg);
}

function sirius_cardcheck_feed_offline_row_process($feed_node, $info, $data, $row) {
  // return array('success' => FALSE, 'msg' => 'Disabled');
  $result = sirius_cardcheck_feed_offline_row_verify($feed_node, $info, $data, $row, TRUE);
  if (!$result['success']) { return $result; }

  $instance = Sirius_Cardcheck::getInstance();
  $definition_node = node_load($data['cardcheck']['definition_nid']);
  $worker_node = node_load($row['worker_nid']);

  // If there's an unsigned, we sign it.
  $options = array();
  $options['worker_nid'] = $row['worker_nid'];
  $options['definition_nid'] = $data['cardcheck']['definition_nid'];
  $options['status'] = 'unsigned';
  $probe = $instance->search($options);
  if ($probe) {
    $cardcheck_node = reset($probe);
  } else {
     $cardcheck_node = $instance->create($definition_node, array('entity_node' => $worker_node));
  }

  $values = array();
  if ($row['bu_nid']) {
    $values['bu_nid'] = $row['bu_nid'];
  }
  $values['filename'] = $row['filename'];
  $values['signature_date'] = $row['signature_date'];

  $instance->accept_offline($cardcheck_node, $values);

  return array('success' => TRUE, 'msg' => 'Cardcheck imported');
}






function sirius_cardcheck_feed_offline_wizard_info_settings($info, $data, $wizard_node, $current_step) { 
  return drupal_get_form('sirius_cardcheck_feed_offline_wizard_info_settings_form', $info, $data, $wizard_node, $current_step);
}

function sirius_cardcheck_feed_offline_wizard_info_settings_form($form, &$form_state, $info, $data, $wizard_node, $current_step) {

  drupal_add_css(drupal_get_path('module', 'sirius') . '/css/ux.css');

  $form_state['info'] = $info;
  $form_state['data'] = $data;
  $form_state['wizard_node'] = $wizard_node;
  $form_state['current_step'] = $current_step;

  $form['#tree'] = TRUE;

  $instance = Sirius_Cardcheck::getInstance();
  $definitions = $instance->definition_list();

  $options = array();
  foreach ($definitions as $definition_nid => $definition_node) {
    $options[$definition_nid] = $definition_node->title;
  }

  $form['cardcheck']['definition_nid'] = array(
    '#type' => 'select',
    '#title' => t('Cardcheck'),
    '#default_value' => $data['cardcheck']['definition_nid'],
    '#required' => TRUE,
    '#options' => array(NULL => t('-- Select --')) + $options,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;  
}

function sirius_cardcheck_feed_offline_wizard_info_settings_form_submit($form, &$form_state) {
  $info = $form_state['info'];
  $data = $form_state['data'];
  $wizard_node = $form_state['wizard_node'];
  $current_step = $form_state['current_step'];

  $data['cardcheck'] = $form_state['values']['cardcheck'];
  sirius_wizard_submit($info, $data, $wizard_node, $current_step, "Settings saved.", array('nonsequential' => FALSE));
}
