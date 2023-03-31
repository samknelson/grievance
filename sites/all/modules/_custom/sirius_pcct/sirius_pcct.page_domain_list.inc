<?php

/**
 * Pick a domain.
 * 
 * @todo: Really, you should be picking a plan. But for the time being we're assuming one plan per domain.
 */

function sirius_pcct_page_domain_list() {
	return drupal_get_form('sirius_pcct_page_domain_list_form');
}

function sirius_pcct_page_domain_list_form($form, &$form_state) {
	$domains = sirius_domain_tree_render_as_select();

	$options = array(NULL => t('-- Select --'));
	foreach ($domains as $domain_nid => $ignore) {
		if (!sirius_domain_component_enabled('pcct', $domain_nid)) { continue; }
		$options[$domain_nid] = sirius_node_title($domain_nid);
	}

	$form['domain'] = array(
		'#type' => 'select',
		'#title' => t('Choose Your Current Health Plan'),
		'#options' => $options,
		'#required' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}