<?php

function sirius_ledger_node_tab_entity_auths_access($entity_node) {
	return TRUE;
}

function sirius_ledger_node_tab_entity_auths($entity_node) {
	$domain_nid = sirius_domain_nid();

	$action = sirius_node_tab_arg(0);

	if ($action == 'add') { return drupal_get_form('sirius_ledger_node_tab_entity_auths_add_form', $entity_node);}

	return sirius_ledger_node_tab_entity_auths_list($entity_node);
}

function sirius_ledger_node_tab_entity_auths_list($entity_node) {
	$domain_nid = sirius_domain_nid();
	// $html = sirius_ledger_node_tab_entity_auths_quicknav($entity_node);
	$accounts = sirius_ledger_account_list();

  $domain_nid = sirius_domain_nid();
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_log');
  if ($domain_nid) { $query->fieldCondition('field_sirius_domain', 'target_id', $domain_nid); }
  $query->fieldCondition('field_sirius_category', 'value', 'ledger:auth');
  $query->fieldCondition('field_sirius_log_handler', 'target_id', $entity_node->nid);
  $query->propertyOrderBy('nid', 'desc');
  $stmt = $query->execute();
  if ($stmt['node']) {
  	$auth_nodes = node_load_multiple(array_keys($stmt['node']));
		$rows = array();
		foreach ($auth_nodes as $auth_nid => $auth_node) {
			$auth_json = sirius_jsonfield_util_node_json_get($auth_node);

			$esignature_status = sirius_esignature_entity_status($auth_node);

			$tools = '';
			$tools .= l('View', "node/$auth_nid/sirius_log_auth", array('attributes' => array('class' => array('tool-button'))));
			$tools .= l('Items', "node/$auth_nid/sirius_log_auth_items", array('attributes' => array('class' => array('tool-button'))));
			$tools .= l('Letters', "node/$auth_nid/sirius_log_letter", array('attributes' => array('class' => array('tool-button'))));
			$tools .= l('Signatures', "node/$auth_nid/sirius_log_signatures", array('attributes' => array('class' => array('tool-button'))));

			$row = array();
			$row[] = date('Y-m-d', $auth_node->created); 
			$row[] = $esignature_status['status_msg'];
			$row[] = $tools;
			$rows[] = $row;
		}

		$header = array('Date', 'Signature Status', 'Tools');
		$html .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('sirius_materials'))));
  }

	$html .= l('Add', current_path() . '/add', array('attributes' => array('class' => array('tool-button'))));

  return $html;
}

function sirius_ledger_node_tab_entity_auths_add_form($form, &$form_state, $entity_node) {
	$form_state['entity_nid'] = $entity_node->nid;

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Create a New Authorization Form'),
	);

	return $form;
}

function sirius_ledger_node_tab_entity_auths_add_form_submit($form, &$form_state) {
	$entity_nid = $form_state['entity_nid'];
	$auth_node = sirius_log('ledger:auth', 'draft', 'New', 'New', NULL, array($entity_nid), array(), array());

	sirius_ledger_auth_set_title($auth_node);
	
	drupal_set_message(t('Authorization added.'));
	drupal_goto('node/' . $auth_node->nid);
}
