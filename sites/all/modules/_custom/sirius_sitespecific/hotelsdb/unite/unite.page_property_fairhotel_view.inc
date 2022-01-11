<?php

function unite_node_tab_fairhotel_view($property_node) {
	$fields = unite_fairhotel_fields();
	$html = '';
	foreach ($fields as $field) {
    $renderable = field_view_field('node', $property_node, $field, 'full');
		$html .= render($renderable);
	}

	if (!$html) {
		$html = '<p>' . t('No FairHotel information has been entered.') . '</p>';
	}
	return $html;
}