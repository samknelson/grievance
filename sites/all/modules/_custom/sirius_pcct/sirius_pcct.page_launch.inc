<?php

function sirius_pcct_page_launch($launch_code) {
	$domain_nid = sirius_pcct_launch_code_lookup($launch_code);

	if (!$domain_nid) {
		return sirius_form_error("The launch code $launch_code was not found.");
	}

	sirius_domain($domain_nid);
	drupal_goto('sirius/pcct');
}