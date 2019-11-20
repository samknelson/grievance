<?php

function sirius_contact_definition_get_nid($domain_nid = NULL, $create = FALSE) {
	if (!$domain_nid) { $domain_nid = sirius_domain_nid(); }
	if (!$domain_nid) { return NULL; }

	$type = 'node:contact:field_sirius_json';
	$query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_json_definition');
  $query->fieldCondition('field_sirius_type', 'value', $type);
  $query->fieldCondition('field_sirius_domain', 'target_id', $domain_nid);
  $query->propertyOrderBy('created', 'desc');
  $query->range(0,1);
  $stmt = $query->execute();
  if ($stmt['node']) { 
		return array_keys($stmt['node'])[0];
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
  $node->field_sirius_domain['und'][0]['target_id'] = $domain_nid;
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
	$domain = sirius_domain();
	if (!$domain) { return 'No domain selected.'; }
	$definition_nid = sirius_contact_definition_get_nid($domain->nid, TRUE);
	return drupal_get_form('sirius_json_definition_edit_form', $definition_nid);
}

function sirius_page_contact_definition_preview() {
	$domain = sirius_domain();
	if (!$domain) { return 'No domain selected.'; }
	$definition_nid = sirius_contact_definition_get_nid($domain->nid, TRUE);
	return drupal_get_form('sirius_json_definition_preview_form', $definition_nid);
}

function sirius_page_contact_definition_clone() {
	$domain = sirius_domain();
	if (!$domain) { return 'No domain selected.'; }
	$definition_nid = sirius_contact_definition_get_nid($domain->nid, TRUE);
	return drupal_get_form('sirius_page_contact_definition_clone_form', $definition_nid);
}

function sirius_page_contact_definition_clone_form($form, &$form_state, $definition_nid) {
	$form_state['definition_nid'] = $definition_nid;
	$definition_node = node_load($definition_nid);

	$form['action'] = array(
		'#type' => 'select',
		'#title' => 'Action',
		'#options' => array(
			NULL => t('-- Select --'),
			'export' => 'EXPORT: Export the current fields',
			'import' => 'IMPORT: Import a new set of fields',
			'from_parent' => 'FROM PARENT: Overwrite with the set of fields in the parent domain',
			'to_children' => 'TO CHILDREN: Overwrite with the set of fields in the parent domain',
		),
	);

	$form['export'] = array(
		'#title' => 'Export',
		'#type' => 'textarea',
		'#value' => $definition_node->field_sirius_json['und'][0]['value'],
    '#states' => array(
      'visible' => array(
        array(':input[name="action"]' => array('value' => 'export')),
      )
    ),
    '#description' => t('Copy the field definition for this domain from the box above.'),
	);

	$form['import'] = array(
		'#title' => 'Import',
		'#type' => 'textarea',
    '#states' => array(
      'visible' => array(
        array(':input[name="action"]' => array('value' => 'import')),
      )
    ),
    '#description' => t('Paste the field definition into the box above. This will replace the current field definitions.'),
	);

	$form['from_parent'] = array(
		'#title' => 'Yes, replace the current field definition with definition from the parent domain.',
		'#type' => 'checkbox',
    '#states' => array(
      'visible' => array(
        array(':input[name="action"]' => array('value' => 'from_parent')),
      )
    ),
	);

	$form['to_children'] = array(
		'#title' => 'Yes, use the current field definition for all subdomains.',
		'#type' => 'checkbox',
    '#states' => array(
      'visible' => array(
        array(':input[name="action"]' => array('value' => 'to_children')),
      )
    ),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Submit',
	);

	return $form;
}
