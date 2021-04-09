<?php

function sirius_event_term_tab_intake($event_type_term) {
	return drupal_get_form('sirius_event_intake_settings_form', 'taxonomy_term', $event_type_term);
}
