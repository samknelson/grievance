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

/**
 * Return a map of avaiable department TIDs based on classification TIDs for a given contract template.
 */

function grievance_classification_department_map($contract_tplt_node) {
  // Universe of department TIDs
  $department_tids = array();
  if (isset($contract_tplt_node->field_grievance_departments['und'])) { 
    foreach ($contract_tplt_node->field_grievance_departments['und'] as $tmp) {
      $department_tids[] = $tmp['tid'];
    }
  }
  if (!$department_tids) { return NULL; }
  $department_terms = taxonomy_term_load_multiple($department_tids);

  // Universe of classification TIDs
  $classification_tids = array();
  if (isset($contract_tplt_node->field_grievance_classifications['und'])) { 
    foreach ($contract_tplt_node->field_grievance_classifications['und'] as $tmp) {
      $classification_tids[] = $tmp['tid'];
    }
  }
  if (!$classification_tids) { return NULL; }
  $classification_terms = taxonomy_term_load_multiple($classification_tids);

  // Create the map
  $map = array();
  foreach ($classification_terms as $classification_tid => $classification_term) {
    if (isset($classification_term->field_grievance_departments['und'])) {
      foreach ($classification_term->field_grievance_departments['und'] as $tmp) {
        $department_tid = $tmp['tid'];
        $map[$classification_term->name][$department_tid] = TRUE;
      }
    }
  }

  return $map;
}