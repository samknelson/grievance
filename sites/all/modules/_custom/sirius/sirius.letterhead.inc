<?php

function sirius_letterhead_list($params = array()) {
	$domain_nid = sirius_domain_nid();
	$sql_args = array();
	$sql = "select nid, title ";
	$sql .= "from node ";
	$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_letterhead_format on field_data_field_sirius_letterhead_format.entity_type = 'node' and field_data_field_sirius_letterhead_format.entity_id = node.nid ";
	$sql .= "where type = 'sirius_letterhead' ";
	if ($domain_nid) { 
		$sql .= "and field_sirius_domain_target_id = :domain_nid ";
		$sql_args[':domain_nid'] = $domain_nid;
	}
	if ($params['format']) {
		$sql .= "and field_sirius_letterhead_format_value = :format ";
		$sql_args[':format'] = $params['format'];
	}
	$sql .= "order by title ";
	$stmt = sirius_sql_query($sql, $sql_args);
	$results = array();
	while ($hr = $stmt->fetchAssoc()) {
		$results[$hr['nid']] = $hr['title'];
	}
	return $results;
}

function sirius_node_tab_letterhead_preview($letterhead_node) {
	if ($letterhead_node->field_sirius_letterhead_format['und'][0]['value'] == 'html') {
		return sirius_node_tab_letterhead_preview_html($letterhead_node);
	} else {
		return sirius_node_tab_letterhead_preview_pdf($letterhead_node);
	}
}

function sirius_node_tab_letterhead_preview_html($letterhead_node) {
	$html = $letterhead_node->body['und'][0]['value'];

	// $html = 

	$tokens = array();
	$tokens['sirius']['letterhead']['body'] = sirius_loopback_sample_html();
	$tokens['sirius']['letterhead']['title'] = 'TITLE';
	$html = token_replace($html, $tokens, array('clear' => TRUE));
	print $html;
	drupal_exit();
}

function sirius_node_tab_letterhead_preview_pdf($letterhead_node) {
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
