<?php

function sirius_training_node_tab_certification_view_access($certification_node) {
	// Only log entries of type certification
	if ($certification_node->field_sirius_category['und'][0]['value'] != 'worker:certification') { return FALSE; }

	// Admin can access
	if (user_access('sirius training admin')) { return TRUE; }
	if (user_access('sirius training certification view')) { return TRUE; }

	// Author can access
	global $user;
	if ($user->uid == $certification_node->uid) {
		return TRUE;
	}

	// Oh well
	return FALSE;
}

function sirius_training_node_tab_certification_view($certification_node) {
	$html .= sirius_training_certifications_render($certification_node);
	return $html;
}
