<?php

function sirius_smf_feed_ehrs_term_proposed_form($form, &$form_state, $info, $data, $wizard_node, $current_step) {
	$pk = $data['pk'];
	$fields = sirius_feed_wizard_fields($info, $data, $wizard_node, $current_step);

	// Get some context
  $monthly = Sirius_Smf_Ehrs_Monthly::getInstance();
	$eup_node = sirius_smf_feed_ehrs_get_eup($wizard_node);
	if (!$eup_node) { return sirius_form_error("System error: eup node not found."); }
	$employer_node = $monthly->get_employer($eup_node);
	$year = $monthly->get_year($eup_node);
	$month = $monthly->get_month($eup_node);
	if (!$employer_node || !$year || !$month) { return sirius_form_error("System error: employer, year, or month not found."); }

	// Make a lookup map
	$map = array();
	foreach ($data['records']['rows'] as $idx => $row) {
		$map[$row['worker_nid']] = $idx;
	}

	// Do a dispatch search
	$dispatch_nodes = sirius_dispatch_search(
		array(
			'employer_nid' => $employer_node->nid,
			'types' => array('Primary', 'Secondary'),
			'ts' => strtotime("$year-$month-15"), // Avoid timezone silliness
		)
	);

	foreach ($dispatch_nodes as $dispatch_nid => $dispatch_node) {
		$worker_nid = $dispatch_node->field_sirius_worker['und'][0]['target_id'];
		if (!$worker_nid) { continue; }

		// Skip them if they appear on the upload.
		$idx = $map[$worker_nid];
		if ($idx) { continue; }

		// find the total number of hours in the preceding months
		$y = $year; $m = $month;
		$hrs = 0;
		for ($i = 1; $i<=4; ++$i) {
			$y = $year;
			$m = $month - $i;
			if ($m < 1) { $m += 12; --$y; }
			$sql = "select sum(hours) as c from sirius_hours_cache where worker_nid = :worker_nid and year = :y and month = :m";
			$stmt = sirius_sql_query($sql, array(':worker_nid' => $worker_nid, ':y' => $y, ':m' => $m));
			$hr = $stmt->fetchAssoc();
			$hrs += $hr['c'];
		}
		if ($hrs) { continue; }

		$row = array();
		$worker_node = node_load($worker_nid);
		if (!$worker_node) { continue; }
		$row['ssn'] = $worker_node->field_sirius_ssn['und'][0]['value'];
		if (!$row['ssn']) { $row['ssn'] = "[No SSN; NID $worker_nid]"; }
		$row['name_first'] = $worker_node->field_sirius_name['und'][0]['given'];
		$row['name_last'] = $worker_node->field_sirius_name['und'][0]['family'];
		$rows[$worker_nid] = $row;
	}

	if ($rows) {
		$html = '<p>' . t('The following workers have had 0 hours for the preceding four months. Check the workers that should be terminated.') . '</p>';

		$header = array();
		$header['ssn'] = t('SSN');
		$header['name_first'] = t('First Name');
		$header['name_last'] = t('Last Name');

		$form['intro'] = array('#markup' => $html);
		$form['term_proposed'] = array(
			'#type' => 'tableselect',
			'#header' => $header,
			'#options' => $rows,
			'#attributes' => array('class' => array('sirus_table')),
		);
	} else {
		$html = '<p>' . t('There are no workers with 0 hours for the preceding four mounths.') . '</p>';
		$form['intro'] = array('#markup' => $html);
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Confirm and Continue',
	);

	return sirius_sms_feed_ehrs_form_prep($form, $form_state, $info, $data, $wizard_node, $current_step);
}

function sirius_smf_feed_ehrs_term_proposed_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$wizard_node = $form_state['wizard_node'];
	$current_step = $form_state['current_step'];

	if ($form_state['values']['term_proposed']) {
		$data['term_proposed'] = array_filter(array_values($form_state['values']['term_proposed']));
	}

	sirius_wizard_submit($info, $data, $wizard_node, $current_step, "Confirmed.", array('nonsequential' => FALSE));
}
