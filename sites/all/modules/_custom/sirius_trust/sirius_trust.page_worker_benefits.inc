<?php

function sirius_trust_node_tab_worker_benefits($worker_node) {
	drupal_add_js(drupal_get_path('module', 'sirius') . '/js/table.js', 'file');

	$show_inactive = $_REQUEST['show_inactive'];

	$active_benefits_received = sirius_trust_fetch_worker_benefits($worker_node, 'direct', array('active' => 'Yes'));
	$all_benefits_received = sirius_trust_fetch_worker_benefits($worker_node, 'direct');
	$active_benefits_provided = sirius_trust_fetch_worker_benefits($worker_node, 'dependent', array('active' => 'Yes'));
	$all_benefits_provided = sirius_trust_fetch_worker_benefits($worker_node, 'dependent');

	$html .= '<h2>' . t('Benefits Received') . '</h2>';
	if ($show_inactive) {
		$html .= sirius_trust_render_worker_benefits($worker_node, 'direct', $all_benefits_received, array());
	} else {
		$html .= sirius_trust_render_worker_benefits($worker_node, 'direct', $active_benefits_received, array());
		$count_inactive = count($all_benefits_received) - count($active_benefits_received);
		if ($count_inactive > 0) {
			$html .= '<p>' . t('(Not showing <strong>@count_inactive</strong> inactive records.)', array('@count_inactive' => $count_inactive)) . '</p>';
			$show_button = TRUE;
		}
	}

	$html .= '<h2>' . t('Dependent Benefits') . '</h2>';
	if ($show_inactive) {
		$html .= sirius_trust_render_worker_benefits($worker_node, 'direct', $all_benefits_provided, array('hide_header' => TRUE));
	} else {
		$html .= sirius_trust_render_worker_benefits($worker_node, 'direct', $active_benefits_provided, array('hide_header' => TRUE));
		$count_inactive = count($all_benefits_provided) - count($active_benefits_provided);
		if ($count_inactive > 0) {
			$html .= '<p>' . t('(Not showing <strong>@count_inactive</strong> inactive records.)', array('@count_inactive' => $count_inactive)) . '</p>';
			$show_button = TRUE;
		}
	}

	if ($show_inactive) {
		$html .= l('Hide inactive records', current_path(), array('query' => array('show_inactive' => FALSE), 'attributes' => array('class' => array('tool-button'))));
	} else if ($show_button) {
		$html .= l('Show inactive records', current_path(), array('query' => array('show_inactive' => TRUE), 'attributes' => array('class' => array('tool-button'))));
	}

	return $html;
}

function sirius_trust_fetch_worker_benefits($worker_node, $direction, $options = array()) {
  $params = array();
  $params['active'] = $options['active'];
  if ($direction == 'direct') {
  	$params['worker_nid'] = $worker_node->nid;
  } else {
	  $relationship_params = array();
  	$relationship_params['contact_nid'] = $worker_node->field_sirius_contact['und'][0]['target_id'];
  	$relationship_params['nids_only'] = TRUE;
		$relationship_nids = sirius_contact_relationships($relationship_params);
		if (!$relationship_nids) { return array(); }
		$params['relationship_nids'] = $relationship_nids;
  }

	return sirius_trust_worker_benefits_search($params);
}

