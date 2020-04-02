<?php

function sirius_event_page_participant_attendance($event_node, $rule) {
	sirius_esignature_init();

	$my_query = array();
	$my_query['delta'] = $_REQUEST['delta'];
	$my_query['widget'] = $_REQUEST['widget'];
	$my_query['prole'] = $_REQUEST['prole'];

	$dates = sirius_fieldvals($event_node, 'field_sirius_daterepeat', 'date');
	$links = array();
	foreach ($dates as $delta => $date) {
		$link = array();
		$link['path'] = current_path();
		$link['query'] = $my_query;
		$link['query']['which_date'] = $delta;
		$link['title'] = $date;
		$links[] = $link;
	}
	$html = sirius_quicknav($links);

	$which_date = $_REQUEST['which_date'];
	if (!$which_date) { $which_date = 0; }
	$my_query['which_date'] = $which_date;
	$html .= '<h2>Sign-in Sheet For ' . $dates[$which_date] . '</h2>';

	$which = 'attendance_' . $which_date;

	// Get the participants
	$sql_args = array();
	$sql_args[':event_nid'] = $event_node->nid;
	$sql = "select node.nid as participant_nid ";
	$sql .= "from node ";
	$sql .= "left join field_data_field_sirius_event_prole on field_data_field_sirius_event_prole.entity_type = 'node' and field_data_field_sirius_event_prole.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_event_pstatus on field_data_field_sirius_event_pstatus.entity_type = 'node' and field_data_field_sirius_event_pstatus.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_event on field_data_field_sirius_event.entity_type = 'node' and field_data_field_sirius_event.entity_id = node.nid ";
	$sql .= "where node.type = 'sirius_event_participant' ";
	$sql .= "and field_sirius_event_target_id = :event_nid ";
	$sql .= "and ( ";
	$delta = 0;
	foreach ($rule['participants'] as $tmp) {
		list ($prole_tid, $pstatus_tid) = explode('|', $tmp);
		if ($delta > 0) { $sql .= ' or '; }
		$sql .= " (field_sirius_event_prole_tid = :prole_tid_$delta and field_sirius_event_pstatus_tid = :pstatus_tid_$delta ) ";
		$sql_args[":prole_tid_$delta"] = $prole_tid;
		$sql_args[":pstatus_tid_$delta"] = $pstatus_tid;
		++$delta;
	}
	$sql .= ') ';

	$stmt = db_query($sql, $sql_args);
	$participant_nids = array();
	while ($hr = $stmt->fetchAssoc()) {
		$participant_nids[] = $hr['participant_nid'];
	}

	// If we're passed in an action, handle it
	$action = $_REQUEST['action'];
	$participant_nid = $_REQUEST['participant_nid'];
	if ($action && $participant_nid && in_array($participant_nid, $participant_nids)) {
		$participant_node = node_load($participant_nid);
		$contact_node = node_load($participant_node->field_sirius_contact['und'][0]['target_id']);
		if ($action == 'sign') {
			$statement = 'By signing below, I, <strong>' . $contact_node->title . '</strong>, affirm that I was present at <strong>' . $event_node->title . '</strong> on <strong>' . $date . '</strong>.';
			return drupal_get_form('sirius_event_page_participant_attendance_form', $participant_nid, $which, $statement);
		} else if ($action == 'clear') {
			$participant_json = sirius_jsonfield_util_node_json_get($participant_node);
			sirius_jsonfield_util_esignature_set($participant_json, $which, NULL);
			sirius_jsonfield_util_node_json_set($participant_node, $participant_json);
			node_save($participant_node);
		}
	}

	$participant_nodes = node_load_multiple($participant_nids);

	$header = array('Nane', 'Badge', 'Signature', 'Tools');
	$rows = array();
	foreach ($participant_nodes as $participant_nid => $participant_node) {
		$contact_node = node_load($participant_node->field_sirius_contact['und'][0]['target_id']);
		$worker_node = sirius_worker_get_by_contact_nid($participant_node->field_sirius_contact['und'][0]['target_id']);

		$participant_json = sirius_jsonfield_util_node_json_get($participant_node);
		$esignature = sirius_jsonfield_util_esignature_get($participant_json, $which);
		$row = array();
		$row[] = $worker_node->title;
		$row[] = $worker_node->field_sirius_id['und'][0]['value'];
		if ($esignature) { $row[] .= sirius_esignature_render($esignature, '', $participant_nid, $which); } else { $row[] = ''; }

		$my_query['participant_nid'] = $participant_nid;
		if ($esignature) {
			$my_query['action'] = 'clear';
			$row[] = l('Clear', current_path(), array('attributes' => array('class' => array('tool-button')), 'query' => $my_query));
		} else {
			$my_query['action'] = 'sign';
			$row[] = l('Sign Now', current_path(), array('attributes' => array('class' => array('tool-button')), 'query' => $my_query));
		}
		$rows[] = $row;
	}
	$html .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array()));

	return $html;
}

function sirius_event_page_participant_attendance_form($form, &$form_state, $nid, $which, $statement) {
	$form_state['nid'] = $nid;
	$form_state['which'] = $which;

	sirius_esignature_form_prepare($form, $which, "Signature", $statement);

	return $form;
}

function sirius_event_page_participant_attendance_form_submit($form, &$form_state) {
	$which = $form_state['which'];
	$nid = $form_state['nid'];

	$node = node_load($nid);
	$json = sirius_jsonfield_util_node_json_get($node);
	$signature = sirius_esignature_form_get_json($form, $form_state, $which);
	sirius_jsonfield_util_esignature_set($json, $which, $signature);
	sirius_jsonfield_util_node_json_set($node, $json);
	node_save($node);

	$my_query = array();
	$my_query['delta'] = $_REQUEST['delta'];
	$my_query['widget'] = $_REQUEST['widget'];
	$my_query['prole'] = $_REQUEST['prole'];
	$my_query['which_date'] = $_REQUEST['which_date'];
	drupal_goto(current_path(), array('query' => $my_query));
}

