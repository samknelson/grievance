<?php

function sirius_customfield_definition_list() {
	$domain_nid = sirius_domain_nid();
	// $type = 'customfield';
	// return views_embed_view('sirius_json_definitions', 'block_1', $domain_nid, $type);

  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_json_definition');
  $query->fieldCondition('field_sirius_type', 'value', 'customfield');
  if ($domain_nid) { $query->fieldCondition('field_sirius_domain', 'target_id', $domain_nid); }
  $stmt = $query->execute();
  $nids = array();
  if ($stmt['node']) { $nids = array_keys($stmt['node']); }

  $nodes = node_load_multiple($nids);

  if ($nodes) {
	  $rows = array();
	  foreach ($nodes as $nid => $node) {
	  	$json = sirius_json_get($node);
	  	$content_types_options = sirius_customfield_content_type_options(sirius_customfield_content_types($json));
	  	$content_types_render = join('<br />', array_values($content_types_options));
	  	$tools = '';
	  	$tools .= l('Form', "node/$nid/sirius_event_participant_definition_form", array('attributes' => array('class' => array('tool-button'))));
	  	$tools .= l('Content Types', "node/$nid/sirius_customfield_content_types", array('attributes' => array('class' => array('tool-button'))));
	  	$tools .= l('Tab', "node/$nid/sirius_customfield_tab", array('attributes' => array('class' => array('tool-button'))));

	  	$json = sirius_jsonfield_util_node_json_get($node);
	  	$machine_name = $json['machine_name'];

	  	$row = array();
	  	$row[] = $node->title;
	  	$row[] = $machine_name;
	  	$row[] = $content_types_render;
	  	$row[] = $tools;
	  	$rows[] = $row;
	  }

	  $header = array(
	  	t('Title'),
	  	t('Machine Name'),
	  	t('Content Types'),
	  	t('Tools'),
	  );

	  $html .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array()));
	} else { 
		$html .= '<p>' . t('No custom field sets have been created yet.') . '</p>';
	}

	$html .= l('Add a New Custom Field Set', 'node/add/sirius-json-definition', array('attributes' => array('class' => array('tool-button')), 'query' => array('sirius_type' => 'customfield')));

	return $html;
}
