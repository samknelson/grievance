<?php

function sirius_bookmark_view_form_alter(&$form, &$form_state) {
	if ($form['type']['#options']) {
		$form['type']['#options'] = array('All' => '- Any -') + sirius_bookmark_type_options();
	}
}

function sirius_bookmark_view_pre_render(&$view) {
	$type_options = sirius_bookmark_type_options();
	if ($view->result) {
    foreach ($view->result as $delta => $row) {
      if ($row->node_type) {
        if ($type_options[$row->node_type]) {
        	$row->node_type = $type_options[$row->node_type];
        }
      }
    }
  }
}

/**
 * When you change this, don't forget to also modify the flag (admin/structure/flags/manage/bookmarks) to allow the additional node types to be referenced.
 */

function sirius_bookmark_types() {
	return array('sirius_worker', 'grievance_shop', 'sirius_contact', 'sirius_phonenumber');
}

function sirius_bookmark_type_options() {
	$types = sirius_bookmark_types();
	$options = array();
	foreach ($types as $type) {
		$options[$type] = sirius_content_type_name($type);
	}
	return $options;
}
