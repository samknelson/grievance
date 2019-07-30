<?php

function sirius_node_tab_worker_dispatch_preferences_access($worker_node) {
	return node_access('view', $worker_node);
}

function sirius_node_tab_worker_dispatch_preferences($worker_node) {
	return drupal_get_form('sirius_node_tab_worker_dispatch_preferences_form', $worker_node);
}

function sirius_node_tab_worker_dispatch_preferences_form($form, &$form_state, $worker_node) {
	$form_state['worker_node'] = $worker_node;
  drupal_add_library('system', 'ui.accordion');
  drupal_add_js('jQuery(document).ready(function () { jQuery("#sirius_accordion").accordion({active: false, collapsible: true, autoehight: true, heightStyle: "content"}); });', 'inline');
	global $user;

  $form['accordion_open'] = array('#markup' => '<div id="sirius_accordion">');

	// 
	// Dispatch methods
	//

  $form['accordion_open_notification'] = array('#markup' => '<div class="sirius_accordion_label">' . t('Notifications') . '</div><div class="sirius_accordion_body">');

	$medium_options = sirius_dispatch_notify_medium_options($worker_node);

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

	unset($medium_options['voice']);
	unset($medium_options['voice_alt']);
	$form['medium_2'] = array(
		'#title' => t('Send other notifications (skill expirations, termination notifications, etc.) by:'),
		'#type' => 'select',
		'#options' => $medium_options,
		'#default_value' => $worker_node->field_sirius_dispatch_medium['und'][2]['value'],
	);

  $form['accordion_close_notifications'] = array('#markup' => '</div>');

	if (user_access('sirius dispatch staff') || (user_access('sirius dispatch own preferences') && $worker_node->field_sirius_dispatch_sib['und'][0]['value'] != 'Yes')) {

		// 
		// Auto Sign-In?
		//

	  $form['accordion_open_asi'] = array('#markup' => '<div class="sirius_accordion_label">' . t('Auto Sign-In') . '</div><div class="sirius_accordion_body">' . t('When a dispatch ends, would you like to be automatically placed on the "available" list?'));

		$form['asi'] = array(
			'#title' => '',
			'#type' => 'select',
			'#options' => array(
				'No' => t('No, when a dispatch ends I will sign in to the out of work list manually'),
				'Yes' => t('Yes, when a dispatch ends I should be automatically signed in to the out of work list'),
			),
			'#default_value' => $worker_node->field_sirius_dispatch_asi['und'][0]['value'],
		);

		$help_node = sirius_help_get('field|sirius_worker_dispatch_preferences|asi');
		if ($help_node) {
			$form['asi_help'] = array('#markup' => sirius_help_render($help_node));
		}

	  $form['accordion_close_asi'] = array('#markup' => '</div>');
	}


	if (user_access('sirius dispatch staff') || user_access('sirius dispatch own preferences skill') || user_access('sirius dispatch own preferences department')) {

	  $form['accordion_open_skills'] = array('#markup' => '<div class="sirius_accordion_label">' . t('Skill Preferences') . '</div><div class="sirius_accordion_body">' . t('If you would prefer to only receive dispatches for certain skills, select them below. Remember that if you select specific skills, you will not receive certain open-call dispatches.'));

		if (user_access('sirius dispatch staff') || user_access('sirius dispatch own preferences skill')) {

			//
			// Skils
			//

			if ($worker_node->field_sirius_skill['und']) {
				$skills = sirius_worker_skills($worker_node);
				$skill_options = array();
				foreach ($skills as $skill_tid => $skill_term) {
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
				if ($worker_node->field_sirius_skills_availx['und'][0]['value']) {
					$default_restricted = 'restrict';
				}

				$form['skills_restricted'] = array(
					'#title' => '',
					'#type' => 'select',
					'#options' => array(
						'all' => t('Send me dispatches for all skills'),
						'restrict' => t('Send me dispatches for specified skills'),
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

				$form['skills_availx'] = array(
					'#title' => t('In addition to the skills selected above, I want to receive dispatches with no skill specified.'),
					'#type' => 'select',
					'#options' => array(
						'No' => t('No'),
						'Yes' => t('Yes'),
					),
					'#default_value' => $worker_node->field_sirius_skills_availx['und'][0]['value'],
					'#states' => array(
						'visible' => array(
							array(':input[name="skills_restricted"]' => array('value' => 'restrict')),
						)
					),
				);
			}
		}

		if (user_access('sirius dispatch staff') || user_access('sirius dispatch own preferences department')) {

			//
			// Departments
			//

			$departments = sirius_worker_department_options($worker_node); // @todo: departments
			$department_options = array();
			foreach ($departments as $department_tid => $department_term) {
				$department_options[$department_tid] = $department_term->name;
			}
			$form['department_not_avail'] = array(
				'#title' => t('I would prefer <strong>not</strong> to receive dispatches for:'),
				'#type' => 'checkboxes',
				'#options' => $department_options,
				'#default_value' => sirius_fieldvals($worker_node, 'field_grievance_departments', 'tid'),
			);
		}

		$form['accordion_close_skills'] = array('#markup' => '</div>');
	}

	if (user_access('sirius dispatch staff') || user_access('sirius dispatch own preferences')) {

		//
		// DNCs
		//

		$employer_options = array(NULL => t('-- Select --'));
		$employer_options += sirius_employer_list();
		$max_delta = 2;
		if (is_array($worker_node->field_sirius_dispatch_dncs2['und'])) {
			$max_delta += count($worker_node->field_sirius_dispatch_dncs2['und']);
		}

	  $form['accordion_open_dncs2'] = array('#markup' => '<div class="sirius_accordion_label">' . t('Worker-Initiated Do Not Call') . '</div><div class="sirius_accordion_body">' . t('If you would prefer not to receive dispatches from certain employers, select them below.'));

		for ($delta=0; $delta<$max_delta; ++$delta) {
			$form['dncs2_nid_' . $delta] = array(
				'#type' => 'select',
				'#title' => 'Employer #' . ($delta+1),
				'#options' => $employer_options,
				'#default_value' => $worker_node->field_sirius_dispatch_dncs2['und'][$delta]['target_id'],
			);
		}
	  $form['accordion_close_dncs2'] = array('#markup' => '</div>');

		//
		// CBN Only
		//

	  $form['accordion_open_cbn'] = array('#markup' => '<div class="sirius_accordion_label">' . t('Call by Name Only') . '</div><div class="sirius_accordion_body">');
		$form['cbn'] = array(
			'#type' => 'select',
			'#title' => '',
			'#options' => array(
				'No' => 'No, I want both open calls and call-by-names',
				'Yes' => 'Yes, I want call by names only. I will not receive open calls.',
			),
			'#default_value' => $worker_node->field_sirius_dispatch_cbn['und'][0]['value'],
		);
	  $form['accordion_close_cbn'] = array('#markup' => '</div>');

		//
		// HFE
		//

	  $form['accordion_open_hfe'] = array('#markup' => '<div class="sirius_accordion_label">' . t('Hold for Employer') . '</div><div class="sirius_accordion_body">' . t('If you are expecting a dispatch from a specific employer, you can temporarily reject calls from all other employers.'));

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
				24*365 => '1 year',
			),
			'#default_value' => 72,
		);

	  $form['accordion_close_hfe'] = array('#markup' => '</div>');

		//
		// EBA
		//

	  // Can set EBA if you're currently employed, and you're either staff or unblocked.
	  if ($worker_node->field_grievance_shop['und'][0]['target_id'] && 
	  	(user_access('sirius dispatch staff') || $worker_node->field_sirius_dispatch_eba['und'][0]['value'] != 'Blocked') ) {

		  $dates = sirius_dispatch_eba_dates_upcoming();
		  $dates_default = sirius_dispatch_eba_dates_worker($worker_node);
		  $eba_options = array(
				'No' => t('No, I am not available for additional work.'),
				'Yes' => t('Yes, I will be available for additional work in the upcoming week.'),
			);
			if (user_access('sirius dispatch staff')) {
				$eba_options['Blocked'] = t('Blocked, this worker cannot set their EBA status.');
			}

		  $form['accordion_open_eba'] = array('#markup' => '<div class="sirius_accordion_label">' . t('Employed but Available') . '</div><div class="sirius_accordion_body">' . t('If you are currently employed but avaialble to work certain days in the next week, indicate your avaiability below.'));

			$form['eba'] = array(
				'#title' => '',
				'#type' => 'select',
				'#options' => $eba_options,
				'#default_value' => $worker_node->field_sirius_dispatch_eba['und'][0]['value'],
			);

			$form['eba_dates'] = array(
				'#title' => t('I am available on the following days.'),
				'#type' => 'checkboxes',
				'#options' => $dates,
				'#default_value' => $dates_default,
				'#states' => array(
					'visible' => array(
						array(':input[name="eba"]' => array('value' => 'Yes')),
					)
				),
			);

		  $form['accordion_close_eba'] = array('#markup' => '</div>');
		}
	}

  $form['accordion_close'] = array('#markup' => '</div>');
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Submit',
	);

	return $form;
}

