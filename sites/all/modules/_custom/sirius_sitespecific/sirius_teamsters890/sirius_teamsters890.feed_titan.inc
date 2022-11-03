<?php

function sirius_teamsters890_feed_titan_info() {
  $items = array();
  $items['sirius_teamsters890_feed_titan'] = array(
    'name' => 'sirius_teamsters890_feed_titan',
    'title' => 'Teamsters 890 - Titan Import',
    'description' => 'Import new or existing workers from titan.',
    'steps' => array(
      'upload' => array(
        'label' => 'Upload',
        'callback' => 'sirius_teamsters890_feed_titan_upload'
      ),
      'map' => array(
        'label' => 'Mapping',
        'callback' => 'sirius_teamsters890_feed_titan_map'
      ),
      'verify' => array(
        'label' => 'Verify', 
        'callback' => 'sirius_teamsters890_feed_titan_verify',
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
        // 'nid' => 'Internal ID (NID)',
        // 'id' => 'External ID',
        'ssn' => 'SSN',
      ),
      'callback_fields' => 'sirius_feed_worker_fields',
      'batch_size' => 50,
      'callback_row_verify' => 'sirius_teamsters890_feed_titan_row_verify',
      'callback_row_process' =>'sirius_teamsters890_feed_titan_row_process',
    ),
    'callback_launch' => 'sirius_feed_wizard_launch',
    'callback_launch_args' => array('sirius_teamsters890_feed_titan'),
  );

  return $items;
}

function sirius_teamsters890_feed_titan_row_verify($feed_node, $info, $data, &$row, $precompute) {
  // Titan IDs must begin with a 0
  if ($row['id']) {
    if (!preg_match('/^0/', $row['id'])) {
      $row['id'] = '0' . $row['id'];
    }
  }
  $result = sirius_feed_worker_row_verify($feed_node, $info, $data, $row, $precompute);
  return $result;
}

function sirius_teamsters890_feed_titan_row_process($feed_node, $info, $data, $row) {
  drupal_set_message($row['id']);
  $result = sirius_feed_worker_row_process($feed_node, $info, $data, $row, TRUE);
  if (!$result['success']) { return $result; }
}

/************************************************************************************************
 * Step 1: Upload
 ************************************************************************************************/

function sirius_teamsters890_feed_titan_upload($info, $data, $feed_node, $current_step) { return drupal_get_form('sirius_teamsters890_feed_titan_upload_form', $info, $data, $feed_node, $current_step); }

function sirius_teamsters890_feed_titan_upload_form($form, &$form_state, $info, $data, $feed_node, $current_step) {
  $form_state['info'] = $info;
  $form_state['data'] = $data;
  $form_state['feed_node'] = $feed_node;
  $form_state['current_step'] = $current_step;
  $steps = sirius_wizard_steps($info, $data, $feed_node);
  $last_data = sirius_feed_lastdata($info, $feed_node);

  if ($feed_node->field_sirius_attachments['und'][0]) {
    $mode = 'full';
    $renderable = field_view_field('node', $feed_node, 'field_sirius_attachments', 'full');
    $renderable['#title'] = 'Currently Processing';
    $form['current'] = array('#markup' => render($renderable));
    if ($data['origname']) {

      $form['origname'] = array('#markup' => sirius_fakerender_field('Uploaded Filename:', $data['origname']));
    }
    $title = "Replace with";
  } else {
    $title = "Upload";
  }

  $form['file'] = array(
    '#type' => 'file',
    '#title' => $title,
  );

  $form['source'] = array('#type' => 'value', '#value' => 'Import ' . date('Y-m-d H:i:s'));
  $form['pk'] = array('#type' => 'value', '#value' => 'ssn');
  $form['header_row'] = array('#type' => 'value', '#value' => 'Yes');

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  return $form;
}

function sirius_teamsters890_feed_titan_upload_form_validate($form, &$form_state) {
  return sirius_feed_wizard_upload_form_validate($form, $form_state);
}

function sirius_teamsters890_feed_titan_upload_form_submit($form, &$form_state) {
  return sirius_feed_wizard_upload_form_submit($form, $form_state);
}

/************************************************************************************************
 * Step 2: Mapping
 ************************************************************************************************/

function sirius_teamsters890_feed_titan_map($info, $data, $feed_node, $current_step) { return drupal_get_form('sirius_teamsters890_feed_titan_map_form', $info, $data, $feed_node, $current_step); }

