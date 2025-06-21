<?php

function unite_relationships_type_order($type_name) {
	$order['Colocation Host'] = -2;
	$order['Colocated'] = -1;
	$order['Owner'] = 0;
	$order['Operator'] = 1;
	$order['Leaseholder'] = 2;
	$order['Land Leaseholder'] = 3;
	$order['Landowner'] = 4;
	$order['Developer'] = 5;
	$order['Loan Servicer'] = 6;
	$order['Previous Operator'] = 7;
	$order['Previous Owner'] = 8;

	return $order[$type_name];
}

function unite_relationships_render($node, $options = array()) {
	drupal_add_css(drupal_get_path('module', 'unite') . '/css/unite.css', 'file');
	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/sirius.modal.css', 'file');

	if ($node->type == 'actor') {
		return unite_relationships_render_actor($node, $options);
	} else if ($node->type == 'property') {
		return unite_relationships_render_property($node, $options);
	} else {
		return 'Don\'t know how to render relationships for ' . $node->title . ' of type ' . $node->type;
	}
}

function unite_relationships_render_ownership($pa_node) {
	$parts = array();
	if ($pa_node->field_owned_raw['und'][0]['value']) {
		$parts[] = $pa_node->field_owned_raw['und'][0]['value'];
	}
	if ($pa_node->field_percent_owned['und'][0]['value'] > 0) {
		$parts[] = number_format($pa_node->field_percent_owned['und'][0]['value'], 2) . "%";
	}
	if ($pa_node->field_amount_owned['und'][0]['value'] > 0) {
		$parts[] = '$' . number_format($pa_node->field_amount_owned['und'][0]['value'], 2);
	}
	if ($pa_node->field_lg_type['und'][0]['tid']) {
		$term = taxonomy_term_load($pa_node->field_lg_type['und'][0]['tid']);
		$parts[] = $term->name;
	}
	if ($pa_node->field_lg_status['und'][0]['tid']) {
		$term = taxonomy_term_load($pa_node->field_lg_status['und'][0]['tid']);
		$parts[] = $term->name;
	}
	if ($pa_node->field_lg_balance['und'][0]['value']) {
		$parts[] = 'Balance $' . number_format($pa_node->field_lg_balance['und'][0]['value'], 0);
	}
	if ($pa_node->field_lg_amount['und'][0]['value']) {
		$parts[] = 'Initial $' . number_format($pa_node->field_lg_amount['und'][0]['value'], 0);
	}
	return join(' / ', $parts);
}

