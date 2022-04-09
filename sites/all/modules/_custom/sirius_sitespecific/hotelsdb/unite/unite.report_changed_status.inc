<?php

function unite_report_changed_status() {
	return drupal_get_form('unite_report_changed_status_form', $node);
}

function unite_report_changed_status_form($form, $form_state) {

	$form['description'] = array(
		'#type' => 'markup', 
		'#markup' => t('This report will select properties where the union status, labor activity index, or property name has changed recently.'),
	);

	$form['start_date'] = array(
		'#type' => 'date', 
		'#title' => t("Start Date"),
		'#description' => t('Select properties which have changed their status AFTER this date.'),
		'#required' => 1,
	);

	$form['end_date'] = array(
		'#type' => 'date', 
		'#title' => t("End Date"),
		'#description' => t('Select properties which have changed their status BEFORE this date.'),
		'#required' => 1,
	);

	$form['submit'] = array(
		'#type' => 'submit', 
		'#value' => t("Generate Report"),
	);

	if ($form_state['results']) {
		$form['results'] = array(
			'#type' => 'markup',
			'#markup' => $form_state['results'],
		);
	}

	return $form;
}

function unite_report_changed_status_form_submit($form, &$form_state) {
	$form_state['rebuild'] = 1;

	// Form values
	$start_date = $form_state['values']['start_date'];
	$end_date = $form_state['values']['end_date'];
	$start_ts = strtotime($start_date['year'] . '-' . $start_date['month'] . '-' . $start_date['day']);
	$end_ts = strtotime('+1 day', strtotime($end_date['year'] . '-' . $end_date['month'] . '-' . $end_date['day']));

	$data = array();

	// Fetch the changed NIDs
	$fields = array('title', 'union_status', 'labor_activity_index');
	foreach ($fields as $field) {
		if ($field == 'title') {
			$sql = "select distinct r.nid, r.title from ";
			$sql .= "node_revision r, node n ";
			$sql .= "where n.nid = r.nid ";
			$sql .= "and n.type = 'property' ";
			$sql .= "and r.timestamp >= :start_ts ";
			$sql .= "and r.timestamp < :end_ts ";
			$sql .= "and ";
			$sql .= "( ";
			$sql .= "   ( ";
			$sql .= "    select IF(ISNULL(r1.title), '', r1.title) from ";
			$sql .= "    node_revision r1 ";
			$sql .= "    where r1.nid = r.nid ";
			$sql .= "    and r1.timestamp < r.timestamp ";
			$sql .= "    order by r1.timestamp desc limit 1 ";
			$sql .= "  ) ";
			$sql .= "  != r.title ";
			$sql .= ") ";
		} else {
			$sql = "select distinct r.nid, r.title from ";
			$sql .= "node_revision r, field_revision_field_${field} s ";
			$sql .= "where s.entity_type = 'node' ";
			$sql .= "and s.bundle = 'property' ";
			$sql .= "and s.revision_id = r.vid ";
			$sql .= "and r.timestamp >= :start_ts ";
			$sql .= "and r.timestamp < :end_ts ";
			$sql .= "and ";
			$sql .= "( ";
			$sql .= "  ( SELECT IF(ISNULL(s1.field_${field}_value), '', s1.field_${field}_value) ";
			$sql .= "    from node_revision r1, field_revision_field_${field} s1 ";
			$sql .= "    where r1.nid = r.nid ";
			$sql .= "    and r1.timestamp < r.timestamp ";
			$sql .= "    and s1.entity_type = 'node' ";
			$sql .= "    and s1.revision_id = r1.vid ";
			$sql .= "    order by r1.timestamp desc limit 1 ";
			$sql .= "  ) != s.field_${field}_value ";
			$sql .= ")";
		}

		$stmt = sirius_sql_query($sql, array(':start_ts' => $start_ts, ':end_ts' => $end_ts));
		while ($hr = $stmt->fetchAssoc()) {
			$nid = $hr['nid'];
			if (!$data[$nid]) { $data[$nid] = array(); }
			$data[$nid]['title'] = $hr['title'];
		}
	}

	foreach ($data as $nid => $record) {
		foreach ($fields as $field) {
			if ($field == 'title') {
				$sql = "select * from ";
				$sql .= "node_revision r ";
				$sql .= "where r.nid = :nid ";
				$sql .= "order by timestamp desc ";
			} else {
				$sql = "select * from ";
				$sql .= "field_revision_field_${field} s, node_revision r ";
				$sql .= "where s.entity_type = 'node' ";
				$sql .= "and s.bundle = 'property' ";
				$sql .= "and s.revision_id = r.vid ";
				$sql .= "and r.nid = :nid ";
				$sql .= "order by timestamp desc ";
			}

			$stmt = db_query($sql, array(':nid' => $nid));
			while ($hr = $stmt->fetchAssoc()) {
				if ($field == 'title') {
					$value = $hr["title"];
				} else {
					$value = $hr["field_${field}_value"];
				}
				$ts = $hr['timestamp'];

				if (!$data[$nid]["$field-value"]) { $data[$nid]["$field-value"] = $value; }
				if (!$data[$nid]["$field-ts"]) { $data[$nid]["$field-ts"] = date('Y-m-d', $ts); }

				if (!$data[$nid]["$field-prev-value"] && ($value != $data[$nid]["$field-value"])) {
					$data[$nid]["$field-prev-value"] = $value;
					$data[$nid]["$field-prev-ts"] = date('Y-m-d', $ts);
				}
			}
		}
	}

	$header = array(
		'NID',
		'Title',
		'Title Date',
		'Previous Title',
		'Previous Title Date',
		'Current Status',
		'Current Status Date',
		'Prev Status',
		'Prev Status Date',
		'Current HLA',
		'Current HLA Date',
		'Prev HLA',
		'Prev HLA Date'
	);
	$rows = array();
	foreach ($data as $nid => $record) {
		$row = array();
		$row[] = $nid;
		$row[] = l($record['title'], "node/$nid");
		$row[] = $record['title-ts'];
		$row[] = $record['title-prev-value'];
		$row[] = $record['title-prev-ts'];

		$row[] = $record['union_status-value'];
		$row[] = $record['union_status-ts'];
		$row[] = $record['union_status-prev-value'];
		$row[] = $record['union_status-prev-ts'];

		$row[] = $record['labor_activity_index-value'];
		$row[] = $record['labor_activity_index-ts'];
		$row[] = $record['labor_activity_index-prev-value'];
		$row[] = $record['labor_activity_index-prev-ts'];

		$rows[] = $row;
	}

	$form_state['results'] = '<hr>' . '<h2>' . t('Results') . '</h2>' . theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array()));

	drupal_set_message("Generating report.");
}
