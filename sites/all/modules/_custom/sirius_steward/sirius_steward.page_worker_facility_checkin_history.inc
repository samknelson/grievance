<?php

function sirius_steward_node_tab_worker_facility_checkin_history($worker_node) {
	$checkin_nids = sirius_steward_facility_checkin_search(array('worker_nid' => $worker_node->nid));
	return sirius_steward_facility_checkin_render_list(
		$checkin_nids, array('hide_worker' => TRUE, 'hide_phone' => TRUE));
}

