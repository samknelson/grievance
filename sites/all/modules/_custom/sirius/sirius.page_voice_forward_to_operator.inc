<?php

function sirius_page_voice_forward_to_operator() {
	$html = '<h2>Calls forwarded to operator in the last 4 hours (max 50)</h2>';

  // Find any recent forwarded calls
  $cutoff = strtotime('-4 hours');
	$query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_log');
  $query->propertyCondition('created', $cutoff, '>=');
  $query->fieldCondition('field_sirius_category', 'value', 'twilio:call');
  $query->fieldCondition('field_sirius_type', 'value', 'dial');
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

		$html .= '<h2>';
  	$html .= date('Y-m-d h:i a', $log_node->created) . ' / ';
  	$html .= sirius_phone_format($json['from']);
  	$html .= '</h2>';

  	if ($json['worker_nid']) {
  		$worker_nid = $json['worker_nid'];
  		$worker_node = node_load($worker_nid);
  		$html .= l('View', "node/$worker_nid", array('attributes' => array('class' => array('tool-button'))));
  		$html .= ' ';
  		$html .= l('Dispatch Status', "node/$worker_nid/sirius_worker_dispatch_status", array('attributes' => array('target' => '_blank', 'class' => array('tool-button'))));
  		$html .= ' ';
  		$html .= l('Dispatches', "node/$worker_nid/sirius_worker_dispatches", array('attributes' => array('target' => '_blank', 'class' => array('tool-button'))));
  		$html .= ' ';
  		$html .= l('Preferences', "node/$worker_nid/sirius_worker_dispatch_preferences", array('attributes' => array('target' => '_blank', 'class' => array('tool-button'))));
  		$html .= ' Signed in as: <strong>' . $worker_node->title . '</strong>';
  		$html .= '<br />';
  	} else if ($from) {
			$query = new EntityFieldQuery();
		  $query->entityCondition('entity_type', 'node');
		  $query->entityCondition('bundle', 'sirius_worker');
		  $query->fieldCondition('field_sirius_phone', 'value', $from);
		  $query->propertyOrderBy('nid', 'desc');
		  $query->range(0, 10);
		  $stmt = $query->execute();
		  if (!$stmt['node']) {
		  	$html .= 'No matching worker found.<br />';
		  } else {
		  	$worker_nodes = node_load_multiple(array_keys($stmt['node']));
		  	foreach ($worker_nodes as $worker_nid => $worker_node) {
		  		$html .= l('View', "node/$worker_nid", array('attributes' => array('class' => array('tool-button'))));
		  		$html .= ' ';
		  		$html .= l('Dispatch Status', "node/$worker_nid/sirius_worker_dispatch_status", array('attributes' => array('target' => '_blank', 'class' => array('tool-button'))));
		  		$html .= ' ';
		  		$html .= l('Dispatches', "node/$worker_nid/sirius_worker_dispatches", array('attributes' => array('target' => '_blank', 'class' => array('tool-button'))));
		  		$html .= ' ';
		  		$html .= l('Preferences', "node/$worker_nid/sirius_worker_dispatch_preferences", array('attributes' => array('target' => '_blank', 'class' => array('tool-button'))));
		  		$html .= ' Phone number lookup: <strong>' . $worker_node->title . '</strong>';
		  		$html .= '<br />';
		  	}
		  }
  	} else {
  		$html .= 'No worker or phone number found.';
  	}

		$html .= '</div>';
  }

	return $html;
}