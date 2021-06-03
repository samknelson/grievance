<?php

function sirius_bulk_node_tab_body_sms_access($bulk_node) {
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

	$form['sms'] = array(
		'#title' => t('SMS Body'),
		'#type' => 'textarea',
		'#default_value' => $bulk_node->field_sirius_sms['und'][0]['value'],
		'#required' => TRUE,
		'#id' => 'sms_body',
		'#description' => sirius_sms_segment_count('sms_body'),
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
	$bulk_node->field_sirius_sms['und'][0]['value'] = $form_state['values']['sms'];
  node_submit($bulk_node);
	node_save($bulk_node);
	drupal_set_message("Message body saved.");
}