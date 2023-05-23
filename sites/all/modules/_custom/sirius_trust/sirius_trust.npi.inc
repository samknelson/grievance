<?php

function sirius_trust_provider_npi_format($string) {
	$string = trim($string);
	if ($string) {
		while (strlen($string) < 10) { $string = "0$string"; }
	}
	return $string;
}

function sirius_trust_provider_npi_validate($string) {
	$string = sirius_trust_provider_npi_format($string);
	return preg_match('/^\d\d\d\d\d\d\d\d\d\d$/', $string);
}

function sirius_trust_provider_npi_node_validate($node, $form, &$form_state) {
	$npi = $node->field_sirius_provider_npi['und'][0]['value'];
	if ($npi) {
		if (!sirius_trust_provider_npi_validate($npi)) {
			form_set_error('field_sirius_provider_npi', t('Please enter a valid 10-digit NPI'));
		}
	}
}
