<?php

function sirius_bac1_hours_by_employer() {
  $year = date('Y');
  $month = date('n');
  --$month;
  if ($month < 1) { $month = 12; --$year; }
  drupal_set_title("Hours by Employer: $month/$year");

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

	$html .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array()));
	$html .= '<div>';
	$html .= "Note: The above totals show the hours for the CURRENT member status of each worker. They do not necessarily reflect the member status at the time that the work was performed.";
	$html .= '</div>';
	return $html;
}