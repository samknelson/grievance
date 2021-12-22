<?php

function unite_relationships_type_order($type_name) {
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

	if ($node->type == 'actor') {
		return unite_relationships_render_actor($node, $options);
	} else if ($node->type == 'property') {
		return unite_relationships_render_property($node, $options);
	} else {
		return 'Don\'t know how to render relationships for ' . $node->title . ' of type ' . $node->type;
	}
}

function unite_relationships_render_property($property_node, $options = array()) {
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
		'entity_2' => t('Actor'),
		'relationship_type' => t('Relationship'),
		'ownership' => t('Ownership'),
		'notes' => t('Notes'),
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
		$tools .= l('Edit Relationship', 'node/' . $pa_node->nid . '/edit', array('attributes' => array('class' => array('tool-button')), 'query' => array('destination' => current_path())));

		$ownership = '';		
		if ($pa_node->field_owned_raw['und'][0]['value']) {
			$ownership .= $pa_node->field_owned_raw['und'][0]['value'];
		}
		if ($pa_node->field_percent_owned['und'][0]['value'] > 0) {
			if ($ownership) { $ownership .= ' / '; }
			$ownership .= number_format($pa_node->field_percent_owned['und'][0]['value'], 2) . "%";
		}
		if ($pa_node->field_amount_owned['und'][0]['value'] > 0) {
			if ($ownership) { $ownership .= ' / '; }
			$ownership .= '$' . number_format($pa_node->field_amount_owned['und'][0]['value'], 2);
		}

		$name = $actor_node->title;

		$notes = substr($pa_node->field_notes['und'][0]['value'], 0, 200);
		$parent_nids = sirius_fieldvals($actor_node, 'field_actors', 'target_id');
		foreach ($parent_nids as $parent_nid) {
			if ($notes) { $notes .= '<br />'; }
			$notes .= l(sirius_node_title($parent_nid), "node/$parent_nid");
		}

		$row = array();
		$row[] = $name;
		$row[] = $pa_node->field_relationship_type['und'][0]['value'];
		$row[] = $ownership;
		$row[] = $notes;
		$row[] = $tools;
		$rows[] = $row;
	}

	$html = '<h2>Actors</h2>';
	$html .= sirius_showhide_open('actors_' . $property_node->nid);
	$html .= "<div id=\"$showhide_target_id\">";
	if ($rows) {
		$html .= theme_table(array('header' => array_values($header), 'rows' => $rows, 'attributes' => array()));
	} else {
		$html .= '<p><em>' . t('There are no actors connected to this property.') . '</em></p>';
	}
	$html .= l('Add an Actor', 'node/add/property-actor', array('attributes' => array('class' => array('tool-button')), 'query' => array('property' => $property_node->nid, 'destination' => current_path())));
	$html .= sirius_showhide_close();

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
		'ownership' => t('Ownership'),
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

		$ownership = '';		
		if ($pa_node->field_owned_raw['und'][0]['value']) {
			$ownership .= $pa_node->field_owned_raw['und'][0]['value'];
		}
		if ($pa_node->field_percent_owned['und'][0]['value'] > 0) {
			if ($ownership) { $ownership .= ' / '; }
			$ownership .= number_format($pa_node->field_percent_owned['und'][0]['value'], 2) . "%";
		}
		if ($pa_node->field_amount_owned['und'][0]['value'] > 0) {
			if ($ownership) { $ownership .= ' / '; }
			$ownership .= '$' . number_format($pa_node->field_amount_owned['und'][0]['value'], 2);
		}

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
