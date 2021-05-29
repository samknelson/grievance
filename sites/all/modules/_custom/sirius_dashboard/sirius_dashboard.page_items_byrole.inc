<?php

function sirius_dashboard_page_items_byrole() {
	sirius_backlink_add('sirius/dashboard', array('name' => 'Dashboard'));

	$roles = user_roles(TRUE, 'sirius dashboard user');

	$rows = array();
	foreach ($roles as $rid => $role_name) {
		$row = array();
		$row[] = $role_name;
		$row[] = l('Manage', 'sirius/dashboard/items_by_role/' . $rid, array('attributes' => array('class' => array('tool-button'))));

		$rows[] = $row;
	}

	$header = array('Role', 'Tools');
	return theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => array()));
}