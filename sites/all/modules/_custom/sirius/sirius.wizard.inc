<?php


function sirius_node_tab_wizard($node, $wizard_name) {
	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/wizard.css');

	// Which wizard?
	$info = sirius_wizard_info($wizard_name);
	if (!$info) { return "No wizard found for $wizard_name"; }

	$data = sirius_wizard_data($info, $node);
	$steps = sirius_wizard_steps($info, $data, $node);
	$current_step = sirius_wizard_current_step($steps);

	$html = sirius_wizard_render_steps($steps, $current_step);
	$renderable = sirius_wizard_content($info, $data, $node, $current_step);
	$html .= drupal_render($renderable);

	return $html;
}

function sirius_wizard_render_steps($steps, $current_step) {
	$html = '<ul class="sirius_wizard">';
	foreach ($steps as $step_name => $step) {
		$complete = $step['complete'];
		$access = $step['access'];

		$class = "sirius_wizard_step ";
		if ($access) { $class .= " sirius_wizard_step_clickable"; }
		if ($complete) { $class .= " sirius_wizard_step_complete"; } else { $class .= " sirius_wizard_step_incomplete"; }
		if ($step_name == $current_step) { $class .= " sirius_wizard_step_current"; }

		$html .= "<li class=\"$class\">";
		if ($access) { $html .= '<a href="/' . current_path() . '?step=' . $step_name . '">'; } else { $html .= '<span>'; }
		$html .= $step['label'];
		if ($access) { $html .= '</a>'; } else { $html .= '</span>'; }
		$html .= '</li>';
	}
	$html .= '</ul>';
	return $html;
}

function sirius_wizard_info($wizard_name) {
	$wizard_infos = module_invoke_all('sirius_wizard_info');
	return $wizard_infos[$wizard_name];
}

function sirius_wizard_info_all() {
	$all = module_invoke_all('sirius_wizard_info');
	ksort($all);
	return $all;
}

function sirius_wizard_steps($info, $data = NULL, $node = NULL) {
	// Get the list of steps.
	if ($info['callback_step_list']) {
		$steps = call_user_func($info['callback_step_list'], $data, $node);
	} else {
		$steps = $info['steps'];
	}

	// Get the list of complete steps
	$prior_complete = TRUE;
	foreach ($steps as $step_name => $ignore) {
		$complete = $data['steps'][$step_name]['complete'];
		$steps[$step_name]['complete'] = $complete;
		$steps[$step_name]['access'] = $prior_complete;
		$prior_complete = $prior_complete && $complete;
	}
	return $steps;
}

function sirius_wizard_current_step($steps) {
	// Get the current step from the query string.
	$aiming_for = $_REQUEST['current_step'];

	foreach ($steps as $step_name => $step) {
		// If a prior step is incomplete, return it.
		if (!$step['complete']) { return $step_name; }

		// If it's in the request string, and all prior steps are complete, then return it
		if ($step_name == $_REQUEST['step']) { return $step_name; }
	}

	// All steps are complete, so we return the last one
	return $step_name;
}

function sirius_wizard_content($info, $data, $node, $current_step) {
	$steps = sirius_wizard_steps($info, $data, $node);
	$callback = $steps[$current_step]['callback'];
	if (!$callback) { $callback = 'sirius_wizard_content_default'; }
	return call_user_func($callback, $info, $data, $node, $current_step);	
}

function sirius_wizard_content_default($info, $data, $node, $current_step) {
	return array('#markup' => "The wizard [$info[name]] is misconfigured. Please specific a value for callback_step_content[$current_step].");
}

function sirius_wizard_data($info, $node) {
	$wizard_name = $info['name'];
	$json = json_decode($node->field_sirius_json['und'][0]['value'], TRUE);
	return $json['wizard'][$wizard_name];
}

function sirius_wizard_data_set($info, $node, $data) {
	$wizard_name = $info['name'];
	$json = json_decode($node->field_sirius_json['und'][0]['value'], TRUE);
	$json['wizard'][$wizard_name] = $data;
	$node->field_sirius_json['und'][0]['value'] = json_encode($json);
  node_object_prepare($node);
  $node = node_submit($node);
	return node_save($node);
}

function sirius_wizard_launch($info) {
	$callback = $info['callback_launch'];
	if ($info['callback_launch_args']) { $args = $info['callback_launch_args']; } else { $args = array(); }
	call_user_func_array($callback, $args);
}

/***
 * Implements hook_sirius_wizard_info() 
 */

