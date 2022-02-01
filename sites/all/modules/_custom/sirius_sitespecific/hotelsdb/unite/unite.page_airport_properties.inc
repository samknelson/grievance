<?php

function unite_node_tab_airport_properties($airport_node) {
	foreach (array('field_airport' => 'Primary Airport', 'field_airport_alt' => 'Secondary Airport') as $field => $label) {
		$html .= "<h2>$label</h2>";

	  $query = new EntityFieldQuery();
	  $query->entityCondition('entity_type', 'node');
	  $query->entityCondition('bundle', 'property');
	  $query->fieldCondition($field, 'target_id', $airport_node->nid);
	  $query->propertyOrderBy('title');
	  $query->range(0,100);
	  $stmt = $query->execute();
	  if (!$stmt['node']) {
	  	$html .= 'No properties found.';
	  	continue;
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
	}

  return $html;

}