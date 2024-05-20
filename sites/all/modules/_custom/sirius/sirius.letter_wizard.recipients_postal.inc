<?php

/************************************************************************************************
 * Step 3: Recipients
 ************************************************************************************************/

function sirius_letter_wizard_recipients_postal($info, $data, $letter_node, $current_step) { return drupal_get_form('sirius_letter_wizard_recipients_postal_form', $info, $data, $letter_node, $current_step); }

function sirius_letter_wizard_recipients_postal_form($form, &$form_state, $info, $data, $letter_node, $current_step) {
	form_load_include($form_state, 'inc', 'sirius', 'sirius.letter_wizard.recipients_postal');

	$items = array_values($data['recipients']);
	if (!$items) { $items = array(); }
	if ($items) { uksort($items, function($a, $b) use ($items) { return $items[$a]['weight'] <=> $items[$a]['weight']; }); }
	if (!sirius_domain_component_enabled('postal')) { return sirius_form_error('Postal module is not enabled.'); }
	$instance = sirius_postal_client_instance();
	if (!$instance) { return sirius_form_error('No postal client has been configured.'); }

	$form_state['info'] = $info;
	$form_state['data'] = $data;
	$form_state['letter_nid'] = $letter_node->nid;
	$form_state['current_step'] = $current_step;
	$form['#tree'] = TRUE;

	$entity_node = node_load($letter_node->field_sirius_log_handler['und'][0]['target_id']);
	$steps = sirius_wizard_steps($info, $data, $entity_node);

	$form['items'][0] = array(
		'#type' => 'fieldset',
		'#title' => t('Recipient'),
	);
	$form['items'][0]["disposition"] = array(
		'#type' => 'value',
		'#value' => 'from',
	);
	$form['items'][0]['addr'] = $instance->addr_form();

	$form['items'][1] = array(
		'#type' => 'fieldset',
		'#title' => t('Sender'),
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
	);
	$form['items'][1]["disposition"] = array(
		'#type' => 'value',
		'#value' => 'to',
	);
	$form['items'][1]['addr'] = $instance->addr_form();

	foreach ($items	as $delta => $item) {
		if ($delta > 1) { continue; }
		$keys = array_keys($item['addr']);
		foreach ($keys as $key) {
			$form['items'][$delta]['addr'][$key]['#default_value'] = $item['addr'][$key];
		}
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_letter_wizard_recipients_postal_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$letter_node = node_load($form_state['letter_nid']);
	$current_step = $form_state['current_step'];
	$recipient_count = $form_state['recipient_count'];

	$entity_node = node_load($letter_node->field_sirius_log_handler['und'][0]['target_id']);

	$items = array();
	$item_count = $form_state['item_count'];
	for ($delta = 0; $delta <= 2; ++$delta) {
		$item = $form_state['values']['items'][$delta];
		$items[] = $item;
	}
	$data['recipients'] = $items;

	sirius_letter_set_title($letter_node);
	sirius_wizard_submit($info, $data, $letter_node, $current_step, "Recipients saved.");
}

