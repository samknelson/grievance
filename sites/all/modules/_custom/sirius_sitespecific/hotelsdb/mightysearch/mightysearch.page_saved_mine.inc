<?php 

function mightysearch_page_saved_mine() {
	return drupal_get_form('mightysearch_page_saved_mine_form');
}

function mightysearch_page_saved_mine_form($form, &$form_state) {
	$searches = mightysearch_saved_list(array('mine' => TRUE));
	if (!$searches) { return array('#markup' => t('No saved searches found.')); }

	$rows = array();
	foreach ($searches as $search_nid => $search_node) {
		$tools = array();
		$tools[] = l('Load', 'mightysearch', array('attributes' => array('class' => array('tool-button')), 'query' => array('saved' => $search_nid)));
		//$tools[] = l('Delete', 'mightysearch/delete', array('attributes' => array('class' => array('tool-button')), 'query' => array('saved' => $search_nid, 'destination' => current_path())));
		//$tools[] = l('Rename', 'mightysearch/rename', array('attributes' => array('class' => array('tool-button')), 'query' => array('saved' => $search_nid, 'destination' => current_path())));

		$row = array();
		$row[] = $search_node->title;
		// $row[] = $search_node->name;
		$row[] = date('m/d/Y', $search_node->changed);
		$row[] = join('', $tools);
		$rows[$search_nid] = $row;
	}

	$header = array();
	$header[] = t('Name');
	// $header[] = t('Author');
	$header[] = t('Date');
	$header[] = t('Tools');


	$form['search_nid'] = array(
	  '#type' => 'tableselect',
	  '#header' => $header,
	  '#options' => $rows,
	  '#empty' => t('No searches found.'),
	  '#multiple' => FALSE,
	);

	$form['action'] = array(
		'#type' => 'select',
		'#title' => 'Action',
		'#options' => array(
			'rename' => t('Rename'),
			'delete' => t('Delete'),
		),
		'#default_value' => 'rename',
	);

	$form['title'] = array(
		'#type' => 'textfield',
		'#title' => 'New Name',
		'#states' => array('visible' => array(":input[name=\"action\"]" => array('value' => 'rename'))),
	);

	$form['submit'] = array('#type' => 'submit', '#value' => t('Submit'));
	return $form;
}

function mightysearch_page_saved_mine_form_submit($form, &$form_state) {
	$search_nid = $form_state['values']['search_nid'];
	if (!$search_nid) {
		drupal_set_message("Please select a search.", 'error'); 
		return;
	}

	$search_node = node_load($search_nid);
	if (!$search_node) {
		drupal_set_message("Search not found.", 'error');
		return;
	}
	$action = $form_state['values']['action'];
	if ($action == 'rename') {
		$new_title = $form_state['values']['title'];
		if (!$new_title) {
			drupal_set_message("Please select a new title.", 'error');
			return;
		}
		$search_node->title = $new_title;
		node_save($search_node);
		drupal_set_message("Name changed.");
	}

	if ($action == 'delete') {
		node_delete($search_node->nid);
		drupal_set_message("Search deleted.");
		return;
	}
}