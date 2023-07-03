<?php


function sirius_pcct_node_tab_policy_settings_access($policy_node) {
	return $policy_node->field_sirius_type['und'][0]['value'] == 'sirius_ledger:policy';
}

function sirius_pcct_node_tab_policy_settings_form($form, &$form_state, $policy_node) {
	$form_state['policy_nid'] = $policy_node->nid;
	$json = sirius_json_get($policy_node);
	$pcct_json = $json['pcct'];

	$form['#tree'] = TRUE;

	$form['pcct']['settings']['deductible_display'] = array(
		'#title' => t('Deductible (Display)'),
		'#type' => 'textfield',
		'#default_value' => $pcct_json['settings']['deductible_display'],
		'#description' => t('Enter the deductible as it should be displayed to members, e.g. "$250 ind / $500 fam".'),
	);

	$form['pcct']['settings']['oop_display'] = array(
		'#title' => t('Out-of-pocket (Display)'),
		'#type' => 'textfield',
		'#default_value' => $pcct_json['settings']['oop_display'],
		'#description' => t('Enter the out-of-pocket max as it should be displayed to members, e.g. "$5,000 ind / $10,000 fam".'),
	);




	$form['pcct']['settings']['mbbh_outpatient_copay_display'] = array(
		'#title' => t('MH/BH Outpatient Copay (Display)'),
		'#type' => 'textfield',
		'#default_value' => $pcct_json['settings']['mbbh_outpatient_copay_display'],
		'#description' => t('Copay as it should be rendered, i.e. "$15".'),
	);

	$form['pcct']['settings']['mbbh_outpatient_copay'] = array(
		'#title' => t('MH/BH Outpatient Copay (number)'),
		'#type' => 'textfield',
		'#default_value' => $pcct_json['settings']['mbbh_outpatient_copay'],
		'#size' => 4,
		'#description' => t('Copay as it should be computed, i.e. "15".'),
	);

	$form['pcct']['settings']['mbbh_outpatient_copay_units'] = array(
		'#title' => t('MH/BH Outpatient Copay (units)'),
		'#type' => 'select',
		'#default_value' => $pcct_json['settings']['mbbh_outpatient_copay_units'],
		'#options' => array(
			NULL => t('-- Select --'),
			'$' => '$',
			'%' => '%',
		),
		'#description' => t('Copay units'),
	);




	$form['pcct']['settings']['mbbh_inpatient_coinsurance_display'] = array(
		'#title' => t('MH/BH Inpatient Coinsurance (Display)'),
		'#type' => 'textfield',
		'#default_value' => $pcct_json['settings']['mbbh_inpatient_coinsurance_display'],
		'#description' => t('Coinsurance as it should be rendered, i.e. "10%".'),
	);

	$form['pcct']['settings']['mbbh_inpatient_coinsurance'] = array(
		'#title' => t('MH/BH Inpatient Coinsurance (Number)'),
		'#type' => 'textfield',
		'#default_value' => $pcct_json['settings']['mbbh_inpatient_coinsurance'],
		'#size' => 4,
		'#description' => t('Coinsurance as it should be computed, i.e. "10".'),
	);

	$form['pcct']['settings']['mbbh_inpatient_coinsurance_units'] = array(
		'#title' => t('MH/BH Inpatient Coinsurance (units)'),
		'#type' => 'select',
		'#default_value' => $pcct_json['settings']['mbbh_inpatient_coinsurance_units'],
		'#options' => array(
			NULL => t('-- Select --'),
			'$' => '$',
			'%' => '%',
		),
		'#description' => t('Coinsurance units'),
	);

	$form['pcct']['settings']['description'] = array(
		'#title' => t('Policy Description'),
		'#type' => 'text_format',
		'#default_value' => $pcct_json['settings']['description'],
		'#description' => t('Enter a policy description as it should appear to members.'),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	$html = '<hr><h2>' . t('Preview') . '</h2>';
	$html .= sirius_pcct_policy_render($policy_node);
	$form['preview'] = array('#markup' => $html);

	return $form;
}

function sirius_pcct_node_tab_policy_settings_form_submit($form, &$form_state) {
	$policy_node = node_load($form_state['policy_nid']);
	$json = sirius_json_get($policy_node);
	foreach (array('description') as $name) {
		$form_state['values']['pcct']['settings'][$name] = sirius_richtext_handle_input($form_state['values']['pcct']['settings'][$name]);
	}
	$json['pcct']['settings'] = $form_state['values']['pcct']['settings'];
	sirius_json_set($policy_node, $json);
	node_save($policy_node);
	drupal_set_message('Settings saved.');
}