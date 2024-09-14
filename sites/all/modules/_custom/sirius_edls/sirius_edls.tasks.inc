<?php

function sirius_edls_departments($employer_node = NULL) {
  if ($employer_node) {
    $industry_tids = sirius_fieldvals($employer_node, 'field_sirius_industry', 'tid');
  }

  $terms = sirius_taxonomy_load('grievance_department', 'tid', 'full');

  $options = array();
  foreach ($terms as $tid => $term) {
    if ($industry_tids) {
      $term_industry_tids = sirius_fieldvals($term, 'field_sirius_industry', 'tid');
      if (!array_intersect($industry_tids, $term_industry_tids)) { continue; }
    }
    $options[$tid] = $term->name;
  }

  return $options;
}

function sirius_edls_tasks_encode($tasks) {
	if (!$tasks) { return ''; }
	return join("\n", $tasks);
}

function sirius_edls_tasks_decode($tasks_encoded) {
  $lines = explode("\n", $tasks_encoded);
  $tasks = array();
  foreach ($lines as $task) {
    $task = trim($task);
    $task = preg_replace('/[^a-zA-Z0-9_, ]/', '', $task);
    if (!$task) { continue; }
    $tasks[] = $task;
  }

  return array('success' => TRUE, 'tasks' => $tasks);
}

function sirius_edls_tasks_by_department($employer_node) {
	$employer_json = sirius_json_get($employer_node);
  $result = $employer_json['edls']['tasks_by_department'];
  if (!$result) { $result = array(); }
  return $result;
}

function sirius_edls_all_tasks($employer_node) {
  $tasks_by_department = sirius_edls_tasks_by_department($employer_node);
  $all_tasks = array();
  foreach ($tasks_by_department as $department_tid => $tasks) {
    $all_tasks = array_merge($all_tasks, $tasks);
  }
  $all_tasks = array_unique($all_tasks);
  return array_combine($all_tasks, $all_tasks);
}