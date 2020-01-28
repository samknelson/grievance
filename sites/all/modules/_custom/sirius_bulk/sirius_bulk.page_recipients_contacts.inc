<?php

function sirius_bulk_node_tab_recipients_contacts($bulk_node) {
	return drupal_get_form('sirius_bulk_node_tab_recipients_contacts_form', $bulk_node);
}

function sirius_bulk_node_tab_recipients_contacts_form($form, &$form_state, $bulk_node) {
	$form_state['bulk_nid'] = $bulk_node->nid;

	$contact_form_definition = sirius_contact_definition();
	$contact_form_paths = array();
	$contact_form_paths[NULL] = t('-- Select --');
	$contact_form_paths['node__sirius_name__given'] = ':node:name:given';
	$contact_form_paths['node__sirius_name__middle'] = ':node:name:middle';
	$contact_form_paths['node__sirius_name__family'] = ':node:name:family';
	$contact_form_paths['node__sirius_address__thoroughfare'] = ':node:address:address';
	$contact_form_paths['node__sirius_address__locality'] = ':node:address:city';
	$contact_form_paths['node__sirius_address__administrative_area'] = ':node:address:state_code';
	$contact_form_paths['node__sirius_address__postal_code'] = ':node:address:zip';
	$contact_form_paths['node__sirius_address__country'] = ':node:address:country';
	$contact_form_paths += sirius_jsonfield_util_form_list_paths($contact_form_definition);

	$clause_count = 5;

	$form['#tree'] = TRUE;

	for ($i=0; $i<$clause_count; ++$i) {
		$form['contact_search'][$i] = array(
			'#type' => 'fieldset',
		);

		$form['contact_search'][$i]['andor'] = array(
			'#type' => 'select',
			'#options' => array(
				'and' => 'AND',
				'or' => 'OR',
			),
			'#prefix' => '<div style="float: left; min-width: 80px;">',
			'#suffix' => '</div>',
		);
		if ($i == 0) { $form['contact_search'][$i]['andor']['#options'] = array(NULL => 'WHERE'); }

		$form['contact_search'][$i]['open'] = array(
			'#type' => 'select',
			'#options' => array(
				NULL => '',
				'(' => '(',
				'((' => '((',
				'(((' => '(((',
				'((((' => '((((',
			),
			'#prefix' => '<div style="float: left;">',
			'#suffix' => '</div>',
		);

		$form['contact_search'][$i]['field'] = array(
			'#type' => 'select',
			'#options' => $contact_form_paths,
			'#prefix' => '<div style="float: left;">',
			'#suffix' => '</div>',
		);

		$form['contact_search'][$i]['criterion'] = array(
			'#type' => 'select',
			'#options' => array(
				'=' => '=',
				'!=' => '!=',
				'>=' => '>=',
				'>' => '>',
				'<' => '<',
				'<=' => '<=',
				'contains' => 'conains',
				'does not contain' => 'does not contain',
				'starts' => 'starts with',
				'ends' => 'ends with',
			),
			'#prefix' => '<div style="float: left;">',
			'#suffix' => '</div>',
		);

		$form['contact_search'][$i]['value'] = array(
			'#type' => 'textfield',
			'#prefix' => '<div style="float: left;">',
			'#suffix' => '</div>',
		);

		$form['contact_search'][$i]['close'] = array(
			'#type' => 'select',
			'#options' => array(
				NULL => '',
				')' => ')',
				'))' => '))',
				')))' => ')))',
				'))))' => '))))',
			),
			'#prefix' => '<div style="float: left;">',
			'#suffix' => '</div>',
		);
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Search'),
	);

	$form['suffix'] = array('#markup' => '</div>');

	if ($form_state['results']) { $form['results'] = array('#markup' => $form_state['results']); }
	return $form;
}

