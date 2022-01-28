<?php

function sirius_event_page_catalog_list() {
	$domain_nid = sirius_domain_nid();
	$type = 'sirius_event:catalog';

  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_json_definition');
  $query->fieldCondition('field_sirius_type', 'value', $type);
  if ($domain_nid) { $query->fieldCondition('field_sirius_domain', 'target_id', $domain_nid); }
  $stmt = $query->execute();
  $nids = array();
  if ($stmt['node']) { $nids = array_keys($stmt['node']); }

  $nodes = node_load_multiple($nids);

  if ($nodes) {
	  $rows = array();
	  foreach ($nodes as $nid => $node) {
	  	$tools = '';
	  	$tools .= l('View', "node/$nid", array('attributes' => array('class' => array('tool-button'))));
	  	$tools .= l('Settings', "node/$nid/sirius_event_catalog_settings", array('attributes' => array('class' => array('tool-button'))));

	  	$json = sirius_jsonfield_util_node_json_get($node);

	  	$row = array();
	  	$row[] = $node->title;
	  	$row[] = $tools;
	  	$rows[] = $row;
	  }

	  $header = array(
	  	t('Title'),
	  	t('Tools'),
	  );

	  $html .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array()));
	} else { 
		$html .= '<p>' . t('No custom field sets have been created yet.') . '</p>';
	}

	$html .= l('Add a New Catalog', 'node/add/sirius-json-definition', array('attributes' => array('class' => array('tool-button')), 'query' => array('sirius_type' => $type)));

	return $html;
}
