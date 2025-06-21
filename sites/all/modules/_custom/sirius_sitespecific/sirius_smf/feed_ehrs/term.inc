<?php

function sirius_smf_feed_ehrs_term_form($form, &$form_state, $info, $data, $wizard_node, $current_step) {
	$pk = $data['pk'];
	$fields = sirius_feed_wizard_fields($info, $data, $wizard_node, $current_step);

	$html = '';

	$rows = array();
	foreach ($data['records']['rows'] as $data_row) {
		if ($data_row['hour_type_id'] != 'terminated') { continue; }
		$row = array();
		$row[] = $data_row[$pk];
		$row[] = $data_row['name_first'];
		$row[] = $data_row['name_last'];
		$row[] = 'Flagged for termination on the upload';
		$rows[] = $row;
	}

	if ($data['term_proposed']) {
		foreach ($data['term_proposed'] as $worker_nid) {
			$worker_node = node_load($worker_nid);
			$row = array();
			$row[] = $worker_node->field_sirius_ssn['und'][0]['value'];
			$row[] = $worker_node->field_sirius_name['und'][0]['given'];
			$row[] = $worker_node->field_sirius_name['und'][0]['family'];
			$row[] = 'Marked for termination because of low hours';
			$rows[] = $row;
		}
	}

	if ($rows) {
		$html = '<p>' . t('Please confirm the following terminations.') . '</p>';
		$header = array();
		$header[] =  $fields[$pk]['title'];
		$header[] = t('First Name');
		$header[] = t('Last Name');
		$header[] = '';

		$html .= sirius_table_render($header, $rows);
		$form['intro'] = array('#markup' => $html);
		$form['confirm'] = array(
			'#type' => 'checkbox',
			'#title' => t('Yes, the data above is correct.'),
			'#required' => TRUE,
		);
	} else {
		$html = '<p>' . t('There are no workers marked for termination on this upload.') . '</p>';
		$form['intro'] = array('#markup' => $html);
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Confirm and Continue',
	);

	return sirius_smf_feed_ehrs_form_prep($form, $form_state, $info, $data, $wizard_node, $current_step);
}

function sirius_smf_feed_ehrs_term_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$wizard_node = $form_state['wizard_node'];
	$current_step = $form_state['current_step'];

	sirius_wizard_submit($info, $data, $wizard_node, $current_step, "Terminations Confirmed.", array('nonsequential' => FALSE, 'feed_status' => 'draft'));
}
