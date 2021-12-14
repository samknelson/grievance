<?php

function unite_actor_acquire() {
	return drupal_get_form('unite_actor_acquire_form', $node);
}

function unite_actor_acquire_form($form, $form_state) {

	$form['acquire_source'] = array(
		'#type' => 'textfield', 
		'#title' => t("Acquiring Actor"),
    '#autocomplete_path' => 'unite/ajax/quicksearch',
		'#description' => t('This is the actor who will RECEIVE the relationships.'),
		'#required' => 1,
	);

	$form['acquire_target'] = array(
		'#type' => 'textfield', 
		'#title' => t("Acquired Actor"),
    '#autocomplete_path' => 'unite/ajax/quicksearch',
		'#description' => t('This is the actor who will LOSE the relationships.'),
		'#required' => 1,
	);
	
	$form['date'] = array(
		'#type' => 'textfield', 
		'#title' => t("Date of Sale"),
		'#description' => t('This date will be used as the end date for all "Previous Owner / Operator" relationships, and the start date for the new relationships.'),
		'#required' => 1,
	);

	$form['data_souce'] = array(
		'#type' => 'textfield', 
		'#title' => t("Source"),
		'#description' => t('Source information, which will be placed into the notes field for both the new and old relationships.'),
		'#required' => 1,
	);

	// All operations acquired from [] on 2014-1-1, for $x million total, ref. [Elliott], src: http://www.whatever.com

	$form['confirm'] = array(
		'#type' => 'checkbox', 
		'#title' => t("Yes, I'm really sure!"),
		'#required' => 1,
	);

	$form['submit'] = array(
		'#type' => 'submit', 
		'#value' => t('Acquire Now')
	);
	
	return $form;
}

function unite_actor_acquire_form_get_actors($form, &$form_state) {
	$acquire_source = $form_state['values']['acquire_source'];
	if (!$acquire_source) { 
		form_set_error('acquire_source', t('Select an ACQUIRING actor.'));
		return NULL;
	}
	$acquire_target = $form_state['values']['acquire_target'];
	if (!$acquire_target) { 
		form_set_error('acquire_target', t('Select an ACQUIRED actor.'));
		return NULL;
	}
	
	$matches = array();
	if (preg_match('/\((\d+)\)/', $acquire_source, $matches) && $matches[1]) { 
		$acquire_source_nid = $matches[1];
	}
	if (!$acquire_source_nid) {
		form_set_error('acquire_source', t('Select an ACQUIRING actor.'));
		return NULL;
	}

	$matches = array();
	if (preg_match('/\((\d+)\)/', $acquire_target, $matches) && $matches[1]) { 
		$acquire_target_nid = $matches[1];
	}
	if (!$acquire_target_nid) {
		form_set_error('acquire_target', t('Select an ACQUIRED actor.'));
		return NULL;
	}
	
	if ($acquire_source_nid == $acquire_target_nid) {
		form_set_error('acquire_target', t('The two actors cannot be the same!'));
		return NULL;
	}

	$acquire_source_node = node_load($acquire_source_nid);
	if (!$acquire_source_node || $acquire_source_node->type != 'actor') {
		form_set_error('acquire_source', t('ACQUIRING actor must be an actor, not a property.'));
		return NULL;
	}

	$acquire_target_node = node_load($acquire_target_nid);
	if (!$acquire_target_node || $acquire_target_node->type != 'actor') {
		form_set_error('acquire_target', t('ACQUIRED actor must be an actor, not a property.'));
		return NULL;
	}
	
	return array($acquire_source_node, $acquire_target_node);
}

function unite_actor_acquire_form_validate($form, &$form_state) {
	$date = $form_state['values']['date'];
	if (strtotime($date) < 1) {
		form_set_error('date', t('Please enter a valid date.'));
	}
	
	unite_actor_acquire_form_get_actors($form, $form_state);
}

