<?php

function sirius_steward_node_tab_employer_sched_access($employer_node) {
	return sirius_steward_sched_access('view', $employer_node->nid);
}

function sirius_steward_node_tab_employer_sched_form($form, &$form_state, $employer_node) {
	$form_state['employer_nid'] = $employer_node->nid; 

	// Render the schedule for the next few days
	$html = '<div class="sirius_form_intro">';
	$found = FALSE;
	for ($i = 0; $i<7; ++$i) {
		if ($i > 0) { $ts = strtotime("+$i days"); } else { $ts = time(); }
		$sched_node = sirius_steward_sched_fetch($employer_node->nid, date('Y-m-d', $ts), FALSE);
		if ($sched_node) {
			$options = array();
			$options['hide_checkins'] = TRUE;
			if ($found) { $options['hide_header'] = TRUE; } else { $found = TRUE; }
			$html .= sirius_steward_sched_render($sched_node, $options);
		}
	}
	if (!$found) {
		$html .= t('No steward schedules have been entered for the upcoming seven days.');
	}
	$html .= '</div>';

	$form['intro'] = array('#markup' => $html);

	if (user_access('sirius steward sched worker')) {
		$html = '<h2>' . t('Create or Edit a Steward Schedule') . '</h2>';

		$form['date'] = array(
			'#title' => t('Date'),
			'#type' => 'date_popup',
	    '#attributes' => array(
	      'autocomplete' => 'off',
	    ),
	    '#required' => TRUE,
	    '#date_format' => 'Y-m-d',
	    '#default_value' => date('Y-m-d'),
	    '#prefix' => $html,
		);

		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Edit'),
		);
	}

	return $form;
}

function sirius_steward_node_tab_employer_sched_form_validate($form, &$form_state) {
	// Only admins can edit past schedules
	if (!user_access('sirius steward admin')) {
		$date = $form_state['values']['date'];
		if (!$date) { return FALSE; }
		if (strtotime($date) < strtotime(date('Y-m-d'))) { 
			form_set_error('date', 'Please enter a date in the future.');
		}
	}
}


function sirius_steward_node_tab_employer_sched_form_submit($form, &$form_state) {
	$employer_nid = $form_state['employer_nid'];
	$sched_node = sirius_steward_sched_fetch($employer_nid, $form_state['values']['date'], TRUE);
	drupal_goto('node/' . $sched_node->nid . '/sirius_steward_log_sched/edit', array('query' => array('destination' => current_path())));
}
