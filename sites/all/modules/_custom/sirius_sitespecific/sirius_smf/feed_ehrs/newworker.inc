<?php

function sirius_smf_feed_ehrs_newworker_form($form, &$form_state, $info, $data, $feed_node, $current_step) {
	$form_state['info'] = $info;
	$form_state['data'] = $data;
	$form_state['feed_nid'] = $feed_node->nid;
	$form_state['current_step'] = $current_step;

	$which = preg_replace('/new_worker_/', '', $current_step);
	if (!($which > 0)) { return sirius_form_error("Don't know which new worker."); }
	$row = $data['records']['rows'][$which-1];
	if (!$row) { return sirius_form_error("New worker not found."); }

	$html = '<h2>Confirm New Worker Creation</h2>';

	$fields = sirius_feed_wizard_fields($info, $data, $feed_node, $current_step);

	foreach ($fields as $field_name => $field_info) {
		if ($field_info['type'] == 'term_reference') {
			$val = sirius_term_title($row[$field_name]); 
		} else {
			$val = $row[$field_name];
		}
		$html .= sirius_fakerender_field($field_info['title'], $val);
	}
	$html .= '<br />';

	$form['intro'] = array('#markup' => $html);


	$form['confirm'] = array(
		'#type' => 'checkbox',
		'#title' => t('Yes, the data above is correct. A new worker will be created.'),
		'#required' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Confirm New Worker Creation',
	);

	return sirius_smf_feed_ehrs_form_prep($form, $form_state, $info, $data, $feed_node, $current_step);
}

function sirius_smf_feed_ehrs_newworker_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$feed_node = node_load($form_state['feed_nid']);
	$current_step = $form_state['current_step'];

	sirius_wizard_submit($info, $data, $feed_node, $current_step, "New worker confirmed.", array('wizard_status' => 'incomplete', 'feed_status' => 'draft'));
}