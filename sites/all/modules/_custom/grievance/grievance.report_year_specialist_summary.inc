<?php

function grievance_report_year_specialist_summary() {
	return drupal_get_form('grievance_report_year_specialist_summary_form');
}

function grievance_report_year_specialist_summary_form($form, &$form_state) {

	$y = date('Y');
	$options = array(NULL => '[select]');
	for ($i=-10; $i<=10; ++$i) {
		$options[$y+$i] = $y+$i;
	}
  $form['year'] = array(
    '#title' => t('Year'),
    '#type' => 'select',
    '#required' => TRUE,
    '#options' => $options,
	);

	$options = array(
		1 => 'January',
		2 => 'February',
		3 => 'March',
		4 => 'April',
		5 => 'May',
		6 => 'June',
		7 => 'July',
		8 => 'August',
		9 => 'September',
		10 => 'October',
		11 => 'November',
		12 => 'December',
	);

  $form['start_month'] = array(
    '#title' => t('Start Month'),
    '#type' => 'select',
    '#required' => TRUE,
    '#options' => $options,
	);

  $form['end_month'] = array(
    '#title' => t('End Month'),
    '#type' => 'select',
    '#required' => TRUE,
    '#options' => $options,
	);

  $form['display'] = array(
    '#title' => t('Display'),
    '#type' => 'select',
    '#required' => TRUE,
    '#options' => array(
    	'month_first' => t('Month, then Specialist'),
    	'specialist_first' => t('Specialist, then Month'),
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

function grievance_report_year_specialist_summary_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
	$year = $form_state['values']['year'];

  $status_vocabulary = taxonomy_vocabulary_machine_name_load('grievance_status');
  $status_terms = taxonomy_get_tree($status_vocabulary->vid);
  $open_status_tids = array();
  $closed_status_tids = array();
  foreach ($status_terms as $status_term) {
  	$status_term_full = taxonomy_term_load($status_term->tid);
  	if ($status_term_full->field_grievance_open['und'][0]['value'] == 1) {
  		$open_status_tids[] = $status_term_full->tid;
  	} else {
  		$closed_status_tids[] = $status_term_full->tid;
  	}
  }

  $insert_log_tid = variable_get('grievance_log_insert_type', '');
  $update_log_tid = variable_get('grievance_log_update_type_with_status_change', '');

  // A grievance might be opened and closed multiple times, so we need to pull them all and sort in memory; simply doing a "group by ... distinct" in the SQL doesn't work.

	$start_month = $form_state['values']['start_month'];
	$end_month = $form_state['values']['end_month'];
	$display = $form_state['values']['display'];

  //
  // Count the grievances opened, by month and specialist
  //

  $sql = "select ";
	$sql .= "extract(month from field_data_field_grievance_status_date.field_grievance_status_date_value) as m, ";
	$sql .= "field_data_field_grievance.field_grievance_target_id as grievance_nid, ";
	$sql .= "users.name as organizer_name ";
	$sql .= "from node ";
	$sql .= "join field_data_field_grievance_log_type on (field_data_field_grievance_log_type.entity_id = node.nid and field_data_field_grievance_log_type.entity_type = 'node') ";
	$sql .= "join field_data_field_grievance_status_date on (field_data_field_grievance_status_date.entity_id = node.nid and field_data_field_grievance_status_date.entity_type = 'node') ";
	$sql .= "join field_data_field_grievance on (field_data_field_grievance.entity_id = node.nid and field_data_field_grievance.entity_type = 'node') ";
	$sql .= "join field_data_field_grievance_rep_organizer on (";
	$sql .= "   field_data_field_grievance_rep_organizer.entity_type = 'node' and ";
	$sql .= "   field_data_field_grievance_rep_organizer.entity_id = field_data_field_grievance.field_grievance_target_id and ";
	$sql .= "   field_data_field_grievance_rep_organizer.delta = 0 ";
	$sql .= ") ";
	$sql .= "join users on users.uid = field_grievance_rep_organizer_target_id ";
	$sql .= "where node.type = 'grievance_log' ";
	$sql .= "and field_data_field_grievance_log_type.field_grievance_log_type_tid = :insert_log_tid ";
	$sql .= "and extract(year from field_data_field_grievance_status_date.field_grievance_status_date_value) = $year ";
	$sql .= "order by field_data_field_grievance_status_date.field_grievance_status_date_value ";

	$stmt = db_query($sql, array(':insert_log_tid' => $insert_log_tid));
	$opened_count = array();
	$opened_count_total = array();
	$already_seen = array();
	$opened_count_grand_total = 0;

	$specialists = array();
	while ($hr = $stmt->fetchAssoc()) {
		$month = $hr['m'];
		if ($start_month && $month < $start_month) { continue; }
		if ($end_month && $month > $end_month) { continue; }

		if ($already_seen[$hr['grievance_nid']]) { continue; }
		$already_seen[$hr['grievance_nid']] = TRUE;

		if ($display == 'month_first') {
			$opened_count[$hr['m']][$hr["organizer_name"]]++;
			$opened_count_total[$hr['m']]++;
		} else {
			$opened_count[$hr["organizer_name"]][$hr['m']]++;
			$opened_count_total[$hr['organizer_name']]++;
		}
		++$opened_count_grand_total;
		$specialists[$hr['organizer_name']] = TRUE;
	}

  //
  // Count the grievances closed, by month. 
  //

  $sql = "select ";
	$sql .= "extract(month from field_data_field_grievance_status_date.field_grievance_status_date_value) as m, ";
	$sql .= "field_data_field_grievance.field_grievance_target_id as grievance_nid, ";
	$sql .= "users.name as organizer_name ";
	$sql .= "from node ";
	$sql .= "join field_data_field_grievance_log_type on (field_data_field_grievance_log_type.entity_id = node.nid and field_data_field_grievance_log_type.entity_type = 'node') ";
	$sql .= "join field_data_field_grievance_status_date on (field_data_field_grievance_status_date.entity_id = node.nid and field_data_field_grievance_status_date.entity_type = 'node') ";
	$sql .= "join field_data_field_grievance_status on (field_data_field_grievance_status.entity_id = node.nid and field_data_field_grievance_status.entity_type = 'node') ";
	$sql .= "join field_data_field_grievance on (field_data_field_grievance.entity_id = node.nid and field_data_field_grievance.entity_type = 'node') ";
	$sql .= "join field_data_field_grievance_rep_organizer on (";
	$sql .= "   field_data_field_grievance_rep_organizer.entity_type = 'node' and ";
	$sql .= "   field_data_field_grievance_rep_organizer.entity_id = field_data_field_grievance.field_grievance_target_id and ";
	$sql .= "   field_data_field_grievance_rep_organizer.delta = 0 ";
	$sql .= ") ";
	$sql .= "join users on users.uid = field_grievance_rep_organizer_target_id ";
	$sql .= "where node.type = 'grievance_log' ";
	$sql .= "and field_data_field_grievance_log_type.field_grievance_log_type_tid = :update_log_tid ";
	$sql .= "and field_data_field_grievance_status.field_grievance_status_tid in (:closed_status_tids) ";
	$sql .= "and extract(year from field_data_field_grievance_status_date.field_grievance_status_date_value) = $year ";
	$sql .= "order by field_data_field_grievance_status_date.field_grievance_status_date_value ";

	$stmt = db_query($sql, array(':update_log_tid' => $update_log_tid, ':closed_status_tids' => $closed_status_tids));
	$closed_count = array();
	$already_seen = array();
	$closed_count_total = array();
	$closed_count_grand_total = 0;

	while ($hr = $stmt->fetchAssoc()) {
		$month = $hr['m'];
		if ($start_month && $month < $start_month) { continue; }
		if ($end_month && $month > $end_month) { continue; }

		if ($already_seen[$hr['grievance_nid']]) { continue; }
		$already_seen[$hr['grievance_nid']] = TRUE;

		if ($display == 'month_first') {
			$closed_count[$hr['m']][$hr["organizer_name"]]++;
			$closed_count_total[$hr['m']]++;
		} else {
			$closed_count[$hr["organizer_name"]][$hr['m']]++;
			$closed_count_total[$hr['organizer_name']]++;
		}

		++$closed_count_grand_total;
		$specialists[$hr['organizer_name']] = TRUE;
	}


	$rows = array();
	$months = array(
		1 => 'January', 
		2 => 'February', 
		3 => 'March', 
		4 => 'April', 
		5 => 'May', 
		6 => 'June', 
		7 => 'July', 
		8 => 'August', 
		9 => 'September', 
		10 => 'October', 
		11 => 'November', 
		12 => 'December', 
	);


	if ($display == 'month_first') {
		$header = array(
			t('Month'),
			t('Specialist'),
			t('Opened'),
			t('Closed')
		);

		foreach ($months as $month => $month_name) {
			if ($start_month && $month < $start_month) { continue; }
			if ($end_month && $month > $end_month) { continue; }

			$first = TRUE;
			foreach (array_keys($specialists) as $specialist_name) {
				if (!$opened_count[$month][$specialist_name] && !$closed_count[$month][$specialist_name]) { continue; }
				if (!$opened_count[$month][$specialist_name]) { $opened_count[$month][$specialist_name] = 0; }
				if (!$closed_count[$month][$specialist_name]) { $closed_count[$month][$specialist_name] = 0; }

				$row = array();
				if ($first) { 
					$row[] = '<strong>' . $month_name . '</strong>';
					$first = FALSE;
				} else {
					$row[] = '';
				}
				$row[] = $specialist_name;
				$row[] = $opened_count[$month][$specialist_name];
				$row[] = $closed_count[$month][$specialist_name];

				$rows[] = $row;
			}
			if (!$first) {
				$row = array();
				$row[] = '';
				$row[] = '<strong>' . t('Total') . '</strong>';
				$row[] = $opened_count_total[$month];
				$row[] = $closed_count_total[$month];
				$rows[] = $row;
			}
		}
	} else { // display = specialist_first
		$header = array(
			t('Specialist'),
			t('Month'),
			t('Opened'),
			t('Closed')
		);

		foreach (array_keys($specialists) as $specialist_name) {
			$first = TRUE;
			foreach ($months as $month => $month_name) {
				if ($start_month && $month < $start_month) { continue; }
				if ($end_month && $month > $end_month) { continue; }
				if (!$opened_count[$specialist_name][$month] && !$closed_count[$specialist_name][$month]) { continue; }
				if (!$opened_count[$specialist_name][$month]) { $opened_count[$specialist_name][$month] = 0; }
				if (!$closed_count[$specialist_name][$month]) { $closed_count[$specialist_name][$month] = 0; }

				$row = array();
				if ($first) { 
					$row[] = '<strong>' . $specialist_name . '</strong>';
					$first = FALSE;
				} else {
					$row[] = '';
				}
				$row[] = $month_name;
				$row[] = $opened_count[$specialist_name][$month];
				$row[] = $closed_count[$specialist_name][$month];

				$rows[] = $row;
			}

			if (!$first) {
				$row = array();
				$row[] = '';
				$row[] = '<strong>' . t('Total') . '</strong>';
				$row[] = $opened_count_total[$specialist_name];
				$row[] = $closed_count_total[$specialist_name];
				$rows[] = $row;
			}
		}
	}

	$row = array();
	$row[] = '<strong>' . t('Grand Total') . '</strong>';
	$row[] = '';
	$row[] = '<strong>' . number_format($opened_count_grand_total, 0) . '</strong>';
	$row[] = '<strong>' . number_format($closed_count_grand_total, 0) . '</strong>';
	$rows[] = $row;



	$html = '<h2>' . t('Opened / Closed Report: ') . $year . '</h2>';
	$html .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('sirius_data_table'))));

	$form_state['results'] = $html;

	return $html;
}