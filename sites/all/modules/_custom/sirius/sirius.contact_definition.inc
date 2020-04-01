<?php

function sirius_contact_definition_get_nid($domain_nid = NULL, $create = FALSE) {
	if (!$domain_nid) { $domain_nid = sirius_domain_nid(); }
	// if (!$domain_nid) { return NULL; }

	$type = 'node:contact:field_sirius_json';

	/*
	// EntityFieldQuery doesn't support NULL, dammit.
	$query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_json_definition');
  $query->fieldCondition('field_sirius_type', 'value', $type);
  if ($domain_nid) {
  	$query->fieldCondition('field_sirius_domain', 'target_id', $domain_nid);
  } else {
  	$query->fieldCondition('field_sirius_domain', 'target_id', NULL);
  }
  $query->propertyOrderBy('created', 'desc');
  $query->range(0,1);
  $stmt = $query->execute();
  */
  $sql = "select nid ";
  $sql .= "from node ";
  $sql .= "left join field_data_field_sirius_type on field_data_field_sirius_type.entity_type = 'node' and field_data_field_sirius_type.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
  $sql .= "where node.type = 'sirius_json_definition' ";
  $sql .= "and field_sirius_type_value = :type ";
  $sql_args[':type'] = $type;
  if ($domain_nid) {
  	$sql .= "and field_sirius_domain_target_id = :domain_nid ";
  	$sql_args[':domain_nid'] = $domain_nid;
  } else {
  	$sql .= "and field_sirius_domain_target_id is null ";
  }
  $sql .= "order by created desc limit 1";
  $stmt = db_query($sql, $sql_args);
  $hr = $stmt->fetchAssoc();
  if ($hr['nid']) {
		return $hr['nid'];
  }

	if (!$create) { 
		return NULL;
	}

	global $user;
  $node = new stdClass();
  $node->type = 'sirius_json_definition';
  $node->title = 'Contact Fields';
  $node->no_recurse = TRUE;
  $node->language = LANGUAGE_NONE;
  $node->uid = $user->uid;
  $node->status = 1;
  $node->promote = 0;
  $node->comment = 0;
  $node->field_sirius_type['und'][0]['value'] = 'node:contact:field_sirius_json';
  if ($domain_nid) {
  	$node->field_sirius_domain['und'][0]['target_id'] = $domain_nid;
  }
  $node = node_submit($node);
  node_object_prepare($node);
  node_save($node);
  return $node->nid;
}

function sirius_contact_definition($domain_nid = NULL, $create = FALSE) {
	$definition_nid = sirius_contact_definition_get_nid($domain_nid);
	if ($definition_nid) { $definition_node = node_load($definition_nid); }
	if ($definition_node) {
		return json_decode($definition_node->field_sirius_json['und'][0]['value'], TRUE); 
	}

	return NULL;
}

function sirius_page_contact_definition_edit() {
  $domain_nid = sirius_domain_nid();
  $definition_nid = sirius_contact_definition_get_nid($domain_nid, TRUE);
	return drupal_get_form('sirius_json_definition_edit_form', $definition_nid);
}

function sirius_page_contact_definition_preview() {
  $domain_nid = sirius_domain_nid();
  $definition_nid = sirius_contact_definition_get_nid($domain_nid, TRUE);
	return drupal_get_form('sirius_json_definition_preview_form', $definition_nid);
}

function sirius_page_contact_definition_exim() {
  $domain_nid = sirius_domain_nid();
  $definition_nid = sirius_contact_definition_get_nid($domain_nid, TRUE);
	return drupal_get_form('sirius_json_definition_exim_form', $definition_nid);
}
