<?php

/**
 * Implements hook_node_delete()
 */

function grievance_node_delete($node) {

	$source_nid = $node->nid;

	$cascades = array(
		'grievance' => array(
			'field_grievance' => array(
				'grievance_log', 'grievance_attachment', 'grievance_settlement'
			),
		),
		'grievance_contract_template' => array(
			'field_grievance_contract_tplt' => array(
				'grievance_contract_clause', 'grievance_contract_rule', 'grievance_contract_section'
			),
		),
	);

	foreach ($cascades as $bundle => $fields) {
		if ($node->type == $bundle) {
			foreach ($fields as $field_name => $target_bundles) {

				$query = new EntityFieldQuery();
			  $query->entityCondition('entity_type', 'node');
			  $query->fieldCondition($field_name, 'target_id', $source_nid);
			  $query->entityCondition('bundle', $target_bundles, 'IN');
			  $stmt = $query->execute();
			  $target_nids = array();
			  if ($stmt['node']) {
			    $target_nids = array_keys($stmt['node']);
			    node_delete_multiple($target_nids);
			  }

			}
		}
	}
}