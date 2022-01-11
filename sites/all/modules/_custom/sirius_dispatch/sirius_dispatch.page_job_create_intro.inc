<?php

/***********************************************************
 * Intro - select an employer and job type
 ***********************************************************/

function sirius_dispatch_page_job_create_intro() {
	return drupal_get_form('sirius_dispatch_page_job_create_intro_form');
}

function sirius_dispatch_page_job_create_intro_form_ajax($form, $form_state) {
  $commands = array();
  $commands[] = ajax_command_replace("#sirius_dispatch_page_job_create_intro_form_ajax_industry", render($form['industry_tid']));

  return array(
    '#type' => 'ajax',
    '#commands' => $commands
  );
}

function sirius_dispatch_page_job_create_intro_form($form, &$form_state) {
	if (user_access("sirius dispatch staff")) {
		$employer_options = array(NULL => t('-- Select --')) + sirius_employer_list();
		$form['employer_nid'] = array(
			'#title' => t('Employer'),
			'#type' => 'select',
			'#options' => $employer_options,
			'#required' => TRUE,
			'#ajax' => array('callback' => 'sirius_dispatch_page_job_create_intro_form_ajax',
  	  ),
    );
	} else {
		$employer_node = sirius_employer_get();
		if (!$employer_node) { 
			drupal_set_message('No employer found for this user', 'error');
			return array();
		}
		$form['employer_nid'] = array(
			'#type' => 'value',
			'#value' => $employer_node->nid,
		);
		$form['employer_markup'] = array('#markup' => '<div class="sirius_form_intro">Creating a job for <strong>' . $employer_node->title . '</strong></div>');
	}

	// Job Type
	$job_types_all = sirius_taxonomy_load('sirius_dispatch_job_type', 'tid', 'field_sirius_name_display');
	if (user_access('sirius dispatch staff')) {
		$job_types = $job_types_all;		
	} else {
		$job_types = array();
		if ($employer_node->field_sirius_dispatch_job_types['und']) {
			$job_type_tids = sirius_fieldvals($employer_node, 'field_sirius_dispatch_job_types', 'tid');
			foreach ($job_type_tids as $job_type_tid) {
				$job_types[$job_type_tid] = $job_types_all[$job_type_tid];
			}
		}
	}

	if (!$job_types) {
		$msg = "This employer is misconfigured. Please make sure that at least one job type is enabled. ";
		drupal_set_message($msg, 'error');
		return array();
	}

	$form['job_type_tid'] = array(
		'#type' => 'select',
		'#options' => $job_types,
		'#title' => t('Request Type'),
		'#required' => TRUE,
	);

	if (!$employer_node && $form_state['values']['employer_nid']) {
		$employer_node = node_load($form_state['values']['employer_nid']);
	}

	if ($employer_node) {
		$industry_names = sirius_taxonomy_load('sirius_industry', 'tid', 'name');
		$industry_tids = sirius_fieldvals($employer_node, 'field_sirius_industry', 'tid');
		$industry_options = array(NULL => t('-- Select --'));
		foreach ($industry_tids as $industry_tid) { $industry_options[$industry_tid] = $industry_names[$industry_tid]; }
	} else {
		$industry_options = array();
	}

	if (count($industry_options) > 2) {
		$form['industry_tid'] = array(
			'#type' => 'select',
			'#options' => $industry_options,
			'#title' => t('Industry'),
			'#required' => TRUE,
			'#prefix' => '<div id="sirius_dispatch_page_job_create_intro_form_ajax_industry">',
			'#suffix' => '</div>',
		);
	} else {
		$form['industry_tid'] = array('#markup' => '<div id="sirius_dispatch_page_job_create_intro_form_ajax_industry"></div>');
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Go',
	);

	return $form;
}

function sirius_dispatch_page_job_create_intro_form_submit($form, &$form_state) {
	$employer_nid = $form_state['values']['employer_nid'];
	$job_type_tid = $form_state['values']['job_type_tid'];
	$industry_tid = $form_state['values']['industry_tid'];
	$path = current_path() . '/' . $employer_nid . '/' . $job_type_tid;
	if ($industry_tid) { $path .= '/' . $industry_tid; }
	drupal_goto($path);
}
