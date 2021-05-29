<?php

function sirius_autorefresh_render($js_settings) {
	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/autorefresh.css', 'file');
 	drupal_add_js(array('sirius_autoreresh' => $js_settings), 'setting');
  drupal_add_js(drupal_get_path('module', 'sirius') . '/js/autorefresh.js', 'file');
  $html = '<div class="sirius_autorefresh_wrapper">';
  $html .= '<span id="sirius_autorefresh_msg">&nbsp;</span>';
  $html .= '<a href="#" id="sirius_autorefresh_link_refreshnow">Refresh Now</a>';
  $html .= '</div>';
  return $html;
}