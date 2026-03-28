<?php

/*
function sirius_smf_node_tab_worker_echp_access($worker_node) {
	return TRUE;
}
*/

function sirius_smf_node_tab_worker_echp_form($form, &$form_state, $worker_node) {
	sirius_ledger_charge_plugin_trigger_info_all();
	sirius_require('sirius_smf', 'sirius_smf.echp.inc');

	// Show staff the error message
	$echp = Sirius_Smf_Echp::getInstance();

	$context = array('worker_node' => $worker_node, 'staff_bypass' => FALSE);

	if (user_access('admin') && $_REQUEST['date']) {
		$ts = strtotime($_REQUEST['date']);
		if ($ts) {
			$context['ts'] = $ts;
		}
	}

	$echp->build_context($context);
	$ok = $echp->access('purchase', $context);

	$form_state['worker_nid'] = $worker_node->nid;
	$form['#tree'] = TRUE;

	if (!$ok) {
		$form['intro'] = array('#markup' => '<p><strong>' . "No purchases available: " . $echp->error_msg() . '</strong></p>');
		$form['#disabled'] = TRUE;
		unset($form['submit']);
	}

	if ($ok) {
		$form['month'] = array('#markup' => sirius_fakerender_field('Purchase hours for:', $context['target_m'] . ' / ' . $context['target_y']));
		$form['hrs_worked'] = array('#markup' => sirius_fakerender_field('Hours worked:', $context['hrs_worked']));
		$form['hrs_threshold'] = array('#markup' => sirius_fakerender_field('Threshold:', $context['hrs_threshold']));
		$form['hrs_needed'] = array('#markup' => sirius_fakerender_field('Needed:', $context['hrs_needed']));
		$form['price'] = array('#markup' => sirius_fakerender_field('Cost:', '$' . number_format($context['price'], 2)));

		$form['confirm'] = array(
			'#type' => 'checkboxe',
			'#title' => 'I agree to pay the charge listed above.',
			'#required' => TRUE,
		);

		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit'),
		);
	}

	return $form;
}

function sirius_smf_node_tab_worker_echp_form_submit($form, &$form_state) {
	sirius_ledger_charge_plugin_trigger_info_all();
	sirius_require('sirius_smf', 'sirius_smf.echp.inc');

	$worker_node = node_load($form_state['worker_nid']);

	// @todo: This should be a function in the object, not the UX
	$echp = Sirius_Smf_Echp::getInstance();
	$context = array('worker_node' => $worker_node, 'staff_bypass' => FALSE);

	if (user_access('admin') && $_REQUEST['date']) {
		$ts = strtotime($_REQUEST['date']);
		if ($ts) {
			$context['ts'] = $ts;
		}
	}

	$echp->build_context($context);

	$hrs = $context['hrs_needed'];
	if (!$hrs) {
		drupal_set_message("No hours needed.", 'error');
		return;
	}

	$dept_tid = sirius_taxonomy_lookup_by_anything('grievance_department', 'default');
  if (!$dept_tid) {
  	drupal_set_message("No department found with ID='default'.", 'error');
  	return;
  }

	$hour_type_tid = $echp->hour_type_tid();
  if (!$hour_type_tid) {
  	drupal_set_message("No hour type found with ID='echp'.", 'error');
  	return;
  }

	// Get the payperiod node
	$payperiod_node = sirius_hour_payperiod_load(
		sirius_domain_nid(), 
		$context['worker_node']->nid, 
		$context['echp_employer_node']->nid, 
		$context['target_y'],
		$context['target_m'],
		1,
		TRUE
	);

	$payperiod_json = sirius_json_get($payperiod_node);

	$payperiod_json['entries'] = array();
	$payperiod_json['entries']['upload']['source_name'] = 'Event Center Hours Purchasing';
	$payperiod_json['entries']['upload']['status'] = 'live';

	$entry = array();
	$entry['day'] = 1;
	$entry['department'] = $dept_tid;
	$entry['hour_type'] = $hour_type_tid;
	$entry['qty'] = $hrs;
	$entry['note'] = 'Event Center Hours Purchase';
	$entry['comment'] = "$hrs purchased on " . date('c');
	$payperiod_json['entries']['upload']['rows'] = array($entry);
	$payperiod_node->field_sirius_active['und'][0]['value'] = 'Yes';
	sirius_hour_payperiod_compute($payperiod_json);
	sirius_json_set($payperiod_node, $payperiod_json);
	node_save($payperiod_node);
	drupal_set_message("$hrs hours were purchased at a cost of \$$context[price].");
	drupal_set_message("Please go to your account page to pay any outstanding balance. HOURS WILL NOT BE CREDITED UNTIL THE BALANCE IS PAID.");

	$echp->track_purchase($context);

	return;
}