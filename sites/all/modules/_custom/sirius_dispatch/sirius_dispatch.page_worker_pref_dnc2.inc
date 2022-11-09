<?php

function sirius_dispatch_node_tab_worker_pref_dnc2_access($worker_node) {
	// Can't see this tab if you're sign-in blocked
	if (!user_access('sirius dispatch staff')) {
	  $sib_term = taxonomy_term_load($worker_node->field_sirius_dispatch_sib_status['und'][0]['tid']);
		if ($sib_term->field_sirius_dispatch_available['und'][0]['value'] == 'No') { return FALSE; }
	}

	return TRUE; 
}

function sirius_dispatch_node_tab_worker_pref_dnc2_form($form, &$form_state, $worker_node) {
	$form_state['worker_nid'] = $worker_node->nid;

	$employer_options = array(NULL => t('-- Select --'));
	$employer_options += sirius_employer_list();
	$max_delta = 2;
	if (is_array($worker_node->field_sirius_dispatch_dncs2['und'])) {
		$max_delta += count($worker_node->field_sirius_dispatch_dncs2['und']);
	}

	for ($delta=0; $delta<$max_delta; ++$delta) {
		$form['dncs2_nid_' . $delta] = array(
			'#type' => 'select',
			'#title' => t('Do not send me dispatches from (#@i)', array('@i' => ($delta+1))),
			'#options' => array(NULL => t('-- Select --')) + $employer_options,
			'#default_value' => $worker_node->field_sirius_dispatch_dncs2['und'][$delta]['target_id'],
		);
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_dispatch_node_tab_worker_pref_dnc2_form_submit($form, &$form_state) {
	$worker_node = node_load($form_state['worker_nid']);

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
}