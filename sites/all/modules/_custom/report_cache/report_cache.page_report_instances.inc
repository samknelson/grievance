<?php

function report_cache_page_report_instances_access($report_type) {
	// Must have a report type
	if (!$report_type) { return FALSE; }
	$info = report_cache_info($report_type);
	if (!$info) { return FALSE; }

	// Must have suitable privileges
	$access = $info['access'];
	if (!$access) { $access = 'access site reports'; }
	if (!user_access($access)) { return FALSE; }

	// Must have a parameteres callback; otherwise, there's no need for instances
	if (! ($info['generator']->form()) ) { return FALSE; }

	return TRUE;
}

function report_cache_page_report_instances($report_type) {
	return drupal_get_form('report_cache_page_report_instances_form', $report_type);
}

function report_cache_page_report_instances_form($form, &$form_state, $report_type) {
	sirius_backlink_add('sirius/reports', array('name' => 'All Reports'));
	drupal_add_css(drupal_get_path('module', 'sirius_event') . '/css/event.css', 'file');

	$form_state['report_type'] = $report_type;
	form_load_include($form_state, 'inc', 'report_cache', 'report_cache.page_report_instances');
  $form['#tree'] = TRUE;

  $info = report_cache_info($report_type);
	drupal_set_title("Reports: " . $info['name']);
  $ajax_params = $form_state['triggering_element']['#sirius_ajax'];
	$items = report_cache_instances($report_type);
	if (!$items) { $items = array(); }
	$items = array_values($items);

	if ($form_state['item_count']) { $item_count = $form_state['item_count']; } 
	else { $item_count = count($items); }
  $item_count += $ajax_params['qty'];
  if ($item_count < 1) { $item_count = 1; }
  $form_state['item_count'] = $item_count;

  for ($i=count($items); $i<$item_count; ++$i) { $items[] = array(); }

	$form['items']['prefix'] = array('#markup' => '<div id="items">');
	foreach ($items as $delta => $item) {
		$uuid = $item['uuid'];
    $default_weight = $item['weight'];
    if (!$default_weight) { $default_weight = 10 * ($delta+1); }

		$title = sirius_event_section_title($item);
    $title .= "<span class=\"sirius_event_weight_indicator\">Sort Order: $default_weight</span>";

    $form['items'][$delta] = array(
    	'#type' => 'fieldset',
    	'#title' => $title,
    	'#collapsible' => TRUE,
    	'#collapsed' => TRUE,
    );

    $form['items'][$delta]['uuid'] = array(
    	'#type' => 'value',
    	'#value' => $item['uuid'],
    );
    $form['items'][$delta]['title'] = array(
    	'#type' => 'textfield',
    	'#title' => t('Title'),
    	'#default_value' => $item['title'],
    );

		$form['items'][$delta]['auto']['enabled'] = array(
			'#type' => 'select',
			'#title' => 'Auto-Generation Enabled?',
			'#options' => array(
				NULL => t('-- Select --'),
				'No' => t('No'),
				'Yes' => t('Yes'),
			),
			'#default_value' => $item['auto']['enabled'],
		);

  	$form['items'][$delta]['parameters'] = $info['generator']->form();
		sirius_form_prep($form['items'][$delta]['parameters'], $item['parameters']);

    $form['items'][$delta]['weight'] = array(
      '#title' => t('Sort Order'),
      '#type' => 'textfield',
      '#size' => 4,
    );
    if ($item['weight']) { $form['items'][$delta]['weight']['#default_value'] = $item['weight']; }

    $form['items'][$delta]['delete'] = array(
      '#type' => 'select',
      '#title' => t('Delete this item'),
      '#options' => array(
        NULL => t('No, keep this item'),
        'Yes' => t('Yes, delete this item.')
      ),
    );

    $form['items'][$delta]['weight'] = array(
      '#type' => 'textfield',
      '#title' => 'Sort Order',
      '#default_value' => $default_weight,
      '#size' => 4,
    );
  }

	$form['items']['suffix'] = array('#markup' => '</div>');

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
    '#limit_validation_errors' => array(),
  );

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function report_cache_page_report_instances_form_submit($form, &$form_state) {
	$report_type = $form_state['report_type'];
	$items = array();
	$items_input = $form_state['values']['items'];
	if (!$items_input) { $items_input = array(); }

	foreach ($items_input as $key => $item) {
    if ($item['delete']) { continue; }
		$uuid = $item['uuid'];
		if (!$uuid) {
			$uuid = uuid_generate();
			$item['uuid'] = $uuid;
		}

		// @todo: This is painfully awkward. But ...
		//        We want to prepopulate the generate form in ajax from an instance. But 
		//        the data representation of form_state['values'] is different than 
		//        the data representation of form_state['input']. So we're storing both. Yuck.
		$item['xxxrawinputxxx'] = $form_state['input']['items'][$key];

		$items[$uuid] = $item;
	}

	report_cache_instances_set($report_type, $items);
	drupal_set_message("Instances saved.");
}
