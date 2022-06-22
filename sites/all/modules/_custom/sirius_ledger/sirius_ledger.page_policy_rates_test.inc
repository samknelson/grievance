<?php 

function sirius_ledger_node_tab_policy_rates_test_access($policy_node) {
	return sirius_ledger_node_tab_policy_definition_access($policy_node);
}

function sirius_ledger_node_tab_policy_rates_test($policy_node) {
	return drupal_get_form('sirius_ledger_node_tab_policy_rates_test_form', $policy_node);
}

function sirius_ledger_node_tab_policy_rates_test_form($form, &$form_state, $policy_node) {
	$form_state['policy_nid'] = $policy_node->nid;
	$policy_json = sirius_json_get($policy_node);

	$form['mode'] = array(
		'#type' => 'select',
		'#title' => t('Mode'),
		'#required' => TRUE,
		'#options' => array(
			'test' => t('Test mode - execute all the steps but do not save any charges.'),
			'live' => t('Live mode - create or update A/R entries'),
		),
	);

	$form['trigger'] = array(
		'#type' => 'select',
		'#title' => t('Trigger'),
		'#options' => array(NULL => t('-- Select --')) + sirius_ledger_policy_trigger_options(),
		'#required' => TRUE,
	);

	$form['date'] = array(
		'#type' => 'date_popup',
		'#title' => t('As Of'),
    '#attributes' => array('autocomplete' => 'off'),
    '#description' => t('Leave blank to make charges "as of" today.'),
	);

  $form['worker'] = array(
    '#title' => t('Worker'),
    '#type' => 'textfield',
    '#autocomplete_path' => 'sirius/ajax/bullpen-quicksearch',
    '#description' => t('Enter or scan the worker\'s dispatch ID, SSN, or name.'),
	);

  $form['employer'] = array(
    '#title' => t('Employer'),
    '#type' => 'select',
    '#options' => array(NULL => t('-- Select --')) + sirius_employer_list(),
    '#states' => array('visible' => array(":input[name=\"trigger\"]" => array('value' => 'hour'))),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

  $log = sirius_minilog_render_html();
  if ($log) { $form['results'] = array('#markup' => '<hr>' . $log); }

	return $form;
}

function sirius_ledger_node_tab_policy_rates_test_form_submit($form, &$form_state) {
	$policy_node = node_load($form_state['policy_nid']);

	$ts = strtotime($form_state['values']['date']);
	if ($ts <= 1) { $ts = NULL; }

	$form_state['rebuild'] = TRUE;

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
    drupal_set_message("Please make sure that you have selected a matching worker.", 'warning');
    return;
  }

  $worker_node = node_load($worker_nid);
  if ($worker_node->type != 'sirius_worker') {
    drupal_set_message("That is not a worker record.", 'warning');
    return;
  }
  if (!node_access('update', $worker_node))  {
    drupal_set_message("You do not have access to that worker.", 'warning');
    return;
  }

  $mode = $form_state['values']['mode'];
  $trigger = $form_state['values']['trigger'];

  if ($trigger == 'hour') {
		$year = date('Y', $ts);
		$month = date('m', $ts);
		$day = date('d', $ts);
		$employer_nid = $form_state['values']['employer'];
		if (!$employer_nid) {
			drupal_set_message("Please select an employer.", 'warning');
			return;
		}

		$employer_title = sirius_node_title($employer_nid);
		sirius_minilog("Loading payperiod for worker " . $worker_node->title . " and employer $employer_title as of $year-$month-$day.");
		$payperiod_node = sirius_hour_payperiod_load(sirius_domain_nid(), $worker_node->nid, $form_state['values']['employer'], $year, $month, $day);
		if (!$payperiod_node) {
			sirius_minilog("No payperiod entry found.", 'warning');
			return;
		}
		$total_charged = sirius_ledger_payperiod_set($payperiod_node, $mode, array('policy_node' => $policy_node));
	} else {
		$total_charged = sirius_ledger_monthly_charge_worker($worker_nid, $mode, $trigger, $ts, 'generate', array('policy_node' => $policy_node));
	}
	drupal_set_message("Total charged: " . '$' . number_format($total_charged, 2));
}	
