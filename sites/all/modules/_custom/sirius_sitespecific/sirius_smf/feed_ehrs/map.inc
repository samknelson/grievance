<?php

function sirius_smf_feed_ehrs_map_form($form, &$form_state, $info, $data, $feed_node, $current_step) {
	$form = sirius_feed_wizard_map_form($form, $form_state, $info, $data, $feed_node, $current_step);
	$form['map_withheld_label']['#markup'] = '<td>Employee Contribution Amount Witheld<br><em>(Event Center plans only)</em></td>';
	return sirius_smf_feed_ehrs_form_prep($form, $form_state, $info, $data, $feed_node, $current_step);
}

function sirius_smf_feed_ehrs_map_form_submit($form, &$form_state) {
	return sirius_feed_wizard_map_form_submit($form, $form_state);
}