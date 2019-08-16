<?php

function sirius_dispatch_job_policy($job_node) {
	return sirius_dispatch_job_policy_from_tid($job_node->field_sirius_dispatch_job_type['und'][0]['tid']);
}

function sirius_dispatch_job_policy_from_tid($job_type_tid) {
	$job_type_term = taxonomy_term_load($job_type_tid);
	$job_policy = json_decode($job_type_term->field_sirius_json['und'][0]['value'], TRUE);
	return $job_policy;
}

function sirius_dispatch_policy_sirius_jsonfield_form_definitions() {
	// JSON field for job types
	$form = array();
	$form['workers'] = array(
		'#type' => 'fieldset',
		'#title' => 'Worker Settings',
	);
	$form['workers']['must_be_eligible'] = array(
		'#type' => 'select', 
		'#title' => 'Must be Eligible',
		'#options' => array('No' => 'No', 'Yes' => 'Yes'),
		'#description' => t('If No, then dispatches may be created for ineligible workers (i.e. workers who are currently employed) and those workers will be notified when they become available. If Yes, then only eligible workers may be dispatched to this job type.')
	);
	$form['workers']['must_be_rollover'] = array(
		'#type' => 'select', 
		'#title' => 'Must be Rollover',
		'#options' => array('No' => 'No', 'Yes' => 'Yes'),
		'#description' => t('If Yes, then only rollover workers will be available to this job type.')
	);
	$form['workers']['may_be_unavailable'] = array(
		'#type' => 'select', 
		'#title' => 'May be Unavailable',
		'#options' => array('No' => 'No', 'Yes' => 'Yes'),
		'#description' => t('If Yes, then workers who are unemployed and unavailable may still be dispatched.')
	);
	$form['workers']['may_be_employed'] = array(
		'#type' => 'select', 
		'#title' => 'May be Employed',
		'#options' => array('No' => 'No', 'Yes' => 'Yes'),
		'#description' => t('If Yes, then workers who are currently employed elsehwere may still be dispatched (i.e. for a temporary rollover).')
	);
	$form['workers']['may_be_eba'] = array(
		'#type' => 'select', 
		'#title' => 'May be EBA',
		'#options' => array('No' => 'No', 'Yes' => 'Yes'),
		'#description' => t('If Yes, then employers may allow employed-but-available workers to be dispatched.')
	);
	$form['workers']['cbn'] = array(
		'#type' => 'select', 
		'#title' => 'CBN?',
		'#options' => array('No' => 'No', 'Yes' => 'Yes'),
		'#description' => t('If Yes, then call-by-name is supported for this job type.')
	);
	$form['workers']['count_policy'] = array(
		'#type' => 'select', 
		'#title' => 'Worker Count',
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
		'#title' => 'Fixed Worker Count',
		'#size' => 5,
		'#states' => array(
			'visible' => array(
				array(':input[name="field_sirius_json[und][0][sirius_jsonfield][workers][count_policy]"]' => array('value' => 'fixed')),
			)
		),
		'#description' => t('(This setting is ignored unless the worker count is set to "Fixed")'),
	);

	$form['run'] = array(
		'#type' => 'fieldset',
		'#title' => 'Run Settings',
	);

	$roles = user_roles();
	$form['run']['role'] = array(
		'#type' => 'checkboxes', 
		'#title' => 'Roles',
		'#options' => $roles,
		'#description' => t('Check the roles with permission to run this job type.')
	);

	$form['run']['fan'] = array(
		'#type' => 'select', 
		'#title' => 'FAN?',
		'#options' => array('No' => 'No', 'Yes' => 'Yes'),
		'#description' => t('Fill as Needed - If Yes, then once all call-by-name dispatches have been sent, additional dispatches will be created from all available workers until the job is filled.')
	);

	$form['run']['timelimit'] = array(
		'#title' => 'Default Expire After',
		'#type' => 'textfield',
		'#size' => 5,
		'#default_value' => 30,
		'#description' => t('Enter the number of minutes before a dispatch notification should expire.'),
	);

	$form['run']['batch_max'] = array(
		'#title' => 'Default Maximum Batch Size',
		'#type' => 'textfield',
		'#size' => 5,
		'#default_value' => 10,
		'#description' => t('Enter the maxminum number ofnotifications that will be sent in any given batch'),
	);

	$form['run']['batch_min'] = array(
		'#title' => 'Default Minimum Batch Size',
		'#type' => 'textfield',
		'#size' => 5,
		'#default_value' => 1,
		'#description' => t('Enter the minimum numer of notifications that will be sent in any given batch. If this is greater than 1, then it is possible that more notifications will be sent than the number of open slots, meaning that there is no guarantee that the job will be filled in seniority order.'),
	);

	$form['run']['batch_ratio'] = array(
		'#title' => 'Default Ratio of Notifications to Slots',
		'#type' => 'textfield',
		'#size' => 5,
		'#default_value' => 1,
		'#description' => t('Enter the number of notifications to be sent for each open slot. If this is greater than 1, then there will be more than one notification sent for each slot, meaning that there is no guarantee that the job will be filled in seniority order.'),
	);

	$form['form'] = array(
		'#type' => 'fieldset',
		'#title' => 'Form Settings',
	);

	$form['form']['hide_skill'] = array(
		'#type' => 'select', 
		'#title' => 'Hide Skill?',
		'#options' => array('No' => 'No', 'Yes' => 'Yes'),
		'#description' => t('If "Yes", then the "Skill" field will be hidden on the "create a job" form.')
	);

	$form['form']['hide_department'] = array(
		'#type' => 'select', 
		'#title' => 'Hide Department?',
		'#options' => array('No' => 'No', 'Yes' => 'Yes'),
		'#description' => t('If "Yes", then the "Department" field will be hidden on the "create a job" form.')
	);

	$form['form']['hide_address_notes'] = array(
		'#type' => 'select', 
		'#title' => 'Hide Report Location?',
		'#options' => array('No' => 'No', 'Yes' => 'Yes'),
		'#description' => t('If "Yes", then the "Report Location" field will be hidden on the "create a job" form.')
	);

	$form['form']['hide_address_parking'] = array(
		'#type' => 'select', 
		'#title' => 'Hide Parking Instructions?',
		'#options' => array('No' => 'No', 'Yes' => 'Yes'),
		'#description' => t('If "Yes", then the "Parking Instructions" field will be hidden on the "create a job" form.')
	);

	$form['event'] = array(
		'#type' => 'fieldset',
		'#title' => 'Event Settings',
	);

	$form['event']['has_event'] = array(
		'#type' => 'select', 
		'#title' => 'Has Event?',
		'#options' => array('No' => 'No', 'Yes' => 'Yes'),
		'#description' => t('If "Yes", then each job of this type should be associated with an event. The worker list will be drawn from event registrants.'),
	);

	$event_type_options = sirius_taxonomy_load('sirius_event_type', 'tid', 'name', TRUE);
	$form['event']['event_type'] = array(
		'#type' => 'select',
		'#title' => 'Event Type',
		'#options' => $event_type_options,
	);

	$form['event']['on_job_create'] = array(
		'#type' => 'select',
		'#title' => 'On Job Creation',
		'#options' => array(
			NULL => t('-- Select --'),
			'create' => t('Create a new event'),
			'select' => t('Select an existing event'),
		),
	);

	$definition['taxonomy_term']['sirius_dispatch_job_type']['field_sirius_json'] = $form;
	return $definition;
}

/*
 * Incomplete - got started working on this, but on reflection I think it's better handled either natively within the jsonfield, or
 * as a views bulk operation. Not sure it needs a standalone form.

function sirius_dispatch_job_policy_exim_access($tid) {
	$term = taxonomy_term_load($tid);
	if (!$term) { return FALSE; }
	$vid = $term->vid;
	$vocabulary = taxonomy_vocabulary_load($vid);
	if ($vocabulary->machine_name != 'sirius_dispatch_job_type') { return FALSE; }
	return user_access('sirius admin');
}

function sirius_dispatch_job_policy_exim($tid) {
	return drupal_get_form('sirius_dispatch_job_policy_exim_form', $tid);
}

function sirius_dispatch_job_policy_exim_form($form, &$form_state, $tid) {
	$form_state['tid'] = $tid;
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Save',
	);
	return $form;
}
*/