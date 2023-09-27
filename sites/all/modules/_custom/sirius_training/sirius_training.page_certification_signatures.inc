<?php

function sirius_training_node_tab_certification_signatures_access($certification_node) {
	// Only log entries of type certification
	if ($certification_node->field_sirius_category['und'][0]['value'] != 'worker:certification') { return FALSE; }

	// Only if signatures are configured
	$json = sirius_jsonfield_util_node_json_get($certification_node);
	$json_form = sirius_jsonfield_util_form_get($json, 'certification');
	$signature_definitions = $json_form['#sirius']['signature_definitions'];
	if (!$signature_definitions) { return FALSE; }

	// Admin can access
	if (user_access('sirius training admin')) { return TRUE; }
	if (user_access('sirius training certification edit')) { return TRUE; }

	// Author can access
	global $user;
	$status = $certification_node->field_sirius_type['und'][0]['value'];
	if ($user->uid == $certification_node->uid) {
		return TRUE;
	}

	// Oh well
	return FALSE;
}

function sirius_training_node_tab_certification_signatures($certification_node) {
	sirius_esignature_init();
	drupal_add_css(drupal_get_path('module', 'sirius_training') . '/css/training.css');

	$json = sirius_jsonfield_util_node_json_get($certification_node);
	$json_form = sirius_jsonfield_util_form_get($json, 'certification');
	$signature_definitions = $json_form['#sirius']['signature_definitions'];
	foreach ($signature_definitions as $signature_definition) {
		$signature_definition_map[$signature_definition['name']] = $signature_definition;
	}

	// If we're passed in an action, handle it
	$status = $certification_node->field_sirius_type['und'][0]['value'];

	if ($status == 'draft') {
		$action = $_REQUEST['action'];
		$which = $_REQUEST['which'];
		if ($action && $which) {
			if (!$signature_definition_map[$which]) {
				drupal_set_message("Error: $which not found.", 'error');
				return 'Error.';
			}
			if ($action == 'sign') {
				$statement = 'Signature: ' . $signature_definition_map[$which]['name'];
				return drupal_get_form('sirius_training_node_tab_certification_signatures_form', $certification_node->nid, $which, $statement);
			} else if ($action == 'clear') {
				sirius_jsonfield_util_esignature_set($json, $which, NULL);
				sirius_jsonfield_util_node_json_set($certification_node, $json);
				node_save($certification_node);
			}
		}
	}


	$header = array('Name', 'Signature', 'Tools');
	$rows = array();
	foreach ($signature_definitions as $signature_definition) {
		$name = $signature_definition['name'];
		$esignature = sirius_jsonfield_util_esignature_get($json, $name);
		$tools = '';
		if ($status == 'draft') {
			if ($esignature) {
				$tools = l('Clear', current_path(), array('attributes' => array('class' => array('tool-button')), 'query' => array('action' => 'clear', 'which' => $name)));
			} else {
				$tools = l('Sign Now', current_path(), array('attributes' => array('class' => array('tool-button')), 'query' => array('action' => 'sign', 'which' => $name)));
			}
		}

		$row = array();
		$row[] = $name;
		$row[] .= sirius_esignature_render($esignature, '', $certification_node->nid, $name);

		$row[] = $tools;

		$rows[] = $row;
	}

	$html .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array()));

	return $html;
}

function sirius_training_node_tab_certification_signatures_form($form, &$form_state, $nid, $which, $statement) {
	$form_state['nid'] = $nid;
	$form_state['which'] = $which;

	sirius_esignature_form_prepare($form, $which, "Signature", $statement);

	return $form;
}

function sirius_training_node_tab_certification_signatures_form_submit($form, &$form_state) {
	$which = $form_state['which'];
	$nid = $form_state['nid'];

	$node = node_load($nid);
	$json = sirius_jsonfield_util_node_json_get($node);
	$signature = sirius_esignature_form_get_json($form, $form_state, $which);
	sirius_jsonfield_util_esignature_set($json, $which, $signature);
	sirius_jsonfield_util_node_json_set($node, $json);
	drupal_set_message("Signature saved.");
	if (sirius_esignature_fully_signed($json, 'certification')) {
		if ($node->field_sirius_type['und'][0]['value'] == 'draft') {
			drupal_set_message(t('The certification is now fully signed, and can no longer be changed.'));
			$node->field_sirius_type['und'][0]['value'] = 'signed';
		}
	}

	node_save($node);

	drupal_goto(current_path());
}

