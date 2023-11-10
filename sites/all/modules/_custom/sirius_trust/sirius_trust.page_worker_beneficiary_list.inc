<?php

function sirius_trust_node_tab_worker_beneficiary_list($worker_node) {
	drupal_add_js(drupal_get_path('module', 'sirius') . '/js/table.js', 'file');
	$show_inactive = $_REQUEST['show_inactive'];
	$show_inactive = TRUE;
	$html .= sirius_trust_render_worker_beneficiaries($worker_node, $show_inactive);

	/*
	if ($show_inactive) {
		$html .= l('Hide inactive beneficiaries', current_path(), array('query' => array('show_inactive' => FALSE), 'attributes' => array('class' => array('tool-button'))));
	} else {
		$html .= l('Show inactive beneficiaries', current_path(), array('query' => array('show_inactive' => TRUE), 'attributes' => array('class' => array('tool-button'))));
	}
	*/

	return $html;
}

function sirius_trust_render_worker_beneficiaries($worker_node, $show_inactive) {
  $params = array();
  if (!$show_inactive) { $params['active'] = 'Yes'; }
	$params['worker_nid'] = $worker_node->nid;
	$beneficiary_nodes = sirius_trust_worker_beneficiary_search($params);
	if (!$beneficiary_nodes) { return '<p>' . t('No worker beneficiary records found.') . '</p>'; }

  $rows = array();
  foreach ($beneficiary_nodes as $beneficiary_nid => $beneficiary_node) {
  	$tools = array();
  	$tools[] = l('Beneficiary', "node/$beneficiary_nid", array('attributes' => array('class' => array('tool-button'))));

  	$start_ts = sirius_fieldval($beneficiary_node, 'field_sirius_date_start', 'ts');
  	if ($start_ts > 0) { $start_date = gmdate('m/d/Y', $start_ts); } else { $start_date = ''; }

  	$end_ts = sirius_fieldval($beneficiary_node, 'field_sirius_date_end', 'ts');
  	if ($end_ts > 0) { $end_date = gmdate('m/d/Y', $end_ts); } else { $end_date = ''; }

  	$row = array();
  	$row[] = $beneficiary_node->field_sirius_active['und'][0]['value'];
  	$row[] = $start_date;
  	$row[] = $end_date;
  	$row[] = join(' ', $tools);

  	if ($beneficiary_node->field_sirius_active['und'][0]['value'] == 'Yes') {
	  	$class = 'active';
  	} else {
	  	$class = 'inactive';
  	}

  	$rows[] = array('data' => $row, 'class' => array($class));
  }

  $header = array(
  	'Active',
  	'Start Date',
  	'End Date',
  	'Tools'
  );

  return sirius_table_render($header, $rows, array('class' => array('sirius_table_align')));
}
