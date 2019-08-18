<?php


///////////////////////////////////////////////////////////////////
// View 
///////////////////////////////////////////////////////////////////

function sirius_node_tab_dispatch_bullpen_access($job_node) {
	if ($job_node->field_sirius_dispatch_job_type['und'][0]['tid'] != sirius_get_special_tid('sirius_dispatch_job_type|BULL')) { return FALSE; }
	// @todo: Who has access to this page?
	return TRUE;
}

function sirius_node_tab_dispatch_bullpen($job_node) {
	drupal_set_title($job_node->title); 

  $html .= '<div class="sirius_dispatch_job_intro">';
  $count = $job_node->field_sirius_count['und'][0]['value'];
  if (!$count) { $count = 0; }
  $count_yes = $job_node->field_sirius_count_yes['und'][0]['value'];
  if (!$count_yes) { $count_yes = 0; }
  if ($count) {
  	$percent = 100 * $count_yes / $count;
	  $html .= theme_progress_bar(array('percent' => $percent, 'message' => "$count_yes / $count workers have accepted."));
  }
  $html .= t('To contact the dispatch operator regarding this bullpen, reference job ID <strong>#@job_nid</strong>.', array('@job_nid' => $job_node->nid));
  $html .= '<br /><br />';
  $html .= '</div>';
	$renderable = node_view($job_node, 'full');
	$html .= drupal_render($renderable);

	return $html;	

	return drupal_get_form('sirius_node_tab_dispatch_bullpen_form', $job_node);
}

function sirius_node_tab_bullpen_info($job_node) {
	$ts = sirius_dispatch_bullpen_signin_ts($job_node);
	$datestr = 'g:i A, l, F j';
	$style = "display: inline-block; width: 170px; font-weight: bold;";


  $html = '<div class="sirius_help"><div class="inner">';
  $html .= sirius_fakerender_field('EBA Sign-in Opens', date($datestr, $ts['signin_opens_self']));
  $html .= sirius_fakerender_field('General Sign-in Opens:', date($datestr, $ts['signin_opens_others']));
  $html .= sirius_fakerender_field('Sign-in Closes:', date($datestr, $ts['signin_closes']));
  $html .= sirius_fakerender_field('Time Now:', date($datestr));
  $html .= render(field_view_field('node', $job_node, 'field_sirius_representatives', 'full'));
  $html .= '</div></div>';

	return $html;
}

///////////////////////////////////////////////////////////////////
// Signin Self 
///////////////////////////////////////////////////////////////////

function sirius_node_tab_dispatch_bullpen_signin_self_access($job_node) { 
	return sirius_node_tab_dispatch_bullpen_access($job_node);
}

function sirius_node_tab_dispatch_bullpen_signin_self($job_node) {
	drupal_set_title($job_node->title); 
	return drupal_get_form('sirius_node_tab_dispatch_bullpen_signin_self_form', $job_node);
}

function sirius_node_tab_dispatch_bullpen_signin_self_form($form, &$form_state, $job_node) {
	$form_state['job_node'] = $job_node;
	$worker_node = sirius_worker_get();

	$dispatch_node = sirius_dispatch_bullpen_signin_get_dispatch($job_node, $worker_node);
	$status = $dispatch_node->field_sirius_dispatch_status['und'][0]['value'];

	$form['info'] = array('#markup' => sirius_node_tab_bullpen_info($job_node));

	if ($status) {
		$form['msg'] = array('#markup' => '<div>Your current status is: <strong>' . $status . '</strong></div>');
	} else {
		$form['msg'] = array('#markup' => '<div>Your current status is: <strong>Not Signed In</strong></div>');
	}

	$tmp = sirius_dispatch_bullpen_signin_possible($job_node, $worker_node, FALSE);
	if (!$tmp['success']) {
		if ($tmp['msg'] != 'You are not signed in to this bullpen.') {
			drupal_set_message("You cannot sign out of this bullpen: " . $tmp['msg'], 'warning');
		}
	} else {
		$form['signout'] = array(
			'#title' => 'Sign out of this bullpen',
			'#type' => 'checkbox',
			'#required' => TRUE,
		);
		$need_submit = TRUE;
	}

	$tmp = sirius_dispatch_bullpen_signin_possible($job_node, $worker_node, TRUE);
	if (!$tmp['success']) {
		if ($tmp['msg'] != 'You are signed in to this bullpen.') {
			drupal_set_message("You cannot sign in to this bullpen: " . $tmp['msg'], 'warning');
		}
	} else {
		$form['signin'] = array(
			'#title' => 'Sign in to this bullpen',
			'#type' => 'checkbox',
			'#required' => TRUE,
		);
		$need_submit = TRUE;
	}

	if ($need_submit) {
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => 'Go',
		);
	}

	return $form;
}

