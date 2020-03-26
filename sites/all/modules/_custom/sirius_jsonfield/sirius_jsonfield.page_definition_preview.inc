<?php

function sirius_json_definition_preview_form($form, &$form_state, $nid, $which_form = 'default') {
	$form_state['nid'] = $nid;
	$node = node_load($nid);
	$json = json_decode($node->field_sirius_json['und'][0]['value'], TRUE);
	$json_form = $json['form'][$which_form];
	if ($json['form_encoded'][$which_form]) { $json_form = json_decode($json['form_encoded'][$which_form], TRUE); }

	$json_form['!intro'] = array(
		'#markup' => '<div class="sirius_form_intro">' . 
		'<h2>' .
		t('Form Preview') .
		'</h2>' .
		'<hr>' .
		'</div>' . 
		'<div class="sirius-form">',
		'#weight' => -9999,
	);

	$json_form['!outro'] = array(
		'#markup' => '</div>',
		'#weight' => 9999,
	);

	return $json_form;
}