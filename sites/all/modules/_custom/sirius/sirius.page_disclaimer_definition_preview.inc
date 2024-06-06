<?php 

function sirius_node_tab_disclaimer_definition_preview_form($form, &$form_state, $disclaimer_node) {
	$form['#tree'] = TRUE;
	$entity_node = sirius_esignature_test_node();
	$entity_json = sirius_json_get($entity_node);

	$disclaimer_json = sirius_json_get($disclaimer_node)['disclaimer'];
	$acceptance_json = $entity_json['disclaimer'][$disclaimer_node->nid]['acceptances'][0];

	$form_state['disclaimer_nid'] = $disclaimer_node->nid;
	$form_state['entity_nid'] = $entity_node->nid; 

	$form['disclaimer'] = sirius_disclaimer_render_form($disclaimer_json, $acceptance_json);

	// Just for kicks, we're going to let you sign the disclaimer.
	sirius_esignature_init();
	$esig_which = 'esig_for_' . $disclaimer_node->nid;
	if (!$acceptance_json) {
		sirius_esignature_form_prepare(
			$form, 
			$esig_which, 
			$disclaimer_json['title'], 
			$disclaimer_json['body'], 
			array(
				'hide_title' => TRUE,
				'hide_statement' => TRUE,
				'hide_submit' => TRUE,
			)
		);
	} else {
		if ($acceptance_json['esig_nid']) {
			$esig_node = node_load($acceptance_json['esig_nid']);
			$esig_json = sirius_json_get($esig_node);
			$esignature = sirius_jsonfield_util_esignature_get($esig_json, $esig_which);
			if ($esignature) { 
				$html .= sirius_esignature_render($esignature, '', $esig_node->nid, $esig_which);
				$form['esig'] = array('#markup' => $html);
			}
		}
	}

	if ($acceptance_json) {
		$form['action'] = array('#type' => 'value', '#value' => 'revoke');
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Revoke'),
			'#id' => 'sirius_esig_submit',
		);
	} else {
		$form['action'] = array('#type' => 'value', '#value' => 'accept');
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit'),
			'#id' => 'sirius_esig_submit',
		);
	}

	return $form;
}

function sirius_node_tab_disclaimer_definition_preview_form_submit($form, &$form_state) {
	$disclaimer_node = node_load($form_state['disclaimer_nid']);
	$disclaimer_json = sirius_json_get($disclaimer_node)['disclaimer'];
	$esig_which = 'esig_for_' . $disclaimer_node->nid;

	$entity_node = node_load($form_state['entity_nid']);
	$entity_json = sirius_json_get($entity_node);

	if ($form_state['values']['action'] == 'accept') {
		// Save the esignature
		$entity_json = sirius_json_get($entity_node);
		$signature = sirius_esignature_form_get_json(
			$form, 
			$form_state,
			$esig_which
		);
		sirius_jsonfield_util_esignature_set($entity_json, $esig_which, $signature);
		drupal_set_message("Saving signature ");
		sirius_json_set($entity_node, $entity_json);
		node_save($entity_node);

		// Accept the disclaimer
		sirius_disclaimer_accept(
			$entity_node, 
			array($disclaimer_node), 
			array(
				'acceptance' => array(
					'esig_nid' => $entity_node->nid,
				),
			)
		);

		drupal_set_message("Disclaimer accepted.");
	} else {
		unset($entity_json['disclaimer']);
		unset($entity_json[$entity_which]);
		sirius_json_set($entity_node, $entity_json);
		node_save($entity_node);

		drupal_set_message("Disclaimer revoked.");
	}
}