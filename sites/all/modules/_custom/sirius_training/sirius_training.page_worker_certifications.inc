<?php

function sirius_training_node_tab_worker_certifications_access($worker_node) {
	// Certifications must be enabled
	if (sirius_domain_variable_get('sirius_training_certifications_enabled', '') != 'Yes') { return FALSE; }

	// Training admin can access
	if (user_access('sirius admin')) { return TRUE; }
	if (user_access('sirius training admin')) { return TRUE; }
	if (user_access('sirius training certification view')) { return TRUE; }

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

	if (user_access("sirius training certification edit")) {
		$html .= drupal_render(drupal_get_form('sirius_training_node_tab_worker_certifications_add_form', $worker_node->nid));
	}

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
		'#title' => t('Add a Certification'),
		'#required' => TRUE,
		'#attributes' => array('class' => array('container-inline')),
		'#prefix' => '<div class="container-inline">',
	);

	$form['action'] = array(
		'#type' => 'select',
		'#options' => array(
			'online' => t('Fill out the certification form online'),
			'upload' => t('Upload a scanned certification form'),
		),
		'#required' => TRUE,
		'#attributes' => array('class' => array('container-inline')),
	);

	$form['attach'] = array(
		'#type' => 'file',
		'#title' => 'Upload',
		'#states' => array('visible' => array(":input[name=\"action\"]" => array('value' => 'upload'))),
		 '#attributes' => array('class' => array('container-inline')),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Add'),
		'#attributes' => array('class' => array('container-inline')),
		'#suff' => '</div>',
	);

	return $form;
}

function sirius_training_node_tab_worker_certifications_add_form_validate($form, &$form_state) {
	$info = field_info_instance('node', 'field_sirius_attachments', 'sirius_log');
	$extensions = $info['settings']['file_extensions'];
  $file = file_save_upload("attach", array(
    'file_validate_extensions' => array($extensions),
  ));
  if ($file) {
    $name = rand();
    if ($file = file_move($file, 'private://attach_' . rand())) {
      $form_state['storage']["attach"] = $file;
    }
    else {
      form_set_error("attach", t("Failed to write the uploaded file to the site's file folder."));
    }
  }

  if ($form_state['values']['action'] == 'upload' && !$file) {
  	form_set_error("attach", t("Please upload a certification form."));
  }
}

function sirius_training_node_tab_worker_certifications_add_form_submit($form, &$form_state) {
	$action = $form_state['values']['action'];
	if ($action == 'online') { drupal_goto('sirius/training/certification/' . $form_state['worker_nid'] . '/' . $form_state['values']['definition_nid']); }

	$worker_nid = $form_state['worker_nid'];
	$worker_node = node_load($worker_nid);
	$definition_nid = $form_state['values']['definition_nid'];
	$participant_nid = $form_state['participant_nid'];

	$files = array();
  $file = $form_state['storage']["attach"];
  if ($file) {
		$file = (array) $file;
		$file['display'] = 1;
		unset($form_state['storage']["attach"]);
		unset($file['destination']);
		unset($file['uri']);
		$file['description'] = $form_state['values']["description"];
		$files[] = $file;
	}

	// Save the evaluation.
	$json = array();
	$handler_nids = array();
	$handler_nids[] = $worker_nid;
	$handler_nids[] = $definition_nid;
	if ($participant_nid) { $handler_nids[] = $participant_nid; }
	$certification_node = sirius_training_certification_add($handler_nids, $json, $files);
	sirius_minilog("Created certification from upload.");
	sirius_training_certification_grant($certification_node);
	drupal_set_message(sirius_minilog_render_html());
}
