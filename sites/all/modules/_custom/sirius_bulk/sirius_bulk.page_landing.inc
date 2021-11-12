<?php

function sirius_bulk_page_landing_access() {
	if (!user_access('sirius bulk send')) { return FALSE; }
	if (sirius_domain_nid() && !sirius_domain_component_enabled('bulk')) { return FALSE; }
	return TRUE;
}

function sirius_bulk_page_landing() {
	return views_embed_view('sirius_bulk_admin', 'block_1');
}