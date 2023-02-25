<?php

function sirius_steward_node_tab_facility_checkin_history($facility_node) {
	$checkin_nids = sirius_steward_checkin_search(array('facility_nid' => $facility_node->nid));
	return sirius_steward_checkin_render_list($checkin_nids, array('hide_facility' => TRUE));
}