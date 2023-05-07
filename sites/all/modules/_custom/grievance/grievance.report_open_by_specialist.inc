<?php

/**
 * This is all unused now. The view at the bottom of the page should be committed!
 */

function grievance_report_open_by_specialist() {
	return drupal_get_form('grievance_report_open_by_specialist_form');
}

function grievance_report_open_by_specialist_form($form, &$form_state) {
	$broughtby_options = sirius_taxonomy_load('grievance_broughtby', 'tid', 'name', TRUE);

	$form['prefix'] = array('#markup' => '<div class="form-start">');

	if (count($broughtby_options) > 1) {
	  $form['broughtby_tid'] = array(
	    '#type' => 'select',
	    '#title' => t('Brought By'),
	    '#options' => $broughtby_options,
	  );
	}

  $form['organizer'] = array(
    '#type' => 'entityreference',
    '#title' => t('Organizer'),
    '#era_entity_type' => 'user',
    '#era_bundles' => array(),
    '#era_query_settings' => array(),
    '#description' => t('Leave blank to select all organizers.'),
  );

  $form['company'] = array(
    '#type' => 'textfield',
    '#title' => t('Company'),
    '#description' => t('Enter all or part of the copmany name.  Leave blank to select all companies.'),
  );

  $form['shop'] = array(
    '#type' => 'textfield',
    '#title' => t('Shop'),
    '#description' => t('Enter all or part of the shop name.  Leave blank to select all shops.'),
  );

  $form['shop_match'] = array(
    '#type' => 'select',
    '#title' => t('Exact match on shop name?'),
    '#options' => array('No' => t('No'), 'Yes' => t('Yes')),
  );

  $form['classaction'] = array(
    '#type' => 'select',
    '#title' => t('General Grievances Only?'),
    '#options' => array(NULL => t('Both Individual and General'), 'No' => t('Individual Grievances Only'), 'Yes' => t('General / Class Action Grievances Only')),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Generate Report'),
    '#suffix' => '</div>',
  );

  if ($form_state['results']) {
  	$form['results'] = array(
  		'#type' => 'markup',
  		'#markup' => $form_state['results'],
  	);
  }

  return $form;
}

function grievance_report_open_by_specialist_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;

	$company = trim($form_state['values']['company']);
	$shop = trim($form_state['values']['shop']);
	$shop_match = $form_state['values']['shop_match'];
	$organizer = $form_state['values']['organizer']['entity_id'];

	// Find the organizers
	$organizers = array();
	if ($organizer) {
		$organizers = array($organizer);
	} else {
		$query = new EntityFieldQuery();
		$query->entityCondition('entity_type', 'user');
		$query->propertyOrderBy('name');
		$stmt = $query->execute();
		if (isset($stmt['user'])) {
			$organizers = array_keys($stmt['user']);
		}
	}

	// Find the copmanies
	$companies = array();
	if ($company) {
		$query = new EntityFieldQuery();
		$query->entityCondition('entity_type', 'node');
		$query->entityCondition('bundle', 'grievance_company');
		$query->propertyCondition('title', $company, 'CONTAINS');
		$query->propertyOrderBy('title');
		$stmt = $query->execute();
		if ($stmt['node']) {
			$companies = array_keys($stmt['node']);
		}
		if (!$companies) {
			drupal_set_message("No matching companies.");
			return;
		}
	}

	// Find the shops
	$shops = array();
	if ($company || $shop || $shop_match) {
		$query = new EntityFieldQuery();
		$query->entityCondition('entity_type', 'node');
		$query->entityCondition('bundle', 'grievance_shop');
		if ($companies) {
			$query->fieldCondition('field_grievance_company', 'target_id', $companies, 'in');
		}
		if ($shop) {
			if ($shop_match == 'Yes') {
				$query->propertyCondition('title', $shop);
			} else {
				$query->propertyCondition('title', $shop, 'CONTAINS');
			}
		}
		$query->propertyOrderBy('title');
		$stmt = $query->execute();
		if ($stmt['node']) {
			$shops = array_keys($stmt['node']);
		}
	}
	if ($shops) {
		$shop_str = join('+', $shops);
	}

	$classaction = $form_state['values']['classaction'];
	if (!$classaction) { $classaction = 'all'; }
	$broughtby_tid = $form_state['values']['broughtby_tid'];
	if (!$broughtby_tid) { $broughtby_tid = 'all'; }

	$results = '';
	foreach ($organizers as $organizer_uid) {
		$tmp = views_embed_view('grievance_open_report', 'page', $organizer_uid, $shop_str, $classaction, $broughtby_tid);
		if (preg_match('/table/', $tmp)) {
			$organizer = user_load($organizer_uid);
			$results .= '<h2>' . $organizer->name . '</h2>' . $tmp;
		}
	}
	$form_state['results'] = $results;
}


