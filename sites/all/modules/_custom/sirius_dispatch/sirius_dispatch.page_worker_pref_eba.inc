<?php

function sirius_dispatch_node_tab_worker_pref_eba_access($worker_node) {
	if (!user_access('sirius dispatch staff')) {
		// Can't see this tab if you're sign-in blocked
	  $sib_term = taxonomy_term_load($worker_node->field_sirius_dispatch_sib_status['und'][0]['tid']);
		if ($sib_term->field_sirius_dispatch_available['und'][0]['value'] == 'No') { return FALSE; }

		// Can't see this tab if you're EBA-blocked
		if ($worker_node->field_sirius_dispatch_eba['und'][0]['value'] == 'Blocked') { return FALSE; }
	}

	if (!user_access('sirius admin')) {
		// Can't see this tab if you're not employed
		if (! ($worker_node->field_grievance_shop['und'][0]['target_id'])) { return FALSE; }
	}

	return TRUE; 
}

function sirius_dispatch_node_tab_worker_pref_eba_form($form, &$form_state, $worker_node) {
	$form_state['worker_nid'] = $worker_node->nid;

  $dates = sirius_dispatch_eba_dates_upcoming();
  $dates_default = sirius_dispatch_eba_dates_worker($worker_node);
  $eba_options = array(
		'No' => t('No, I am not available for additional work.'),
		'Yes' => t('Yes, I will be available for additional work in the upcoming week.'),
	);
	if (user_access('sirius dispatch staff')) {
		$eba_options['Blocked'] = t('Blocked, this worker cannot set their EBA status.');
	}

	$form['eba'] = array(
		'#title' => 'Employed but Available Status',
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

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_dispatch_node_tab_worker_pref_eba_form_submit($form, &$form_state) {
	$worker_node = node_load($form_state['worker_nid']);

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

	if ($need_save) { node_save($worker_node); }
}