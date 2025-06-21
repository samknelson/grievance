<?php

function sirius_smf_page_employer_buildup_settings_form($form, &$form_state, $employer_node) {
	$form_state['employer_nid'] = $employer_node->nid;
	$employer_json = sirius_json_get($employer_node);
	$ms_names = sirius_taxonomy_load('sirius_member_status', 'tid', 'name');

	form_load_include($form_state, 'inc', 'sirius_smf', 'sirius_smf.page_employer_buildup_settings');
  drupal_add_css(drupal_get_path('module', 'sirius_event') . '/css/event.css', 'file');
  $form['#tree'] = TRUE;

  $info = sirius_trust_eligibility_plugin_info('smf-buildup');
  $instance = $info['instance'];
  $sections = $instance->hours_thresholds_history($employer_node);
	$sections = array_values($sections);
  $ajax_params = $form_state['triggering_element']['#sirius_ajax'];
	if ($form_state['section_count']) { $section_count = $form_state['section_count']; } 
	else { $section_count = count($sections); }
  $section_count += $ajax_params['qty'];
  if ($section_count < 1) { $section_count = 1; }
  $form_state['section_count'] = $section_count;
  for ($i=count($sections); $i<$section_count; ++$i) { $sections[] = array(); }

  $form['#tree'] = TRUE;

	$form['sections']['prefix'] = array('#markup' => '<div id="sections">');
	foreach ($sections as $delta => $section) {
		$uuid = $section['uuid'];

		$title = $section['date'] . ': ' . $info['instance']->hours_thresholds_render($section['hours_threshold']);

    $form['sections'][$delta] = array(
    	'#type' => 'fieldset',
    	'#title' => $title,
    );

    $form['sections'][$delta]['uuid'] = array(
    	'#type' => 'value',
    	'#value' => $section['uuid'],
    );
    $form['sections'][$delta]['date'] = array(
    	'#type' => 'textfield',
    	'#title' => t('Effective Date'),
    	'#default_value' => $section['date'],
    	'#size' => 10,
    );

    $form['sections'][$delta]['hours_threshold'] = array(
    	'#type' => 'fieldset',
    	'#title' => t('Monthly Threshold by Member Status'),
    );
    foreach ($ms_names as $ms_tid => $ms_name) {
    	$form['sections'][$delta]['hours_threshold'][$ms_tid] = array(
    		// '#title' => $ms_name,
    		'#type' => 'textfield',
    		'#size' => 4,
    		'#default_value' => $section['hours_threshold'][$ms_tid],
    		'#prefix' => '<div class="container-inline">',
    		'#suffix' => $ms_name . '</div>',
    	);
    }

    $form['sections'][$delta]['weight'] = array(
      '#title' => t('Sort Order'),
      '#type' => 'value',
    	'#default_value' => $section['weight'],
    );
  }

	$form['sections']['suffix'] = array('#markup' => '</div>');

  $form['add_one'] = array(
    '#type' => 'button',
    '#value' => t('Add one'),
    '#ajax' => array(
      'callback' => 'sirius_ajax_generic_replace',
    ),
    '#sirius_ajax' => array(
    	'qty' => 1,
    	'wrapper' => '#sections',
    	'element' => 'sections',
    ),
  );

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_smf_page_employer_buildup_settings_form_submit($form, &$form_state) {
	$employer_node = node_load($form_state['employer_nid']);

	$sections = array();
	$sections_input = $form_state['values']['sections'];
	if (!$sections_input) { $sections_input = array(); }

	foreach ($sections_input as $section) {
		if (!$section['date']) { continue; }
		$section['date'] = date('Y-m-d', strtotime($section['date']));
		$section['weight'] = date('Ymd', strtotime($section['date']));
    if ($section['delete']) { continue; }
		$uuid = $section['uuid'];
		if (!$uuid) { 
			$uuid = uuid_generate();
			$section['uuid'] = $uuid;
		}
		$sections[$uuid] = $section;
	}

  $info = sirius_trust_eligibility_plugin_info('smf-buildup');
	$info['instance']->hours_thresholds_history_set($employer_node, $sections);
	drupal_set_message("Settings saved.");
}