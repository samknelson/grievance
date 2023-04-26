<?php

function sirius_smf_feed_dependents_field_name($fields, $title) {
	$lookup = &drupal_static(__FUNCTION__);
  if (!isset($lookup)) {
  	foreach ($fields as $field_name => $field_record) { $lookup[$field_record['title']] = $field_name; }
  }
  return $lookup[$title];
}

function sirius_smf_feed_dependents_fields($info, $data, $feed_node, $current_step) {
	$fields['A'] = array('title' => 'Row #', 'smf_a' => 'Map: Content Type:Field', 'smf_notes' => 'Notes', 'smf_log' => 'Log?', );
	$fields['B'] = array('title' => '10 BPA-DEP2-CLIENT', 'smf_a' => '', 'smf_notes' => 'Skip', 'smf_log' => 'No', );
	$fields['C'] = array('title' => '10 BPA-DEP2-SSN', 'smf_a' => 'sirius_contact_relationship:field_sirius_contact', 'smf_notes' => 'Member/Subscriber SSN - Use this to create the relationship to set field_sirius_contact in the relationship.', 'smf_log' => 'Yes', );
	$fields['D'] = array('title' => '10 BPA-DEP2-SEQ', 'smf_a' => '', 'smf_notes' => 'Depenent sequence number', 'smf_log' => 'Yes', );
	$fields['E'] = array('title' => '10 BPA-DEP2-CLIENT2', 'smf_a' => '', 'smf_notes' => 'Skip', 'smf_log' => 'No', );
	$fields['F'] = array('title' => '10 BPA-DEP2-SSN2', 'smf_a' => 'sirius_contact_relationship:field_sirius_contact_alt', 'smf_notes' => 'Dependent SSN; use this field to create a worker with this SSN', 'smf_log' => 'Yes', );
	$fields['G'] = array('title' => '10 BPA-DEP2-CLIENT3', 'smf_a' => '', 'smf_notes' => 'Skip', 'smf_log' => 'No', );
	$fields['H'] = array('title' => '15 BPA-DEP2-LNAME', 'smf_a' => 'sirius_worker:field_sirius_name:Family', 'smf_notes' => '', 'smf_log' => 'Yes', );
	$fields['I'] = array('title' => '15 BPA-DEP2-FNAME', 'smf_a' => 'sirius_worker:field_sirius_name:Given', 'smf_notes' => '', 'smf_log' => 'Yes', );
	$fields['J'] = array('title' => '05 BPA-DEP2-MI', 'smf_a' => 'sirius_worker:field_sirius_name:Middle Name(s)', 'smf_notes' => '', 'smf_log' => 'Yes', );
	$fields['K'] = array('title' => '05 BPA-DEP2-ADDR1', 'smf_a' => 'sirius_worker:field_sirius_address:thoroughfare', 'smf_notes' => '', 'smf_log' => 'Yes', );
	$fields['L'] = array('title' => '10 BPA-DEP2-ADDR2', 'smf_a' => 'sirius_worker:field_sirius_address:premise', 'smf_notes' => '', 'smf_log' => 'Yes', );
	$fields['M'] = array('title' => '10 BPA-DEP2-ADDR3', 'smf_a' => 'sirius_worker:field_sirius_address:locality', 'smf_notes' => '', 'smf_log' => 'Yes', );
	$fields['N'] = array('title' => '05 BPA-DEP2-ST', 'smf_a' => 'sirius_worker:field_sirius_address:administrative_area', 'smf_notes' => '', 'smf_log' => 'Yes', );
	$fields['O'] = array('title' => '05 BPA-DEP2-ZIP', 'smf_a' => 'sirius_worker:field_sirius_address:postal_code', 'smf_notes' => '', 'smf_log' => 'Yes', );
	$fields['P'] = array('title' => '05 BPA-DEP2-2NDADDR1', 'smf_a' => 'Sirius Log', 'smf_notes' => 'CREATE LOG ENTRY', 'smf_log' => 'Yes', );
	$fields['Q'] = array('title' => '05 BPA-DEP2-2NDADDR2', 'smf_a' => 'Sirius Log', 'smf_notes' => 'CREATE LOG ENTRY', 'smf_log' => 'Yes', );
	$fields['R'] = array('title' => '05 BPA-DEP2-2NDADDR3', 'smf_a' => 'Sirius Log', 'smf_notes' => 'CREATE LOG ENTRY', 'smf_log' => 'Yes', );
	$fields['S'] = array('title' => '05 BPA-DEP2-2NDST', 'smf_a' => 'Sirius Log', 'smf_notes' => 'CREATE LOG ENTRY', 'smf_log' => 'Yes', );
	$fields['T'] = array('title' => '05 BPA-DEP2-2NDZIP', 'smf_a' => 'Sirius Log', 'smf_notes' => 'CREATE LOG ENTRY', 'smf_log' => 'Yes', );
	$fields['U'] = array('title' => '05 BPA-DEP2-GUARD-LNAME', 'smf_a' => 'Sirius Log', 'smf_notes' => 'CREATE LOG ENTRY', 'smf_log' => 'Yes', );
	$fields['V'] = array('title' => '05 BPA-DEP2-GUARD-FNAME', 'smf_a' => 'Sirius Log', 'smf_notes' => 'CREATE LOG ENTRY', 'smf_log' => 'Yes', );
	$fields['W'] = array('title' => '05 BPA-DEP2-GUARD-MI', 'smf_a' => 'Sirius Log', 'smf_notes' => 'CREATE LOG ENTRY', 'smf_log' => 'Yes', );
	$fields['X'] = array('title' => '05 BPA-DEP2-AREA', 'smf_a' => 'sirius_worker:field_sirius_phone', 'smf_notes' => '05 BPA-DEP2-PHONEconcatenate 05 BPA-DEP2-AREA and ', 'smf_log' => 'Yes', );
	$fields['Y'] = array('title' => '05 BPA-DEP2-PHONE', 'smf_a' => 'sirius_worker:field_sirius_phone', 'smf_notes' => '05 BPA-DEP2-PHONEconcatenate 05 BPA-DEP2-AREA and ', 'smf_log' => 'Yes', );
	$fields['Z'] = array('title' => '05 BPA-DEP2-BD', 'smf_a' => 'sirius_worker:field_sirius_dob', 'smf_notes' => '', 'smf_log' => 'Yes', );
	$fields['AA'] = array('title' => '05 BPA-DEP2-SEX', 'smf_a' => 'sirius_worker:field_sirius_gender', 'smf_notes' => '', 'smf_log' => 'Yes', );
	$fields['AB'] = array('title' => '05 BPA-DEP2-REL', 'smf_a' => 'sirius_contact_relationship:ield_sirius_contact_reltype', 'smf_notes' => 'SP =  Spouse DP = Domestic Partner ES = Ex Spouse C = Child (Parent) SC = Step Child H = Handicap Child AC = Adopted Child', 'smf_log' => 'Yes', );
	$fields['AC'] = array('title' => '05 BPA-DEP2-EFF', 'smf_a' => 'sirius_contact_relationship:field_sirius_date_start OR sirius_contact_relationship:field_sirius_date_end', 'smf_notes' => 'If 05 BPA-DEP2-ENROLL = E, then set START; if BPA-DEP2-ENROLL = null, then set end', 'smf_log' => 'Yes', );
	$fields['AD'] = array('title' => '05 BPA-DEP2-ENROLL', 'smf_a' => 'if = E, then set benefit based on subscriber starting on 05 BPA-DEP2-EFF ; if null, end benefits as of 05 BPA-DEP2-EFF', 'smf_notes' => '', 'smf_log' => 'Yes', );
	$fields['AE'] = array('title' => '05 BPA-DEP2-MARR-STAT', 'smf_a' => 'skip', 'smf_notes' => '', 'smf_log' => 'Yes', );
	$fields['AF'] = array('title' => '05 BPA-DEP2-MARR-DATE', 'smf_a' => 'skip', 'smf_notes' => '', 'smf_log' => 'Yes', );
	$fields['AG'] = array('title' => '05 BPA-DEP2-DIV-STAT', 'smf_a' => 'skip', 'smf_notes' => '', 'smf_log' => 'Yes', );
	$fields['AH'] = array('title' => '05 BPA-DEP2-DIV-DATE', 'smf_a' => 'skip', 'smf_notes' => '', 'smf_log' => 'Yes', );
	$fields['AI'] = array('title' => '05 BPA-DEP2-QMSCO', 'smf_a' => 'if Y, then set C type relationship as QMSCO', 'smf_notes' => '', 'smf_log' => 'Yes', );
	$fields['AJ'] = array('title' => '05 BPA-DEP2-BCERT', 'smf_a' => 'skip', 'smf_notes' => '', 'smf_log' => 'No', );
	$fields['AK'] = array('title' => '05 BPA-DEP2-ADOPT', 'smf_a' => 'skip', 'smf_notes' => '', 'smf_log' => 'No', );
	$fields['AL'] = array('title' => '05 BPA-DEP2-TERM', 'smf_a' => 'if > 0, set sirius_contact_relationship:field_sirius_date_end', 'smf_notes' => '', 'smf_log' => 'Yes', );
	$fields['AM'] = array('title' => '05 BPA-DEP2-COMMENT', 'smf_a' => 'skip', 'smf_notes' => '', 'smf_log' => 'Yes', );
	$fields['AN'] = array('title' => '05 BPA-DEP2-USERID', 'smf_a' => 'skip', 'smf_notes' => '', 'smf_log' => 'Yes', );
	$fields['AO'] = array('title' => '05 BPA-DEP2-REVDATE', 'smf_a' => 'skip', 'smf_notes' => '', 'smf_log' => 'Yes', );
	$fields['AP'] = array('title' => '10 BPA-DEP2-CLAIMS-SUFFIX', 'smf_a' => 'skip', 'smf_notes' => '', 'smf_log' => 'No', );
	$fields['AQ'] = array('title' => '05 BPA-DEP2-CASENO', 'smf_a' => 'skip', 'smf_notes' => '', 'smf_log' => 'Yes', );
	$fields['AR'] = array('title' => '05 BPA-DEP2-BADADDR', 'smf_a' => 'skip', 'smf_notes' => '', 'smf_log' => 'Yes', );
	$fields['AS'] = array('title' => '05 BPA-DEP2-BADADDR2', 'smf_a' => 'skip', 'smf_notes' => '', 'smf_log' => 'No', );
	$fields['AT'] = array('title' => '05 BPA-DEP2-PARTD', 'smf_a' => 'skip', 'smf_notes' => '', 'smf_log' => 'No', );
	$fields['AU'] = array('title' => '05 BPA-DEP2-PARTDREJ', 'smf_a' => 'skip', 'smf_notes' => '', 'smf_log' => 'No', );
	$fields['AV'] = array('title' => '05 BPA-DEP2-PARTDDT', 'smf_a' => 'skip', 'smf_notes' => '', 'smf_log' => 'No', );
	$fields['AW'] = array('title' => '05 BPA-DEP2-REENROLL', 'smf_a' => 'skip', 'smf_notes' => '', 'smf_log' => 'No', );
	$fields['AX'] = array('title' => '05 BPA-DEP2-TERM2', 'smf_a' => 'If 05 BPA-DEP2-TERM2 is newer than 05 BPA-DEP2-EFF or BPA-DEP2-EFF2, set end date of relationship using BPA-DEP2-TERM2; if the order is BPA-DEP2-EFF2 < BPA-DEP2-TERM2 < 05 BPA-DEP2-EFF, then end and create a second relationship', 'smf_notes' => '', 'smf_log' => 'Yes', );
	$fields['AY'] = array('title' => '05 BPA-DEP2-EFF2', 'smf_a' => 'If 05 BPA-DEP2-EFF2 is older than 05 BPA-DEP2-EFF, use BPA-DEP2-EFF2 as relationship start date.', 'smf_notes' => '', 'smf_log' => 'Yes', );
	$fields['AZ'] = array('title' => '05 BPA-DEP2-VERIFY', 'smf_a' => 'skip', 'smf_notes' => '', 'smf_log' => 'No', );
	$fields['BA'] = array('title' => '05 BPA-DEP2-MEDFLAG', 'smf_a' => 'skip', 'smf_notes' => '', 'smf_log' => 'No', );
	$fields['BB'] = array('title' => '05 BPA-DEP2-ACCT-CLI', 'smf_a' => 'skip', 'smf_notes' => '', 'smf_log' => 'No', );
	$fields['BC'] = array('title' => '05 BPA-DEP2-ACCT', 'smf_a' => 'skip', 'smf_notes' => '', 'smf_log' => 'No', );
	$fields['BD'] = array('title' => '05 BPA-TMT-TERM-DD', 'smf_a' => 'skip', 'smf_notes' => '', 'smf_log' => 'No', );
	$fields['BE'] = array('title' => '05 FILLER', 'smf_a' => 'skip', 'smf_notes' => '', 'smf_log' => 'No', );

	return $fields;
}

function sirius_smf_feed_dependents_fields_oneoff() {
  $file = fopen('/var/www/sirius-smf.activistcentral.net/sites/all/modules/_custom/sirius_sitespecific/sirius_smf/dep_fields.csv', 'r');

  $col = 0;
  while ($hr = fgetcsv($file, 10000)) {
    $row = array();
    $row['title'] = $hr[0];
    $row['smf_a'] = $hr[1];
    $row['smf_notes'] = $hr[2];
    $row['smf_log'] = $hr[3];
    $rows[sirius_feed_default_column_title($col)] = $row;
    ++$col;
  }
  fclose($file);

  foreach ($rows as $key => $row) {
    print "\$fields['$key'] = array(";
    foreach ($row as $subkey => $value) {
      $value = preg_replace('/[\'\n\r]/', ' ', $value);
      print "'$subkey' => '$value', ";
    }
    print ");\n";
  }
  // print_r($rows);
}
