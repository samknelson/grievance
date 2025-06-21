<?php

function sirius_cardcheck_page_definition_list() {
	$instance = Sirius_Cardcheck::getInstance();

	$definition_nodes = $instance->definition_list(array('include_inactive' => TRUE));

	if ($definition_nodes) {
		$rows = array();
		foreach ($definition_nodes as $definition_nid => $definition_node) {
			$definition_json = sirius_json_get($definition_node);
			$definition = $definition_json['cardcheck_definition'];

			$row = array();
			$row[] = $definition['title'];
			$row[] = $definition['active'];

			$tools = array();
			$tools[] = l('Edit', "node/$definition_nid/sirius_json_definition_cardcheck_definition/edit", array('attributes' => array('class' => array('tool-button'))));
			$tools[] = l('Preview', "node/$definition_nid/sirius_json_definition_cardcheck_definition/preview", array('attributes' => array('class' => array('tool-button'))));
			$row[] = join('', $tools);
			$rows[] = $row;
		}
		$header = array();
		$header[] = t('Title');
		$header[] = t('Active?');
		$header[] = t('Tools');
		$html .= sirius_table_render($header, $rows);
	}

	$html .= l('Add a Card Check', current_path() . '/add', array('query' => array('destination' => current_path()), 'attributes' => array('class' => array('tool-button'), 'query')));
	return $html;
}
