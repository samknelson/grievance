<?php

function sirius_trust_relationship_title($relationship_node) {
	$name = '';

	$dependent_contact_nid = $relationship_node->field_sirius_contact_alt['und'][0]['target_id'];
	if ($dependent_contact_nid) {
		$name = sirius_node_title($dependent_contact_nid);
	} else {
		$name = '[no name]';
	}

	$relationship_type_tid = $relationship_node->field_sirius_contact_reltype['und'][0]['tid'];
	$relationship_type_term = taxonomy_term_load($relationship_type_tid);
	if ($relationship_type_term) {
		$name .= ' (' . $relationship_type_term->field_sirius_name_alt['und'][0]['value'] . ')';
	}
	return $name;
}

function sirius_trust_relationship_criteria_check($relationship_criteria, $relationship_nids) {
	foreach ($relationship_criteria as $criterion) {
		$result = sirius_trust_relationship_criterion_check($criterion, $relationship_nids);
		if (!$result['success']) { return $result; }
	}

	return array('success' => TRUE);
}

function sirius_trust_relationship_criterion_check($criterion, $relationship_nids) {
	$min = $criterion['min'];
	$max = $criterion['max'];
	$relationship_type_tids = array_filter(array_values($criterion['relationship_types']));
	if (!$relationship_type_tids) { return array('success' => TRUE); }

	$relationship_nodes = node_load_multiple($relationship_nids);
	$counts = array();
	foreach ($relationship_nodes as $relationship_nid => $relationship_node) {
		$counts[$relationship_node->field_sirius_contact_reltype['und'][0]['tid']]++;
	}

	$total = 0;
	foreach ($relationship_type_tids as $relationship_type_tid) { $total += $counts[$relationship_type_tid]; }

	if ( ($min && ($min > $total)) || ($max && $max < $total)) {
		return array('success' => FALSE, 'msg' => 'Please select: ' . sirius_trust_relationship_criterion_render($criterion));
	}

	return array('success' => TRUE);
}

function sirius_trust_relationship_criteria_get($json) {
	$relationship_criteria = $json['relationship_criteria'];
	if (!$relationship_criteria) { return array(); }

	uksort($relationship_criteria, function($a, $b) use ($relationship_criteria) { return $relationship_criteria[$a]['weight'] <=> $relationship_criteria[$b]['weight']; });

	return $relationship_criteria;
}

function sirius_trust_relationship_criteria_render($relationship_criteria, $options = array()) {

	foreach ($relationship_criteria as $uuid => $criterion) {
		$t = sirius_trust_relationship_criterion_render($criterion, $options);
		if ($t) { $text .= '<li>' . $t . '</li>'; }
	}

	if ($text) {
		$title = $options['title'];
		if (!$title) { $title = 'Relationship Criteria'; }
		$text = '<strong>' . $title . '</strong>' . '<ul>' . $text . '</ul>';
	} else {
		$text = '';
	}
	return $text;
}

function sirius_trust_relationship_criterion_render($criterion, $options = array()) {
 $relationship_type_name_lookup = &drupal_static(__FUNCTION__);
  if (!isset($relationship_type_name_lookup)) {
		$relationship_type_name_lookup = sirius_taxonomy_load('sirius_contact_relationship_types', 'tid', 'name');
	}

	$min = $criterion['min'];
	$max = $criterion['max'];
	$relationship_type_tids = array_filter(array_values($criterion['relationship_types']));
	if (!$relationship_type_tids) { return ''; }

	if ($min && $max) {
		if ($min == $max) {
			$text .= "Exactly $min ";
		} else {
			$text .= "Between $min and $max ";
		}
	} else if ($min) {
		$text .= "At least $min ";
	} else if ($max) {
		$text .= "At most $max ";
	} else {
		$text .= "Any number of ";
	}
	//$text .= " relationships of type ";
	$relationship_type_names = array();
	foreach ($relationship_type_tids as $relationship_type_tid) {
		$relationship_type_names[] = $relationship_type_name_lookup[$relationship_type_tid];
	}
	$text .= join(', ', $relationship_type_names);
	return $text;
}

function sirius_trust_relationship_options($worker_node, $params = array()) {
	$contact_nid = $worker_node->field_sirius_contact['und'][0]['target_id'];
	if (!$contact_nid) { return array(); }

	$params['contact_nid'] = $contact_nid;
	$relationship_nodes = sirius_contact_relationships($params);

	if (!$relationship_nodes) { return array(); }

	$relationship_options = array();
	foreach ($relationship_nodes as $relationship_nid => $relationship_node) {
		$relationship_type_tid = $relationship_node->field_sirius_contact_reltype['und'][0]['tid'];
		$relationship_type_term = taxonomy_term_load($relationship_type_tid);
		$beneficiary_contact_nid = $relationship_node->field_sirius_contact_alt['und'][0]['target_id'];
		$beneficiary_worker_node = sirius_worker_get_by_contact_nid($beneficiary_contact_nid);
		$relationship_options[$relationship_nid] = '(' . $relationship_type_term->name . ' to) ' . $beneficiary_worker_node->title;
		if ($relationship_node->field_sirius_active['und'][0]['value'] != 'Yes') {
			$relationship_options[$relationship_nid] .= ' [INACTIVE]';
		}
	}

	return $relationship_options;
}