function sirius_trust_render_worker_benefits($worker_node, $direction, $wb_nodes, $options) {
	if (!$wb_nodes) {
		return '<p>' . t('No records found.') . '</p>';
	}

	static $relationship_type_terms;
	if (!isset($relationship_type_terms)) { $relationship_type_terms = sirius_taxonomy_load('sirius_contact_relationship_types', 'tid', 'full'); }

  foreach ($wb_nodes as $wb_nid => $wb_node) {
  	$benefit_nid = $wb_node->field_sirius_trust_benefit['und'][0]['target_id'];
  	$benefit_node = node_load($benefit_nid);

  	$beneficiary_worker_node = node_load($wb_node->field_sirius_worker['und'][0]['target_id']);
  	$beneficiary_name = $beneficiary_worker_node->title;

  	$relationship_node = node_load($wb_node->field_sirius_contact_relation['und'][0]['target_id']);

  	$subscriber_worker_node = NULL;
  	$relationship_type_name = '';
  	if ($relationship_node) {
  		$subscriber_contact_nid = $relationship_node->field_sirius_contact['und'][0]['target_id'];
			$subscriber_worker_node = sirius_worker_get_by_contact_nid($subscriber_contact_nid);
			$relationship_type_tid = $relationship_node->field_sirius_contact_reltype['und'][0]['tid'];
			$relationship_type_term = $relationship_type_terms[$relationship_type_tid];
			if ($relationship_type_term) {
				$relationship_type_name = $relationship_type_term->name . ' to';
			}
  	} 

  	if ($subscriber_worker_node) {
  		$subscriber_name = $subscriber_worker_node->title;
  	} else {
  		$subscriber_name = $beneficiary_worker_node->title;
  		$relationship_type_name = '[self]';
  	}

  	$tools = array();
  	$tools[] = l('Benefit', "node/$wb_nid", array('attributes' => array('class' => array('tool-button'))));
  	if ($subscriber_worker_node) {
	  	$tools[] = l('Subscriber', "node/" . $subscriber_worker_node->nid . '/sirius_trust_worker_benefits', array('attributes' => array('class' => array('tool-button'))));
	  	$tools[] = l('Dependent', "node/" . $beneficiary_worker_node->nid . '/sirius_trust_worker_benefits', array('attributes' => array('class' => array('tool-button'))));
	  } else {
	  	$tools[] = l('Subscriber', "node/" . $beneficiary_worker_node->nid . '/sirius_trust_worker_benefits', array('attributes' => array('class' => array('tool-button'))));
	  }

  	$relationship_type_term = $relationship_type_terms[$wb_node->field_sirius_contact_reltype['und'][0]['tid']];
  	$relationship_name = '';
  	if ($direction == 'from') {
  		$relationship_name = $relationship_type_term->name . ' to';
  	} else {
  		$relationship_name = $relationship_type_term->field_sirius_name_alt['und'][0]['value'] . ' of';
  	}

  	$start_ts = sirius_fieldval($wb_node, 'field_sirius_date_start', 'ts');
  	if ($start_ts > 0) { $start_date = gmdate('m/d/Y', $start_ts); } else { $start_date = '(no start date)'; }

  	$end_ts = sirius_fieldval($wb_node, 'field_sirius_date_end', 'ts');
  	if ($end_ts > 0) { $end_date = gmdate('m/d/Y', $end_ts); } else { $end_date = '(no end date)'; }

  	$employer_name = '';
  	$employer_node = node_load($wb_node->field_grievance_shop['und'][0]['target_id']);
  	if ($employer_node) { $employer_name = $employer_node->title; }

  	$wb_json = sirius_json_get($wb_node);

  	$notes = array();
  	if ($employer_name) { $notes[] = '<strong>Employer: </strong>' . $employer_name; }
  	if ($wb_node->field_sirius_notes['und'][0]['value']) { $notes[] = '<strong>Notes: </strong>' . $wb_node->field_sirius_notes['und'][0]['value']; }
  	if ($wb_json['scan']['msg']) { $notes[] = '<strong>Scan: </strong>' . $wb_json['scan']['msg']; }
  	if ($wb_json['scan']['warnings']) {
  		foreach ($wb_json['scan']['warnings'] as $plugin_key => $warning) {
  			$plugin_info = sirius_trust_eligibility_plugin_info($plugin_key);
  			$notes[] = '<strong>' . $plugin_info['title'] . ' Warning:</strong> ' . $warning;
  		}
  	}
  	$notes = join('<br>', $notes);
  	if ($wb_node->field_sirius_active['und'][0]['value'] == 'Yes') { $active = 'Active'; } else { $active = 'Inactive'; }

  	if ($relationship_node) {
  		$name_render = $subscriber_name . '<br>' . $relationship_type_name . '<br>' . '<strong>' . $beneficiary_name . '</strong>';
  	} else {
  		$name_render = '<strong>' . $beneficiary_name . '</strong>' . '<br />' . $relationship_type_name;
  	}

  	$row = array();
  	$row[] = $benefit_node->title;
  	$row[] = $name_render;
  	$row[] = $active . '<br>' . $start_date . '<br>' . $end_date;
  	$row[] = $notes;
  	$row[] = join(' ', $tools);

  	if ($wb_node->field_sirius_active['und'][0]['value'] == 'Yes') {
  		if ($wb_json['scan']['warnings']) {
  	 		$class = 'warning';
	  	} else {
	  		$class = 'active';
	  	}
  	} else {
	  	$class = 'inactive';
  	}

  	$rows[] = array('data' => $row, 'class' => array($class));
  }

  if ($options['hide_header']) {
  	$header = array();
  } else {
	  $header = array(
	  	'Benefit', 
	  	'Subscriber<br>Relationship<br>Dependent',
	  	'Active<br>Start Date<br>End Date',
	  	'Notes',
	  	'Tools'
	  );
	}

	return sirius_table_render($header, $rows, array('class' => array('sirius_table_align')));
}
