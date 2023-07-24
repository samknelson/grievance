<?php

function sirius_trust_node_tab_worker_election_list($worker_node) {
	drupal_add_js(drupal_get_path('module', 'sirius') . '/js/table.js', 'file');

	$show_inactive = $_REQUEST['show_inactive'];

	$html .= sirius_trust_render_worker_elections($worker_node, $show_inactive);

	if ($show_inactive) {
		$html .= l('Hide inactive elections', current_path(), array('query' => array('show_inactive' => FALSE), 'attributes' => array('class' => array('tool-button'))));
	} else {
		$html .= l('Show inactive elections', current_path(), array('query' => array('show_inactive' => TRUE), 'attributes' => array('class' => array('tool-button'))));
	}

	return $html;
}

function sirius_trust_render_worker_elections($worker_node, $show_inactive) {
  $params = array();
  if (!$show_inactive) { $params['active'] = 'Yes'; }
	$params['worker_nid'] = $worker_node->nid;
	$election_nodes = sirius_trust_worker_elections_search($params);
	if (!$election_nodes) { return '<p>' . t('No worker election records found.') . '</p>'; }

  $rows = array();
  foreach ($election_nodes as $election_nid => $election_node) {
  	$tools = array();
  	$tools[] = l('Election', "node/$election_nid", array('attributes' => array('class' => array('tool-button'))));

  	$benefit_nids = sirius_fieldvals($election_node, 'field_sirius_trust_benefits', 'target_id');
  	$benefit_names = array();
  	foreach ($benefit_nids as $benefit_nid) { $benefit_names[] = sirius_node_title($benefit_nid); }

  	$start_ts = sirius_fieldval($election_node, 'field_sirius_date_start', 'ts');
  	if ($start_ts > 0) { $start_date = date('m/d/Y', $start_ts); } else { $start_date = ''; }

  	$end_ts = sirius_fieldval($election_node, 'field_sirius_date_end', 'ts');
  	if ($end_ts > 0) { $end_date = date('m/d/Y', $end_ts); } else { $end_date = ''; }

  	$employer_name = '';
  	$employer_node = node_load($election_node->field_grievance_shop['und'][0]['target_id']);
  	if ($employer_node) { $employer_name = $employer_node->title; }

  	$row = array();
  	$row[] = join(', ', $benefit_names);
  	$row[] = $employer_name;
  	$row[] = $election_node->field_sirius_active['und'][0]['value'];
  	$row[] = $start_date;
  	$row[] = $end_date;
  	$row[] = join(' ', $tools);

  	if ($election_node->field_sirius_active['und'][0]['value'] == 'Yes') {
	  	$class = 'active';
  	} else {
	  	$class = 'inactive';
  	}

  	$rows[] = array('data' => $row, 'class' => array($class));
  }

  $header = array(
  	'Benefits', 
  	'Employer',
  	'Active',
  	'Start Date',
  	'End Date',
  	'Tools'
  );

  return sirius_table_render($header, $rows, array('class' => array('sirius_table_align')));
}
