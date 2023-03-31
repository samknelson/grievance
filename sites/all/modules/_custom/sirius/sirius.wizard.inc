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
  $wizard_infos = &drupal_static(__FUNCTION__); 
  if (!isset($wizard_infos)) { 
  	$wizard_infos = sirius_wizard_info_all();
  }

	// @todo: This is a terrible kludge, put in place to avoid touching lots of feeds
	//        Sinze we're no longer supporting XLSX files, there's no need to have a sheet-selection step.
	//        The right thing to do is to remove it from the configuration for each feed.
	//        But I'm lazy and scared to touch every feed, so I'm doing this atrocious thing here.
	unset($wizard_infos[$wizard_name]['steps']['sheet']);

	return $wizard_infos[$wizard_name];
}

function sirius_wizard_info_all() {
  $info_all = &drupal_static(__FUNCTION__);

  if (!isset($info_all)) {
		// We want to do this:
		// $wizard_infos = module_invoke_all('sirius_wizard_info');
		// But we want to add information about the module that defines it. So we call each function manually.

		$info_all = array();
		$hook = 'sirius_wizard_info';
		foreach (module_implements($hook) as $module) {
			$function = $module . '_' . $hook;
    	if (function_exists($function)) {
    		$module_info = system_get_info('module', $module);
      	$result = call_user_func_array($function, array());
      	if (isset($result) && is_array($result)) {
        	$info_all = array_merge_recursive($info_all, $result);
      		// Add the module to the results, so we know which module provides which report.
      		foreach ($result as $key => $info) {
      			$info_all[$key]['module'] = $module;
      			$info_all[$key]['module_name'] = $module_info['name'];
      		}
      	} elseif (isset($result)) {
        	$info_all[] = $result;
      	}
    	}
  	}

		uksort($info_all, function($a, $b) use ($info_all) {
			$result = strcasecmp($info_all[$a]['module_name'], $info_all[$b]['module_name']);
			if (!$result) { $result = strcasecmp($info_all[$a]['title'], $info_all[$b]['title']); }
			return $result;
		});
  }

	return $info_all;
}

function sirius_wizard_steps($info, $data = NULL, $node = NULL) {
	// Get the list of steps.
	if ($info['callback_step_list']) {
		$steps = call_user_func($info['callback_step_list'], $data, $node);
	} else {
		$steps = $info['steps'];
	}

	$context = array('info' => $info, 'data' => $data, 'node' => $node);
	drupal_alter('sirius_wizard_steps', $steps, $context);

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
	$args = $steps[$current_step]['callback_args'];
	if (!$callback) { $callback = 'sirius_wizard_content_default'; }

	if ($steps[$current_step]['file']) { 
		sirius_require($steps[$current_step]['module'], $steps[$current_step]['file']);
	}

	if (preg_match('/_form$/', $callback)) {
		if ($args) {
			return drupal_get_form($callback, $info, $data, $node, $current_step, $args);	
		} else {
			return drupal_get_form($callback, $info, $data, $node, $current_step);	
		}
	} else {
		if ($args) {
			return call_user_func($callback, $info, $data, $node, $current_step, $args);	
		} else {
			return call_user_func($callback, $info, $data, $node, $current_step);	
		}
	}
}

function sirius_wizard_content_default($info, $data, $node, $current_step) {
	return array('#markup' => "The wizard [$info[name]] is misconfigured. Please specific a value for callback_step_content[$current_step].");
}

function sirius_wizard_data($info, $node) {
	$wizard_name = $info['name'];
	$json = sirius_jsonfield_util_node_json_get($node);
	return $json['wizard'][$wizard_name];
}

function sirius_wizard_data_set($info, &$node, $data) {
	$wizard_name = $info['name'];
	$json = sirius_jsonfield_util_node_json_get($node);
	$json['wizard'][$wizard_name] = $data;
	sirius_jsonfield_util_node_json_set($node, $json);
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

  $items['feed_worker_skill'] = array(
    'name' => 'feed_worker_skill',
    'title' => 'Worker Skills',
    'description' => 'Create or upload worker skills and expiration dates.',
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
    	'batch_size' => 500,
	    'callback_row_verify' => 'sirius_feed_worker_skill_row_verify',
	    'callback_row_process' =>'sirius_feed_worker_skill_row_process',
    ),
    'callback_launch' => 'sirius_feed_wizard_launch',
    'callback_launch_args' => array('feed_worker_skill'),
  );

  $items += sirius_feed_contact_info();
  $items += sirius_feed_grievance_info();
  $items += sirius_feed_grievance_log_info();
  $items += sirius_letter_wizard_info();
  $items += sirius_feed_worker_info();
  $items += sirius_feed_wws_info();
  $items += sirius_feed_employee_info();
  $items += sirius_feed_hours_info();
  $items += sirius_feed_user_info();

  return $items;
}

function sirius_wizard_submit($info, $data, &$node, $current_step, $msg = NULL, $options = array()) {
	// Mark this step as complete
	if ($options['incomplete']) { $data['steps'][$current_step]['complete'] = FALSE; } else { $data['steps'][$current_step]['complete'] = TRUE;	}

	// Mark all subsequent steps as incomplete
	$steps = sirius_wizard_steps($info, $data, $node);
	$found = FALSE;
	foreach ($steps as $step_name => $ignore) {
		if ($step_name == $current_step) { $found = TRUE; continue; }
		if (!$found) { continue; }
		if ($options['nonsequential']) { continue; }
		$data['steps'][$step_name]['complete'] = FALSE;
	}

	// Save our data
	sirius_wizard_data_set($info, $node, $data);

	// Set a friendly message
	if (!$msg) { $msg = "Data saved for step: " . $steps[$current_step]['label']; }
	drupal_set_message($msg);

	$alterable = array(
		'info' => $info, 
		'data' => $data,
		'node' => $node,
		'current_step' => $current_step,
		'msg' => $msg,
		'options' => $options
	);
	drupal_alter('sirius_wizard_submit', $alterable);

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
