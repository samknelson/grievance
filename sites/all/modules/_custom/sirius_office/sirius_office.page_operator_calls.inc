<?php

function sirius_office_node_tab_operator_calls_access($operator_node) {
	return sirius_office_node_tab_operator_status_access($operator_node);
}

function sirius_office_node_tab_operator_call_render_worker($operator_node, $worker_node, $log_node, $json, $authenticated_by, $jwa_prefs) {
  $worker_nid = $worker_node->nid;

  if ($jwa_prefs && $jwa_prefs['hour_type']) {
    $sql = "select ";
    $sql .= sirius_dispatch_jwa_hours_clause($jwa_prefs);
    $sql .= '1 as ignoreme ';
    $sql .= "from sirius_hours_cache where worker_nid = :worker_nid ";
    $stmt = db_query($sql, array(':worker_nid' => $worker_nid));
    $hr = $stmt->fetchAssoc();
    $hours = sirius_dispatch_jwa_hours_render($jwa_prefs, $hr);
  }

  $work_status_tid = $worker_node->field_sirius_work_status['und'][0]['tid'];
  $sql = "select name from taxonomy_term_data where tid = :tid";
  $stmt = db_query($sql, array(':tid' => $work_status_tid));
  $hr = $stmt->fetchAssoc();
  $work_status_name = $hr['name'];

  $html = '<div style="float: right;">';
  $html .= sirius_dispatch_view_render_field($worker_node, 'field_sirius_headshot');
  $html .= '</div>';
  $html .= date('Y-m-d h:i A', $log_node->created) . ' / ';
  $html .= sirius_phone_format($json['from']);
  $html .= '<br />';
  $html .= '<strong>' . $worker_node->title . '</strong>' . ' - ' . $work_status_name;
  if ($hours) {
    $html .= " - Hours: $hours";
  }
  $html .= '<br />';
  $html .= l('View', "node/$worker_nid", array('attributes' => array('class' => array('tool-button'))));
  $html .= ' ';
  $html .= l('Dispatch Status', "node/$worker_nid/sirius_worker_dispatches/status", array('attributes' => array('target' => '_blank', 'class' => array('tool-button'))));
  $html .= ' ';
  $html .= l('Dispatches', "node/$worker_nid/sirius_worker_dispatches/current", array('attributes' => array('target' => '_blank', 'class' => array('tool-button'))));
  $html .= ' ';
  $html .= l('Preferences', "node/$worker_nid/sirius_worker_dispatches/sirius_worker_dispatch_preferences", array('attributes' => array('target' => '_blank', 'class' => array('tool-button'))));
  $html .= '<br />Authenticated by: ' . $authenticated_by;
  $html .= '<br style="clear: right;" />';
  return $html;
}

function sirius_office_node_tab_operator_calls($operator_node) {
	$json = json_decode($operator_node->field_sirius_json['und'][0]['value'], TRUE);
	$phone = $operator_node->field_sirius_phone['und'][0]['value'];
	if (!$phone) {
		return "There is no phone number assigned to this operator.";
	}
	$phone_format = sirius_phone_format_e164($phone);

  $jwa_prefs = sirius_dispatch_jwa_prefs();

  if ($json['status']['status'] == 'open') {
  	$html = '<h2>';
  	$html .= 'Calls forwarded to ' . $operator_node->title . ' (' . $phone . ') ';
  	$html .= 'in the last 4 hours (max 50)';
  	$html .= '</h2>';
   } else {
    $html = '<h2>The Office Is Closed</h2>';
    $which = $json['status']['which'];
    $text = $json['status']["msg_$which"];
    $html .= '<div><em>' . $text . '</em></div>';
   }

  // Find any recent forwarded calls
  $cutoff = strtotime('-4000 hours');
	$query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_log');
  $query->propertyCondition('created', $cutoff, '>=');
  $query->fieldCondition('field_sirius_category', 'value', 'twilio:call');
  $query->fieldCondition('field_sirius_type', 'value', 'dial');
  $query->fieldCondition('field_sirius_message', 'value', "%to $phone_format%", 'LIKE'); // @todo: Should be an attribute we can query directly
  $query->propertyOrderBy('created', 'desc');
  $query->range(0,50);
  $stmt = $query->execute();
  if (!$stmt['node']) { 
  	$html .= 'No calls found.';
  	return $html;
  }
  $log_nids = array_keys($stmt['node']);
  $log_nodes = node_load_multiple($log_nids);
  foreach ($log_nodes as $log_nid => $log_node) {
  	$json = json_decode($log_node->field_sirius_json['und'][0]['value'], TRUE);
  	$from = sirius_phone_format($json['from']);

		$html .= '<div class="sirius_call">';

    $html .= '<hr>';

  	if ($json['worker_nid']) {
  		$worker_nid = $json['worker_nid'];
  		$worker_node = node_load($worker_nid);
      $html .= sirius_office_node_tab_operator_call_render_worker($operator_node, $worker_node, $log_node, $json, 'Signed in to the phone system.', $jwa_prefs);
  	} else if ($from) {
			$query = new EntityFieldQuery();
		  $query->entityCondition('entity_type', 'node');
		  $query->entityCondition('bundle', 'sirius_worker');
		  $query->fieldCondition('field_sirius_phone', 'value', $from);
		  $query->propertyOrderBy('nid', 'desc');
		  $query->range(0, 10);
		  $stmt = $query->execute();
		  if (!$stmt['node']) {
        $html .= date('Y-m-d h:i A', $log_node->created) . ' / ';
        $html .= sirius_phone_format($json['from']);
        $html .= '<br />';
		  	$html .= 'No matching worker found.<br />';
		  } else {
		  	$worker_nodes = node_load_multiple(array_keys($stmt['node']));
        $total = count($worker_nodes);
        $count = 0;
		  	foreach ($worker_nodes as $worker_nid => $worker_node) {
          $count++;
          if ($count > 1) { $html .= '<hr>'; }
          $html .= sirius_office_node_tab_operator_call_render_worker($operator_node, $worker_node, $log_node, $json, "Phone number lookup, match #$count / $total", $jwa_prefs);
		  	}
		  }
  	} else {
      $html .= date('Y-m-d h:i A', $log_node->created) . ' / ';
      $html .= sirius_phone_format($json['from']);
      $html .= '<br />';
  		$html .= "<strong>Unknown number</strong>";
  	}

		$html .= '</div>';
  }

	return $html;
}