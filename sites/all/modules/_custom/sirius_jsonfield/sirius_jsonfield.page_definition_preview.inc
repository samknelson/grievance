<?php

function sirius_json_definition_preview_form($form, &$form_state, $nid, $which_form = 'default') {
	$form_state['nid'] = $nid;
	$node = node_load($nid);
	$json = sirius_jsonfield_util_node_json_get($node);
	$json_form = sirius_jsonfield_util_form_get($json, $which_form);

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