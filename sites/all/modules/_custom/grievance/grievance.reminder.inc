<?php

function grievance_reminders() {
	// Get all the open statuses
  $status_vocabulary = taxonomy_vocabulary_machine_name_load('grievance_status');
  $status_terms = taxonomy_get_tree($status_vocabulary->vid);
  dpm($status_terms);

	// Get all my open grievances
	/*
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_log');
  $query->fieldCondition('field_grievance_log_type', 'tid', variable_get('grievance_log_notify_type', ''));
  $query->fieldCondition('field_grievance', 'target_id', $grievance_node->nid);
  $stmt = $query->execute();
	*/

	return "Hello, world.";
}