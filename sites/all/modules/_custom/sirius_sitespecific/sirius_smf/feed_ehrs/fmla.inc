<?php

function sirius_smf_feed_ehrs_fmla_form($form, &$form_state, $info, $data, $wizard_node, $current_step) {
	$pk = $data['pk'];
	$fields = sirius_feed_wizard_fields($info, $data, $wizard_node, $current_step);

	$html = '';

	$eup_node = sirius_smf_feed_ehrs_get_eup($wizard_node);
	if (!$eup_node) { return sirius_form_error("This feed is not associated with an employer upload."); }
	$eup = Sirius_Smf_Ehrs_Monthly::getInstance();
	$year = $eup->get_year($eup_node);
	$month = $eup->get_month($eup_node);
	$day = 1;
	$employer_node = $eup->get_employer($eup_node);
	$domain_nid = sirius_domain_nid();


	$rows = array();

	$fmla = Sirius_Smf_Fmla::getInstance();
	
	foreach ($data['records']['rows'] as $data_row) {
		if ($data_row['hour_type_id'] != 'fmla') { continue; }


		$row = array();
		$row[] = $data_row[$pk];
		$row[] = $data_row['name_first'];
		$row[] = $data_row['name_last'];
		$row[] = $data_row['hrs'];
		$row[] = $fmla->start_date($data_row['worker_nid'], $employer_node->nid, $year, $month);
		$rows[] = $row;
	}

	if ($rows) {
		$html = '<p>' . t('Please confirm the following FMLA workers.') . '</p>';
		$header = array();
		$header[] = $fields[$pk]['title'];
		$header[] = t('First Name');
		$header[] = t('Last Name');
		$header[] = t('Hours This Month');
		$header[] = t('FMLA Start Date');

		$html .= sirius_table_render($header, $rows);
		$form['intro'] = array('#markup' => $html);
		$form['confirm'] = array(
			'#type' => 'checkbox',
			'#title' => t('Yes, the data above is correct.'),
			'#required' => TRUE,
		);
	} else {
		$html = '<p>' . t('There are no FMLA workers in this upload.') . '</p>';
		$form['intro'] = array('#markup' => $html);
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Confirm FMLA and Continue',
	);

	return sirius_smf_feed_ehrs_form_prep($form, $form_state, $info, $data, $wizard_node, $current_step);
}

function sirius_smf_feed_ehrs_fmla_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$wizard_node = $form_state['wizard_node'];
	$current_step = $form_state['current_step'];

	sirius_wizard_submit($info, $data, $wizard_node, $current_step, "FMLA Confirmed.", array('nonsequential' => FALSE, 'feed_status' => 'draft'));
}
