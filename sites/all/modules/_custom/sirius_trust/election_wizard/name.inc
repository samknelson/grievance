<?php

function sirius_trust_election_wizard_name($info, $data, $wizard_node, $current_step) { return drupal_get_form('sirius_trust_election_wizard_name_form', $info, $data, $wizard_node, $current_step); }

function sirius_trust_election_wizard_name_form($form, &$form_state, $info, $data, $wizard_node, $current_step) {
	$worker_node = sirius_trust_election_wizard_get_worker($wizard_node);
	$policy_node = sirius_trust_election_wizard_get_policy($data);

	$html = sirius_fakerender_field('Name:', $worker_node->title);
	if ($worker_node->field_sirius_dob['und'][0]['value']) {
		$html .= sirius_fakerender_field('Date of Birth:', date('m/d/Y', strtotime($worker_node->field_sirius_dob['und'][0]['value'])));
	}
	$html .= sirius_fakerender_field('Gender:', $worker_node->field_sirius_gender_nota_calc['und'][0]['value']);
	$html .= '<br />';
	$form['name']['intro'] = array('#markup' => $html);

	$must_change = FALSE;
	if (!$worker_node->field_sirius_dob['und'][0]['value']) { $must_change = TRUE; }
	if (!$worker_node->field_sirius_gender_nota_calc['und'][0]['value']) { $must_change = TRUE; }

	$def = $data['name']['change'];
	if (!$def) { $def = 'No'; }

	$form['name']['change'] = array(
		'#type' => 'radios',
		'#options' => array(
			'No' => t('The values above are correct'),
			'Yes' => t('The values above are not correct'),
		),
		'#default_value' => $def,
		'#required' => TRUE,
		'#id' => 'change_name',
	);

	if ($must_change) {
		$form['name']['change']['#value'] = 'Yes';
		$form['name']['change']['#default_value'] = 'Yes';
		$form['name']['change']['#disabled'] = TRUE;
	}

	$fieldset_title = t('New Personal Info');

	if (!$data['name']['new']['given']) {
		$data['name']['new']['given'] = $worker_node->field_sirius_name['und'][0]['given'];
		$data['name']['new']['family'] = $worker_node->field_sirius_name['und'][0]['family'];
		$data['name']['new']['middle'] = $worker_node->field_sirius_name['und'][0]['middle'];
		$data['name']['new']['dob'] = $worker_node->field_sirius_dob['und'][0]['value'];
		$data['name']['new']['gender'] = $worker_node->field_sirius_gender['und'][0]['tid'];
		$data['name']['new']['gender_nota_val'] = $worker_node->field_sirius_gender_nota_val['und'][0]['value'];
	}


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

	$form['name']['new']['dob'] = array(
		'#type' => 'date_popup',
		'#title' => t('Date of Birth'),
		'#attributes' => array(
			'autocomplete' => 'off',
		),
		'#date_format' => 'Y-m-d',
		'#default_value' => $data['name']['new']['dob'],
	);

	$gender_options = sirius_taxonomy_load('sirius_gender', 'tid', 'name');
	$form['name']['new']['gender'] = array(
		'#type' => 'select',
		'#title' => t('Gender'),
		'#default_value' => $data['name']['new']['gender'],
		'#options' => array(NULL => t('-- Select --')) + $gender_options,
		'#id' => 'sirius_gender_dropdown',
	);

	// Figure out which tids are "none of the above"
	$nota_tids = sirius_nota_tids('sirius_gender');
	if ($nota_tids) {
		$tmp = array();
		foreach ($nota_tids as $nota_tid) {
			$tmp[] = array(":input[id=\"sirius_gender_dropdown\"]" => array('value' => $nota_tid));
		}
		$form['name']['new']['gender_nota_val'] = array(
			'#type' => 'textfield',
			'#title' => t('I Identify As'),
			'#default_value' => $data['name']['new']['gender_nota_val'],
			'#states' => array('visible' => $tmp),
		);
	}

	return sirius_trust_election_wizard_form_prep($form, $form_state, $info, $data, $wizard_node, $current_step);
}

function sirius_trust_election_wizard_name_form_validate($form, &$form_state) {
	if ($form_state['values']['name']['change'] == 'Yes') {

		$required = array(
			'given' => 'First Name', 
			'family' => 'Last Name',
			'dob' => 'Date of Birth',
			'gender' => 'Gender',
		);

		$ok = TRUE;
		foreach ($required as $key => $value) {
			if (!$form_state['values']['name']['new'][$key]) {
				form_set_error("name][new][$key", "$value field is required.");
			}
		}

		$nota_tids = sirius_nota_tids('sirius_gender');
		if (in_array($form_state['values']['name']['new']['gender'], $nota_tids)) {
			if (!$form_state['values']['name']['new']['gender_nota_val']) {
				form_set_error("name][new][gender_nota_val", "I Identify As field is required.");
			}
		}
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
	$worker_node->field_sirius_name['und'][0]['given'] = $data['name']['new']['given'];
	$worker_node->field_sirius_name['und'][0]['middle'] = $data['name']['new']['middle'];
	$worker_node->field_sirius_name['und'][0]['family'] = $data['name']['new']['family'];
	$worker_node->field_sirius_dob['und'][0]['value'] = date('Y-m-d', strtotime($data['name']['new']['dob'])) . ' 00:00:00';
	$worker_node->field_sirius_gender['und'][0]['tid'] = $data['name']['new']['gender'];
	$worker_node->field_sirius_gender_nota_val['und'][0]['value'] = $data['name']['new']['gender_nota_val'];
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
