<?php

function sirius_btu_page_csg_public_thanks() {
	drupal_add_css(drupal_get_path('module', 'sirius_btu') . '/css/public.css', 'file');
	// Embeddable
 drupal_add_http_header('X-Frame-Options', 'allow-from *');
 drupal_add_http_header('Access-Control-Allow-Origin', '*');
 drupal_add_http_header('Frame-Ancestors', '*');
 drupal_add_http_header('Content-Security-Policy', 'frame-ancestors *');

	$val = variable_get('sirius_btu_csg_public_thanks', "Thank you for contacting us!<br />We will get back to you as soon as possible.");
	return sirius_richtext_handle_input($val);
}