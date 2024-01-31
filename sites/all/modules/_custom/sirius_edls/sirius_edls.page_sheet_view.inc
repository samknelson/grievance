<?php

function sirius_edls_node_tab_sheet_view_access($sheet_node) {
  return sirius_access_instance('edls')->access('sheet_view', array('sheet_node' => $sheet_node));
}

function sirius_edls_node_tab_sheet_view($sheet_node) {
	// module_load_include('inc', 'diff', 'diff.pages');
	//  return diff_diffs_overview($sheet_node);
	$base = 'node/' . $sheet_node->nid . '/sirius_edls_sheet_view';
	$crews = sirius_edls_crews($sheet_node, 'uuid');

  $links = array();
  $links[] = array(
	  'path' => $base,
	  'title' => t('All Crews'),
  );
  foreach ($crews as $crew_uuid => $crew) {
		$links[] = array(
			'path' => "$base/$crew_uuid",
			'title' => sirius_edls_crew_title($crew),
		);
  }
  $html = sirius_quicknav($links);

  $options = array();
  $options['crew_uuid'] = sirius_node_tab_arg(0);
	$html .= sirius_edls_sheet_render($sheet_node, $options);
	return $html;
}