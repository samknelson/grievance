<?php

function sirius_smf_feed_contributions_field_name($fields, $title) {
	$lookup = &drupal_static(__FUNCTION__);
  if (!isset($lookup)) {
  	foreach ($fields as $field_name => $field_record) { $lookup[$field_record['title']] = $field_name; }
  }
  return $lookup[$title];
}

function sirius_smf_feed_contributions_fields($info, $data, $feed_node, $current_step) {
	$fields['A'] = array('title' => 'Row #', );
	$fields['B'] = array('title' => '10  SMCCON-KEY1-CON', );
	$fields['C'] = array('title' => '10  SMCCON-KEY1-SSN', );
	$fields['D'] = array('title' => '10  SMCCON-KEY1-DATE', );
	$fields['E'] = array('title' => '10  SMCCON-KEY1-SEQ', );
	$fields['F'] = array('title' => '10  SMCCON-KEY2-CON', );
	$fields['G'] = array('title' => '10  SMCCON-KEY2-EMPR', );
	$fields['H'] = array('title' => '10  SMCCON-KEY2-DATE', );
	$fields['I'] = array('title' => '10  SMCCON-KEY2-SEQ', );
	$fields['J'] = array('title' => '05 SMCCON-LNAME', );
	$fields['K'] = array('title' => '05 SMCCON-FNAME', );
	$fields['L'] = array('title' => '05 SMCCON-MI', );
	$fields['M'] = array('title' => '05 SMCCON-HRS', );
	$fields['N'] = array('title' => '05 SMCCON-CD', );
	$fields['O'] = array('title' => '05 SMCCON-ELIGHRS', );
	$fields['P'] = array('title' => '05 SMCCON-WELFARE', );
	$fields['Q'] = array('title' => '05 SMCCON-WAPPLIED', );
	$fields['R'] = array('title' => '05 SMCCON-PENSION', );
	$fields['S'] = array('title' => '05 SMCCON-PAPPLIED', );
	$fields['T'] = array('title' => '05 SMCCON-REMSP', );
	$fields['U'] = array('title' => '05 SMCCON-DCRATE', );
	$fields['V'] = array('title' => '05 SMCCON-NOWAIT', );
	$fields['W'] = array('title' => '05 SMCCON-PROCDATE', );
	$fields['X'] = array('title' => '05 SMCCON-CKNO', );
	$fields['Y'] = array('title' => '05 SMCCON-COPAY', );
	$fields['Z'] = array('title' => '05 SMCCON-ERRCOD', );

	return $fields;
}

function sirius_smf_feed_contributions_fields_oneoff() {
  $file = fopen('/var/www/sirius-smf.activistcentral.net/sites/default/files/private/vsam/CONTRIBUTIONS_Y20220824.csv', 'r');

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
