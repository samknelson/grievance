<?php

/************************************************************************************************
 * Step 3: Recipients
 ************************************************************************************************/

function sirius_letter_wizard_recipients_email($info, $data, $letter_node, $current_step) { return drupal_get_form('sirius_letter_wizard_recipients_email_form', $info, $data, $letter_node, $current_step); }

function sirius_letter_wizard_recipients_email_form($form, &$form_state, $info, $data, $letter_node, $current_step) {
	form_load_include($form_state, 'inc', 'sirius', 'sirius.letter_wizard.recipients_email');

	$form_state['info'] = $info;
	$form_state['data'] = $data;
	$form_state['letter_nid'] = $letter_node->nid;
	$form_state['current_step'] = $current_step;
	$form['#tree'] = TRUE;

	$entity_node = node_load($letter_node->field_sirius_log_handler['und'][0]['target_id']);
	$steps = sirius_wizard_steps($info, $data, $entity_node);

  $ajax_params = $form_state['triggering_element']['#sirius_ajax'];

	$items = array_values($data['recipients']);
	if (!$items) { $items = array(); }
	if ($items) { uksort($items, function($a, $b) use ($items) { return $items[$a]['weight'] <=> $items[$a]['weight']; }); }

	if ($form_state['item_count']) {
		$item_count = $form_state['item_count'];
	} else {
		$item_count = count($items);
	}
  $item_count += $ajax_params['qty'];
  if ($item_count < 1) { $item_count = 1; }
  $form_state['item_count'] = $item_count;

  for ($i=count($items); $i<$item_count; ++$i) {
  	$items[] = array();
  }

	$form['items']['intro'] = array('#markup' => '<div id="sirius_item_list_wrapper"><table><tr><th>Description</th><th>Name</th><th>Email</th><th>Disposition</th></tr>');

	$delta = 0;
	foreach ($items as $delta => $item) {
		$form['items'][$delta]["tr"] = array('#markup' => '<tr>');

		$form['items'][$delta]["nid"] = array('#type' => 'hidden', '#value' => $item['nid']);
		$form['items'][$delta]["uid"] = array('#type' => 'hidden', '#value' => $item['uid']);

		$form['items'][$delta]["description_markup"] = array('#markup' => '<td>' . $item['description'] . '</td>');
		$form['items'][$delta]["description"] = array('#type' => 'hidden', '#value' => $item['description']);

		$form['items'][$delta]["name"] = array(
			'#prefix' => '<td>',
			'#suffix' => '</td>',
			'#type' => 'textfield',
			'#default_value' => $item['name'],
			'#size' => 20,
		);

		$form['items'][$delta]["email"] = array(
			'#prefix' => '<td>',
			'#suffix' => '</td>',
			'#type' => 'textfield',
			'#default_value' => $item['email'],
			'#size' => 20,
		);

		$form['items'][$delta]["disposition"] = array(
			'#prefix' => '<td>',
			'#suffix' => '</td>',
			'#type' => 'select',
			'#default_value' => $item['disposition'],
			'#options' => array('skip' => t('Skip'), 'Reply-To' => t('From'), 'to' => t('To'), 'cc' => t('Cc'), 'bcc' => t('Bcc'))
		);

		if ($item['locked'] == 'yes') {
			$form['items'][$delta]["name"]['#disabled'] = TRUE;
			$form['items'][$delta]["email"]['#disabled'] = TRUE;
			$form['items'][$delta]["disposition"]['#disabled'] = TRUE;
		}

		$form['items'][$delta]["tr2"] = array('#markup' => '</tr>');
		++$delta;
	}

	$form['items']['outro'] = array('#markup' => '</table></div>');

  $form['add_one'] = array(
    '#type' => 'button',
    '#value' => t('Add one'),
    '#ajax' => array(
      'callback' => 'sirius_ajax_generic_replace',
    ),
    '#sirius_ajax' => array(
    	'qty' => 1,
    	'wrapper' => '#sirius_item_list_wrapper',
    	'element' => 'items',
    ),
    '#limit_validation_errors' => array(),
  );

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_letter_wizard_recipients_email_form_validate($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$letter_node = node_load($form_state['letter_nid']);
	$current_step = $form_state['current_step'];

	$entity_node = node_load($letter_node->field_sirius_log_handler['und'][0]['target_id']);

	$found_from = FALSE;
	$found_to = 0;
	$item_count = $form_state['item_count'];
	for ($delta = 0; $delta < $item_count; ++$delta) {
		$item = $form_state['values']['items'][$delta];
		if ($item["disposition"] == 'skip') { continue; }

		if ($item["disposition"] == 'Reply-To') {
			if ($found_from) { form_set_error("items][$delta][disposition", 'Select exactly one recipient as the sender.'); }
			$found_from = TRUE;
		}

		if ($item["disposition"] == 'to') {
			++$found_to;
		}

		if (!$item['email']) { form_set_error("items][$delta][email", "Email address cannot be blank."); }
		else if (!sirius_email_validate($item['email'])) { form_set_error("items][$delta][email", "Please enter a valid email address (" . $item['email'] . ")"); }
	}
	if (!$found_from) { form_set_error("items][0][disposition", 'Select exactly one recipient as the sender.'); }
	if (!$found_to) { form_set_error("items][0][disposition", 'Select at least one recipient as the main recipient'); }

}

function sirius_letter_wizard_recipients_email_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$letter_node = node_load($form_state['letter_nid']);
	$current_step = $form_state['current_step'];
	$recipient_count = $form_state['recipient_count'];

	$entity_node = node_load($letter_node->field_sirius_log_handler['und'][0]['target_id']);

	$items = array();
	$item_count = $form_state['item_count'];
	for ($delta = 0; $delta < $item_count; ++$delta) {
		$item = $form_state['values']['items'][$delta];
		if (!$item['email'] && !$item['name']) { continue; }
		$items[] = $item;
	}
	$data['recipients'] = $items;

	sirius_letter_set_title($letter_node);
	sirius_wizard_submit($info, $data, $letter_node, $current_step, "Recipients saved.");
}