function sirius_sirius_wizard_info() {
  $items = array();

  $items['demo'] = array(
    'name' => 'demo',
    'title' => 'Demo',
    'description' => 'Demo Wizard',
    'callback_step_list' => 'sirius_wizard_demo_step_list',
    'steps' => array(
			'a' 	=> array('label' => 'Demo Step A', 	'callback' => 'sirius_wizard_demo_content'),
			'b'		=> array('label' => 'Demo Step B', 	'callback' => 'sirius_wizard_demo_content'),
			'c' 	=> array('label' => 'Demo Step C', 	'callback' => 'sirius_wizard_demo_content'),
			'd' 	=> array('label' => 'Demo Step D', 	'callback' => 'sirius_wizard_demo_content'),
			'e' 	=> array('label' => 'Demo Step E', 	'callback' => 'sirius_wizard_demo_content'),
    ),
  );

  $items['feed_wws'] = array(
    'name' => 'feed_wws',
    'title' => 'Worker Work Status',
    'description' => 'Upload a new list of worker work statuses.',
    'steps' => array(
			'upload' 		=> array('label' => 'Upload', 		'callback' => 'sirius_feed_wizard_upload'),
			'sheet' 		=> array('label' => 'Worksheet', 	'callback' => 'sirius_feed_wizard_sheet'),
			'map' 			=> array('label' => 'Mapping', 		'callback' => 'sirius_feed_wizard_map'),
			'verify' 		=> array('label' => 'Verify', 		'callback' => 'sirius_feed_wizard_verify'),
			'process' 	=> array('label' => 'Process', 		'callback' => 'sirius_feed_wizard_process'),
			'review' 		=> array('label' => 'Review', 		'callback' => 'sirius_feed_wizard_review'),
    ),
    'feed' => array(
    	'fields' => array(
    		'ssn' => array(
    			'title' => 'SSN',
    			'required' => TRUE,
    		),
    		'work_status_id' => array(
    			'title' => 'Work Status ID',
    			'required' => TRUE,
    		),
    	),
    	'batch_size' => 50,
	    'callback_row_verify' => 'sirius_feed_wws_row_verify',
	    'callback_row_process' =>'sirius_feed_wws_row_process',
    ),
    'callback_launch' => 'sirius_feed_wizard_launch',
    'callback_launch_args' => array('feed_wws'),
  );

  $items['feed_hours'] = array(
    'name' => 'feed_hours',
    'title' => 'Hours',
    'description' => 'Upload a new list of hours.',
    'steps' => array(
			'upload' 		=> array('label' => 'Upload', 		'callback' => 'sirius_feed_wizard_upload'),
			'sheet' 		=> array('label' => 'Worksheet', 	'callback' => 'sirius_feed_wizard_sheet'),
			'map' 			=> array('label' => 'Mapping', 		'callback' => 'sirius_feed_wizard_map'),
			'verify' 		=> array('label' => 'Verify', 		'callback' => 'sirius_feed_wizard_verify'),
			'process' 	=> array('label' => 'Process', 		'callback' => 'sirius_feed_wizard_process'),
			'review' 		=> array('label' => 'Review', 		'callback' => 'sirius_feed_wizard_review'),
    ),
    'feed' => array(
	    'pks' => array(
	    	'id' => 'ID',
	    	'id3' => 'ID3',
	    	'ssn' => 'SSN',
	    ),
    	'fields' => array(
    		'id' => array('title' => 'ID'),
    		'ssn' => array('title' => 'SSN'),
    		'id3' => array('title' => 'ID3'),
    		'hour_type' => array('title' => 'Hour Type'),
    		'employer' => array('title' => 'Employer'),
    		'date' => array('title' => 'Date'),
    		'hours' => array('title' => 'Hours'),
    		'department' => array('title' => 'Department'),
    		'notes1' => array('title' => 'Notes 1'),
    		'notes2' => array('title' => 'Notes 2'),
    		'notes3' => array('title' => 'Notes 3'),
    	),
    	'batch_size' => 50,
	    'callback_row_verify' => 'sirius_feed_hours_row_verify',
	    'callback_row_process' =>'sirius_feed_hours_row_process',
    ),
    'callback_launch' => 'sirius_feed_wizard_launch',
    'callback_launch_args' => array('feed_hours'),
  );

  $items['feed_contact'] = array(
    'name' => 'feed_contact',
    'title' => 'Contacts',
    'description' => 'Upload a new list of contacts.',
    'steps' => array(
			'upload' 		=> array('label' => 'Upload', 		'callback' => 'sirius_feed_wizard_upload'),
			'sheet' 		=> array('label' => 'Worksheet', 	'callback' => 'sirius_feed_wizard_sheet'),
			// 'tag' 			=> array('label' => 'Tag', 				'callback' => 'sirius_feed_wizard_tag'),
			'map' 			=> array('label' => 'Mapping', 		'callback' => 'sirius_feed_wizard_map'),
			'verify' 		=> array('label' => 'Verify', 		'callback' => 'sirius_feed_wizard_verify'),
			'process' 	=> array('label' => 'Process', 		'callback' => 'sirius_feed_wizard_process'),
			'review' 		=> array('label' => 'Review', 		'callback' => 'sirius_feed_wizard_review'),
    ),
    'feed' => array(
    	'batch_size' => 50,
	    'callback_row_verify' => 'sirius_feed_contact_row_verify',
	    'callback_row_process' =>'sirius_feed_contact_row_process',
			'callback_fields' => 'sirius_feed_contact_fields',
    ),
    'callback_launch' => 'sirius_feed_wizard_launch',
    'callback_launch_args' => array('feed_contact'),
  );

  $items['feed_worker_skill'] = array(
    'name' => 'feed_worker_skill',
    'title' => 'Worker Skills',
    'description' => 'Upload worker skills and expiration dates.',
    'steps' => array(
			'upload' 		=> array('label' => 'Upload', 		'callback' => 'sirius_feed_wizard_upload'),
			'sheet' 		=> array('label' => 'Worksheet', 	'callback' => 'sirius_feed_wizard_sheet'),
			'map' 			=> array('label' => 'Mapping', 		'callback' => 'sirius_feed_wizard_map'),
			'verify' 		=> array('label' => 'Verify', 		'callback' => 'sirius_feed_wizard_verify'),
			'process' 	=> array('label' => 'Process', 		'callback' => 'sirius_feed_wizard_process'),
			'review' 		=> array('label' => 'Review', 		'callback' => 'sirius_feed_wizard_review'),
    ),
    'feed' => array(
	    'pks' => array(
	    	'id' => 'ID',
	    	'ssn' => 'SSN',
	    ),
    	'fields' => array(
    		'id' => array('title' => 'ID'),
    		'ssn' => array('title' => 'SSN'),
    		'skill' => array('title' => 'Skill', 'description' => 'Enter the skill name exactly as it appears, or the short name, ID, External ID, or NID.'),
    		'skill_expiration' => array('title' => 'Expiration Date'),
    		'skill_action' => array('title' => 'Action', 'description' => 'Enter "add" or "remove"'),
    		'skill_comment' => array('title' => 'Comment'),
    	),
    	'batch_size' => 50,
	    'callback_row_verify' => 'sirius_feed_worker_skill_row_verify',
	    'callback_row_process' =>'sirius_feed_worker_skill_row_process',
    ),
    'callback_launch' => 'sirius_feed_wizard_launch',
    'callback_launch_args' => array('feed_worker_skill'),
  );

  $items += sirius_feed_grievance_info();
  $items += sirius_feed_grievance_log_info();
  $items += sirius_letter_wizard_info();
  $items += sirius_feed_worker_info();

  return $items;
}

