<?php

function sirius_node_tab_event_participants_current_access($event_node) {
	if (user_access('sirius event admin')) { return TRUE; }

	$event_type_term = taxonomy_term_load($event_node->field_sirius_event_type['und'][0]['tid']);
	$json = json_decode($event_type_term->field_sirius_json['und'][0]['value'], TRUE);
	$rules = sirius_event_paccess_rule_match($event_node->nid, $json['participants']['rules'], TRUE);
	if ($rules) { return TRUE; }

	return FALSE; 
}

function sirius_node_tab_event_participants_current($event_node) {
	$event_type_term = taxonomy_term_load($event_node->field_sirius_event_type['und'][0]['tid']);
	$json = json_decode($event_type_term->field_sirius_json['und'][0]['value'], TRUE);
	$all_rules = $json['participants']['rules'];
	if (user_access('sirius_event_admin')) {
		$rules = $all_rules;
	} else {
		$rules = sirius_event_paccess_rule_match($event_node->nid, $all_rules, TRUE);
	}

	$delta = $_REQUEST['delta'];
	if (!isset($delta)) {
		$widgets = sirius_event_participants_widgets();

		if (!$rules) {
			return t('You do not have access to any participant rules for this event type.');
		}
		$html = '';
		foreach ($rules as $delta => $rule) {
			$html .= '<strong>' . $rule['name'] . '</strong><br />';

			$proles = array();
			foreach ($rule['participants'] as $key) {
				list ($prole, $pstatus) = explode('|', $key);
				$proles[$prole] = $prole;
			}

			foreach (array_keys($proles) as $prole) {
				$prole_term = taxonomy_term_load($prole);
				foreach ($rule['widget'] as $widget) {
					$html .= l($widgets[$widget] . ': ' . $prole_term->name, current_path(), array('attributes' => array('class' => array('tool-button')), 'query' => array('delta' => $delta, 'widget' => $widget, 'prole' => $prole))); 
				}
			}

			$html .= '<br /><br />';
		}
		return $html;
	} else {
		$rule = $rules[$delta];
		$widget = $_REQUEST['widget'];
		$prole = preg_replace('/[^\d]/', '', $_REQUEST['prole']);
		if (!$rule) { return t('No such rule.'); }
		if (!in_array($widget, $rule['widget'])) { return t('No such widget.'); }
		$found = FALSE;
		foreach ($rule['participants'] as $key) {
			if (preg_match("/^$prole/", $key)) { $found = TRUE; }
		}
		if (!$found) { return t('No access to that prole.'); }

		return sirius_node_tab_event_participants_current_rule($event_node, $rule, $widget, $prole);
	}
}

function sirius_node_tab_event_participants_current_rule($event_node, $rule, $widget, $prole) {
	$prole_term = taxonomy_term_load($prole);
	drupal_set_title(drupal_get_title() . ': ' . $prole_term->name);
	if ($widget == 'view') {
		return views_embed_view('sirius_event_participants', 'block_1', $event_node->nid, $prole);
	} else if ($widget == 'status') {
		return views_embed_view('sirius_event_participants', 'block_1', $event_node->nid, $prole);
	} else if ($widget == 'contact') {
		return views_embed_view('sirius_event_participants', 'block', $event_node->nid, $prole);
	} else if ($widget == 'reports') {
		return sirius_node_tab_event_participants_reports($event_node->nid);
	} else {
		return t('Widget not found: @widget.', array('@widget' => $widget));;
	}
}

function sirius_node_tab_event_participants_current_defunct($event_node) {

	$html = sirius_node_tab_event_participants_quicknav($event_node, 'current');

	$participant_role_tid = $_REQUEST["participant_role_tid"];
	if (!$participant_role_tid) {
		$html .= "To see the participant list or register additional participants,. please select a role from the list above.";
	} else {
		$html .= views_embed_view('sirius_event_participants', 'block', $event_node->nid, $participant_role_tid);
	}

	return $html;
}

function sirius_event_participant_role_status_tids($participant_role_tid) {
	$args = array();
	$sql = "select tid, name from taxonomy_term_data, field_data_field_sirius_event_proles ";
	$sql .= "where taxonomy_term_data.tid = entity_id ";
	$sql .= "and entity_type = 'taxonomy_term' ";
	$sql .= "and bundle = 'sirius_event_participant_status' ";
	if (is_array($participant_role_tid)) {
		$sql .= "and field_sirius_event_proles_tid in (:participant_role_tid) ";
		$args[':participant_role_tid'] = $participant_role_tid;
	} else {
		$sql .= "and field_sirius_event_proles_tid = :participant_role_tid ";
		$args[':participant_role_tid'] = $participant_role_tid;
	}
	$sql .= "order by weight, name ";
	$stmt = db_query($sql, $args);
	$statuses = array();
	while ($hr = $stmt->fetchAssoc()) {
		$statuses[$hr['tid']] = $hr['name'];
	}
	return $statuses;
}

