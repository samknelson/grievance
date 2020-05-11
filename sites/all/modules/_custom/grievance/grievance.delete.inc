<?php

/**
 * Implements hook_node_delete()
 */

function grievance_node_delete($node) {
	$source_nid = $node->nid;
	$cascades = grievance_node_delete_cascades();

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

function grievance_node_delete_cascades() {
	return array(
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
		'grievance_contract_section' => array(
			'field_grievance_contract_section' => array(
				'grievance_contract_clause'
			),
		),
		'sirius_contact' => array(
			'field_sirius_contact' => array(
				'sirius_event_participant'
			),
		),
	);
}

function grievance_delete_form_alter_node_delete_confirm(&$form, &$form_state) { return grievance_delete_confirm($form, $form_state); }
function sirius_contact_delete_form_alter_node_delete_confirm(&$form, &$form_state) { return grievance_delete_confirm($form, $form_state); }

function grievance_delete_confirm($form, $form_state) {
	$source_node = $form['#node'];
	if (!$source_node) { return; }
	$source_nid = $source_node->nid;
	$cascades = grievance_node_delete_cascades();
	$cascade = $cascades[$source_node->type];
	if (!$cascade) {
		return;
	}

	foreach ($cascade as $field_name => $target_bundles) {
		$query = new EntityFieldQuery();
	  $query->entityCondition('entity_type', 'node');
	  $query->fieldCondition($field_name, 'target_id', $source_nid);
	  $query->entityCondition('bundle', $target_bundles, 'IN');
	  $stmt = $query->execute();
	  $target_nids = array();
	  if ($stmt['node']) {
	  	$count = count(array_keys($stmt['node']));

	  	$type_name = '';
	  	foreach ($target_bundles as $target_bundle) {
	  		if ($type_name) { $type_name .= ' or '; }
		  	$type_name .= node_type_get_name($target_bundle);
	  	}
			drupal_set_message(t('Warning: <strong>@count</strong> record(s) of type <strong>@type_name</strong> reference this node; they will also be deleted.', array('@count' => $count, '@type_name' => $type_name)), 'warning');
	  }
	}
}
