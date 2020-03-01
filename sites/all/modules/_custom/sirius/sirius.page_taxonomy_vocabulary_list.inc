<?php

function sirius_page_taxonomy_vocabulary_list() {
	$sql = "select name, machine_name, vid, description ";
	$sql .= "from taxonomy_vocabulary ";
	$sql .= "order by name ";
	$stmt = db_query($sql, array());
	$vocabularies = array();

	$domain_node = sirius_domain();

	if ($domain_node) {
		$html .= t('Use the links below to manage drop-down lists (taxonomies) for <strong>@title.</strong>', array('@title' => $domain_node->title));
	} else {
		$html .= t('Use the links below to manage the drop-down lists (taxonomies) for this site.');
	}

  $all_settings = sirius_taxonomy_settings('all');

	$html .= '<ul>';
	while ($hr = $stmt->fetchAssoc()) {
		// This taxonomy must be configured for Sirius access
		if (!$all_settings[$hr['machine_name']]) { continue; }

		// The user must have access to these terms
		if (!user_access('edit terms in ' . $hr['vid'])) { continue; }

		$html .= '<dl>';

		$html .= '<dt>';
		$html .= '<strong>' . $hr['name'] . '</strong>';
		$html .= '</dt>';

		$html .= '<dd>';
		if ($hr['description']){
			$html .= $hr['description'];
			$html .= '<br />';
		}
		$html .= l(t('List'), 'sirius/taxonomy/' . $hr['machine_name'], array('attributes' => array('class' => array('tool-button'))));
		$html .= '&nbsp;';
		$html .= l(t('Sort'), 'sirius/taxonomy/' . $hr['machine_name'] . '/sort', array('attributes' => array('class' => array('tool-button'))));
		$html .= '</dd>';

		$html .= '</li>';
	}
	$html .= '</ul>';

	return $html;
}
