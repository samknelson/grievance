<?php

function sirius_worker_denorm_history_queue($log_node) {
  if ($log_node->sirius_nodenorm) { return; }
  $history_info = sirius_worker_history_info($log_node->field_sirius_category['und'][0]['value']);
  if (!$history_info) { return; }
  $parent_node = sirius_log_handler($log_node, $history_info['content_type']);
  if (!$parent_node) { return; }

  sirius_denorm_queue_add($log_node->nid, $parent_node->nid, 'sirius_worker_history');
}

function sirius_worker_denorm_history($denorm_item) {
  // @todo: If we're triggered as a result of a delete, this fails.
  $log_node = node_load($denorm_item['denorm_source_nid']);
  if (!$log_node) { return; }
  $history_key = $log_node->field_sirius_category['und'][0]['value'];
  $history_info = sirius_worker_history_info($history_key);
  if (!$history_info) { return; }
  $parent_node = sirius_log_handler($log_node, $history_info['content_type']);
  if (!$parent_node) { return; }

  $result = sirius_worker_history_calc($parent_node, $history_key);
  watchdog('denorm', $result['msg']);
  return $result;
}

function sirius_worker_history_calc($parent_node, $history_key) {
	sirius_minilog("$history_key: " . $parent_node->title . ": Starting computations.");

  $history_info = sirius_worker_history_info($history_key);
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_log');
  $query->fieldCondition('field_sirius_category', 'value', $history_key);
  $query->fieldCondition('field_sirius_log_handler', 'target_id', $parent_node->nid);
  $query->propertyOrderBy('created');
  $stmt = $query->execute();

  if (!$stmt['node']) { return array('success' => TRUE, 'msg' => 'History is empty.'); }
  $log_nodes = node_load_multiple(array_keys($stmt['node']));

  $changes = array();
  foreach ($log_nodes as $log_nid => $log_node) {
    $log_json = sirius_json_get($log_node);
		sirius_minilog("... Scanning log node " . $log_node->nid . ", found change record " . json_encode($log_json['change']));
    if ($log_json['change']['active'] == 'No') {
      sirius_minilog("... Change is flagged as inactive. Skipping this record.");
      continue;
    }

    /////////////////////////////////////////
    // Begin legacy support
    /////////////////////////////////////////

    if (!$log_json['change']['new_value']) {
      $msg = $log_node->field_sirius_message['und'][0]['value'];
      sirius_minilog("... Change record not found, scanning legacy data with name [$msg]");

      //
      // Legacy support for work status
      //

      if ($history_key == 'worker:work_status') {
        $msg = preg_replace('/.*has been assigned the new work status /', '', $msg);
        $msg = trim($msg);
        $ws_tid = sirius_taxonomy_lookup_by_anything('sirius_work_status', $msg);

        if (!$ws_tid) {
          sirius_minilog("... TID not found. Skipping.");
        	continue;
        }


        $log_json['change'] = array();
        $log_json['change']['ts'] = $log_node->created;
        $log_json['change']['new_value'] = $ws_tid;
      }

      //
      // Legacy support for member status
      // Not supported.
      //
      
      if ($history_key == 'worker:member_status') {
        sirius_minilog("... Legacy history for member status not supported. Skipping.");
        continue;
        /*
        $add_ms_tid = $remove_ms_tid = $remove_name = $add_name = NULL;
        $matches = array();
        if (preg_match('/Removed\: (.+)\. Added\: (.+)\./', $msg, $matches)) {
          $remove_name = $matches[1];
          $add_name = $matches[2];
        } else if (preg_match('/Added\: (.+)\./', $msg, $matches)) {
          $add_name = $matches[1];
        } else if (preg_match('/Removed\: (.+)\./', $msg, $matches)) {
          $remove_name = $matches[1];
        }

        if (!$add_name && !$remove_name) {
          sirius_minilog("... Couldn't parse. Skipping.");
          continue;
        }


        if ($add_name) {
          $add_ms_tid = sirius_taxonomy_lookup_by_anything('sirius_member_status', $add_name);
          if (!$add_ms_tid) {
            sirius_minilog("... Unable to find member status TID associated with [$add_name]. Skipping.");
            continue;
          }
        }

        if ($remove_name) {
          $remove_ms_tid = sirius_taxonomy_lookup_by_anything('sirius_member_status', $remove_name);
          if (!$remove_ms_tid) {
            sirius_minilog("... Unable to find member status TID associated with [$remove_name]. Skipping.");
            continue;
          }
        }

        $value = $change['new_value'];
        if (!$value) { $value = array(); }
        if ($add_ms_tid) { $value[] = $add_ms_tid; }
        if ($remove_ms_tid) { 
          $index = array_search($remove_ms_tid, $value);
          if($index !== false) { unset($value[$index]); }
        }
        $value = array_unique(array_values($value));

        $log_json['change'] = array();
        $log_json['change']['ts'] = $log_node->created;
        $log_json['change']['new_value'] = $value;
        sirius_minilog("... value is now [" . join(',', $value) . '].');
        */
      }

      $log_node->sirius_nodenorm = TRUE; 
      sirius_json_set($log_node, $log_json);
      node_save($log_node);
    }

    /////////////////////////////////////////
    // End legacy support
    /////////////////////////////////////////

    $change = array();
    $change['new_value'] = $log_json['change']['new_value'];
    $change['log_nid'] = $log_node->nid;
    $ts = $log_json['change']['ts'];

    $changes[$ts] = $change;
  }
  sirius_minilog("... Computed " . count($changes) . " change records.");
  ksort($changes);

  $history = array();
  $history['changes'] = $changes;
  $history['ts'] = time();
  if ($changes) {
    $history['last'] = end($changes); 
    reset($changes);
  } else {
    $history['last'] = NULL;
  }

  $parent_json = sirius_json_get($parent_node);

  $result = array();
  $result['success'] = TRUE;
  $result['history'] = $history;

  $orig_history = $parent_json['history'][$history_key];
  if ($history['changes'] && sirius_worker_history_changed($history, $orig_history)) {
    $msg = "$history_key: " . $parent_node->title . ": History changed, node updated.";
    $parent_json['history'][$history_key] = $history;
    sirius_json_set($parent_node, $parent_json);
    $parent_node->sirius_nodenorm = TRUE;

    $callback = $history_info['callback_calc'];
    if ($callback) { $callback($parent_node, $history); }

    node_save($parent_node);
  } else {
    $msg = "$history_key: " . $parent_node->title . ": History unchanged, node save not necessary";
  }

  sirius_minilog($msg);
  $result['msg'] = $msg;
  return $result;
}

