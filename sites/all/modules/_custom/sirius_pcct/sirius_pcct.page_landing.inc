<?php

function sirius_pcct_page_landing() {
	drupal_set_title('Choose a Service');
	
	return drupal_get_form('sirius_pcct_page_landing_form');
}

function sirius_pcct_page_landing_form($form, &$form_state) {
	// Must have picked a domain by this point. If not, go away and pick one.
	if (!sirius_domain_nid()) { drupal_goto('user'); }

	$form['#tree'] = TRUE;

	drupal_add_css(drupal_get_path('module', 'sirius_pcct') . '/css/pcct.css', 'file');	
	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/table.css', 'file');	

	$html = '<div class="sirius_pcct_policy_wrap">';
	$html .= sirius_pcct_policy_render();
	$html .= '</div>';
	$form['policy'] = array('#markup' => $html);

	$html = '<div class="sirius_pcct_landing_header">';
	$html .= sirius_domain_variable_get('sirius_pcct_landing_header');
	$html .= '</div>'; // sirius_pcct_landing_header
	$form['header'] = array('#markup' => $html);

	$form['service_type'] = array(
		'#prefix' => '<div class="sirius_pcct_landing_body">',
		'#suffix' => '</div>',
	);

	$result = sirius_trust_service_type_hardcode_lookup();
	if (!$result['success']) { return sirius_form_error($result['msg']); }
	foreach ($result['data'] as $service_type_id => $service_type_record) {
		$form['service_type'][$service_type_id] = array(
			'#prefix' => '<div class="sirius_pcct_landing_service_type">',
			'#suffix' => '</div>',
		);

		$options = sirius_trust_service_options(
			array(
				'service_type_tid' => $service_type_record['tid'],
				'active' => 'Yes',
			)
		);
		if (!$options) { continue; }

		/*
		$form['service_type'][$service_type_id]['intro'] = array(
			'#markup' => '<h2>' . $service_type_record['landing_prompt'] . '</h2>',
		);
		*/

		$form['service_type'][$service_type_id]['service_nid'] = array(
			'#type' => 'select',
			'#title' => $service_type_record['landing_prompt'],
			'#options' => array(NULL => t('-- Select --')) + $options,
		);

		$form['service_type'][$service_type_id]['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Go'),
			'#id' => 'submit_' . $service_type_id,
			'#data' => array(
				'id' => $service_type_id,
				'tid' => $service_type_record['service_type_tid'],
			),
			'#name' => $service_type_id,
		);
	}

	$html .= '';

	/*
	$html .= '<div class="sirius_pcct_popular_searches">';
	$html .= '<h2>' . t('Popular Searches') . '</h2>';
	$options = preg_split('/[\n\r]+/', sirius_domain_variable_get('sirius_pcct_popular_searches'));
	$len = (int) count($options);
	$col1 = array_slice($options, 0, $len / 2);
	$col2 = array_slice($options, $len / 2);

	$html .= '<div class="sirius_pcct_popular_searches_col1">';
	$html .= '<ul>';
	foreach ($col1 as $item) {
		$html .= '<li>' . l($item, '<front>') . '</li>';
	}
	$html .= '</ul>';
	$html .= '</div>'; // sirius_pcct_popular_searches_col1

	$html .= '<div class="sirius_pcct_popular_searches_col2">';
	$html .= '<ul>';
	foreach ($col2 as $item) {
		$html .= '<li>' . l($item, '<front>') . '</li>';
	}
	$html .= '</ul>';
	$html .= '</div>'; // sirius_pcct_popular_searches_col2
	*/

	$html .= '</div>'; // sirius_pcct_popular_searches

	$html .= '</div>'; // sirius_pcct_landing_body


	$html = '<div class="sirius_pcct_landing_header">';
	$html .= sirius_domain_variable_get('sirius_pcct_landing_footer');
	$html .= '</div>';
	$form['footer'] = array('#markup' => $html);

	return $form;
}


function sirius_pcct_page_landing_form_submit($form, &$form_state) {
	if ($form_state['triggering_element']['#data']['id']) {
		$ids = array($form_state['triggering_element']['#data']['id']);
	} else {
		$result = sirius_trust_service_type_hardcode_lookup();
		$ids = array_keys($result['data']);
	}

	foreach ($ids as $service_type_id) {
		$service_nid = $form_state['values']['service_type'][$service_type_id]['service_nid'];
		if ($service_nid) {
			drupal_goto("node/$service_nid/sirius_pcct_service_public_list");
		}
	}

	drupal_set_message("Please select a service.", 'warning');
}