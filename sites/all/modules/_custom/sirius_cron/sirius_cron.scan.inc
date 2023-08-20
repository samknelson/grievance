<?php

function sirius_cron_scan_key($scan_node) {
	$type = $scan_node->field_sirius_type['und'][0]['value'];
	if (!preg_match('/^cron\:scan\:/', $type)) { return NULL; }

	$key = preg_replace('/^cron\:scan\:/', '', $type);
	if (!$key) { return NULL; }
	
	return $key;
}


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
  	
		// We want to do this:
		// $info_all = module_invoke_all('sirius_cron_scan_info');
		// But we want to add information about the module that defines it. So we call each function manually.

		$info_all = array();
		$hook = 'sirius_cron_scan_info';
		foreach (module_implements($hook) as $module) {
			$function = $module . '_' . $hook;
    	if (function_exists($function)) {
      	$result = call_user_func_array($function, array());
      	if (isset($result) && is_array($result)) {
        	$info_all = array_merge_recursive($info_all, $result);
      		// Add the module to the results, so we know which module provides which report.
      		foreach ($result as $key => $info) {
      			$info_all[$key]['module'] = $module;
      			$info_all[$key]['module_name'] = sirius_module_friendly_name($module);
      		}
      	} elseif (isset($result)) {
        	$info_all[] = $result;
      	}
    	}
    }

	  foreach ($info_all as $key => $info) {
	  	$info_all[$key]['key'] = $key;
	  }
		ksort($info_all);
  }
	return $info_all;
}

/**
 * Implements hook_sirius_cron_scan_info()
 */

function sirius_cron_sirius_cron_scan_info() {
	$items['check_active_expired_nodes'] = array(
		'title' => 'Active but Expired Nodes',
		'description' => 'Check for active nodes whose expiration date has passed',
		'schedule' => 'daily',
		'callback' => 'sirius_cron_scan_expired_active_nodes',
	);

	$items['test'] = array(
		'title' => 'Test Scan',
		'description' => 'Run a test of the cron system',
		'schedule' => 'daily',
		'callback' => 'sirius_cron_scan_test',
		'multidomain' => TRUE,
	);

	return $items;
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
	sirius_minilog_reset();

	$info = sirius_cron_scan_info($key);
	if (!$info) {
		$msg = "Info not found: $key";
		sirius_minilog($msg, 'error');
		return array('success' => FALSE, 'msg' => $msg);
	}

	// Too soon to run?
	if ($info['schedule'] == 'daily') { $today = gmdate('Y-m-d'); }
	else if ($info['schedule'] == 'hourly') { $today = gmdate('Y-m-d H'); }
	else { $today = gmdate('Y-m-d H:i'); }

	$last_run = variable_get("sirius_cron_scan_${key}_last_run", 0);
	if ($last_run == $today && !$force) {
		$msg = "Cron scan: $info[title]: Too soon to run ($today)";
		sirius_minilog($msg);
		return array('success' => TRUE, 'msg' => $msg);
	}
	variable_set("sirius_cron_scan_${key}_last_run", $today);

	$scan_node = sirius_cron_scan_get($key, TRUE);
	$scan_json = sirius_json_get($scan_node);
	$scan_json['cron']['start_ts'] = time();
	sirius_minilog("Cron scan: $info[title]: Starting.");

	if ($info['multidomain']) {
		$domains = sirius_domain_tree_render_as_select();
		$component = $info['component'];

		$count = 0;
		$count_success = 0;
		$count_failure = 0;
		foreach ($domains as $domain_nid => $domain_name) {
			sirius_minilog("Cron scan: $info[title]: $domain_name: Starting");
			if (!sirius_domain_active($domain_nid)) {
				sirius_minilog("Cron scan: $info[title]: $domain_name: Domain is inactive. Skipping.");
				continue; 
			}

			if ($component && !sirius_domain_component_enabled($component, $domain_nid)) {
				sirius_minilog("Cron scan: $info[title]: $domain_name: Component [$component] is not enabled. Skipping.");
				continue; 
			}

			$old_domain_nid = sirius_domain_nid();
			sirius_domain($domain_nid);
			$sub_result = $info['callback']($mode);
			sirius_domain($old_domain_nid);

			++$count;
			if ($sub_result['success']) { 
				++$count_success;
				sirius_minilog("Cron scan: $info[title]: $domain_name: $sub_result[msg]");
			} else {
				++$count_failure;
				sirius_minilog("Cron scan: $info[title]: $domain_name: $sub_result[msg]", 'error');
			}
		}
		$result = array('success' => TRUE, 'msg' => "Scanned $count domains, with $count_success success and $count_failure failures.");
	} else {
		$result = $info['callback']($mode);
	}

	if (!$result['success']) {
		$msg = "Cron scan: $info[title]: $result[msg]";
		sirius_minilog($msg, 'error');
		return array('success' => FALSE, 'msg' => $msg);
	}

	$msg = "Cron scan: $info[title]: Scan complete";
	if ($result['msg']) { $msg .= ': ' . $result['msg']; }
	sirius_minilog($msg);

	$scan_json['cron']['end_ts'] = time();
	$scan_json['cron']['log'] = sirius_minilog_render_html();
	$scan_json['cron']['msg'] = $msg;
	$scan_json['cron']['duration_ts'] = $scan_json['cron']['end_ts'] - $scan_json['cron']['start_ts'];
	$scan_json['cron']['duration_render'] = format_interval($scan_json['cron']['duration_ts']);
	$scan_json['cron']['start_render'] = date('Y-m-d H:i:s T', $scan_json['cron']['start_ts']);
	$scan_json['cron']['end_render'] = date('Y-m-d H:i:s T', $scan_json['cron']['end_ts']);
	sirius_minilog_reset();
	sirius_json_set($scan_node, $scan_json);
	node_save($scan_node);
	sirius_minilog("Updated scan node: " . $scan_node->nid);

	return array('success' => TRUE, 'msg' => $msg);
}