function sirius_teamsters890_feed_titan_map_form($form, &$form_state, $info, $data, $feed_node, $current_step) {
  $form_state['info'] = $info;
  $form_state['data'] = $data;
  $form_state['feed_node'] = $feed_node;
  $form_state['current_step'] = $current_step;
  $steps = sirius_wizard_steps($info, $data, $feed_node);
  $last_data = sirius_feed_lastdata($info, $feed_node);

  // Preview
  $reader = sirius_feed_spout_get_reader($feed_node);
  $sheet = sirius_feed_spout_get_sheet($reader, $data['worksheet']);
  $header = sirius_feed_spout_get_header($sheet, $data['header_row']);

  $count = 0;
  $html_rows = array();

  if (!$sheet) { 
    drupal_set_message("Unable to load spreadsheet.", 'error');
    return array();
  }

  foreach ($sheet->getRowIterator() as $row) {
    if ($count == 0 && $data['header_row'] == 'Yes') { ++$count; continue; }
    $html_row = array();
    foreach ($header as $col => $ignore) { $html_row[] = sirius_feed_spout_get_cell($row, $col); }
    $html_rows[] = $html_row;
    ++$count; 
    if ($count >= 10) { break; }
  }

  $fields = sirius_feed_wizard_fields($info, $data, $feed_node, $current_step);

  $form['map_shop_name'] = array('#type' => 'value', '#value' => '0');
  $form['map_ssn'] = array('#type' => 'value', '#value' => '1');
  $form['map_id'] = array('#type' => 'value', '#value' => '2');
  $form['map_name_family'] = array('#type' => 'value', '#value' => '3');
  $form['map_name_given'] = array('#type' => 'value', '#value' => '4');
  $form['map_name_middle'] = array('#type' => 'value', '#value' => '5');
  $form['map_address_thoroughfare'] = array('#type' => 'value', '#value' => '6');
  $form['map_address_locality'] = array('#type' => 'value', '#value' => '7');
  $form['map_address_administrative_area'] = array('#type' => 'value', '#value' => '8');
  $form['map_address_postal_code'] = array('#type' => 'value', '#value' => '9');
  $form['map_dob'] = array('#type' => 'value', '#value' => '10');
  $form['map_phone'] = array('#type' => 'value', '#value' => '11');
  $form['map_phone_alt'] = array('#type' => 'value', '#value' => '12');
  $form['map_id2'] = array('#type' => 'value', '#value' => '13');
  $form['map_CONTACT_JSON::union_info_fieldset:seniority_date'] = array('#type' => 'value', '#value' => '14');

  $form['intro'] = array('#markup' => '<p>' . t('Using the standard Titan field mapping. Please check the columns below.') . '</p>');

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  $html = '<div style="width: 100%; height: 600px; overflow: scroll; border: 1px solid #333333; padding: 20px; margin-top: 20px;">';
  $html .= "<h2>Preview - First $count Rows</h2>";
  $html .= theme_table(array('rows' => $html_rows, 'header' => $header, 'attributes' => array()));
  $html .= '</div>';
  $form["preview"] = array('#markup' => $html);

  return $form;
}

function sirius_teamsters890_feed_titan_map_form_submit($form, &$form_state) {
  return sirius_feed_wizard_map_form_submit($form, $form_state);
}



/************************************************************************************************
 * Step 4: Verify
 ************************************************************************************************/

function sirius_teamsters890_feed_titan_verify($info, $data, $feed_node, $current_step) { return drupal_get_form('sirius_teamsters890_feed_titan_verify_form', $info, $data, $feed_node, $current_step); }

function sirius_teamsters890_feed_titan_verify_form($form, &$form_state, $info, $data, $feed_node, $current_step) {
  $form_state['info'] = $info;
  $form_state['data'] = $data;
  $form_state['feed_node'] = $feed_node;
  $form_state['current_step'] = $current_step;

  $form['verify_limit'] = array('#type' => 'value', '#value' => 100000);
  $form['precompute'] = array('#type' => 'value', '#value' => 'Yes');

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  if ($data['verify_html']) {
    $form['verify_html'] = array('#markup' => $data['verify_html']);
  }

  return $form;
}


function sirius_teamsters890_feed_titan_verify_form_validate($form, &$form_state) {
  return sirius_feed_wizard_verify_form_validate($form, $form_state);
}

function sirius_teamsters890_feed_titan_verify_form_submit($form, &$form_state) {
  return sirius_feed_wizard_verify_form_submit($form, $form_state);
}