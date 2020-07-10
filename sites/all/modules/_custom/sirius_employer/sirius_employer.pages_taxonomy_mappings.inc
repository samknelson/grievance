<?php

function sirius_employer_node_tab_taxonomy_mappings_access($employer_node) {
	// Admins can access
	if (user_access('sirius employer admin')) { return TRUE; }

	// Employers can access their own page only
	if (user_access('sirius employer user manager')) { 
		if (!$employer_node->nid) { return FALSE; }
		global $user;
		$user_full = user_load($user->uid);
		if ($user_full->field_grievance_shops['und'][0]['target_id'] == $employer_node->nid) { return TRUE; }
	}

	// Oh well.
	return FALSE;
}


function sirius_employer_node_tab_taxonomy_mappings($employer_node) {
	$vocabulary_machine_name = sirius_node_tab_arg(0);
	if ($vocabulary_machine_name) {
		$settings = sirius_taxonomy_settings($vocabulary_machine_name);
		if ($settings['employer_map'] != 'Yes') { 
			drupal_set_message("Not mappable.");
			return array();
		}
	}


	if (!$vocabulary_machine_name) {
		$html = sirius_employer_node_tab_taxonomy_mappings_nav($employer_node);
		$html .= 'Use the links above to create mappings for drop-down lists from your system. For example, if you tag overtime hours as "OT" but this system tags them as "Overtime", you can set up a mapping under the "Hour Type" link to allow import and export.';
		return $html;
	}

	return drupal_get_form('sirius_employer_node_tab_taxonomy_mappings_form', $employer_node, $vocabulary_machine_name);
}

function sirius_employer_node_tab_taxonomy_mappings_form($form, &$form_state, $employer_node, $vocabulary_machine_name) {
	$form_state['employer_nid'] = $employer_node->nid;
	$form_state['vocabulary_machine_name'] = $vocabulary_machine_name;

	$json = sirius_jsonfield_util_node_json_get($employer_node);

	$form['#tree'] = TRUE;
	$form['nav'] = array('#markup' => sirius_employer_node_tab_taxonomy_mappings_nav($employer_node));

	$options = sirius_taxonomy_load($vocabulary_machine_name, 'tid', 'name', TRUE);

	$form['mappings']['table_open'] = array('#markup' => '<table><tr><th>From</th><th>To</th></tr>');

	$count = 5;
	if ($json['taxonomy_mappings'][$vocabulary_machine_name]['mappings']) {
		$count = 0;
		foreach ($json['taxonomy_mappings'][$vocabulary_machine_name]['mappings'] as $record) {
			if ($record['from'] || $record['to']) { ++$count; }
		}
		$count += 5;
	}

	for ($i=0; $i<$count; ++$i) {
		$form['mappings'][$i]['from'] = array(
			'#type' => 'textfield',
			'#prefix' => '<tr><td>',
			'#suffix' => '</td>',
			'#default_value' => $json['taxonomy_mappings'][$vocabulary_machine_name]['mappings'][$i]['from'],
		);
		$form['mappings'][$i]['to'] = array(
			'#type' => 'select',
			'#options' => $options,
			'#prefix' => '<td>',
			'#suffix' => '</td></tr>',
			'#default_value' => $json['taxonomy_mappings'][$vocabulary_machine_name]['mappings'][$i]['to'],
		);
	}
	$form['mappings']['table_close'] = array('#markup' => '</table>');

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_employer_node_tab_taxonomy_mappings_form_submit($form, &$form_state) {
	$employer_node = node_load($form_state['employer_nid']);
	$vocabulary_machine_name = $form_state['vocabulary_machine_name'];

	$json = sirius_jsonfield_util_node_json_get($employer_node);
	$json['taxonomy_mappings'][$vocabulary_machine_name]['mappings'] = $form_state['values']['mappings'];
	sirius_jsonfield_util_node_json_set($employer_node, $json);
	node_save($employer_node);
	drupal_set_message("Mappings saved.");
}

function sirius_employer_node_tab_taxonomy_mappings_nav($employer_node) {
	$path = 'node/' . $employer_node->nid . '/employer_taxonomy_mappings';

	$links = array();
  $links['overview'] = array('path' => $path, 'title' => 'Overview');

	$settings = sirius_taxonomy_settings('all');
	foreach ($settings as $vocabulary_machine_name => $record) {
		if ($record['employer_map'] != 'Yes') { continue; }
		$vocabulary = taxonomy_vocabulary_machine_name_load($vocabulary_machine_name);
		$title = $vocabulary->name;
		$title = preg_replace('/^sirius\s*/i', '', $title);
		$title = preg_replace('/^grievance\s*/i', '', $title);
		$title = trim($title);
		$links[$vocabulary_machine_name] = array('path' => "$path/$vocabulary_machine_name", 'title' => $title);
	}
  $html .= sirius_quicknav($links);

  return $html;
}