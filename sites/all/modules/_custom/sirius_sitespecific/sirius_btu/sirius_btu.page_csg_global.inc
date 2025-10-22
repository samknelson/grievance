<?php

function sirius_btu_page_csg_global() {
	return drupal_get_form('sirius_btu_page_csg_global_form');
}

function sirius_btu_page_csg_global_form($form, &$form_state) {
	$form_state['#tree'] = TRUE;

	$form['sirius_btu_csg_main_domain_nid'] = array(
		'#title' => t('Main Domain'),
		'#type' => 'select',
		'#description' => t('Which domain is the "host" for unauthenticated, publicly-submitted CSG entries?'),
		'#options' => array(NULL => t('-- Select --')) + sirius_domain_tree_render_as_select(),
		'#default_value' => variable_get('sirius_btu_csg_main_domain_nid', NULL),
	);

	$form['sirius_btu_csg_public_thanks'] = array(
		'#title' => t('Completion Text'),
		'#type' => 'text_format',
		'#description' => 'Thank-you text to be shown to the user when a public form is submitted. This can be previewed <a href="/csg/thanks">here</a>.',
		'#default_value' => sirius_richtext_handle_input(variable_get('sirius_btu_csg_public_thanks', NULL)),
	);

	$form['sirius_btu_csg_public_confirmation_msg_top'] = array(
		'#title' => t('Confirmtion Message Top'),
		'#type' => 'text_format',
		'#description' => 'Text to be sent at the top of the confirmation email',
		'#default_value' => sirius_richtext_handle_input(variable_get('sirius_btu_csg_public_confirmation_msg_top', NULL)),
	);

	$form['sirius_btu_csg_public_confirmation_msg_bottom'] = array(
		'#title' => t('Confirmtion Message Bottom'),
		'#type' => 'text_format',
		'#description' => 'Text to be sent at the bottom of the confirmation email',
		'#default_value' => sirius_richtext_handle_input(variable_get('sirius_btu_csg_public_confirmation_msg_bottom', NULL)),
	);

	return system_settings_form($form);
}