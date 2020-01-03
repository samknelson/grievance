<?php

function sirius_event_page_list_bullpens_access() {
	return TRUE; 
}

function sirius_event_page_list_bullpens() {
	$event_types = sirius_taxonomy_load('sirius_event_type', 'tid', 'full');

	$event_type_tids = array();
	foreach ($event_types as $event_type_tid => $event_type_term) {
		if (sirius_event_bullpen_enabled_type($event_type_term)) {
			$event_type_tids[] = $event_type_tid;
		}
	}

	if (!$event_type_tids) {
		return t('There are no event types configured as bullpens.');
	}

	$query = new EntityFieldQuery;
	$query->entityCondition('entity_type', 'node');
	$query->entityCondition('bundle', 'sirius_event');
	$query->fieldCondition('field_sirius_event_type', 'tid', $event_type_tids, 'in');;
	$query->fieldCondition('field_sirius_active', 'value', 'Yes');
	$query->fieldCondition('field_sirius_daterepeat', 'value', gmdate('Y-m-d 00:00:00'), '>=');
	$query->fieldOrderBy('field_sirius_daterepeat', 'value', 'desc');
	$stmt = $query->execute();
	if (!$stmt['node']) { 
		return t('There are no upcoming bullpens.');
	}

	$event_nodes = node_load_multiple(array_keys($stmt['node']));

	$html = '<h2>' . t('Upcoming Bullpens') . '</h2>';
	foreach ($event_nodes as $event_node) {
		$ts = sirius_event_bullpen_ts($event_node);
		$datestr = 'l, F j, g:i A';
		$style = "display: inline-block; width: 170px; font-weight: bold;";

	  $host_job_node = sirius_event_bullpen_job($event_node->nid);
	  $employer_node = node_load($host_job_node->field_grievance_shop['und'][0]['target_id']);
	  $facility_node = node_load($host_job_node->field_sirius_dispatch_facility['und'][0]['target_id']);
	  $hall_node = node_load($host_job_node->field_sirius_dispatch_hall['und'][0]['target_id']);

	  $html .= sirius_fakerender_field('Date:', date($datestr, $ts['job_ts']));
	  if ($employer_node) { $html .= sirius_fakerender_field('Host Employer:', $employer_node->title); }
	  if ($facility_node) { $html .= sirius_fakerender_field('Facility:', $facility_node->title); }
	  if ($hall_node) { $html .= sirius_fakerender_field('Hall:', $hall_node->title); }
	  if ($host_job_node->field_sirius_address_notes['und'][0]['value']) { $html .= sirius_fakerender_field('Report Location:', $host_job_node->field_sirius_address_notes['und'][0]['value']); }
	  $html .= sirius_dispatch_view_render_field($host_job_node, 'field_sirius_member_status', t('Member Statuses'));
	  $html .= l('View', 'node/' . $event_node->nid . '/sirius_event_info', array('attributes' => array('class' => array('tool-button'))));
	  $html .= '<br /><br />';
	}

	return $html;
}
