<?php

function sirius_worker_department_options($worker_node) {
	$industry_tids = array();
	if ($worker_node->field_sirius_industry['und'][0]) {
		foreach ($worker_node->field_sirius_industry['und'] as $tmp) {
			$industry_tids[] = $tmp['tid'];
		}
	}

	return sirius_worker_department_by_industry($industry_tids);
}

function sirius_worker_department_by_industry($industry_tids) {
  if (!$industry_tids) { return array(); }
  $vocabulary = taxonomy_vocabulary_machine_name_load('grievance_department');
  if (!$vocabulary) { return array(); }
	$query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'taxonomy_term');
  $query->propertyCondition('vid', $vocabulary->vid);
  $query->fieldCondition('field_sirius_industry', 'tid', $industry_tids, 'in');
  $query->fieldCondition('field_sirius_dispatch_available', 'value', 'Yes');
  $stmt = $query->execute();
  if (!$stmt['taxonomy_term']) { return array(); }
  $department_tids = array_keys($stmt['taxonomy_term']);

  return taxonomy_term_load_multiple($department_tids);
}
