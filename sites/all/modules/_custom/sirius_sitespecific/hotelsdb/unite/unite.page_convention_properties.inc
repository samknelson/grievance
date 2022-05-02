<?php

function unite_node_tab_convention_properties($convention_center_node) {
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'property');
  $query->fieldCondition('field_convention_center', 'target_id', $convention_center_node->nid);
  $query->propertyOrderBy('title');
  $query->range(0,100);
  $stmt = $query->execute();
  if (!$stmt['node']) {
  	return 'No properties found.';
  }
  $property_nodes = node_load_multiple(array_keys($stmt['node']));

  if (count($property_nodes) >= 99) {
  	$html .= '<p><em>' . t('Only showing the first 100 properties. Use the advanced search to see all properties.') . '</em></p>';
  }

  $rows = array();
  $header = array(
  	t('Title'),
  	t('Tools'),
  );
  foreach ($property_nodes as $property_nid => $property_node) {
  	$row = array();
  	$row[] = $property_node->title;
  	$row[] = l('View', 'node/' . $property_node->nid, array('attributes' => array('class' => array('tool-button'))));
  	$rows[] = $row;
  }

  $html .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array()));

  return $html;

}