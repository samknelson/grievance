<?php

function sirius_steward_sched_access($action, $employer_nid) {
	// Must have an employer
	if (!$employer_nid) { return FALSE; }

	// Admins can do anything
	if (user_access('sirius steward admin')) { return TRUE; }
	if (user_access('sirius employer admin')) { return TRUE; }
	if (user_access('sirius domain admin')) { return TRUE; }
	if (user_access('sirius admin')) { return TRUE; }


	// Employers can view their own record
	if ($action == 'view') {
		if (user_access('sirius steward sched employer')) { 
			global $user;
			$user_full = user_load($user->uid);
			if ($user_full->field_grievance_shops['und'][0]['target_id'] == $employer_nid) { return TRUE; }
		}
	}

	// Workers can access this page for their own employer only
	if (user_access('sirius steward sched worker')) {
		$my_worker = sirius_worker_get();
		$my_employer_nid = $my_worker->field_grievance_shop['und'][0]['target_id'];
		if ($my_employer_nid == $employer_nid) { return TRUE; }
	}

	// Oh well.
	return FALSE;
}

function sirius_steward_sched_list($search_params) {
	$domain_nid = sirius_domain_nid();

	$sql_args = array();
	$sql = "select distinct nid from node ";
	$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_category on field_data_field_sirius_category.entity_type = 'node' and field_data_field_sirius_category.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_notes on field_data_field_sirius_notes.entity_type = 'node' and field_data_field_sirius_notes.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_json on field_data_field_sirius_json.entity_type = 'node' and field_data_field_sirius_json.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_log_handler on field_data_field_sirius_log_handler.entity_type = 'node' and field_data_field_sirius_log_handler.entity_id = node.nid ";
	$sql .= "where node.type = 'sirius_log' ";
	$sql .= "and field_sirius_category_value = 'steward:sched' ";
	if ($search_params['key']) {
		$sql .= "and field_sirius_notes_value = :key ";
		$sql_args[':key'] = $search_params['key'];
	}
	if ($search_params['employer_nid']) {
		$sql .= "and field_sirius_log_handler_target_id = :employer_nid ";
		$sql_args[':employer_nid'] = $search_params['employer_nid'];
	}
	if ($search_params['date']) {
		$sql .= "and field_sirius_notes_value like :date_pattern ";
		$sql_args[':date_pattern'] = '%/' . $search_params['date'];
	}
	if ($search_params['start_date']) {
		$sql .= "and json_unquote(json_extract(field_sirius_json_value, '\$.date')) >= :start_date ";
		$sql_args[':start_date'] = $search_params['start_date'];
	}
	if ($search_params['end_date']) {
		$sql .= "and json_unquote(json_extract(field_sirius_json_value, '\$.date')) <= :end_date ";
		$sql_args[':end_date'] = $search_params['end_date'];
	}
	if ($domain_nid) { 
		$sql .= "and field_sirius_domain_target_id = :domain_nid ";
		$sql_args[':domain_nid'] = $domain_nid;
	} else {
		$sql .= "and field_sirius_domain_target_id is null ";
	}
	$limit = $search_params['limit'];
	if (!$limit) { $limit = 1000; }
	$sql .= "order by nid desc ";
	$sql .= "limit $limit";

	$stmt = sirius_sql_query($sql, $sql_args);
	$sched_nids = array();
	while ($hr = $stmt->fetchAssoc()) { $sched_nids[] = $hr['nid']; }
	return $sched_nids;
}

function sirius_steward_sched_fetch($employer_nid, $date, $create = FALSE) {
	if (!$employer_nid) { return NULL; }
	if (!$date) { return NULL; }
	
	$search_params = array();
	$search_params['employer_nid'] = $employer_nid;
	$search_params['key'] = sirius_steward_sched_key($employer_nid, $date);
	$sched_nids = sirius_steward_sched_list($search_params);
	$sched_nid = array_shift($sched_nids);
	$sched_node = node_load($sched_nid);
	if ($sched_node) { return $sched_node; }

	if (!$create) { return FALSE; }

	$json = array();
	$json['date'] = $date;

	$sched_node = sirius_log(
		'steward:sched', 
		'draft', 
		sirius_steward_sched_title($employer_nid, $date), 
  	'', 
  	sirius_steward_sched_key($employer_nid, $date), 
  	array($employer_nid), 
  	NULL,
  	$json
  );

  $sched_node->title = sirius_steward_sched_title($employer_nid, $date);
  node_save($sched_node);

  return $sched_node;
}

