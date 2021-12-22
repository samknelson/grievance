<?php

function sirius_page_taxonomy_vocabulary_list() {
	$sql = "select name, machine_name, vid, description ";
	$sql .= "from taxonomy_vocabulary ";
	$sql .= "order by name ";
	$stmt = db_query($sql, array());
	$vocabularies = array();

	$domain_node = sirius_domain();

	if ($domain_node) {
		drupal_set_title(drupal_get_title() . ' - ' . $domain_node->title);
	} else {
		drupal_set_title(drupal_get_title() . ' - ' . t('No Domain'));
	}

  $all_settings = sirius_taxonomy_settings('all');

  $rows = array();
	while ($hr = $stmt->fetchAssoc()) {
		// This taxonomy must be configured for Sirius access
		if (!$all_settings[$hr['machine_name']]) { continue; }

		// The user must have access to these terms
		if (!user_access('edit terms in ' . $hr['vid']) && !user_access('sirius admin')) { continue; }

		$row = array();
		$row[] = $hr['name'];
		$row[] = $hr['description'];
		$row[] = l(t('List'), 'sirius/taxonomy/' . $hr['machine_name'], array('attributes' => array('class' => array('tool-button')))) . 
			l(t('Sort'), 'sirius/taxonomy/' . $hr['machine_name'] . '/sort', array('attributes' => array('class' => array('tool-button'))));

		$rows[] = $row;
	}

	$header = array('List Name', 'Description', 'Tools');

	$html .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array()));
	return $html;
}
