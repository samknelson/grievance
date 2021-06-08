<?php

function sirius_dashboard_plugin_training_level_counts_prefs($json) {
	$ms_options = sirius_taxonomy_load('sirius_member_status', 'tid', 'name');
	$form['ms_tid'] = array(
		'#type' => 'select',
		'#title' => 'Member Status',
		'#options' => array(NULL => t('-- Select --')) + $ms_options,
		'#default_value' => $json['ms_tid'],
	);

	return $form;
}

function sirius_dashboard_plugin_training_level_counts_data($prefs, $options = array()) {
	$ms_tid = $prefs['ms_tid'];
	if (!$ms_tid) { return array(); }
	$levels = sirius_training_levels_fetch($ms_tid);

	$ms_term = sirius_taxonomy_load($ms_tid);

	$ws_terms = sirius_taxonomy_load('sirius_work_status', 'tid', 'full');
	$ws_member_active_tids = array();
	$ws_dispatch_available_tids = array();
	foreach ($ws_terms as $ws_tid => $ws_term) {
		if ($ws_term->field_sirius_member_active['und'][0]['value'] == 'Yes') { $ws_member_active_tids[] = $ws_tid; }
		if ($ws_term->field_sirius_dispatch_available['und'][0]['value'] == 'Yes') { $ws_dispatch_available_tids[] = $ws_tid; }
	}

	$rows = array();
	foreach ($levels as $level_code => $level_record) {
		$domain_nid = sirius_domain_nid();
		$query = new EntityFieldQuery;
		$query->entityCondition('entity_type', 'node');
		$query->entityCondition('bundle', 'sirius_worker');
		if ($domain_nid) { $query->fieldCondition('field_sirius_domain', 'target_id', $domain_nid); }
		$query->fieldCondition('field_sirius_denorm_level', 'value', "$ms_tid|$level_code");
		$query->fieldCondition('field_sirius_member_status', 'tid', $ms_tid);
		if ($ws_member_active_tids) { $query->fieldCondition('field_sirius_work_status', 'tid', $ws_member_active_tids, 'in'); }
		$query->count();
		$count = $query->execute();
		if (!$count) { continue; }

		$title = "Count: " . $ms_term->name . ' ' . $level_record['title'];

		$tools = l(
			'List', 
			'sirius/staff/worker/list', 
			array(
				'attributes' => array('class' => array('tool-button')),
				'query' => array(
					'field_sirius_denorm_level_value' => "$ms_tid|$level_code",
					'field_sirius_member_active_value' => 'Yes',
					'field_sirius_member_status_tid[]' => $ms_tid,
					'field_sirius_member_status_tid_op' => 'or',
				),
			)
		);

		$row = array(
			'title' => $title,
			'count' => $count,
			'tools' => $tools,
		);

		$rows[] = $row;
	}

	return $rows;
}

