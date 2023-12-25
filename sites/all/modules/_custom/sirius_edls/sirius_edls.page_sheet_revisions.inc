<?php

function sirius_edls_node_tab_sheet_revisions_access($sheet_node) {
  return sirius_access_instance('edls')->access('sheet_revisions', array('sheet_node' => $sheet_node));
}

function sirius_edls_node_tab_sheet_revisions($sheet_node) {
	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/ux.css');


  $vid = $_REQUEST['vid'];
  if ($vid) { return sirius_edls_node_tab_sheet_revisions_view($sheet_node, $vid); }

  $revisions = node_revision_list($sheet_node);

  $rows = array();
  foreach ($revisions as $revision) {
  	$row = array();
  	$row[] = sirius_sheet_render_version_impl($sheet_node->nid, $revision->vid);
		$row[] = sirius_edls_node_tab_sheet_revisions_date($revision->timestamp);
  	$row[] = $revision->name;
  	$row[] = l('View', current_path(), array('query' => array('vid' => $revision->vid), 'attributes' => array('class' => array('tool-button'))));
  	$rows[] = $row;
  }

  $header = array();
  $header[] = t('Version');
  $header[] = t('Date');
  $header[] = t('Author');
  $header[] = t('Tools');

  return sirius_table_render($header, $rows);
}

function sirius_edls_node_tab_sheet_revisions_date($ts) {
	$html = date('m/d/Y h:i a', $ts);
	$interval = time() - $ts;
	$html .= ' (' . format_interval($interval, 2) . ' ago)';
	return $html;
}

function sirius_edls_node_tab_sheet_revisions_view($sheet_node, $vid) {
	$old_node = node_load($sheet_node->nid, $vid);
	if (!$old_node) { return sirius_page_error('Invalid vid.'); }

  $revisions = node_revision_list($sheet_node);
  module_load_include('inc', 'diff', 'diff.pages');
  $next_vid = _diff_get_next_vid($revisions, $vid);
  $prev_vid = _diff_get_previous_vid($revisions, $vid);


	$tools = array();
	if ($prev_vid) {
		$tools[] = l(
			'<i class="fas fa-arrow-left"></i> Prev', 
			current_path(), 
			array(
				'query' => array('vid' => $prev_vid), 
				'html' => TRUE, 
				'attributes' => array('class' => array('tool-button'))
			)
		);
	}
	$tools[] = l('List', current_path(), array('attributes' => array('class' => array('tool-button'))));
	if ($next_vid) {
		$tools[] = l(
			'Next <i class="fas fa-arrow-right"></i>', 
			current_path(), 
			array(
				'query' => array('vid' => $next_vid), 
				'html' => TRUE, 
				'attributes' => array('class' => array('tool-button'))
			)
		);
	}

	$author = user_load($old_node->revision_uid);

	$html = '<div class="sirius_callout_fullwidth">';
	$html .= sirius_fakerender_field('Version:', sirius_sheet_render_version($old_node));
	$html .= sirius_fakerender_field('Date:', sirius_edls_node_tab_sheet_revisions_date($old_node->revision_timestamp));
	if ($author) { $html .= sirius_fakerender_field('Author:', $author->name); }
	$html .= '<br>';
	$html .= join('', $tools);
	$html .= '</div>';

	$options = array();
	$options['fetch_workers_from_json'] = TRUE;
	$options['hide_version'] = TRUE;
	$options['hide_count'] = TRUE;
	$html .= sirius_edls_sheet_render($old_node, $options);
	return $html;
}