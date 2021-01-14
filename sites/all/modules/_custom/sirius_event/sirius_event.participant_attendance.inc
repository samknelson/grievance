<?php

function sirius_event_page_participant_attendance($event_node, $rule) {
	sirius_esignature_init();
	drupal_add_css(drupal_get_path('module', 'sirius_event') . '/css/attendance.css', 'file');

	$dates = sirius_fieldvals($event_node, 'field_sirius_daterepeat', 'date');
	$which = $_REQUEST['which'];
	if (!$which) { $which = 0; }
	$html .= sirius_event_page_participant_attendance_quicknav($event_node);

	// Get the participants
	$participant_nids = sirius_event_participant_list_by_rule($event_node->nid, $rule);

	list ($action, $participant_nid, $action_which) = explode('|', $_REQUEST['action']);
	if ($action == 'clear') { 
		if (!in_array($participant_nid, $participant_nids) || !sirius_event_page_participant_attendance_can_edit($event_node, $action_which)) { return 'Access denied.'; }
		$html .= drupal_render(drupal_get_form('sirius_event_page_participant_attendance_signature_clear_form', $event_node, $participant_nid, $action_which));
	} else if ($action == 'sign') {
		if (!in_array($participant_nid, $participant_nids) || !sirius_event_page_participant_attendance_can_edit($event_node, $action_which)) { return 'Access denied.'; }
		$html .= drupal_render(drupal_get_form('sirius_event_page_participant_attendance_signature_form', $event_node, $participant_nid, $action_which));
	} else if ($which >= 0) {
		$html .= drupal_render(drupal_get_form('sirius_event_page_participant_attendance_form', $event_node, $participant_nids, $which, sirius_event_page_participant_attendance_can_edit($event_node, $which)));
	} else {
		for ($i = 0; $i<count($dates); ++$i) {
			if ($i > 0) { $html .= '<hr>'; }
			$html .= '<h2>';
			if (sirius_event_page_participant_attendance_can_edit($event_node, $i)) {
				$my_query = array();
				$my_query['delta'] = $_REQUEST['delta'];
				$my_query['widget'] = $_REQUEST['widget'];
				$my_query['prole'] = $_REQUEST['prole'];
				$my_query['which'] = $i;
				$html .= l('Edit', current_path(), array('attributes' => array('class' => array('tool-button')), 'query' => $my_query));
			}
			$html .= $dates[$i];
			$html .= '</h2>';
			$html .= drupal_render(drupal_get_form('sirius_event_page_participant_attendance_form', $event_node, $participant_nids, $i, FALSE));
		}
	}

	return $html;
}

function sirius_event_page_participant_attendance_can_edit($event_node, $which) {
	// Admin can edit anything
	if (user_access('sirius training admin')) { return TRUE; }

	// Non-admin can't edit "All"
	if ($which == -1) { return FALSE; }

	// Non-admins can edit one hour in advance and 24 hours after.
	$all_ts = sirius_fieldvals($event_node, 'field_sirius_daterepeat', 'ts');
	$class_ts = $all_ts[$which];
	if ( ($class_ts-(60*60) <= time()) && ($class_ts+(60*60*24) >= time()) )  { return TRUE; }

	return FALSE;
}

///////////////////////////////////////////////////////////////////
// Main Form
///////////////////////////////////////////////////////////////////


