<?php

function sirius_dispatch_node_tab_worker_pref_hfe_access($worker_node) {
	// Can't see this tab if you're sign-in blocked
	if (!user_access('sirius dispatch staff')) {
		if (class_exists('Sirius_Sib') && !Sirius_Sib::getInstance()->access('dispatch_signin_self', array('entity_node' => $worker_node))) { return FALSE; }
	}

	return TRUE; 
}

function sirius_dispatch_node_tab_worker_pref_hfe_form($form, &$form_state, $worker_node) {
	$form_state['worker_nid'] = $worker_node->nid;

	$employer_options = sirius_employer_list();

	list ($current_hfe, $current_hfe_until) = sirius_dispatch_hfe_get($worker_node);
	if ($current_hfe) {
		$current_hfe_name = $employer_options[$current_hfe];
		$current_hfe_until_format = date('Y-m-d H:i', $current_hfe_until);
		$form['hfe_action'] = array(
			'#title' => '',
			'#type' => 'select',
			'#options' => array(
				'Leave' => t('Leave the current hold'),
				'Clear' => t('Clear the current hold'),
				'Set' => t('Clear the current hold and set a new one'),
			),
			'#prefix' => '<div class="sirius_form_intro"><strong>' . 
				t('You are currently holding for @current_hfe_name until @current_hfe_until_format.', array('@current_hfe_name' => $current_hfe_name, '@current_hfe_until_format' => $current_hfe_until_format)) .
				'</strong>' .
				'</div>',
			'#default_value' => $default_restricted,
		);
	} else {
		$form['hfe_action'] = array(
			'#title' => '',
			'#type' => 'select',
			'#options' => array(
				'Leave' => t('Continue accepting all calls'),
				'Set' => t('Set a new hold'),
			),
			'#prefix' => '<div class="sirius_form_intro"><strong>' . 
				t('You have no active hold.') .
				'</strong>' .
				'</div>',
			'#default_value' => $default_restricted,
		);
	}

	$form['hfe_wrapper'] = array(
		'#type' => 'fieldset',
		'#states' => array(
      'visible' => array(
				array(':input[name="hfe_action"]' => array('value' => 'Set')),
      ),
		),
	);

	$form['hfe_wrapper']['hfe'] = array(
		'#type' => 'select',
		'#title' => 'Hold for:',
		'#options' => array(NULL => t('-- Select --')) + $employer_options,
		'#default_value' => NULL,
	);

	$form['hfe_wrapper']['hfe_hrs'] = array(
		'#type' => 'select',
		'#title' => 'Hold for:',
		'#options' => array(
			24 => '24 Hours',
			36 => '36 Hours',
			48 => '48 Hours',
			72 => '72 Hours',
			96 => '96 Hours',
			24*365 => '1 year',
		),
		'#default_value' => 72,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_dispatch_node_tab_worker_pref_hfe_form_validate($form, &$form_state) {
	if ($form_state['values']['hfe_action'] == 'Set') {
		if (!$form_state['values']['hfe']) {
			form_set_error('hfe', t('Please select an employer.'));
		}
	}
}

function sirius_dispatch_node_tab_worker_pref_hfe_form_submit($form, &$form_state) {
	$worker_node = node_load($form_state['worker_nid']);

	$hfe_action = $form_state['values']['hfe_action'];
	if ($hfe_action == 'Clear') {
		sirius_dispatch_hfe_clear($worker_node);
		drupal_set_message(t('"Hold for employer" has been cleared'));
	}
	if ($hfe_action == 'Set') {
		$employer_nid = $form_state['values']['hfe'];
		$employer_node = node_load($employer_nid);
		$hfe_hrs = $form_state['values']['hfe_hrs'];
		$hfe_ts = time() + (60*60*$hfe_hrs);
		sirius_dispatch_hfe_set($worker_node, $employer_node, $hfe_ts);
		$need_save = TRUE;
		drupal_set_message(t('You will not receive any new dispatches except for those from @val until @date at @time.', array('@val' => $employer_node->title, '@date' => date('l, F d ', $hfe_ts), '@time' => date('h:i a', $hfe_ts))));
	}

	if ($need_save) { node_save($worker_node); }
}