function sirius_wizard_submit($info, $data, $node, $current_step, $msg = NULL) {
	// Mark this step as complete
	$data['steps'][$current_step]['complete'] = TRUE;

	// Mark all subsequent steps as incomplete
	$steps = sirius_wizard_steps($info, $data, $node);
	$found = FALSE;
	foreach ($steps as $step_name => $ignore) {
		if ($step_name == $current_step) { $found = TRUE; continue; }
		if (!$found) { continue; }
		$data['steps'][$step_name]['complete'] = FALSE;
	}

	// Save our data
	sirius_wizard_data_set($info, $node, $data);

	// Set a friendly message
	if (!$msg) { $msg = "Data saved for step: " . $steps[$current_step]['label']; }
	drupal_set_message($msg);

	// If we requested a specific step in the URL, we have to clear it.
	if ($_REQUEST['step']) {
		drupal_goto(current_path());
	}
}

/************************************************************************************************
 * Demo Wizard
 ************************************************************************************************/

function sirius_wizard_demo_step_list($data, $node) {
	return array(
	);
}

function sirius_wizard_demo_content($info, $data, $node, $current_step) {
	return drupal_get_form('sirius_wizard_demo_content_anystep', $info, $data, $node, $current_step);
}

function sirius_wizard_demo_content_anystep($form, &$form_state, $info, $data, $node, $current_step) {
	$form_state['info'] = $info;
	$form_state['data'] = $data;
	$form_state['node'] = $node;
	$form_state['current_step'] = $current_step;
	$steps = sirius_wizard_steps($info, $data, $node);

	$form['a_random_variable'] = array(
		'#type' => 'textfield',
		'#title' => 'Input for Step ' . $steps[$current_step]['label'],
		'#default_value' => $data['steps'][$current_step]['data']['a_random_variable'],
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Submit',
	);

	return $form;
}

function sirius_wizard_demo_content_anystep_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$node = $form_state['node'];
	$current_step = $form_state['current_step'];

	// Save the data for this step
	$data['steps'][$current_step]['data']['a_random_variable'] = $form_state['values']['a_random_variable'];

	sirius_wizard_submit($info, $data, $node, $current_step);
}