function sirius_steward_sched_title($employer_nid, $date) {
	return 'Steward Schedule - ' . sirius_node_title($employer_nid) . " - $date"; 
}

function sirius_steward_sched_key($employer_nid, $date) { 
	return "$employer_nid/$date";
}

function sirius_steward_sched_render($sched_node, $options = array()) {
	$sched_json = sirius_json_get($sched_node);

	$html = '<div class="sirius_steward_sched">';
	$date = $sched_json['date'];
	if ($sched_json) {
		$html .= '<h2>' . date('l, F j', strtotime($sched_json['date'])) . '</h2>';
	}
	if ($sched_json['sched']['stewards']) {
		if (!$options['hide_header']) {
			$header = array(
				t('Steward'),
				t('Worker Count'),
				t('Time'),
				t('Show') . '<br />' . t('Location'),
				t('Last Crew'),
				t('Department') . '<br />' . t('Lead'),
			);
		} else {
			$header = array();
		}
		$rows = array();
		foreach ($sched_json['sched']['stewards'] as $steward_nid => $steward_json) {
			$row = array();
			$row[] = sirius_node_title($steward_nid);

			if ($steward_json['worker_count'] || $steward_json['time'] || $steward_json['facility_nid']) {
				$row[] = $steward_json['worker_count'];
				if ($steward_json['time']) {
					$row[] = date('h:i A', strtotime($steward_json['time']));
				} else {
					$row[] = '';
				}
				$row[] = sirius_node_title($steward_json['job_group_nid']) . '<br />' . sirius_node_title($steward_json['facility_nid']);
				if ($steward_json['last_crew']) {
					$row[] = date('h:i A', strtotime($steward_json['last_crew']));
				} else {
					$row[] = '';
				}
				$row[] = sirius_term_title($steward_json['department_tid']) . '<br />' . $steward_json['lead'];
			} else {
				$row[] = array('data' => '<em>' . t('Not Scheduled') . '</em>', 'colspan' => 5);
			}
			$rows[] = $row;

			if ($options['hide_checkins']) { continue; }
			
			$search_params = array();
			$search_params['worker_nid'] = $steward_nid;
			$search_params['in_ts_after'] = strtotime($date);
			$search_params['in_ts_before'] = strtotime("+1 day", $search_params['in_ts_after']);
			$checkin_nids = sirius_steward_checkin_search($search_params);

			if ($checkin_nids) {
				$checkin_nodes = node_load_multiple($checkin_nids);
				$checkin_htmls = array();
				foreach ($checkin_nodes as $checkin_nid => $checkin_node) {
					$facility_node = sirius_log_handler($checkin_node, 'sirius_dispatch_facility');
					$checkin_json = sirius_json_get($checkin_node);
					if ($checkin_json['in']['ts']) { $in_date = date('h:i A', $checkin_json['in']['ts']); } else { $in_date = ''; }
					if ($checkin_json['out']['ts']) { $out_date = date('h:i A', $checkin_json['out']['ts']); } else { $out_date = ''; }
					$status = sirius_steward_checkin_status($checkin_node);

					$checkin_html = l('View', "node/$checkin_nid/sirius_steward_log_checkin", array('attributes' => array('class' => array('tool-button'))));
					$checkin_html .= '<em>Checked in';
					if ($facility_node->nid != $steward_json['facility_nid']) { 
						$checkin_html .= ' to <strong>' . $facility_node->title . '</strong>';
					}
					$checkin_html .=' at <strong>' . $in_date . '</strong>';
					if ($out_date) { $checkin_html .= ' / out at <strong>' . $out_date . '</strong>'; }
					$checkin_html .= '</em>';
					$checkin_htmls[] = $checkin_html;
				}

				$checkin_html = join('<br />', $checkin_htmls);
				$row = array();
				$row[] = array('colspan' => 6, 'data' => $checkin_html);
				$rows[] = $row;
			}
		}
		$html .= sirius_table_render($header, $rows, array('class' => array('sirius_table_align')));

	}
	if (!$options['hide_edit']) {
		$html .= l('Edit', 'node/' . $sched_node->nid . '/sirius_steward_log_sched/edit', array('attributes' => array('class' => array('tool-button'))));
	}
	$html .= '</div>';

	return $html;
}
