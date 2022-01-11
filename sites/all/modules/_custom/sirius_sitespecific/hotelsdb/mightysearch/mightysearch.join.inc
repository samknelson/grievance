<?php

function mightysearch_join(&$query, &$already_joined, $schema, $table, $field) {
	// 
	// Joins for Property
	//
	
	if ($table == 'property') {
		if ($field != 'title' && $field != 'nid') {
			if (!$already_joined["${table}_${field}"]) {
				$already_joined["${table}_${field}"] = 1;
				$query->leftJoin(
					"field_data_field_" . $field,
					"${table}_${field}",
					"${table}_${field}.entity_type = 'node' and ${table}_${field}.entity_id = property_node.nid"
				);
			}
		}
	}
	
	// 
	// Joins for Local
	//
	
	if ($table == 'local') {
		if (!$already_joined["field_data_field_${table}"]) {
			$already_joined["field_data_field_${table}"] = 1;

			$query->leftJoin(
				"field_data_field_${table}",
				"field_data_field_${table}",
				"field_data_field_${table}.entity_type = 'node' and field_data_field_${table}.entity_id = property_node.nid"
			);
		}

		if (!$already_joined["${table}_node"]) {
			$already_joined["${table}_node"] = 1;
			$query->leftJoin(
				"node",
				"${table}_node",
				"${table}_node.type = '${table}' and ${table}_node.status = 1 and ${table}_node.nid = field_data_field_${table}.field_${table}_target_id"
			);
		}
		
		if ($field != 'title' && $field != 'nid') {
			if (!$already_joined["${table}_${field}"]) {
				$already_joined["${table}_${field}"] = 1;
				$query->leftJoin(
					"field_data_field_" . $field,
					"${table}_${field}",
					"${table}_${field}.entity_type = 'node' and ${table}_${field}.entity_id = ${table}_node.nid"
				);
			}
		}
	}

	//
	// Joins for Relationship
	//

	$relationship_types = array(
		'owner' => 'Owner',
		'operator' => 'Operator',
		'actor_a' => 'Actor A',
		'actor_b' => 'Actor B',
		'actor_c' => 'Actor C',
		'actor_d' => 'Actor D',
		'actor_e' => 'Actor E',
	);
	
	foreach ($relationship_types as $relationship_type => $relationship_prompt) {
		if ($table == $relationship_type || $table == $relationship_type . '_pa') {

			if ($already_joined[$relationship_type]) { continue; }
			$already_joined[$relationship_type] = 1;

			$query->leftJoin(
				"field_data_field_property",
				"${relationship_type}_field_data_field_property",
				"${relationship_type}_field_data_field_property.entity_type = 'node' and ${relationship_type}_field_data_field_property.field_property_target_id = property_node.nid"
			);
			if (preg_match('/actor/', $relationship_type)) {
				$query->leftJoin(
					"field_data_field_relationship_type",
					"${relationship_type}_field_data_field_relationship_type",
					"${relationship_type}_field_data_field_relationship_type.entity_type = 'node' and ${relationship_type}_field_data_field_relationship_type.entity_id = ${relationship_type}_field_data_field_property.entity_id ",
				);
			} else {
				$query->leftJoin(
					"field_data_field_relationship_type",
					"${relationship_type}_field_data_field_relationship_type",
					"${relationship_type}_field_data_field_relationship_type.entity_type = 'node' and ${relationship_type}_field_data_field_relationship_type.entity_id = ${relationship_type}_field_data_field_property.entity_id and ${relationship_type}_field_data_field_relationship_type.field_relationship_type_value = '$relationship_type' ",
				);
			}

			$query->leftJoin(
				"node",
				"${relationship_type}_pa_node",
				"${relationship_type}_pa_node.nid = ${relationship_type}_field_data_field_relationship_type.entity_id"
			);
			$query->leftJoin(
				"field_data_field_actor",
				"${relationship_type}_field_data_field_actor",
				"${relationship_type}_field_data_field_actor.entity_type = 'node' and ${relationship_type}_field_data_field_actor.entity_id = ${relationship_type}_pa_node.nid"
			);
			$query->leftJoin(
				"node",
				"${relationship_type}_node",
				"${relationship_type}_node.nid = ${relationship_type}_field_data_field_actor.field_actor_target_id"
			);

			/*
				$condition = db_or();
				$condition->condition("${relationship_type}_field_data_field_relationship_type.field_relationship_type_value", $relationship_prompt);
				$condition->isNull("${relationship_type}_field_data_field_relationship_type.field_relationship_type_value");
				$query->condition($condition);
			}
			*/
		}
	}

	if ($table != 'property') {

		if ($schema[$table]['table']['table_type'] == 'term') {

			if (!$already_joined["field_data_field_${table}"]) {
				$already_joined["field_data_field_${table}"] = 1;
				$query->leftJoin(
					"field_data_field_${table}",
					"field_data_field_${table}",
					"field_data_field_${table}.entity_type = 'node' and field_data_field_${table}.entity_id = property_node.nid"
				);
			}

			if (!$already_joined["${table}_term"]) {
				$already_joined["${table}_term"] = 1;
				$query->leftJoin(
					"taxonomy_term_data",
					"${table}_term",
					"${table}_term.tid = field_data_field_${table}.field_${table}_tid"
				);
			}

			if ($field != 'name' && $field != 'tid') {
				if (!$already_joined["${table}_${field}"]) {
					$already_joined["${table}_${field}"] = 1;
					$query->leftJoin(
						"field_data_field_" . $field,
						"${table}_${field}",
						"${table}_${field}.entity_type = 'taxonomy_term' and ${table}_${field}.entity_id = ${table}_term.tid"
					);
				}
			}

		} else {

			if (!$already_joined["${table}_${field}"]) {
				$already_joined["${table}_${field}"] = 1;
				if ($field != 'title' && $field != 'nid') {
					$query->leftJoin(
						"field_data_field_" . $field,
						"${table}_${field}",
						"${table}_${field}.entity_type = 'node' and ${table}_${field}.entity_id = ${table}_node.nid"
					);
				}
			}
		}
	}

	//
	// Joins for terms (obsolete, new schema above)
	//

	if ($schema[$table]['fields'][$field]['type'] == 'tid') {
		if (!$already_joined["${table}_${field}_vocabulary"]) {
			$already_joined["${table}_${field}_vocabulary"] = 1;
			$query->leftJoin(
				"taxonomy_vocabulary",
				"${table}_${field}_vocabulary",
				"${table}_${field}_vocabulary.machine_name = '" . $schema[$table]['fields'][$field]['vocabulary'] . "'"
			);
		}
		
		if (!$already_joined["${table}_${field}_term"]) {
			$already_joined["${table}_${field}_term"] = 1;
			$query->leftJoin(
				"taxonomy_term_data",
				"${table}_${field}_term",
				"${table}_${field}_term.vid = ${table}_${field}_vocabulary.vid and ${table}_${field}_term.tid = ${table}_${field}.field_${field}_tid"
			);
		}
	}

}
