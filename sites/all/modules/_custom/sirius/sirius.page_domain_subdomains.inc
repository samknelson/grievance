<?php

function sirius_node_tab_sirius_domain_subdomains_access($domain_node) {
	if (user_access('sirius admin')) { return TRUE; }
	if (!user_access('sirius domain admin')) { return FALSE; }
	return sirius_domain_access($domain_node->nid);
}	

function sirius_node_tab_sirius_domain_subdomains($domain_node) {
	if ($domain_node->field_sirius_domain['und'][0]['target_id']) {
		$domain_node_parent = node_load($domain_node->field_sirius_domain['und'][0]['target_id']);
		$backlink = l('[Back to ' . $domain_node_parent->title . ']', 'node/' . $domain_node_parent->nid . '/sirius_domain_subdomains');
	} else {
		$backlink = l('[Back to root domains]', '/sirius/domains');
	}

	$html = '';
	$html .= '<div class="sirius_form_intro">' . $backlink . '</div>';
	$html .= views_embed_view('sirius_domains', 'block_1', $domain_node->nid);
	return $html;
}


function sirius_page_domains() {
	drupal_goto('sirius/domain/switch');
	/*
	if (user_access('sirius admin')) {
		drupal_set_title('Root Domains');
		return views_embed_view('sirius_domains', 'block_2');
	} else {
		$root_domain_node = sirius_domain_root();
		if (!$root_domain_node) { return 'Non-admin, no domain configured.'; }
		drupal_set_title('Root Domains: ' . $root_domain_node->title);
		return views_embed_view('sirius_domains', 'block_1', $root_domain_node->nid);
	}
	*/
}
