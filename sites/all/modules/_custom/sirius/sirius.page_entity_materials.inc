<?php

function sirius_materials_download_access($file_item, $entity_type, $material_node) {
	if ($entity_type != 'node') { return FALSE; }

	$node = node_load($material_node->field_sirius_log_handler['und'][0]['target_id']);

	if ($node->type == 'sirius_taxonomy_proxy') { $entity = taxonomy_term_load($node->field_sirius_term_proxy['und'][0]['target_id']); } else { $entity = $node; }

	return sirius_materials_access('view', $entity, $material_node);
}

function sirius_materials_list($entity) {
	if ($entity->tid) { $node = sirius_taxonomy_proxy($entity->tid); } else { $node = $entity; } 
	$query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_log');
  $query->fieldCondition('field_sirius_category', 'value', 'material');
	$query->fieldCondition('field_sirius_log_handler', 'target_id', $node->nid);
  $query->propertyOrderBy('title');
  $stmt = $query->execute();
  if (!$stmt['node']) { return array(); }
  return node_load_multiple(array_keys($stmt['node']));
}

function sirius_materials_has_body($material_node) {
	$json = sirius_jsonfield_util_node_json_get($material_node);
	return strlen($json['material']['body']['value']) > 10;
}

function sirius_materials_access($action, $entity, $material_node = NULL) {
	if ($entity->tid) { $node = sirius_taxonomy_proxy($entity->tid); } else { $node = $entity; } 

	// Verify that this material belongs to this node or entity
	if ($material_node) { 
		$good = FALSE; 
		if ($material_node->field_sirius_log_handler['und'][0]['target_id'] == $node->nid) { $good = TRUE; }
		if (!$good) {
			$real_parent = node_load($material_node->field_sirius_log_handler['und'][0]['target_id']);
			if ($real_parent->nid != $node->nid) { 
				return sirius_materials_access($action, $real_parent, $material_node); 
			}
		}
		if (!$good) { return FALSE; }
	}

	// Admins can do anything
	if (user_access('sirius admin')) { return TRUE; }

	// //////////////////////////////////////////////////////
	// Type-specific logic
	// @todo: This should be in callbacks
	// //////////////////////////////////////////////////////

	if ($node->type == 'sirius_event') {
		// Admins can do anything
		if (user_access('sirius event admin')) { return TRUE; }
		if (user_access('sirius training admin')) { return TRUE; }

		// Check access rules for the event type
		$event_type_term = taxonomy_term_load($node->field_sirius_event_type['und'][0]['tid']);
		if (!$event_type_term) { return FALSE; }
		$term_json = sirius_jsonfield_util_term_json_get($event_type_term);
		$matches = sirius_event_paccess_rule_match($node->nid, $term_json['materials']['rules'], TRUE);
		foreach ($matches as $match) {
			if (in_array($action, array_values($match['actions']))) { return TRUE; }
		}

		// Oh well
		return FALSE;
	}

	if ($node->type == 'sirius_term_proxy') {
		if ($entity->tid) { $term = $entity; } else { $term = taxonomy_term_load($node->field_sirius_term_proxy['und'][0]['target_id']); } 
		if ($term->vocabulary_machine_name == 'sirius_event_type') {
			// Admins can do anything
			if (user_access('sirius event admin')) { return TRUE; }
			if (user_access('sirius training admin')) { return TRUE; }

			// Anybody can view or list
			if ($action == 'view' || $action == 'list') { return TRUE; }

			// Oh well
			return FALSE;
		}
	}

	return FALSE;
}

function sirius_node_tab_entity_materials($entity) {
	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/materials.css');
	$action = sirius_node_tab_arg(0);
	if (!$action) { $action = 'list'; }
	$material_nid = sirius_node_tab_arg(1);
	if ($material_nid) { $material_node = node_load($material_nid); }

	$access = sirius_materials_access($action, $entity, $material_node);
	if (!$access) { return 'Access denied.'; }

	if ($action == 'add') {
		return drupal_get_form('sirius_node_tab_entity_materials_addedit_form', $entity);
	} else if ($action == 'view') {
		return sirius_node_tab_entity_materials_view($entity, $material_node);
	} else if ($action == 'edit') {
		return drupal_get_form('sirius_node_tab_entity_materials_addedit_form', $entity, $material_node);
	} else if ($action == 'delete') {
		return drupal_get_form('sirius_node_tab_entity_materials_delete_form', $entity, $material_node);
	} else {
		return sirius_node_tab_entity_materials_list($entity);
	}
}

