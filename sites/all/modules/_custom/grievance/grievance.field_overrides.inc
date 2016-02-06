<?php

function grievance_field_overrides_apply() {
	$dest = 'admin/config/grievances/fieldoverrides';

	// Get all overrides
	$query = new EntityFieldQuery();
	$query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_field_overrides');
	$stmt = $query->execute();
	if (!$stmt['node']) {
		drupal_set_message(t('There are no overrides to apploy.'));
		drupal_goto($dest);
	}

	$nids = array_keys($stmt['node']);
	$nodes = node_load_multiple($nids);

	foreach ($nodes as $nid => $node) {
		$entity_type = $node->field_grievance_entity_type['und'][0]['value'];
		$bundle = $node->field_grievance_bundle['und'][0]['value'];
		$field_name = $node->field_grievance_field_name['und'][0]['value'];
		$label = $node->field_grievance_label['und'][0]['value'];
		$description = $node->field_grievance_description['und'][0]['value'];
		$default = $node->field_grievance_default['und'][0]['value'];

	  $instance_info = field_info_instance($entity_type, $field_name, $bundle);
	  if (!$instance_info) {
	  	drupal_set_message(
	  		t('Unable to find field instance for @entity_type / @bundle / @field_name.', 
	  			array(
	  				'@entity_type' => $entity_type,
	  				'@bundle' => $bundle,
	  				'@field_name' => $field_name
	  			)
	  		), 
	  		'error'
	  	);
	  	continue;
	  }

	  if ($label) {
	  	$instance_info['label'] = $label;

  		drupal_set_message(
  			t('Set field label for @entity_type / @bundle / @field_name to @label', 
	  			array(
	  				'@entity_type' => $entity_type,
	  				'@bundle' => $bundle,
	  				'@field_name' => $field_name,
	  				'@label' => $label
	  			)
		  	)
		  );
	  }

	  if ($description) {
	  	$instance_info['description'] = $description;

  		drupal_set_message(
  			t('Set field description for @entity_type / @bundle / @field_name to @description', 
	  			array(
	  				'@entity_type' => $entity_type,
	  				'@bundle' => $bundle,
	  				'@field_name' => $field_name,
	  				'@description' => $description
	  			)
		  	)
		  );
	  }

	  if ($default) {
	  	dpm($instance_info);
	  	$instance_info['default_value'] = array('value' => $default);

  		drupal_set_message(
  			t('Set default for @entity_type / @bundle / @field_name to @default', 
	  			array(
	  				'@entity_type' => $entity_type,
	  				'@bundle' => $bundle,
	  				'@field_name' => $field_name,
	  				'@default' => $default
	  			)
		  	)
		  );
	  }

	  field_update_instance($instance_info);		
	}

	drupal_set_message("Overrides applied.");
	drupal_goto($dest);
}