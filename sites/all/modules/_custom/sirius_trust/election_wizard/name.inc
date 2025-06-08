<?php

function sirius_trust_election_wizard_name($info, $data, $wizard_node, $current_step) { return drupal_get_form('sirius_trust_election_wizard_name_form', $info, $data, $wizard_node, $current_step); }

function sirius_trust_election_wizard_name_form($form, &$form_state, $info, $data, $wizard_node, $current_step) {
	$worker_node = sirius_trust_election_wizard_get_worker($wizard_node);
	$policy_node = sirius_trust_election_wizard_get_policy($data);

	$html = sirius_fakerender_field('Current Name:', $worker_node->title);
	$html .= '<br />';
	$form['name']['intro'] = array('#markup' => $html);

	$def = $data['name']['change'];
	if (!$def) { $def = 'No'; }

	$form['name']['change'] = array(
		'#type' => 'radios',
		'#options' => array(
			'No' => t('Keep my current name'),
			'Yes' => t('Enter a new name'),
		),
		'#default_value' => $def,
		'#required' => TRUE,
		'#id' => 'change_name',
	);

	$fieldset_title = t('New Name');

	$form['name']['new'] = array(
		'#type' => 'fieldset',
		'#description' => $fieldset_intro,
		'#title' => $fieldset_title,
		'#states' => array('visible' => array(":input[name=\"name[change]\"]" => array('value' => 'Yes'))),
	);

	$form['name']['new']['given'] = array(
		'#type' => 'textfield',
		'#title' => t('First Name'),
		'#default_value' => $data['name']['new']['given'],
	);

	$form['name']['new']['middle'] = array(
		'#type' => 'textfield',
		'#title' => t('Middle Name(s)'),
		'#default_value' => $data['name']['new']['middle'],
	);

	$form['name']['new']['family'] = array(
		'#type' => 'textfield',
		'#title' => t('Last Name'),
		'#default_value' => $data['name']['new']['family'],
	);

	return sirius_trust_election_wizard_form_prep($form, $form_state, $info, $data, $wizard_node, $current_step);
}

function sirius_trust_election_wizard_name_form_validate($form, &$form_state) {
	if ($form_state['values']['name']['change'] == 'Yes') {

		$required = array(
			'given' => 'First Name', 
			'family' => 'Last Name'
		);

		$ok = TRUE;
		foreach ($required as $key => $value) {
			if (!$form_state['values']['name']['new'][$key]) {
				form_set_error("name][new][$key", "$value field is required.");
				$ok = FALSE;
			}
		}
		if (!$ok) { return; }
	}
}

function sirius_trust_election_wizard_name_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$wizard_node = $form_state['wizard_node'];
	$current_step = $form_state['current_step'];

	$data['name'] = $form_state['values']['name'];
	sirius_wizard_submit($info, $data, $wizard_node, $current_step, "Name saved.", array('nonsequential' => FALSE));

	sirius_trust_election_set_title($wizard_node);
}

function sirius_trust_election_wizard_name_render($info, $data, $wizard_node, $current_step) {
	if ($data['name']['change'] == 'Yes') {
		$html .= sirius_fakerender_field('First Name:', $data['name']['new']['given']);
		$html .= sirius_fakerender_field('Middle Name(s):', $data['name']['new']['middle']);
		$html .= sirius_fakerender_field('Last Name:', $data['name']['new']['family']);
	} else {
		$html .= '<p>' . t('Name is unchanged.') . '</p>';
	}

	return $html;
}

function sirius_trust_election_wizard_name_post($info, &$data, $wizard_node, $current_step) {
	$change = $data['name']['change'];
	if ($change != 'Yes') { return array('success' => TRUE); }

	$worker_node = sirius_trust_election_wizard_get_worker($wizard_node);
	$worker_node->field_sirius_name['und'][0] = $data['name']['new'];
	node_save($worker_node);
	return array('success' => TRUE);

	/*
	$queue_item = array();
	$queue_item['source_nid'] = $wizard_node->nid;
	$queue_item['type'] = 'node_field';
	$queue_item['node_field'] = array(
		'nid' => $worker_node->nid,
		'field_name' => 'field_sirius_name',
		'value' => $data['name']['new'],
	);

	$cq = Sirius_Trust_Changequeue::getInstance();
	return $cq->queue($queue_item);
	*/
}
