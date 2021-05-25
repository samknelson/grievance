<?php

function sirius_event_page_landing_access() {
	return TRUE;
}

function sirius_event_page_landing() {
	global $user;
	$domain_nid = sirius_domain_nid();
	if (!$user || !$user->uid) { return 'No user.'; }
	$user_full = user_load($user->uid);
	$contact_nid = $user_full->field_sirius_contact['und'][0]['target_id'];
	if (!$contact_nid) { return 'No contact.'; }

	$query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_event_participant');
	$query->fieldCondition('field_sirius_contact', 'target_id', $contact_nid);
	if ($domain_nid) { $query->fieldCondition('field_sirius_domain', 'target_id', $domain_nid); }
	$query->propertyOrderBy('nid', 'desc');
  $stmt = $query->execute();
  if ($stmt['node']) {
  	$participant_nids = array_keys($stmt['node']);
  }

	$my_event_nids = array();
  if ($participant_nids) {
  	$participant_nodes = node_load_multiple($participant_nids);
  	foreach ($participant_nodes as $participant_nid => $participant_node) {
  		$my_event_nids[] = $participant_node->field_sirius_event['und'][0]['target_id'];
  	}
  	$my_event_nodes = node_load_multiple($my_event_nids);
		$html .= '<h2>' . t('Your Events') . '</h2>';
  	$html .= sirius_event_page_landing_render($my_event_nodes, $participant_nodes);
  } 

  $now = gmdate('Y-m-d 00:00:00', strtotime('-1 days'));
	$query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_event');
	$query->fieldCondition('field_sirius_daterepeat', 'value', $now, '>=');
	if ($domain_nid) { $query->fieldCondition('field_sirius_domain', 'target_id', $domain_nid); }
	if ($my_event_nids) { $query->propertyCondition('nid', $my_event_nids, 'not in'); }
	$query->propertyOrderBy('nid', 'desc');
  $stmt = $query->execute();
  if ($stmt['node']) {
  	$other_event_nids = array_keys($stmt['node']);
  }
  if ($other_event_nids) {
  	$other_event_nodes = node_load_multiple($other_event_nids);
		$html .= '<h2>' . t('Upcoming Events') . '</h2>';
  	$html .= sirius_event_page_landing_render($other_event_nodes, array());
  } 

  if (sirius_event_page_list_access()) {
  	// $html .= sirius_event_page_list();
  }

	$event_types = sirius_taxonomy_load('sirius_event_type', 'tid', 'full');
	$create_html = '';
	foreach ($event_types as $event_type_tid => $event_type_term) {
		if (drupal_valid_path("sirius/event/list/$event_type_tid")) {
			$create_html .= $event_type_term->name . ': ';
			$create_html .= l(
				t('Full List', array('@type' => $event_type_term->name)),
				"sirius/event/list/$event_type_tid/all",
				array('attributes' => array('class' => 'tool-button'))
			);
			$create_html .= ' ';
		}
		if (drupal_valid_path("sirius/event/create/$event_type_tid")) {
			$create_html .= l(
				t('Create', array('@type' => $event_type_term->name)),
				"sirius/event/create/$event_type_tid",
				array('attributes' => array('class' => 'tool-button'))
			);
			$create_html .= '<br />';
		}
	}

	if ($create_html) {
		$html .= '<h2>' . t('All Events') . '</h2>';
		$html .= $create_html;
	}

	if (!$html) {
		$html = "There are no events or event types.";
	}

	return $html;
}

function sirius_event_page_landing_render($event_nodes, $participant_nodes = array()) {
	if (!$event_nodes) { return ''; }

	$participant_map = array();
	foreach ($participant_nodes as $participant_nid => $participant_node) {
		$participant_map[$participant_node->field_sirius_event['und'][0]['target_id']] = $participant_nid;
	}

	$rows = array();
	foreach ($event_nodes as $event_nid => $event_node) {
		$event_type_tid = $event_node->field_sirius_event_type['und'][0]['tid'];
		$event_type_term = taxonomy_term_load($event_type_tid);
		$json = json_decode($event_type_term->field_sirius_json['und'][0]['value'], TRUE);

		$tools = '';
		$paths = array();

		$paths['node/' . $event_nid . '/sirius_event_info'] = 'Info';
		$paths['node/' . $event_nid . '/sirius_event_participants_current'] = t('Participants');
		$paths['node/' . $event_nid . '/sirius_event_phonebank/call'] = t('Phone Bank');
		$paths['node/' . $event_nid . '/sirius_event_map'] = t('Map');
		$paths['node/' . $event_nid . '/sirius_event_intake'] = t('Intake');


		foreach ($paths as $path => $name) {
			if (!drupal_valid_path($path)) { continue; }
			if ($tools) { $tools .= ' '; }
			$tools .= l($name, $path, array('attributes' => array('class' => array('tool-button'))));
		}

		// Register
		$widgets = sirius_event_register_widgets();
		$all_rules = $json['register']['rules'];
		if (user_access('sirius event admin')) { $rules = $all_rules; } 
		else { $rules = sirius_event_paccess_rule_match($event_node->nid, $all_rules, TRUE); }
		if ($rules) {
			$first = TRUE;
			foreach ($rules as $delta => $rule) {
				foreach ($rule['widget'] as $widget) {
					if ($first) { $tools .= '<br />' . t('Register: '); $first = FALSE; }
					if ($widget == 'user_self' && $participant_map[$event_nid]) { continue; }
					$tools .= l($widgets[$widget], 'node/' . $event_nid . '/sirius_event_participants_register', array('attributes' => array('class' => array('tool-button')), 'query' => array('delta' => $delta, 'widget' => $widget))); 
				}
			}
		}

		$dates = sirius_fieldvals($event_node, 'field_sirius_daterepeat');
		$datestr = '';
		foreach ($dates as $date) {
			if ($datestr) { $datestr .= '<br />'; }
			$datestr .= $date;
		}

		$row = array();
		$row[] = $event_type_term->name;
		$row[] = $event_node->title;
		$row[] = $datestr;
		$row[] = $tools;
		$rows[] = $row;
	}
	$header = array(
		array('data' => 'Event Type', 	'width' => '15%'),
		array('data' => 'Event', 				'width' => '25%'),
		array('data' => 'Date(s)', 			'width' => '20%'),
		array('data' => 'Tools', 				'width' => '40%'),
	);

	$html .= '<div class="sirius_event_list">';
	$html .= theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => array()));
	$html .= '</div>';

	return $html;
}