function unite_relationships_render_property($property_node, $options = array()) {
	$path = $options['path'];
	if (!$path) { $path = $_REQUEST['path']; }
	if (!$path) { $path = current_path(); }

	$pa_nodes = array();
	$query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'property_actor');
  $query->fieldCondition('field_property', 'target_id', $property_node->nid);
  $stmt = $query->execute();
  if ($stmt['node']) {
  	$pa_nodes = node_load_multiple(array_keys($stmt['node']));
	}

	$actors = array();
	foreach ($pa_nodes as $pa_nid => $pa_node) {
		$actor_nid = $pa_node->field_actor['und'][0]['target_id'];
		$actor_node = node_load($actor_nid);
		$actors[] = array('pa_node' => $pa_node, 'actor_node' => $actor_node);
	}

	$header = array(
		'actor' => t('Actor'),
		'relationship' => t('Relationship'),
		'notes' => t('Notes'),
		'verified' => t('Verified'),
		'tools' => 'Tools',
	);

	usort($actors, function($a, $b) {
		$a_type_name = $a['pa_node']->field_relationship_type['und'][0]['value'];
		$b_type_name = $b['pa_node']->field_relationship_type['und'][0]['value'];
		$a_order = unite_relationships_type_order($a_type_name);
		$b_order = unite_relationships_type_order($b_type_name);
		if ($a_order > $b_order) { return 1; }
		if ($a_order < $b_order) { return -1; }
		return strcmp(strtoupper($a['actor_node']->title), strtoupper($b['actor_node']->title));
	});

	$rows = array();
	foreach ($actors as $actor) {
		$actor_node = $actor['actor_node'];
		$pa_node = $actor['pa_node'];

		$tools = l('Actor', 'node/' . $actor_node->nid, array('attributes' => array('class' => array('tool-button'))));
		$tools .= l('Relationship', 'node/' . $pa_node->nid, array('attributes' => array('class' => array('tool-button'))));
		$tools .= l('Edit Relationship', 'node/' . $pa_node->nid . '/edit', array('attributes' => array('class' => array('tool-button')), 'query' => array('destination' => $path)));

		$ownership = unite_relationships_render_ownership($pa_node);

		$name = $actor_node->title;

		$notes = array();
		$notes[] = substr($pa_node->field_notes['und'][0]['value'], 0, 200);
		$parent_nids = sirius_fieldvals($actor_node, 'field_actors', 'target_id');
		$notes[] = $pa_node->field_sublocation['und'][0]['value'];
		foreach ($parent_nids as $parent_nid) {
			$notes[] = l(sirius_node_title($parent_nid), "node/$parent_nid");
		}
		$notes[] = $ownership;
		$notes = join('<br />', array_filter($notes));

	  $last_verified = $pa_node->field_verified_date['und'][0]['value'];
	  if ($last_verified) {
	  	$last_verified_ts = strtotime($last_verified . ' GMT');
	  	$last_verified_interval = time() - $last_verified_ts;
	  	$last_verified_txt = "Verified: <strong>" . format_interval($last_verified_interval, 1) . " ago</strong>";
	  	if ($last_verified_interval < 60*60*24*30*6) { $class = "hotelsdb_last_verified_green"; }
	  	else if ($last_verified_interval < 60*60*24*30*18) { $class = "hotelsdb_last_verified_blue"; }
	  	else  { $class = "hotelsdb_last_verified_red"; }
	  } else {
	  	$class = "hotelsdb_last_verified_red";
	  	$last_verified_txt = "Verified: <strong>Never</strong>";
	  }

	  $verified = l(
	  	$last_verified_txt, 
	  	'sirius/modal/' . $pa_node->nid . '/unite-relationship-verify', 
	  	array('query' => array('path' => $path), 'html' => TRUE, 'attributes' => array('class' => array('hotelsdb_last_verified_modal_link', $class)))
	  );


		$row = array();
		$row[] = $name;
		$row[] = $pa_node->field_relationship_type['und'][0]['value'];
		$row[] = $notes;
		$row[] = $verified;
		$row[] = $tools;

		$rows[] = $row;
	}

	$html .= '<div id="hotelsdb_actors_table">';
	$html .= '<h2>Actors</h2>';
	$html .= sirius_showhide_open('actors_' . $property_node->nid);
	$html .= "<div id=\"$showhide_target_id\">";
	if ($rows) {
		$html .= theme_table(array('header' => array_values($header), 'rows' => $rows, 'attributes' => array()));
	} else {
		$html .= '<p><em>' . t('There are no actors connected to this property.') . '</em></p>';
	}
	$html .= l('Add an Actor', 'node/add/property-actor', array('attributes' => array('class' => array('tool-button')), 'query' => array('property' => $property_node->nid, 'destination' => $path)));
	$html .= sirius_showhide_close();
	$html .= '<div id="sirius_modal_messages"></div>';
	$html .= '</div>';

	return $html;
}

