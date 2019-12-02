<?php

function sirius_domain_page_switch() {
	// Fetch our domain tree.
	if (user_access('sirius admin')) {
		$tree = sirius_domain_tree();
	} else {
		$domain_root = sirius_domain_root();
		if (!$domain_root) { return 'Non-admin user with no domain set. Switch is not possible.'; }
		$tree = sirius_domain_tree($domain_root->nid);
	}

	// Are we switching?
	$new_domain_nid = $_REQUEST['switch'];
	if ($new_domain_nid) { 
		// Verify access
		if (!sirius_domain_access($new_domain_nid)) { return 'You do not have access to that domain.'; }

		// Switch 
		$domain = sirius_domain($new_domain_nid);
		drupal_set_message("Switched to: " . $domain->title);

		// Redirect
		if ($_REQUEST['destination']) { drupal_goto($_REQUEST['destination']); }
	} else {
		// No neww domain, just fetch the currently active domain
		$domain = sirius_domain();
	}

	if ($domain) {
		$html .= "Current domain: <strong>" . $domain->title . "</strong><br />";
	} else {
		$html .= "Current domain: <strong>None</strong><br />";
	}

	$html .= '<h2>' . t('Switch To:') . '</h2>';
	if (user_access('sirius admin')) {
		$html .= l('[No domain]', current_path(), array('query' => array('switch' => -1)));
	} else {
		$html .= l($domain_root->title, current_path(), array('query' => array('switch' => $domain_root->nid)));
	}
	$html .= sirius_domain_page_switch_render($tree);

	return $html;
}

function sirius_domain_page_switch_render($tree) {
	$html = '<ul>';
	foreach ($tree as $nid => $branch) {
		$html .= '<li>';
		$html .= l($branch['title'], current_path(), array('query' => array('switch' => $nid)));
		$html .= sirius_domain_page_switch_render($branch['children']);
		$html .= '</li>';
	}
	$html .= '</ul>';
	return $html;
}