function sirius_worker_history_info_all() {
  $info_all = &drupal_static(__FUNCTION__);
  if (!isset($info_all)) {
    $info_all = array();

    $info_all['worker:work_status'] = array(
      'content_type' => 'sirius_worker',
      'callback_calc' => 'sirius_worker_work_status_history_callback'
    );

    $info_all['worker:member_status'] = array(
      'content_type' => 'sirius_worker',
      'callback_calc' => 'sirius_worker_member_status_history_callback'
    );
  }
  return $info_all;
}

function sirius_worker_history_info($history_key) {
  $info_all = sirius_worker_history_info_all();
  return $info_all[$history_key];
}

function sirius_worker_history_render($parent_node, $history_key) {
  $history_info = sirius_worker_history_info($history_key);
	$history = sirius_worker_history($parent_node, $history_key);

  if (!$history) { return ''; }

  if ($history['last']) {
    $html .= sirius_fakerender_field('Last Value:', sirius_worker_history_render_value($history['last']['new_value'], $history_key));
  }

  if ($history['ts']) {
    $html .= sirius_fakerender_field('Computed at:', date('Y-m-d H:i:s T', $history['ts']));
  }

  $changes = $history['changes'];
  if ($changes) {
    // $changes = array_reverse($changes, TRUE);
    $header = array();
    $header[] = 'Date';
    $header[] = 'New Value';
    $header[] = 'Tools';

    $rows = array();

    $current_path = current_path();
    foreach ($changes as $ts => $change) {
      $tools = array();
      // $tools[] = l('View', 'node/' . $change['log_nid'], array('query' => array('destination' => $current_path), 'attributes' => array('class' => array('tool-button'))));
      $tools[] = l('Manage', 'node/' . $change['log_nid'] . '/sirius_log_history', array('query' => array('destination' => $current_path), 'attributes' => array('class' => array('tool-button'))));

      $row = array();
      $row[] = date('Y-m-d H:i:s T', $ts);
      $row[] = sirius_worker_history_render_value($change['new_value'], $history_key);
      $row[] = join(' ', $tools);
      $rows[] = $row;
    }

    $html .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array()));
  }

  return $html;
}

function sirius_worker_history_render_value($value, $history_key) {
  if ($history_key == 'worker:work_status') {
    $term = taxonomy_term_load($value);
    if ($term) { return $term->name; }
  }

  if ($history_key == 'worker:member_status') {
    $html = '';
    foreach ($value as $subvalue) {
      if ($html) { $html .= '<br />'; }
      $term = taxonomy_term_load($subvalue);
      $html .= $term->name;
    }
    return $html;
  }
  
  return $value;
}

function sirius_worker_history($parent_node, $history_key) {
	$json = sirius_json_get($parent_node);
	return $json['history'][$history_key];
}

function sirius_worker_history_changed($history1, $history2) {
  if (!$history1 && !$history2) { return FALSE; }
  if (!$history1 || !$history2) { return TRUE; }

  $key = sirius_worker_history_changed_impl($history1, $history2);
  if (!$key) { $key = sirius_worker_history_changed_impl($history2, $history1); }

  return $key;
}

function sirius_worker_history_changed_impl($arr1, $arr2, $depth = 0) {
  if ($depth > 99) { return FALSE; }

  static $ignore;
  if (!isset($ignore)) {
    $ignore[0]['ts'] = TRUE;
  }

  foreach ($arr1 as $key => $value1) {
    $value2 = $arr2[$key];

    // Some keys we ignore
    if ($ignore[$depth][$key]) { continue; }

    // If neither value1 nore value2 is an array and if they're equal, we ignore them
    if (!is_array($value1) && !is_array($value2) && $value1 == $value) { continue; }

    // If neither value1 nore value2 is an array and they're not equal, it's a change
    if (!is_array($value1) && !is_array($value2) && ($value1 != $value2)) {
      return $key;
    }

    // If value1 is an array and value2 is a non-null non-array, it's a change. Or vice versa.
    if (is_array($value1) && !is_array($value2) && ($value2 != NULL)) { 
      return $key;
    }
    if (is_array($value2) && !is_array($value1) && ($value1 != NULL)) { 
      return $key;
    }

    // At least one of value1 and value2 is an array. We need to recurse.
    if ($subkey = sirius_revision_changed_impl($value1, $value2, $depth+1, $field_name)) {
      return "$key/$subkey";
    }
  }

  return FALSE;
}

function sirius_worker_history_asof($parent_node, $history_key, $ts) {
  $history = sirius_worker_history($parent_node, $history_key);
  if (!$history || !$history['changes']) { return NULL; }

  foreach ($history['changes'] as $history_ts => $history_entry) {
    if ($history_ts <= $ts) {
      $value = $history_entry['new_value'];
    }
    if ($history_ts >= $ts) { break; }
  }
  return $value;
}