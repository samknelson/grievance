<?php

function sirius_domain_page_list() {
	return sirius_domain_page_list_impl(array('switch' => TRUE, 'tools' => TRUE, 'stats' => FALSE));
}

function sirius_domain_page_list_impl($options) {
	// Fetch our domain tree.
	if (user_access('sirius admin')) {
		$tree = sirius_domain_tree();
	} else {
		$domain_root = sirius_domain_root();
		if (!$domain_root) { return 'Non-admin user with no domain set. Switch is not possible.'; }
		$tree = sirius_domain_tree($domain_root->nid);
	}

	if ($options['switch']) {
		// Are we switching?
		$new_domain_nid = $_REQUEST['switch'];
		if ($new_domain_nid) { 
			// Verify access
			if (!sirius_domain_access($new_domain_nid)) { return 'You do not have access to that domain.'; }

			// Switch 
			$domain = sirius_domain($new_domain_nid);
			if ($domain) {
				drupal_set_message("Switched to: " . $domain->title);
			} else {
				drupal_set_message("Switched to: [no domain]");
			}

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
			$html .= l('switch into no domain / root', current_path(), array('attributes' => array('class' => array('tool-button')), 'query' => array('switch' => -1)));
		} else {
			$html .= l($domain_root->title, current_path(), array('query' => array('switch' => $domain_root->nid)));
		}
		$html .= '<br /><br />';
	}

	if ($options['stats']) {
		global $user;
		$user_full = user_load($user->uid);
		$user_json = sirius_jsonfield_util_user_json_get($user_full);
		if ($user_json['sirius']['domain']['stats']['key']) { $key_stats = array_filter($user_json['sirius']['domain']['stats']['key']); }
		else { $key_stats = array(); }
		if (!$key_stats) {
			$html .= '<strong>' . t('No key stats selected.') . '</strong> ';
			$html .= t('Please go to the "Preferences" tab and select the key stats to display.');
			$html .= '<br /><br />';
		}
	}

	$html .= sirius_domain_page_list_render($tree, NULL, $options);

	$paths = array(
		'node/add/sirius-domain' => 'Add a Domain',
		'sirius/domain/components/root' => 'Root Domain Components',
	);

	foreach ($paths as $path => $title) {
		if (drupal_valid_path($path)) {
			$html .= l($title, $path, array('attributes' => array('class' => array('tool-button'))));
		}
	}

	return $html;
}

function sirius_domain_page_list_render($tree, $depth = NULL, $options = array()) {
	if ($options['stats']) {
		global $user;
		$user_full = user_load($user->uid);
		$user_json = sirius_jsonfield_util_user_json_get($user_full);
		if ($user_json['sirius']['domain']['stats']['key']) { $key_stats = array_filter($user_json['sirius']['domain']['stats']['key']); }
		else { $key_stats = array(); }

		$include_subdomains = FALSE;
		if ($user_json['sirius']['domain']['stats']['include_subdomains'] == 'Yes') { $include_subdomains = TRUE; }
		$domains = node_load_multiple(array_keys($tree));
	}

	if (!$depth) {
		$table_css_class = ' ';
		if ($options['stats']) { $table_css_class .= 'sirius_data_table sirius_data_table_wide '; }
		$html .= "<table class=\"$table_css_class\">";
		$html .= '<tr>';
		$html .= '<th>' . t('Domain') . '</th>';
		if ($options['switch']) { $html .= '<th>' . t('Switch') . '</th>'; }
		if ($options['tools']) { $html .= '<th>' . t('Tools') . '</th>'; }
		if ($options['stats']) { 
			foreach ($key_stats as $key_stat) {
				$title = sirius_domain_stats_info($key_stat)['title_full'];
				$title = preg_replace('/:/', '<br />', $title);
				$html .= '<th>';
				$html .= $title;
				$html .= '</th>';
			}
			$html .= '<th>';
			$html .= t('Details');
			$html .= '</th>';
		}
		$html .= '</tr>';
	}
	foreach ($tree as $nid => $branch) {
		$html .= '<tr>';
		$html .= '<td>';
		for ($i=0; $i<$depth; ++$i) {
			$html .= '-- ';
		}
		$html .= $branch['title'];

		if ($branch['name_short']) {
			$html .= ' (' . $branch['name_short'] . ')';
		}

		$html .= '</td>';

		if ($options['switch']) {
			$html .= '<td>';
			$html .= l('switch into this domain', current_path(), array('query' => array('switch' => $nid), 'attributes' => array('class' => array('tool-button'))));;
			$html .= '</td>';
		}

		if ($options['tools']) {
			$html .= '<td>';
			$tools = array(
				array('path' => "node/$nid", 'title' => 'view'),
				array('path' => "node/$nid/sirius_domain_users", 'title' => 'users'),
				array('path' => "node/$nid/sirius_domain_components", 'title' => 'components'),
				array('path' => "node/$nid/sirius_domain_stats", 'title' => 'stats'),
			);
			foreach ($tools as $link) {
				if (drupal_valid_path($link['path'])) {
					$html .= l($link['title'], $link['path'], array('attributes' => array('class' => array('tool-button'))));
				}
			}

			$html .= '</td>';
		}


		if ($options['stats']) {
			$domain_json = sirius_jsonfield_util_node_json_get($domains[$nid]);
			foreach ($key_stats as $key_stat) {
				if ($include_subdomains) { 
					$val = $domain_json['stats_total'][$key_stat];
				} else {
					$val = $domain_json['stats'][$key_stat];
				}

				$html .= '<td>';
				$html .= number_format($val);
				$html .= '</td>';
			}

			$html .= '<td>';
			$html .=  l('details', 'node/' . $nid . '/sirius_domain_stats', array('attributes' => array('class' => array('tool-button'))));
			$html .= '</td>';
		}
		$html .= '</tr>';

		$html .= sirius_domain_page_list_render($branch['children'], $depth + 1, $options);
	}
	if (!$depth) { $html .= '</table>'; }

	return $html;
}