function unite_relationships_render_actor($actor_node, $options = array()) {
	$pa_nodes = array();
	$query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'property_actor');
  $query->fieldCondition('field_actor', 'target_id', $actor_node->nid);
  $stmt = $query->execute();
  if ($stmt['node']) {
  	$pa_nodes = node_load_multiple(array_keys($stmt['node']));
	}

	$properties = array();
	foreach ($pa_nodes as $pa_nid => $pa_node) {
		$property_nid = $pa_node->field_property['und'][0]['target_id'];
		$property_node = node_load($property_nid);
		$properties[] = array('pa_node' => $pa_node, 'property_node' => $property_node);
	}
	usort($properties, function($a, $b) {
		$a_type_name = $a['pa_node']->field_relationship_type['und'][0]['value'];
		$b_type_name = $b['pa_node']->field_relationship_type['und'][0]['value'];
		$a_order = unite_relationships_type_order($a_type_name);
		$b_order = unite_relationships_type_order($b_type_name);
		if ($a_order > $b_order) { return 1; }
		if ($a_order < $b_order) { return -1; }
		return strcmp(strtoupper($a['property_node']->title), strtoupper($b['property_node']->title));
	});

	$header = array(
		'entity_1' => t('Property'),
		'relationship_type' => t('Relationship'),
		'ownership' => t('Summary'),
		'rooms' => t('Rooms'),
		'city' => t('City'),
		'state' => t('State'),
		'union_status' => t('Union Status'),
		'tools' => 'Tools',
	);

	$union_status_field = field_info_field('field_union_status');
	$union_status_lookup = list_allowed_values($union_status_field);

	$rows = array();
	$count = 0;
	foreach ($properties as $property) {
		$property_node = $property['property_node'];
		$pa_node = $property['pa_node'];

		++$count;
		if ($count > 100) {
			break;
		}

		$tools = l('Property', 'node/' . $property_node->nid, array('attributes' => array('class' => array('tool-button'))));
		$tools .= l('View Relationship', 'node/' . $pa_node->nid, array('attributes' => array('class' => array('tool-button'))));
		$tools .= l('Edit Relationship', 'node/' . $pa_node->nid . '/edit', array('attributes' => array('class' => array('tool-button')), 'query' => array('destination' => current_path())));

		$ownership = unite_relationships_render_ownership($pa_node);

		if ($property_node->field_rooms['und'][0]['value']) {
			$rooms = number_format($property_node->field_rooms['und'][0]['value'], 0);
		} else {
			$rooms = '';
		}

		$row = array();
		$row[] = $property_node->title;
		$row[] = $pa_node->field_relationship_type['und'][0]['value'];
		$row[] = $ownership;
		$row[] = $rooms;
		$row[] = $property_node->field_address['und'][0]['locality'];
		$row[] = $property_node->field_address['und'][0]['administrative_area'];
		$row[] = $union_status_lookup[$property_node->field_union_status['und'][0]['value']];
		$row[] = $tools;
		$rows[] = $row;
	}

	$html = '<h2>Properties</h2>';
	$html .= sirius_showhide_open('properties_' . $actor_node->nid);
	if ($rows) {
		$html .= theme_table(array('header' => array_values($header), 'rows' => $rows, 'attributes' => array()));
	} else {
		$html .= '<p><em>' . t('There are no properties connected to this actor.') . '</em></p>';
	}
	if ($count > 50) {
		$html .= '<p>' . '[additional relationships suppressed, use advanced search to view all]' . '</p>';
	}
	if (!$options['skip_add']) {
		$html .= l('Add an Property', 'node/add/property-actor', array('attributes' => array('class' => array('tool-button')), 'query' => array('actor' => $actor_node->nid, 'destination' => current_path())));
	}
	$html .= sirius_showhide_close();

	if (!$options['skip_parents']) {
		$parent_nids = sirius_fieldvals($actor_node, 'field_actors', 'target_id');
		$header = array('Parent', 'Tools');
		$rows = array();
		foreach ($parent_nids as $parent_nid) {
			if ($options['skip_parent_nid'] == $parent_nid) { continue; }
			$parent_title = sirius_node_title($parent_nid);
			$row = array();
			$row[] = $parent_title;
			$row[] = l('View', "node/$parent_nid", array('attributes' => array('class' => array('tool-button'))));
			$rows[] = $row;
		}
		if ($rows) {
			$html .= '<h2>' . t('Parents') . '</h2>';
			$html .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array()));
		}
	}

	$sql = "select entity_id from field_data_field_actors where entity_type = 'node' and field_actors_target_id = :actor_nid ";
	$stmt = sirius_sql_query($sql, array(':actor_nid' => $actor_node->nid));
	$child_actor_nids = array();
	while ($hr = $stmt->fetchAssoc()) { $child_actor_nids[] = $hr['entity_id']; }


	if ($child_actor_nids) {
		$count = 0;
		foreach ($child_actor_nids as $child_actor_nid) {
			++$count;
			$child_actor_node = node_load($child_actor_nid);

			$html .= '<h2>' . 'Child: ' . $child_actor_node->title . '</h2>';
			$html .= sirius_showhide_open('child_' . $actor_node->nid . '_' . $child_actor_nid, TRUE);
			$html .= '<div class="unite_child_relationships">';
			if ($count > 20) {
				$html .= '<p>' . '[additional relationships suppressed, use advanced search to view all]' . '</p>';
			} else {
				$html .= unite_relationships_render_actor($child_actor_node, array('skip_add' => TRUE, 'skip_parent_nid' => $actor_node->nid));
			}
			$html .= l('View', "node/$child_actor_nid", array('attributes' => array('class' => array('tool-button'))));
			$html .= '</div>';
			$html .= sirius_showhide_close();
		}
		/*
		$header = array('Child', 'Tools');
		$rows = array();
		foreach ($child_actor_nids as $child_nid) {
			$child_title = sirius_node_title($child_nid);
			$row = array();
			$row[] = $child_title;
			$row[] = l('View', "node/$child_nid", array('attributes' => array('class' => array('tool-button'))));
			$rows[] = $row;
		}
		if ($rows) {
			$html .= '<h2>' . t('Children') . '</h2>';
			$html .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array()));
		}
		*/

	}

	return $html;
}

