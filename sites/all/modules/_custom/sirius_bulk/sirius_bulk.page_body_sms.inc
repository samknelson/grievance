<?php

function sirius_bulk_node_tab_body_sms_access($bulk_node) {
	if (sirius_domain_nid() && !sirius_domain_component_enabled('bulk')) { return FALSE; }
	$medium = $bulk_node->field_sirius_bulk_medium['und'][0]['value'];
	return in_array($medium, array('sms', 'choice'));
}

function sirius_bulk_node_tab_body_sms($bulk_node) {
	return drupal_get_form('sirius_bulk_node_tab_body_sms_form', $bulk_node);
}

function sirius_bulk_node_tab_body_sms_form($form, &$form_state, $bulk_node) {
	$form_state['bulk_nid'] = $bulk_node->nid;
	$json = sirius_json_get($bulk_node);

	$form['#tree'] = TRUE;

	// Add a link shortener
	/*
  drupal_add_js(drupal_get_path('module', 'sirius') . '/js/sirius.popup.js');
  drupal_add_css(drupal_get_path('module', 'sirius') . '/css/sirius.popup.css');
  $html .= '<a href="#" class="sirius_popup_trigger tool-button">' . t('Link Shortener') . '</a>';
  $html .= '<div style="display:none" class="sirius_popup_overlay"></div>';
  $html .= '<div style="display:none" class="sirius_popup_wrap">';
  $html .= '<div class="sirius_popup_contents">';
  $shorten_block = block_load('shorten', 'shorten');
	$shorten_build = module_invoke('shorten', 'block_view', 'shorten');
	drupal_alter(array('block_view', "block_view_shorten_shorten"), $shorten_build, $shorten_block);
	$html .= $shorten_build['content'];
  $html .= '</div>';
  $html .= '<div class="sirius_popup_close"></div>';
  $html .= '</div>';
  */

  $form['#prefix'] = $html;

  $result = sirius_twilio_incoming_phone_numbers_list();
  if (!$result['success']) { return sirius_form_error($result['msg']); }
  $ipns = $result['records'];
  if (!$ipns) { return sirius_form_error(t('No phone numbers were found in the account. Please create at least one SMS-capable phone number in your Twilio account.')); }

  $ipn_options = array();
  foreach ($ipns as $ipn) {
  	$ipn_options[sirius_phone_format($ipn['phoneNumber'])] = sirius_phone_format($ipn['phoneNumber']) . ' - ' . $ipn['friendlyName'];
  	// drupal_set_message($ipn['phoneNumber'] . ': ' . $ipn['smsUrl']);
  }
	$bulk_json = sirius_json_get($bulk_node);
	$ipn_default = $bulk_json['sms']['from'];
	if (!$ipn_default) { $ipn_default = sirius_phone_format(sirius_domain_variable_get('sirius_twilio_phone', '')); }

	$form['from'] = array(
		'#title' => t('Outgoing Phone Number'),
		'#type' => 'select',
		'#required' => TRUE,
		'#options' => $ipn_options,
		'#default_value' => $ipn_default,
	);

	$form['sms']['body'] = array(
		'#title' => t('SMS Body'),
		'#type' => 'textarea',
		'#default_value' => $bulk_node->field_sirius_sms['und'][0]['value'],
		'#required' => TRUE,
		'#id' => 'sms_body',
		'#description' => sirius_sms_segment_count('sms_body'),
	);

	$form['sms']['attach']['url'] = array(
		'#type' => 'textfield',
		'#title' => t('MMS URL'),
		'#states' => array('visible' => array(":input[id=\"sms_attach_medthod\"]" => array('value' => 'url'))),
		'#default_value' => $bulk_json['sms']['attach']['url'],
		'#description' => t('If a URL is entered, the message will be sent as an MMS. Additional carrier fees may apply.'),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
		'#required' => TRUE,
	);

	return $form;
}

function sirius_bulk_node_tab_body_sms_form_submit($form, &$form_state) {
	$bulk_node = node_load($form_state['bulk_nid']);
	$bulk_node->field_sirius_sms['und'][0]['value'] = $form_state['values']['sms']['body'];

	$bulk_json = sirius_json_get($bulk_node);
	$bulk_json['sms'] = $form_state['values']['sms'];
	sirius_json_set($bulk_node, $bulk_json);
  node_submit($bulk_node);
	node_save($bulk_node);
	drupal_set_message("Message body saved.");
}