<?php

function sirius_letterhead_list() {
	$sql = "select nid, title from node where type = 'sirius_letterhead' order by title ";
	$stmt = db_query($sql, array());
	$results = array();
	while ($hr = $stmt->fetchAssoc()) {
		$results[$hr['nid']] = $hr['title'];
	}
	return $results;
}

function sirius_node_tab_letterhead_preview($letterhead_node) {
	global $base_url;
	$url = $base_url;
	$url .= '/sirius/letterhead/sample-html/';
	$url .= $letterhead_node->nid;

	sirius_pdf_stream($url, array('letterhead' => $letterhead_node->nid));
}

/***
 * Implements hook_sirius_jsonfield_form_definitions()
 */

function sirius_letterhead_sirius_jsonfield_form_definitions() {
	$form['page'] = array(
		'#type' => 'fieldset',
		'#title' => 'Page',
	);
	$form['page']['margin_left'] = array(
		'#type' => 'textfield',
		'#title' => 'Left Margin',
	);
	$form['page']['margin_right'] = array(
		'#type' => 'textfield',
		'#title' => 'Right Margin',
	);
	$form['page']['margin_top'] = array(
		'#type' => 'textfield',
		'#title' => 'Top Margin',
	);
	$form['page']['margin_bottom'] = array(
		'#type' => 'textfield',
		'#title' => 'Bottom Margin',
	);

	$definitions['node']['sirius_letterhead']['field_sirius_json'] = $form;

	return $definitions;
}

function sirius_letterhead_preview_loopback($letterhead_node) {
	return sirius_pdf_loopback(sirius_loopback_sample_html(), $letterhead_node);
}


function sirius_node_access_letterhead($node, $op, $account) {
	if ($op == 'view') { return user_access('sirius letterhead use'); }
	return user_access('sirius letterhead edit');
}
