<?php

function sirius_node_tab_entity_customfield_access($entity_node) {
	// For the moment, you neeed to be an admin. 
	
	if (user_access('sirius admin')) { return TRUE; }
	if (user_access('sirius domain admin')) { return TRUE; }

  // @todo: At some point, we probably need some kind of a fancy system for deciding which event roles and statuses can access which custom field sets.
  if ($entity_node->type == 'sirius_event') {
	 if (user_access('sirius event admin')) { return TRUE; }
  }

  if ($entity_node->type == 'sirius_ledger_account') {
   if (user_access('sirius ledger admin')) { return TRUE; }
  }

  if ($entity_node->type == 'sirius_trust_benefit') {
   if (user_access('sirius trust admin')) { return TRUE; }
  }

	return FALSE;
}

function sirius_node_tab_entity_customfield($entity_node) {
	// Get the list of available custom field sets
	$entity_json = sirius_jsonfield_util_node_json_get($entity_node);
	$sets = sirius_customfield_set_list($entity_json);
	if (!$sets) { return 'There are no custom field sets assigned.'; }

	// Get our current customfield set
	$set_nid = sirius_node_tab_arg(0);
	if (!$set_nid) {
		$set_nid = array_keys($sets)[0];
		drupal_goto('node/' . $entity_node->nid . '/' . $entity_node->type . '_customfield/edit/' . $set_nid);
	}
	if (!$sets[$set_nid]) { return t('That custom field set has not been configured for this entity.'); }

	// Create a navbar
	if (count($sets) > 1) {
		foreach ($sets as $nid => $name) {
			$link = array();
			$link['path'] = 'node/' . $entity_node->nid .  '/' . $entity_node->type . '_customfield/edit/' . $nid;
			$link['title'] = $name;
			$links[] = $link;
		}
		$html = sirius_quicknav($links);
	}

	// Render the particular form we're interested in.
	$html .= drupal_render(drupal_get_form('sirius_node_tab_entity_customfield_form', $entity_node, $set_nid));

	return $html;
}


function sirius_node_tab_entity_customfield_form($form, &$form_state, $entity_node, $customfield_set_nid) {
  $form_state['entity_nid'] = $entity_node->nid;
  $form_state['customfield_set_nid'] = $customfield_set_nid;
  $customfield_set_node = node_load($customfield_set_nid);
  $customfield_set_json = sirius_jsonfield_util_node_json_get($customfield_set_node);
  $customfield_form = sirius_jsonfield_util_form_get($customfield_set_json, 'default');
  if (!$customfield_form) { 
    drupal_set_message("No custom fields have been configured. They can be set up " . l('here.', 'sirius/customfield/definition/list'));
    return array();
  }

  $which = 'customfield_' . $customfield_set_nid;
  $entity_json = sirius_jsonfield_util_node_json_get($entity_node);
  $entity_results = sirius_jsonfield_util_results_get($entity_json, $which);
  sirius_jsonfield_util_set_defaults($customfield_form, $entity_results);

  $form[$which] = $customfield_form;
  $form['#tree'] = TRUE;

  $form['!intro'] = array(
          '#markup' => '<div class="sirius-form">',
          '#weight' => -9999,
  );

  $form['!outro'] = array(
          '#markup' => '</div>',
          '#weight' => 9999,
  );

  $form['submit'] = array(
          '#type' => 'submit',
          '#value' => t('Submit'),
          '#weight' => 10000,
  );

  return $form;
}

function sirius_node_tab_entity_customfield_form_submit($form, &$form_state) {
	$entity_nid = $form_state['entity_nid'];
	$customfield_set_nid = $form_state['customfield_set_nid'];
  $which = 'customfield_' . $customfield_set_nid;

	$entity_node = node_load($entity_nid);
  $entity_json = sirius_jsonfield_util_node_json_get($entity_node);
  sirius_jsonfield_util_results_set($entity_json, $which, $form_state['values'][$which]);
  sirius_jsonfield_util_node_json_set($entity_node, $entity_json);
  sirius_jsonfield_log($entity_node, 'survey', 'Custom fields updated.');
  node_save($entity_node);
  drupal_set_message("Record saved.");
}















