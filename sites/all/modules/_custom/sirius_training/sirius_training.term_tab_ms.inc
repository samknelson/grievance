<?php

function sirius_training_term_tab_ms($ms_term) {
	return drupal_get_form('sirius_term_tab_ms_form', $ms_term);
}

function sirius_term_tab_ms_form($form, &$form_state, $ms_term) {
	$form_state['ms_tid'] = $ms_term->tid;
	$json = json_decode($ms_term->field_sirius_json['und'][0]['value'], TRUE);

	$form['#tree'] = TRUE;

	$form['levels_enabled'] = array(
		'#title' => t('Levels Enabled?'),
		'#type' => 'select',
		'#options' => array(
			'No' => t('No'),
			'Yes' => t('Yes'),
		),
		'#default_value' => $json['training']['levels_enabled'],
	);

	$departments = sirius_taxonomy_load('grievance_department', 'tid', 'name', TRUE);
	$departments['TOTAL'] = t('TOTAL HOURS');

	$levels = sirius_training_levels_fetch($ms_term);
	$levels['new'] = array('title' => t('New Level'));

	foreach ($levels as $level_code => $level) {
		$form['levels'][$level_code] = array(
			'#type' => 'fieldset',
			'#title' => $level['title'],
		);

		$form['levels'][$level_code]['code'] = array(
			'#title' => t('Code'),
			'#type' => 'textfield',
			'#default_value' => $level['code'],
			'#description' => t('Enter the code for this level, a unique identifier with only letters, numbers, and the underscore character')
		);

		$form['levels'][$level_code]['title'] = array(
			'#title' => t('Title'),
			'#type' => 'textfield',
			'#default_value' => $level['title'],
			'#description' => t('The level title as it will be displayed to users')
		);

		$form['levels'][$level_code]['payrate_skilled'] = array(
			'#title' => t('Pay Rate (Skilled)'),
			'#type' => 'textfield',
			'#default_value' => $level['payrate_skilled'],
			'#size' => 4,
		);

		$form['levels'][$level_code]['payrate_unskilled'] = array(
			'#title' => t('Pay Rate (Unskilled)'),
			'#type' => 'textfield',
			'#default_value' => $level['payrate_unskilled'],
			'#size' => 4,
		);

		$form['levels'][$level_code]['weight'] = array(
			'#title' => t('Weight'),
			'#type' => 'textfield',
			'#default_value' => $level['weight'],
			'#description' => t('Numeric value used for sorting'),
			'#size' => 4,
		);

		$form['levels'][$level_code]['notification_emails'] = array(
			'#title' => t('Notification Emails'),
			'#type' => 'textarea',
			'#default_value' => $level['notification_emails'],
			'#description' => t('Enter email addresses which should be notified when a worker attains this level, one per line.')
		);

		$hour_requirements = $level['hour_requirements'];
		if (!$hour_requirements) { $hour_requirements = array(); }
		for ($i=0; $i<3; ++$i) {
			$form['levels'][$level_code]['hour_requirements'][$i]['dept'] = array(
				'#type' => 'select',
				'#title' => t('Hour Type #@i', array('@i' => $i+1)),
				'#options' => $departments,
				'#default_value' => $hour_requirements[$i]['dept'],
			);

			$form['levels'][$level_code]['hour_requirements'][$i]['qty'] = array(
				'#type' => 'textfield',
				'#title' => t('Count #@i', array('@i' => $i+1)),
				'#default_value' => $hour_requirements[$i]['qty'],
				'#size' => 4,
			);
		}
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}



function sirius_term_tab_ms_form_submit($form, &$form_state) {
	$ms_tid = $form_state['ms_tid'];
	$ms_term = taxonomy_term_load($ms_tid);
	$json = json_decode($ms_term->field_sirius_json['und'][0]['value'], TRUE);

	$values = $form_state['values'];

	unset($values['op']);
	unset($values['submit']);
	unset($values['form_id']);
	unset($values['form_token']);
	unset($values['form_build_id']);

	$new_code = $values['levels']['new']['code'];
	if ($new_code) { $values['levels'][$new_code] = $values['levels']['new']; }
	unset($values['levels']['new']);

	$json['training'] = $values;
	$ms_term->field_sirius_json['und'][0]['value'] = json_encode($json);
	taxonomy_term_save($ms_term);
	drupal_set_message(t('Settings saved.'));
}
