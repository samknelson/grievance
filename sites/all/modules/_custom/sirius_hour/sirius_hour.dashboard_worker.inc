<?php

function sirius_hour_dashboard_worker_data($prefs, $options = array()) {
	$rows = array();

	$worker_node = sirius_worker_get();
	if (!$worker_node) { return array(); }

	$tools = l('View All', 'node/' . $worker_node->nid . '/sirius_worker_hours', array('attributes' => array('class' => array('tool-button'))));

	$sql = "select year, month, sum(hours) as c ";
	$sql .= "from sirius_hours_cache ";
	$sql .= "where worker_nid = :worker_nid ";
	$sql .= "group by year, month ";
	$sql .= "order by year desc, month desc ";
	$sql .= "limit 4 ";

	$stmt = sirius_sql_query($sql, array(':worker_nid' => $worker_node->nid));
	while ($hr = $stmt->fetchAssoc()) {
		$details .= '<div>';
		$details .= date('F', strtotime($hr['year'] . '-' . $hr['month'] . '-' . 15));
		$details .= ': '; 
		$details .= '<strong>' . round($hr['c'], 2) . '</strong>';
		$details .= '</div>';
	}

	$record = array(
		'count' => $total,
		'tools' => $tools,
		'details' => $details,
	);

	return array($record);
}
