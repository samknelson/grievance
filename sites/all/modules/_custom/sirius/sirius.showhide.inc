<?php 

function sirius_showhide_open($target_id, $hidden = FALSE) {
	drupal_add_js(drupal_get_path('module', 'sirius') . '/js/showhide.js', 'file');
	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/showhide.css', 'file');

	$target_id = "sirius_showhide_target_$target_id";

	if ($hidden) {
		$html = "<a href=\"#\" class=\"sirius_showhide_toggle\" data-sirius_showhide_target=\"$target_id\">show</a>";
		$html .= "<div class=\"sirius_showhide_target sirius_showhide_target_hidden\" id=\"$target_id\">";
	} else {
		$html = "<a href=\"#\" class=\"sirius_showhide_toggle\" data-sirius_showhide_target=\"$target_id\">hide</a>";
		$html .= "<div class=\"sirius_showhide_target sirius_showhide_target_visible\" id=\"$target_id\">";
	}

	return $html;
}

function sirius_showhide_close() {
	return '</div>';
}