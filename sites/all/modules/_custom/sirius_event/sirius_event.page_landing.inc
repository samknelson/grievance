<?php

function sirius_event_page_landing_access() {
	return TRUE;
}

function sirius_event_page_landing() {
	global $user;
	if (!$user || !$user->uid) { return 'No user.'; }
	$user_full = user_load($user->uid);
	$contact_nid = $user_full->field_sirius_contact['und'][0]['target_id'];
	if (!$contact_nid) { return 'No contact.'; }

	$query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_event_participant');
	$query->fieldCondition('field_sirius_contact', 'target_id', $contact_nid);
	$query->propertyOrderBy('nid', 'desc');
  $stmt = $query->execute();
  if ($stmt['node']) {
  	$participant_nids = array_keys($stmt['node']);
  }

  /*
	$event_types = sirius_taxonomy_load('sirius_event_type', 'tid', 'full');
	$create_html = '';
	foreach ($event_types as $event_type_tid => $event_type_term) {
		if (drupal_valid_path("sirius/event/create/$event_type_tid")) {
			$create_html .= l(
				t('Create a new @type', array('@type' => $event_type_term->name)),
				"sirius/event/create/$event_type_tid",
				array('attributes' => array('class' => 'tool-button'))
			);
			$create_html .= ' ';
		}
	}
	if ($create_html) {
		$html .= '<h2>' . t('Create a New Event') . '</h2>';
		$html .= $create_html;
	}
	*/

	$html .= '<h2>' . t('Your Events') . '</h2>';

  if ($participant_nids) {
  	$participant_nodes = node_load_multiple($participant_nids);
  	$event_nids = array();
  	foreach ($participant_nodes as $participant_nid => $participant_node) {
  		$event_nids[] = $participant_node->field_sirius_event['und'][0]['target_id'];
  	}
  	$event_nodes = node_load_multiple($event_nids);
  	$rows = array();
		foreach ($event_nodes as $event_nid => $event_node) {
			$tools = '';
			$paths = array();
			$paths['node/' . $event_nid] = 'View';
			if (sirius_event_phonebank_enabled($event_node)) {
				$paths['node/' . $event_nid . '/sirius_event_phonebank/call'] = t('Make Calls');
			}
			if (sirius_event_bullpen_enabled($event_node)) {
				$paths['node/' . $event_nid . '/sirius_event_bullpen'] = t('Manage Bullpen');
			}

			foreach ($paths as $path => $name) {
				if (!drupal_valid_path($path)) { continue; }
				if ($tools) { $tools .= ' '; }
				$tools .= l($name, $path, array('attributes' => array('class' => array('tool-button'))));
			}

			$dates = sirius_fieldvals($event_node, 'field_sirius_daterepeat');
			$datestr = '';
			foreach ($dates as $date) {
				if ($datestr) { $datestr .= '<br />'; }
				$datestr .= $date;
			}

			$row = array();
			$row[] = $event_node->title;
			$row[] = $datestr;
			$row[] = $tools;
			$rows[] = $row;
		}
		$header = array('Event', 'Date(s)', 'Tools');
		$html .= theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => array()));
  } else {
  	$html .= "You are not currently registered for any events.";
  }

  if (sirius_event_page_list_access()) {
  	$html .= sirius_event_page_list();
  }

	return $html;
}
