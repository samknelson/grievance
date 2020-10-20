<?php

function sirius_domain_page_list_stats() {
	return sirius_domain_page_list_impl(array('switch' => FALSE, 'tools' => FALSE, 'stats' => TRUE));
}