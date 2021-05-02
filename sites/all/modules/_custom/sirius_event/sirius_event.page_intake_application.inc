<?php

function sirius_node_tab_event_intake_application_access($application_node) {
	if ($application_node->field_sirius_category['und'][0]['value'] != 'event:intake_application') { return FALSE; }
	return TRUE;
}

function sirius_node_tab_event_intake_application($application_node) {
	$application_json = sirius_jsonfield_util_node_json_get($application_node);

	$html = '';

	$defs = sirius_esignature_defs($application_node);
	if ($defs) {
		sirius_esignature_init();

		$html .= '<h2>' . t('Signatures') . '</h2>';
		foreach ($defs as $def) {
			$name = $def['title'];
			$code = $def['code'];
			$esignature = sirius_jsonfield_util_esignature_get($application_json, $code);
			$json = sirius_jsonfield_util_node_json_get($application_node);
			$html .= sirius_esignature_render($esignature, 'full', $application_node->nid, $code);
			$html .= '<br />';
		}
	}

	if (!$html) {
		$html = '<p>' . t('(Empty application -- no information.)') . '</p>';
	}

	return $html;
}