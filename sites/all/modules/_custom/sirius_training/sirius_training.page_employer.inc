<?php

function sirius_training_node_tab_employer_access($employer_node) {
	return sirius_employer_node_tab_employer_users_access($employer_node);
}

function sirius_training_node_tab_employer($employer_node) {
	return drupal_get_form('sirius_training_node_tab_employer_form', $employer_node);
}

function sirius_training_node_tab_employer_form($form, &$form_state, $employer_node) {
	$form_state['employer_nid'] = $employer_node->nid;
	$json = json_decode($employer_node->field_sirius_json['und'][0]['value'], TRUE);

	$all_levels = sirius_training_levels_fetch_all();
	$form['#tree'] = TRUE;

	foreach ($all_levels as $ms_tid => $levels) {
		$ms_term = taxonomy_term_load($ms_tid);

		foreach ($levels as $level_code => $level) {
			$form["ms_$ms_tid"][$level_code] = array(
				'#type' => 'fieldset',
				'#title' => 'Notification Emails for ' . $ms_term->name . ': ' . $level['title'],
				'#description' => 'Enter the names and email addresses who should be notified when a worker attains this level.',
			);

			$recipients = array();
			if (is_array($json['training']["ms_$ms_tid"][$level_code]['notification_emails'])) {
				foreach ($json['training']["ms_$ms_tid"][$level_code]['notification_emails'] as $delta => $recipient) {
					if (!$recipient['name'] && !$recipient['email']) { continue; }
					$recipients[] = $recipient;
				}
			}
			for ($i=0; $i<5; ++$i) { $recipients[] = array(); }

			$form["ms_$ms_tid"][$level_code]['notification_emails']['table_intro'] = array('#markup' => '<table><tr><th>Name</th><th>Email</th></tr>');

			$delta = 0;
			foreach ($recipients as $recipient) {
				$form["ms_$ms_tid"][$level_code]['notification_emails'][$delta]['tr_intro'] = array('#markup' => '<tr>');

				$form["ms_$ms_tid"][$level_code]['notification_emails'][$delta]['name'] = array(
					'#prefix' => '<td>',
					'#suffix' => '</td>',
					'#type' => 'textfield',
					'#default_value' => $recipient['name'],
				);

				$form["ms_$ms_tid"][$level_code]['notification_emails'][$delta]['email'] = array(
					'#prefix' => '<td>',
					'#suffix' => '</td>',
					'#type' => 'textfield',
					'#default_value' => $recipient['email'],
				);

				$form["ms_$ms_tid"][$level_code]['notification_emails'][$delta]['tr_outro'] = array('#markup' => '</tr>');
				++$delta;
			}
			$form["ms_$ms_tid"][$level_code]['notification_emails']['table_outro'] = array('#markup' => '</table>');
		}
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_training_node_tab_employer_form_validate($form, &$form_state) {
	$all_levels = sirius_training_levels_fetch_all();
	if ($all_levels) {
		foreach ($all_levels as $ms_tid => $levels) {
			foreach ($levels as $level_code => $level) {
				if ($form_state['values']["ms_$ms_tid"][$level_code]['notification_emails']) {
					foreach ($form_state['values']["ms_$ms_tid"][$level_code]['notification_emails'] as $delta => $recipient) {
						if (!$recipient['email']) { continue; }
						if (!sirius_email_validate($recipient['email'])) {
							form_set_error("ms_${ms_tid}][$level_code][notification_emails][$delta][email", 'Please enter a valid email address.');
						}
					}
				}
			}
		}
	}
}

function sirius_training_node_tab_employer_form_submit($form, &$form_state) {
	$employer_node = node_load($form_state['employer_nid']);
	$json = json_decode($employer_node->field_sirius_json['und'][0]['value'], TRUE);

	$values = $form_state['values'];

	unset($values['op']);
	unset($values['submit']);
	unset($values['form_id']);
	unset($values['form_token']);
	unset($values['form_build_id']);

	$json['training'] = $values;
	$employer_node->field_sirius_json['und'][0]['value'] = json_encode($json);
	node_save($employer_node);
	drupal_set_message(t('Settings saved.'));

}