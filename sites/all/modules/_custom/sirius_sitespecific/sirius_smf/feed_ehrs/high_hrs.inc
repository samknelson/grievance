<?php

function sirius_smf_feed_ehrs_high_hrs_form($form, &$form_state, $info, $data, $wizard_node, $current_step) {
	$pk = $data['pk'];
	$fields = sirius_feed_wizard_fields($info, $data, $wizard_node, $current_step);

	$html = '';

	$rows = array();
	foreach ($data['records']['rows'] as $data_row) {
		if ($data_row['hrs'] < 220) { continue; }
		$row = array();
		$row[] = $data_row[$pk];
		$row[] = $data_row['name_first'];
		$row[] = $data_row['name_last'];
		$row[] = $data_row['hrs'];
		$rows[] = $row;
	}

	if ($rows) {
		$html = '<p>' . t('Please confirm the following workers with high hours.') . '</p>';
		$header = array();
		$header[] = $fields[$pk]['title'];
		$header[] = t('First Name');
		$header[] = t('Last Name');
		$header[] = t('Hours This Month');

		$html .= sirius_table_render($header, $rows);
		$form['intro'] = array('#markup' => $html);
		$form['confirm'] = array(
			'#type' => 'checkbox',
			'#title' => t('Yes, the data above is correct.'),
			'#required' => TRUE,
		);
	} else {
		$html = '<p>' . t('There are no workers with high hours on this upload.') . '</p>';
		$form['intro'] = array('#markup' => $html);
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Confirm and Continue',
	);

	return sirius_smf_feed_ehrs_form_prep($form, $form_state, $info, $data, $wizard_node, $current_step);
}

function sirius_smf_feed_ehrs_high_hrs_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$wizard_node = $form_state['wizard_node'];
	$current_step = $form_state['current_step'];

	sirius_wizard_submit($info, $data, $wizard_node, $current_step, "Confirmed.", array('nonsequential' => FALSE, 'feed_status' => 'draft'));
}
