<?php

function sirius_worker_skill_term_renew() {
	return drupal_get_form('sirius_worker_skill_term_renew_form');
}

function sirius_worker_skill_term_renew_form($form, &$form_state) {
	$form['intro'] = array('#markup' => '<div class="sirius_form_intro">' . t('Use this form to globally renew a skill for all workers.') . '</div>');

	$form['tid'] = array(
		'#title' => t('Skill'),
		'#type' => 'select',
		'#options' => sirius_taxonomy_load('sirius_skill', 'tid', 'name', TRUE),
		'#required' => TRUE,
	);

	$form['renew_until'] = array(
		'#type' => 'textfield',
		'#title' => t('Renew until'),
		'#description' => t('Enter an absolute date or a number of days.'),
		'#required' => TRUE,
	);

	$form['reason'] = array(
		'#type' => 'textara',
		'#title' => t('Reason'),
		'#required' => TRUE,
	);

	$form['confirm'] = array(
		'#type' => 'checkbox',
		'#title' => 'Yes, renew this skill for ALL workers.',
		'#value' => 1,
		'#required' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Submit',
	);

	return $form;
}

/*
function sirius_worker_skill_term_renew_form_submit($form, &$form_state) {
	// Init
	$tid = $form_state['values']['tid'];

	$operations = array();
	$sql = "select entity_id from field_data_field_sirius_skill ";
	$sql .= "where entity_type = 'node' ";
	$sql .= "and bundle = 'sirius_worker' ";
	$sql .= "and field_sirius_skill_tid = :tid ";
	$sql .= "order by entity_id ";
	$stmt = db_query($sql, array(':tid' => $tid));
	$worker_nids = array();
	while ($hr = $stmt->fetchAssoc()) {
		$worker_nids[] = $hr['entity_id'];
	}

	$count = 0;
	foreach ($worker_nids as $worker_nid) {
		$operations[] = array('sirius_worker_skill_set', array($worker_nid, array($tid), NULL, 'remove', 'Skill was deleted', array()));
		++$count;
		// if ($count > 5) { break; }
	}
  $operations[] = array('taxonomy_term_delete', array($tid));

	// Launch the batch processor.
  $batch = array(
    'operations' => $operations,
  );
  batch_set($batch);

	return;
}
*/