<?php

/**
 * This is all unused now. The view at the bottom of the page should be committed!
 */

function grievance_report_open_by_specialist() {
	return drupal_get_form('grievance_report_open_by_specialist_form');
}

function grievance_report_open_by_specialist_form($form, &$form_state) {
	$broughtby_options = sirius_taxonomy_load('grievance_broughtby', 'tid', 'name', TRUE);

	$form['prefix'] = array('#markup' => '<div class="form-start">');

	if (count($broughtby_options) > 1) {
	  $form['broughtby_tid'] = array(
	    '#type' => 'select',
	    '#title' => t('Brought By'),
	    '#options' => $broughtby_options,
	  );
	}

  $form['organizer'] = array(
    '#type' => 'entityreference',
    '#title' => t('Organizer'),
    '#era_entity_type' => 'user',
    '#era_bundles' => array(),
    '#era_query_settings' => array(),
    '#description' => t('Leave blank to select all organizers.'),
  );

  $form['company'] = array(
    '#type' => 'textfield',
    '#title' => t('Company'),
    '#description' => t('Enter all or part of the copmany name.  Leave blank to select all companies.'),
  );

  $form['shop'] = array(
    '#type' => 'textfield',
    '#title' => t('Shop'),
    '#description' => t('Enter all or part of the shop name.  Leave blank to select all shops.'),
  );

  $form['shop_match'] = array(
    '#type' => 'select',
    '#title' => t('Exact match on shop name?'),
    '#options' => array('No' => t('No'), 'Yes' => t('Yes')),
  );

  $form['classaction'] = array(
    '#type' => 'select',
    '#title' => t('General Grievances Only?'),
    '#options' => array(NULL => t('Both Individual and General'), 'No' => t('Individual Grievances Only'), 'Yes' => t('General / Class Action Grievances Only')),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Generate Report'),
    '#suffix' => '</div>',
  );

  if ($form_state['results']) {
  	$form['results'] = array(
  		'#type' => 'markup',
  		'#markup' => $form_state['results'],
  	);
  }

  return $form;
}

function grievance_report_open_by_specialist_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;

	$company = trim($form_state['values']['company']);
	$shop = trim($form_state['values']['shop']);
	$shop_match = $form_state['values']['shop_match'];
	$organizer = $form_state['values']['organizer']['entity_id'];

	// Find the organizers
	$organizers = array();
	if ($organizer) {
		$organizers = array($organizer);
	} else {
		$query = new EntityFieldQuery();
		$query->entityCondition('entity_type', 'user');
		$query->propertyOrderBy('name');
		$stmt = $query->execute();
		if (isset($stmt['user'])) {
			$organizers = array_keys($stmt['user']);
		}
	}

	// Find the copmanies
	$companies = array();
	if ($company) {
		$query = new EntityFieldQuery();
		$query->entityCondition('entity_type', 'node');
		$query->entityCondition('bundle', 'grievance_company');
		$query->propertyCondition('title', $company, 'CONTAINS');
		$query->propertyOrderBy('title');
		$stmt = $query->execute();
		if ($stmt['node']) {
			$companies = array_keys($stmt['node']);
		}
		if (!$companies) {
			drupal_set_message("No matching companies.");
			return;
		}
	}

	// Find the shops
	$shops = array();
	if ($company || $shop || $shop_match) {
		$query = new EntityFieldQuery();
		$query->entityCondition('entity_type', 'node');
		$query->entityCondition('bundle', 'grievance_shop');
		if ($companies) {
			$query->fieldCondition('field_grievance_company', 'target_id', $companies, 'in');
		}
		if ($shop) {
			if ($shop_match == 'Yes') {
				$query->propertyCondition('title', $shop);
			} else {
				$query->propertyCondition('title', $shop, 'CONTAINS');
			}
		}
		$query->propertyOrderBy('title');
		$stmt = $query->execute();
		if ($stmt['node']) {
			$shops = array_keys($stmt['node']);
		}
	}
	if ($shops) {
		$shop_str = join('+', $shops);
	}

	$classaction = $form_state['values']['classaction'];
	if (!$classaction) { $classaction = 'all'; }
	$broughtby_tid = $form_state['values']['broughtby_tid'];
	if (!$broughtby_tid) { $broughtby_tid = 'all'; }

	$results = '';
	foreach ($organizers as $organizer_uid) {
		$tmp = views_embed_view('grievance_open_report', 'page', $organizer_uid, $shop_str, $classaction, $broughtby_tid);
		if (preg_match('/table/', $tmp)) {
			$organizer = user_load($organizer_uid);
			$results .= '<h2>' . $organizer->name . '</h2>' . $tmp;
		}
	}
	$form_state['results'] = $results;
}


