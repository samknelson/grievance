<?php

function sirius_bulk_node_tab_body_email_access($bulk_node) {
	$medium = $bulk_node->field_sirius_bulk_medium['und'][0]['value'];
	return in_array($medium, array('email', 'choice'));
}

function sirius_bulk_node_tab_body_email($bulk_node) {
	return drupal_get_form('sirius_bulk_node_tab_body_email_form', $bulk_node);
}

function sirius_bulk_node_tab_body_email_form($form, &$form_state, $bulk_node) {
	$form_state['bulk_nid'] = $bulk_node->nid;
	$json = sirius_json_get($bulk_node);

	$form['#tree'] = TRUE;

	// Add a link shortener
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
  $form['#prefix'] = $html;

	$form['title'] = array(
		'#title' => t('Subject'),
		'#type' => 'textfield',
		'#default_value' => $bulk_node->title,
		'#description' => t('Will be used as the subject for an email. The subject is <strong>not used</strong> for SMS (text messages) or audio messages (robocalls).'),
		'#required' => TRUE,
	);

	$form['body'] = array(
		'#title' => t('Email Body'),
		'#type' => 'text_format',
		'#default_value' => $bulk_node->body['und'][0]['value'],
		'#format' => $bulk_node->body['und'][0]['text_format'],
		'#required' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
		'#required' => TRUE,
	);

	return $form;
}

function sirius_bulk_node_tab_body_email_form_submit($form, &$form_state) {
	$bulk_node = node_load($form_state['bulk_nid']);
	$bulk_node->title = $form_state['values']['title'];
	$bulk_node->body['und'][0]['value'] = $form_state['values']['body']['value'];
	$bulk_node->body['und'][0]['text_format'] = $form_state['values']['body']['text_format'];
  node_submit($bulk_node);
	node_save($bulk_node);
	drupal_set_message("Message body saved.");
}