<?php

function sirius_training_page_evaluations_queue() {
	return drupal_get_form('sirius_training_page_evaluations_queue_form');
}

function sirius_training_page_evaluations_queue_form($form, &$form_state) {
	$evaluation_nodes = sirius_training_evaluations_fetch_by_type('draft');

	$form['#tree'] = TRUE;
	if ($evaluation_nodes) {
		foreach ($evaluation_nodes as $evaluation_nid => $evaluation_node) {
			$html = sirius_training_evaluations_render($evaluation_node, array());
			$form['evaluations'][$evaluation_nid]['intro'] = array('#markup' => $html);
			$form['evaluations'][$evaluation_nid]['status'] = array(
				'#type' => 'select',
				'#options' => sirius_training_evaluation_status_options(),
				'#title' => 'Change Status To',
				'#prefix' => '<div class="container-inline">',
				'#suffix' => '</div>',
			);
		}
	} else {
		$html .= '<p>There are no draft evaluations.</p>';
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_training_page_evaluations_queue_form_submit($form, &$form_state) {
	$counts = array();
	if ($form_state['values']['evaluations']) {
		foreach ($form_state['values']['evaluations'] as $evaluation_nid => $tmp) {
			$new_status = $tmp['status'];

			$evaluation_node = node_load($evaluation_nid);
			$old_status = $evaluation_node->field_sirius_type['und'][0]['value'];

			if ($old_status != $new_status) {
				$evaluation_node->field_sirius_type['und'][0]['value'] = $new_status;
				sirius_jsonfield_log($evaluation_node, 'status', "Changed status from $old_status to $new_status");
				node_save($evaluation_node);
			}
			++$counts[$new_status];
		}
	}

	$msg = "Status changed<ul>";
	foreach ($counts as $status => $count) {
		$msg .= '<li>' . "$status: $count" . '</li>';
	}
	$msg .= '</ul>';
	drupal_set_message($msg);
}