$view = new view();
$view->name = 'grievance_open_report';
$view->description = '';
$view->tag = 'default';
$view->base_table = 'node';
$view->human_name = 'Grievance Open Report';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'Open Grievances Report';
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['access']['perm'] = 'grievance view';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'input_required';
$handler->display->display_options['exposed_form']['options']['text_input_required_format'] = 'full_html';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '1000';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['pager']['options']['id'] = '0';
$handler->display->display_options['pager']['options']['quantity'] = '9';
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'nid' => 'nid',
  'field_grievance_rep_organizer' => 'field_grievance_rep_organizer',
  'field_grievance_shop' => 'field_grievance_shop',
  'field_grievance_id' => 'field_grievance_id',
  'field_grievance_status' => 'field_grievance_status',
  'field_grievance_first_name' => 'field_grievance_first_name',
  'field_grievance_last_name' => 'field_grievance_last_name',
  'created' => 'created',
  'field_grievance_class_tid' => 'field_grievance_class_tid',
  'field_grievance_clauseref' => 'field_grievance_clauseref',
  'edit_node' => 'edit_node',
  'title' => 'title',
);
$handler->display->display_options['style_options']['class'] = '';
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'nid' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_grievance_rep_organizer' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_grievance_shop' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_grievance_id' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_grievance_status' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_grievance_first_name' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_grievance_last_name' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'created' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_grievance_class_tid' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_grievance_clauseref' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'edit_node' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'title' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
);
/* Relationship: Content: Status (field_grievance_status) */
$handler->display->display_options['relationships']['field_grievance_status_tid']['id'] = 'field_grievance_status_tid';
$handler->display->display_options['relationships']['field_grievance_status_tid']['table'] = 'field_data_field_grievance_status';
$handler->display->display_options['relationships']['field_grievance_status_tid']['field'] = 'field_grievance_status_tid';
$handler->display->display_options['relationships']['field_grievance_status_tid']['label'] = 'Status Term';
/* Relationship: Entity Reference: Referenced Entity */
$handler->display->display_options['relationships']['field_grievance_rep_organizer_target_id']['id'] = 'field_grievance_rep_organizer_target_id';
$handler->display->display_options['relationships']['field_grievance_rep_organizer_target_id']['table'] = 'field_data_field_grievance_rep_organizer';
$handler->display->display_options['relationships']['field_grievance_rep_organizer_target_id']['field'] = 'field_grievance_rep_organizer_target_id';
$handler->display->display_options['relationships']['field_grievance_rep_organizer_target_id']['label'] = 'Specialist';
/* Relationship: Entity Reference: Referenced Entity */
$handler->display->display_options['relationships']['field_grievance_shop_target_id']['id'] = 'field_grievance_shop_target_id';
$handler->display->display_options['relationships']['field_grievance_shop_target_id']['table'] = 'field_data_field_grievance_shop';
$handler->display->display_options['relationships']['field_grievance_shop_target_id']['field'] = 'field_grievance_shop_target_id';
$handler->display->display_options['relationships']['field_grievance_shop_target_id']['label'] = 'Shop';
/* Relationship: Entity Reference: Referenced Entity */
$handler->display->display_options['relationships']['field_grievance_company_target_id']['id'] = 'field_grievance_company_target_id';
$handler->display->display_options['relationships']['field_grievance_company_target_id']['table'] = 'field_data_field_grievance_company';
$handler->display->display_options['relationships']['field_grievance_company_target_id']['field'] = 'field_grievance_company_target_id';
$handler->display->display_options['relationships']['field_grievance_company_target_id']['relationship'] = 'field_grievance_shop_target_id';
$handler->display->display_options['relationships']['field_grievance_company_target_id']['label'] = 'Company';
/* Field: Content: Nid */
$handler->display->display_options['fields']['nid']['id'] = 'nid';
$handler->display->display_options['fields']['nid']['table'] = 'node';
$handler->display->display_options['fields']['nid']['field'] = 'nid';
$handler->display->display_options['fields']['nid']['exclude'] = TRUE;
/* Field: Field: Organizer */
$handler->display->display_options['fields']['field_grievance_rep_organizer']['id'] = 'field_grievance_rep_organizer';
$handler->display->display_options['fields']['field_grievance_rep_organizer']['table'] = 'field_data_field_grievance_rep_organizer';
$handler->display->display_options['fields']['field_grievance_rep_organizer']['field'] = 'field_grievance_rep_organizer';
$handler->display->display_options['fields']['field_grievance_rep_organizer']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_grievance_rep_organizer']['empty'] = '[No Specialist Assigned]';
$handler->display->display_options['fields']['field_grievance_rep_organizer']['settings'] = array(
  'bypass_access' => 1,
  'link' => 0,
  'field_delimiter' => '',
);
$handler->display->display_options['fields']['field_grievance_rep_organizer']['delta_limit'] = '1';
$handler->display->display_options['fields']['field_grievance_rep_organizer']['delta_offset'] = '0';
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['relationship'] = 'field_grievance_company_target_id';
$handler->display->display_options['fields']['title']['label'] = 'Company';
$handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
/* Field: Content: Employer */
$handler->display->display_options['fields']['field_grievance_shop']['id'] = 'field_grievance_shop';
$handler->display->display_options['fields']['field_grievance_shop']['table'] = 'field_data_field_grievance_shop';
$handler->display->display_options['fields']['field_grievance_shop']['field'] = 'field_grievance_shop';
$handler->display->display_options['fields']['field_grievance_shop']['settings'] = array(
  'link' => 0,
);
/* Field: Content: ID */
$handler->display->display_options['fields']['field_grievance_id']['id'] = 'field_grievance_id';
$handler->display->display_options['fields']['field_grievance_id']['table'] = 'field_data_field_grievance_id';
$handler->display->display_options['fields']['field_grievance_id']['field'] = 'field_grievance_id';
/* Field: Content: Status */
$handler->display->display_options['fields']['field_grievance_status']['id'] = 'field_grievance_status';
$handler->display->display_options['fields']['field_grievance_status']['table'] = 'field_data_field_grievance_status';
$handler->display->display_options['fields']['field_grievance_status']['field'] = 'field_grievance_status';
$handler->display->display_options['fields']['field_grievance_status']['type'] = 'taxonomy_term_reference_plain';
/* Field: Content: First Name */
$handler->display->display_options['fields']['field_grievance_first_name']['id'] = 'field_grievance_first_name';
$handler->display->display_options['fields']['field_grievance_first_name']['table'] = 'field_data_field_grievance_first_name';
$handler->display->display_options['fields']['field_grievance_first_name']['field'] = 'field_grievance_first_name';
/* Field: Content: Last Name */
$handler->display->display_options['fields']['field_grievance_last_name']['id'] = 'field_grievance_last_name';
$handler->display->display_options['fields']['field_grievance_last_name']['table'] = 'field_data_field_grievance_last_name';
$handler->display->display_options['fields']['field_grievance_last_name']['field'] = 'field_grievance_last_name';
/* Field: Content: Post date */
$handler->display->display_options['fields']['created']['id'] = 'created';
$handler->display->display_options['fields']['created']['table'] = 'node';
$handler->display->display_options['fields']['created']['field'] = 'created';
$handler->display->display_options['fields']['created']['label'] = 'Intake Date';
$handler->display->display_options['fields']['created']['date_format'] = 'date_only_sortable';
$handler->display->display_options['fields']['created']['second_date_format'] = 'long';
/* Field: Content: Classification */
$handler->display->display_options['fields']['field_grievance_class_tid']['id'] = 'field_grievance_class_tid';
$handler->display->display_options['fields']['field_grievance_class_tid']['table'] = 'field_data_field_grievance_class_tid';
$handler->display->display_options['fields']['field_grievance_class_tid']['field'] = 'field_grievance_class_tid';
$handler->display->display_options['fields']['field_grievance_class_tid']['type'] = 'taxonomy_term_reference_plain';
/* Field: Content: CBA Provision(s) Violated */
$handler->display->display_options['fields']['field_grievance_clauseref']['id'] = 'field_grievance_clauseref';
$handler->display->display_options['fields']['field_grievance_clauseref']['table'] = 'field_data_field_grievance_clauseref';
$handler->display->display_options['fields']['field_grievance_clauseref']['field'] = 'field_grievance_clauseref';
$handler->display->display_options['fields']['field_grievance_clauseref']['settings'] = array(
  'link' => 0,
  'field_delimiter' => '',
);
$handler->display->display_options['fields']['field_grievance_clauseref']['delta_offset'] = '0';
$handler->display->display_options['fields']['field_grievance_clauseref']['separator'] = '<br>';
/* Field: Global: Custom text */
$handler->display->display_options['fields']['nothing']['id'] = 'nothing';
$handler->display->display_options['fields']['nothing']['table'] = 'views';
$handler->display->display_options['fields']['nothing']['field'] = 'nothing';
$handler->display->display_options['fields']['nothing']['label'] = 'Tools';
$handler->display->display_options['fields']['nothing']['alter']['text'] = '<a href="/node/[nid]" class="tool-button">View</a>';
/* Sort criterion: User: Name */
$handler->display->display_options['sorts']['name']['id'] = 'name';
$handler->display->display_options['sorts']['name']['table'] = 'users';
$handler->display->display_options['sorts']['name']['field'] = 'name';
$handler->display->display_options['sorts']['name']['relationship'] = 'field_grievance_rep_organizer_target_id';
/* Sort criterion: Content: Title */
$handler->display->display_options['sorts']['title']['id'] = 'title';
$handler->display->display_options['sorts']['title']['table'] = 'node';
$handler->display->display_options['sorts']['title']['field'] = 'title';
$handler->display->display_options['sorts']['title']['relationship'] = 'field_grievance_shop_target_id';
/* Sort criterion: Content: ID (field_grievance_id) */
$handler->display->display_options['sorts']['field_grievance_id_value']['id'] = 'field_grievance_id_value';
$handler->display->display_options['sorts']['field_grievance_id_value']['table'] = 'field_data_field_grievance_id';
$handler->display->display_options['sorts']['field_grievance_id_value']['field'] = 'field_grievance_id_value';
/* Filter criterion: Content: Published status */
$handler->display->display_options['filters']['status']['id'] = 'status';
$handler->display->display_options['filters']['status']['table'] = 'node';
$handler->display->display_options['filters']['status']['field'] = 'status';
$handler->display->display_options['filters']['status']['value'] = 1;
$handler->display->display_options['filters']['status']['group'] = 1;
$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
/* Filter criterion: Content: Type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'node';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['value'] = array(
  'grievance' => 'grievance',
);
$handler->display->display_options['filters']['type']['group'] = 1;
/* Filter criterion: Taxonomy term: Unresolved? (field_grievance_open) */
$handler->display->display_options['filters']['field_grievance_open_value']['id'] = 'field_grievance_open_value';
$handler->display->display_options['filters']['field_grievance_open_value']['table'] = 'field_data_field_grievance_open';
$handler->display->display_options['filters']['field_grievance_open_value']['field'] = 'field_grievance_open_value';
$handler->display->display_options['filters']['field_grievance_open_value']['relationship'] = 'field_grievance_status_tid';
$handler->display->display_options['filters']['field_grievance_open_value']['value'] = array(
  1 => '1',
);
$handler->display->display_options['filters']['field_grievance_open_value']['group'] = 1;
/* Filter criterion: Content: Brought By (field_grievance_broughtby) */
$handler->display->display_options['filters']['field_grievance_broughtby_tid']['id'] = 'field_grievance_broughtby_tid';
$handler->display->display_options['filters']['field_grievance_broughtby_tid']['table'] = 'field_data_field_grievance_broughtby';
$handler->display->display_options['filters']['field_grievance_broughtby_tid']['field'] = 'field_grievance_broughtby_tid';
$handler->display->display_options['filters']['field_grievance_broughtby_tid']['group'] = 1;
$handler->display->display_options['filters']['field_grievance_broughtby_tid']['exposed'] = TRUE;
$handler->display->display_options['filters']['field_grievance_broughtby_tid']['expose']['operator_id'] = 'field_grievance_broughtby_tid_op';
$handler->display->display_options['filters']['field_grievance_broughtby_tid']['expose']['label'] = 'Brought By';
$handler->display->display_options['filters']['field_grievance_broughtby_tid']['expose']['operator'] = 'field_grievance_broughtby_tid_op';
$handler->display->display_options['filters']['field_grievance_broughtby_tid']['expose']['identifier'] = 'field_grievance_broughtby_tid';
$handler->display->display_options['filters']['field_grievance_broughtby_tid']['expose']['remember_roles'] = array(
  2 => '2',
);
$handler->display->display_options['filters']['field_grievance_broughtby_tid']['type'] = 'select';
$handler->display->display_options['filters']['field_grievance_broughtby_tid']['vocabulary'] = 'grievance_broughtby';
/* Filter criterion: User: Name */
$handler->display->display_options['filters']['uid']['id'] = 'uid';
$handler->display->display_options['filters']['uid']['table'] = 'users';
$handler->display->display_options['filters']['uid']['field'] = 'uid';
$handler->display->display_options['filters']['uid']['relationship'] = 'field_grievance_rep_organizer_target_id';
$handler->display->display_options['filters']['uid']['value'] = '';
$handler->display->display_options['filters']['uid']['group'] = 1;
$handler->display->display_options['filters']['uid']['exposed'] = TRUE;
$handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
$handler->display->display_options['filters']['uid']['expose']['label'] = 'Organizer';
$handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
$handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
$handler->display->display_options['filters']['uid']['expose']['remember_roles'] = array(
  2 => '2',
);
/* Filter criterion: Content: Title */
$handler->display->display_options['filters']['title_1']['id'] = 'title_1';
$handler->display->display_options['filters']['title_1']['table'] = 'node';
$handler->display->display_options['filters']['title_1']['field'] = 'title';
$handler->display->display_options['filters']['title_1']['relationship'] = 'field_grievance_company_target_id';
$handler->display->display_options['filters']['title_1']['operator'] = 'contains';
$handler->display->display_options['filters']['title_1']['group'] = 1;
$handler->display->display_options['filters']['title_1']['exposed'] = TRUE;
$handler->display->display_options['filters']['title_1']['expose']['operator_id'] = 'title_1_op';
$handler->display->display_options['filters']['title_1']['expose']['label'] = 'Company';
$handler->display->display_options['filters']['title_1']['expose']['description'] = '(substring ok)';
$handler->display->display_options['filters']['title_1']['expose']['operator'] = 'title_1_op';
$handler->display->display_options['filters']['title_1']['expose']['identifier'] = 'title_1';
$handler->display->display_options['filters']['title_1']['expose']['remember_roles'] = array(
  2 => '2',
);
/* Filter criterion: Content: Title */
$handler->display->display_options['filters']['title']['id'] = 'title';
$handler->display->display_options['filters']['title']['table'] = 'node';
$handler->display->display_options['filters']['title']['field'] = 'title';
$handler->display->display_options['filters']['title']['relationship'] = 'field_grievance_shop_target_id';
$handler->display->display_options['filters']['title']['operator'] = 'contains';
$handler->display->display_options['filters']['title']['group'] = 1;
$handler->display->display_options['filters']['title']['exposed'] = TRUE;
$handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
$handler->display->display_options['filters']['title']['expose']['label'] = 'Shop';
$handler->display->display_options['filters']['title']['expose']['description'] = '(substring ok)';
$handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
$handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
$handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
  2 => '2',
);
/* Filter criterion: Content: General Grievance? (field_grievance_classaction) */
$handler->display->display_options['filters']['field_grievance_classaction_value']['id'] = 'field_grievance_classaction_value';
$handler->display->display_options['filters']['field_grievance_classaction_value']['table'] = 'field_data_field_grievance_classaction';
$handler->display->display_options['filters']['field_grievance_classaction_value']['field'] = 'field_grievance_classaction_value';
$handler->display->display_options['filters']['field_grievance_classaction_value']['group'] = 1;
$handler->display->display_options['filters']['field_grievance_classaction_value']['exposed'] = TRUE;
$handler->display->display_options['filters']['field_grievance_classaction_value']['expose']['operator_id'] = 'field_grievance_classaction_value_op';
$handler->display->display_options['filters']['field_grievance_classaction_value']['expose']['label'] = 'General Grievance Only?';
$handler->display->display_options['filters']['field_grievance_classaction_value']['expose']['operator'] = 'field_grievance_classaction_value_op';
$handler->display->display_options['filters']['field_grievance_classaction_value']['expose']['identifier'] = 'field_grievance_classaction_value';
$handler->display->display_options['filters']['field_grievance_classaction_value']['expose']['remember_roles'] = array(
  2 => '2',
);

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'page');
$handler->display->display_options['path'] = 'grievance/report/open';

