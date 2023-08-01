<?php

function sirius_node_tab_worker_tags_form($form, &$form_state, $worker_node) {
	$contact_node = sirius_worker_contact_get($worker_node);
	if (!$contact_node) { return sirius_form_error('There is no contact associated with that user.'); }

	sirius_require('sirius', 'sirius.page_contact_tags.inc');
	return sirius_node_tab_contact_tags_form($form, $form_state, $contact_node);
}

function sirius_node_tab_worker_tags_form_submit($form, &$form_state) {
	sirius_require('sirius', 'sirius.page_contact_tags.inc');
	return sirius_node_tab_contact_tags_form_submit($form, $form_state);
}