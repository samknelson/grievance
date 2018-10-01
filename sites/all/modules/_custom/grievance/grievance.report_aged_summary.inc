<?php

function grievance_report_aged_summary() {
	return drupal_get_form('grievance_report_aged_summary_form');
}

function grievance_report_aged_summary_form($form, &$form_state) {
  $form['which'] = array(
    '#type' => 'select',
    '#title' => 'Report on:',
    '#options' => array(
    	'specialist' => 'Specialist',
    	'shop' => 'Employer',
    ),
  );


  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Generate Report',
  );

  if ($form_state['results']) {
  	$form['results'] = array(
  		'#type' => 'markup',
  		'#markup' => $form_state['results'],
  	);
  }

  return $form;
}

function grievance_report_aged_summary_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;

	// Get the list of open and closed statuses
  $status_vocabulary = taxonomy_vocabulary_machine_name_load('grievance_status');
  $status_terms = array();
  $tmp = taxonomy_get_tree($status_vocabulary->vid);
  foreach ($tmp as $status_term) {
    $status_terms[$status_term->tid] = taxonomy_term_load($status_term->tid);
  }
  $open_status_tids = array(-999);
  $closed_status_tids = array(-999);

  foreach ($status_terms as $tid => $term) {
    if ($term->field_grievance_open['und'][0]['value'] == 1) {
      $open_status_tids[] = $tid;
    }
    if ($term->field_grievance_open['und'][0]['value'] == 0) {
      $closed_status_tids[] = $tid;
    }
  }

  // Do this as one SQL query, because it's so much quicker
  $sql = "select nid, ";
  $sql .= "created, ";
  $sql .= "field_grievance_rep_organizer_target_id, ";
  $sql .= "field_grievance_shop_target_id ";
  $sql .= "from node ";
  $sql .= "left join field_data_field_grievance_status on (field_data_field_grievance_status.entity_type = 'node' and node.nid = field_data_field_grievance_status.entity_id) ";
  $sql .= "left join field_data_field_grievance_rep_organizer on (field_data_field_grievance_rep_organizer.entity_type = 'node' and node.nid = field_data_field_grievance_rep_organizer.entity_id) ";
  $sql .= "left join field_data_field_grievance_shop on (field_data_field_grievance_shop.entity_type = 'node' and node.nid = field_data_field_grievance_shop.entity_id) ";
  $sql .= "where node.type = 'grievance' ";
  $sql .= "and field_data_field_grievance_status.field_grievance_status_tid in (:open_status_tids) ";

  $breakpoints = array(30, 60, 90, 180, 365, 365*2, 365*3, 365*4, 365*5);
  $header = array(
  	'Name',
  	'>30 days',
  	'>60 days',
  	'>90 days',
  	'>180 days',
  	'>1 year',
  	'>2 years',
  	'>3 years',
  	'>4 years',
  	'>5 years'
  );
  $attributes = array();

  $stmt = db_query($sql, array(':open_status_tids' => $open_status_tids));
  $count = 0;
  $ts_now = time();
  $counts = array();

  $count_total = array();
  $count_by_specialist = array();
  $count_by_shop = array();

  while ($hr = $stmt->fetchAssoc()) {
  	++$count;
  	$secs = $ts_now - $hr['created'];
  	$days = floor($secs / (24 * 60 * 60)); // I know, I know.

  	$breakpoint = 0;
  	foreach (array_reverse($breakpoints) as $check_breakpoint) {
  		if ($days > $check_breakpoint) { 
  			$breakpoint = $check_breakpoint;
  			break;
  		}
  	}

  	++$count_total[$breakpoint];
  	++$count_by_specialist[$hr['field_grievance_rep_organizer_target_id']][$breakpoint];
  	++$count_by_shop[$hr['field_grievance_shop_target_id']][$breakpoint];
  }

  $which = $form_state['values']['which'];

  if ($which == 'specialist') {
  	$lookups = user_load_multiple(array_keys($count_by_specialist));
  	$count_by_which = $count_by_specialist;
  } else {
  	$lookups = node_load_multiple(array_keys($count_by_shop));
  	$count_by_which = $count_by_shop;
  }

  foreach ($count_by_which as $id => &$counts) {
  	if (!$id) {
  		$counts['name'] = '(Not assigned)';
  		$counts['id'] = 0;
  	} else {
  		if ($which == 'specialist') {
  			$counts['name'] = $lookups[$id]->name;
  		} else {
  			$counts['name'] = $lookups[$id]->title;
  		}
  		$counts['id'] = $id;
  	}
  }

  usort($count_by_which, function($a, $b) { 
  	if (!$a['id']) { return 1; }
  	if (!$b['id']) { return -1; }
  	return strcmp($a['name'], $b['name']);
	});

  $rows = array();
  foreach ($count_by_which as $id => &$counts) {
  	$row = array();
  	$row[] = $counts['name'];
  	foreach ($breakpoints as $breakpoint) {
  		$row[] = number_format($counts[$breakpoint]);
  	}
  	$rows[] = $row;
  }

  $row = array();
  $row[] = '<strong>Total</strong>';
	foreach ($breakpoints as $breakpoint) {
		$row[] = '<strong>' . number_format($count_total[$breakpoint], 0) . '</strong>';
	}
	$rows[] = $row;


	$form_state['results'] = theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => $attributes));
}

