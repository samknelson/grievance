<?php

function sirius_worker_page_create_intro() {
	$access = Sirius_Worker_Access::getInstance();

	if ($access->access('create worker') && $access->access('staff')) {
		return drupal_get_form('sirius_worker_page_create_intro_form');
	}

	$employer_node = sirius_employer_get();
	if (!$employer_node) { 
		return sirius_page_error("You do not have staff access and you are not associated with an employer.");
	}

	drupal_goto(current_path() . '/' . $employer_node->nid);
}

function sirius_worker_page_create_intro_form($form, &$form_state) {
	if (user_access("sirius dispatch staff")) {
		$employer_options = array(NULL => t('-- Select --')) + sirius_employer_list();
		$form['employer_nid'] = array(
			'#title' => t('Employer'),
			'#type' => 'select',
			'#options' => $employer_options,
			'#required' => TRUE,
    );
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Go',
	);

	return $form;
}

function sirius_worker_page_create_intro_form_submit($form, &$form_state) {
	$employer_nid = $form_state['values']['employer_nid'];
	$path = current_path() . '/' . $employer_nid;
	drupal_goto($path);
}
