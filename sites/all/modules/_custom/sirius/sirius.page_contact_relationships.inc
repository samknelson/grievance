<?php

function sirius_node_tab_contact_relationships($contact_node) {
	drupal_add_js(drupal_get_path('module', 'sirius') . '/js/table.js', 'file');

	$show_inactive = $_REQUEST['show_inactive'];

	$html .= '<h2>' . t('Relationships FROM ') . $contact_node->title . '</h2>';
	$html .= sirius_contact_render_relationships($contact_node, 'from', $show_inactive);
	$html .= '<h2>' . t('Relationships TO ') . $contact_node->title . '</h2>';
	$html .= sirius_contact_render_relationships($contact_node, 'to', $show_inactive);

	if ($show_inactive) {
		$html .= l('Hide inactive relationships', current_path(), array('query' => array('show_inactive' => FALSE), 'attributes' => array('class' => array('tool-button'))));
	} else {
		$html .= l('Show inactive relationships', current_path(), array('query' => array('show_inactive' => TRUE), 'attributes' => array('class' => array('tool-button'))));
	}

	return $html;
}

function sirius_contact_render_relationships($contact_node, $direction, $show_inactive = FALSE) {
	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/contacts.css');

  $params = array();
  if (!$show_inactive) { $params['active'] = 'Yes'; }
  if ($direction == 'from') {
  	$params['contact_nid'] = $contact_node->nid;
  } else {
  	$params['contact_alt_nid'] = $contact_node->nid;
  }

	$relationship_nodes = sirius_contact_relationships($params);
	if (!$relationship_nodes) { return '<p>' . t('There are no relationships from this contact.') . '</p>'; }

	static $relationship_type_terms;
	if (!isset($relationship_type_terms)) { $relationship_type_terms = sirius_taxonomy_load('sirius_contact_relationship_types', 'tid', 'full'); }

  $rows = array();
  foreach ($relationship_nodes as $relationship_nid => $relationship_node) {
  	if ($direction == 'from') {
  		$other_contact_nid = $relationship_node->field_sirius_contact_alt['und'][0]['target_id'];
  	} else {
  		$other_contact_nid = $relationship_node->field_sirius_contact['und'][0]['target_id'];
  	}
  	$other_contact_node = node_load($other_contact_nid);
  	$other_worker_nid = sirius_worker_get_by_contact_nid($other_contact_nid, TRUE);

  	$tools = array();
  	$tools[] = l('Relationship', "node/$relationship_nid", array('attributes' => array('class' => array('tool-button'))));
  	if ($other_worker_nid) {
  		$tools[] = l('Worker', "node/$other_worker_nid/sirius_worker_relationships", array('attributes' => array('class' => array('tool-button'))));
  	} else {
  		$tools[] = l('Contact', "node/$other_contact_nid/sirius_contact_relationships", array('attributes' => array('class' => array('tool-button'))));
  	}

  	$relationship_type_term = $relationship_type_terms[$relationship_node->field_sirius_contact_reltype['und'][0]['tid']];
  	$relationship_name = '';
  	if ($direction == 'from') {
  		$relationship_name = $relationship_type_term->name . ' to';
  	} else {
  		$relationship_name = $relationship_type_term->field_sirius_name_alt['und'][0]['value'] . ' of';
  	}

  	$start_ts = sirius_fieldval($relationship_node, 'field_sirius_date_start', 'ts');
  	if ($start_ts > 0) { $start_date = gmdate('m/d/Y', $start_ts); } else { $start_date = ''; }

  	$end_ts = sirius_fieldval($relationship_node, 'field_sirius_date_end', 'ts');
  	if ($end_ts > 0) { $end_date = gmdate('m/d/Y', $end_ts); } else { $end_date = ''; }

  	$row = array();
  	$row[] = $relationship_name;
  	$row[] = $other_contact_node->title;
    $row[] = $relationship_node->field_sirius_count['und'][0]['value'];
    $row[] = $relationship_node->field_sirius_active['und'][0]['value'];
  	$row[] = $start_date;
  	$row[] = $end_date;
  	$row[] = join(' ', $tools);

  	if ($relationship_node->field_sirius_active['und'][0]['value'] == 'Yes') {
	  	$class = 'active';
  	} else {
	  	$class = 'inactive';
  	}

  	$rows[] = array('data' => $row, 'class' => array($class));
  }

  $header = array(
  	t('Relationship'), 
  	t('Contact'),
    t('Sequence'),
  	t('Active'),
  	t('Start Date'),
  	t('End Date'),
  	t('Tools'),
  );

  return theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('sirius_table_align', 'sirius_contact_relationships'))));
}