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
	$domain_nodes = node_load_multiple(array_keys($domains));

	$options = array(NULL => t('-- Select --'));
	foreach ($domain_nodes as $domain_nid => $domain_node) {
		// Only PCCT domains
		if (!sirius_domain_component_enabled('pcct', $domain_nid)) { continue; }

		// Only active domains
		if ($domain_node->field_sirius_active['und'][0]['value'] != 'Yes') { continue; }

		// Only if "show on global list"
		if (sirius_domain_variable_get('sirius_pcct_show_on_global_list', NULL, $domain_nid) != 'Yes') { continue; }
		
		$options[$domain_nid] = $domain_node->title;
	}

	if (count($options) < 2) {
		return sirius_form_error('There are no available health plans on this site.');
	}

	$html = variable_get('sirius_pcct_domain_list_header', '');
	if ($html) {
		$form['header'] = array('#markup' => '<div class="sirius_pcct_header">' . $html . '</div>');
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

	$html = variable_get('sirius_pcct_domain_list_footer', '');
	if ($html) {
		$form['footer'] = array('#markup' => '<div class="sirius_pcct_footer">' . $html . '</div>');
	}

	return $form;
}

function sirius_pcct_page_domain_list_form_submit($form, &$form_state) {
	sirius_domain($form_state['values']['domain']);
	drupal_goto('sirius/pcct');
}