function sirius_node_tab_event_participants_quicknav($event_node, $suffix) {
	$event_type_term = taxonomy_term_load($event_node->field_sirius_event_type['und'][0]['tid']);
	if (!$event_type_term) { 
		drupal_set_message('This event does not have a type set.', 'warning');
		return '';
	}

	$participant_role_tids = sirius_fieldvals($event_type_term, 'field_sirius_event_proles', 'tid');
	if (!$participant_role_tids) { 
		drupal_set_message("The event type \"" . $event_type_term->name . "\" does not have any participant roles associated with it. Please go to the drop-down list configuration page and configure the event type.", 'warning');
		return '';
	}

	$base_path = 'node/' . $event_node->nid . '/sirius_event_participants_' . $suffix;

	$participant_role_terms = taxonomy_term_load_multiple($participant_role_tids);
	$links = array();

	// Overview page
	$link = array();
	$link['title'] = 'Overview';
	$link['path'] = $base_path;
	$links[] = $link;

	foreach ($participant_role_terms as $participant_role_tid => $participant_role_term) {
		$participant_json = json_decode($participant_role_term->field_sirius_json['und'][0]['value'], TRUE);

		// Which registration options are available?
		if ($suffix == 'register/byname' && $participant_json['registration']['byname'] != 'Yes') { continue; }
		if ($suffix == 'register/bulk' && $participant_json['registration']['bulk'] != 'Yes') { continue; }

		// Participant list for each role
		$link = array();
		$link['title'] = $participant_role_term->name;
		$link['path'] = $base_path;
		$link['query'] = array('participant_role_tid' => $participant_role_tid);
		$links[] = $link;
	}

	return sirius_quicknav($links);
}

function sirius_event_contact_search($event_nid, $participant_role_tid, $string) {
	$match = '%' . $string . '%';
	$domain_nid = sirius_domain_nid();
	$sql = "select nid, title from ";
	$sql .= "node ";
	$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
	$sql .= "where node.type = 'sirius_contact' ";
	$sql .= "and field_sirius_domain_target_id = :domain_nid ";
	$sql .= "and LOWER(node.title) like LOWER(:match) ";
	$sql .= "order by node.title ";
	$sql .= "limit 10 ";
	$stmt = db_query($sql, array(':domain_nid' => $domain_nid, ':match' => $match));
	$matches = array();
	while ($hr = $stmt->fetchAssoc()) {
		$idx = check_plain($hr['title']) . ' (' . $hr['nid'] . ')';
		$label = check_plain($hr['title']);
		$matches[$idx] = $label;
	}

  drupal_json_output($matches);
}


function sirius_node_tab_event_participants_reports($event_nid) {
	return drupal_get_form('sirius_node_tab_event_participants_reports_form', $event_nid);
}

function sirius_node_tab_event_participants_reports_form($form, &$form_state, $event_nid) {
	$form_state['event_nid'] = $event_nid;
	$event_node = node_load($event_nid);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_node_tab_event_participants_reports_form_submit($form, &$form_state) {
	$event_node = node_load($form_state['event_nid']);

	$survey_nid = $event_node->field_sirius_json_definition['und'][0]['target_id'];
	$survey_node = node_load($survey_nid);
	if (!$survey_node) { drupal_set_message("No survey!"); return array(); }
	$survey_form_definition = json_decode($survey_node->field_sirius_json['und'][0]['value'], TRUE);

	$contact_form_definition = sirius_contact_definition();
	$contact_form_paths = sirius_jsonfield_util_form_list_paths($contact_form_definition);
	$survey_form_paths = sirius_jsonfield_util_form_list_paths($survey_form_definition);

	$sql_args = array();
	$sql_args[':event_nid'] = $event_node->nid;
	$sql = "select node.nid as participant_nid, ";
	$sql .= "node.title as participant_title, ";
	$sql .= "field_data_field_sirius_json.field_sirius_json_value as participant_json, ";
	$sql .= "contact_node.nid as contact_nid, ";
	$sql .= "contact_node.title as contact_title, ";
	$sql .= "contact_json.field_sirius_json_value as contact_json ";
	$sql .= "from node ";
	$sql .= "left join field_data_field_sirius_event on field_data_field_sirius_event.entity_type = 'node' and field_data_field_sirius_event.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_json on field_data_field_sirius_json.entity_type = 'node' and field_data_field_sirius_json.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_contact on field_data_field_sirius_contact.entity_type = 'node' and field_data_field_sirius_contact.entity_id = node.nid ";
	$sql .= "left join node contact_node on field_sirius_contact_target_id = contact_node.nid ";
	$sql .= "left join field_data_field_sirius_json contact_json on contact_json.entity_type = 'node' and contact_json.entity_id = contact_node.nid ";
	$sql .= "where node.type = 'sirius_event_participant' ";
	$sql .= "and field_sirius_event_target_id = :event_nid ";

	$header = array('participant_nid', 'contact_nid', 'contact_name') + $contact_form_paths + $survey_form_paths;
	$fp = fopen('php://output', 'w');
	$filename = 'reports.csv';
	drupal_add_http_header('Content-Type', 'text/csv');
	drupal_add_http_header('Content-Disposition', 'attachment;filename=' . $filename);
	fputcsv($fp, $header);

	$stmt = db_query($sql, $sql_args);
	$rows = array();
	while ($hr = $stmt->fetchAssoc()) {
		$row = array();
		$row[] = $hr['participant_nid'];
		$row[] = $hr['contact_nid'];
		$row[] = $hr['contact_title'];

		$contact_json = json_decode($hr['contact_json'], TRUE);
		$participant_json = json_decode($hr['participant_json'], TRUE);
		foreach ($contact_form_paths as $json_path) {
			$row[] = sirius_jsonfield_util_get_from_path($contact_json, $json_path);
		}
		foreach ($survey_form_paths as $json_path) {
			$row[] = sirius_jsonfield_util_get_from_path($participant_json['survey'], $json_path);
		}
		
		fputcsv($fp, $row);
	}
	fclose($fp);
	exit();
}
