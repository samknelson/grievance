<?php

/************************************************************************************************
 * Step 2: Mapping
 ************************************************************************************************/

function sirius_smf_feed_members_map($info, $data, $feed_node, $current_step) { return drupal_get_form('sirius_smf_feed_members_map_form', $info, $data, $feed_node, $current_step); }

function sirius_smf_feed_members_map_form($form, &$form_state, $info, $data, $feed_node, $current_step) {
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
  foreach ($header as $idx => $field_name) {
  	if (!$fields[$field_name]) {
  		drupal_set_message("Warning: an unknown field, $field_name, was found in the header for the uploaded file. This field will be ignored.", 'warning');
  		continue; 
  	}
  	$form["map_$field_name"] = array('#type' => 'value', '#value' => $idx); 
  }

  $form['intro'] = array('#markup' => '<p>' . t('This wizard assumes a fixed set of column names. Please verify the data below.') . '</p>');

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

function sirius_smf_feed_members_map_form_submit($form, &$form_state) {
  return sirius_feed_wizard_map_form_submit($form, $form_state);
}

