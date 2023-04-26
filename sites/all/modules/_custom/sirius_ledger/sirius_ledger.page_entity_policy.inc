<?php

function sirius_ledger_node_tab_entity_policy($entity_node) {
	return drupal_get_form('sirius_ledger_node_tab_entity_policy_form', $entity_node);
}

function sirius_ledger_node_tab_entity_policy_form($form, &$form_state, $entity_node) {
	$form_state['entity_nid'] = $entity_node->nid;

	$json = sirius_jsonfield_util_node_json_get($entity_node);

	$policies = sirius_ledger_policy_list();
	$policy_options = array(NULL => t('-- Select --'));
	foreach ($policies as $policy_nid => $policy_node) { $policy_options[$policy_nid] = $policy_node->title; }

	$form['#tree'] = TRUE;

	$current_policy_nid = $json['ledger']['policy']['nid'];
	if ($current_policy_nid && $policy_options[$current_policy_nid]) { $current_policy_name = $policy_options[$current_policy_nid]; }
	else { $current_policy_name = '[not set]'; }
	$form['intro'] = array(
		'#markup' => sirius_fakerender_field('Current policy:', $current_policy_name) . '<br /><br />',
	);

	if ($json['ledger']['policy']['ebh']) { $items = array_values($json['ledger']['policy']['ebh']); }
	if (!$items) { $items = array(); }
	uksort($items, function($a, $b) use ($items) { return $items[$a]['date'] <=> $items[$b]['date']; });

  $ajax_params = $form_state['triggering_element']['#sirius_ajax'];

	if ($form_state['item_count']) { $item_count = $form_state['item_count']; } 
	else { $item_count = count($items); }
  $item_count += $ajax_params['qty'];
  if ($item_count < 1) { $item_count = 1; }
  $form_state['item_count'] = $item_count;
  for ($i=count($items); $i<$item_count; ++$i) { $items[] = array(); }

	$form['items']['intro'] = array('#markup' => '<div id="sirius_item_list_wrapper"><table><tr><th>Policy</th><th>Effective From</th></tr>');
	foreach ($items as $delta => $item) {
		$form['items'][$delta]['tr'] = array('#markup' => '<tr>');
		$form['items'][$delta]['policy'] = array(
			'#type' => 'select',
			'#options' => $policy_options,
			'#default_value' => $item['policy'],
			'#prefix' => '<td>',
			'#suffix' => '</td>',
		);
		$form['items'][$delta]['date'] = array(
			'#type' => 'date_popup',
			'#prefix' => '<td>',
			'#suffix' => '</td>',
			'#date_format' => 'Y-m-d',
			'#default_value' => $item['date'],
			'#attributes' => array('autocomplete' => 'off'),
		);
		$form['items'][$delta]['tr'] = array('#markup' => '<tr>');
		$form['items'][$delta]['tr2'] = array('#markup' => '</tr>');
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

function sirius_ledger_node_tab_entity_policy_form_validate($form, &$form_state) {
	$items = $form_state['values']['items'];
	$last_ts = strtotime('1000-01-01');
	foreach ($items as $delta => $item) {
		if (!$item['date'] && !$item['policy']) { continue; }
		if (!$item['date']) { 
			form_set_error("items][$delta][date", 'Please enter the date that this policy became effective.');
			continue;
		}
		if (!$item['policy']) { 
			form_set_error("items][$delta][policy", 'Please select a policy.');
			continue;
		}

		$ts = strtotime($item['date']);
		if ($ts === FALSE) {
			form_set_error("items][$delta][date", 'Please enter a valid date.');
			continue;			
		}

		if ($ts <= $last_ts) {
			form_set_error("items][$delta][date", 'Entries must be in chronological order.');
			continue;
		}

		$last_ts = $ts;
	}
}

function sirius_ledger_node_tab_entity_policy_form_submit($form, &$form_state) {
	$entity_node = node_load($form_state['entity_nid']);
	$json = sirius_jsonfield_util_node_json_get($entity_node);

	$items = $form_state['values']['items'];
	$current_policy_nid = NULL;
	foreach ($items as $delta => $item) {
		if (!$item['date'] && !$item['policy']) { 
			unset($items[$delta]); 
			continue;
		}

		if (strtotime($item['date']) <= time()) {
			$current_policy_nid = $item['policy'];
		}
	}
	$json['ledger']['policy']['ebh'] = $items;

	$json['ledger']['policy']['nid'] = $current_policy_nid;


	sirius_jsonfield_util_node_json_set($entity_node, $json);
	node_save($entity_node);
	drupal_set_message("Policy selection saved.");
}