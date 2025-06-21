<?php

function unite_node_tab_property_colocation_form($form, &$form_state, $property_node) {
	return sirius_form_error("Under development");
	$form['#tree'] = TRUE;
	$form_state['property_nid'] = $property_node->nid;

	$pa_nodes = array();
	$query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'property_actor');
  $query->fieldCondition('field_property', 'target_id', $property_node->nid);
  $query->fieldCondition('field_relationship_type', 'value', 'Colocation Host');
  $stmt = $query->execute();
  if ($stmt['node']) {
  	$pa_nodes = node_load_multiple(array_keys($stmt['node']));
  	$pa_node = reset($pa_nodes);
	}
}

/*
function unite_node_tab_property_colocation_form($form, &$form_state, $property_node) {
	$form['#tree'] = TRUE;
	$form_state['property_nid'] = $property_node->nid;

	$pa_nodes = array();
	$query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'property_actor');
  $query->fieldCondition('field_property', 'target_id', $property_node->nid);
  $query->fieldCondition('field_relationship_type', 'value', 'Colocation Host');
  $stmt = $query->execute();
  if ($stmt['node']) {
  	$pa_nodes = node_load_multiple(array_keys($stmt['node']));
  	$pa_node = reset($pa_nodes);
	}

	if ($pa_node) {
		$actor_node = node_load($pa_node->field_actor['und'][0]['target_id']);
		$html = '<p>' . 'This property is configured as a colocation host.' . '<br />';
		$html .= l('View Actor Record', 'node/' . $actor_node->nid, array('attributes' => array('class' => array('tool-button'))));
		$html .= '</p>';
		$form['markup'] = array('#markup' => $html);
		return $form;
	}

	$html = '<p>' . t('This property is not configured as a colocation host. Click below to create an actor record and relationship which will allow other properties, such as restaurants, to be colocated at this property.') . '</p>';
	$form['markup'] = array('#markup' => $html);

	$form['confirm'] = array(
		'#type' => 'checkbox',
		'#title' => 'Yes, I\'m sure',
		'#required' => TRUE
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function unite_node_tab_property_colocation_form_submit($form, &$form_state) {
	drupal_set_message("Under development", 'warning');
}
*/