function sirius_event_page_participant_attendance_form($form, &$form_state, $event_node, $participant_nids, $which, $can_edit) {
	$form_state['event_nid'] = $event_node->nid;
	$form_state['which'] = $which;

	$my_query = array();
	$my_query['delta'] = $_REQUEST['delta'];
	$my_query['widget'] = $_REQUEST['widget'];
	$my_query['prole'] = $_REQUEST['prole'];
	$my_query['which'] = $which;

	$dates = sirius_fieldvals($event_node, 'field_sirius_daterepeat', 'date');
	$form['#tree'] = TRUE;

	if (!$can_edit) { $form['#disabled'] = TRUE; }

	$participant_nodes = node_load_multiple($participant_nids);

	$form['table_open'] = array('#markup' => '<table class="sirius_event_attendance">');
	$form['table_header'] = array('#markup' => '<tr><th>Name</th><th>Hours Completed</th><th>Comments</th><th>Signature</th><th>Tools</th></tr>');

	$header = array('Name', 'Badge', 'Signature', 'Tools');
	$rows = array();
	$duration_options = sirius_event_duration_options();
	$duration_default = strtotime($event_node->field_sirius_daterepeat['und'][$which]['value2']) - strtotime($event_node->field_sirius_daterepeat['und'][$which]['value']);
	foreach ($participant_nodes as $participant_nid => $participant_node) {

		$contact_node = node_load($participant_node->field_sirius_contact['und'][0]['target_id']);
		$worker_node = sirius_worker_get_by_contact_nid($participant_node->field_sirius_contact['und'][0]['target_id']);
		if (!$worker_node) { continue; }

		$participant_json = sirius_jsonfield_util_node_json_get($participant_node);
		$signature = sirius_jsonfield_util_esignature_get($participant_json, "attendance_$which");

		$tools = '';
		if ($can_edit) {
			if ($signature) {
				$my_query['action'] = "clear|$participant_nid|$which";
				$tools = l('Clear', current_path(), array('attributes' => array('class' => array('tool-button')), 'query' => $my_query));
			} else {
				$my_query['action'] = "sign|$participant_nid|$which";
				$tools = l('Sign', current_path(), array('attributes' => array('class' => array('tool-button')), 'query' => $my_query));
			}
		} 
		$path = 'node/' . $worker_node->nid . '/sirius_worker_tc/classes';
		if (drupal_valid_path($path)) {
			$tools .= l('Worker', $path, array('attributes' => array('class' => array('tool-button'))));
		}

		$form['participants'][$participant_nid]['tr_open'] = array('#markup' => '<tr>');

		$form['participants'][$participant_nid]['title'] = array('#markup' => '<td>' . $worker_node->title . '<br />' . $worker_node->field_sirius_id['und'][0]['value'] . '</td>');;

		if ($can_edit) {
			$my_duration_default = $participant_json["attendance_$which"]['data']['hours'];
			if (!isset($my_duration_default)) { $my_duration_default = $duration_default; }
			$form['participants'][$participant_nid]['hours'] = array(
				'#type' => 'select',
				'#options' => $duration_options,
				'#prefix' => '<td>',
				'#suffix' => '</td>',
				'#default_value' => $my_duration_default,
			);

			$form['participants'][$participant_nid]['comments'] = array(
				'#type' => 'textarea',
				'#prefix' => '<td>',
				'#suffix' => '</td>',
				'#default_value' => $participant_json["attendance_$which"]['data']['comments'],
			);
		} else {
			if (isset($participant_json["attendance_$which"]['data']['hours'])) { $duration_render = sirius_event_duration_format_hours($participant_json["attendance_$which"]['data']['hours']); }
			else { $duration_render = sirius_event_duration_format_hours($duration_default) . ' (est.)'; }
			$comments_render = sirius_text_to_html($participant_json["attendance_$which"]['data']['comments']);

			$form['participants'][$participant_nid]['hours'] = array('#markup' => '<td>' . $duration_render . '</td>');
			$form['participants'][$participant_nid]['comments'] = array('#markup' => '<td>' . $comments_render . '</td>');
		}

		$form['participants'][$participant_nid]['signature'] = array('#markup' => '<td>' . sirius_esignature_render($signature, '', $participant_nid, "attendance_$which") . '</td>');
		$form['participants'][$participant_nid]['tools'] = array('#markup' => '<td>' . $tools . '</td>');

		$form['participants'][$participant_nid]['tr_close'] = array('#markup' => '</tr>');

		if ($participant_json['comments']) {
			$form['participants'][$participant_nid]['participant_comments'] = array('#markup' => '<tr><td colspan="6">' . 'Participant notes for  ' . $worker_node->title . ': <strong>' . sirius_text_to_html($participant_json['comments']) . '</strong></td></tr>');
		}
	}

	$form['table_close'] =  array('#markup' => '</table>');

	if ($can_edit) {
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit'),
		);
	}

	return $form;
}

function sirius_event_page_participant_attendance_form_submit($form, &$form_state) {
	if ($form_state['values']['participants']) {
		$which = $form_state['which'];
		foreach ($form_state['values']['participants'] as $participant_nid => $data) {
			$participant_node = node_load($participant_nid);
			$participant_json = sirius_jsonfield_util_node_json_get($participant_node);
			$is_changed = FALSE;
			foreach ($data as $key => $value) {
				if ($participant_json["attendance_$which"]['data'][$key] != $value) {
					$participant_json["attendance_$which"]['data'][$key] = $value;
					$is_changed = TRUE;
				}
			}
			if ($is_changed) {
				sirius_jsonfield_util_node_json_set($participant_node, $participant_json);
				node_save($participant_node);
			}
		}
	}
}

