<?php

function sirius_smf_node_tab_worker_cobra_rates($worker_node) { 
	$cobra = Sirius_Smf_Cobra::getInstance();
	$result = SiriusResult::create($cobra->rate_from_worker($worker_node));
	// $result->drupalSetMessage();

	if ($result->success()) {
		$html = sirius_fakerender_field('Cobra Rate:', '$' . number_format($result->get('rate'), 2));
	} else {
		$html = sirius_fakerender_field('Cobra Rate:', $result->msg());
	}

	$html .= '<hr>';
	$html .= '<h2>' . t('Computation Details') . '</h2>';
	$html .= sirius_minilog_render_html();

	return $html;
}