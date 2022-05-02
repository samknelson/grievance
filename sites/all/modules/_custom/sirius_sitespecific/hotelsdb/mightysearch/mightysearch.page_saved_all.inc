<?php 

function mightysearch_page_saved_all() {
	$searches = mightysearch_saved_list();
	if (!$searches) { return t('No saved searches found.'); }

	$rows = array();
	foreach ($searches as $search_nid => $search_node) {
		$tools = array();
		$tools[] = l('Load', 'mightysearch', array('attributes' => array('class' => array('tool-button')), 'query' => array('saved' => $search_nid)));
		//$tools[] = l('Delete', 'mightysearch', array('attributes' => array('class' => array('tool-button')), 'query' => array('saved' => $search_nid)));
		//$tools[] = l('Rename', 'mightysearch', array('attributes' => array('class' => array('tool-button')), 'query' => array('saved' => $search_nid)));

		$row = array();
		$row[] = $search_node->title;
		$row[] = $search_node->name;
		$row[] = date('m/d/Y', $search_node->changed);
		$row[] = join('', $tools);
		$rows[] = $row;
	}

	$header = array();
	$header[] = t('Name');
	$header[] = t('Author');
	$header[] = t('Date');
	$header[] = t('Tools');
	return theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => array()));
}