function sirius_bulk_node_tab_recipients_contacts_form_submit($form, &$form_state) {
	$bulk_node = node_load($form_state['bulk_nid']);

	$contact_form_definition = sirius_contact_definition();
	$contact_form_paths = array();
	$contact_form_paths[NULL] = t('-- Select --');
	$contact_form_paths['node__sirius_name__given'] = ':node:name:given';
	$contact_form_paths['node__sirius_name__middle'] = ':node:name:middle';
	$contact_form_paths['node__sirius_name__family'] = ':node:name:family';
	$contact_form_paths['node__sirius_address__thoroughfare'] = ':node:address:address';
	$contact_form_paths['node__sirius_address__locality'] = ':node:address:city';
	$contact_form_paths['node__sirius_address__administrative_area'] = ':node:address:state_code';
	$contact_form_paths['node__sirius_address__postal_code'] = ':node:address:zip';
	$contact_form_paths['node__sirius_address__country'] = ':node:address:country';
	$contact_form_paths += sirius_jsonfield_util_form_list_paths($contact_form_definition);

	$sql_detail_start = "select nid, title, field_sirius_json_value ";
	$sql_count_start = "select count(nid) as c "; 
	$sql_args = array();
	$domain_nid = sirius_domain_nid();
	$sql = '';
	$sql .= "from node ";
	if ($domain_nid) {
		$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
	}
	$sql .= "left join field_data_field_sirius_json on field_data_field_sirius_json.entity_type = 'node' and field_data_field_sirius_json.entity_id = node.nid ";
	$sql .= "where type = 'sirius_contact' ";
	if ($domain_nid) {
		$sql .= "and field_sirius_domain_target_id = :domain_nid ";
		$sql_args[':domain_nid'] = $domain_nid;
	}

	$headers = array();
	$first_clause = TRUE;

	for ($i=0; $i<5; ++$i) {
		$values = $form_state['values']['contact_search'][$i];
		if (!$values['field']) { continue; }
		if ($i == 0) { $sql .= " and "; } else { $sql .= ' ' . $values['andor'] . ' '; }
		$sql .= $values['open'];

		$field = $values['field'];
		$path_for_query = $contact_form_paths[$field];
		$path_for_header = $json_path_for_header . ":$key";
		$path_for_header = preg_replace('/^:/', '', $path_for_header);
		$value = $values['value'];
		$sql .= "json_extract(field_sirius_json_value, '$path_for_query') = :value_$i";
		$sql_args["value_$i"] = $value;

		$sql .= $values['close'];
	}
	if (!$first_clause) {
		$sql .= "and ($clauses) ";
	}

	$sql_detail_end = "limit 100 ";

	$form_state['reubild'] = TRUE;
	drupal_set_message($sql);
	dpm($sql_args);
	$stmt = db_query($sql_detail_start . $sql, $sql_args);
	while ($hr = $stmt->fetchAssoc()) {
		dpm($hr);
	}
	return;

	$sql_count = $sql_count_start . $sql . $sql_count_end; 
	$sql_detail = $sql_detail_start . $sql . $sql_detail_end; 

	$stmt = db_query($sql_detail, $args);
	$results = array();
	$rows = array();
	$count = 0;
	while ($hr = $stmt->fetchAssoc()) {
		$results[] = $hr;
		$json = json_decode($hr['field_sirius_json_value'], TRUE);
		$row = array();
		$row[] = $hr['title'];
		foreach (array_keys($headers) as $json_path) {
			$row[] = sirius_jsonfield_util_get_from_path($json, $json_path);
		}
		$row[] = l('View', 'node/' . $hr['nid'], array('attributes' => array('target' => '_blank', 'class' => array('tool-button')))) . ' ' .
			l('Register', 'node/' . $hr['nid'], array('attributes' => array('target' => '_blank', 'class' => array('tool-button'))));
		$rows[] = $row;
		++$count;
	}

	$header = array(
		'Name',
	);
	foreach (array_keys($headers) as $json_path) {
		$header[] = $json_path;
	}
	$header[] = t('Tools');

	$html = '<hr>';
	$html .= '<h2>' . t('Results') . '</h2>';
	if ($count >= 100) {
		$stmt = db_query($sql_count, $sql_args);
		$hr = $stmt->fetchAssoc();
		$count_total = $hr['c'];

		$html .= t('@count_total matching contacts found, showing the first 100', array('@count_total' => $count_total));
	} else if ($count < 1) {
		$html .= t('No contacts match your criteria.');
	} else {
		$html .= t('@count contacts found.', array('@count' => $count));
	}
	if ($count) {
		$html .= theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => array()));
	}

	$form_state['results'] = $html;

	$action = $form_state['values']['action'];
	if ($action == 'save') {
		drupal_set_message("Save is not implemented", 'warning');
	}

	if ($action == 'register') {
		$participant_role_tid = $form_state['participant_role_tid'];
		$participant_status_tid = $form_state['values']['participant_status_tid'];
		$count_registered = $count_failed = 0;
		foreach ($results as $hr) {
			$contact_nid = $hr['nid'];
			$result = sirius_event_participant_register($event_nid, $contact_nid, $participant_role_tid, $participant_status_tid);
			if ($result['success']) { $count_registered++; } else { $count_failed++; }
		}
		drupal_set_message(t('Registration complete. Registered @count_registered with @count_failed failures.', array('@count_registered' => $count_registered, '@count_failed' => $count_failed)));
		if ($count_failed) { drupal_set_message(t('(Failures generally mean that the participant was already registered.)')); }
	}
}