function sirius_node_tab_dispatch_bullpen_signin_self_form_submit($form, &$form_state) {
	$job_node = $form_state['job_node'];
	$worker_node = sirius_worker_get();

	if ($form_state['values']['signin']) { $in = TRUE; } else { $in = FALSE; }
	$result = sirius_dispatch_bullpen_signin($job_node, $worker_node, $in);
	if (!$result['success']) {
		drupal_set_message("Action failed: " . $result['msg'], 'warning');
	} else {
		if ($in) {
			drupal_set_message("You are now signed in to this bullpen.");
		} else {
			drupal_set_message("You are now signed out of this bullpen.");
		}
		drupal_goto('node/' . $job_node->nid . "/sirius_dispatch_bullpen");
	}
}

///////////////////////////////////////////////////////////////////
// Signin Others
///////////////////////////////////////////////////////////////////

function sirius_node_tab_dispatch_bullpen_signin_others_access($job_node) {
	// Must have access to the bullpen
	if (!sirius_node_tab_dispatch_bullpen_access($job_node)) { return FALSE; }

	// If you have "sign-in others" permission, you can see this page
	if (user_access('sirius dispatch bullpen signin others')) { return TRUE; }

	// Employers can see the sign-in list for their own jobs.
	if (user_access('sirius dispatch bullpen signin view')) {
		$employer_node = sirius_employer_get();
		if ($employer_node->nid && $employer_node->nid == $job_node->field_grievance_shop['und'][0]['target_id']) { return TRUE; }
	}

	return FALSE;
}

function sirius_node_tab_dispatch_bullpen_signin_others($job_node) {
	drupal_set_title($job_node->title); 
	$html .= sirius_node_tab_bullpen_info($job_node);
	$html .= drupal_render(drupal_get_form('sirius_node_tab_dispatch_bullpen_signin_others_form', $job_node));
	$html .= views_embed_view('sirius_dispatches', 'block_2', $job_node->nid);
	return $html;
}

function sirius_node_tab_dispatch_bullpen_signin_others_form($form, &$form_state, $job_node) {
	$form_state['job_node'] = $job_node;

	$open = sirius_dispatch_bullpen_signin_open($job_node, FALSE);
	if ($open['success'] && user_access('sirius dispatch bullpen signin others')) {
	  $form['worker'] = array(
	    '#title' => t('Sign-in a Worker'),
	    '#type' => 'textfield',
	    '#autocomplete_path' => 'sirius/ajax/bullpen-quicksearch',
			'#size' => 25,
			'#description' => t('Enter or scan the worker\'s dispatch ID, SSN, or name.'),
	   );
	 
	  $form['submit'] = array(
	    '#type' => 'submit',
	    '#value' => 'Sign In',
	  );
	}

	return $form;
}

function sirius_node_tab_dispatch_bullpen_signin_others_form_submit($form, &$form_state) {
	$job_node = $form_state['job_node'];
	$worker_str = $form_state['values']['worker'];
	if (!$worker_str) { 
		drupal_set_message("Please select a worker.", 'warning');
		return;
	}
	
	$matches = array();
	if (preg_match('/\s+\((\d+)\)/', $worker_str, $matches) && $matches[1]) { 
		$worker_nid = $matches[1];
	}
	if (!$worker_nid) {
		drupal_set_message("Please make sure that you have selected a a matching worker.", 'warning');
		return;
	}

	$worker_node = node_load($worker_nid);	

	$result = sirius_dispatch_bullpen_signin($job_node, $worker_node, TRUE);
	if (!$result['success']) {
		drupal_set_message("Sign-in failed: " . $result['msg'], 'warning');
	} else {
		drupal_set_message(t('@worker [@nid] has been signed in.', array('@worker' => $worker_node->title, '@nid' => $worker_node->nid)));
	}
}

///////////////////////////////////////////////////////////////////
// Admin
///////////////////////////////////////////////////////////////////

function sirius_dispatch_bullpen_admin() {
  $form = array();

  $var = 'sirius_dispatch_bullpen_signin_opens';
  $form[$var] = array(
    '#type' => 'textfield',
    '#title' => t('Sign-in opens'),
    '#size' => 6,
    '#default_value' => variable_get($var, 60*60*24*5),
    '#description' => t("The number of seconds before the registered start time for a bullpoen, when workers can sign themselves in"),
  );

  $var = 'sirius_dispatch_bullpen_signin_opens_others';
  $form[$var] = array(
    '#type' => 'textfield',
    '#title' => t('Sign-in opens (others)'),
    '#size' => 6,
    '#default_value' => variable_get($var, 60*60),
    '#description' => t("The number of seconds before the registered start time for a bullpen, when stewards can sign in other workers."),
  );

  $var = 'sirius_dispatch_bullpen_signin_grace';
  $form[$var] = array(
    '#type' => 'textfield',
    '#title' => t('Sign-in grace period (in seconds)'),
    '#size' => 6,
    '#default_value' => variable_get($var, 900),
    '#description' => t("The number of seconds after the registered start time for a bullpen when workers can still sign in."),
  );

  return system_settings_form($form);
}