<?php

function sirius_smf_feed_notes_field_name($fields, $title) {
	$lookup = &drupal_static(__FUNCTION__);
  if (!isset($lookup)) {
  	foreach ($fields as $field_name => $field_record) { $lookup[$field_record['title']] = $field_name; }
  }
  return $lookup[$title];
}

function sirius_smf_feed_notes_fields($info, $data, $feed_node, $current_step) {
	$fields['A'] = array('title' => 'Row #', );
	$fields['B'] = array('title' => 'NOTE-KEY-ID', );
	$fields['C'] = array('title' => 'NOTE-KEY-FUND', );
	$fields['D'] = array('title' => 'NOTE-KEY-REC-FOR', );
	$fields['E'] = array('title' => 'NOTE-KEY-INFO', );
	$fields['F'] = array('title' => 'NOTE-KEY-WHEN-RECIP', );
	$fields['G'] = array('title' => 'NOTE-TIME-HH', );
	$fields['H'] = array('title' => 'NOTE-TIME-MM', );
	$fields['I'] = array('title' => 'NOTE-TIME-SS', );
	$fields['J'] = array('title' => 'FILLER', );
	$fields['K'] = array('title' => 'NOTE-WHEN-YY', );
	$fields['L'] = array('title' => 'NOTE-WHEN-MM', );
	$fields['M'] = array('title' => 'NOTE-WHEN-DD', );
	$fields['N'] = array('title' => 'NOTE-WHO', );
	$fields['O'] = array('title' => 'NOTE-KEY-TYPE', );
	$fields['P'] = array('title' => 'NOTE-LINE1', );
	$fields['Q'] = array('title' => 'NOTE-LINE2', );
	$fields['R'] = array('title' => 'NOTE-LINE3', );
	$fields['S'] = array('title' => 'NOTE-LINE4', );
	$fields['T'] = array('title' => 'NOTE-LINE5', );
	$fields['U'] = array('title' => 'NOTE-LINE6', );
	$fields['V'] = array('title' => 'NOTE-CREATED-BY', );
	$fields['W'] = array('title' => 'NOTE-ACCT-NO', );
	$fields['X'] = array('title' => 'FILLER', );
	return $fields;
}

function sirius_smf_feed_notes_fields_oneoff() {
  $file = fopen('/var/www/sirius-smf.activistcentral.net/sites/default/files/private/vsam/NOTES_Y20220825.csv', 'r');

  $hr = fgetcsv($file, 10000);
  fclose($file);

  $col = 0;
  foreach ($hr as $col => $title) {
    $key = sirius_feed_default_column_title($col);
    print "\$fields['$key'] = array(";
    $title = preg_replace('/[\'\n\r]/', ' ', $title);
    print "'title' => '$title', ";
    print ");\n";
  }
  // print_r($rows);
}
