<?php

function sirius_trust_election_wizard_addr($info, $data, $wizard_node, $current_step) { return drupal_get_form('sirius_trust_election_wizard_addr_form', $info, $data, $wizard_node, $current_step); }

function sirius_trust_election_wizard_addr_form($form, &$form_state, $info, $data, $wizard_node, $current_step) {
	$worker_node = sirius_trust_election_wizard_get_worker($wizard_node);

	$old_addr = $worker_node->field_sirius_address['und'][0];

	if ($old_addr['thoroughfare']) {
		$html = sirius_fakerender_field('Current Address:', sirius_addr_format_html($old_addr));
		$html .= '<br />';
		$form['addr']['intro'] = array('#markup' => $html);

		$def = $data['addr']['change'];
		if (!$def) { $def = 'No'; }

		$form['addr']['change'] = array(
			'#type' => 'radios',
			'#options' => array(
				'No' => t('Keep my current address'),
				'Yes' => t('Enter a new address'),
			),
			'#default_value' => $def,
			'#required' => TRUE,
			'#id' => 'change_addr',
		);

		$fieldset_title = t('New Address');
		// $fieldset_intro = t('Please enter your new address.');
	} else {
		$form['addr']['change'] = array(
			'#type' => 'value',
			'#value' => 'Yes',
		);

		$fieldset_title = 'Address';
		// $fieldset_intro = t('Please enter your postal address.');
	}

	$form['addr']['new'] = array(
		'#type' => 'fieldset',
		'#description' => $fieldset_intro,
		'#title' => $fieldset_title,
		'#states' => array('visible' => array(":input[name=\"addr[change]\"]" => array('value' => 'Yes'))),
	);

	$form['addr']['new']['thoroughfare'] = array(
		'#type' => 'textfield',
		'#title' => t('Address'),
		'#default_value' => $data['addr']['new']['thoroughfare'],
	);

	$form['addr']['new']['premise'] = array(
		'#type' => 'textfield',
		'#title' => t('Address 2'),
		'#default_value' => $data['addr']['new']['premise'],
	);

	$form['addr']['new']['locality'] = array(
		'#type' => 'textfield',
		'#title' => t('City'),
		'#default_value' => $data['addr']['new']['locality'],
	);

	$form['addr']['new']['administrative_area'] = array(
		'#type' => 'select',
		'#title' => t('State'),
		'#default_value' => $data['addr']['new']['administrative_area'],
		'#options' => array(NULL => t('-- Select --')) + sirius_addr_states(),
	);

	$form['addr']['new']['postal_code'] = array(
		'#type' => 'textfield',
		'#title' => t('Zip'),
		'#default_value' => $data['addr']['new']['postal_code'],
		'#size' => 5,
	);

	return sirius_trust_election_wizard_form_prep($form, $form_state, $info, $data, $wizard_node, $current_step);
}

function sirius_trust_election_wizard_addr_form_validate($form, &$form_state) {
	if ($form_state['values']['addr']['change'] == 'Yes') {

		$required = array(
			'thoroughfare' => 'Address', 
			'locality' => 'City',
			'administrative_area' => 'State',
			'postal_code' => 'Zip'
		);

		$ok = TRUE;
		foreach ($required as $key => $value) {
			if (!$form_state['values']['addr']['new'][$key]) {
				form_set_error("addr][new][$key", "$value field is required.");
				$ok = FALSE;
			}
		}
		if (!$ok) { return; }

		$addr = $form_state['values']['addr']['new'];
		$result = sirius_addr_validate_full($addr);
		if (!$result['success']) {
			form_set_error('addr', $result['msg']);
		} else {
			$form_state['values']['addr']['new']['lon'] = $result['point']->coords[0];
			$form_state['values']['addr']['new']['lat'] = $result['point']->coords[1];
		}
	}
}

function sirius_trust_election_wizard_addr_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$wizard_node = $form_state['wizard_node'];
	$current_step = $form_state['current_step'];

	$data['addr'] = $form_state['values']['addr'];
	sirius_wizard_submit($info, $data, $wizard_node, $current_step, "Address saved.", array('nonsequential' => FALSE));

	sirius_trust_election_set_title($wizard_node);
}

function sirius_trust_election_wizard_addr_render($info, $data, $wizard_node, $current_step) {
	if ($data['addr']['change'] == 'Yes') {
		$html .= sirius_fakerender_field('Address:', $data['addr']['new']['thoroughfare']);
		$html .= sirius_fakerender_field('Address 2:', $data['addr']['new']['premise']);
		$html .= sirius_fakerender_field('City:', $data['addr']['new']['locality']);
		$html .= sirius_fakerender_field('State:', $data['addr']['new']['administrative_area']);
		$html .= sirius_fakerender_field('Zip:', $data['addr']['new']['postal_code']);
	} else {
		$html .= '<p>' . t('Address is unchanged.') . '</p>';
	}

	return $html;
}

function sirius_trust_election_wizard_addr_post($info, &$data, $wizard_node, $current_step) {
	$change = $data['addr']['change'];
	if ($change != 'Yes') { return array('success' => TRUE); }

	$worker_node = sirius_trust_election_wizard_get_worker($wizard_node);
	$worker_node->field_sirius_address['und'][0] = $data['addr']['new'];
	node_save($worker_node);
	return array('success' => TRUE);
}
