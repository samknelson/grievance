<?php

/************************************************************************************************
 * Step 3: Recipients (Phone)
 ************************************************************************************************/

function sirius_letter_wizard_recipients_phone($info, $data, $letter_node, $current_step) { return drupal_get_form('sirius_letter_wizard_recipients_phone_form', $info, $data, $letter_node, $current_step); }

function sirius_letter_wizard_recipients_phone_form($form, &$form_state, $info, $data, $letter_node, $current_step) {
	$form_state['info'] = $info;
	$form_state['data'] = $data;
	$form_state['letter_nid'] = $letter_node->nid;
	$form_state['current_step'] = $current_step;

	$entity_node = node_load($letter_node->field_sirius_log_handler['und'][0]['target_id']);
	$steps = sirius_wizard_steps($info, $data, $entity_node);

	$recipients = $data['recipients'];
	if ($recipients) { usort($recipients, function($a, $b) { 
			if ($a['delta'] < $b['delta']) { return -1; }
			if ($a['delta'] > $b['delta']) { return 1; }
			return 0;
		});
	}
	for ($i=0; $i<5; ++$i) {
		$recipients[] = array('description' => t('Additional Recipient #@i', array('@i' => ($i+1))));
	}

	$form['intro'] = array('#markup' => '<table><tr><th>Description</th><th>Name</th><th>Phone</th><th>Disposition</th></tr>');

	$delta = 0;
	foreach ($recipients as $recipient) {
		$form["tr_$delta"] = array('#markup' => '<tr>');

		$form["description_markup_$delta"] = array('#markup' => '<td>' . $recipient['description'] . '</td>');
		$form["description_$delta"] = array('#type' => 'hidden', '#value' => $recipient['description']);

		$form["name_$delta"] = array(
			'#prefix' => '<td>',
			'#suffix' => '</td>',
			'#type' => 'textfield',
			'#default_value' => $recipient['name'],
			'#size' => 20,
		);

		$form["phone_$delta"] = array(
			'#prefix' => '<td>',
			'#suffix' => '</td>',
			'#type' => 'textfield',
			'#default_value' => $recipient['phone'],
			'#size' => 20,
		);

		$form["disposition_$delta"] = array(
			'#prefix' => '<td>',
			'#suffix' => '</td>',
			'#type' => 'select',
			'#default_value' => $recipient['disposition'],
			'#options' => array('skip' => t('Skip'), 'send' => t('Send'))
		);


		$form["tr2_$delta"] = array('#markup' => '<tr>');
		++$delta;
	}

	$form['outro'] = array('#markup' => '</table>');
	$form_state['recipient_count'] = count($recipients);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_letter_wizard_recipients_phone_form_validate($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$letter_node = node_load($form_state['letter_nid']);
	$current_step = $form_state['current_step'];
	$recipient_count = $form_state['recipient_count'];

	$entity_node = node_load($letter_node->field_sirius_log_handler['und'][0]['target_id']);

	// Verify at least one "Send"
	$found_send = 0;
	for ($delta = 0; $delta < $recipient_count; ++$delta) {
		if ($form_state['values']["disposition_$delta"] == 'send') {
			++$found_send;
		}
	}
	if (!$found_send) { form_set_error("disposition_0", 'Select at least one recipient to send to'); }

	// Verify all email addresses
	for ($delta = 0; $delta < $recipient_count; ++$delta) {
		if ($form_state['values']["disposition_$delta"] == 'skip') { continue; }
		$phone = $form_state['values']["phone_$delta"];
		if (!sirius_phone_validate($phone)) { form_set_error("phone_$delta", "Please enter a valid phone address for recipient #$delta (" . $phone . ")"); }
	}
}

function sirius_letter_wizard_recipients_phone_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$letter_node = node_load($form_state['letter_nid']);
	$current_step = $form_state['current_step'];
	$recipient_count = $form_state['recipient_count'];

	$entity_node = node_load($letter_node->field_sirius_log_handler['und'][0]['target_id']);

	$recipients = array();
	for ($delta = 0; $delta < $recipient_count; ++$delta) {
		$recipient = array(
			'delta' => $delta,
			'disposition' => $form_state['values']["disposition_$delta"],
			'name' => $form_state['values']["name_$delta"],
			'description' => $form_state['values']["description_$delta"],
			'phone' => $form_state['values']["phone_$delta"],
		);
		if (!$recipient['phone'] && !$recipient['name']) { continue; }
		$recipients[] = $recipient;
	}
	$data['recipients'] = $recipients;

	sirius_letter_set_title($letter_node);
	sirius_wizard_submit($info, $data, $letter_node, $current_step, "Recipients saved.");
}
