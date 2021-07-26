<?php

function sirius_worker_node_tab_schedule_tor_worker_list_access($worker_node) {
	return sirius_worker_schedule_tor_access('list', $worker_node);
}

function sirius_worker_node_tab_schedule_tor_worker_list($worker_node) {
	return drupal_get_form('sirius_worker_schedule_tor_list_form', $worker_node);
}

function sirius_worker_schedule_tor_list_form($form, &$form_state, $worker_node) {
	$form_state['worker_nid'] = $worker_node->nid;
	$tor_type_lookup = sirius_taxonomy_load('sirius_worker_tor_type', 'tid', 'name');
	$tor_status_lookup = sirius_worker_schedule_tor_status_options();

	$form['#tree'] = TRUE;
	$form['filters'] = array(
		'#type' => 'fieldset',
		'#title' => t('Filters'),
		'#collapsible' => TRUE,
	);
	if (!$form_state['values']['filters']) { $form['filters']['#collapsed'] = TRUE; }

	$form['filters']['type_tid'] = array(
		'#title' => t('Type'),
		'#type' => 'select',
		'#options' => array(NULL => t('-- Select --')) + $tor_type_lookup,
	);

	$form['filters']['status'] = array(
		'#title' => t('Status'),
		'#type' => 'select',
		'#options' => array(NULL => t('-- Select --')) + $tor_status_lookup,
	);

	$form['filters']['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	$search_params = $form_state['values']['filters'];
	$search_params['worker_nid'] = $worker_node->nid;
	$tors = sirius_worker_schedule_tor_list($search_params);


	$rows = array();
	foreach ($tors as $tor_nid => $tor_node) {
		$tools = '';
		$tools .= l('View', "node/$tor_nid/sirius_worker_schedule_tor", array('attributes' => array('class' => array('tool-button'))));
		if (sirius_worker_schedule_tor_access('edit', $worker_node, $tor_node)) {
			$tools .= l('Edit', "node/$tor_nid/sirius_worker_schedule_tor/edit", array('attributes' => array('class' => array('tool-button'))));
		}

		if (sirius_worker_schedule_tor_access('view', $worker_node, $tor_node)) {
			$renderable = field_view_field('node', $tor_node, 'field_sirius_attachments', 'full');
			if ($renderable) {
				$renderable['#label_display'] = 'hidden';
				$attachment_render = render($renderable);
			} else {
				$attachment_render = '';
			}
		} else {
			$attachment_render = '';
		}

		$json = sirius_jsonfield_util_node_json_get($tor_node);
		$status_options = sirius_worker_schedule_tor_status_options();

		$row = array();
		$row[] = $tor_type_lookup[$json['tor']['type_tid']];
		$row[] = $json['tor']['start_date'];
		$row[] = $json['tor']['end_date'];
		$row[] = sirius_text_to_html(sirius_fieldval($tor_node, 'field_sirius_summary'));
		$row[] = $status_options[$json['tor']['status']];
		$row[] = $attachment_render;	
		$row[] = $tools;
		$rows[] = $row;
	}

	if ($tors) {
		$header = array(
			t('Type'),
			t('Start'), 
			t('End'), 
			t('Description'), 
			t('Status'),
			t('Attachment'), 
			t('Tools'),
		);
		$html .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('sirius_worker_schedule_tors'))));
	} else {
		$html .= '<p>No time-off requests found.</p>';
	}

	// Are there any types that we can add?
	$can_add = FALSE;
	foreach ($tor_type_lookup as $tor_type_tid => $tor_type_name) {
		if (sirius_worker_schedule_tor_access('add', $worker_node, NULL, $tor_type_tid)) { 
			$can_add = TRUE;
			break;
		}
	}
	if ($can_add) {
		$html .= l('Add a Time-Off Request', current_path() . '/add', array('attributes' => array('class' => 'tool-button')));
	}

	$form['results'] = array('#markup' => $html);

	return $form;
}

function sirius_worker_schedule_tor_list_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
}