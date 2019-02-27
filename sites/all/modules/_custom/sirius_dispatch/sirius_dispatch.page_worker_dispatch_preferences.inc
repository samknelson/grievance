<?php

function sirius_node_tab_worker_dispatch_preferences_access($worker_node) {
	return node_access('view', $worker_node);
}

function sirius_node_tab_worker_dispatch_preferences($worker_node) {
	return drupal_get_form('sirius_node_tab_worker_dispatch_preferences_form', $worker_node);
	/*
	$html = '<ul>';
	$html .= '<li>Set skills that you are available for</li>';
	$html .= '<li>Set preferred notification method</li>';
	$html .= '<li>JOURNEYMEN ONLY: Set whether you are on the out-of-work list.</li>';
	$html .= '</ul>';
	return $html;
	*/
}

function sirius_node_tab_worker_dispatch_preferences_form($form, &$form_state, $worker_node) {
	$form_state['worker_node'] = $worker_node;

	drupal_set_title($worker_node->title . ': Dispatch Preferences');

	global $user;

	// 
	// Dispatch methods
	//

	$form['notification_intro'] = array(
		'#type' => 'markup',
		'#markup' => '<div class="sirius_form_intro">' . 
			'<h2>' . t('Notifications') . '</h2>' . 
			'</div>',
	);

	$form['phone'] = array(
		'#title' => t('Primary Phone Number'),
		'#type' => 'textfield',
		'#size' => 15,
		'#default_value' => $worker_node->field_sirius_phone['und'][0]['value'],
	);

	$form['phone_alt'] = array(
		'#title' => t('Secondary Phone Number'),
		'#type' => 'textfield',
		'#size' => 15,
		'#default_value' => $worker_node->field_sirius_phone_alt['und'][0]['value'],
	);

	$form['email'] = array(
		'#title' => t('Email Address'),
		'#type' => 'textfield',
		'#default_value' => $worker_node->field_sirius_email['und'][0]['value'],
	);

	$form['aat'] = array(
		'#title' => t('Access Code'),
		'#type' => 'textfield',
		'#default_value' => $worker_node->field_sirius_aat['und'][0]['value'],
		'#description' => t('Enter a six-digit numeric passcode that will let you sign in to the web site or to the phone system.'),
		'#required' => TRUE,
		'#size' => 6,
	);

	$medium_options = array(NULL => t('-- Select --'));
	$medium_options += sirius_dispatch_notify_medium_options();

	$form['medium'] = array(
		'#title' => t('Send the first dispatch notification by:'),
		'#type' => 'select',
		'#options' => $medium_options,
		'#default_value' => $worker_node->field_sirius_dispatch_medium['und'][0]['value'],
	);

	$form['medium_1'] = array(
		'#title' => t('Send the second dispatch notification by:'),
		'#type' => 'select',
		'#options' => $medium_options,
		'#default_value' => $worker_node->field_sirius_dispatch_medium['und'][1]['value'],
	);

	// 
	// Auto Sign-In?
	//

	$form['asi_intro'] = array(
		'#type' => 'markup',
		'#markup' => '<div class="sirius_form_intro">' . 
			'<h2>' . t('Auto Sign-In') . '</h2>' . 
			t('When a dispatch ends, would you like to be automatically placed on the "available" list?') .
			'</div>',
	);

	$form['asi'] = array(
		'#title' => '',
		'#type' => 'select',
		'#options' => array(
			'No' => t('No, when a dispatch ends I will sign in to the out of work list manually'),
			'Yes' => t('Yes, when a dispatch ends I should be automatically signed in to the out of work list'),
		),
		'#default_value' => $worker_node->field_sirius_dispatch_asi['und'][0]['value'],
	);

	//
	// Skils
	//

	if ($worker_node->field_sirius_skill['und']) {
		$skill_options = array();
		foreach ($worker_node->field_sirius_skill['und'] as $tmp) {
			$skill_tid = $tmp['tid'];
			$skill_term = taxonomy_term_load($skill_tid);
			$skill_options[$skill_tid] = $skill_term->name;
		}

		$skill_defaults = array();
		$default_restricted = 'all';
		if ($worker_node->field_sirius_skills_avail['und']) {
			foreach ($worker_node->field_sirius_skills_avail['und'] as $tmp) {
				$skill_tid = $tmp['tid'];
				$skill_defaults[$skill_tid] = $skill_tid;
				$default_restricted = 'restrict';
			}
		}
		$form['skills_intro'] = array(
			'#type' => 'markup',
			'#markup' => '<div class="sirius_form_intro">' . 
				'<h2>' . t('Skill Preferences') . '</h2>' . 
				t('If you would prefer to only receive dispatches for certain skills, select them below. Remember that if you select specific skills, you will not receive certain open-call dispatches.') .
				'</div>',
		);

		$form['skills_restricted'] = array(
			'#title' => '',
			'#type' => 'select',
			'#options' => array(
				'all' => t('Send me dispatches for all skills'),
				'restrict' => t('Send me dispatches only for certain skills'),
			),
			'#default_value' => $default_restricted,
		);

		$form['skills_avail'] = array(
			'#title' => t('I only want to receive dispatches for the skills checked below.'),
			'#type' => 'checkboxes',
			'#options' => $skill_options,
			'#default_value' => $skill_defaults,
			'#states' => array(
				'visible' => array(
					array(':input[name="skills_restricted"]' => array('value' => 'restrict')),
				)
			),
		);
	}

	//
	// DNCs
	//

	$form['dncs2_intro'] = array(
		'#type' => 'markup',
		'#markup' => '<div class="sirius_form_intro">' . 
			'<h2>' . t('Worker-Initiated Do Not Call') . '</h2>' . 
			t('If you would prefer not to receive dispatches from certain employers, select them below.') .
			'</div>',
	);

	$employer_options = array(NULL => t('-- Select --'));
	$employer_options += sirius_employer_list();
	$max_delta = 2;
	if (is_array($worker_node->field_sirius_dispatch_dncs2['und'])) {
		$max_delta += count($worker_node->field_sirius_dispatch_dncs2['und']);
	}
	for ($delta=0; $delta<$max_delta; ++$delta) {
		$form['dncs2_nid_' . $delta] = array(
			'#type' => 'select',
			'#title' => 'Employer #' . ($delta+1),
			'#options' => $employer_options,
			'#default_value' => $worker_node->field_sirius_dispatch_dncs2['und'][$delta]['target_id'],
		);
	}

	//
	// CBN Only
	//

	$form['cbn'] = array(
		'#type' => 'select',
		'#title' => 'Call by Name Only?',
		'#options' => array(
			'No' => 'No, I want both open calls and call-by-names',
			'Yes' => 'Yes, I want call by names only. I will not receive open calls.',
		),
		'#default_value' => $worker_node->field_sirius_dispatch_cbn['und'][0]['value'],
	);

	//
	// HFE
	//

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
			'#prefix' => '<div class="sirius_form_intro">' . 
				'<h2>' . t('Hold for Employer') . '</h2>' . 
				t('If you are expecting a dispatch from a specific employer, you can temporarily reject calls from all other employers.') .
				'<br /><br /><strong>' . 
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
			'#prefix' => '<div class="sirius_form_intro">' . 
				'<h2>' . t('Hold for Employer') . '</h2>' . 
				t('If you are expecting a dispatch from a specific employer, you can temporarily reject calls from all other employers.') .
				'<br /><br /><strong>' . 
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
		'#options' => $employer_options,
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
		),
		'#default_value' => 72,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Submit',
	);

	return $form;
}

