<?php

function sirius_node_tab_worker_relationships_access($worker_node) {
	// Anybody with global worker view permission
	if (user_access('sirius worker view')) { return TRUE; }

	// Handle Sirius Trust access
	if (sirius_domain_component_enabled('trust')) {
		if (Sirius_Trust_Access::getInstance()->access('worker_relationships_landing', array('worker_node' => $worker_node))) {
			return TRUE;
		}
	}

	// Oh well
	return FALSE;
}


function sirius_node_tab_worker_relationships($worker_node) {
	sirius_require('sirius', 'sirius.page_contact_relationships.inc');

	$is_staff = user_access('sirius worker view');
	if (!$is_staff && sirius_domain_component_enabled('trust')) {
		$is_staff = Sirius_Trust_Access::getInstance()->access('staff', array('worker_node' => $worker_node));
	}

	$options = array();
	if (!$is_staff) {
		$options['hide_tools'] = TRUE;
	}
	$html = sirius_node_tab_contact_relationships(node_load($worker_node->field_sirius_contact['und'][0]['target_id']), $options);

	return $html;
}
