<?php 

function sirius_node_tab_entity_plugins_access($entity_node, $plugin_class) {
	if ($plugin_class == 'ledger_charge') {
		if ( ($entity_node->type == 'sirius_json_definition') && 
			   ($entity_node->field_sirius_type['und'][0]['value'] == 'sirius_ledger:policy')
			 ) { return TRUE; }
	}

	if ($plugin_class == 'service_client') {
		if ($entity_node->type == 'sirius_domain') { return TRUE; }
		if ( ($entity_node->type == 'sirius_json_definition') && 
			   ($entity_node->field_sirius_type['und'][0]['value'] == 'domain_root_shadow')
			 ) { return TRUE; }
	}

	return FALSE;
}

function sirius_node_tab_entity_plugins_quicknav($entity_node, $plugin_class) {
	$plugins = sirius_plugin_entity_plugins($entity_node, $plugin_class);

	$base_path = current_path();
	$base_path = preg_replace('/\/[0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}$/', '', $base_path);

	$links = array();
	$link = array();
	$link['title'] = 'All';
	$link['path'] = $base_path;
	$links[] = $link;

	foreach ($plugins as $plugin_uuid => $plugin) {
		$link['title'] = $plugin['title'];
		$link['path'] = "$base_path/$plugin_uuid";
		$links[] = $link;
	}

	return sirius_quicknav($links);
}

function sirius_node_tab_entity_plugins($entity_node, $plugin_class) {
	$plugin_uuid = sirius_node_tab_arg(0);
	if ($plugin_uuid) {
		sirius_require('sirius', 'sirius.page_entity_plugin.inc');
		return drupal_get_form('sirius_node_tab_entity_plugin_form', $entity_node, $plugin_class, $plugin_uuid);
	}

	return drupal_get_form('sirius_node_tab_entity_plugins_form', $entity_node, $plugin_class);
}

function sirius_node_tab_entity_plugins_form($form, &$form_state, $entity_node, $plugin_class) {
	$form_state['entity_nid'] = $entity_node->nid;
	$form_state['plugin_class'] = $plugin_class;
	$entity_json = sirius_json_get($entity_node);
	$info_all = sirius_plugin_info_all($plugin_class);

	$options = array();
	foreach ($info_all as $key => $info) {
		$options[$key] = $info['name'];
	}

  drupal_add_css(drupal_get_path('module', 'sirius_event') . '/css/event.css', 'file');

	$form['#tree'] = TRUE;

	$form['nav'] = array('#markup' => sirius_node_tab_entity_plugins_quicknav($entity_node, $plugin_class), '#weight' => -10);

  $ajax_params = $form_state['triggering_element']['#sirius_ajax'];

	$plugins = sirius_plugin_entity_plugins($entity_node, $plugin_class);
	if (!$plugins) { $plugins = array(); }
	uksort($plugins, function($a, $b) use ($plugins) { return $plugins[$a]['weight'] <=> $plugins[$b]['weight']; });
	$plugins = array_values($plugins);

	if ($form_state['plugin_count']) {
		$plugin_count = $form_state['plugin_count'];
	} else {
		$plugin_count = count($plugins);
	}
  $plugin_count += $ajax_params['qty'];
  if ($plugin_count < 1) { $plugin_count = 1; }
  $form_state['plugin_count'] = $plugin_count;

  for ($i=count($plugins); $i<$plugin_count; ++$i) {
  	$plugins[] = array();
  }

	$form['plugins']['prefix'] = array('#markup' => '<div id="plugins">');
	foreach ($plugins as $delta => $plugin) {
		$uuid = $plugin['uuid'];
    $default_weight = $plugin['weight'];
    if (!$default_weight) { $default_weight = 10 * ($delta+1); }

		$title = $plugin['title'];
    $title .= "<span class=\"sirius_event_weight_indicator\">Sort Order: $default_weight</span>";

    $form['plugins'][$delta] = array(
    	'#type' => 'fieldset',
    	'#title' => $title,
    	'#collapsible' => FALSE,
    	'#collapsed' => FALSE,
    );

    $form['plugins'][$delta]['uuid'] = array(
    	'#type' => 'value',
    	'#value' => $plugin['uuid'],
    );

    $form['plugins'][$delta]['title'] = array(
    	'#type' => 'textfield',
    	'#title' => t('Title'),
    	'#default_value' => $plugin['title'],
    );

		$form['plugins'][$delta]['plugin'] = array(
			'#type' => 'select',
			'#title' => t('Plugin'),
			'#options' => array(NULL => t('-- Select --')) + $options,
			'#default_value' => $plugin['plugin'],
		);

    $form['plugins'][$delta]['delete'] = array(
      '#type' => 'select',
      '#title' => t('Delete this section'),
      '#options' => array(
        NULL => t('No, keep this section'),
        'Yes' => t('Yes, delete this section.')
      ),
      '#description' => t('The section will be removed, and any students assigned to it will be unassigned.'),
    );

    $form['plugins'][$delta]['weight'] = array(
      '#type' => 'textfield',
      '#title' => 'Sort Order',
      '#default_value' => $default_weight,
      '#size' => 4,
    );
  }

	$form['plugins']['suffix'] = array('#markup' => '</div>');

	form_load_include($form_state, 'inc', 'sirius', 'sirius.page_entity_plugins');

  $form['add_one'] = array(
    '#type' => 'button',
    '#value' => t('Add one'),
    '#ajax' => array(
      'callback' => 'sirius_ajax_generic_replace',
    ),
    '#sirius_ajax' => array(
    	'qty' => 1,
    	'wrapper' => '#plugins',
    	'element' => 'plugins',
    ),
  );

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_node_tab_entity_plugins_form_submit($form, &$form_state) {
	$entity_node = node_load($form_state['entity_nid']);
	$entity_json = sirius_json_get($entity_node);
	$plugin_class = $form_state['plugin_class'];

	$plugins = array();
	$plugins_input = $form_state['values']['plugins'];
	if (!$plugins_input) { $plugins_input = array(); }

	foreach ($plugins_input as $plugin) {
		if (!$plugin['plugin'] && !$plugin['title']) { continue; }
    if ($plugin['delete']) { continue; }
		$uuid = $plugin['uuid'];
		if (!$uuid) { 
			$uuid = uuid_generate();
			$plugin['uuid'] = $uuid;
		}
		$plugins[$uuid] = $plugin;
	}

	$entity_json['plugins'][$plugin_class] = $plugins;

	sirius_json_set($entity_node, $entity_json);
	node_save($entity_node);
	drupal_set_message("Settings saved.");
}
