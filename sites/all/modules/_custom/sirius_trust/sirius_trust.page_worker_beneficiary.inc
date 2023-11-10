<?php

function sirius_trust_node_tab_worker_beneficiary($worker_node) {
  $result = sirius_proxy_call('sirius_trust_worker_beneficiary', $worker_node);
  if (!$result['success']) { return sirius_page_error($result['msg']); }
  $beneficiary_rnode = $result['data'];

  $html = '<h2>' . t('Current Beneficiary Designation') . '</h2>';
  if (!$beneficiary_rnode) {
     $html .= '<p>' . t('There is no current beneficiary designation.') . '</p>';
  } else {
    $renderable = node_view($beneficiary_rnode, 'full');
    $html .= drupal_render($renderable);
  }

  return $html;
}












/*
function sirius_trust_node_tab_worker_beneficiary_form($form, &$form_state, $worker_node) {
	$form_state['worker_nid'] = $worker_node->nid;

	$worker_json = sirius_json_get($worker_node);

	$benefits = sirius_trust_benefits();

	$form['#tree'] = TRUE;

	foreach ($benefits as $benefit_nid => $benefit_node) {
		$subform = sirius_trust_beneficiary_form($form, $form_state, $worker_node, $benefit_node);
		if (!$subform) { continue; }
		$form['beneficiaries'][$benefit_nid] = array(
			'#type' => 'fieldset',
			'#title' => $benefit_node->title,
			'#collapsible' => FALSE,
			'#collapsed' => FALSE,
		);
		$form['beneficiaries'][$benefit_nid]['beneficiary'] = $subform;
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_trust_node_tab_worker_beneficiary_form_submit($form, &$form_state) {
	$worker_node = node_load($form_state['worker_nid']);

	if ($form_state['values']['beneficiaries']) {
		foreach ($form_state['values']['beneficiaries'] as $benefit_nid => $beneficiary_json) {
			$benefit_node = node_load($benefit_nid);
			$result = sirius_trust_beneficiary_set($worker_node, $benefit_node, $beneficiary_json['beneficiary']);
			drupal_set_message($result['msg']);
		}
	}

	drupal_set_message("Beneficiaries saved.");
}
*/