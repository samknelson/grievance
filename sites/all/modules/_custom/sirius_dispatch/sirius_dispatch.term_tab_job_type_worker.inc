<?php

function sirius_dispatch_term_tab_job_type_worker_form($form, &$form_state, $job_type_term) {
	$form_state['job_type_tid'] = $job_type_term->tid;
	$form_state['key'] = 'workers';

	$form['#tree'] = TRUE;
	$key = 'workers';
	$form['workers']['must_be_eligible'] = array(
		'#type' => 'select', 
		'#title' => t('Must be Eligible'),
		'#options' => array('No' => 'No', 'Yes' => 'Yes'),
		'#description' => t('If No, then dispatches may be created for ineligible workers (i.e. workers who are currently employed) and those workers will be notified when they become available. If Yes, then only eligible workers may be dispatched to this job type.'),
	);
	$form['workers']['must_be_rollover'] = array(
		'#type' => 'select', 
		'#title' => t('Must be Rollover'),
		'#options' => array('No' => 'No', 'Yes' => 'Yes'),
		'#description' => t('If Yes, then only rollover workers will be available to this job type.')
	);
	$form['workers']['may_be_unavailable'] = array(
		'#type' => 'select', 
		'#title' => t('May be Unavailable'),
		'#options' => array('No' => 'No', 'Yes' => 'Yes'),
		'#description' => t('If Yes, then workers who are unemployed and unavailable may still be dispatched.')
	);
	$form['workers']['may_be_employed'] = array(
		'#type' => 'select', 
		'#title' => t('May be Employed'),
		'#options' => array('No' => 'No', 'Yes' => 'Yes'),
		'#description' => t('If Yes, then workers who are currently employed elsehwere may still be dispatched (i.e. for a temporary rollover).')
	);
	$form['workers']['may_be_eba'] = array(
		'#type' => 'select', 
		'#title' => t('May be EBA'),
		'#options' => array('No' => 'No', 'Yes' => 'Yes'),
		'#description' => t('If Yes, then employers may allow employed-but-available workers to be dispatched.')
	);
	$form['workers']['cbn'] = array(
		'#type' => 'select', 
		'#title' => t('CBN?'),
		'#options' => array('No' => 'No', 'Yes' => 'Yes'),
		'#description' => t('If Yes, then call-by-name is supported for this job type.')
	);
	$form['workers']['always_secondary'] = array(
		'#type' => 'select', 
		'#title' => t('Always Secondary'),
		'#options' => array('No' => 'No', 'Yes' => 'Yes'),
		'#description' => t('If "Yes", then dispatches to this type of job are ALWAYS secondary. They do not set the employer and they do not affect the worker\'s out-of-work status.'),
	);
	$form['workers']['count_policy'] = array(
		'#type' => 'select', 
		'#title' => t('Worker Count'),
		'#options' => array(
			'none' => t('None, no worker count.'),
			'optional' => t('Optional, the job creator may specify a worker count'),
			'required' => t('Required, the job creator must specify a worker count'),
			'automatic' => t('Automatic, calculated from the number of CBN dispatches'),
			'fixed' => t('Fixed, all jobs of this type have exactly the same number of workers')
		),
	);
	$form['workers']['count_fixed'] = array(
		'#type' => 'textfield', 
		'#title' => t('Fixed Worker Count'),
		'#size' => 5,
		'#states' => array(
			'visible' => array(
				array(':input[name="workers[count_policy]"]' => array('value' => 'fixed')),
			)
		),
		'#description' => t('(This setting is ignored unless the worker count is set to "Fixed")'),
	);

	$form['workers']['ignore_prefs_skill'] = array(
		'#type' => 'select', 
		'#title' => t('Ignore Worker Preferences - Skill'),
		'#options' => array('No' => 'No', 'Yes' => 'Yes'),
	);
	$form['workers']['ignore_prefs_dept'] = array(
		'#type' => 'select', 
		'#title' => t('Ignore Worker Preferences - Department'),
		'#options' => array('No' => 'No', 'Yes' => 'Yes'),
	);
	$form['workers']['ignore_prefs_dnc'] = array(
		'#type' => 'select', 
		'#title' => t('Ignore Worker Preferences - Worker-Initiated DNC'),
		'#options' => array('No' => 'No', 'Yes' => 'Yes'),
	);
	$form['workers']['ignore_prefs_cbn'] = array(
		'#type' => 'select', 
		'#title' => t('Ignore Worker Preferences - Call-by-Name Only'),
		'#options' => array('No' => 'No', 'Yes' => 'Yes'),
	);
	$form['workers']['ignore_prefs_hfe'] = array(
		'#type' => 'select', 
		'#title' => t('Ignore Worker Preferences - Hold for Employer'),
		'#options' => array('No' => 'No', 'Yes' => 'Yes'),
	);

	$form['workers']['multiple_dispatches'] = array(
		'#type' => 'select', 
		'#title' => t('Allow multiple dispatches?'),
		'#options' => array('No' => 'No', 'Yes' => 'Yes'),
		'#description' => t('If set to "Yes", then a worker can be dispatched multiple times to the same job. <strong>Important: This setting is primarily for "stub" jobs, and should not be used for standard dispatch job types.</strong>'),
	);

	$form['workers']['layoff_secondary_without_primary'] = array(
		'#type' => 'select', 
		'#title' => t('Layoff Secondary Without Primary?'),
		'#options' => array('No' => 'No', 'Yes' => 'Yes'),
		'#description' => t('If set to "Yes", then a worker who is on a secondary dispatch for this job type will be laid off if they do not have a primary dispatch.'),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

 	sirius_jsonfield_util_set_defaults($form, sirius_json_get($job_type_term));

	return $form;
}

function sirius_dispatch_term_tab_job_type_worker_form_submit($form, &$form_state) {
	$job_type_tid = $form_state['job_type_tid'];
	$job_type_term = taxonomy_term_load($job_type_tid);
	$json = sirius_json_get($job_type_term);
	$json['workers'] = $form_state['values']['workers'];
	sirius_json_set($job_type_term, $json);
	taxonomy_term_save($job_type_term);
	drupal_set_message(t('Settings saved.'));
}