///////////////////////////////////////////////////////////////////
// Signature forms
///////////////////////////////////////////////////////////////////

function sirius_event_page_participant_attendance_signature_form($form, &$form_state, $event_node, $participant_nid, $which) {
	$form_state['participant_nid'] = $participant_nid;
	$form_state['which'] = $which;

	$dates = sirius_fieldvals($event_node, 'field_sirius_daterepeat', 'date');
	$participant_node = node_load($participant_nid);
	$contact_node = node_load($participant_node->field_sirius_contact['und'][0]['target_id']);
	$statement = 'By signing below, I, <strong>' . $contact_node->title . '</strong>, affirm that I was present at <strong>' . $event_node->title . '</strong> on <strong>' . $dates[$which] . '</strong>.';

	sirius_esignature_form_prepare($form, "attendance_$which", "Signature", $statement);

	return $form;
}

function sirius_event_page_participant_attendance_signature_form_submit($form, &$form_state) {
	$which = $form_state['which'];
	$participant_nid = $form_state['participant_nid'];

	$participant_node = node_load($participant_nid);
	$json = sirius_jsonfield_util_node_json_get($participant_node);
	$signature = sirius_esignature_form_get_json($form, $form_state, "attendance_$which");
	sirius_jsonfield_util_esignature_set($json, "attendance_$which", $signature);
	sirius_jsonfield_util_node_json_set($participant_node, $json);
	node_save($participant_node);

	$my_query = array();
	$my_query['delta'] = $_REQUEST['delta'];
	$my_query['widget'] = $_REQUEST['widget'];
	$my_query['prole'] = $_REQUEST['prole'];
	$my_query['which'] = $_REQUEST['which'];
	drupal_goto(current_path(), array('query' => $my_query));
}

function sirius_event_page_participant_attendance_signature_clear_form($form, &$form_state, $event_node, $participant_nid, $which) {
	$form_state['participant_nid'] = $participant_nid;
	$form_state['which'] = $which;

	$dates = sirius_fieldvals($event_node, 'field_sirius_daterepeat', 'date');
	$participant_node = node_load($participant_nid);
	$contact_node = node_load($participant_node->field_sirius_contact['und'][0]['target_id']);
	$json = sirius_jsonfield_util_node_json_get($participant_node);
	$signature = sirius_esignature_form_get_json($form, $form_state, "attendance_$which");
	$form['intro'] = array('#markup' => sirius_esignature_render($signature, 'full', $participant_nid, "attendance_$which") . '<hr>');

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Clear This Signature'),
	);
	return $form;
}

function sirius_event_page_participant_attendance_signature_clear_form_submit($form, &$form_state) {
	$which = $form_state['which'];
	$participant_nid = $form_state['participant_nid'];

	$participant_node = node_load($participant_nid);
	$json = sirius_jsonfield_util_node_json_get($participant_node);
	sirius_jsonfield_util_esignature_set($json, "attendance_$which", NULL);
	sirius_jsonfield_util_node_json_set($participant_node, $json);
	node_save($participant_node);

	$my_query = array();
	$my_query['delta'] = $_REQUEST['delta'];
	$my_query['widget'] = $_REQUEST['widget'];
	$my_query['prole'] = $_REQUEST['prole'];
	$my_query['which'] = $_REQUEST['which'];
	drupal_goto(current_path(), array('query' => $my_query));
}

///////////////////////////////////////////////////////////////////
// Utility Functions
///////////////////////////////////////////////////////////////////

function sirius_event_page_participant_attendance_quicknav($event_node) {
	$dates = sirius_fieldvals($event_node, 'field_sirius_daterepeat', 'date');
	$my_query = array();
	$my_query['delta'] = $_REQUEST['delta'];
	$my_query['widget'] = $_REQUEST['widget'];
	$my_query['prole'] = $_REQUEST['prole'];

	$links = array();
	foreach ($dates as $delta => $date) {
		$link = array();
		$link['path'] = current_path();
		$link['query'] = $my_query;
		$link['query']['which'] = $delta;
		$link['title'] = $date;
		$links[] = $link;
	}

	if (count($dates) > 1) {
		$link = array();
		$link['path'] = current_path();
		$link['query'] = $my_query;
		$link['query']['which'] = '-1';
		$link['title'] = 'All';
		$links[] = $link;
	}

	return sirius_quicknav($links);
}

