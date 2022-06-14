<?php 

function sirius_ledger_node_tab_policy_rates_new_access($policy_node) {
	return sirius_ledger_node_tab_policy_definition_access($policy_node);
}

function sirius_ledger_node_tab_policy_rates_new($policy_node) {
	return drupal_get_form('sirius_ledger_node_tab_policy_rates_new_form', $policy_node);
}

function sirius_ledger_node_tab_policy_rates_new_form($form, &$form_state, $policy_node) {
	$form_state['policy_nid'] = $policy_node->nid;
	$policy_json = sirius_json_get($policy_node);

  drupal_add_css(drupal_get_path('module', 'sirius_event') . '/css/event.css', 'file');
  $form['#tree'] = TRUE;
	$items = $policy_json['ledger']['policy']['rates_new'];

	$account_options = sirius_ledger_account_list();
	$schedule_options = array('month' => 'Month', 'semimonth' => 'Semi Month', 'hour' => 'Hour');
	$ms_level_options = sirius_ledger_ms_level_options();
	$ws_options = sirius_taxonomy_load('sirius_work_status', 'tid', 'name');
 	if (sirius_domain_component_enabled('trust')) {
		$election_type_options = sirius_trust_policy_election_types($policy_node);
		$benefit_options = sirius_trust_policy_benefits_offered_options($policy_node);
	}
	if ($account_options['pay']) {
		$skill_options = array('all' => 'All Pay Rates', 'skill' => 'Skilled', 'unskilled' => 'Unskilled');
	}
	$trigger_options = sirius_ledger_policy_trigger_options($schedule_key);

	$lookups = array(
		'account_options' => $account_options,
		'schedule_options' => $schedule_options,
		'ms_level_options' => $ms_level_options,
		'ws_options' => $ws_options,
		'election_type_options' => $election_type_options,
		'benefit_options' => $benefit_options,
		'skill_options' => $skill_options,
	);
	$form_state['lookups'] = $lookups;

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

    $form['items'][$delta]['schedule'] = array(
    	'#type' => 'select',
    	'#title' => t('Schedule'),
    	'#options' => $schedule_options,
    	'#default_value' => $item['schedule'],
    );

	  if ($trigger_options) {
			$form['items'][$delta]['triggers'] = array(
	      '#type' => 'checkboxes',
	      '#title' => t('Triggers'),
	      '#options' => $trigger_options,
	      '#default_value' => sirius_form_array_default_value($item['triggers']),
	    );
		}

    $form['items'][$delta]['account'] = array(
    	'#type' => 'select',
    	'#title' => t('Account'),
    	'#options' => $account_options,
    	'#default_value' => $item['account'],
    );

    if ($skill_options) {
	    $form['items'][$delta]['skill'] = array(
	    	'#type' => 'select',
	    	'#title' => t('Skill'),
	    	'#options' => $skill_options,
	    	'#default_value' => $item['skill'],
	    );
		}

		if ($ws_options) {
	    $form['items'][$delta]['ws'] = array(
	    	'#type' => 'checkboxes',
	    	'#title' => t('Work Status'),
	    	'#options' => $ws_options,
	    	'#checkall' => TRUE,
	    	'#default_value' => sirius_form_array_default_value($item['ws']),
	    );
	  }

		if ($ms_level_options) {
	    $form['items'][$delta]['ms_level'] = array(
	    	'#type' => 'checkboxes',
	    	'#title' => t('Member Status'),
	    	'#options' => $ms_level_options,
	    	'#checkall' => TRUE,
	    	'#default_value' => sirius_form_array_default_value($item['ms_level']),
	    );
	  }

    if ($election_type_options) {
	    $form['items'][$delta]['election_type'] = array(
	    	'#type' => 'checkboxes',
	    	'#title' => t('Election Type'),
	    	'#options' => $election_type_options,
	    	'#checkall' => TRUE,
	    	'#default_value' => $item['election_type'],
	    );
	  }

    if ($benefit_options) {
	    $form['items'][$delta]['benefit'] = array(
	    	'#type' => 'checkboxes',
	    	'#title' => t('Benefit'),
	    	'#options' => $benefit_options,
	    	'#checkall' => TRUE,
	    	'#default_value' => $item['benefit'],
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

	form_load_include($form_state, 'inc', 'sirius_ledger', 'sirius_ledger.page_policy_rates_new');

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

function sirius_ledger_node_tab_policy_rates_new_form_submit($form, &$form_state) {
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


	$policy_json['ledger']['policy']['rates_new'] = $items;
	sirius_json_set($policy_node, $policy_json);
	node_save($policy_node);
	drupal_set_message("Settings saved.");
}
