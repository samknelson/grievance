<?php

function grievance_report_open_by_specialist() {
	return drupal_get_form('grievance_report_open_by_specialist_form');
}

function grievance_report_open_by_specialist_form($form, &$form_state) {
  $form['organizer'] = array(
    '#type' => 'entityreference',
    '#title' => t('Organizer'),
    '#era_entity_type' => 'user',
    '#era_bundles' => array(),
    '#era_query_settings' => array(),
    '#description' => t('Leave blank to select all organizers.'),
    '#prefix' => '<div class="form-start">',
  );

  $form['shop'] = array(
    '#type' => 'textfield',
    '#title' => t('Shop'),
    '#description' => t('Enter all or part of the shop name.  Leave blank to select all shops.'),
  );

  $form['shop_match'] = array(
    '#type' => 'select',
    '#title' => t('Exact match on shop name?'),
    '#options' => array('No' => 'No', 'Yes' => 'Yes'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Generate Report',
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

	$shop = $form_state['values']['shop'];
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

	// Find the shops
	$shops = array();
	if ($shop || $shop_match) {
		$query = new EntityFieldQuery();
		$query->entityCondition('entity_type', 'node');
		$query->entityCondition('bundle', 'grievance_shop');
		if ($shop_match == 'Yes') {
			$query->propertyCondition('title', $shop);
		} else {
			$query->propertyCondition('title', $shop, 'CONTAINS');
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

	$results = '';
	foreach ($organizers as $organizer_uid) {
		$tmp = views_embed_view('grievance_open_report', 'page', $organizer_uid, $shop_str);
		if (preg_match('/table/', $tmp)) {
			$organizer = user_load($organizer_uid);
			$results .= '<h2>' . $organizer->name . '</h2>' . $tmp;
		}
	}
	$form_state['results'] = $results;
}

