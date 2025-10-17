<?php

function sirius_btu_page_csg_public() {
	return drupal_get_form('sirius_btu_page_csg_public_form');
}

function sirius_btu_page_csg_public_form($form, &$form_state) {
	// Embeddable
 drupal_add_http_header('X-Frame-Options', 'allow-from *');
 drupal_add_http_header('Access-Control-Allow-Origin', '*');
 drupal_add_http_header('Frame-Ancestors', '*');
 drupal_add_http_header('Content-Security-Policy', 'frame-ancestors *');

	// Flood control
	if (!sirius_flood_is_allowed('sirius_btu_csg_submit')) { return sirius_form_error('Flood control: too many attempts. Please try again later.', 'error'); }

	$csg = Sirius_Btu_Csg::getInstance();
	$csg->switch_into_main_domain();

	drupal_add_css(drupal_get_path('module', 'sirius_btu') . '/css/public.css', 'file');

	$options = array();
	$options['send_confirmation_msg'] = TRUE;
	$form = $csg->form($form, $form_state, NULL, $options);
	$form['#prefix'] = '<div class="sirius_btu_public">';
	$form['#suffix'] = '</div>';

	return $form;
}

function sirius_btu_page_csg_public_form_validate($form, &$form_state) {
	$csg = Sirius_Btu_Csg::getInstance();
	$csg->switch_into_main_domain();
	return $csg->form_validate($form, $form_state);
}

function sirius_btu_page_csg_public_form_submit($form, &$form_state) {
	$csg = Sirius_Btu_Csg::getInstance();
	$csg->switch_into_main_domain();

	// Flood control
	if (!sirius_flood_is_allowed('sirius_btu_csg_submit')) { drupal_set_message('Flood control: too many attempts. Please try again later.', 'error'); }
	sirius_flood_register_event('sirius_btu_csg_submit');

	$csg->form_submit($form, $form_state);
	drupal_goto('csg/thanks');
}
