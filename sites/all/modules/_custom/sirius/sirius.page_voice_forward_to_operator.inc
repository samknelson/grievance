<?php

function sirius_page_voice_forward_to_operator() {
  $hours = $_REQUEST['hours'];
  if (!$hours) { $hours = 4; }

  if (variable_get('sirius_voice_forward_to_operator_status', 'open') == 'open') {
	  $html = '<h2>' . t('Calls forwarded to operator in the last @hours hours (max 50)', array('@hours' => $hours)) . '</h2>';
   } else {
    $html = '<h2>The Office Is Closed</h2>';
    $which = variable_get('sirius_voice_forward_to_operator_disabled_which', 1);
    $text = variable_get("sirius_voice_forward_to_operator_disabled_$which", "We're sorry, the office is closed right now.");
    $html .= '<div><strong>Incoming calls will be answered with the message:</strong> <em>' . $text . '</em></div>';
   }

  // Find any recent forwarded calls
  $cutoff = strtotime("-$hours hours");
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
    $call_sid = sirius_twilio_get_call_sid_from_node($log_node);
    if ($call_sid) {
      $result = sirius_twilio_call_fetch($call_sid);
      if ($result['success']) {
        $call = $result['call'];
        drupal_set_message("Call: $call_sid");
        dpm($call->toArray());
      }
      if ($result['recording']) {
        $recording = $result['recording'];
        drupal_set_message("Recording: ");
        dpm($recording->toArray());
      }
    }
  }

  foreach ($log_nodes as $log_nid => $log_node) {
  	$json = json_decode($log_node->field_sirius_json['und'][0]['value'], TRUE);
  	$from = sirius_phone_format($json['context']['from']);

		$html .= '<div class="sirius_call">';

    $html .= '<hr>';

  	if ($json['worker_nid']) {
  		$worker_nid = $json['worker_nid'];
  		$worker_node = node_load($worker_nid);
      $html .= '<div style="float: left; width: 275px;">';
      $html .= sirius_dispatch_view_render_field($worker_node, 'field_sirius_headshot');
      $html .= '</div>';
      $html .= date('Y-m-d h:i A', $log_node->created) . ' / ';
      $html .= sirius_phone_format($from);
      $html .= '<br />';
      $html .= '<strong>' . $worker_node->title . '</strong><br />';
      $html .= l('View', "node/$worker_nid", array('attributes' => array('class' => array('tool-button'))));
  		$html .= ' ';
  		$html .= l('Dispatch Status', "node/$worker_nid/sirius_worker_dispatches/status", array('attributes' => array('target' => '_blank', 'class' => array('tool-button'))));
  		$html .= ' ';
  		$html .= l('Dispatches', "node/$worker_nid/sirius_worker_dispatches/current", array('attributes' => array('target' => '_blank', 'class' => array('tool-button'))));
  		$html .= ' ';
  		$html .= l('Preferences', "node/$worker_nid/sirius_worker_dispatches/sirius_worker_dispatch_preferences", array('attributes' => array('target' => '_blank', 'class' => array('tool-button'))));
      $html .= '<br />Authenticated by: Signed in to the phone system';
  		$html .= '<br style="clear: left;" />';
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
        $html .= sirius_phone_format($from);
        $html .= '<br />';
		  	$html .= 'No matching worker found.<br />';
		  } else {
		  	$worker_nodes = node_load_multiple(array_keys($stmt['node']));
        $total = count($worker_nodes);
        $count = 0;
		  	foreach ($worker_nodes as $worker_nid => $worker_node) {
          $count++;
          if ($count > 1) { $html .= '<hr>'; }
          $html .= '<div style="float: left; width: 275px; min-height: 165px;">';
          $html .= sirius_dispatch_view_render_field($worker_node, 'field_sirius_headshot');
          $html .= '</div>';
          $html .= date('Y-m-d h:i A', $log_node->created) . ' / ';
          $html .= sirius_phone_format($from);
          $html .= '<br />';
          $html .= '<strong>' . $worker_node->title . '</strong><br />';
		  		$html .= l('View', "node/$worker_nid", array('attributes' => array('class' => array('tool-button'))));
		  		$html .= ' ';
		  		$html .= l('Dispatch Status', "node/$worker_nid/sirius_worker_dispatch_status", array('attributes' => array('target' => '_blank', 'class' => array('tool-button'))));
		  		$html .= ' ';
		  		$html .= l('Dispatches', "node/$worker_nid/sirius_worker_dispatches", array('attributes' => array('target' => '_blank', 'class' => array('tool-button'))));
		  		$html .= ' ';
		  		$html .= l('Preferences', "node/$worker_nid/sirius_worker_dispatch_preferences", array('attributes' => array('target' => '_blank', 'class' => array('tool-button'))));
          $html .= "<br />Authenticated by: Phone number lookup, match #$count / $total";
          $html .= '<br style="clear: left;" />';
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


function sirius_page_voice_forward_to_operator_settings() {
  $form['sirius_voice_forward_to_operator_status'] = array(
    '#title' => t('Foward to Operator Status'),
    '#type' => 'select',
    '#options' => array(
      'open' => 'Open',
      'closed' => 'Closed',
    ),
    '#default_value' => variable_get('sirius_voice_forward_to_operator_status', 'open'),
  );

  $form['sirius_voice_forward_to_operator_disabled_which'] = array(
    '#title' => t('When the office is closed, and a user requests the operator, which message should be played?'),
    '#type' => 'select',
    '#options' => array(
      1 => 'Message #1',
      2 => 'Message #2',
      3 => 'Message #3',
      4 => 'Message #4',
      5 => 'Message #5',
    ),
    '#default_value' => variable_get('sirius_voice_forward_to_operator_disabled_which', 1),
  );
  for ($i=1; $i<=5; ++$i) {
    $form["sirius_voice_forward_to_operator_disabled_$i"] = array(
      '#title' => "Message #$i",
      '#type' => 'textarea',
      '#default_value' => variable_get("sirius_voice_forward_to_operator_disabled_$i", ""),
    );
  }

  return system_settings_form($form);
}