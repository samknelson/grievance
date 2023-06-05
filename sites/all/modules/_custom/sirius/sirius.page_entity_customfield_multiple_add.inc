<?php

function sirius_node_tab_entity_customfield_multiple_add_form($form, &$form_state, $entity_node, $set_node) {
  $form['quicknav'] = array('#markup' => sirius_node_tab_entity_customfield_multiple_quicknav($entity_node, $set_node));

  $form_state['entity_nid'] = $entity_node->nid;
  $form_state['set_nid'] = $set_node->nid;
  $set_json = sirius_json_get($set_node);

  $set_form = sirius_jsonfield_util_form_get($set_json, 'default');
  if (!$set_form) { return sirius_form_error("The form for this custom field set has not been configured. It can be set up " . l('here.', 'sirius/customfield/definition/list')); }

  $which = 'customfield_' . $set_node->nid;

  $form[$which] = $set_form;
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

function sirius_node_tab_entity_customfield_multiple_add_form_submit($form, &$form_state) {
	$entity_node = node_load($form_state['entity_nid']);
	$set_node = node_load($form_state['set_nid']);
	$json = array();
	$which = 'customfield_' . $set_node->nid;

	$category = 'customfield';
	$type = $set_node->nid;
	$comment = $set_node->title . ' - ' . $entity_node->title;
	$files = array();
	$data_node = sirius_log($category, $type, $comment, $comment, NULL, array($entity_node->nid), $files);

	$data_json = sirius_json_get($data_node);
  sirius_jsonfield_util_results_set($data_json, $which, $form_state['values'][$which]);
  sirius_json_set($data_node, $data_json);
  $data_node->title = substr($entity_node->title . ' - ' . $set_node->title . ' - ' . date('m/d/Y h:i a'), 0, 253);
  node_save($data_node);

	drupal_set_message("Record saved.");
  drupal_goto("node/" . $data_node->nid . "/sirius_log_customfield_multiple_edit");
}