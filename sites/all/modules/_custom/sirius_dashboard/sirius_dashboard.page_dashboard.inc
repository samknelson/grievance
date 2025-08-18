<?php

function sirius_dashboard_page_dashboard() {
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

	$items = sirius_dashboard_content();
	$html = sirius_dashboard_render($items);

	if ($print_and_exit) {
		print $html;
		exit();
	}

	return $html;
}
