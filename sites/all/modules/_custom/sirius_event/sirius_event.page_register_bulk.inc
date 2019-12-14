<?php

function sirius_node_tab_event_participants_register_bulk($event_node) {
	$participant_role_tid = $_REQUEST["participant_role_tid"];
	if (!$participant_role_tid) {
		$html = sirius_node_tab_event_participants_quicknav($event_node, 'register/bulk');
		$html .= "To register new participants for this event, please select a role from the list above.";
		return $html;
	} 
	return drupal_get_form('sirius_node_tab_event_participants_register_bulk_form', $event_node, $participant_role_tid);
}

function sirius_contact_search_addone($form, $form_state) {
  $commands = array();
  $commands[] = ajax_command_replace("#contact_search", render($form['contact_search']));
	$commands[] = ajax_command_invoke(
		'#sirius_accordion', 
		'accordion', 
		array(
			array('active' => FALSE),
			array('collapsible' => TRUE),
			array('autoheight' => TRUE),
			array('heightStyle' => 'content')
		)
	);
  return array(
    '#type' => 'ajax',
    '#commands' => $commands
  );
}

function sirius_node_tab_event_participants_register_bulk_form($form, &$form_state, $event_node, $participant_role_tid) {
	$form_state['event_nid'] = $event_node->nid;
	$form_state['participant_role_tid'] = $participant_role_tid;
	$form['nav'] = array('#markup' => sirius_node_tab_event_participants_quicknav($event_node, 'register/bulk'));
	$participant_role_term = taxonomy_term_load($participant_role_tid);
	$statuses = sirius_event_participant_role_status_tids($participant_role_tid);

  drupal_add_library('system', 'ui.accordion');
  drupal_add_js('jQuery(document).ready(function () { jQuery("#sirius_accordion").accordion({active: false, collapsible: true, autoehight: true, heightStyle: "content"}); });', 'inline');

  $form['#tree'] = TRUE;

	$form['prefix'] = array('#markup' => '<div class="sirius-form">');

  //
  // I am a super search! Hear me roar.
  //

  $form['contact_search'] = array(
    '#type' => 'fieldset',
    '#title' => t('Contact Search'),
    '#prefix' => '<div id="contact_search">',
    '#suffix' => '</div>',
  );

	$criteria_count = $form_state['contact_search']['criteria_count'];
	if (!$criteria_count) { $criteria_count = 1; }
  if ($form_state['clicked_button']['#value'] == 'Add Criterion') { ++$criteria_count; }
  $form_state['contact_search']['criteria_count'] = $criteria_count;

  /*
  $form['contact_search']['accordion_open'] = array('#markup' => '<div id="sirius_accordion">');

  for ($i=0; $i<$criteria_count; ++$i) {
  	$title = "Criteria #" . ($i+1);
    $form['contact_search']["criterion_$i"]['accordion_open'] = array('#markup' => '<div class="sirius_accordion_label">' . $title . '</div><div class="sirius_accordion_body">');

    $form['contact_search']["criterion_$i"]['which'] = array(
    	'#type' => 'select',
    	'#options' => array(
    		'include' => t('INCLUDE contacts matching these criteria'),
    		'exclude' => t('EXCLUDE contacts matching these criteria'),
    	),
    );

    $form['contact_search']["criterion_$i"]['form'] = sirius_contact_definition();

    $form['contact_search']["criterion_$i"]['accordion_close'] = array('#markup' => '</div>');
  }
  $form['contact_search']['accordion_close'] = array('#markup' => '</div>');
  */

	$contact_form_definition = sirius_contact_definition();
	$contact_form_paths = array(NULL => t('-- Select --')) + sirius_jsonfield_util_form_list_paths($contact_form_definition);
	$form['contact_search']['add_criterion'] = array(
		'#type' => 'select',
		'#title' => 'Add a Contact Criterion: ',
		'#options' => $contact_form_paths,
	);

  $form['contact_search']['addone'] = array(
    '#type' => 'button',
    '#value' => t('Add Criterion'),
    '#ajax' => array(
      'callback' => 'sirius_contact_search_addone',
    ),
    '#limit_validation_errors' => array(),
  );

	$form['action'] = array(
		'#title' => 'Action',
		'#type' => 'select',
		'#options' => array(
			'search' => t('Search'),
			'save' => t('Save Search'),
			'register' => t('Register')
		),
	);

	$form['participant_status_tid'] = array(
		'#title' => 'Status',
		'#type' => 'select',
		'#options' => $statuses,
		'#states' => array(
			'visible' => array(
				array(':input[name="action"]' => array('value' => 'register')),
			)
		),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	$form['suffix'] = array('#markup' => '</div>');

	if ($form_state['results']) { $form['results'] = array('#markup' => $form_state['results']); }
	return $form;
}

function sirius_node_tab_event_participants_register_bulk_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
	$event_nid = $form_state['event_nid'];
	$event_node = node_load($event_nid);
	$domain_nid = sirius_domain_nid();
	$args = array();

	$sql_detail_start = "select nid, title, field_sirius_json_value ";
	$sql_count_start = "select count(nid) as c "; 
	$sql = '';
	$sql .= "from node ";
	if ($domain_nid) {
		$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
	}
	$sql .= "left join field_data_field_sirius_json on field_data_field_sirius_json.entity_type = 'node' and field_data_field_sirius_json.entity_id = node.nid ";
	$sql .= "where type = 'sirius_contact' ";
	if ($domain_nid) {
		$sql .= "and field_sirius_domain_target_id = :domain_nid ";
		$args[':domain_nid'] = $domain_nid;
	}
	$criteria_count = $form_state['contact_search']['criteria_count'];

	$headers = array();
	$first_clause = TRUE;
	for ($i=0; $i<$criteria_count; ++$i) {
		$values = $form_state['values']['contact_search']["criterion_$i"];
		$clause = rename_this_function_get_clause($values['form'], $headers);
		if ($clause) {
			if ($first_clause) {
				$first_clause = FALSE;
				if ($values['which'] == 'include') { $clauses .= "($clause) "; }
				if ($values['which'] == 'exclude') { $clauses .= "not ($clause) "; }
			} else {
				if ($values['which'] == 'include') { $clauses .= "or ($clause) "; }
				if ($values['which'] == 'exclude') { $clauses .= "and not ($clause) "; }
			}
		}
	}
	if (!$first_clause) {
		$sql .= "and ($clauses) ";
	}

	$sql_detail_end = "limit 100 ";

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
		$stmt = db_query($sql_count, $args);
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

function rename_this_function_get_clause($values, &$headers, $json_path_for_query = '$', $json_path_for_header = '') {
	$clause = '';
	foreach ($values as $key => $value) {
		$subclause = '';
		$sub_json_path_for_query = $json_path_for_query . ".$key";
		$sub_json_path_for_header = $json_path_for_header . ":$key";
		$sub_json_path_for_header = preg_replace('/^:/', '', $sub_json_path_for_header);

		if (is_array($value)) {
			$subclause .= rename_this_function_get_clause($value, $headers, $sub_json_path_for_query, $sub_json_path_for_header);
		} else {
			if ($value) {
				$subclause = "json_contains(field_sirius_json_value, '\"$value\"', '$sub_json_path_for_query') ";
				$headers[$sub_json_path_for_header] = TRUE;
			}
		}
		if ($subclause) {
			if ($clause) { $clause .= 'and '; }
			$clause .= $subclause;
		}
	}
	return $clause;
}
