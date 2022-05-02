<?php 

function sirius_cron_scan_info($key) {
  $info_all = &drupal_static(__FUNCTION__); 
  if (!isset($info_all)) { 
  	$info_all = sirius_cron_scan_info_all();
  }

	return $info_all[$key];
}

function sirius_cron_scan_info_all() {
  $info_all = &drupal_static(__FUNCTION__);
  if (!isset($info_all)) {
  	$info_all = module_invoke_all('sirius_cron_scan_info');
		ksort($info_all);
  }
	return $info_all;
}

/**
 * Implements hook_sirius_cron_scan_info()
 */

function sirius_sirius_cron_scan_info() {
	return array(
		'check_active_expired_nodes' => array(
			'title' => 'Active but Expired Nodes',
			'description' => 'Check for active nodes whose expiration date has passed',
			'schedule' => 'daily',
			'callback' => 'sirius_cron_scan_expired_active_nodes',
		),
	);
}

/**
 * A good thing to do from the command line is:
 * drush --user=1 php-eval "sirius_minilog_echo_active(TRUE); print_r(sirius_cron_scan_all(TRUE, 'live'));"
 */

function sirius_cron_scan_all($force = FALSE, $mode = 'live') {
	$info_all = sirius_cron_scan_info_all();
	foreach ($info_all as $key => $info) {
		$result = sirius_cron_scan($key, $force, $mode);
		if (!$result['success']) { return $result;}
	}
	return array('success' => TRUE);
}

function sirius_cron_scan($key, $force = FALSE, $mode = 'live') {
	$info = sirius_cron_scan_info($key);
	if (!$info) {
		$msg = "Info not found: $key";
		sirius_minilog($msg, 'error');
		return array('success' => FALSE, 'msg' => $msg);
	}

	if ($info['schedule'] == 'daily') {
		// Run once a day
		$today = date('Y-m-d');
		$last_run = variable_get("sirius_cron_scan_${key}_last_run", 0);
		if ($last_run == $today && !$force) {
			$msg = "Cron scan: $info[title]: Too soon to run";
			sirius_minilog($msg);
			return array('success' => TRUE, 'msg' => $msg);
		}
		variable_set("sirius_cron_scan_${key}_last_run", $today);
	}

	sirius_minilog("Cron scan: $info[title]: Starting.");

	$result = $info['callback']($mode);

	if (!$result['success']) {
		$msg = "Cron scan: $info[title]: $result[msg]";
		sirius_minilog($msg, 'error');
		return array('success' => FALSE, 'msg' => $msg);
	}

	$msg = "Cron scan: $info[title]: Scan complete.";
	sirius_minilog($msg);
	return array('success' => TRUE, 'msg' => $msg);
}

function sirius_cron_scan_expired_active_nodes($mode) {
	$content_types = array(
		'sirius_contact_relationship',
		'sirius_trust_worker_benefit',
		'sirius_trust_worker_election',
	);

	$current_date = gmdate('Y-m-d H:i:s');
	$sql_args = array();
	$sql_args[':current_date'] = $current_date;

	foreach ($content_types as $content_type) {
		$sql_args[':content_type'] = $content_type;

		sirius_minilog("$content_type: Scanning for active nodes that should be expired.");

		$sql = "select nid, title ";
		$sql .= "from node ";
		$sql .= "left join field_data_field_sirius_date_start on field_data_field_sirius_date_start.entity_type = 'node' and field_data_field_sirius_date_start.entity_id = node.nid ";
		$sql .= "left join field_data_field_sirius_date_end on field_data_field_sirius_date_end.entity_type = 'node' and field_data_field_sirius_date_end.entity_id = node.nid ";
		$sql .= "left join field_data_field_sirius_active on field_data_field_sirius_active.entity_type = 'node' and field_data_field_sirius_active.entity_id = node.nid ";
		$sql .= "where node.type = :content_type ";
		$sql .= "and field_sirius_active_value = 'Yes' ";
		$sql .= "and (field_sirius_date_end_value is not null and field_sirius_date_end_value < :current_date) ";
		$sql .= "order by nid ";
		$stmt = db_query($sql, $sql_args);

		$count = 0;
		while ($hr = $stmt->fetchAssoc()) {
			++$count;
			if ($mode == 'live') {
				sirius_minilog("$content_type: Found $hr[nid] ($hr[title]). Resaving.");
				$node = node_load($hr['nid']);
				node_save($node);
				sirius_minilog("$content_type: Found $hr[nid] ($hr[title]). Save complete.");

			} else {
				sirius_minilog("$content_type: Found $hr[nid] ($hr[title]). Mode is $mode, so no action is taken.");
			}

			sirius_minilog("$content_type: Found $count nodes that needed to be expired.");
		}

		sirius_minilog("$content_type: Scanning for inactive nodes that should be activated.");

		$sql = "select nid, title ";
		$sql .= "from node ";
		$sql .= "left join field_data_field_sirius_date_start on field_data_field_sirius_date_start.entity_type = 'node' and field_data_field_sirius_date_start.entity_id = node.nid ";
		$sql .= "left join field_data_field_sirius_date_end on field_data_field_sirius_date_end.entity_type = 'node' and field_data_field_sirius_date_end.entity_id = node.nid ";
		$sql .= "left join field_data_field_sirius_active on field_data_field_sirius_active.entity_type = 'node' and field_data_field_sirius_active.entity_id = node.nid ";
		$sql .= "where node.type = :content_type ";
		$sql .= "and field_sirius_active_value = 'No' ";
		$sql .= "and (field_sirius_date_start_value is not null and field_sirius_date_start_value <= :current_date) ";
		$sql .= "and (field_sirius_date_end_value is null or field_sirius_date_end_value >= :current_date) ";
		$sql .= "order by nid ";
		$stmt = db_query($sql, $sql_args);

		$count = 0;
		while ($hr = $stmt->fetchAssoc()) {
			++$count;
			if ($mode == 'live') {
				sirius_minilog("$content_type: Found $hr[nid] ($hr[title]). Resaving.");
				$node = node_load($hr['nid']);
				node_save($node);
				sirius_minilog("$content_type: Found $hr[nid] ($hr[title]). Save complete.");

			} else {
				sirius_minilog("$content_type: Found $hr[nid] ($hr[title]). Mode is $mode, so no action is taken.");
			}

			sirius_minilog("$content_type: Found $count nodes that needed to be activated.");
		}

	}

	return array('success' => TRUE);
}
