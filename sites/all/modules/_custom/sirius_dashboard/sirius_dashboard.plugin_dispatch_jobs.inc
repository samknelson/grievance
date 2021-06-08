<?php

function sirius_dispatch_plugin_dispatch_jobs_prefs($json) {
	$job_types = sirius_taxonomy_load('sirius_dispatch_job_type', 'tid', 'name');
	$form['job_types'] = array(
		'#type' => 'checkboxes',
		'#title' => 'Job Types',
		'#options' => $job_types
	);
	if ($json['job_types']) { $form['job_types']['#default_value'] = $json['job_types']; }

	$status_field = field_info_field('field_sirius_dispatch_job_status');
	$status_options = list_allowed_values($status_field);
	$form['statuses'] = array(
		'#type' => 'checkboxes',
		'#title' => 'Statuses',
		'#options' => $status_options
	);
	if ($json['statuses']) { $form['statuses']['#default_value'] = $json['statuses']; }

	$form['link_path'] = array(
		'#type' => 'textfield',
		'#title' => 'Link Path',
		'#default_value' => $json['link_path'],
		'#description' => t('Enter the system path that this dashboard item should link to. The pathshould not begin with a slash (/). For example, "sirius/config".'),
	);

	$form['link_text'] = array(
		'#type' => 'textfield',
		'#title' => 'Link Text',
		'#default_value' => $json['link_text'],
		'#description' => t('Enter the text that should appear in the link button.'),
	);

	return $form;
}

function sirius_dispatch_plugin_dispatch_jobs_data($prefs, $options = array()) {
	if (!$prefs['job_types']) { return array(); }
	if (!$prefs['statuses']) { return array(); }

	$domain_nid = sirius_domain_nid();
	$query = new EntityFieldQuery;
	$query->entityCondition('entity_type', 'node');
	$query->entityCondition('bundle', 'sirius_dispatch_job');
	if ($domain_nid) { $query->fieldCondition('field_sirius_domain', 'target_id', $domain_nid); }
	$query->fieldCondition('field_sirius_dispatch_job_type', 'tid', array_filter(array_values($prefs['job_types'])), 'in');
	$query->fieldCondition('field_sirius_dispatch_job_status', 'value', array_filter(array_values($prefs['statuses'])), 'in');
	$stmt = $query->execute();
	if (!$stmt['node']) { return array(); }

	$tools = '';
	if ($prefs['link_path']) {
		$tools = l($prefs['link_text'], $prefs['link_path'], array('attributes' => array('class' => array('tool-button'))));
	}
	$row = array(
		'details' => '',
		'count' => count($stmt['node']),
		'tools' => $tools,
	);

	return array($row);
}

