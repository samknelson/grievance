<?php

function sirius_lang_options_global() {
	require_once DRUPAL_ROOT . '/includes/iso.inc';
	$x = _locale_get_predefined_list();
	$options = array();
	foreach ($x as $key => $y) {
		$options[$key] = $y[0];
	}
	return $options;
}

function sirius_lang_options() {
	$options = sirius_str_to_options(sirius_domain_variable_get('sirius_lang_options'));
	if (!$options) { return sirius_lang_options_global(); }
	return $options;
}

function sirius_lang_node_view(&$node) {
	if (!$node->content['langs']) { return; }

	if ($node->type == 'sirius_contact') { $source_node = $node; }
	else if ($node->type == 'sirius_worker') { $source_node = sirius_worker_contact_get($node); }

	if (!$source_node) {
		unset($node->content['langs'][0]['#markup']);
		return;
	}

  $renderable = field_view_field('node', $source_node, 'field_sirius_lang', 'full');
  $node->content['langs'][0]['#markup'] = render($renderable);
}

function sirius_contact_lang_primary($contact_node) {
	$lang = $contact_node->field_sirius_lang['und'][0]['value'];
	if (!$lang) { $lang = 'en'; }
	return $lang;
}

function sirius_contact_lang_all($contact_node) {
	if (!$contact_node->field_sirius_lang['und'][0]['value']) { return array('en'); }

	$result = array();
	foreach ($contact_node->field_sirius_lang['und'] as $tmp ) {
		if ($tmp['value']) {
			$result[] = $tmp['value'];
		}
	}
	return $result;
}