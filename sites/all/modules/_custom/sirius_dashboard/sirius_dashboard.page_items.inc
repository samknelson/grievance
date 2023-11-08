<?php

function sirius_dashboard_page_items($rid) {
	return drupal_get_form('sirius_dashboard_page_items_form', $rid);
}

function sirius_dashboard_page_items_form($form, &$form_state, $rid) {
  drupal_add_css(drupal_get_path('module', 'sirius_event') . '/css/event.css', 'file');

	$form_state['rid'] = $rid;

	$roles = user_roles(TRUE);
	drupal_set_title("Dashboard Items by Role: " . $roles[$rid]);

	sirius_backlink_add('sirius/dashboard/items_by_role', array('name' => 'All Roles'));
	sirius_backlink_add('sirius/dashboard', array('name' => 'Dashboard'));
	$json = sirius_dashboard_items_by_role($rid);

	$intro = '<p>The following dashboard items will appear for all users with the role <strong>' . $roles[$owner_id] . '</strong>.</p>';
	$form['intro'] = array('#markup' => $intro);

  $ajax_params = $form_state['triggering_element']['#sirius_ajax'];

	$item_list = $json['item_list'];
	if (!$item_list) { $item_list = array(); }
	uksort($item_list, function($a, $b) use ($item_list) { return $item_list[$a]['weight'] <=> $item_list[$b]['weight']; });
	$item_list = array_values($item_list);

	if ($form_state['item_count']) {
		$item_count = $form_state['item_count'];
	} else {
		$item_count = count($item_list);
	}
  $item_count += $ajax_params['qty'];
  if ($item_count < 1) { $item_count = 1; }
  $form_state['item_count'] = $item_count;

  for ($i=count($item_list); $i<$item_count; ++$i) {
  	$item_list[] = array();
  }

  $form['#tree'] = TRUE;

  $plugin_options = sirius_dashboard_plugin_list();

	$form['item_list']['prefix'] = array('#markup' => '<div id="item_list">');
	foreach ($item_list as $delta => $item) {
		$uuid = $item['uuid'];
    $default_weight = $item['weight'];
    if (!$default_weight) { $default_weight = 10 * ($delta+1); }

		$title = $item['title'];
		if (!$title) { $title = '(new item)'; }
    $title .= "<span class=\"sirius_event_weight_indicator\">Sort Order: $default_weight</span>";

    $form['item_list'][$delta] = array(
    	'#type' => 'fieldset',
    	'#title' => $title,
    	'#collapsible' => TRUE,
    	'#collapsed' => TRUE,
    );

    $form['item_list'][$delta]['uuid'] = array(
    	'#type' => 'value',
    	'#value' => $item['uuid'],
    );
    $form['item_list'][$delta]['title'] = array(
    	'#type' => 'textfield',
    	'#title' => t('Title'),
    	'#default_value' => $item['title'],
    );

    $form['item_list'][$delta]['plugin'] = array(
    	'#type' => 'select',
    	'#title' => t('Plugin'),
    	'#default_value' => $item['plugin'],
    	'#options' => array(NULL => t('-- Select --')) + $plugin_options,
    );

    if ($item['plugin']) { 
    	$info = sirius_dashboard_plugin_info($item['plugin']);
    	$form['item_list'][$delta]['prefs'] = array('#markup' => l('Plugin Preferences', current_path() . '/' . $item['uuid'], array('query' => array('destination' => current_path()), 'attributes' => array('class' => array('tool-button')))));
    }

		$form['item_list'][$delta]['priority_cutoffs'] = array(
			'#type' => 'fieldset',
			'#title' => t('Priority Cutoffs'),
			'#description' => t('Enter the cutoffs for each priority level. For instance, you might indicate that a value above 100 should be considered "normal priority", but a value above 500 is "high priority".')
		);

		$priority_options = array(-1 => t('Hidden (do not show)')) + sirius_dashboard_priority_options();
		foreach ($priority_options as $priority => $title) {
			$form['item_list'][$delta]['priority_cutoffs'][$priority] = array(
				'#title' => t('Cutoff for Priority: ') . $title,
				'#type' => 'textfield',
				'#size' => 4,
				'#default_value' => $item['priority_cutoffs'][$priority],
			);
		}

		/*
    $form['item_list'][$delta]['priority'] = array(
      '#title' => t('Priority'),
      '#type' => 'select',
      '#options' => sirius_dashboard_priority_options(),
      '#default_value' => $item['priority'],
    );
    */
  
    $form['item_list'][$delta]['weight'] = array(
      '#title' => t('Sort Order'),
      '#type' => 'textfield',
      '#size' => 4,
    );
    if ($item['weight']) { $form['item_list'][$delta]['weight']['#default_value'] = $item['weight']; }

    $form['item_list'][$delta]['delete'] = array(
      '#type' => 'select',
      '#title' => t('Delete this item'),
      '#options' => array(
        NULL => t('No, keep this item'),
        'Yes' => t('Yes, delete this item.')
      ),
      '#description' => t('The item will be removed, and any students assigned to it will be unassigned.'),
    );

    $form['item_list'][$delta]['weight'] = array(
      '#type' => 'textfield',
      '#title' => 'Sort Order',
      '#default_value' => $default_weight,
      '#size' => 4,
    );
  }

	$form['item_list']['suffix'] = array('#markup' => '</div>');

  $form['add_one'] = array(
    '#type' => 'button',
    '#value' => t('Add one'),
    '#ajax' => array(
      'callback' => 'sirius_ajax_generic_replace',
    ),
    '#sirius_ajax' => array(
    	'qty' => 1,
    	'wrapper' => '#item_list',
    	'element' => 'item_list',
    ),
  );

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Save',
	);

	$form['suffix'] = array('#markup' => '</div>');

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}


function sirius_dashboard_page_items_form_submit($form, &$form_state) {
	$rid = $form_state['rid'];
	$json = sirius_dashboard_items_by_role($rid);

	$item_list = array();
	$item_list_input = $form_state['values']['item_list'];
	if (!$item_list_input) { $item_list_input = array(); }

	foreach ($item_list_input as $item) {
		if (!$item['plugin'] && !$item['title']) { continue; }
    if ($item['delete']) { continue; }
		$uuid = $item['uuid'];
		if (!$uuid) { 
			$uuid = uuid_generate();
			$item['uuid'] = $uuid;
		}
		$item_list[$uuid] = $item;
	}

	$json['item_list'] = $item_list;

	sirius_domain_variable_set("sirius_dashboard_items_by_rid_$rid", json_encode($json));

	drupal_set_message("Items saved.");
}

