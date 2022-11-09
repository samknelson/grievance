<?php

function sirius_node_tab_sirius_domain_info($domain_node) {

	$fields = array('field_sirius_name_short', 'field_sirius_active', 'field_sirius_tz', 'field_sirius_domain');
	foreach ($fields as $field) {
		$renderable = field_view_field('node', $domain_node, $field, 'full');
		$html .= drupal_render($renderable);
	}

	$info_all = sirius_domain_component_info_all();
	$rows = array();
	foreach ($info_all as $key => $info) {
		if (!sirius_domain_component_enabled($key)) { continue; }
		$enabled = 'Yes';
		$class = 'green';
		$row = array();
		$row[] = $key;
		$row[] = $info['title'];

		$rows[] = array('data' => $row, 'class' => array($class));
	}

	$header = array(
		t('ID'),
		t('Title'),
	);

	$html .= '<h2>' . t('Enabled Components') . '</h2>';
	$html .= sirius_table_render($header, $rows);
	return $html;
}
