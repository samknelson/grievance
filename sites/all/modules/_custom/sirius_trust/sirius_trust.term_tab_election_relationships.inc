<?php

function sirius_trust_term_tab_election_relationships($election_type_term) {
	return drupal_get_form('sirius_trust_term_tab_election_relationships_form', $election_type_term);
}

function sirius_trust_term_tab_election_relationships_form($form, &$form_state, $election_type_term) {
	$form_state['election_type_tid'] = $election_type_term->tid;

  drupal_add_css(drupal_get_path('module', 'sirius_event') . '/css/event.css', 'file');
  $form['#tree'] = TRUE;

  $relationship_type_options = sirius_taxonomy_load('sirius_contact_relationship_types', 'tid', 'name');

	$json = sirius_json_get($election_type_term);
	
  $ajax_params = $form_state['triggering_element']['#sirius_ajax'];

	$relationship_criteria = $json['relationship_criteria'];
	if (!$relationship_criteria) { $relationship_criteria = array(); }
	uksort($relationship_criteria, function($a, $b) use ($relationship_criteria) { return $relationship_criteria[$a]['weight'] <=> $relationship_criteria[$b]['weight']; });
	$relationship_criteria = array_values($relationship_criteria);

	if ($form_state['section_count']) {
		$section_count = $form_state['section_count'];
	} else {
		$section_count = count($relationship_criteria);
	}
  $section_count += $ajax_params['qty'];
  if ($section_count < 1) { $section_count = 1; }
  $form_state['section_count'] = $section_count;

  for ($i=count($relationship_criteria); $i<$section_count; ++$i) {
  	$relationship_criteria[] = array();
  }

  $form['#tree'] = TRUE;

	$form['relationship_criteria']['prefix'] = array('#markup' => '<div id="relationship_criteria">');
	foreach ($relationship_criteria as $delta => $section) {
		$uuid = $section['uuid'];
    $default_weight = $section['weight'];
    if (!$default_weight) { $default_weight = 10 * ($delta+1); }

		$title = sirius_event_section_title($section);
    $title .= "<span class=\"sirius_event_weight_indicator\">Sort Order: $default_weight</span>";

    $form['relationship_criteria'][$delta] = array(
    	'#type' => 'fieldset',
    	'#title' => $title,
    	'#collapsible' => TRUE,
    	'#collapsed' => FALSE,
    );

    $form['relationship_criteria'][$delta]['uuid'] = array(
    	'#type' => 'value',
    	'#value' => $section['uuid'],
    );
    $form['relationship_criteria'][$delta]['title'] = array(
    	'#type' => 'textfield',
    	'#title' => t('Title'),
    	'#default_value' => $section['title'],
    );

    $form['relationship_criteria'][$delta]['min'] = array(
    	'#type' => 'textfield',
    	'#title' => t('Minimum number of relationships'),
    	'#size' => 4,
    	'#default_value' => $section['min'],
    );

    $form['relationship_criteria'][$delta]['max'] = array(
    	'#type' => 'textfield',
    	'#title' => t('Maximum number of relationships'),
    	'#size' => 4,
    	'#default_value' => $section['max'],
    );

    $form['relationship_criteria'][$delta]['relationship_types'] = array(
    	'#type' => 'checkboxes',
    	'#title' => t('Allowed Relationships'),
    	'#size' => 4,
    	'#options' => $relationship_type_options,
    );
    if ($section['relationship_types']) { $form['relationship_criteria'][$delta]['relationship_types']['#default_value'] = $section['relationship_types']; }

    $form['relationship_criteria'][$delta]['weight'] = array(
      '#title' => t('Sort Order'),
      '#type' => 'textfield',
      '#size' => 4,
    );
    if ($section['weight']) { $form['relationship_criteria'][$delta]['weight']['#default_value'] = $section['weight']; }


    $form['relationship_criteria'][$delta]['delete'] = array(
      '#type' => 'select',
      '#title' => t('Delete this section'),
      '#options' => array(
        NULL => t('No, keep this section'),
        'Yes' => t('Yes, delete this section.')
      ),
      '#description' => t('The section will be removed, and any students assigned to it will be unassigned.'),
    );

    $form['relationship_criteria'][$delta]['weight'] = array(
      '#type' => 'textfield',
      '#title' => 'Sort Order',
      '#default_value' => $default_weight,
      '#size' => 4,
    );
  }

	$form['relationship_criteria']['suffix'] = array('#markup' => '</div>');

	form_load_include($form_state, 'inc', 'sirius_trust', 'sirius_trust.term_tab_election_relationships');

  $form['add_one'] = array(
    '#type' => 'button',
    '#value' => t('Add one'),
    '#ajax' => array(
      'callback' => 'sirius_ajax_generic_replace',
    ),
    '#sirius_ajax' => array(
    	'qty' => 1,
    	'wrapper' => '#relationship_criteria',
    	'element' => 'relationship_criteria',
    ),
  );

  $form['add_ten'] = array(
    '#type' => 'button',
    '#value' => t('Add ten'),
    '#ajax' => array(
      'callback' => 'sirius_ajax_generic_replace',
    ),
    '#sirius_ajax' => array(
    	'qty' => 10,
    	'wrapper' => '#relationship_criteria',
    	'element' => 'relationship_criteria',
    ),
    '#suffix' => '<br clear="left" />',
  );

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}



function sirius_trust_term_tab_election_relationships_form_submit($form, &$form_state) {
	$election_type_tid = $form_state['election_type_tid'];
	$election_type_term = taxonomy_term_load($election_type_tid);
	$json = sirius_json_get($election_type_term);

	$relationship_criteria = array();
	$relationship_criteria_input = $form_state['values']['relationship_criteria'];
	if (!$relationship_criteria_input) { $relationship_criteria_input = array(); }

	foreach ($relationship_criteria_input as $section) {
		if (!$section['date'] && !$section['title']) { continue; }
    if ($section['delete']) { continue; }
		$uuid = $section['uuid'];
		if (!$uuid) { 
			$uuid = uuid_generate();
			$section['uuid'] = $uuid;
		}
		// $section['ts'] = strtotime($section['date']);
		$relationship_criteria[$uuid] = $section;
	}

	$json['relationship_criteria'] = $relationship_criteria;
	sirius_json_set($election_type_term, $json);
	taxonomy_term_save($election_type_term);
	drupal_set_message("Settings saved.");
}