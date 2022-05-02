<?php

function mightysearch_saved_list($params = array()) {
	$query = new EntityFieldQuery();
	$query->entityCondition('entity_type', 'node');
	$query->entityCondition('bundle', 'sirius_json_definition');
	$query->fieldCondition('field_sirius_type', 'value', 'mightysearch:saved_search');
	if ($params['mine']) {
		global $user;
		$query->propertyCondition('uid', $user->uid);
	}
	$query->propertyOrderBy('title');
	$stmt = $query->execute();
	if (!$stmt['node']) { return array(); }

	return node_load_multiple(array_keys($stmt['node']));
}

function mightysearch_saved_render_home() {
	$searches = mightysearch_saved_list(array('mine' => TRUE));
	if (!$searches) { return '<p>' . t('No saved searches found.') . '</p>'; }

	$rows = array();
	foreach ($searches as $search_nid => $search_node) {
		$tools = array();
		$tools[] = 
		//$tools[] = l('Delete', 'mightysearch', array('attributes' => array('class' => array('tool-button')), 'query' => array('saved' => $search_nid)));
		//$tools[] = l('Rename', 'mightysearch', array('attributes' => array('class' => array('tool-button')), 'query' => array('saved' => $search_nid)));

		$row = array();
		$row[] = l($search_node->title, 'mightysearch', array('query' => array('saved' => $search_nid)));
		$rows[] = $row;
	}

	$header = array();
	$header[] = t('Name');
	return theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => array()));
}