function sirius_cron_scan_get_nid($key, $create = FALSE) {
	$info = sirius_cron_scan_info($key);
	$type = "cron:scan:$key";

  $sql = "select nid ";
  $sql .= "from node ";
  $sql .= "left join field_data_field_sirius_type on field_data_field_sirius_type.entity_type = 'node' and field_data_field_sirius_type.entity_id = node.nid ";
  $sql .= "where node.type = 'sirius_json_definition' ";
  $sql .= "and field_sirius_type_value = :type ";
  $sql_args[':type'] = $type;
  $sql .= "order by created desc limit 1";
  $stmt = db_query($sql, $sql_args);
  $hr = $stmt->fetchAssoc();
  if ($hr['nid']) { return $hr['nid']; }

	if (!$create) { return NULL; }

	global $user;
  $node = new stdClass();
  $node->type = 'sirius_json_definition';
  $node->title = 'Cron Scan: ' . $info['title'];
  $node->no_recurse = TRUE;
  $node->language = LANGUAGE_NONE;
  $node->uid = $user->uid;
  $node->status = 1;
  $node->promote = 0;
  $node->comment = 0;
  $node->field_sirius_type['und'][0]['value'] = $type;
  $node = node_submit($node);
  node_object_prepare($node);
  node_save($node);
  return $node->nid;
}

function sirius_cron_scan_get($key, $create = FALSE) {
	$nid = sirius_cron_scan_get_nid($key, $create);
	if (!$nid) { return NULL; }
	return node_load($nid);
}

/**
 * Implements hook_sirius_backlinks_from_node_alter
 */

function sirius_cron_sirius_backlinks_from_node_alter(&$links, $node) {
	if ($node->type == 'sirius_json_definition' && sirius_cron_scan_key($node)) {
		$links['sirius/cron/list'] = array('name' => 'All Scans');
	}
}
