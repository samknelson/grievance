<?php

function sirius_letter_access($action, $entity, $letter_node = NULL) {
	global $user; 

	if ($entity->tid) { $node = sirius_taxonomy_proxy($entity->tid); } else { $node = $entity; } 

	// Verify that this letter belongs to this node or entity
	if ($letter_node) { 
		if ($letter_node->field_sirius_log_handler['und'][0]['target_id'] == $node->nid) { $good = TRUE; }
		if (!$good) {
			$real_parent = node_load($letter_node->field_sirius_log_handler['und'][0]['target_id']);
			if ($real_parent->nid != $node->nid) { 
				return sirius_letter_access($action, $real_parent, $letter_node); 
			}
		}
		if (!$good) { return FALSE; }
	}

	// Verify that the user has access to this letter type
	// @todo

	// Admins can do anything
	if (user_access('sirius admin')) { return TRUE; }
	if (user_access('sirius domain admin')) { return TRUE; }

	// //////////////////////////////////////////////////////
	// Type-specific logic
	// @todo: This should be in callbacks
	// //////////////////////////////////////////////////////

	// Must be able to view the underlying node
	if (!node_access('view', $node)) { return FALSE; }

	if ($node->type == 'sirius_worker') {
		if (user_access('sirius worker admin')) { return TRUE; }
		return FALSE;
	}

	if ($node->type == 'grievance') {
		if (user_access('grievance admin')) { return TRUE; }
		if (user_access('grievance user')) { return TRUE; }

		return FALSE;
	}

	if ($node->type == 'grievance_shop') {
		if (user_access('sirius employer admin')) { return TRUE; }

		return FALSE;
	}

	return FALSE;
}

function sirius_letter_list($entity) {
	if ($entity->tid) { $node = sirius_taxonomy_proxy($entity->tid); } else { $node = $entity; } 
	$query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_log');
  $query->fieldCondition('field_sirius_category', 'value', 'letter');
	$query->fieldCondition('field_sirius_log_handler', 'target_id', $node->nid);
  $query->propertyOrderBy('title');
  $query->range(0,1000);
  $stmt = $query->execute();
  if (!$stmt['node']) { return array(); }

  $all_nodes = node_load_multiple(array_keys($stmt['node']));
  $good_nodes = array();
  foreach ($all_nodes as $nid => $node) {
  	if (!sirius_letter_access('view', $entity, $node)) { continue; }
  	$good_nodes[$nid] = $node;
  }
  return $good_nodes;
}

function sirius_letter_admin() {
  $form = array();

  $name = sirius_domain_variable_name('sirius_letter_content_types');
  $form[$name] = array(
    '#type' => 'checkboxes',
    '#title' => t('Enable letters for these content types'),
    '#options' => sirius_letter_content_types_all(),
  );
  if (variable_get($name)) { $form[$name]['#default_value'] = variable_get($name); }

  return system_settings_form($form);
}

function sirius_letter_content_types_all() {
 return array(
		'sirius_contact' => 'Contact',
		'grievance_shop' => 'Employer',
		'grievance' => 'Grievance',
		'sirius_log' => 'Log (Authorizations, etc.)',
		'sirius_worker' => 'Worker',
	);
}

function sirius_letter_content_types_enabled() { 
	return sirius_domain_variable_get('sirius_letter_content_types');
}

function sirius_letter_format_options($entity) {
	$format_options = array(
		'email' => t('Email'),
		'pdf' => t('PDF Download'),
	);
	if (sirius_fax_enabled()) { $format_options['fax'] = t('Fax'); }
	if (sirius_sms_enabled()) { $format_options['sms'] = t('SMS Text Message'); }
	if (sirius_domain_component_enabled('postal')) { $format_options['postal'] = t('Postal Letter'); }
	return $format_options;
}

function sirius_letter_permission_check($letter_template_node) {
	global $user;

	// Admin can send any letter
	if (user_access('sirius admin')) { return TRUE; }
	if (user_access('sirius domain admin')) { return TRUE; }

	// @todo Legacy - grievance admin can send any letter
	if (user_access('grievance admin')) { return TRUE; }

	// No permissions set, return false
	if (!isset($letter_template_node->field_grievance_roles['und'])) { return FALSE; }

	// Does the user have nay of the specified roles?
	$user_roles = array_keys($user->roles);
	$letter_roles = sirius_fieldvals($letter_template_node, 'field_grievance_roles', 'value');
	if (array_intersect($user_roles, $letter_roles)) { return TRUE; }

	// Apparently not.
	return FALSE;
}

function sirius_letter_template_list($entity_node_or_type) {
	if (is_object($entity_node_or_type)) { $entity_type = $entity_node_or_type->type; } else { $entity_type = $entity_node_or_type; }
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_letter_template');
  $query->fieldCondition('field_sirius_letter_content_type', 'value', $entity_type);
  $domain_nid = sirius_domain_nid();
  if ($domain_nid) { $query->fieldCondition('field_sirius_domain', 'target_id', $domain_nid); }
  $query->propertyOrderBy('title');
  $stmt = $query->execute();
  if (!$stmt['node']) { return array(); }
	$letter_template_nodes = node_load_multiple(array_keys($stmt['node']));

	$letter_template_nodes_with_access = array();
	foreach ($letter_template_nodes as $letter_template_nid => $letter_template_node) {
		if (sirius_letter_permission_check($letter_template_node)) { $letter_template_nodes_with_access[$letter_template_nid] = $letter_template_node; }
  }

  return $letter_template_nodes_with_access;
}

