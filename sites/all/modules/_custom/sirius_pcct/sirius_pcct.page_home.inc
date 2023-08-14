<?php

function sirius_pcct_page_home() {
	if (sirius_domain_nid()) {
		drupal_goto('sirius/pcct/landing');
	} else {
		drupal_goto('user');
	}
}