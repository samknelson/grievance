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
  sirius_event_disclaimer_redirect($event_node);
	$event_type_term = taxonomy_term_load($event_node->field_sirius_event_type['und'][0]['tid']);
	$json = json_decode($event_type_term->field_sirius_json['und'][0]['value'], TRUE);
	$all_rules = $json['participants']['rules'];
	if (user_access('sirius event admin')) {
		$rules = $all_rules;
	} else {
		$rules = sirius_event_paccess_rule_match($event_node->nid, $all_rules, TRUE);
	}

	$delta = $_REQUEST['delta'];
	if (!isset($delta)) {
		$widgets = sirius_event_participants_widgets();
		$widgets['view'] = $widgets['status'] = $widgets['contact'] = 'Roster';

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
				$is_first = TRUE;
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

	$all_widgets = sirius_event_participants_widgets();
	$all_widgets['view'] = $all_widgets['status'] = $all_widgets['contact'] = 'Roster';

	drupal_set_title(drupal_get_title() . ': ' . $rule['name'] . ' - ' . $all_widgets[$widget]);
	if ($widget == 'view' || $widget == 'status' || $widget == 'contact' || $widget == 'list') {
		return sirius_event_page_participant_list($event_node, $prole, $rule);
		//return views_embed_view('sirius_event_participants', 'block_1', $event_node->nid, $prole);
	/*} else if ($widget == 'status') {
		return views_embed_view('sirius_event_participants', 'block_1', $event_node->nid, $prole);
	} else if ($widget == 'contact') {
		return views_embed_view('sirius_event_participants', 'block', $event_node->nid, $prole); */
	} else if ($widget == 'reports') {
		return sirius_node_tab_event_participants_reports($event_node->nid, $prole);
	} else if ($widget == 'summary') { 
		return sirius_node_tab_event_participants_summary($event_node->nid, $prole);
	} else if ($widget == 'attendance') {
		return sirius_event_page_participant_attendance($event_node, $rule);
	} else if ($widget == 'certifications') {
		return sirius_event_page_participant_certifications($event_node, $rule);
	} else if ($widget == 'mightysearch_lite') {
		return sirius_event_page_participant_mightysearch($event_node, $rule, TRUE);
	} else if ($widget == 'mightysearch') {
		return sirius_event_page_participant_mightysearch($event_node, $rule, FALSE);
	} else {
		return t('Widget not found: @widget.', array('@widget' => $widget));
	}
}

