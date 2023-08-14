<?php

function sirius_pcct_policy() {
	$policy_nid = sirius_domain_variable_get('sirius_pcct_policy_nid');
	return node_load($policy_nid);
}

function sirius_pcct_policy_render($policy_node = NULL) {
	if (!$policy_node) { $policy_node = sirius_pcct_policy(); }

	$html = '<div class="sirius_pcct_policy">';

	if ($policy_node) {
		$settings = sirius_pcct_policy_settings($policy_node);

		$html .= '<h2>' . t('Benefits Summary: @policy', array('@policy' => $policy_node->title)) . '</h2>';

		$html .= '<table class="sirius_pcct_policy_table sirius_table sirius_table_basic">';

		$html .= '<tr>';
		$html .= '<th></th>';
		$html .= '<th>' . '<strong>' . t('Deductible') . '</strong>' . '</th>';
		$html .= '<th>' . '<strong>' . t('Out-of-Pocket Max') . '</strong>' . '</th>';
		$html .= '<th>' . '<strong>' . t('MH/BH Outpatient Copay') . '</strong>' . '</th>';
		$html .= '<th>' . '<strong>' . t('MH/BH Inpatient Coinsurance') . '</strong>' . '</th>';
		$html .= '</tr>';

		$html .= '<tr>';
		$html .= '<td>' . '<strong>' . t('Plan Coverage') . '</strong>' . '</td>';
		$html .= '<td>' . $settings['deductible_display'] . '</td>';
		$html .= '<td>' . $settings['oop_display'] . '</td>';
		$html .= '<td>' . $settings['mbbh_outpatient_copay_display'] . '</td>';
		$html .= '<td>' . $settings['mbbh_inpatient_coinsurance_display'] . '</td>';
		$html .= '</tr>';

		$html .= '</table>';

		$html .= '<div class="sirius_pcct_policy_description">'; 
		$html .= $settings['description'];
		$html .= '</div>';
	} else {
		$html .= '<em>' . t('No policy has been selected.') . '</em>';
	}

	$html .= '</div>';

	return $html;
}

function sirius_pcct_policy_settings($policy_node) {
	$json = sirius_json_get($policy_node);
	return $json['pcct']['settings'];
}