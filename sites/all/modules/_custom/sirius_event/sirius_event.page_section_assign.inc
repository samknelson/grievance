<?php

function sirius_node_tab_event_section_assign_access($event_node) {
	return sirius_node_tab_event_section_settings_access($event_node);
}

function sirius_node_tab_event_section_assign($event_node) {
  sirius_event_disclaimer_redirect($event_node);
 	return drupal_get_form('sirius_node_tab_event_section_assign_form', $event_node);
}

function sirius_node_tab_event_section_assign_form($form, &$form_state, $event_node) {
	drupal_add_css(drupal_get_path('module', 'sirius_event') . '/css/event.css', 'file');
	$form_state['event_nid'] = $event_node->nid;
	$event_json = sirius_jsonfield_util_node_json_get($event_node);

	$participants = sirius_event_section_participants($event_node);
	if (!$participants) {
		drupal_set_message("There are no participants assigned to this event who have a role which requires sections.", 'warning');
		return array();		
	}
	$counts = sirius_event_section_counts($event_node);
	$options = sirius_event_section_options($event_node, $counts);
	$limit = $event_json['section']['limit'];

	$form['#tree'] = TRUE;

	$form['participants']['table_open'] = array('#markup' => '<table><tr>' . 
		'<th>' . t('Name') . '</th>' .
		'<th>' . t('Role') . '</th>' . 
		'<th>' . t('Status') . '</th>' .
		'<th>' . t('Section Assignment') . '</th>' .
		'<th>' . t('Counts') . '</th>' .
		'<th>' . t('Tools') . '</th>' .
		'</tr>'
	);
	foreach ($participants as $participant_nid => $participant) {
		$row_class = ' ';
		if ($participant['participant_comments']) { $row_class .= "sirius_table_row_with_details "; }
		$form['participants'][$participant_nid]['row_open'] = array('#markup' => "<tr class=\"$row_class\">");
		$form['participants'][$participant_nid]['name'] = array('#markup' => '<td>' . $participant['contact_title'] . '</td>');
		$form['participants'][$participant_nid]['prole'] = array('#markup' => '<td>' . $participant['prole_name'] . '</td>');
		$form['participants'][$participant_nid]['pstatus'] = array('#markup' => '<td>' . $participant['pstatus_name'] . '</td>');

		$uuid = $participant['section_uuid'];
		$section_limit = $event_json['section_list'][$uuid]['limit'];
		if (!$section_limit) { $section_limit = $limit; }
		$form['participants'][$participant_nid]['uuid'] = array(
			'#type' => 'select',
			'#options' => $options,
			'#default_value' => $uuid,
			'#prefix' => '<td>',
			'#suffix' => '</td>',
		);

		$count_msg = '';
		if ($uuid) {
			$count_msg = "Assigned " . $counts[$uuid] . " / " . $section_limit;
			if ($counts[$uuid] > $section_limit) {
				$count_msg .= ' <strong>(oversubscribed)</strong>';
			}
		}
		$form['participants'][$participant_nid]['counts'] = array('#markup' => '<td>' . $count_msg . '</td>');

		$tools = '';
		if ($participant['worker_nid']) { $tools .= l('Worker', 'node/' . $participant['worker_nid'], array('attributes' => array('class' => array('tool-button')))); }
		$tools .= l('Participant', 'node/' . $participant_nid, array('attributes' => array('class' => array('tool-button'))));
		$tools .= l('Contact', 'node/' . $participant['contact_nid'], array('attributes' => array('class' => array('tool-button'))));
		$form['participants'][$participant_nid]['tools'] = array('#markup' => '<td>' . $tools . '</td>');

		$form['participants'][$participant_nid]['row_close'] = array('#markup' => '</tr>');

		if ($participant['participant_comments']) {
			$form['participants'][$participant_nid]['comments'] = array('#markup' => '<tr><td colspan="6">' . $participant['participant_comments'] . '</td></tr>');
		}
	}
	$form['participants']['table_close'] = array('#markup' => '</table>');

	$form['notify'] = array(
		'#type' => 'select',
		'#title' => t('Send Notifications'),
		'#description' => t('Send notifications by email to all participants when their section assignment has been changed.'),
		'#options' => array(
			'No' => t('No'),
			'Yes' => t('Yes'),
		),
	);

	$form['comments'] = array(
		'#type' => 'textarea',
		'#title' => t('Comments'),
		'#description' => t('Add a note that will be sent to any participants who are notified that their section assignment has been changed.'),
 		'#states' => array('visible' => array(":input[name=\"notify\"]" => array('value' => 'Yes'))),
 	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_node_tab_event_section_assign_form_submit($form, &$form_state) {
	$event_node = node_load($form_state['event_nid']);

	$options = array();
	$options['allow_oversubscribe'] = TRUE;
	if ($form_state['values']['notify'] == 'Yes') {
		$options['notify'] = array('comments' => $form_state['values']['comments']);
	}

	$participants = sirius_event_section_participants($event_node);
	foreach ($form_state['values']['participants'] as $participant_nid => $values) {
		$uuid = $values['uuid'];
		$result = sirius_event_section_assign($participant_nid, $uuid, $options);

		if ($result['success'] && !$result['changed']) { continue; }
		$msg = $participants[$participant_nid]['contact_title'] . ': ' . $result['msg'];
		if ($result['success']) { drupal_set_message($msg); } else { drupal_set_message($msg, 'warning'); }
	}

	drupal_set_message("Assignments saved.");
}