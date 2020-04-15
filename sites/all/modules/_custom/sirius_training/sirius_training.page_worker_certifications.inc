<?php

function sirius_training_node_tab_worker_certifications_access($worker_node) {
	// Certifications must be enabled
	if (sirius_domain_variable_get('sirius_training_certifications_enabled', '') != 'Yes') { return FALSE; }

	// Training admin can access
	if (user_access('sirius training admin')) { return TRUE; }

	// Worker can access their own
	$my_worker_node = sirius_worker_get();
	if ($my_worker_node) { return $my_worker_node->nid == $worker_node->nid; }

	return FALSE;
}

function sirius_training_node_tab_worker_certifications($worker_node) {
	$links = array();
	$status_options = sirius_training_certification_status_options();

	$link = array();
	$link['path'] = current_path();
	$link['title'] = 'Granted';
	$link['query'] = array();
	$links['granted'] = $link;

	foreach ($status_options as $key => $name) {
		if ($key == 'granted') { continue; }
		$link = array();
		$link['path'] = current_path();
		$link['title'] = $name;
		$link['query'] = array('status' => $key);
		$links[$key] = $link;
	}

	$link = array();
	$link['path'] = current_path();
	$link['title'] = 'All';
	$link['query'] = array('status' => 'all');
	$links['all'] = $link;

	$html .= sirius_quicknav($links);

	$status = $_REQUEST['status'];
	if (!$status) { $status = 'granted'; }
	else if ($status == 'all') { $status = NULL; }

	$certification_nodes = sirius_training_certifications_fetch_by_worker($worker_node->nid, $status);

	if ($certification_nodes) {
		foreach ($certification_nodes as $certification_node) {
			$html .= sirius_training_certifications_render($certification_node, array('hide_worker' => TRUE));
		}
	} else {
		$html .= '<p>';
		if ($status == 'all') {
			$html .= t('There are no certifications for this worker.');
		} else {
			$html .= t('There are no certifications for this worker with the status: ') . '<strong>' . $status_options[$status] . '</strong>';
		}
		$html .= '</p>';
	}

	$html .= drupal_render(drupal_get_form('sirius_training_node_tab_worker_certifications_add_form', $worker_node->nid));

	return $html;
}

function sirius_training_node_tab_worker_certifications_add_form($form, &$form_state, $worker_nid) {
	$form_state['worker_nid'] = $worker_nid;

	$definition_nodes = sirius_training_certification_definitions();
	if (!$definition_nodes) {
		drupal_set_message("There are no certification forms available.", 'warning');
		return array();
	}
	$options = array(NULL => t('-- Select --'));
	foreach ($definition_nodes as $definition_nid => $definition_node) {
		$options[$definition_nid] = $definition_node->title;
	}

	$form['definition_nid'] = array(
		'#type' => 'select',
		'#options' => $options,
		'#title' => t('Certification'),
		'#required' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Add a Certification'),
	);

	return $form;
}

function sirius_training_node_tab_worker_certifications_add_form_submit($form, &$form_state) {
	drupal_goto('sirius/training/certification/' . $form_state['worker_nid'] . '/' . $form_state['values']['definition_nid']);
}
