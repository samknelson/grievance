<?php

/**
 * Use which_form = 'ALL' for a full exim of the entire JSON 
 */

function sirius_json_definition_exim_form($form, &$form_state, $entity_id, $which_form = 'default', $entity_type = 'node') {
	$form_state['entity_id'] = $entity_id;
	$form_state['entity_type'] = $entity_type;
	$form_state['which_form'] = $which_form;

	if ($entity_type == 'taxonomy_term') { $entity = taxonomy_term_load($entity_id);  } else { $entity = node_load($entity_id);	}

	$json = sirius_jsonfield_util_node_json_get($entity);
	if ($which_form == 'ALL') {
		$json_form = $json;
	} else {
		$json_form = sirius_jsonfield_util_form_get($json, $which_form);
	}

	$form['json'] = array(
		'#type' => 'textarea',
		'#default_value' => json_encode($json_form, TRUE),
		'#title' => 'Raw Field Definition',
	);

	$form['confirm'] = array(
		'#type' => 'checkbox',
		'#title' => 'Yes, replace the ENTIRE form definition. This action cannot be undone.',
		'#required' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Import',
	);

	return $form;
}

function sirius_json_definition_exim_form_submit($form, &$form_state) {
	$entity_id = $form_state['entity_id'];
	$entity_type = $form_state['entity_type'];
	$which_form = $form_state['which_form'];

	if ($entity_type == 'taxonomy_term') { $entity = taxonomy_term_load($entity_id);  } else { $entity = node_load($entity_id);	}

	$json = sirius_jsonfield_util_node_json_get($entity);
	$txt = $form_state['values']['json'];
	$json_form = json_decode($txt, TRUE);
	if (!$json_form) {
		drupal_set_message("Please enter a valid JSON-encoded string.", 'error');
		return;
	}

	if ($which_form == 'ALL') {
		$json = $json_form;
	} else {
		sirius_jsonfield_util_form_set($json, $which_form, $json_form);
	}

	sirius_jsonfield_util_node_json_set($entity, $json);

	if ($entity_type == 'taxonomy_term') { taxonomy_term_save($entity); } else { node_save($entity);	}

	drupal_set_message("Settings updated.");
}