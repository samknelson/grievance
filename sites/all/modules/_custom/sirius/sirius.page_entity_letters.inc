<?php

function sirius_node_tab_entity_letter_access($entity) {
	$enabled_types = sirius_domain_variable_get('sirius_letter_content_types');
	if (!$entity->type) { return FALSE; }
	if (!$enabled_types[$entity->type]) { return FALSE; }

	if ($entity->type == 'sirius_log') {
		if (!in_array($entity->field_sirius_category['und'][0]['value'], array('ledger:auth'))) { return FALSE; }
	}
	
	return TRUE;
}


function sirius_node_tab_entity_letter($entity) {
	$action = sirius_node_tab_arg(0);
	if (!$action) { $action = 'list'; }
	$letter_nid = sirius_node_tab_arg(1);
	if ($letter_nid) { $letter_node = node_load($letter_nid); }

	$access = sirius_letter_access($action, $entity, $letter_node);
	if (!$access) { return 'Access denied.'; }

	if ($action == 'add') {
		return sirius_node_tab_entity_letter_add($entity);
	} else if ($action == 'edit') {
		return sirius_node_tab_entity_letter_edit($entity, $letter_node);
	} else if ($action == 'oneclick') {
		return sirius_node_tab_entity_letter_oneclick($entity);
	} else {
		return sirius_node_tab_entity_letter_list($entity);
	}
}

function sirius_node_tab_entity_letter_list($entity, $options = array()) {
	$letters = sirius_letter_list($entity);
	if (!$letters && $options['hide_empty']) { return ''; }

	$rows = array();
	foreach ($letters as $letter_nid => $letter_node) {

		$tools = '';
		if (sirius_letter_access('edit', $entity, $letter_node)) {
			$tools .= l('Edit', current_path() . '/edit/' . $letter_nid, array('attributes' => array('class' => array('tool-button'))));
		}

		if (sirius_letter_access('view', $entity, $letter_node)) {
			$renderable = field_view_field('node', $letter_node, 'field_sirius_attachments', 'full');
			if ($renderable) {
				$renderable['#label_display'] = 'hidden';
				$attachment_render = render($renderable);
			} else {
				$attachment_render = '';
			}
		} else {
			$attachment_render = '';
		}

		$title = $letter_node->title;

		$row = array();
		$row[] = $title;
		$row[] = $letter_node->field_sirius_type['und'][0]['value'];
		$row[] = $tools;
		$rows[] = $row;
	}

	if (!$options['hide_add']) {
		if (sirius_letter_access('add', $entity)) {
			$html .= l('Add a Letter', current_path() . '/add', array('attributes' => array('class' => 'tool-button')));
		}
	}

	if ($letters) {
		$header = array('Title', 'Status', 'Tools');
		$html .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('sirius_materials'))));
	} else {
		$html .= '<p>No letters have been sent yet.</p>';
	}

	return $html;
}


function sirius_node_tab_entity_letter_add($entity) {
	if ($entity->tid) { $entity_node = sirius_taxonomy_proxy($entity->tid); } else { $entity_node = $entity; } 
	
	$letter_nid = sirius_node_tab_arg(1);
	if ($letter_nid > 0) {
		$path = preg_replace('/\/add\//', '/edit/', current_path());
		drupal_goto($path);
	}

	$letter_node = sirius_letter_create($entity, FALSE);
	return sirius_node_tab_wizard($letter_node, 'letter');
}

function sirius_node_tab_entity_letter_edit($entity, $letter_node) {
	return sirius_node_tab_wizard($letter_node, 'letter');
}

function sirius_node_tab_entity_letter_oneclick($entity) {
	return drupal_get_form('sirius_node_tab_entity_letter_oneclick_form', $entity);
}

function sirius_node_tab_entity_letter_oneclick_form($form, &$form_state, $entity) { 
	if ($entity->tid) { $entity_node = sirius_taxonomy_proxy($entity->tid); } else { $entity_node = $entity; } 
	$form_state['entity_nid'] = $entity_node->nid;

	$letter_templates = sirius_letter_template_list($entity_node);
	if (!$letter_templates) { drupal_set_message("You do not have access to any letter templates for this content type"); }
	$letter_options = array(NULL => t('-- Select --'));
	if ($letter_templates) {
		foreach ($letter_templates as $letter_template_nid => $letter_template_node) {
			$letter_options[$letter_template_nid] = $letter_template_node->title;
		}
	}
	$form['#tree'] = TRUE;
	$form["letter_template_nid_0"] = array(
		'#type' => 'select',
		'#title' => t('Template'),
		'#options' => $letter_options,
		'#required' => TRUE,
	);
	$format_options = array(NULL => t('-- Select --')) + sirius_letter_format_options($entity_node);
	$form["format"] = array(
		'#type' => 'select',
		'#title' => t('Format'),
		'#default_value' => $data["format"],
		'#options' => $format_options,
		'#required' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_node_tab_entity_letter_oneclick_form_submit($form, &$form_state) {
	$entity_node = node_load($form_state['entity_nid']);
	$path = preg_replace('/\/oneclick.*/', '', current_path());

	drupal_set_message("Working...");
	$form_state['rebuild'] = TRUE;
	return;
	drupal_goto($path);
}