/* Display: Data export */
$handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
$handler->display->display_options['pager']['type'] = 'none';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['style_plugin'] = 'views_data_export_csv';
$handler->display->display_options['style_options']['provide_file'] = 1;
$handler->display->display_options['style_options']['parent_sort'] = 0;
$handler->display->display_options['style_options']['quote'] = 1;
$handler->display->display_options['style_options']['trim'] = 0;
$handler->display->display_options['style_options']['replace_newlines'] = 0;
$handler->display->display_options['style_options']['newline_token'] = '1';
$handler->display->display_options['style_options']['header'] = 1;
$handler->display->display_options['style_options']['keep_html'] = 0;
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Content: Nid */
$handler->display->display_options['fields']['nid']['id'] = 'nid';
$handler->display->display_options['fields']['nid']['table'] = 'node';
$handler->display->display_options['fields']['nid']['field'] = 'nid';
$handler->display->display_options['fields']['nid']['exclude'] = TRUE;
/* Field: Field: Organizer */
$handler->display->display_options['fields']['field_grievance_rep_organizer']['id'] = 'field_grievance_rep_organizer';
$handler->display->display_options['fields']['field_grievance_rep_organizer']['table'] = 'field_data_field_grievance_rep_organizer';
$handler->display->display_options['fields']['field_grievance_rep_organizer']['field'] = 'field_grievance_rep_organizer';
$handler->display->display_options['fields']['field_grievance_rep_organizer']['label'] = '';
$handler->display->display_options['fields']['field_grievance_rep_organizer']['exclude'] = TRUE;
$handler->display->display_options['fields']['field_grievance_rep_organizer']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_grievance_rep_organizer']['empty'] = '[No Specialist Assigned]';
$handler->display->display_options['fields']['field_grievance_rep_organizer']['settings'] = array(
  'link' => 0,
  'field_delimiter' => '',
);
$handler->display->display_options['fields']['field_grievance_rep_organizer']['delta_limit'] = 'all';
$handler->display->display_options['fields']['field_grievance_rep_organizer']['delta_offset'] = '0';
/* Field: Content: Employer */
$handler->display->display_options['fields']['field_grievance_shop']['id'] = 'field_grievance_shop';
$handler->display->display_options['fields']['field_grievance_shop']['table'] = 'field_data_field_grievance_shop';
$handler->display->display_options['fields']['field_grievance_shop']['field'] = 'field_grievance_shop';
$handler->display->display_options['fields']['field_grievance_shop']['settings'] = array(
  'link' => 0,
);
/* Field: Content: ID */
$handler->display->display_options['fields']['field_grievance_id']['id'] = 'field_grievance_id';
$handler->display->display_options['fields']['field_grievance_id']['table'] = 'field_data_field_grievance_id';
$handler->display->display_options['fields']['field_grievance_id']['field'] = 'field_grievance_id';
/* Field: Content: Status */
$handler->display->display_options['fields']['field_grievance_status']['id'] = 'field_grievance_status';
$handler->display->display_options['fields']['field_grievance_status']['table'] = 'field_data_field_grievance_status';
$handler->display->display_options['fields']['field_grievance_status']['field'] = 'field_grievance_status';
$handler->display->display_options['fields']['field_grievance_status']['type'] = 'taxonomy_term_reference_plain';
/* Field: Content: First Name */
$handler->display->display_options['fields']['field_grievance_first_name']['id'] = 'field_grievance_first_name';
$handler->display->display_options['fields']['field_grievance_first_name']['table'] = 'field_data_field_grievance_first_name';
$handler->display->display_options['fields']['field_grievance_first_name']['field'] = 'field_grievance_first_name';
/* Field: Content: Last Name */
$handler->display->display_options['fields']['field_grievance_last_name']['id'] = 'field_grievance_last_name';
$handler->display->display_options['fields']['field_grievance_last_name']['table'] = 'field_data_field_grievance_last_name';
$handler->display->display_options['fields']['field_grievance_last_name']['field'] = 'field_grievance_last_name';
/* Field: Content: Post date */
$handler->display->display_options['fields']['created']['id'] = 'created';
$handler->display->display_options['fields']['created']['table'] = 'node';
$handler->display->display_options['fields']['created']['field'] = 'created';
$handler->display->display_options['fields']['created']['label'] = 'Intake Date';
$handler->display->display_options['fields']['created']['date_format'] = 'date_only_sortable';
$handler->display->display_options['fields']['created']['second_date_format'] = 'long';
/* Field: Content: Classification */
$handler->display->display_options['fields']['field_grievance_class_tid']['id'] = 'field_grievance_class_tid';
$handler->display->display_options['fields']['field_grievance_class_tid']['table'] = 'field_data_field_grievance_class_tid';
$handler->display->display_options['fields']['field_grievance_class_tid']['field'] = 'field_grievance_class_tid';
$handler->display->display_options['fields']['field_grievance_class_tid']['type'] = 'taxonomy_term_reference_plain';
/* Field: Content: CBA Provision(s) Violated */
$handler->display->display_options['fields']['field_grievance_clauseref']['id'] = 'field_grievance_clauseref';
$handler->display->display_options['fields']['field_grievance_clauseref']['table'] = 'field_data_field_grievance_clauseref';
$handler->display->display_options['fields']['field_grievance_clauseref']['field'] = 'field_grievance_clauseref';
$handler->display->display_options['fields']['field_grievance_clauseref']['settings'] = array(
  'link' => 0,
  'field_delimiter' => '',
);
$handler->display->display_options['fields']['field_grievance_clauseref']['delta_offset'] = '0';
$handler->display->display_options['fields']['field_grievance_clauseref']['separator'] = '<br>';
$handler->display->display_options['path'] = 'grievance/report/open/csv';
$handler->display->display_options['displays'] = array(
  'page' => 'page',
  'default' => 0,
);
$translatables['grievance_open_report'] = array(
  t('Master'),
  t('Open Grievances Report'),
  t('more'),
  t('Apply'),
  t('Reset'),
  t('Sort by'),
  t('Asc'),
  t('Desc'),
  t('Select any filter and click on Apply to see results'),
  t('Items per page'),
  t('- All -'),
  t('Offset'),
  t('« first'),
  t('‹ previous'),
  t('next ›'),
  t('last »'),
  t('Status Term'),
  t('Specialist'),
  t('Shop'),
  t('Company'),
  t('Nid'),
  t('Organizer'),
  t('[No Specialist Assigned]'),
  t('Employer'),
  t('ID'),
  t('Status'),
  t('First Name'),
  t('Last Name'),
  t('Intake Date'),
  t('Classification'),
  t('CBA Provision(s) Violated'),
  t('Tools'),
  t('<a href="/node/[nid]" class="tool-button">View</a>'),
  t('Brought By'),
  t('(substring ok)'),
  t('General Grievance Only?'),
  t('Page'),
  t('Data export'),
);