/**
 * Set owner / operator last updated
 */

function unite_relationship_node_presave($relationship_node) {
	$original_node = $relationship_node->original;

	// Are we updating the owner and / or operator?
	if ($relationship_node->field_relationship_type['und'][0]['value'] == 'Owner') { $is_owner = TRUE; }
	if ($relationship_node->field_relationship_type['und'][0]['value'] == 'Operator') { $is_operator = TRUE; }

	if ($original_node) {
		if ($original_node->field_relationship_type['und'][0]['value'] == 'Owner') { $is_owner = TRUE; }
		if ($original_node->field_relationship_type['und'][0]['value'] == 'Operator') { $is_operator = TRUE; }
	}

	if (!$is_owner && !$is_operator) { return; }

	// Is this a "real" change?
	if (!unite_nodes_have_interesting_differences($relationship_node, $original_node)) { return; }

	// Which properties? Might be none, might be two.
	$property_nids = array();
	$property_nids[] = $relationship_node->field_property['und'][0]['target_id'];
	if ($original_node) { $property_nids[] = $original_node->field_property['und'][0]['target_id']; }
	$property_nids = array_filter(array_unique($property_nids));

	foreach ($property_nids as $property_nid) {
		$property_node = node_load($property_nid);
		if ($is_owner) { $property_node->field_calc_owner_last_updated['und'][0]['value'] = gmdate('Y-m-d H:i'); }
		if ($is_operator) { $property_node->field_calc_operator_last_updated['und'][0]['value'] = gmdate('Y-m-d H:i'); }
		node_save($property_node);
	}
}

/**
 * A rather hacky way to determine if two nodes have "real" differences.
 * 
 * We'd like to do this with a simple recursive comparison, or by serializing the fields, or something like that.
 * But unfortunately when you save a node with some empty fields, you end up with e.g.
 * 
 *   $node->field_whatever['und'][0]['target_id'] = NULL
 *   $node->original->field_whatever = NULL;
 * 
 * Which, of course, serialize very differently.
 * 
 * So, short of spending a lot of time extracting actual values and then deciding on the difference between 0 and NULL, I'm 
 * simply rendering each of the fields and doing a string comparison.
 * 
 * Returns the first field name where an interesting difference is found.
 */ 

function unite_nodes_have_interesting_differences($node1, $node2) {
	if (!$node1 && !$node2) { return FALSE; }
	if (!$node1 || !$node2) { return TRUE; }

	$fields = array_merge(array_keys((array) $node1), array_keys((array) $node2));
	$fields = array_unique($fields);

	foreach ($fields as $field) {
		if (!preg_match('/^field_/', $field)) { continue; }
		$renderable1 = field_view_field('node', $node1, $field);
		$renderable2 = field_view_field('node', $node2, $field);
		$render1 = drupal_render($renderable1);
		$render2 = drupal_render($renderable2);
		if (strcmp($render1, $render2)) { return $field; }
	}

	return FALSE;
}

