<?php

function grievaince_id_algorithms() {
	$algorithms = array();

	$algorithms['grievance_id_algorithm_nid'] = array(
		'name' => t('Node ID'),
		'description' => t('Grievance IDs are generated using the internal system node ID.  This is typically a 4, 5, or 6-digit number.  Examples: 2356; 12994'),
	);

	$algorithms['grievance_id_algorithm_nidmd5'] = array(
		'name' => t('Node ID MD5'),
		'description' => t('Grievance IDs are generated using an MD5 hash of the internal system node ID.  This obfuscates the ID number, meaning that the sequence cannot be deduced.  Example: 0aae0fede9a4d278e2f9a171e62fc76b'),
	);

	$algorithms['grievance_id_algorithm_year4'] = array(
		'name' => t('Sequential by year'),
		'description' => t('Grievance IDs are generated using the year as a prefix, followed by a four-digit sequential number.  Examples: 20160001; 20150963.'),
	);

	return $algorithms;
}

function grievance_id_generate($node) {
	$id = NULL;
  $algorithm = variable_get('grievance_id_algorithm', '');

  // We could be calling $algorithm() here.  But for security purposes we're reluctant to use eval()'d code unless
  // we really need to.  So at the moment, we're just hardcoding the functions.
  if ($algorithm == 'grievance_id_algorithm_nid')     { $id = grievance_id_algorithm_nid($node); }
  if ($algorithm == 'grievance_id_algorithm_nidmd5')  { $id = grievance_id_algorithm_nidmd5($node); }
  if ($algorithm == 'grievance_id_algorithm_year4')   { $id = grievance_id_algorithm_year4($node); }

  if ($id) { $id .= variable_get('grievance_id_suffix', ''); }

  return $id;
}

function grievance_id_algorithm_nid($node) { 
	return $node->nid;
}

function grievance_id_algorithm_nidmd5($node) {
	return md5($node->nid);
}

function grievance_id_algorithm_year4($node) {
	$suffix = variable_get('grievance_id_suffix', '');

	print "Here with $suffix\n";
	
	$year = date('Y');

	$values = array();
	$sql = "select max(field_grievance_id_value) as m ";
	$sql .= "from field_data_field_grievance_id ";
	$sql .= "where entity_type = 'node' and bundle = 'grievance' and field_grievance_id_value like '$year%' ";
	/*
	if ($suffix) {
		$sql .= "and field_grievance_id_value like :suffix ";
		$values[':suffix'] = '%' . $suffix;
	} else {
		$sql .= "and field_grievance_id_value like '[0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]";
	}

	print "$sql";
	*/
	$stmt = db_query($sql, $values);
	$hr = $stmt->fetchAssoc();
	$last_id = $hr['m'];
	if ($suffix) {
		$last_id = preg_replace("/$suffix\$/", '', $last_id);
	}
	$number = substr($last_id, 4);
	$number++;
	while (strlen($number) < 4) { $number = "0$number"; }
	$id = "$year$number";

	return $id;
}

function grievance_fetch_by_id($id) {
	$query = new EntityFieldQuery();
	$query->entityCondition('entity_type', 'node');
	$query->entityCondition('bundle', 'grievance');
	$query->propertyCondition('status', 1);
	$query->fieldCondition('field_grievance_id', 'value', $id);
	$result = $query->execute();
	if (!isset($result['node'])) { return NULL; }
	$nid = array_keys($result['node'])[0];
	return node_load($nid);
}