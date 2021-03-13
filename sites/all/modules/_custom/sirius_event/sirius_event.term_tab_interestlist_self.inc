<?php

function sirius_event_term_tab_interestlist_self_access($event_type_term) {
	if (!sirius_event_interestlist_enabled()) { return FALSE; }
	if (user_access('sirius admin') || user_access('sirius domain admin') || user_access('sirius event admin')) { return TRUE; }
	if (!sirius_event_interestlist_enabled_byterm($event_type_term)) { return FALSE; }
	return TRUE;
}

function sirius_event_term_tab_interestlist_self($event_type_term) {
	return drupal_get_form('sirius_event_term_tab_interestlist_self_form', $event_type_term);
}

function sirius_event_term_tab_interestlist_self_form($form, &$form_state, $event_type_term) {
	drupal_add_css('sites/all/modules/_custom/sirius_event/css/event.css');

	$form_state['event_type_tid'] = $event_type_term->tid;

	$event_type_json = sirius_jsonfield_util_term_json_get($event_type_term);

	// Already registered?
	$contact_nid = 	sirius_contact_nid_get_from_user();
	$interestlist_node = sirius_event_interestlist_lookup($event_type_term, $contact_nid);

  $form['#tree'] = TRUE;
  $form['backlink'] = array('#markup' => '<div class="sirius_form_intro">' . l('[All Interest Lists]', '/sirius/event/interestlists', array('attributes' => array('class' => array('tool-button')))) . '</div>');

  $form['intro'] = array('#markup' => drupal_render(taxonomy_term_view($event_type_term)) . '<hr>');

	$domain_nid = sirius_domain_nid();

	if ($interestlist_node) {
		$interestlist_json = sirius_jsonfield_util_node_json_get($interestlist_node);
		$html = '<div class="sirius_form_intro">';
		$html .= '<strong>' . t('You are registered for this interest list.') . '</strong><br /><br />';
		$html .= sirius_fakerender_field('Signup Date', date('l, F j, Y h:i a', $interestlist_node->created));
		$comments = $interestlist_json['interestlist']['signup']['comments'];
		if ($comments) { $html .= sirius_fakerender_field('Comments', sirius_text_to_html($comments)); }
		$html .= '</div><hr>';
		$form['participant_intro'] = array('#markup' => $html);
		$form['action'] = array('#type' => 'value', '#value' => 'cancel');
		$form['confirm'] = array(
			'#type' => 'checkbox', 
			'#title' => 'I no longer want to be on this interest list.',
			'#required' => TRUE,
		);

		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Remove me from the interest list'),
			'#states' => array('enabled' => array(':input[name="confirm"]' => array('checked' => TRUE))),
		);

	} else {
		$form['action'] = array('#type' => 'value', '#value' => 'signup');

		$form['interestlist']['comments'] = array(
			'#type' => 'textarea',
			'#title' => t('Comments'),
		);

		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Add me to the Interest List'),
		);
	}

	return $form;
}

function sirius_event_term_tab_interestlist_self_form_submit($form, &$form_state) {
	$event_type_tid = $form_state['event_type_tid'];
	$event_type_term = taxonomy_term_load($event_type_tid);

	$event_type_json = sirius_jsonfield_util_term_json_get($event_type_term);

	$contact_nid = 	sirius_contact_nid_get_from_user();
	if (!$contact_nid) {
		drupal_set_message("No contact associated with this user", 'error');
		return;
	}

	$action = $form_state['values']['action'];
	if ($action == 'cancel') {
		$result = sirius_event_interestlist_cancel($event_type_term, $contact_nid);
	} else {
		$params = array('interestlist' => array('signup' => $form_state['values']['interestlist']));
		$result = sirius_event_interestlist_signup($event_type_term, $contact_nid, $params);
	}

	if (!$result['success']) {
		drupal_set_message($result['msg'], 'error'); 
		return;
	}

	drupal_set_message($result['msg']);
}
