<?php

// @TODO: This is an incredible kludge and involved hacking the term_reference_filter_by_views_autocomplete module.
function grievance_term_reference_filter_by_views_autocomplete_alter(&$entity, &$context) {
  if (!isset($entity['#field_name'])) { return; }
  if ($entity['#field_name'] != 'field_grievance_class_tid') { return; }

  $contract_tid = NULL;
  if (isset($entity['#entity']->field_grievance_contract_tplt['und'][0]['target_id'])) {
    $contract_tid = $entity['#entity']->field_grievance_contract_tplt['und'][0]['target_id'];
  }
  $entity['#autocomplete_path'] = "grievance/autocomplete/grievance-job-class-tid/$contract_tid/";
}

function grievance_autocompete_job_class_tid($contract_tid, $tid = '', $match = '') {
  // Get the vocabulary
  $vocabulary = taxonomy_vocabulary_machine_name_load('grievance_job_classification');
  if (!$vocabulary) {
    drupal_json_output(array());
    exit();
  }

  // Get the contract
  $contract_node = NULL;
  if ($contract_tid > 0) {
    $contract_node = node_load($contract_tid);
  }

  // Get the available classifications
  $possible_classifications = array();
  if ($contract_node && isset($contract_node->field_grievance_classifications['und'][0])) {
    foreach ($contract_node->field_grievance_classifications['und'] as $tmp) {
      $possible_classifications[] = $tmp['tid'];
    }
  }

  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'taxonomy_term');
  $query->propertyCondition('vid', $vocabulary->vid);
  $query->propertyOrderBy('weight');
  $query->propertyCondition('name', $match, 'CONTAINS');
  if ($possible_classifications) {
    $query->propertyCondition('tid', $possible_classifications, 'IN');
  }
  $query->range(0, 10);
  $stmt = $query->execute();

  if (!$stmt['taxonomy_term']) { 
    drupal_json_output(array()); 
    exit();
  }

  $terms = taxonomy_term_load_multiple(array_keys($stmt['taxonomy_term']));
  $matches = array();
  foreach ($terms as $tid => $term) {
    // $key = $term->name . '(id:' . $term->tid . ')';
    $key = $term->name;
    if (strpos($key, ',') !== FALSE || strpos($key, '"') !== FALSE) {
      $key = '"' . str_replace('"', '""', $key) . '"';
    }
    $matches[$key] = $key;
  }
  drupal_json_output($matches);
}
