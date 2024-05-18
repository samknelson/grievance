<?php

function sirius_worker_create_settings($new_settings = NULL) {
	if (isset($new_settings)) {
		sirius_domain_variable_set('sirius_worker_create_settings', $new_settings);
	}

	return sirius_domain_variable_get('sirius_worker_create_settings', array());
}