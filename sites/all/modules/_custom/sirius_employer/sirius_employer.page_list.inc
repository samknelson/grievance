<?php

/**
 * @todo: Basically, at the moment, this page just exists to provide a component-controlled destination for a menu item.
 *        All it really does is redirect to one of the role-specific menu items.
 *        It would be a good thing to refactor this, and stop linking directly to View pages.
 */

function sirius_employer_page_list_access() {
	if (!sirius_domain_component_enabled('employer')) { return FALSE; }

	$paths = array(
		'sirius/admin/shops',
		'sirius/employer/me',
	);
	foreach ($paths as $path) { 
		if (drupal_valid_path($path)) { return TRUE; }
	}

	return FALSE;
}

function sirius_employer_page_list() {
	$paths = array(
		'sirius/admin/shops',
		'sirius/employer/me',
	);
	foreach ($paths as $path) { 
		if (drupal_valid_path($path)) { drupal_goto($path); }
	}
}
