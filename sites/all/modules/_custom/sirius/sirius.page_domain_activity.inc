<?php

function sirius_node_tab_sirius_domain_activity($domain_node) {
	return sirius_domain_activity_render(array($domain_node->nid));
}