function sirius_term_tab_entity_customfield_access($entity_term) {
	// For the moment, you neeed to be an admin. 
	
	if (user_access('sirius admin')) { return TRUE; }
	if (user_access('sirius domain admin')) { return TRUE; }

  // @todo: At some point, we probably need some kind of a fancy system for deciding which event roles and statuses can access which custom field sets.
  if ($entity_term->vocabulary_machine_name == 'sirius_event_type') {
	 if (user_access('sirius event admin')) { return TRUE; }
  }

	return FALSE;
}

function sirius_term_tab_entity_customfield($entity_term) {
	// Get the list of available custom field sets
	$entity_json = sirius_json_get($entity_term);
	$sets = sirius_customfield_set_list($entity_json);
	if (!$sets) { return 'There are no custom field sets assigned.'; }

	// Get our current customfield set
	$set_nid = sirius_node_tab_arg(0);
	if (!$set_nid) {
		$set_nid = array_keys($sets)[0];
		drupal_goto('taxonomy/term/' . $entity_term->tid . '/' . $entity_term->vocabulary_machine_name . '_customfield/edit/' . $set_nid);
	}
	if (!$sets[$set_nid]) { return t('That custom field set has not been configured for this entity.'); }

	// Create a navbar
	if (count($sets) > 1) {
		foreach ($sets as $nid => $name) {
			$link = array();
			$link['path'] = 'taxonomy/term/' . $entity_term->tid . '/' . $entity_term->vocabulary_machine_name . '_customfield/edit/' . $nid;
			$link['title'] = $name;
			$links[] = $link;
		}
		$html = sirius_quicknav($links);
	}

	// Render the particular form we're interested in.
	$html .= drupal_render(drupal_get_form('sirius_term_tab_entity_customfield_form', $entity_term, $set_nid));

	return $html;
}


function sirius_term_tab_entity_customfield_form($form, &$form_state, $entity_term, $customfield_set_nid) {
	$form_state['entity_tid'] = $entity_term->tid;
  $form_state['customfield_set_nid'] = $customfield_set_nid;
  $customfield_set_node = node_load($customfield_set_nid);
  $customfield_set_json = sirius_json_get($customfield_set_node);
  $customfield_form = sirius_jsonfield_util_form_get($customfield_set_json, 'default');
  if (!$customfield_form) { 
    drupal_set_message("No custom fields have been configured. They can be set up " . l('here.', 'sirius/customfield/definition/list'));
    return array();
  }

  $which = 'customfield_' . $customfield_set_nid;
  $entity_json = sirius_jsonfield_util_node_json_get($entity_term);
  $entity_results = sirius_jsonfield_util_results_get($entity_json, $which);
  sirius_jsonfield_util_set_defaults($customfield_form, $entity_results);

  $form[$which] = $customfield_form;
  $form['#tree'] = TRUE;

  $form['!intro'] = array(
          '#markup' => '<div class="sirius-form">',
          '#weight' => -9999,
  );

  $form['!outro'] = array(
          '#markup' => '</div>',
          '#weight' => 9999,
  );

  $form['submit'] = array(
          '#type' => 'submit',
          '#value' => t('Submit'),
          '#weight' => 10000,
  );

  return $form;
}

function sirius_term_tab_entity_customfield_form_submit($form, &$form_state) {
	$entity_tid = $form_state['entity_tid'];
	$customfield_set_nid = $form_state['customfield_set_nid'];
  $which = 'customfield_' . $customfield_set_nid;

	$entity_term = taxonomy_term_load($entity_tid);
  $entity_json = sirius_json_get($entity_term);
  sirius_jsonfield_util_results_set($entity_json, $which, $form_state['values'][$which]);
  sirius_json_set($entity_term, $entity_json);
  sirius_jsonfield_log($entity_term, 'survey', 'Custom fields updated.');
  taxonomy_term_save($entity_term);
  drupal_set_message("Record saved.");
}
