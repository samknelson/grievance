<?php 

function sirius_trust_node_tab_policy_service_rates_access($policy_node) {
	return $policy_node->field_sirius_type['und'][0]['value'] == 'sirius_ledger:policy';
}

function sirius_trust_node_tab_policy_service_rates($policy_node) {
	return drupal_get_form('sirius_trust_node_tab_policy_service_rates_form', $policy_node);
}

function sirius_trust_node_tab_policy_service_rates_form($form, &$form_state, $policy_node) {
	$form_state['policy_nid'] = $policy_node->nid;
	$policy_json = sirius_json_get($policy_node);

  drupal_add_css(drupal_get_path('module', 'sirius_event') . '/css/event.css', 'file');
  $form['#tree'] = TRUE;
	$items = $policy_json['trust']['service']['rates'];

	$provider_levels = sirius_taxonomy_load('sirius_trust_provider_level', 'tid', 'name');

	// ==== START ADDONE TEMPLATE ==== 
  $ajax_params = $form_state['triggering_element']['#sirius_ajax'];
	if (!$items) { $items = array(); }
	uksort($items, function($a, $b) use ($items) { return $items[$a]['weight'] <=> $items[$b]['weight']; });
	$items = array_values($items);

	if ($form_state['items_count']) {
		$items_count = $form_state['items_count'];
	} else {
		$items_count = count($items);
	}
  $items_count += $ajax_params['qty'];
  if ($items_count < 1) { $items_count = 1; }
  $form_state['items_count'] = $items_count;

  for ($i=count($items); $i<$items_count; ++$i) { $items[] = array(); }

	$form['items']['prefix'] = array('#markup' => '<div id="items">');
	foreach ($items as $delta => $item) {
		$uuid = $item['uuid'];
    $default_weight = $item['weight'];
    if (!$default_weight) { $default_weight = 10 * ($delta+1); }

		$title = sirius_ledger_policy_rate_title($item, $lookups);

		$accordion_title = $title ."<span class=\"sirius_event_weight_indicator\">Sort Order: $default_weight</span>";

    $form['items'][$delta] = array(
    	'#type' => 'fieldset',
    	'#title' => $accordion_title,
    	'#collapsible' => TRUE,
    	'#collapsed' => TRUE,
    );

    $form['items'][$delta]['uuid'] = array(
    	'#type' => 'value',
    	'#value' => $item['uuid'],
    );

		// ==== END ADDONE TEMPLATE ==== 

    $form['items'][$delta]['title'] = array(
    	'#type' => 'textfield',
    	'#title' => t('Title for this Rate Rule'),
    	'#default_value' => $title,
    );

    $form['items'][$delta]['rate'] = array(
    	'#type' => 'textfield',
    	'#title' => t('Rate'),
    	'#size' => 5,
    	'#default_value' => $item['rate'],
    );

    $form['items'][$delta]['service_ids'] = array(
    	'#type' => 'textarea',
    	'#title' => t('Service IDs / CPT Codes'),
    	'#size' => 5,
    	'#default_value' => $item['service_ids'],
    	'#description' => t('Enter service IDs, one per line'),
    );

    if ($provider_levels) {
	    $form['items'][$delta]['provider_levels'] = array(
	    	'#type' => 'checkboxes',
	    	'#title' => t('Provider Levels'),
	    	'#default_value' => sirius_form_array_default_value($item['provider_levels']),
	    	'#options' => $provider_levels,
	    );
	  }

		// ==== START ADDONE TEMPLATE ==== 

    $form['items'][$delta]['delete'] = array(
      '#type' => 'select',
      '#title' => t('Delete this item'),
      '#options' => array(
        NULL => t('No'),
        'Yes' => t('Yes.')
      ),
      '#description' => t('The item will be removed.'),
    );

    $form['items'][$delta]['weight'] = array(
      '#type' => 'textfield',
      '#title' => 'Sort Order',
      '#default_value' => $default_weight,
      '#size' => 4,
    );
  }

	$form['items']['suffix'] = array('#markup' => '</div>');

	form_load_include($form_state, 'inc', 'sirius_trust', 'sirius_trust.page_policy_service_rates');

  $form['add_one'] = array(
    '#type' => 'button',
    '#value' => t('Add one'),
    '#ajax' => array(
      'callback' => 'sirius_ajax_generic_replace',
    ),
    '#sirius_ajax' => array(
    	'qty' => 1,
    	'wrapper' => '#items',
    	'element' => 'items',
    ),
  );

	// ==== END ADDONE TEMPLATE ==== 

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_trust_node_tab_policy_service_rates_form_validate($form, &$form_state) {
	foreach ($form_state['values']['items'] as $delta => $item) {
		$service_ids = sirius_trust_service_rate_ids($item);
		foreach ($service_ids as $service_id) {
			if (!sirius_find_nid_by_id($service_id, 'sirius_trust_service')) {
				form_set_error("items][$delta][service_id", "The id [$service_id] does not correspond to a valid service.");
			}
		}
	}
}

function sirius_trust_service_rate_ids($item) {
	$candidates = explode("\n", $item['service_ids']);
	$result = array();
	foreach ($candidates as $candidate) {
		$candidate = trim($candidate);
		if (!$candidate) { continue; } 
		$result[] = $candidate;
	}
	return $result;
}

function sirius_trust_node_tab_policy_service_rates_form_submit($form, &$form_state) {
	$policy_node = node_load($form_state['policy_nid']);
	$policy_json = sirius_json_get($policy_node);

	// ==== START ADDONE TEMPLATE ==== 
	$items = array();
	$items_input = $form_state['values']['items'];
	if (!$items_input) { $items_input = array(); }

	foreach ($items_input as $item) {
		if (!$item['rate'] && !$item['account']) { continue; }
    if ($item['delete']) { continue; }
		$uuid = $item['uuid'];
		if (!$uuid) { 
			$uuid = uuid_generate();
			$item['uuid'] = $uuid;
		}
		if (!$item['title']) { $item['title'] = sirius_ledger_policy_rate_title($item, $form_state['lookups']); }
		$items[$uuid] = $item;
	}
	// ==== END ADDONE TEMPLATE ==== 


	$policy_json['trust']['service']['rates'] = $items;
	sirius_json_set($policy_node, $policy_json);
	node_save($policy_node);
	drupal_set_message("Settings saved.");
}
