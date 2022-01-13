<?php

function sirius_node_tab_phonenumber_scan($phonenumber_node) {
	return drupal_get_form('sirius_page_phonenumber_check_form', $phonenumber_node);
}
