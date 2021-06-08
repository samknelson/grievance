<?php

function sirius_dashboard_page_dashboard() {
  drupal_add_css(drupal_get_path('module', 'sirius_dashboard') . '/css/dashboard.css', 'file');

	$html .= '<div class="sirius_dashboard_wrapper" id="sirius_dashboard_content">';
	$html .= sirius_dashboard_page_dashboard_content(FALSE);
	$html .= '</div>';

  $js_settings = array();
  $js_settings['html_id'] = 'sirius_dashboard_content';
  $js_settings['url'] = '/sirius/dashboard/content';
  $js_settings['interval'] = 60;
  sirius_require('sirius', 'sirius.autorefresh.inc');
  $html .= sirius_autorefresh_render($js_settings);

	$path = current_path() . '/items_by_role';
	if (drupal_valid_path($path)) {
		$html .= l('Manage Dashboard Items', $path, array('attributes' => array('class' => array('tool-button'))));
	}

	return $html;
}

function sirius_dashboard_page_dashboard_content($print_and_exit = TRUE) {
	global $user;
	$rows = array();
	$roles = $user->roles;
	if (!$roles) { $roles = array(); }
	foreach ($roles as $rid => $role) {
		$json = sirius_dashboard_items_by_role($rid);
		if (!$json['item_list']) { continue; }
		foreach ($json['item_list'] as $uuid => $item) {
			$new_rows = sirius_dashboard_item_data($json, $uuid);
			if (is_array($new_rows)) {
				foreach ($new_rows as $row_assoc) {
					$priority = $item['priority'];
					if (isset($row_assoc['priority'])) { $priority = $row_assoc['priority']; }

					$title = $item['title'];
					if (isset($row_assoc['title'])) { $title = $row_assoc['title']; }
					
					$row = array();
					$row[] = $item['title'];
					$row[] = sirius_dashboard_priority_render($priority);
					$row[] = $row_assoc['count'];
					$row[] = $row_assoc['tools'];
					$row[] = $row_assoc['details'];
					$rows[] = $row;
				}
			}
		}
	}

	$header = array(
		'Item Type',
		'Priority',
		'Count',
		'Tools',
		'Comments',
	);

	$html = theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array()));

	if ($print_and_exit) {
		print $html;
		exit();
	}

	return $html;
}