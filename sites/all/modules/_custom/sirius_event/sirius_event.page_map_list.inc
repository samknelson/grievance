<?php

function sirius_node_tab_event_map_list_access($event_node) {
	return sirius_node_tab_event_map_view_access($event_node);
}

function sirius_node_tab_event_map_list($event_node) {
  sirius_event_disclaimer_redirect($event_node);
	global $user;
	$event_nid = $event_node->nid;
	$event_json = sirius_jsonfield_util_node_json_get($event_node);
	$type_json = sirius_event_type_json($event_node);
	$survey_node = sirius_event_survey_get($event_node, 'map');
	$survey_json = sirius_jsonfield_util_node_json_get($survey_node);
	$survey_form = sirius_jsonfield_util_form_get($survey_json, 'default');

	$active = sirius_map_active($event_node);
	if (!$active['success']) {
		$html = '<div><em>' . t('Map is closed. Admin access only is allowed.') . '</em></div>';
	}
	
	drupal_add_css(drupal_get_path('module', 'sirius_event') . '/css/survey.css', 'file');
	drupal_add_css(drupal_get_path('module', 'sirius_event') . '/css/map_list.css', 'file');
	drupal_add_js(drupal_get_path('module', 'sirius_event') . '/js/map_list.js', array('type' => 'file', 'weight' => -5));
	$js_settings = array();
	$js_settings['event_nid'] = $event_node->nid;
	$js_settings['poll'] = array('active' => TRUE, 'timeout' => 1000 * 60 * 3);
	drupal_add_js(array('sirius_map' => $js_settings), 'setting');

	$prole_tid = $type_json['map']['prole_tid'];
	$pstatus_options = sirius_event_participant_role_status_tids($prole_tid, array());
	$pstatus_terms = taxonomy_term_load_multiple(array_keys($pstatus_options));
	$counts = sirius_event_participant_summary($event_nid, $prole_tid);
	$rows = array();

	$html .= '<div class="map_list_items">';
	foreach ($pstatus_terms as $pstatus_tid => $pstatus_term) {
		if (!$counts[$pstatus_tid]) { continue; }
		$pstatus_json = sirius_jsonfield_util_term_json_get($pstatus_term);
		$pin = sirius_map_pin($pstatus_json['map']['pin']);

		$class = "map_list_item map_legend_item";

		$chunk = '';
		$chunk .= "<div class=\"$class\">";
		$chunk .= '<div class="icon_wrap">';
		$chunk .= sirius_map_pin_render($pin);
		$chunk .= '</div>';
		$chunk .= '<div class="body_wrap">';
		$chunk .= $pstatus_term->name;
		// $chunk .= ' (' . number_format($counts[$pstatus_tid], 0) . ')';
		$chunk .= '</div>';
		$chunk .= '</div>';

		$html .= l($chunk, '/', array('html' => TRUE, 'attributes' => array('data-pstatus_tid' => $pstatus_tid, 'class' => array('map_list_toggle'))));
	}
	$html .= '</div>';
	$html .= '<br clear="all" />';
	$html .= '<hr>';

	$map_features = sirius_event_map_list($event_node, array('disable_group_by_address' => TRUE));

	$html .= '<div class="map_list_items">';
	foreach ($map_features as $map_feature) {
		if ($map_feature['sirius']['type'] != 'contact') { continue; }
		$html .= $map_feature['sirius']['list_html'];
	}
	$html .= '</div>';

	$html .= '<br clear="all" />';
	$html .= l('Legend', 'sirius/modal/event/map-legend/' . $event_node->nid, array('attributes' => array('class' => array('tool-button'))));
	$html .= '<a href="/sirius/map" id="sirius_map_poll" class="tool-button">Refresh Now</a>';
	$html .= '<div id="sirius_map_poll_message"></div>';

	return $html;
}

function sirius_map_list_html($event_nid, $contact_nid, $contact_name, $thoroughfare, $premise, $locality, $administrative_area, $postal_code, $marker_id, $pstatus_name, $pstatus_tid, $icon_url) {
	$chunk = '';
	$chunk .= "<div class=\"map_list_item\">";
	$chunk .= '<div class="icon_wrap">';
	$chunk .= '<img src="' . $icon_url . '">';
	$chunk .= '</div>';
	$chunk .= '<div class="body_wrap">';
	$chunk .= $contact_name;
	$chunk .= '<br />';
	$chunk .= $thoroughfare;
	if ($premise) { $chunk .= ' ' . $premise; }
	if ($locality) { 
		$chunk .= '<br />' . $locality . ', ' . $administrative_area . ' ' . $postal_code; 
	}
	$chunk .= '</div>';
	$chunk .= '</div>';

	$html .= l($chunk, 'sirius/modal/event/map/' . $event_nid . '/' . $marker_id, 
		array('html' => TRUE, 'attributes' => array('id' => 'marker_' . $marker_id, 'class' => array('pstatus_' . $pstatus_tid))));
	return $html;
}