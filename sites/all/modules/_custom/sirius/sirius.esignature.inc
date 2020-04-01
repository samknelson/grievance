<?php

function sirius_esignature_init() {
	drupal_add_js('sites/all/modules/jquery_update/replace/ui/ui/minified/jquery-ui.min.js');
	drupal_add_js('sites/all/libraries/jquery_signature/js/jquery.touch-punch.min.js');
	drupal_add_js('sites/all/libraries/jquery_signature/js/jquery.signature.js');
	drupal_add_js(drupal_get_path('module', 'sirius') . '/js/sirius.esignature.js');

	drupal_add_css('sites/all/libraries/jquery_signature/css/jquery.signature.css');
	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/sirius.esignature.css');

	$settings_for_js = array();
	// $settings_for_js['fields'] = $fields;
	drupal_add_js(array('esignature' => $settings_for_js), 'setting');
}

/**
 * Stream an esignature image. 
 * @todo: Access control
 */

function sirius_esignature_stream($nid, $which) {
	$node = node_load($nid);
	$json = sirius_jsonfield_util_node_json_get($node);
	$esignature = sirius_jsonfield_util_esignature_get($json, $which);
	$graphic = $esignature['graphic'];
	$graphic = preg_replace('/^data\:image\/png\;base64/', '', $graphic);
	header("Content-type: image/png");
	print base64_decode($graphic);
	exit();
}

function sirius_esignature_render($esignature, $format, $nid, $which) {
	$html .= '<div class="sirius_esignature_render">';
	if ($format == 'full') {
		$html .= $esignature['statement'];
		$html .= "<img src=\"/sirius/esignature/stream/$nid/$which\" class=\"sirius_esignature_render_img_full\" />";
		$html .= '<br />';
		$html .= 'IP: ' . $esignature['ip'] . '<br />';
		$html .= 'Date: ' . $esignature['date_str'] . '<br />';
		$html .= 'User: ' . $esignature['user_uid'] . ' / ' . $esignature['user_name'] . ' / ' . $esignature['user_mail'] . '<br />';
	} else {
		$html .= "<img src=\"/sirius/esignature/stream/$nid/$which\" class=\"sirius_esignature_render_img\" />";
		$html .= '<div class="sirius_esignature_render_date">';
		$html .= $esignature['date_str'];
		$html .= '</div>';
	}
	$html .= '</div>';
	return $html;
}

function sirius_esignature_form_prepare(&$form, $field_name, $title, $statement) {
	$suffix = "<div class=\"sirius_esignature\">";
	$suffix .= "<div class=\"sirius_esignature_statement\">";
	$suffix .= $statement;
	$suffix .= '</div>';
	$suffix .= '<h2>Signature</h2>';
	$suffix .= "<div class=\"sirius_esignature_pane_wrap\">";
	$suffix .= "<div class=\"sirius_esignature_pane\" id=\"sirius_esig\">";
	$suffix .= "</div>";
	$suffix .= "</div>";
	$suffix .= "<button id=\"sirius_esig_clear\">Clear</button>";
	$suffix .= "</div>";

	$form[$field_name]['#type'] = 'textarea';
	$form[$field_name]['#suffix'] = $suffix;
	$form[$field_name]['#id'] = "sirius_esig_data";
	$form[$field_name]['#required'] = TRUE;
	$form[$field_name]['#title'] = $title;

	$form[$field_name . '_statement'] = array();
	$form[$field_name . '_statement']['#type'] = 'value';
	$form[$field_name . '_statement']['#value'] = $statement;

	$form[$field_name . '_submit'] = array();
	$form[$field_name . '_submit']['#type'] = 'submit';
	$form[$field_name . '_submit']['#value'] = t('Submit');
	$form[$field_name . '_submit']['#id'] = "sirius_esig_submit";
}

function sirius_esignature_form_get_json($form, &$form_state, $field_name) {
	$json = array();
	$json['statement'] = $form_state['values'][$field_name . '_statement'];
	$json['graphic'] = $form_state['values'][$field_name];
	$json['ip'] = sirius_loopback_remote_ip();
	$ts = time();
	$json['date_ts'] = $ts;
	$json['date_str'] = date('c', $ts);
	global $user;
	if ($user) {
		$json['user_uid'] = $user->uid;
		$json['user_name'] = $user->name;
		$json['user_mail'] = $user->mail;
	}
	return $json;
}
