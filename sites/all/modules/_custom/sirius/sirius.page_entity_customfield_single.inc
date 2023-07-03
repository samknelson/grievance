<?php 

function sirius_node_tab_entity_customfield_single($entity_node, $set_node) {
	return drupal_render(drupal_get_form('sirius_node_tab_entity_customfield_form', $entity_node, $set_node));
}

function sirius_node_tab_entity_customfield_form($form, &$form_state, $entity_node, $set_node) {
  $form_state['entity_nid'] = $entity_node->nid;
  $form_state['set_nid'] = $set_node->nid;
  $set_json = sirius_json_get($set_node);

  $customfield_form = sirius_jsonfield_util_form_get($set_json, 'default');
  if (!$customfield_form) { return sirius_form_error("The form for this custom field set has not been configured. It can be set up " . l('here.', 'sirius/customfield/definition/list')); }

  $which = 'customfield_' . $set_node->nid;
  $entity_json = sirius_json_get($entity_node);
  $entity_results = sirius_jsonfield_util_results_get($entity_json, $which);
  sirius_jsonfield_util_set_defaults($customfield_form, $entity_results);

  $form[$which] = $customfield_form;
  $form['#tree'] = TRUE;

  $form['!intro'] = array(
    '#markup' => $set_json['customfield']['tab']['headfoot']['head'] . '<div class="sirius-form">',
    '#weight' => -9999,
  );

  $form['!outro'] = array(
    '#markup' => '</div>' . $set_json['customfield']['tab']['headfoot']['foot'],
    '#weight' => 9999,
  );

  // Do we have "edit" permission?
  $context = array('set_node' => $set_node, 'entity_node' => $entity_node, 'data_node' => $entity_node);
  if (sirius_customfield_access('edit', $context)) {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
      '#weight' => 10000,
    );
  } else {
    $form['#disabled'] = TRUE;
  }

  return $form;
}

function sirius_node_tab_entity_customfield_form_submit($form, &$form_state) {
	$entity_nid = $form_state['entity_nid'];
	$set_nid = $form_state['set_nid'];
  $which = 'customfield_' . $set_nid;

	$entity_node = node_load($entity_nid);
  $entity_json = sirius_json_get($entity_node);
  sirius_jsonfield_util_results_set($entity_json, $which, $form_state['values'][$which]);
  sirius_json_set($entity_node, $entity_json);
  sirius_jsonfield_log($entity_node, 'survey', 'Custom fields updated.');
  node_save($entity_node);
  drupal_set_message("Record saved.");
}