function sirius_event_participant_role_status_tids($participant_role_tid, $options = array()) {
	if (!$participant_role_tid) { return array(); }
	$sql_args = array();
	$sql = "select tid, name from taxonomy_term_data ";
	$sql .= "left join field_data_field_sirius_event_proles on field_data_field_sirius_event_proles.entity_type = 'taxonomy_term' and field_data_field_sirius_event_proles.bundle = 'sirius_event_participant_status' and field_data_field_sirius_event_proles.entity_id = tid ";
	if ($options['phonebank']['during_call']) {
		$sql .= "left join field_data_field_sirius_json on field_data_field_sirius_json.entity_type = 'taxonomy_term' and field_data_field_sirius_json.entity_id = tid ";
	}
	if (is_array($participant_role_tid)) {
		$sql .= "where field_sirius_event_proles_tid in (:participant_role_tid) ";
		$sql_args[':participant_role_tid'] = $participant_role_tid;
	} else {
		$sql .= "where field_sirius_event_proles_tid = :participant_role_tid ";
		$sql_args[':participant_role_tid'] = $participant_role_tid;
	}
	if ($options['phonebank']['during_call']) {
		$sql .= "and ("; 
		$sql .= "  field_sirius_json_value IS NULL ";
		$sql .= "  or lower(json_unquote(json_extract(field_sirius_json_value, '\$.phonebank.conceal_during_call'))) != 'yes' ";
		$sql .= "  or lower(json_unquote(json_extract(field_sirius_json_value, '\$.phonebank.conceal_during_call'))) is null ";
		if ($options['phonebank']['current_pstatus']) {
			$sql .= "   or tid = :current_pstatus ";
			$sql_args[':current_pstatus'] = $options['phonebank']['current_pstatus'];
		}
		$sql .= ") ";
	}
	$sql .= "order by weight, name ";
	$stmt = sirius_sql_query($sql, $sql_args);
	$statuses = array();
	while ($hr = $stmt->fetchAssoc()) {
		$statuses[$hr['tid']] = $hr['name'];
	}

	return $statuses;
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

function sirius_node_tab_event_participants_reports($event_nid, $prole_tid) {
	return drupal_get_form('sirius_node_tab_event_participants_reports_form', $event_nid, $prole_tid);
}

function sirius_node_tab_event_participants_reports_form($form, &$form_state, $event_nid, $prole_tid = NULL) {
	$form_state['event_nid'] = $event_nid;
	if ($prole_tid) { $form_state['prole_tid'] = $prole_tid; }
	$event_node = node_load($event_nid);

	$form['intro'] = array('#markup' => '<div class="sirius_form_intro">' . t('Click below to download the list of all participants, including all custom fields and all survey responses.') . '</div>');

	$form['output'] = array(
		'#type' => 'select',
		'#options' => array(
			'screen' => t('Screen'),
			'csv' => t('Download'),
		),
		'#description' => t('Screen output will be limited to 1000 records.'),
		'#required' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	if ($form_state['results']) {
		$form['results'] = $form_state['results'];
	}

	return $form;
}

function sirius_node_tab_event_participants_reports_form_submit($form, &$form_state) {
	$output = $form_state['values']['output'];
	$prole_tid = $form_state['prole_tid'];

	$prole_names = sirius_taxonomy_load('sirius_event_participant_role', 'tid', 'name');
	$pstatus_names = sirius_taxonomy_load('sirius_event_participant_status', 'tid', 'name');

	$event_node = node_load($form_state['event_nid']);
	$event_type_term = taxonomy_term_load($event_node->field_sirius_event_type['und'][0]['tid']);
	$is_phonebank = sirius_event_phonebank_enabled_type($event_type_term);

	$participant_definition_node = sirius_event_survey_get($event_node);
	if (!$participant_definition_node) { drupal_set_message("No survey!"); return array(); }
	$participant_definition_json = sirius_jsonfield_util_node_json_get($participant_definition_node);
	$participant_definition_form = sirius_jsonfield_util_form_get($participant_definition_json, 'default');
  $participant_paths = array_values(sirius_jsonfield_util_form_list_paths($participant_definition_form, TRUE));

	$contact_definition_json = sirius_contact_definition();
	$contact_definition_form = sirius_jsonfield_util_form_get($contact_definition_json, 'default');
  $contact_paths = array_values(sirius_jsonfield_util_form_list_paths($contact_definition_form, TRUE));

	$header = array('Participant ID', 'Contact ID', 'Contact Name', 'Role', 'Status', 'Phone');
	if ($is_phonebank) {
		$header[] = "Caller UID";
		$header[] = "Caller Name";
		$header[] = "Caller Mail";
		$header[] = "First Call";
		$header[] = "Last Call";
	}

	foreach ($contact_paths as $path) { $header[] = $path; }
	foreach ($participant_paths as $path) { $header[] = $path; }

	$sql_args = array();
	$sql_args[':event_nid'] = $event_node->nid;
	$sql = "select node.nid as participant_nid, ";
	$sql .= "node.title as participant_title, ";
	$sql .= "field_data_field_sirius_json.field_sirius_json_value as participant_json, ";
	$sql .= "contact_node.nid as contact_nid, ";
	$sql .= "contact_node.title as contact_title, ";
	$sql .= "contact_json.field_sirius_json_value as contact_json, ";
	$sql .= "field_sirius_event_prole_tid as prole_tid, ";
	$sql .= "field_sirius_event_pstatus_tid as pstatus_tid, ";
	$sql .= "field_sirius_phone_value as phone ";
	$sql .= "from node ";
	$sql .= "left join field_data_field_sirius_event_prole on field_data_field_sirius_event_prole.entity_type = 'node' and field_data_field_sirius_event_prole.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_event_pstatus on field_data_field_sirius_event_pstatus.entity_type = 'node' and field_data_field_sirius_event_pstatus.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_event on field_data_field_sirius_event.entity_type = 'node' and field_data_field_sirius_event.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_json on field_data_field_sirius_json.entity_type = 'node' and field_data_field_sirius_json.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_contact on field_data_field_sirius_contact.entity_type = 'node' and field_data_field_sirius_contact.entity_id = node.nid ";
	$sql .= "left join node contact_node on field_sirius_contact_target_id = contact_node.nid ";
	$sql .= "left join field_data_field_sirius_json contact_json on contact_json.entity_type = 'node' and contact_json.entity_id = contact_node.nid ";
	$sql .= "left join field_data_field_sirius_phone contact_phone on contact_phone.entity_type = 'node' and contact_phone.entity_id = contact_node.nid ";
	$sql .= "where node.type = 'sirius_event_participant' ";
	$sql .= "and field_sirius_event_target_id = :event_nid ";
	if ($prole_tid) {
		$sql .= "and field_sirius_event_prole_tid = :prole_tid ";
		$sql_args[':prole_tid'] = $prole_tid;
	}

	$rows = array();
	if ($output == 'csv') {
		$fp = fopen('php://output', 'w');
		$filename = 'reports.csv';
		drupal_add_http_header('Content-Type', 'text/csv');
		drupal_add_http_header('Content-Disposition', 'attachment;filename=' . $filename);
		fputcsv($fp, $header);
	}

	$stmt = db_query($sql, $sql_args);
	$count = 0;
	while ($hr = $stmt->fetchAssoc()) {
		$contact_json = json_decode($hr['contact_json'], TRUE);
		$contact_results_json = sirius_jsonfield_util_results_get($contact_json, 'default');

		$participant_json = json_decode($hr['participant_json'], TRUE);
		$participant_results_json = sirius_jsonfield_util_results_get($participant_json, 'survey');

		$row = array();
		$row[] = $hr['participant_nid'];
		$row[] = $hr['contact_nid'];
		$row[] = $hr['contact_title'];
		$row[] = $prole_names[$hr['prole_tid']];
		$row[] = $pstatus_names[$hr['pstatus_tid']];
		$row[] = $hr['phone'];

		if ($is_phonebank) {
			$row[] = $participant_json['phonebank']['caller']['uid'];
			$row[] = $participant_json['phonebank']['caller']['name'];
			$row[] = $participant_json['phonebank']['caller']['mail'];
			$row[] = $participant_json['phonebank']['first_call']['date'];
			$row[] = $participant_json['phonebank']['last_call']['date'];
		}

		foreach ($contact_paths as $path) { 
			$row[] = sirius_jsonfield_util_render_value(
				sirius_jsonfield_util_get_from_path($contact_definition_form, $path),
				sirius_jsonfield_util_get_from_path($contact_results_json, $path), 
				array('raw' => TRUE)
			);
		}
		foreach ($participant_paths as $path) { 
			$row[] = sirius_jsonfield_util_render_value(
				sirius_jsonfield_util_get_from_path($participant_definition_form, $path),
				sirius_jsonfield_util_get_from_path($participant_results_json, $path), 
				array('raw' => TRUE)
			);
		}

		if ($output == 'csv') { 
			fputcsv($fp, $row);
		} else {
			$rows[] = $row;
		}
		++$count;
		if ($count > 1000 && $output == 'screen') { break; }
	}

	if ($output == 'csv') {
		fclose($fp);
		exit();
	} else {
		$form_state['results'] = array('#markup' => '<hr>' . '<h2>' . t('Results') . '</h2>' . theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array())));
		$form_state['rebuild'] = TRUE;
	}
}

function sirius_node_tab_event_participants_summary($event_nid, $prole_tid = NULL) {
	static $pstatus_names;
	if (!$pstatus_names) { $pstatus_names = sirius_taxonomy_load('sirius_event_participant_status', 'tid', 'name'); }

	$summary = sirius_event_participant_summary($event_nid, $prole_tid);

	$rows = array();
	foreach ($pstatus_names as $pstatus_tid => $pstatus_name) {
		$row = array();
		$row[] = $pstatus_name;
		$row[] = number_format($summary[$pstatus_tid], 0);
		$rows[] = $row;
	}

	$header = array('Status', 'Count');
	return theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => array()));
}
