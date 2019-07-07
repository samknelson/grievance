<?php

function sirius_domain_get_nid() {
	$domain_nid = $_SESSION['sirius_domain'];
	if (!$domain_nid) {
		global $user;
		$user_full = user_load($user->uid);
		$domain_nid = $user_full->field_sirius_domain['und'][0]['target_id'];
	}

	return $domain_nid;
}

function sirius_node_access_domain($node, $op, $account) {
  // Admins can do anything
  if (user_access('sirius domain admin', $account)) { return NODE_ACCESS_ALLOW; }

  // Nobody else can see do anyhing.
  return NODE_ACCESS_DENY;
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