function sirius_node_tab_entity_materials_list($entity, $options = array()) {
	if ($entity->tid) { $node = sirius_taxonomy_proxy($entity->tid); } else { $node = $entity; } 

	$materials = sirius_materials_list($entity);
	if (!$materials && $options['hide_empty']) { return ''; }

	$rows = array();
	foreach ($materials as $material_nid => $material_node) {

		$tools = '';
		if (sirius_materials_has_body($material_node) && sirius_materials_access('view', $entity, $material_node)) {
			$tools .= l('View', current_path() . '/view/' . $material_nid, array('attributes' => array('class' => array('tool-button'))));
		}
		if (sirius_materials_access('edit', $entity, $material_node)) {
			$tools .= l('Edit', current_path() . '/edit/' . $material_nid, array('attributes' => array('class' => array('tool-button')), 'query' => array('destination' => current_path())));
		}
		if (sirius_materials_access('delete', $entity, $material_node)) {
			$tools .= l('Delete', current_path() . '/delete/' . $material_nid, array('attributes' => array('class' => array('tool-button')), 'query' => array('destination' => current_path())));
		}

		if (sirius_materials_access('view', $entity, $material_node)) {
			$renderable = field_view_field('node', $material_node, 'field_sirius_attachments', 'full');
			if ($renderable) {
				$renderable['#label_display'] = 'hidden';
				$attachment_render = render($renderable);
			} else {
				$attachment_render = '';
			}
		} else {
			$attachment_render = '';
		}

		$row = array();
		$row[] = $material_node->title;
		$row[] = sirius_text_to_html(sirius_fieldval($material_node, 'field_sirius_summary'));
		$row[] = $attachment_render;	
		$row[] = $tools;
		$rows[] = $row;
	}

	if ($materials) {
		$header = array('Title', 'Description', 'Attachment', 'Tools');
		$html .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('sirius_materials'))));
	} else {
		$html .= '<p>No materials found.</p>';
	}

	if (!$options['hide_add']) {
		if (sirius_materials_access('add', $entity)) {
			$html .= l('Add a Material', current_path() . '/add', array('attributes' => array('class' => 'tool-button'), 'query' => array('destination' => current_path())));
		}
	}

	// //////////////////////////////////////////////////////
	// Type-specific logic
	// @todo: This should be in callbacks
	// //////////////////////////////////////////////////////

	if ($node->type == 'sirius_event') {
		$event_type_term = taxonomy_term_load($node->field_sirius_event_type['und'][0]['tid']);
		if (sirius_materials_access('list', $event_type_term)) {
			$parent_html = sirius_node_tab_entity_materials_list($event_type_term, array('hide_add' => TRUE, 'hide_empty' => TRUE));
			if ($parent_html) {
				$html .= '<h2>' . t('General materials for: ') . $event_type_term->name . '</h2>';
				$html .= $parent_html;
			}
		}
	}

	return $html;
}

function sirius_node_tab_entity_materials_view($entity, $material_node) {
	$json = sirius_jsonfield_util_node_json_get($material_node);
	$html = '<h2>' . $json['material']['title'] . '</h2>';
	$html .= sirius_text_to_html(sirius_fieldval($material_node, 'field_sirius_summary'));
	$html .= '<hr>';
	$html .= $json['material']['body']['value'];
	$html .= drupal_render(field_view_field('node', $material_node, 'field_sirius_attachments', 'full'));
	return $html;
}

function sirius_node_tab_entity_materials_addedit_form($form, &$form_state, $entity, $material_node = NULL) {
	if ($entity->tid) { $node = sirius_taxonomy_proxy($entity->tid); } else { $node = $entity; } 

	if ($material_node) { $form_state['material_nid'] = $material_node->nid; }

	$json = sirius_jsonfield_util_node_json_get($material_node);

	$form_state['nid'] = $node->nid;

	$form['#tree'] = TRUE;

	$form['material']['title'] = array(
		'#type' => 'textfield',
		'#title' => t('Title'),
		'#required' => TRUE,
	);
	if ($json['material']['title']) { $form['material']['title']['#default_value'] = $json['material']['title']; }

	$form['material']['summary'] = array(
		'#type' => 'textarea',
		'#title' => t('Summary'),
		'#required' => TRUE,
	);
	$default_summary = sirius_fieldval($material_node, 'field_sirius_summary');
	if ($default_summary) { $form['material']['summary']['#default_value'] = $default_summary; }

	$form['material']['body'] = array(
		'#type' => 'text_format',
		'#title' => t('Body'),
	);
	$default_body = $json['material']['body']['value'];
	if ($default_body) { $form['material']['body']['#default_value'] = $default_body; }

	$form["attach"] = array(
		'#type' => 'file',
		'#title' => t('Attachment'),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Submit',
	);

	return $form;
}

function sirius_node_tab_entity_materials_addedit_form_validate($form, &$form_state) {
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
      form_set_error("attach_$delta", t("Failed to write the uploaded file to the site's file folder."));
    }
	}
}

function sirius_node_tab_entity_materials_addedit_form_submit($form, &$form_state) {
	$files = array();
  $file = $form_state['storage']["attach"];
  if ($file) {
		$file = (array) $file;
		$file['display'] = 1;
		unset($form_state['storage']["attach"]);
		unset($file['destination']);
		unset($file['uri']);
		$files[] = $file;
  }

	$node = node_load($form_state['nid']);

	$material_node = node_load($form_state['material_nid']);
	if ($material_node) {
		$json = sirius_jsonfield_util_node_json_get($material_node);
		$json['material'] = $form_state['values']['material'];
		sirius_jsonfield_util_node_json_set($material_node, $json);
		$material_node->field_sirius_summary['und'][0]['value'] = $form_state['values']['material']['summary'];
		if ($files) { $material_node->field_sirius_attachments['und'] = $files; }
		$material_node->title = $json['material']['title'];
		node_save($material_node);
		drupal_set_message("Material edited.");
	} else {
		$json = array();
		$json['material'] = $form_state['values']['material'];
		$summary = $form_state['values']['material']['summary'];
		$category = 'material';
		$type = 'material';
		$material_node = sirius_log($category, $type, $summary, $summary, NULL, array($node->nid), $files, $json);
		$material_node->title = $json['material']['title'];
		node_save($material_node);
		drupal_set_message(t('Material added.'));
	}
}



function sirius_node_tab_entity_materials_delete_form($form, &$form_state, $entity, $material_node) {
	if ($entity->tid) { $node = sirius_taxonomy_proxy($entity->tid); } else { $node = $entity; } 
	$form_state['material_nid'] = $material_node->nid;

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Really Delete?'),
	);

	return $form;
}

function sirius_node_tab_entity_materials_delete_form_submit($form, &$form_state) {
	node_delete($form_state['material_nid']);
	drupal_set_message(t('Material deleted.'));
}
