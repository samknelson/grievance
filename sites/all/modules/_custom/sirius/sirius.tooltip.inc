<?php

function sirius_tooltip_render($tooltip_content, $trigger_content = '') {
	if (!$tooltip_content) { return $trigger_content; }

	$init = &drupal_static(__FUNCTION__);
	if (!isset($init)) {
		$init = TRUE;
		drupal_add_library('system', 'ui.tooltip');
		drupal_add_js(drupal_get_path('module', 'sirius') . '/js/tooltip.js');
		drupal_add_css(drupal_get_path('module', 'sirius') . '/css/tooltip.css');
	}

	$html  = '<span class="sirius-tooltip" title="Tooltip">';
	$html .= '  <span class="sirius-tooltip-trigger">';
	$html .=      $trigger_content;
	if (module_exists('fontawesome')) {
		$html .=      '<span class="sirius-tooltip-info-icon sirius-tooltip-info-icon-awesome">';
		$html .=      	'<i class="fas fa-info-circle"></i>';
		$html .= 			'</span>';
	} else {
		$html .=      '<span class="sirius-tooltip-info-icon sirius-tooltip-info-icon-html">';
		$html .=      	'&#128712;';
		$html .= 			'</span>';
	}
	$html .= '  </span>';
	$html .= '  <span class="sirius-tooltip-content">'; 
	$html .=      $tooltip_content;
	$html .= '  </span>';
	$html .= '</span>';

	return $html;
}