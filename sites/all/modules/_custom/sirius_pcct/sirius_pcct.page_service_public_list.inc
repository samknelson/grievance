<?php

function sirius_pcct_node_tab_service_public_list_access($service_node) {
	$result = sirius_pcct_node_access_util($service_node->nid, 'sirius_trust_service');
	if (!$result['success']) { return FALSE; }
	return TRUE;
}


function sirius_pcct_node_tab_service_public_list_form($form, &$form_state, $service_node) {
	$form_state['service_nid'] = $service_node->nid;
	$form['#tree'] = TRUE;

	$service_json = sirius_json_get($service_node);

	// An ugly and cumbersome way to figure out if providers are "providers" or "facilities"
	$service_type_tid = $service_node->field_sirius_trust_service_type['und'][0]['tid'];
	$result = sirius_trust_service_type_hardcode_lookup();
	if (!$result['success']) { return sirius_page_error($result['msg']); }
	$service_type_record = array();
	foreach ($result['data'] as $key => $record) {
		if ($record['tid'] == $service_type_tid) {
			$service_type_record = $record;
			break;
		}
	}

	// Render the service description
	$description = $service_node->body['und'][0]['value'];
	if ($description) {
		$html = '<div class="sirius_pcct_service_description_wrap">';
		$html .= $description;
		$html .= '</div>';
		$form['description'] = array('#markup' => $html);
	}

	$form['filters'] = array(
		'#type' => 'fieldset',
		'#title' => t('Filters'),
	);

	$form['filters']['title'] = array(
		'#type' => 'textfield',
	);

	$form['filters']['distance'] = array(
		'#type' => 'select',
		'#title' => t('Distance'),
		'#options' => array(
			NULL => t('-- Any Distance --'),
			5 => t('5 Miles'),
			10 => t('10 Miles'),
			25 => t('25 Miles'),
			50 => t('50 Miles'),
		),
		'#id' => 'filters_distance'
	);

	$form['filters']['location_type'] = array(
		'#title' => t('Distance From'),
		'#type' => 'select',
		'#options' => array(
			'latlon' => t('My current location'),
			'address' => t('Address ...'),
		),
		'#states' => array('visible' => array(
			array(":input[id=\"filters_distance\"]" => array('!value' => ''))
		)),
		'#id' => 'filters_location_type'
	);

	$form['filters']['address'] = array(
		'#title' => t('Address'),
		'#type' => 'textfield',
		'#states' => array(
			'visible' => array(
				array(":input[id=\"filters_location_type\"]" => array('value' => 'address'))
			)
		),
	);

	drupal_add_js(drupal_get_path('module', 'sirius_pcct') . '/js/provider_search_form.js');

	$form['filters']['geolocation'] = sirius_map_geolocation_render_form();

	$form['filters']['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);



	if ($service_type_record['is_facility']) {
		$form['filters']['title']['#title'] = t('Facility Name');
	} else {
		$form['filters']['title']['#title'] = t('Provider Name');
	}

	// Render the rates by provider level (if any)
	/*
	if ($service_json['rates']['by_level']) {
		$rates_by_level = array_filter($service_json['rates']['by_level']);
	} else {
		$rates_by_level = array();
	}
	if ($rates_by_level) {
		$level_lookup = sirius_taxonomy_load('sirius_trust_provider_level', 'tid', 'name');

		$rows = array();
		foreach ($level_lookup as $level_tid => $level_name) {
			if (!$rates_by_level[$level_tid]) { continue; }

			$row = array();
			$row[] = $level_name;
			$row[] = '$' . number_format($rates_by_level[$level_tid], 2);
			$rows[] = $row;
		}

		$header = array();
		$header[] = t('License Level');
		$header[] = t('Rate');

		$html .= '<div class="sirius_pcct_service_rates_by_level_wrap">';
		$html .= '<h2>' . t('Estimated out-of-pocket rates by license level') . '</h2>';
		$html .= sirius_table_render($header, $rows, array('class' => array('sirius_pcct_service_rates_by_level')));
		$html .= '</div>';
	}
	*/

	// Render the provider search list
	$params = array();
	if ($form_state['values']['filters']['title']) {
		$params['title_match'] = '%' . DatabaseConnection::escapeLike($form_state['values']['filters']['title']) . '%';
	}

	// If we're doing a distance search...
	if ($form_state['values']['filters']['distance'] > 0) {

		if ($form_state['values']['filters']['location_type'] == 'latlon') {

			// We're using "current location". Just take the lat / lon from the input
			$lat = $form_state['values']['filters']['geolocation']['latlon']['lat'];
			$lon = $form_state['values']['filters']['geolocation']['latlon']['lon'];

		} else {

			// User specified an address. We have to geocode it.
			$address = $form_state['values']['filters']['address'];
			if (!$address) {
				drupal_set_message("Please enter your address.", 'warning');
			} else {
				$result = sirius_addr_geocode_impl($address, TRUE);
				if (!$result['success']) {
					drupal_set_message("Address lookup failed: " . $result['msg']);
				} else {
					$lon = $result['point']->coords[0];
					$lat = $result['point']->coords[1];
				}
			}

		}

		if ($lat || $lon) {
			$params['lat'] = $lat;
			$params['lon'] = $lon;
			$params['distance'] = $form_state['values']['filters']['distance'];
		}
	}

	$providers = sirius_trust_service_provider_search($service_node, $params);

	$count = count($providers);
	$page_size = 100;
  $page = pager_find_page();
  $offset = $page_size * $page;
  $pager = pager_default_initialize($count, $page_size);
  $providers = array_slice($providers, $offset, $page_size, TRUE);

	$html = '<div class="sirius_pcct_service_provider_listwrap">';

	$html .= '<h2>';
	if ($service_type_record['is_facility']) { $html .= t('List of Facilities'); } 
	else { $html .= t('List of Providers'); }
	$html .= '</h2>';

	if (!$providers) {
		$html .= '<p><em>' . t('No matching records found.') . '</em></p>';
	} else {

		$rows = array();
		foreach ($providers as $provider_nid => $provider_record) {
			$provider_node = node_load($provider_nid);

			$row = array();
			$row[] = $provider_node->title;
			// $row[] = '$' . number_format($rate, 2);
			$row[] = $provider_node->field_sirius_phone['und'][0]['value'];
			$row[] = sirius_addr_format_html($provider_node->field_sirius_address['und'][0]);
			$row[] = l('View', "node/$provider_nid/sirius_pcct_provider_public/" . $service_node->nid, array('attributes' => array('class' => array('tool-button'))));
			$rows[] = $row;
		}

		$header = array();
		$header[] = t('Name');
		// $header[] = t('Rate');
		$header[] = t('Phone');
		$header[] = t('Address');
		$header[] = t('');

		$html .= sirius_table_render($header, $rows, array('class' => array('sirius_pcct_providers')));
	}
	$html .= '</div>';
	$form['list'] = array('#markup' => $html);

	$form['pager'] = array('#markup' => theme('pager'));

	return $form;
}

function sirius_pcct_node_tab_service_public_list_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
}