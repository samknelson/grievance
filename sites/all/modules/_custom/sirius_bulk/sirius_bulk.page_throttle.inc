<?php

function sirius_bulk_node_tab_throttle_access($bulk_node) {
	if (sirius_domain_nid() && !sirius_domain_component_enabled('bulk')) { return FALSE; }
	return TRUE;
}

function sirius_bulk_node_tab_throttle($bulk_node) {
	return drupal_get_form('sirius_bulk_node_tab_throttle_form', $bulk_node);
}

function sirius_bulk_node_tab_throttle_form($form, &$form_state, $bulk_node) {
	$form_state['bulk_nid'] = $bulk_node->nid;
	$form['#tree'] = TRUE;

	$bulk_json = sirius_json_get($bulk_node);

	$form['throttle']['batch_size'] = array(
		'#type' => 'textfield',
		'#title' => t('Batch Size'),
		'#size' => 5,
		'#default_value' => $bulk_json['throttle']['batch_size'],
		'#description' => 'Batch size for this bulk message only. If this value is greater than the system-wide batch size for the medium, then it will be ignored.',
	);

	$medium = $bulk_node->field_sirius_bulk_medium['und'][0]['value'];
	if (in_array($medium, array('voice', 'choice'))) {

		for ($i=0; $i<4; ++$i) {
			$form['throttle']['lines'][$i] = array(
				'#type' => 'fieldset',
				'#title' => t('Line Throttle #@i', array('@i' => $i+1)),
			);

			$form['throttle']['lines'][$i]['count'] = array(
				'#type' => 'textfield',
				'#title' => 'Number of Lines',
				'#size' => 4,
				'#default_value' => $bulk_json['throttle']['lines'][$i]['count'],
			);

			$form['throttle']['lines'][$i]['from'] = array(
				'#type' => 'textfield',
				'#title' => 'Sender Phone Number',
				'#size' => 12,
				'#default_value' => $bulk_json['throttle']['lines'][$i]['from'],
			);

			$form['throttle']['lines'][$i]['to'] = array(
				'#type' => 'textfield',
				'#title' => 'Recipient Phone Number',
				'#size' => 12,
				'#default_value' => $bulk_json['throttle']['lines'][$i]['to'],
			);
		}
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_bulk_node_tab_throttle_form_submit($form, &$form_state) {
	$bulk_node = node_load($form_state['bulk_nid']);
	$bulk_json = sirius_json_get($bulk_node);
	$bulk_json['throttle'] = $form_state['values']['throttle'];
	sirius_json_set($bulk_node, $bulk_json);
	node_save($bulk_node);
	drupal_set_message("Settings saved.");
}