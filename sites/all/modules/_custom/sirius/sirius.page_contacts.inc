<?php

function sirius_page_contacts() {
	drupal_set_message("Contact list under development! This is a placeholder view only.", 'warning');
	$domain_node = sirius_domain();
	if (!$domain_node) {
		$html .= views_embed_view('sirius_contacts', 'page');
	} else {
		$html .= views_embed_view('sirius_contacts', 'block_1', $domain_node->nid);
	}
	return $html;
}