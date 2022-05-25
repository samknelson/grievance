<?php

/**
 * Implements hook_sirius_banner_render_html_alter
 */

function sirius_bsh_sirius_banner_render_html_alter(&$html) {
	drupal_add_css(drupal_get_path('module', 'sirius_bsh') . '/css/bsh.css');

	$alias = drupal_get_path_alias();
	if (preg_match('/^public\//', $alias)) { sirius_public_enable(); }

	$classes = grievance_classes_array();

	if (in_array('sirius-public', grievance_classes_array())) {
		$html  = '<section class="full-width lato">';
		$html .= '  <div class="full-width-row bg-white page-head">';
		$html .= '    <div class="page-width shortest-row">';
		$html .= '      <div class="col-6 bg-white col-left asul">';
		$html .= '        <a class="site-name" href="#">TrainHawaii</a>';
		$html .= '      </div>';
		$html .= '      <div class="col-6 bg-white col-right">';
		$html .= '        <a class="button bg-blue border-blue text-white" href="/user">Login</a><a class="button border-blue text-blue bg-white" href="/intake/MAHALO">Register</a>';
		$html .= '      </div>';
		$html .= '    </div>';
		$html .= '  </div>';
		$html .= '</div>';
	}
}