function sirius_node_tab_worker_dispatch_preferences_form_validate($form, &$form_state) {
	if ($medium && $medium_alt && $medium == $medium_alt) {
		form_set_error('medium_alt', t('Primary and secondary notification methods must be different.'));
	}

	/*
	$aat = $form_state['values']['aat'];
	if (!preg_match('/^\d\d\d\d\d\d$/', $aat)) {
		form_set_error('aat', t('The access code must be a six-digit number.'));
	}
	*/

	// If HFE, must select an employer
	if ($form_state['values']['hfe_action'] == 'Set') {
		if (!$form_state['values']['hfe']) {
			form_set_error('hfe', t('Please select an employer.'));
		}
	}

	// If restricting by skill, must select at least one skill or non-skilled
	if ($form_state['values']['skills_restricted'] == 'restrict') {
		$found = FALSE;
		foreach (array_values($form_state['values']['skills_avail']) as $tid) {
			if ($tid) { $found = TRUE; break; }
		}
		if (!$found) {
			if ($form_state['values']['skills_availx'] == 'Yes') { $found = TRUE; }
		}
		if (!$found) {
			form_set_error('skills_restricted', 'If you wish to receive dispatches only for certain skills, you must select at least one skill (or opt in to unskilled work.)');
		}
	}
}


