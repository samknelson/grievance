<?php

/**
 * Implements hook_views_pre_render()
 * 
 */

function sirius_smf_views_pre_render(&$view) {
	drupal_add_css(drupal_get_path('module', 'sirius_smf') . '/css/views.css', 'file');
	
	$settings = explode("\n", variable_get('sirius_views_taxonomy_restrict_by_parent'));

	foreach ($settings as $setting) {
		list ($view_name, $field_name, $tid) = explode('|', $setting);
		$view_name = trim($view_name);
		$field_name = trim($field_name);
		$tid = trim($tid);

		if ($view->name == $view_name) {
			sirius_smf_views_taxonomy_restrict_by_parent_impl($view, $field_name, $tid);
		}
	}
}

function sirius_smf_views_taxonomy_restrict_by_parent_impl(&$view, $field_name, $tid) {
	if (!$view->result) { return; }

	$lookups = &drupal_static(__FUNCTION__);
  if (!isset($lookups)) { $lookups = array(); }

  if (!isset($lookups[$tid])) {
  	$children =  sirius_smf_views_taxonomy_restrict_by_parent_get_all_children($tid);
  	$children[$tid] = taxonomy_term_load($tid);
  	$lookups[$tid] = array_keys($children);
  }

	$field_name = "field_$field_name";

	foreach ($view->result as $row_delta => $row) {
		if (!$row->{$field_name}) { continue; }

		foreach ($row->{$field_name} as $tag_delta => $tag_data) {
			if (!in_array($tag_data['raw']['tid'], $lookups[$tid])) {
				unset($view->result[$row_delta]->{$field_name}[$tag_delta]);
			}
		}
	}
}


function sirius_smf_views_taxonomy_restrict_by_parent_get_all_children($tid, $vid = 0, $key = 'tid'){
	$c = taxonomy_get_children($tid, $vid, $key);
	$result = array();
	foreach ($c as $t => $d){
		$result[$t] = $d;
		$below = sirius_smf_views_taxonomy_restrict_by_parent_get_all_children($t, $vid, $key);
		if (!empty($below)) {
			foreach ($below as $nt => $nd){
				$result[$nt] = $nd;
			}
		}
	}
	return $result;
}