function unite_actor_acquire_form_submit($form, &$form_state) {
	list($acquire_source_node, $acquire_target_node) = unite_actor_acquire_form_get_actors($form, $form_state);
	
	$date = $form_state['values']['date'];
	$data_source = $form_state['values']['data_souce'];

	// This is the list of Field Collection Items which points to the target actor.
	$query = new EntityFieldQuery();
	$query->entityCondition('entity_type', 'field_collection_item');
	$query->fieldCondition('field_actor', 'target_id', $acquire_target_node->nid);
	$query->fieldCondition('field_relationship_type', 'value', array('Owner', 'Operator'), 'in');
	$result = $query->execute();
	if ($result['field_collection_item']) {
		$field_collection_ids = array_keys($result['field_collection_item']);	
	} else {
		$field_collection_ids = array();
	}
	
	// This is the list of nodes which contains one of the field collection items.
	$related_nids = array();
	foreach ($field_collection_ids as $field_collection_id) {
		$query = new EntityFieldQuery();
		$query->entityCondition('entity_type', 'node');
		$query->fieldCondition('field_relationships', 'value', $field_collection_id);
		$result = $query->execute();
		if (!$result['node']) { continue; }
		$related_nids = array_merge($related_nids, array_keys($result['node']));
	}
	if (!$related_nids) { $related_nids = array(); }
	$related_nids = array_unique($related_nids);
	
	// For each, we're going to tinker with the relationships and then save.
	$relationship_count = 0;
	
	foreach (array('Owner', 'Operator') as $relationship_type) {
		foreach ($related_nids as $related_nid) {
			cache_clear_all();
			$raw_node = node_load($related_nid);
			drupal_set_message("Examining $relationship_type for " . $raw_node->title);
			$node = entity_metadata_wrapper('node', $raw_node);
			$relationships = $node->field_relationships->value();
			
			// For each of the node's relationships
			for ($i=0; $i<count($relationships); ++$i) {
				$raw_collection = $relationships[$i];
				$collection = entity_metadata_wrapper('field_collection_item', $raw_collection);
				
				// We only want Owners and Operators of the target
				if ($collection->field_actor->value()->nid != $acquire_target_node->nid) { continue; }
				if ($collection->field_relationship_type->value() != $relationship_type) { continue; } 
	
				// We're interested in these fields
				$fields = array(
					'field_relationship_type',
					'field_actor',
					'field_percent_owned',
					'field_amount_owned',
					'field_pension',
					'field_start_date',
					'field_end_date',
					'field_owned_raw',
					'field_sales_price',
					'field_notes'
				);
				
				// Create a new collection
				$new_collection_raw = entity_create('field_collection_item', array('field_name' => 'field_relationships'));
				$new_collection_raw->setHostEntity('node', $raw_node);
				$new_collection = entity_metadata_wrapper('field_collection_item', $new_collection_raw);
				
				// Copy the fields
				foreach ($fields as $field) {
					$new_collection->{$field} = $collection->{$field}->value();
				}
				
				// Set the new actor
				$new_collection->field_actor = $acquire_source_node->nid;
				
				// Add a "notes" indicating that it was sold
				$collection->field_notes = $collection->field_notes->value() . "\nSold on $date: $data_source";
				$new_collection->field_notes = $collection->field_notes->value() . "\nAcquired on $date: $data_source";
				
				// Change the relationship type to "Former"
				$collection->field_relationship_type = "Previous " . $collection->field_relationship_type->value();
				
				// Set the end date
				$collection->field_end_date = strtotime($date);
				$new_collection->field_start_date = strtotime($date);
				
				// Save both collections
				$collection->save();	
				$new_collection->save();
				
				// Add the new collection
				$node_raw->field_relationships['und'][] = $new_collection_raw;
				
				++$relationship_count;
				$relationships[] = $new_collection_raw;
			}
			
			$node->save();
		}
	}
	
	// All done.
	$form_state['rebuild'] = 1;
	drupal_set_message("Sale complete! $relationship_count relationships changed.");
}
