<?php

function sirius_node_tab_letter_template_recipients($template_node) {
	return drupal_get_form('sirius_node_tab_letter_template_recipients_form', $template_node);
}

function sirius_node_tab_letter_template_recipients_form_validate($form, &$form_state) {
	$recipient_count = $form_state['recipient_count'];

	// Verify all email addresses
	for ($delta = 0; $delta < $recipient_count; ++$delta) {
		if ($form_state['values']["disposition_$delta"] == 'skip') { continue; }
		if (!sirius_email_validate($form_state['values']["email_$delta"])) { form_set_error("email_$delta", "Please enter a valid email address."); }
	}
}

function sirius_node_tab_letter_template_recipients_form($form, &$form_state, $template_node) {
	$form_state['template_nid'] = $template_node->nid;

	$json = json_decode($template_node->field_sirius_json['und'][0]['value'], TRUE);

	$form['#tree'] = TRUE;
	$form['intro'] = array('#markup' => '<table><tr><th>Description</th><th>Name</th><th>Email</th><th>Disposition</th><th>Locked?</th></tr>');

	$recipients = $json['recipients'];
	if (!$recipients) { $recipients = array(); } else { usort($recipients, function($a, $b) { return $a['delta'] <=> $b['delta']; }); }
	for ($i=0; $i<5; ++$i) {
		$recipients[] = array();
	}

	$content_types = sirius_fieldvals($template_node, 'field_sirius_letter_content_type', 'value');
	if (in_array('sirius_worker', $content_types)) {
		$levels = sirius_training_levels_fetch_all();
		if ($levels) {
			$level_options = array(NULL => t('-- Select --'));
			foreach ($levels as $ms_tid => $ms_levels) {
				$ms_term = taxonomy_term_load($ms_tid);
				foreach ($ms_levels as $level_code => $level) {
					$level_options["$ms_tid|$level_code"] = $ms_term->name . ': ' . $level['title'];
				}
			}
			$form['recipient_options']['level_notification'] = array(
				'#type' => 'select',
				'#options' => $level_options,
				'#title' => 'Include Leveling Recipients',
				'#default_value' => $json['recipient_options']['level_notification'],
			);
		}
	}

	$delta = 0;
	foreach ($recipients as $recipient) {
		$form["tr_$delta"] = array('#markup' => '<tr>');

		$form["description_$delta"] = array(
			'#prefix' => '<td>',
			'#suffix' => '</td>',
			'#type' => 'textfield',
			'#default_value' => $recipient['description'],
			'#size' => 20,
		);

		$form["name_$delta"] = array(
			'#prefix' => '<td>',
			'#suffix' => '</td>',
			'#type' => 'textfield',
			'#default_value' => $recipient['name'],
			'#size' => 20,
		);

		$form["email_$delta"] = array(
			'#prefix' => '<td>',
			'#suffix' => '</td>',
			'#type' => 'textfield',
			'#default_value' => $recipient['email'],
			'#size' => 30,
		);

		$form["disposition_$delta"] = array(
			'#prefix' => '<td>',
			'#suffix' => '</td>',
			'#type' => 'select',
			'#default_value' => $recipient['disposition'],
			'#options' => array(
				'skip' => t('Available'), 
				'from' => t('From'), 
				'to' => t('To'), 
				'cc' => t('Cc'), 
				'bcc' => t('Bcc')
			)
		);

		$form["locked_$delta"] = array(
			'#prefix' => '<td>',
			'#suffix' => '</td>',
			'#type' => 'select',
			'#default_value' => $recipient['locked'],
			'#options' => array(
				'no' => t('No'),
				'yes' => t('Yes'),
			)
		);

		$form["tr2_$delta"] = array('#markup' => '<tr>');
		++$delta;
	}
	$form_state['recipient_count'] = $delta;

	$form['outro'] = array('#markup' => '</table>');

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_node_tab_letter_template_recipients_form_submit($form, &$form_state) {
	$template_node = node_load($form_state['template_nid']);
	$json = json_decode($template_node->field_sirius_json['und'][0]['value'], TRUE);

	$template_node->field_sirius_json['und'][0]['value'] = json_encode($json);
	$recipient_count = $form_state['recipient_count'];
	$recipients = array();
	for ($delta = 0; $delta < $recipient_count; ++$delta) {
		$recipient = array(
			'delta' => $delta,
			'disposition' => $form_state['values']["disposition_$delta"],
			'name' => $form_state['values']["name_$delta"],
			'description' => $form_state['values']["description_$delta"],
			'email' => $form_state['values']["email_$delta"],
			'locked' => $form_state['values']["locked_$delta"],
		);
		if (!$recipient['email'] && !$recipient['name']) { continue; }
		$recipients[] = $recipient;
	}
	$json['recipients'] = $recipients;

	$json['recipient_options'] = $form_state['values']['recipient_options'];

	$template_node->field_sirius_json['und'][0]['value'] = json_encode($json);
	node_save($template_node);

	drupal_set_message('Settings saved.');
}