function sirius_node_tab_worker_dispatch_preferences_form_submit($form, &$form_state) {
	$worker_node = $form_state['worker_node'];
	global $user;

	$val = $form_state['values']['medium'];
	$oldval = $worker_node->field_sirius_dispatch_medium['und'][0]['value'];
	if ($val != $oldval) {
		$worker_node->field_sirius_dispatch_medium['und'][0]['value'] = $val;
		$need_save = TRUE;
		$msg = t('Changed primary medium from @oldval to @val', array('@oldval' => ($oldval?$oldval:"[NULL]"), '@val' => ($val?$val:"[NULL]")));
		sirius_log('worker:dispatch', 'update_preferences', $msg, '', '', array($worker_node->nid), array());
		drupal_set_message($msg);
	}

	$val = $form_state['values']['medium_1'];
	$oldval = $worker_node->field_sirius_dispatch_medium['und'][1]['value'];
	if ($val != $oldval) {
		$worker_node->field_sirius_dispatch_medium['und'][1]['value'] = $val;
		$need_save = TRUE;
		$msg = t('Changed secondary medium from @oldval to @val', array('@oldval' => ($oldval?$oldval:"[NULL]"), '@val' => ($val?$val:"[NULL]")));
		sirius_log('worker:dispatch', 'update_preferences', $msg, '', '', array($worker_node->nid), array());
		drupal_set_message($msg);
	}

	$val = $form_state['values']['medium_2'];
	$oldval = $worker_node->field_sirius_dispatch_medium['und'][2]['value'];
	if ($val != $oldval) {
		$worker_node->field_sirius_dispatch_medium['und'][2]['value'] = $val;
		$need_save = TRUE;
		$msg = t('Changed non-dispatch medium from @oldval to @val', array('@oldval' => ($oldval?$oldval:"[NULL]"), '@val' => ($val?$val:"[NULL]")));
		sirius_log('worker:dispatch', 'update_preferences', $msg, '', '', array($worker_node->nid), array());
		drupal_set_message($msg);
	}

	if (user_access('sirius dispatch staff') || user_access('sirius dispatch own preferences skill')) {
		// Clear skill preferences
		if ($form_state['values']['skills_restricted'] == 'all') {
			if ($worker_node->field_sirius_skills_avail['und']) {
				$worker_node->field_sirius_skills_avail['und'] = array();
				$worker_node->field_sirius_skills_availx['und'] = array();
				$need_save = TRUE;
				sirius_log('worker:skills_avail', 'clear', 'Skill preferences have been cleared', '', '', array($worker_node->nid), array());
				drupal_set_message('Skill preferences have been removed.');
			}
		}

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
				$new_skill_terms = taxonomy_term_load_multiple($new_skills);
				if ($new_skill_terms) {
					$msg = "Skill preferences have been set to ";
					$is_first = TRUE;
					foreach ($new_skill_terms as $tid => $term) {
						if ($is_first) { $is_first = FALSE; } else { $msg .= ', '; }
						$msg .= $term->name;
					}
				} else {
					$msg = "Skill preferences have been set to [none].";
				}
				sirius_log('worker:skills_avail', 'set', $msg, '', '', array($worker_node->nid), array());
				drupal_set_message($msg);
			}

			if ($form_state['values']['skills_availx'] && 
				($form_state['values']['skills_availx'] != $worker_node->field_sirius_skills_availx['und'][0]['value'])) {
				$worker_node->field_sirius_skills_availx['und'][0]['value'] = $form_state['values']['skills_availx'];
				$need_save = TRUE;
				sirius_log('worker:skills_availx', 'set', 'Available for unskilled work has been set to ' . $form_state['values']['skills_availx'], '', '', array($worker_node->nid), array());
				drupal_set_message('Availability for unskilled work has been set to ' . $form_state['values']['skills_availx']);
			}
		}
	}

	if (user_access('sirius dispatch staff') || user_access('sirius dispatch own preferences department')) {
		// Department Preferences
		$new_departments = array();
		if ($form_state['values']['department_not_avail']) {
			foreach ($form_state['values']['department_not_avail'] as $key => $value) {
				if ($value) {
					$new_departments[] = $value;
				}
			}
		}
		$old_departments = sirius_fieldvals($worker_node, 'field_grievance_departments', 'tid');
		if ($new_departments != $old_departments) {
			$need_save = TRUE;
			$worker_node->field_grievance_departments['und'] = array();
			foreach ($new_departments as $new_department) {
				$worker_node->field_grievance_departments['und'][] = array('tid' => $new_department);
			}
			drupal_set_message("Department preferences have been updated.");
		}
	}

	if (user_access('sirius dispatch staff') || user_access('sirius dispatch own preferences')) {
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

		$old_asi = $worker_node->field_sirius_dispatch_asi['und']['0']['value'];
		$new_asi = $form_state['values']['asi'];
		if ($old_asi != $new_asi) {
			$worker_node->field_sirius_dispatch_asi['und']['0']['value'] = $new_asi;
			$need_save = TRUE;
			sirius_log('worker:asi', 'set', 'Auto-sign-in preferences have been set to ' . $new_asi, '', '', array($worker_node->nid), array());
			drupal_set_message('Auto-sign-in preferences have been set to ' . $new_asi . '.');
		}


		$old_eba = $worker_node->field_sirius_dispatch_eba['und']['0']['value'];
		$new_eba = $form_state['values']['eba'];
		if ($old_eba != $new_eba) {
			$worker_node->field_sirius_dispatch_eba['und']['0']['value'] = $new_eba;
			$need_save = TRUE;
			sirius_log('worker:eba', 'set', 'EBA status set to ' . $new_eba, '', '', array($worker_node->nid), array());

			if ($new_eba == 'Yes') {
				$newdate = gmdate('Y-m-d H:i:s');
				$worker_node->field_sirius_dispatch_availdate['und'][0]['value'] = $newdate;
				sirius_log('worker:availdate', 'set', "Set worker's availability date to $newdate", 'Signed in to EBA', '', array($worker_node->nid));
			}

			drupal_set_message('Employed-but-available status set to ' . $new_eba . '.');
		}

		if ($new_eba == 'No' && $worker_node->field_sirius_dispatch_eba_dates['und'][0]) {
			$worker_node->field_sirius_dispatch_eba_dates['und'] = array();
			$need_save = TRUE;
			sirius_log('worker:eba_dates', 'clear', 'EBA dates cleared', '', '', array($worker_node->nid), array());
			drupal_set_message('Employed-but-available dates have been cleared.');
		}

		if ($new_eba == 'Yes') {
			$old_eba_dates = sirius_dispatch_eba_dates_worker($worker_node);
			$new_eba_dates = array();
			if ($form_state['values']['eba_dates']) {
				foreach ($form_state['values']['eba_dates'] as $key => $value) {
					if ($value) {
						$new_eba_dates[] = $value;
					}
				}
			}
			sort(array_unique($new_eba_dates));

			if (array_diff($old_eba_dates, $new_eba_dates) || array_diff($new_eba_dates, $old_eba_dates)) {
				$worker_node->field_sirius_dispatch_eba_dates['und'] = array();
				$msg = '';
				foreach ($new_eba_dates as $date) {
					$worker_node->field_sirius_dispatch_eba_dates['und'][] = array('value' => $date);
					if ($msg) { $msg .= ', '; }
					$msg .= $date;
				}
				$need_save = TRUE;
				drupal_set_message('EBA dates have been set to ' . $msg);
				sirius_log('worker:eba_dates', 'set', 'EBA dates set to ' . $msg, '', '', array($worker_node->nid), array());
			}
		}
	}

	if ($need_save) {
		node_save($worker_node);
	}
}
