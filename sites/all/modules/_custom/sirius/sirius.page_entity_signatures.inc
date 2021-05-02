<?php

function sirius_node_tab_entity_signatures_access($entity_node) {
	if ($entity_node->type == 'sirius_log') {
		if (!in_array($entity_node->field_sirius_category['und'][0]['value'], array('ledger:auth'))) { return FALSE; }
	}

	return TRUE;
}

function sirius_node_tab_entity_signatures_defs_form($form, &$form_state, $entity_node) {
	$form_state['entity_nid'] = $entity_node->nid;
	$defs = sirius_esignature_defs();
	if (!$defs) {
		$form['intro'] = array('#markup' => '<p>' . t('There are no signatures configured. Go to the ') . l('configuration page', 'sirius/esignature/admin') . t(' to create them.') . '</p>');
		return $form;
	}
	$options = array();
	foreach ($defs as $def) {
		$options[$def['code']] = $def['title'];
	}

	$entity_json = sirius_jsonfield_util_node_json_get($entity_node);

	$form['defs'] = array(
		'#title' => 'Required Signatures',
		'#type' => 'checkboxes',
		'#options' => $options,
 		'#prefix' => "<div class=\"checkboxes-inline\">",
 		'#suffix' => '</div><br clear="left" />',
 	);
 	if ($entity_json['esignature']['defs']) { $form['defs']['#default_value'] = $entity_json['esignature']['defs']; }
 	if (drupal_valid_path('sirius/esignature/admin')) {
 		$form['defs']['#title'] .= ' ' . l('(Manage)', 'sirius/esignature/admin');
 	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);
	return $form;
}

function sirius_node_tab_entity_signatures_defs_form_submit($form, &$form_state) {
	$entity_node = node_load($form_state['entity_nid']);
	$entity_json = sirius_jsonfield_util_node_json_get($entity_node);
	$entity_json['esignature']['defs'] = $form_state['values']['defs'];
	sirius_jsonfield_util_node_json_set($entity_node, $entity_json);
	node_save($entity_node);
	drupal_set_message("Settings saved.");
}

function sirius_node_tab_entity_signatures($entity_node) {
	sirius_esignature_init();
	drupal_add_css(drupal_get_path('module', 'sirius_training') . '/css/training.css');

	$esignature_status = sirius_esignature_entity_status($entity_node);

	$json = sirius_jsonfield_util_node_json_get($entity_node);
	$defs = sirius_esignature_defs($entity_node);

	// If we're passed in an action, handle it
	$status = $entity_node->field_sirius_type['und'][0]['value'];

	$action = $_REQUEST['action'];
	$which = $_REQUEST['which'];
	if ($action && $which) {
		if (!$defs[$which]) {
			drupal_set_message("Error: $which not found.", 'error');
			return 'Error.';
		}
		if ($action == 'sign') {
			$statement = $defs[$which]['title'];
			return drupal_get_form('sirius_node_tab_entity_signatures_form', $entity_node->nid, $which, $statement);
		} else if ($action == 'clear') {
			sirius_jsonfield_util_esignature_set($json, $which, NULL);
			sirius_jsonfield_util_node_json_set($entity_node, $json);
			node_save($entity_node);
		}
	}

	$header = array('Name', 'Signature', 'Tools');
	$rows = array();
	foreach ($defs as $def) {
		$name = $def['title'];
		$code = $def['code'];
		$esignature = sirius_jsonfield_util_esignature_get($json, $code);
		$tools = '';
		if ($esignature) {
			$tools = l('Revoke', current_path(), array('attributes' => array('class' => array('tool-button')), 'query' => array('action' => 'clear', 'which' => $code)));
		} else {
			$tools = l('Sign Now', current_path(), array('attributes' => array('class' => array('tool-button')), 'query' => array('action' => 'sign', 'which' => $code)));
		}

		$row = array();
		$row[] = $name;
		$row[] .= sirius_esignature_render($esignature, '', $entity_node->nid, $code);

		$row[] = $tools;

		$rows[] = $row;
	}

	if ($rows) {
		$html .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array()));
	}
	if ($html) {
		$html .= '<hr>';
	}
	$html .= drupal_render(drupal_get_form('sirius_node_tab_entity_signatures_defs_form', $entity_node));
	return $html;
}

function sirius_node_tab_entity_signatures_form($form, &$form_state, $nid, $which, $statement) {
	$form_state['nid'] = $nid;
	$form_state['which'] = $which;

	sirius_esignature_form_prepare($form, $which, "Approved", $statement, array('show_name' => TRUE, 'show_org' => TRUE));

	return $form;
}

function sirius_node_tab_entity_signatures_form_submit($form, &$form_state) {
	$which = $form_state['which'];
	$nid = $form_state['nid'];

	$node = node_load($nid);
	$json = sirius_jsonfield_util_node_json_get($node);
	$signature = sirius_esignature_form_get_json($form, $form_state, $which);
	sirius_jsonfield_util_esignature_set($json, $which, $signature);
	sirius_jsonfield_util_node_json_set($node, $json);
	drupal_set_message("Signature saved.");
	node_save($node);

	drupal_goto(current_path());
}