function sirius_letter_pdf_basename($letter_node) {
	$wizard_name = 'letter';
	$info = sirius_wizard_info($wizard_name);
	$data = sirius_wizard_data($info, $letter_node);
	$entity_node = node_load($letter_node->field_sirius_log_handler['und'][0]['target_id']);
	$first_letter_template_node = node_load($data['letter_template_nid_0']);

	$basename = '';

	if ($entity_node->type == 'grievance') {
		if ($entity_node->field_grievance_last_name['und'][0]['value']) {
			$tmp = strtolower($entity_node->field_grievance_last_name['und'][0]['value']);
			$tmp = preg_replace('/[^a-z]/', '', $tmp);
			if ($tmp) { $basename .= $tmp . '_'; }
		}
		if ($entity_node->field_grievance_first_name['und'][0]['value']) {
			$tmp = strtolower($entity_node->field_grievance_first_name['und'][0]['value']);
			$tmp = preg_replace('/[^a-z]/', '', $tmp);
			if ($tmp) { $basename .= $tmp . '_'; }
		}
		if ($entity_node->field_grievance_id['und'][0]['value']) {
			$tmp = strtolower($entity_node->field_grievance_id['und'][0]['value']);
			$tmp = preg_replace('/[^a-z0-9]/', '', $tmp);
			if ($tmp) { $basename .= $tmp . '_'; }
		}
	} else {
		$tmp  = strtolower($entity_node->title);
		$tmp = preg_replace('/[^a-z]/', '', $tmp);
		if ($tmp) { $basename .= $tmp . '_'; }
		if ($entity_node->field_sirius_id['und'][0]['value']) {
			$tmp = strtolower($entity_node->field_sirius_id['und'][0]['value']);
			$tmp = preg_replace('/[^a-z0-9]/', '', $tmp);
			if ($tmp) { $basename .= $tmp . '_'; }
		}
	}

	if ($first_letter_template_node->field_grievance_shortname['und'][0]['value']) {
		$tmp = strtolower($first_letter_template_node->field_grievance_shortname['und'][0]['value']);
		$tmp = preg_replace('/[^a-z0-9]/', '', $tmp);
		if ($tmp) { $basename .= $tmp . '_'; }
	}
	$basename .= date('Ymd');

	return $basename;
}

function sirius_letter_pdf_contents($letter_node) {
	$wizard_name = 'letter';
	$info = sirius_wizard_info($wizard_name);
	$data = sirius_wizard_data($info, $letter_node);
	$entity_node = node_load($letter_node->field_sirius_log_handler['und'][0]['target_id']);
	if (strlen($data['pdf_body']) < 100) { return ''; }

	$url .= sirius_loopback_url() . '/sirius/loopback/echo';
	$params = array();
	$params['post']['body'] = '<div id="grievance-letter-body" class="sirius_pdf_body">' . $data['pdf_body'] . '</div>';
	$params['post']['css_paths'] = 'sites/all/modules/_custom/sirius/css/pdf.css';
	$params['letterhead'] = $data['pdf_letterhead'];
	return sirius_pdf_generate($url, $params);

	$html_path = DRUPAL_ROOT . '/sites/default/files/private/grievance_gform/' . $basename . '.html';
	$pdf_path = DRUPAL_ROOT . '/sites/default/files/private/grievance_gform/' . $basename . '.pdf';
}

function sirius_letter_set_title($letter_node, $status = NULL, $sent_ts = NULL) {
	$wizard_name = 'letter';
	$info = sirius_wizard_info($wizard_name);
	$data = sirius_wizard_data($info, $letter_node);
	$entity_node = node_load($letter_node->field_sirius_log_handler['und'][0]['target_id']);

	if (!$status) { $status = $letter_node->field_sirius_type['und'][0]['value']; }
	if (!$sent_ts) { $sent_ts = $data['sent_ts']; }

	$letter_title = '';
  for ($i=0; $i<5; ++$i) {
  	$letter_template_nid = $data["letter_template_nid_$i"];
		if (!$letter_template_nid) { continue; }
		$letter_template_node = node_load($letter_template_nid);
 		$letter_title .= $letter_template_node->title . ' / ';
	}
	$letter_title .= $status;
	if ($data['sent_ts']) { $letter_title .= ' ' . date('Y-m-d H:i:s', $sent_ts); }

	$letter_node->title = $letter_title;
}

function sirius_letter_create($entity, $save = TRUE) {
	if ($entity->tid) { $entity_node = sirius_taxonomy_proxy($entity->tid); } else { $entity_node = $entity; } 
  global $user;
  $letter_node = new stdClass();
  $letter_node->type = 'sirius_log';
  $letter_node->no_recurse = TRUE;
  node_object_prepare($letter_node);
  $letter_node->language = LANGUAGE_NONE;
  $letter_node->uid = $user->uid;
  $letter_node->status = 1;
  $letter_node->promote = 0;
  $letter_node->comment = 0;
  $letter_node->title = substr(date('c') . " - " . $category . " - " . $type . " - " . $message, 0, 254);
  $letter_node->field_sirius_category['und'][0]['value'] = 'letter';
  $letter_node->field_sirius_type['und'][0]['value'] = 'draft';

  $domain_nid = sirius_domain_nid();
  if ($domain_nid) { $letter_node->field_sirius_domain['und'][0]['target_id'] = $domain_nid; } 
  $letter_node->field_sirius_log_handler['und'][0]['target_id'] = $entity_node->nid;

  $letter_node = node_submit($letter_node);
  node_object_prepare($letter_node);
  if ($save) { node_save($letter_node); }
  return $letter_node;
}

function sirius_letter_body($letter_node, $data, $files, $save = TRUE) {

}