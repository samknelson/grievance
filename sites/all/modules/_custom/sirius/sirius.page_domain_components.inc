<?php

function sirius_node_tab_sirius_domain_components($domain_node) {
	return drupal_get_form('sirius_node_tab_sirius_domain_components_form', $domain_node);
}

function sirius_node_tab_sirius_domain_components_form($form, &$form_state, $domain_node) {
  drupal_add_css(drupal_get_path('module', 'sirius') . '/css/table.css');

	$form_state['domain_nid'] = $domain_node->nid;
	$json = sirius_jsonfield_util_node_json_get($domain_node);
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
		if ($json['component'][$key]['enabled'] == 'Yes') { $class = 'green'; } else { $class = 'gray'; }

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
			'#default_value' => $json['component'][$key]['enabled'],
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

function sirius_node_tab_sirius_domain_components_form_submit($form, &$form_state) {
	$domain_node = node_load($form_state['domain_nid']);
	$json = sirius_jsonfield_util_node_json_get($domain_node);
	$json['component'] = $form_state['values']['component'];
	sirius_jsonfield_util_node_json_set($domain_node, $json);
	node_save($domain_node);
	drupal_set_message("Components saved.");
}