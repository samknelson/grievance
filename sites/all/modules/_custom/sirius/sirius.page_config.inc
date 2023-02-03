<?php

function sirius_page_config($show_category_name = NULL) {
	$links = sirius_config_links();
	$html .= '<table>';

	$html .= '<tr>';
	$html .= '<th>' . t('Feature') . '</th>';
	// $html .= '<th>' . t('Description') . '</th>';
	$html .= '<th colspan="2">' . t('Tools') . '</th>';
	$html .= '</tr>';

	foreach ($links as $category_name => $category) {
		if ($show_category_name) {
			if ($show_category_name != $category_name) { continue; }
		}
		$category_html = '';
		foreach ($category['links'] as $path => $link) {
			if (drupal_valid_path($path)) {
				// Get the local tasks for this item. 
				// Copied from menu_local_tasks(), which really ought to not require a raw db query.
				$sql = "select path, title from menu_router ";
				$sql .= "where tab_root = :path ";
				$sql .= "and type = " . MENU_LOCAL_TASK . ' ';
				$sql .= "order by number_parts, weight, title ";
				$stmt = sirius_sql_query($sql, array(':path' => $path));
				while ($hr = $stmt->fetchAssoc()) { 
					if (drupal_valid_path($hr['path'])) {
						$category['links'][$path]['sub_paths'][$hr['path']] = array('title' => $hr['title']);
					}
				}

				$category_html .= '<tr>';
				$category_html .= '<td>' . sirius_tooltip_render($link['description'], $link['title']) . '</td>';
				// $category_html .= '<td>' . $link['description'] . '</td>';
				$category_html .= '<td>' . l('Manage', $path, array('attributes' => array('class' => array('tool-button')))) . '</td>';
				$category_html .= '<td>';
				if ($category['links'][$path]['sub_paths']) {
					foreach ($category['links'][$path]['sub_paths'] as $sub_path => $sub_category) {
						$sub_title = $sub_category['title'];
						$category_html .= l($sub_title, $sub_path, array('attributes' => array('class' => array('tool-button'))));
					}
				}
				$category_html .= '</td>';
				$category_html .= '</tr>';
			}
		}

		if ($category_html) {
			if (!$show_category_name) {
				$html .= '<tr>';
				$html .= '<td colspan="4"><h2>' . $category['title'] . '</h2></td>';
				$html .= '</tr>';
			}
			$html .= $category_html;
		}
	}

	$html .= '</table>';
	return $html;
}