function sirius_node_tab_worker_dispatch_preferences_form_validate($form, &$form_state) {
	$phone = $form_state['values']['phone'];
	if ($phone && !sirius_phone_validate($phone)) {
		form_set_error('phone', t('Please enter a valid phone number.'));
	}

	$phone_alt = $form_state['values']['phone_alt'];
	if ($phone_alt && !sirius_phone_validate($phone_alt)) {
		form_set_error('phone_alt', t('Please enter a valid phone number.'));
	}

	$email = $form_state['values']['email'];
	if ($email && !sirius_email_validate($email)) {
		form_set_error('email', t('Please enter a valid email address.'));
	}

	$medium = $form_state['values']['medium'];
	if ( ($medium == 'sms' || $medium == 'voice') && !$phone) {
		form_set_error('medium', t('Please enter a primary phone number.'));
	}

	if ( ($medium == 'sms_alt' || $medium == 'voice_alt') && !$phone_alt) {
		form_set_error('medium', t('Please enter a secondary phone number.'));
	}

	if ( ($medium == 'email') && !$email) {
		form_set_error('medium', t('Please enter an email address.'));
	}

	$medium_alt = $form_state['values']['medium_1'];
	if ( ($medium_alt == 'sms' || $medium_alt == 'voice') && !$phone) {
		form_set_error('medium_alt', t('Please enter a primary phone number.'));
	}

	if ( ($medium_alt == 'sms_alt' || $medium_alt == 'voice_alt') && !$phone_alt) {
		form_set_error('medium_alt', t('Please enter a secondary phone number.'));
	}

	if ( ($medium_alt == 'email') && !$email) {
		form_set_error('medium_alt', t('Please enter an email address.'));
	}

	if ($medium && $medium_alt && $medium == $medium_alt) {
		form_set_error('medium_alt', t('Primary and secondary notification methods must be different.'));
	}

	$aat = $form_state['values']['aat'];
	if (!preg_match('/^\d\d\d\d\d\d$/', $aat)) {
		form_set_error('aat', t('The access code must be a six-digit number.'));
	}

	if ($form_state['values']['hfe_action'] == 'Set') {
		if (!$form_state['values']['hfe']) {
			form_set_error('hfe', t('Please select an employer.'));
		}
	}
}


