<?php

function sirius_bac1_hours_by_employer() {
	return drupal_get_form('sirius_bac1_hours_by_employer_form');
}

function sirius_bac1_hours_by_employer_form($form, &$form_state) {
 	$this_year = date('Y');
 	$this_month = date('n');
  --$this_month;
  if ($this_month < 1) { $this_month = 12; --$this_year; }

  $months = array(
    '1' => '1  January',
    '2' => '2  February',
    '3' => '3  March',
    '4' => '4  April',
    '5' => '5  May',
    '6' => '6  June',
    '7' => '7  July',
    '8' => '8  August',
    '9' => '9  September',
    '10' => '10 October',
    '11' => '11 November',
    '12' => '12 December',
  );
	$form['month'] = array(
		'#type' => 'select',
		'#options' => $months,
		'#default_value' => $this_month,
		'#title' => t('End Month'),
		'#required' => TRUE,
	);

	$years = array();
	for ($year = $this_year-10; $year<=$this_year; ++$year) { $years[$year] = $year; }
	$form['year'] = array(
		'#type' => 'select',
		'#options' => $years,
		'#default_value' => $this_year,
		'#title' => t('End Year'),
		'#required' => TRUE,
	);

	$form['output'] = array(
		'#type' => 'select',
		'#options' => array(
			'screen' => t('Screen'),
			'csv' => t('CSV (Spreadsheet)'),
		),
		'#default_value' => 'screen',
		'#title' => t('Ouptut'),
		'#required' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

  $year = $form_state['values']['year']; 
  if (!$year) { $year = $this_year; }

  $month = $form_state['values']['month']; 
  if (!$month) { $month = $this_month; }

  drupal_set_title("Hours by Employer: $month/$year");

  $output = $form_state['values']['output'];

  $domain_nid = sirius_domain_nid();

  // Get the list of member statuses
  $all_ms_terms = sirius_taxonomy_load('sirius_member_status', 'tid', 'full');
  $all_industry_terms = sirius_taxonomy_load('sirius_industry', 'tid', 'full');

  // Get the list of employers
  $employers = sirius_employer_list();
  $data = array();
  $ms_names = array();
  foreach ($employers as $employer_nid => $employer_name) {
  	$data[$employer_nid] = array();
  	$data[$employer_nid]['employer'] = $employer_name;

  	$employer_node = node_load($employer_nid);
  	$employer_industry_tids = sirius_fieldvals($employer_node, 'field_sirius_industry', 'tid');
  	$employer_ms_tids = array();
  	foreach ($all_ms_terms as $ms_tid => $ms_term) {
  		if (in_array($ms_term->field_sirius_industry['und'][0]['tid'], $employer_industry_tids)) {
  			++$employer_ms_tids[$ms_tid];
  		}
  	}
  	$employer_ms_tids = array_keys($employer_ms_tids);

	  $sql_args = array();
	  $sql_args[':month'] = $month;
	  $sql_args[':year'] = $year;
	  $sql_args[':employer_nid'] = $employer_nid;
		$sql = "select worker_nid, year, month, day, hours, ";
		$sql .= "( ";
		$sql .= "  select group_concat(distinct field_sirius_member_status_tid separator '|') ";
		$sql .= "  from field_data_field_sirius_member_status ";
		$sql .= "  where field_data_field_sirius_member_status.entity_type = 'node' ";
		$sql .= "  and field_data_field_sirius_member_status.entity_id = worker_node.nid ";
		$sql .= ") as ms_tids ";
		$sql .= "from sirius_hours_cache ";
		$sql .= "left join node employer_node on employer_node.nid = employer_nid ";
		$sql .= "left join node worker_node on worker_node.nid = worker_nid ";
		$sql .= "where month = :month ";
		$sql .= "and year = :year ";
		$sql .= "and employer_nid = :employer_nid ";
		$stmt = sirius_sql_query($sql, $sql_args);
		while ($hr = $stmt->fetchAssoc()) {
			$worker_ms_tids = explode('|', $hr['ms_tids']);
			$good_ms_tids = array_intersect($worker_ms_tids, $employer_ms_tids);
			$good_ms_tid = reset($good_ms_tids);

			$ms_name = $all_ms_terms[$good_ms_tid]->name;
			$ms_name = preg_replace('/\-.+/', '', $ms_name);
			$ms_name = trim($ms_name);
			if (!$ms_name) { $ms_name = '[other]'; }
			$data[$employer_nid]['hours'][$ms_name] += $hr['hours'];
			++$ms_names[$ms_name];
		}
	}

	$ms_names = array_keys($ms_names);
	sort($ms_names);

	$header = array('Employer');
	foreach ($ms_names as $ms_name) { $header[] = $ms_name; }
	$header[] = 'Total';

	$rows = array();
	$ms_total = array();
	foreach ($data as $employer_nid => $subdata) {
		$employer_total = 0;
		$row = array();
		$row[] = $subdata['employer'];
		foreach ($ms_names as $ms_name) {
			$row[] = $subdata['hours'][$ms_name];
			$employer_total += $subdata['hours'][$ms_name];
			$ms_total[$ms_name] += $subdata['hours'][$ms_name];
		}
		$row[] = '<strong>' . $employer_total . '</strong>';
		$grand_total += $employer_total;

		$rows[] = $row;
	}

	$row = array();
	$row[] = '<strong>Total</strong>';
	foreach ($ms_names as $ms_name) {
		$row[] = '<strong>' . $ms_total[$ms_name] . '</strong>';
	}
	$row[] = '<strong>' . $grand_total . '</strong>';
	$rows[] = $row;

	if ($output == 'csv') {
    $filename = "hours_by_employer_${year}_${month}.csv";

    drupal_add_http_header('Content-Type', 'text/csv');
    drupal_add_http_header('Content-Disposition', 'attachment;filename=' . $filename);
    $fp = fopen('php://output', 'w');
    fputcsv($fp, $header);
    foreach ($rows as $row) { 
      foreach ($row as $delta => $val) { $row[$delta] = strip_tags($val); }
      fputcsv($fp, $row);
    }
    drupal_exit();
  }

	$html .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array()));
	$html .= '<div>';
	$html .= "Note: The above totals show the hours for the CURRENT member status of each worker. They do not necessarily reflect the member status at the time that the work was performed.";
	$html .= '</div>';

	$form['results'] = array('#markup' => $html);

	return $form;
}

function sirius_bac1_hours_by_employer_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
}
