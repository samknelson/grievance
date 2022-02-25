<?php

function unite_node_tab_contract_view($property_node) {
	$fields = unite_contract_fields();
	$html = '';
	foreach ($fields as $field) {
    $renderable = field_view_field('node', $property_node, $field, 'full');
		$html .= render($renderable);
	}

	if (!$html) {
		$html = '<p>' . t('No contract information has been entered.') . '</p>';
	}
	return $html;
}