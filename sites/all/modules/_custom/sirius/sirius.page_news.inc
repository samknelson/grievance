<?php

function sirius_page_news() {
	global $user;
	$domain = sirius_domain();
	if ($domain) { $domain_nid = $domain->nid; }
	$html = views_embed_view('sirius_news', 'block_1', join('+', array_keys($user->roles)), $domain_nid);
	if (drupal_valid_path('sirius/news/admin')) {
		$html .= l('News Admin', 'sirius/news/admin', array('attributes' => array('class' => array('tool-button'))));
	}

	return $html;
}


function sirius_page_news_admin() {
	global $user;
	$domain = sirius_domain();
	if ($domain) { $domain_nid = $domain->nid; }
	return views_embed_view('sirius_news_admin', 'block_1', $domain_nid);
}
