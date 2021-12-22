<?php

function unite_oneoff_find_unserialize_error() {
	$sql = "select name, value from variable";
	$stmt = db_query($sql, array());
	while ($hr = $stmt->fetchAssoc()) {
		print "$hr[name]: ";
		$result = unserialize($hr['value']);
		if ($result === FALSE) {
			print "Bad\n";
			exit();
			variable_set($hr['name'], '');
		} else {
			print "Ok\n";
		}
	}
}


function unite_oneoff_migrate_field_collections_property() {
	$property_nids = array();
	$sql = "select nid, title from node where type = 'property'";
	$stmt = sirius_sql_query($sql, array());
	while ($hr = $stmt->fetchAssoc()) {
		$property_nids[$hr['nid']] = $hr['title'];
	}

	$limit = 200000000;
	foreach ($property_nids as $property_nid => $property_title) {
		++$count;
		if ($count > $limit) { break; }
		print "Processing: $property_title ($property_nid) ...\n";

		$property_node = node_load($property_nid);
		$field_collection_ids = sirius_fieldvals($property_node, 'field_relationships');

		foreach ($field_collection_ids as $field_collection_id) {
			print "... Field collection #$field_collection_id\n";

			$sql = "select entity_id from field_data_field_legacy_id where entity_type = 'node' and bundle = 'propertY_actor' and field_legacy_id_value = :field_collection_id ";
			$stmt = sirius_sql_query($sql, array(':field_collection_id' => $field_collection_id));
			$hr = $stmt->fetchAssoc();
			if ($hr) {
				print "... Found existing node $hr[entity_id]\n";
				continue;
			}


			$pa_node = field_collection_item_load($field_collection_id);

			$actor_title = sirius_node_title($pa_node->field_actor['und'][0]['target_id']);

			$pa_node->type = 'property_actor';
			$pa_node->title = substr($property_title . ' :: ' . $actor_title, 0, 254);
		  $pa_node->language = LANGUAGE_NONE;
		  $pa_node->uid = $user->uid;
		  $pa_node->status = 1;
		  $pa_node->promote = 0;
		  $pa_node->comment = 0;
		  $pa_node->field_property['und'][0]['target_id'] = $property_nid;
		  $pa_node->field_legacy_id['und'][0]['value'] = $field_collection_id;
		  $pa_node = node_submit($pa_node);
		  node_object_prepare($pa_node);
		  node_save($pa_node);
		  print "... Saved: " . $pa_node->title . ' (' . $pa_node->nid . ")\n";
		}
	}
}

function unite_oneoff_migrate_field_collections_actor() {

	$actor_nids = array();
	$sql = "select nid, title from node where type = 'actor'";
	$stmt = sirius_sql_query($sql, array());
	while ($hr = $stmt->fetchAssoc()) {
		$actor_nids[$hr['nid']] = $hr['title'];
	}

	$limit = 20000000;
	foreach ($actor_nids as $actor_nid => $actor_title) {
		$actor_node = node_load($actor_nid);
		$field_collection_ids = sirius_fieldvals($actor_node, 'field_relationships');
		if (!$field_collection_ids) { continue; }

		++$count;
		if ($count > $limit) { break; }

		$existing_parent_actor_nids = sirius_fieldvals($actor_node, 'field_actors', 'target_id');

		print "Processing: $actor_title ($actor_nid) ...\n";
		if (count($field_collection_ids) > 1) { print "... Multiple owners\n"; }
		foreach ($field_collection_ids as $field_collection_id) {
			$field_collection = field_collection_item_load($field_collection_id);
			print "... Field collection #$field_collection_id (" . $field_collection->field_relationship_type['und'][0]['value'] . ")\n";
			$parent_actor_nid = $field_collection->field_actor['und'][0]['target_id'];
			if (!$parent_actor_nid) { continue; }
			if (in_array($parent_actor_nid, $existing_parent_actor_nids)) {
				print "... Parent $parent_actor_nid already found. Skipping.\n";
				continue;
			}

			$actor_node->field_actors['und'][] = array('target_id' => $parent_actor_nid);
			node_save($actor_node);
			print "... Added parent $parent_actor_nid.\n";
		}
	}


}
