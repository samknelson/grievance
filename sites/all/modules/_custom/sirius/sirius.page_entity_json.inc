<?php

function sirius_node_tab_entity_json_access($entity) {
  $json = sirius_jsonfield_util_node_json_get($entity);
  if (!$json) { return FALSE; }
  return TRUE;
}

function sirius_node_tab_entity_json($entity) {
  $json = sirius_jsonfield_util_node_json_get($entity);
  $html_id = 'sirius_jqtree_' . $entity->nid;
	drupal_add_js('sites/all/libraries/jquery_json_viewer/json-viewer/jquery.json-viewer.js');
	drupal_add_css('sites/all/libraries/jquery_json_viewer/json-viewer/jquery.json-viewer.css');
	drupal_add_js(drupal_get_path('module', 'sirius') . '/js/json_viewer.js');
	drupal_add_js(
		array(
			'json_viewer' => array(
				'options' => array(
					'collapsed' => TRUE, 
					'rootCollapsable' => FALSE					
				),
				'data' => array(
					$html_id => $json
				),
			)
		), 
		array('type' => 'setting')
	);

	$html = 'The data attached to this entity is rendered below.';
	$html .= "<div id=\"$html_id\">Loading JSON data ...</div>";

	return $html;
}