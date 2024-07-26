<?php

function sirius_node_tab_contact_tags_display_form($form, &$form_state, $tag_term) {
	$form_state['tag_tid'] = $tag_term->tid;
	$form['#tree'] = TRUE;

	$form['contact_tags']['display']['landing'] = array(
		'#type' => 'select',
		'#title' => t('Show on Main Page?'),
		'#options' => array(
			NULL => t('-- Select --'),
			'No' => t('No'),
			'Yes' => t('Yes'),
		),
	);

	$tag_json = sirius_json_get($tag_term);
	sirius_jsonfield_util_set_defaults($form['contact_tags']['display'], $tag_json['contact_tags']['display']);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_node_tab_contact_tags_display_form_submit($form, &$form_state) {
	$tag_term = taxonomy_term_load($form_state['tag_tid']);
	$tag_json = sirius_json_get($tag_term);
	$tag_json['contact_tags']['display'] = $form_state['values']['contact_tags']['display'];
	sirius_json_set($tag_term, $tag_json);
	taxonomy_term_save($tag_term);
	drupal_set_message("Settings saved.");
}