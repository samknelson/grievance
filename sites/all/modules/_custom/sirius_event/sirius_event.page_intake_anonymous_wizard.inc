<?php

function sirius_event_page_intake_anonymous_wizard($access_code) {
	if (!$access_code) {
		drupal_set_message(t("No access code."), 'error');
		drupal_goto('intake');
	}

	$event_node = sirius_event_intake_fetch_by_access_code($access_code);
	if (!$event_node) {
		drupal_set_message(t("That does not appear to be a valid access code."), 'error');
		drupal_goto('intake');
	}
	$json = sirius_jsonfield_util_node_json_get($event_node);
	if ($json['intake']['enabled'] != 'Yes') {
		drupal_set_message(t('Intake is not enabled for that event.'), 'error');
		drupal_goto('intake');
	}
	if ($json['intake']['anonymous']['enabled'] != 'Yes') {
		drupal_set_message(t('Anonymous intake is not enabled for that event.'), 'error');
		drupal_goto('intake');
	}

	$application_node = sirius_event_intake_mine($event_node->nid);
	if (!$application_node) {
		drupal_set_message(t('Intake application not found.'), 'error');
		drupal_goto('intake/' . $access_code);
	}
	$application_json = sirius_jsonfield_util_node_json_get($application_node);

	$domain_nid = $event_node->field_sirius_domain['und'][0]['target_id'];
	if ($domain_nid) { sirius_domain($domain_nid); }

	drupal_set_title($application_node->title);
	sirius_backlink_add('intake/' . $access_code, array('name' => 'Start Over'));

  $active = sirius_event_intake_active($event_node);
  if (!$active['success']) {
  	return $active['msg'];
  } else {

  	$status_options = sirius_event_intake_application_status_options();
  	$status = $application_node->field_sirius_type['und'][0]['value'];
  	$html = '<p>'; 
  	$html .= "Application status: <strong>" . $status_options[$status] . '</strong>';
  	if ($status == 'draft') {
	  	$closes_in = sirius_event_intake_closes_in($event_node);
	  	if ($closes_in) { 
		  	$html .= " / <strong>" . $closes_in . '</strong>';
	  	}
	  }
		$submitted_ts = $application_json['wizard']['intake']['submitted_ts'];
		if ($submitted_ts) {
			$html .= " / Submitted on: <strong>" . date('l, F d Y - g:i a', $submitted_ts) . '</strong>';
		}

		$html .= sirius_node_tab_wizard($application_node, 'event_intake');
		return $html;
	}
}
