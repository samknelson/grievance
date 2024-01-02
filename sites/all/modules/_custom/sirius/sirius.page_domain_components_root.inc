<?php

function sirius_page_domain_components_root() {
	return drupal_get_form('sirius_page_domain_components_root_form');
}

function sirius_page_domain_components_root_form($form, &$form_state) {
  drupal_add_css(drupal_get_path('module', 'sirius') . '/css/table.css');
	$form['#tree'] = TRUE;

	$all_info = sirius_domain_component_info_all();


	$form['component']['open'] = array(
		'#markup' => '<table class="sirius_table">' .
			'<tr>' . 
			'<th>' . t('Component') . '</th>' .
			'<th>' . t('Enable?') . '</th>' .
			'</tr>'
	);

	foreach ($all_info as $key => $info) {
    $variable_name = 'sirius_domain_root_component_enabled_' . $key;

		if (variable_get($variable_name, 'Yes') == 'Yes') { $class = 'green'; } else { $class = 'gray'; }

		$form['component'][$key]['open'] = array('#markup' => "<tr class=\"$class\">");

		$form['component'][$key]['title'] = array(
			'#markup' => '<td>' .
				sirius_tooltip_render($info['description'], $info['title']) . 
				'</td>',
		);

		$form['component'][$key]['enabled'] = array(
			'#type' => 'select',
			'#options' => array(
				NULL => t('-- Select --'),
				'No' => t('No'),
				'Yes' => t('Yes'),
			),
			'#default_value' => variable_get($variable_name, 'Yes'),
			'#prefix' => '<td>',
			'#suffix' => '</td>',
		);

		$form['component'][$key]['close'] = array('#markup' => '</tr>');
	}

	$form['component']['close'] = array(
		'#markup' => '</table>'
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_page_domain_components_root_form_submit($form, &$form_state) {
	foreach ($form_state['values']['component'] as $key => $record) {
    $variable_name = 'sirius_domain_root_component_enabled_' . $key;
    variable_set($variable_name, $record['enabled']);
	}
	drupal_set_message("Components saved.");
}