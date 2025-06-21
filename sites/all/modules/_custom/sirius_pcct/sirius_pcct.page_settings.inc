<?php

function sirius_pcct_page_settings() {
	return drupal_get_form('sirius_pcct_page_settings_form');
}

function sirius_pcct_page_settings_form($form, &$form_state) {
	$form['#tree'] = TRUE;

	/*
	$form['settings']['sirius_pcct_show_on_global_list'] = array(
		'#title' => t('Show on Global List?'),
		'#type' => 'select',
		'#default_value' => sirius_domain_variable_get('sirius_pcct_show_on_global_list'),
		'#options' => array(
			'No' => t('No'),
			'Yes' => t('Yes'),
		),
		'#description' => t('Should this domain be shown on the global list?'),
		'#required' => TRUE,
	);
	*/

	global $base_url;
	$form['settings']['sirius_pcct_launch_code'] = array(
		'#type' => 'textfield',
		'#title' => t('Launch Code'),
		'#description' => t('The "launch code" is the URL component that visitors will use to find the site. The final URL will be: ') .
			$base_url . '/pcct/[launch code]',
		'#default_value' => sirius_domain_variable_get('sirius_pcct_launch_code'),
		'#required' => TRUE,
	);

	$policy_options = sirius_ledger_policy_options();
	if (!$policy_options) {
		return sirius_form_error('There are no policies available in this domain. In order to use the Personal Price Comparison Tool, please create at least one policy. Policies can be managed ' . l('here.', 'sirius/config/ledger/policies'));
	}

	// @todo: Eventually, we will support multiple policies within a domain.
	$form['settings']['sirius_pcct_policy_nid'] = array(
		'#type' => 'select',
		'#options' => array(NULL => t('-- Select --')) + $policy_options,
		'#title' => t('Policy'),
		'#description' => t('Which policy should be used for the Personal Cost Comparison Tool?'),
		'#default_value' => sirius_domain_variable_get('sirius_pcct_policy_nid'),
		'#required' => TRUE,
	);

	$policy_nid = sirius_domain_variable_get('sirius_pcct_policy_nid');
	if ($policy_nid) {
		$policy_node = node_load($policy_nid);
		$html = '<p>';
		$html .= 'Currently policy: <strong>' . $policy_node->title . '</strong>. ';
		$html .= l('Settings', "node/$policy_nid/sirius_pcct_settings", array('attributes' => array('class' => array('tool-button'))));
		$html .= '</p>';
		$form['settings']['policy_markup'] = array('#markup' => $html);
	}

	$form['settings']['sirius_pcct_map_default_center_address'] = array(
		'#title' => t('Map Default Center'),
		'#type' => 'textfield',
		'#default_value' => sirius_domain_variable_get('sirius_pcct_map_default_center_address'),
		'#description' => t('Enter the address for the (default) center of the "find a provider" map. This should be a full address, e.g. "123 Fake St., Anytown, AB 12345".'),
		'#required' => TRUE,
	);

	// Rough dimensions in meters / pixel for various zoom levels (at latitude +/-40)
	$raw_zoom_options = array(
		0 => 59959.436,
		1 => 29979.718,
		2 => 14989.859,
		3 => 7494.929,
		4 => 3747.465,
		5 => 1873.732,
		6 => 936.866,
		7 => 468.433,
		8 => 234.217,
		9 => 117.108,
		10 => 58.554,
		11 => 29.277,
		12 => 14.639,
		13 => 7.319,
		14 => 3.660,
		15 => 1.830,
		16 => 0.915,
		17 => 0.457,
		18 => 0.229,
		19 => 0.114,
		20 => 0.057,
		21 => 0.029,
		22 => 0.014,
	);

	$zoom_options = array();
	foreach ($raw_zoom_options as $zoom => $meters_per_pixel) {
		$miles_per_map = round(500 * $meters_per_pixel / 1609.34, $decimal_places);
		$decimal_places = 3 - floor(log10($miles_per_map)) - 1;
		$zoom_options[$zoom] = "Zoom $zoom (about $miles_per_map miles on a side)";
	}

	$form['settings']['sirius_pcct_map_default_zoom'] = array(
		'#title' => t('Map Default Zoom'),
		'#type' => 'select',
		'#default_value' => sirius_domain_variable_get('sirius_pcct_map_default_zoom', 13),
		'#options' => $zoom_options,
		'#description' => t('Enter the default "zoom" level for the map.'),
		'#required' => TRUE,
	);

	$form['settings']['sirius_pcct_map_default_center_address'] = array(
		'#title' => t('Map Default Center'),
		'#type' => 'textfield',
		'#default_value' => sirius_domain_variable_get('sirius_pcct_map_default_center_address'),
		'#description' => t('Enter the address for the (default) center of the "find a provider" map. This should be a full address, e.g. "123 Fake St., Anytown, AB 12345".'),
		'#required' => TRUE,
	);

	$form['settings']['sirius_pcct_cost_calculation_header'] = array(
		'#title' => t('Cost Calculation Header'),
		'#type' => 'text_format',
		'#default_value' => sirius_domain_variable_get('sirius_pcct_cost_calculation_header'),
		'#description' => t('Enter the header for the cost calculation area. This is generally something like "The items or services you have requested may be subject to medical management requirements...".'),
	);

	$form['settings']['sirius_pcct_cost_calculation_footer'] = array(
		'#title' => t('Cost Calculation Footer'),
		'#type' => 'text_format',
		'#default_value' => sirius_domain_variable_get('sirius_pcct_cost_calculation_footer'),
		'#description' => t('Enter the footer for the cost calculation area. This is generally something like "Contact your Fund office to see if you\'ve met your deductible and/or out-of-pocket max....".'),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_pcct_page_settings_form_validate($form, &$form_state) {
	$launch_code = $form_state['values']['settings']['sirius_pcct_launch_code'];
	if ($launch_code) {
		$launch_code = strtolower($launch_code);

		if (!preg_match('/^[a-zA-Z0-9\-]+$/', $launch_code)) {
			form_set_error('settings][sirius_pcct_launch_code', 'Launch code must contain just letters, numbers, and hyphens.');
		}
		$domain_nid = sirius_pcct_launch_code_lookup($launch_code);
		if ($domain_nid && ($domain_nid != sirius_domain_nid())) {
			form_set_error('settings][sirius_pcct_launch_code', "The launch code [$launch_code] is already in use by another domain.");
		}
	}
}

function sirius_pcct_page_settings_form_submit($form, &$form_state) {
	$form_state['values']['settings']['sirius_pcct_launch_code'] = strtolower($form_state['values']['settings']['sirius_pcct_launch_code']);

	foreach ($form_state['values']['settings'] as $name => $value) {
		if (in_array($name, array('sirius_pcct_cost_calculation_header', 'sirius_pcct_cost_calculation_footer'))) {
			$value = sirius_richtext_handle_input($value);
		}
		sirius_domain_variable_set($name, $value);
	}

	$result = sirius_addr_geocode_impl($form_state['values']['settings']['sirius_pcct_map_default_center_address']);
	if (!$result['success']) {
		drupal_set_message("Geocoding failed: " . $result['msg'], 'error');
	} else {
		sirius_domain_variable_set('sirius_pcct_map_default_center_lon', $result['point']->coords[0]);
		sirius_domain_variable_set('sirius_pcct_map_default_center_lat', $result['point']->coords[1]);
	}

	drupal_set_message("Settings saved.");
}

