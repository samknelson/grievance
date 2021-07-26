<?php

function sirius_twilio_node_tab_call_worker_access($call_node) {
	sirius_require('sirius_twilio', 'sirius_twilio.page_call.inc');
	return sirius_twilio_node_tab_call_access($call_node);
}

function sirius_twilio_node_tab_call_worker($call_node) {
	return drupal_get_form('sirius_twilio_node_tab_call_worker_form', $call_node);
}

function sirius_twilio_node_tab_call_worker_form($form, &$form_state, $call_node) {
	$form_state['call_nid'] = $call_node->nid;

	$call_json = sirius_json_get($call_node);
	if ($call_json['context']['operator_nid']) { sirius_backlink_add('node/' . $call_json['context']['operator_nid'] . '/sirius_operator_calls', array('name' => 'Incoming Calls')); }
	$form['#tree'] = TRUE;

	$worker_node = sirius_log_handler($call_node, 'sirius_worker');
	$html = '<strong>' . t('Current Worker(s)') . '</strong><br />';
	if ($worker_node) {
		$form['action'] = array(
			'#type' => 'hidden',
			'#value' => 'clear',
		);

		$html = "This call is currently associated with the worker <strong>" . $worker_node->title . '</strong>';
		$html .= l('View', 'node/' . $worker_node->nid, array('attributes' => array('class' => array('tool-button'))));
		$html .= '<hr>';
		$form['intro'] = array('#markup' => $html);

		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Un-Link This Worker'),
		);

	} else {
		$form['action'] = array(
			'#type' => 'hidden',
			'#value' => 'set',
		);

		form_load_include($form_state, 'inc', 'sirius_twilio', 'sirius_twilio.page_call_worker');
		form_load_include($form_state, 'inc', 'sirius_event', 'sirius_event.register_worker_byname');
	  drupal_add_css(drupal_get_path('module', 'sirius_event') . '/css/event.css', 'file');

	  $worker = sirius_worker_get_by_phone('8454537272');
	  $workers = array($worker->nid => $worker->title);
	  $worker_nids = array_keys($workers);
	  
	  $form['table_open'] = array('#markup' => '<table class="columns_table">');
	  $form['col1_open'] = array('#markup' => '<tr><td style="width: 66%; vertical-align: top;">');

	  $form['workers']['prefix'] = array('#markup' => '<div id="workers">');

	  $from_phone = sirius_phone_format($call_json['context']['from']);
	  if ($from_phone) { 
	  	$worker_guess = sirius_worker_get_by_phone($from_phone);
	  	if ($worker_guess) {
		  	$guess_str = $worker_guess->title . ' (' . $worker_guess->nid . ')';
	  		$guess_msg = "Note: the call came from <strong>$from_phone</strong>, which is associated with the worker <strong>" . $worker_guess->title . '</strong>.';
	  	} else {
	  		$guess_msg = "Note: the call came from <strong>$from_phone</strong>, which is not associated with any workers.";
	  	}
	  	$guess_msg = '<p>' . $guess_msg . '</p>';
	  	$form['workers']['guess_msg'] = array('#markup' => $guess_msg);
	  }

	  $form['workers'][0] = array(
	    '#title' => t('Choose a Worker'),
	    '#description' => t('Enter or scan the worker\'s dispatch ID, SSN, or name.'),
	    '#type' => 'textfield',
	    '#autocomplete_path' => 'sirius/ajax/bullpen-quicksearch',
	    '#ajax' => array(
	      'callback' => 'sirius_event_register_worker_byname_form_ajax_preview',
	      'wrapper' => 'sirius_worker_preview',
	      'method' => 'replace',
	      'effect' => 'fade',
	    ),
	    '#sirius_ajax' => array(
	      'delta' => 0,
	    ),
	    '#default_value' => $guess_str,
	  );

	  $form['workers']['suffix'] = array('#markup' => '</div>');

	  $form['submit'] = array(
	    '#type' => 'submit',
	    '#value' => 'Link call to Selected Worker',
	  );
	  $form['col1_close'] = array('#markup' => '</td>');

	  $form['col2_open'] = array('#markup' => '<td style="width: 33%; vertical-align: top;">');
	  $worker_str = $form_state['input']['workers'][0];
	  if (!isset($worker_str) && $guess_str) { $worker_str = $guess_str; }
	  $form['worker_preview'] = array(
	    '#type' => 'markup',
	    '#markup' => sirius_event_register_worker_byname_render_preview($worker_str),
	  );
	  $form['col2_close'] = array('#markup' => '</td></tr></table>');

	  return $form;
	}

	return $form;
}

function sirius_twilio_node_tab_call_worker_form_submit($form, &$form_state) {
	$call_node = node_load($form_state['call_nid']);
	$recording_node = sirius_log_handler($call_node, 'sirius_log');

	$action = $form_state['values']['action'];
	if ($action == 'clear') {
		$worker_node = sirius_log_handler($call_node, 'sirius_worker');
		if ($worker_node) { 
			sirius_log_handler_clear($call_node, $worker_node->nid);
			node_save($call_node);
		}
		if ($worker_node && $recording_node) {
			sirius_log_handler_clear($recording_node, $worker_node->nid);
			node_save($recording_node);
		}
		drupal_set_message("The call has been un-linked from the worker.");
	} else if ($action == 'set') {
		$worker_str = $form_state['values']['workers'][0];
	  $result = sirius_event_register_worker_byname_worker($worker_str);
    if ($result['success']) {
    	$worker_node = node_load($result['worker_nid']);
			if ($worker_node) { 
				sirius_log_handler_set($call_node, $worker_node->nid);
				node_save($call_node);
			}
			if ($worker_node && $recording_node) {
				sirius_log_handler_set($recording_node, $worker_node->nid);
				node_save($recording_node);
			}
			drupal_set_message("The call has been linked with the worker <strong>" . $worker_node->title . "</strong>");
    } else {
			drupal_set_message($result['msg'], 'warning');
    }
	} else {
		drupal_set_message("Unknown action.", 'error');
	}

}