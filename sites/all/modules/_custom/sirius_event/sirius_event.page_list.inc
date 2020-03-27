<?php

function sirius_event_page_list_access() {
	if (user_access('sirius event admin')) { return TRUE; }
	return TRUE;
}

function sirius_event_page_list() {
	$tree = sirius_taxonomy_load('sirius_event_type', 'tid', 'full');
	if (!$tree) { return t('No event types have been configured.'); }

	foreach ($tree as $tid => $term) {
		$html .= '<h2>' . $term->name . '</h2>';
		if (strlen($term->description) > 5) {
			$html .= $term->description;
			$html .= '<br />';
		}

		if (sirius_event_page_list_type_access($tid)) {
			$html .= sirius_event_page_list_type_upcoming($tid);
		}
		$html .= l(t('Full List'), 'sirius/event/list/' . $term->tid . '/all', array('attributes' => array('class' => array('tool-button'))));
		if (drupal_valid_path("sirius/event/create/$event_type_tid")) {
			$html .= ' ' . l(
				t('Create'),
				"sirius/event/create/$tid",
				array('attributes' => array('class' => 'tool-button'))
			);
		}
	}

	return $html;
}

function sirius_event_page_list_type_access($event_type_tid) {
	// @todo
	return user_access('sirius');
}

function sirius_event_page_list_type($event_type_tid) {
	drupal_goto(current_path() . '/upcoming');
}

function sirius_event_page_list_type_upcoming($event_type_tid) {
	$event_type_term = taxonomy_term_load($event_type_tid);
	sirius_event_page_list_type_set_title($event_type_term);
	$domain_nid = sirius_domain_nid();
	if (!$domain_nid) { $domain_nid = 'NULL'; }
	$html .= views_embed_view('sirius_events', 'block_2', $domain_nid, $event_type_tid);
	return $html;
}

function sirius_event_page_list_type_all($event_type_tid) {
	$event_type_term = taxonomy_term_load($event_type_tid);
	sirius_event_page_list_type_set_title($event_type_term);
	$domain_nid = sirius_domain_nid();
	if (!$domain_nid) { $domain_nid = 'NULL'; }
	$html .= views_embed_view('sirius_events', 'block_1', $domain_nid, $event_type_tid);
	return $html;
}

function sirius_event_page_list_type_set_title($event_type_term) {
	drupal_set_title('Events: ' . $event_type_term->name);
}