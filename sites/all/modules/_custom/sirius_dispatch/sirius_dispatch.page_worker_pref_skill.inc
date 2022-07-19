<?php

function sirius_dispatch_node_tab_worker_pref_skill_access($worker_node) {
	// Can't see this tab if you're sign-in blocked
	if (!user_access('sirius dispatch staff')) {
	  $sib_term = taxonomy_term_load($worker_node->field_sirius_dispatch_sib_status['und'][0]['tid']);
		if ($sib_term->field_sirius_dispatch_available['und'][0]['value'] == 'No') { return FALSE; }
	}

	return TRUE; 
}

function sirius_dispatch_node_tab_worker_pref_skill_form($form, &$form_state, $worker_node) {
	$form_state['worker_nid'] = $worker_node->nid;

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
		'#title' => t('Skill Preferences'),
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

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_dispatch_node_tab_worker_pref_skill_form_validate($form, &$form_state) {

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


function sirius_dispatch_node_tab_worker_pref_skill_form_submit($form, &$form_state) {
	$worker_node = node_load($form_state['worker_nid']);

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

	if ($need_save) { node_save($worker_node); }
}
