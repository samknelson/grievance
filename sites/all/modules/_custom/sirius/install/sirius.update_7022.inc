<?php

function sirius_update_7022_impl() {
	sirius_minilog_echo_active(TRUE);
	sirius_minilog("==== Migrating grievances gender field. ====");

	////////////////////////////////////////////////////
	// If there are no values in the legacy field, we don't have to do anything
	////////////////////////////////////////////////////

	$sql = "select count(*) as c from field_data_field_grievance_gender";
	$stmt = db_query($sql, array());
	$hr = $stmt->fetchAssoc();
	$count = $hr['c'];
	sirius_minilog("Found $count legacy gender records.");
	if ($count < 1) {
		sirius_minilog("... Since there aren't any legacy gender records, there's nothing left to do.");
		return;
	}

	////////////////////////////////////////////////////
	// set a default taxonomy
	////////////////////////////////////////////////////

	$vocabulary = taxonomy_vocabulary_machine_name_load('sirius_gender');
	if (!$vocabulary) { die("Vocabulary sirius_gender does not exist."); }
	$existing_terms = taxonomy_get_tree($vocabulary->vid);
	if ($existing_terms) {
		sirius_minilog("The sirius_gender taxonomy is already populated. We don't have to initialize it.");
	} else  {
		sirius_minilog("The gender taxonomy doesn't exist, so we'll initialize it with default terms.");
		$data = array(
			array(
				'name' => 'Female',
				'field_sirius_name_short' => 'F',
				'field_sirius_id' => 'Female',
				'field_sirius_id2' => 'F',
				'field_sirius_nota' => 'No',
			),
			array(
				'name' => 'Male',
				'field_sirius_name_short' => 'M',
				'field_sirius_id' => 'Male',
				'field_sirius_id2' => 'M',
				'field_sirius_nota' => 'No',
			),
			array(
				'name' => 'Non-Binary/Gender Non-Conforming',
				'field_sirius_name_short' => 'NB/GNC',
				'field_sirius_id' => 'Non-Binary/Gender Non-Conforming',
				'field_sirius_id2' => '',
				'field_sirius_nota' => 'No',
			),
			array(
				'name' => 'None',
				'field_sirius_name_short' => 'N',
				'field_sirius_id' => 'None',
				'field_sirius_id2' => 'U',
				'field_sirius_nota' => 'No',
			),
			array(
				'name' => 'Prefer not to say',
				'field_sirius_name_short' => 'PNTS',
				'field_sirius_id' => 'Prefer not to say',
				'field_sirius_id2' => '',
				'field_sirius_nota' => 'No',
			),
			array(
				'name' => 'Self-Describe',
				'field_sirius_name_short' => 'SD',
				'field_sirius_id' => 'Self-Describe',
				'field_sirius_id2' => '',
				'field_sirius_nota' => 'Yes',
			),
		);

		foreach ($data as $record) {
		  $term = new stdClass();
  		$term->name = $record['name'];
  		foreach ($record as $field => $value) {
  			if ($field == 'name') { continue; }
  			$term->{$field} = array();
  			$term->{$field}['und'][0]['value'] = $value;
  		}
	  	$term->vid = $vocabulary->vid;
	  	taxonomy_term_save($term);
	  	sirius_minilog("... Created: " . $term->name . " (" . $term->tid . ")");
	  }
  }

	////////////////////////////////////////////////////
	// Find values for "Male", "Female", and "Unknown"
	////////////////////////////////////////////////////

  $tid_map['M'] = sirius_taxonomy_lookup_by_anything('sirius_gender', 'male');
  if (!$tid_map['M']) { $tid_map['M'] = sirius_taxonomy_lookup_by_anything('sirius_gender', 'M'); }
  if (!$tid_map['M']) { die("Unable to find a taxonomy term for Male."); }

	$tid_map['F'] = sirius_taxonomy_lookup_by_anything('sirius_gender', 'female');
	if (!$tid_map['F']) { $tid_map['F'] = sirius_taxonomy_lookup_by_anything('sirius_gender', 'F'); }
  if (!$tid_map['F']) { die("Unable to find a taxonomy term for Female."); }

	$tid_map['U'] = sirius_taxonomy_lookup_by_anything('sirius_gender', 'unknown');
	if (!$tid_map['U']) { $tid_map['U'] = sirius_taxonomy_lookup_by_anything('sirius_gender', 'U'); }
  if (!$tid_map['U']) { die("Unable to find a taxonomy term for Unknown."); }

  sirius_minilog("Using TID map: " . print_r($tid_map, 1));

	////////////////////////////////////////////////////
	// Update our legacy data
	////////////////////////////////////////////////////

	$sql = "select field_data_field_grievance_gender.* from node ";
	$sql .= "left join field_data_field_sirius_gender on field_data_field_sirius_gender.entity_type = 'node' and field_data_field_sirius_gender.entity_id = node.nid ";
	$sql .= "left join field_data_field_grievance_gender on field_data_field_grievance_gender.entity_type = 'node' and field_data_field_grievance_gender.entity_id = node.nid ";
	$sql .= "where field_grievance_gender_value is not null ";
	$sql .= "and field_sirius_gender_tid is null ";
	$sql .= "order by nid ";
	$stmt = db_query($sql, array());

	$records_to_fix = array();
	while ($hr = $stmt->fetchAssoc()) {
		$records_to_fix[] = $hr;
	}

	print_r($records_to_fix);
}
