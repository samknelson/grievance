<?php

function sirius_page_taxonomy_vocabulary_admin() { 
	return drupal_get_form('sirius_page_taxonomy_vocabulary_admin_form');
}

function sirius_page_taxonomy_vocabulary_admin_form($form, &$form_state) {
  drupal_add_library('system', 'ui.accordion');
  drupal_add_js('jQuery(document).ready(function () { jQuery("#sirius_accordion").accordion({active: false, collapsible: true, autoehight: true, heightStyle: "content"}); });', 'inline');

  $settings = sirius_taxonomy_settings('all');

	$sql = "select name, machine_name, vid ";
	$sql .= "from taxonomy_vocabulary ";
	$sql .= "order by machine_name ";
	$stmt = db_query($sql, array());
	$vocabularies = array();
	while ($hr = $stmt->fetchAssoc()) {
		$vocabularies[$hr['machine_name']] = $hr;
	}

  $form['accordion_open'] = array('#markup' => '<div id="sirius_accordion">');
	foreach ($vocabularies as $vocabulary_machine_name => $hr) {
	  $form["accordion_open_$vocabulary_machine_name"] = array('#markup' => '<div class="sirius_accordion_label">' . $hr['name'] . ' (' . $vocabulary_machine_name . ', vid = ' . $hr['vid'] . ')' . '</div><div class="sirius_accordion_body">');

		$form["sirius_taxonomy_view_$vocabulary_machine_name"] = array(
			'#type' => 'textfield',
			'#title' => t('View'),
			'#description' => t('Enter the machine name of the view to be used for this taxonomy, e.g. "sirius_taxonomy". The view must have two contextual filters, "Vocabulary Machine Name" and "Domain".'),
			'#default_value' => variable_get("sirius_taxonomy_view_$vocabulary_machine_name", ''),
		);

		$form["sirius_taxonomy_view_display_$vocabulary_machine_name"] = array(
			'#type' => 'textfield',
			'#title' => t('Display'),
			'#description' => t('Enter the machine name of the view display to be used for this taxonomy, e.g. "block".'),
			'#default_value' => variable_get("sirius_taxonomy_view_display_$vocabulary_machine_name", ''),
		);

		$form["sirius_taxonomy_multidomain_$vocabulary_machine_name"] = array(
			'#type' => 'select',
			'#title' => t('Multi-domain?'),
			'#description' => t('Select "Yes" to allow this taxonomy to be cloned between domains. Select "No" to share this taxonomy across all domains.'),
			'#default_value' => variable_get("sirius_taxonomy_multidomain_$vocabulary_machine_name", ''),
			'#options' => array(
				NULL => t('-- Select --'),
				'Yes' => t('Yes'),
				'No' => t('No'),
			),
		);

	  $form["accordion_close_$vocabulary_machine_name"] = array('#markup' => '</div>');
	}
  $form['accordion_close'] = array('#markup' => '</div>');

  return system_settings_form($form);
}

