<?php

function sirius_worker_term_tab_tor_statuses($tor_type_term) {
	return drupal_get_form('sirius_worker_term_tab_tor_statuses_form', $tor_type_term);
}

function sirius_worker_term_tab_tor_statuses_form($form, &$form_state, $tor_type_term) {
	$form_state['tor_type_tid'] = $tor_type_term->tid;
	$json = sirius_json_get($tor_type_term);

  drupal_add_css(drupal_get_path('module', 'sirius_event') . '/css/event.css', 'file');
  $form['#tree'] = TRUE;

	
  $ajax_params = $form_state['triggering_element']['#sirius_ajax'];

	$statuses = $json['tor']['statuses'];
	if (!$statuses) { $statuses = array(); }
	uksort($statuses, function($a, $b) use ($statuses) { return $statuses[$a]['weight'] <=> $statuses[$b]['weight']; });
	$statuses = array_values($statuses);

	if ($form_state['section_count']) {
		$section_count = $form_state['section_count'];
	} else {
		$section_count = count($statuses);
	}
  $section_count += $ajax_params['qty'];
  if ($section_count < 1) { $section_count = 1; }
  $form_state['section_count'] = $section_count;

  for ($i=count($statuses); $i<$section_count; ++$i) {
  	$statuses[] = array();
  }

  $form['#tree'] = TRUE;
	$form['prefix'] = array('#markup' => '<div class="sirius-form">');

	$form['statuses']['prefix'] = array('#markup' => '<div id="statuses">');
	foreach ($statuses as $delta => $section) {
		$uuid = $section['uuid'];
    $default_weight = $section['weight'];
    if (!$default_weight) { $default_weight = 10 * ($delta+1); }

		$title = sirius_event_section_title($section);
    $title .= "<span class=\"sirius_event_weight_indicator\">Sort Order: $default_weight</span>";

    $form['statuses'][$delta] = array(
    	'#type' => 'fieldset',
    	'#title' => $title,
    	'#collapsible' => TRUE,
    	'#collapsed' => FALSE,
    );

    $form['statuses'][$delta]['uuid'] = array(
    	'#type' => 'value',
    	'#value' => $section['uuid'],
    );
    $form['statuses'][$delta]['title'] = array(
    	'#type' => 'textfield',
    	'#title' => t('Title'),
    	'#default_value' => $section['title'],
			'#prefix' => '<div class="container-inline">',
			'#suffix' => '</div>',
    );
    $form['statuses'][$delta]['schedule'] = array(
    	'#type' => 'select',
    	'#title' => t('Show on Schedule?'),
    	'#default_value' => $section['schedule'],
    	'#options' => array(
    		'Yes' => t('Yes'),
    		'No' => ('No'),
    	),
			'#prefix' => '<div class="container-inline">',
			'#suffix' => '</div>',
    );
    $form['statuses'][$delta]['notify_worker'] = array(
    	'#type' => 'select',
    	'#title' => t('Notify Worker?'),
    	'#default_value' => $section['notify_worker'],
    	'#options' => array(
    		'Yes' => t('Yes'),
    		'No' => ('No'),
    	),
			'#prefix' => '<div class="container-inline">',
			'#suffix' => '</div>',
    );
    $form['statuses'][$delta]['notify_employer'] = array(
    	'#type' => 'select',
    	'#title' => t('Notify Employer?'),
    	'#default_value' => $section['notify_employer'],
    	'#options' => array(
    		'Yes' => t('Yes'),
    		'No' => ('No'),
    	),
			'#prefix' => '<div class="container-inline">',
			'#suffix' => '</div>',
    );
    $form['statuses'][$delta]['weight'] = array(
      '#type' => 'textfield',
      '#title' => 'Sort Order',
      '#default_value' => $default_weight,
      '#size' => 4,
			'#prefix' => '<div class="container-inline">',
			'#suffix' => '</div>',
    );

    $form['statuses'][$delta]['delete'] = array(
      '#type' => 'select',
      '#title' => t('Delete this section'),
      '#options' => array(
        NULL => t('No, keep this section'),
        'Yes' => t('Yes, delete this section.')
      ),
			'#prefix' => '<div class="container-inline">',
			'#suffix' => '</div>',
			/* '#description' => t('The section will be removed, and any students assigned to it will be unassigned.'), */
    );

  }

	$form['statuses']['suffix'] = array('#markup' => '</div>');

	form_load_include($form_state, 'inc', 'sirius_worker', 'sirius_worker.term_tab_tor_statuses');

  $form['add_one'] = array(
    '#type' => 'button',
    '#value' => t('Add one'),
    '#ajax' => array(
      'callback' => 'sirius_ajax_generic_replace',
    ),
    '#sirius_ajax' => array(
    	'qty' => 1,
    	'wrapper' => '#statuses',
    	'element' => 'statuses',
    ),
  );

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);
	$form['suffix'] = array('#markup' => '</div>');


	return $form;
}



function sirius_worker_term_tab_tor_statuses_form_submit($form, &$form_state) {
	$tor_type_tid = $form_state['tor_type_tid'];
	$tor_type_term = taxonomy_term_load($tor_type_tid);
	$json = sirius_json_get($tor_type_term);

	$statuses = array();
	$statuses_input = $form_state['values']['statuses'];
	if (!$statuses_input) { $statuses_input = array(); }

	foreach ($statuses_input as $section) {
		if (!$section['date'] && !$section['title']) { continue; }
    if ($section['delete']) { continue; }
		$uuid = $section['uuid'];
		if (!$uuid) { 
			$uuid = uuid_generate();
			$section['uuid'] = $uuid;
		}
		// $section['ts'] = strtotime($section['date']);
		$statuses[$uuid] = $section;
	}

	$json['tor']['statuses'] = $statuses;
	sirius_json_set($tor_type_term, $json);
	taxonomy_term_save($tor_type_term);
	drupal_set_message("Statuses saved.");
}