<?php

function sirius_training_node_tab_worker_customfields_access($worker_node) {
	// Training admin can access
	if (user_access('sirius training admin')) { return TRUE; }
}

function sirius_training_node_tab_worker_customfields($worker_node) { 
	return drupal_get_form('sirius_training_node_tab_worker_customfields_form', $worker_node);
}

function sirius_training_node_tab_worker_customfields_form($form, &$form_state,$worker_node) {
  $which = 'tc_form';

  $form_state['worker_nid'] = $worker_node->nid;
  $form_state['which'] = $which;

  $contact_definition_json = sirius_contact_definition();
  $contact_definition_form = sirius_jsonfield_util_form_get($contact_definition_json, $which);

  $worker_json = sirius_jsonfield_util_node_json_get($worker_node);
  $worker_results = sirius_jsonfield_util_results_get($worker_json, $which);
  sirius_jsonfield_util_set_defaults($contact_definition_form, $worker_results);

  $form[$which] = $contact_definition_form;
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

function sirius_training_node_tab_worker_customfields_form_submit($form, &$form_state) {
  $which = $form_state['which'];

  $worker_node = node_load($form_state['worker_nid']);
  $worker_json = sirius_jsonfield_util_node_json_get($worker_node);
  sirius_jsonfield_util_results_set($worker_json, $which, $form_state['values'][$which]);
  sirius_jsonfield_util_node_json_set($worker_node, $worker_json);
  sirius_jsonfield_log($worker_node, 'survey', 'Custom fields updated.');
  node_save($worker_node);
}
