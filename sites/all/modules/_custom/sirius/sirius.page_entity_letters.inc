<?php

function sirius_node_tab_entity_letter_access($entity) {
	$enabled_types = variable_get('sirius_letter_content_types');
	if (!$entity->type) { return TRUE; }
	return $enabled_types[$entity->type];
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
	} else if ($action == 'delete') {
		return 'delete';
		return drupal_get_form('sirius_node_tab_entity_materials_delete_form', $entity, $material_node);
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

  global $user;
  $letter_node = new stdClass();
  $letter_node->type = 'sirius_log';
  $letter_node->no_recurse = TRUE;
  node_object_prepare($letter_node);
  $letter_node->language = LANGUAGE_NONE;
  $letter_node->uid = $user->uid;
  $letter_node->status = 1;
  $letter_node->promote = 0;
  $letter_node->comment = 0;
  $letter_node->title = substr(date('c') . " - " . $category . " - " . $type . " - " . $message, 0, 254);
  $letter_node->field_sirius_category['und'][0]['value'] = 'letter';
  $letter_node->field_sirius_type['und'][0]['value'] = 'draft';

  $domain_nid = sirius_domain_nid();
  if ($domain_nid) { $letter_node->field_sirius_domain['und'][0]['target_id'] = $domain_nid; } 
  $letter_node->field_sirius_log_handler['und'][0]['target_id'] = $entity_node->nid;

  $letter_node = node_submit($letter_node);
  node_object_prepare($letter_node);

	return sirius_node_tab_wizard($letter_node, 'letter');
}

function sirius_node_tab_entity_letter_edit($entity, $letter_node) {
	return sirius_node_tab_wizard($letter_node, 'letter');
}
