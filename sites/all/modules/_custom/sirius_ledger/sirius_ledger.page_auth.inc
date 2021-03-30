<?php

function sirius_ledger_node_tab_auth_access($auth_node) {
	if ($auth_node->field_sirius_category['und'][0]['value'] != 'ledger:auth') { return FALSE; }
	return TRUE;
}

function sirius_ledger_node_tab_auth($auth_node) {
	drupal_add_css(drupal_get_path('module', 'sirius_ledger') . '/css/auth.css');

	$html = '';

	$auth_json = sirius_jsonfield_util_node_json_get($auth_node);

	$settings_json = $auth_json['auth']['settings'];
	if ($settings_json) {
		$html .=  sirius_fakerender_field('Title:', $settings_json['title']);
		if ($settings_json['comments']) {
			$html .=  sirius_fakerender_field('Comments:', sirius_text_to_html($settings_json['comments']));
		}
	}

	$accounts = sirius_ledger_auth_entities($auth_node, 'sirius_ledger_account');
	$events = sirius_ledger_auth_entities($auth_node, 'sirius_event');
	if ($accounts || $events) {
		// $html .= '<div class="sirius_ledger_auth_accounts_events_wrapper">';
		if ($accounts) {
			// $html .= '<div class="sirius_ledger_auth_accounts">';
			// $html .= '<h2>' . t('Accounts') . '</h2>';
			foreach ($accounts as $entity_nid => $entity_node) {
				$field =  $entity_node->title;
				$field .= ' [' . l('View', "node/$entity_nid") . ']';
				$field .= ' [' . l('Authorizations', "node/$entity_nid/sirius_ledger_account_auths") . ']';
				$html .= sirius_fakerender_field('Account:', $field);
			}
			// $html .= '</div>';
		}
		if ($events) {
			// $html .= '<div class="sirius_ledger_auth_events">';
			// $html .= '<h2>' . t('Events') . '</h2>';
			foreach ($events as $entity_nid => $entity_node) {
				$field =  $entity_node->title;
				$field .= ' [' . l('View', "node/$entity_nid") . ']';
				$field .= ' [' . l('Authorizations', "node/$entity_nid/sirius_ledger_account_auths") . ']';
				$html .= sirius_fakerender_field('Event:', $field);
			}
			// $html .= '</div>';
		}
		// $html .= '</div>';
	}

	$items = sirius_ledger_auth_items($auth_node);
	if ($items) {
		$html .= '<h2>' . t('Items') . '</h2>';
		$html .= sirius_ledger_auth_render_items($auth_node);
	}

	$defs = sirius_esignature_defs($auth_node);
	if ($defs) {
		sirius_esignature_init();

		$html .= '<h2>' . t('Signatures') . '</h2>';
		foreach ($defs as $def) {
			$name = $def['title'];
			$code = $def['code'];
			$esignature = sirius_jsonfield_util_esignature_get($auth_json, $code);
			$html .= sirius_esignature_render($esignature, 'full', $auth_node->nid, $code);
			$html .= '<br />';
		}
	}

	if (!$html) {
		$html = '<p>' . t('(Empty authorization -- no items, no funds, no events.)') . '</p>';
	}

	return $html;
}