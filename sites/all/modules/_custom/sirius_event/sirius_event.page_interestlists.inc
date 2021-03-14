<?php

function sirius_event_page_interestlists() {
	drupal_add_js(drupal_get_path('module', 'sirius') . '/js/table.js', 'file');
	$event_type_terms = sirius_taxonomy_load('sirius_event_type', 'tid', 'full');
	if (!$event_type_terms) { return 'There are no event types.'; }

	$is_admin = (user_access('sirius admin') || user_access('sirius domain admin') || user_access('sirius event admin'));

	global $user; 
	$contact_nid = 	sirius_contact_nid_get_from_user();

	$rows_enabled = array();
	$rows_disabled = array();
	foreach ($event_type_terms as $event_type_tid => $event_type_term) {
		$enabled = sirius_event_interestlist_enabled_byterm($event_type_term);
		if (!$enabled && !$is_admin) { continue; }
		if ($contact_nid) { $my_signup = sirius_event_interestlist_lookup($event_type_term, $contact_nid); }

		$tools = '';
		$paths = array();
		$paths["taxonomy/term/$event_type_tid/sirius_event_type_interestlist/list"] = 'List';
		$paths["taxonomy/term/$event_type_tid/sirius_event_type_interestlist/worker"] = 'Add Workers';
		$paths["taxonomy/term/$event_type_tid/sirius_event_type_interestlist/settings"] = 'Settings';
		if ($my_signup) { 
			$paths["taxonomy/term/$event_type_tid/sirius_event_type_interestlist/self"] = 'Cancel Sign Up';
		} else {
			$paths["taxonomy/term/$event_type_tid/sirius_event_type_interestlist/self"] = 'Sign Up';
		}

		foreach ($paths as $path => $title) {
			if (drupal_valid_path($path)) {
				$tools .= l($title, $path, ['attributes' => ['class' => ['tool-button']]]);
			}
		}

		$list = sirius_event_interestlist_list($event_type_term);
		$count = count($list);
		$first = reset($list);
		$first_date = '';
		if ($first) { $first_date = date('Y-m-d h:i:s a', $first['interestlist_signup_ts']); }

		$row = array();
		$row[] = $event_type_term->name;
		if ($my_signup) { $row[] = 'Yes'; } else { $row[] = 'No'; }
		if ($is_admin) {
			$row[] = $count;
			$row[] = $first_date . '<br />' . $first['contact_title'];
		}
		$row[] = $tools;

		if ($enabled) { $rows_enabled[] = $row; } else { $rows_disabled[] = $row; }
		$rows[] = $row;
	}

	if ($is_admin) {
		$header = array(
			t('Name'),
			t('Signed Up?'),
			t('Count'),
			t('Last Signup'),
			t('Tools'),
		);
	} else {
		$header = array(
			t('Name'),
			t('Signed Up?'),
			t('Tools'),
		);
	}

	$html = '';
	if ($is_admin) {
		$html .= '<h2>' . t('Enabled Interest Lists') . '</h2>';
	}
	if ($rows_enabled) {
		$html .= theme_table(array('header' => $header, 'rows' => $rows_enabled, 'attributes' => arraY('class' => array('sirius_table_align'))));
	} else {
		$html .= '<p>' . t('There are no enabled interest lists.') . '</p>';
	}

	if ($is_admin) {
		$html .= '<h2>' . t('Disabled Interest Lists') . '</h2>';
		if ($rows_enabled) {
			$html .= theme_table(array('header' => $header, 'rows' => $rows_disabled, 'attributes' => arraY('class' => array('sirius_table_align'))));
		} else {
			$html .= '<p>' . t('There are no disabled interest lists.') . '</p>';
		}
	}

	return $html;
}