function sirius_node_tab_worker_dispatch_preferences_form_submit($form, &$form_state) {
	$worker_node = $form_state['worker_node'];
	global $user;

	// Set phone, email
	$need_save = FALSE;
	$val = sirius_phone_format($form_state['values']['phone']);
	if ($val != $worker_node->field_sirius_phone['und'][0]['value']) {
		$worker_node->field_sirius_phone['und'][0]['value'] = $val;
		$need_save = TRUE;
		drupal_set_message(t('Set primary phone number to @val', array('@val' => ($val?$val:"[NULL]"))));
	}

	$val = sirius_phone_format($form_state['values']['phone_alt']);
	if ($val != $worker_node->field_sirius_phone_alt['und'][0]['value']) {
		$worker_node->field_sirius_phone_alt['und'][0]['value'] = $val;
		$need_save = TRUE;
		drupal_set_message(t('Set secondary phone number to @val', array('@val' => ($val?$val:"[NULL]"))));
	}

	$val = sirius_email_format($form_state['values']['email']);
	if ($val != $worker_node->field_sirius_email['und'][0]['value']) {
		$worker_node->field_sirius_email['und'][0]['value'] = $val;
		$need_save = TRUE;
		drupal_set_message(t('Set email address to @val', array('@val' => ($val?$val:"[NULL]"))));
	}

	$val = $form_state['values']['medium'];
	if ($val != $worker_node->field_sirius_dispatch_medium['und'][0]['value']) {
		$worker_node->field_sirius_dispatch_medium['und'][0]['value'] = $val;
		$need_save = TRUE;
		drupal_set_message(t('Primary dispatch medium set to @val', array('@val' => ($val?$val:"[NULL]") )));
	}

	$val = $form_state['values']['medium_1'];
	if ($val != $worker_node->field_sirius_dispatch_medium['und'][1]['value']) {
		$worker_node->field_sirius_dispatch_medium['und'][1]['value'] = $val;
		$need_save = TRUE;
		drupal_set_message(t('Secondary dispatch medium set to @val', array('@val' => ($val?$val:"[NULL]"))));
	}

	// AAT
	$val = $form_state['values']['aat'];
	if ($val != $worker_node->field_sirius_aat['und'][0]['value']) {
		$worker_node->field_sirius_aat['und'][0]['value'] = $val;
		$need_save = TRUE;
		drupal_set_message(t('Access code set to @val', array('@val' => ($val?$val:"[NULL]"))));
	}

	// CBN
	$val = $form_state['values']['cbn'];
	if ($val != $worker_node->field_sirius_dispatch_cbn['und'][0]['value']) {
		$worker_node->field_sirius_dispatch_cbn['und'][0]['value'] = $val;
		$need_save = TRUE;
		drupal_set_message(t('"Call by Name Only?" set  to @val', array('@val' => ($val?$val:"[NULL]"))));
	}

	// HFE
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

	// Clear skill preferences
	if ($form_state['values']['skills_restricted'] == 'all') {
		if ($worker_node->field_sirius_skills_avail['und']) {
			$worker_node->field_sirius_skills_avail['und'] = array();
			$need_save = TRUE;
			sirius_log('dispatch:skills_avail', 'clear', 'Skill preferences have been cleared', '', '', array($worker_node->nid), array());
			drupal_set_message('Skill preferences have been removed.');
		}
	}

	// Set skill preferences
	if ($form_state['values']['skills_restricted'] == 'restrict') {
		$old_skills = array();
		if ($worker_node->field_sirius_skills_avail['und']) {
			foreach ($worker_node->field_sirius_skills_avail['und'] as $tmp) {
				$old_skills[$tmp['tid']] = TRUE;
			}
		}
		$old_skills = array_keys($old_skills);

		$new_skills = array();
		foreach (array_values($form_state['values']['skills_avail']) as $tid) {
			if ($tid) {
				$new_skills[$tid] = TRUE;
			}
		}
		$new_skills = array_keys($new_skills);

		if (array_diff($old_skills, $new_skills) || array_diff($new_skills, $old_skills)) {
			$worker_node->field_sirius_skills_avail['und'] = array();
			foreach ($new_skills as $tid) {
				$worker_node->field_sirius_skills_avail['und'][] = array('tid' => $tid);
			}
			$need_save = TRUE;
			sirius_log('dispatch:skills_avail', 'set', 'Skill preferences have been set', '', '', array($worker_node->nid), array());
			drupal_set_message('Skill preferences have been set.');
		}
	}

	// Set worker-initiated DNCs
	for ($delta = 0; $delta < 1000; ++$delta) {
		$new_employer_nid = $form_state['values']['dncs2_nid_' . $delta];
		$old_employer_nid = $worker_node->field_sirius_dispatch_dncs2['und'][$delta]['target_id'];
		if ($old_employer_nid == $new_employer_nid) { continue; }

		if ($new_employer_nid) {
			$employer_node = node_load($new_employer_nid);
			sirius_dispatch_dnc2_set(
				$worker_node, 
				$employer_node, 
				TRUE, 
				t('DNC requested by @name', array('@name' => $user->name)),
				array()
			);
			drupal_set_message(t('Employer @employer has been added to the worker-initiated Do Not Call list for @worker', array('@employer' => $employer_node->title, '@worker' => $worker_node->title)));
		}

		if ($old_employer_nid) {
			$employer_node = node_load($old_employer_nid);
			sirius_dispatch_dnc2_set(
				$worker_node, 
				$employer_node, 
				FALSE, 
				t('DNC cleared by @name', array('@name' => $user->name)),
				array()
			);
			drupal_set_message(t('Employer @employer has been removed from the worker-initiated Do Not Call list for @worker', array('@employer' => $employer_node->title, '@worker' => $worker_node->title)));
		}
	}

	// ASI
	$old_asi = $worker_node->field_sirius_dispatch_asi['und']['0']['value'];
	$new_asi = $form_state['values']['asi'];
	if ($old_asi != $new_asi) {
		$worker_node->field_sirius_dispatch_asi['und']['0']['value'] = $new_asi;
		$need_save = TRUE;
		sirius_log('dispatch:asi', 'set', 'Auto-sign-in preferences have been set to ' . $new_asi, '', '', array($worker_node->nid), array());
		drupal_set_message('Auto-sign-in preferences have been set to ' . $new_asi . '.');
	}

	if ($need_save) {
		node_save($worker_node);
	}
}
