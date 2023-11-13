<?php

function sirius_tfa_page_provision($account, $method = 'tfa_basic_totp') {
	sirius_require('tfa_basic', 'tfa_basic.pages.inc');
	return drupal_get_form('sirius_tfa_page_provision_form', $account, $method);
}

/**
 * This is just a wrapper around tfa_basic_setup_form(), with some cleanup to improve the UX.
 */

function sirius_tfa_page_provision_form($form, &$form_state, $account, $method) {
	// We don't want to check the password every time. This is the logic that tfa_basic_setup_form() users
	$form_state['storage']['skip_password_check'] = TRUE;

	// Require a recent password in order to continue
	sirius_tfa_reauthenticate();

	// Get our setup form
	$form = tfa_basic_setup_form($form, $form_state, $account, $method);

	// I don't know why they did this, but the "cancel" button always adds an explicit submit handler. We just want ours
	if ($form['actions']) {
		foreach ($form['actions'] as $key => $value) {
			unset($form['actions'][$key]['#submit']);
		}
	}

	if ($form['output']['#markup']) {
		// Rewrite some links
		$form['output']['#markup'] = preg_replace("/\/security\/tfa/", "/sirius-tfa", $form['output']['#markup']);
	}

	return $form;
}

function sirius_tfa_page_provision_form_validate($form, &$form_state) {
	tfa_basic_setup_form_validate($form, $form_state);
}

function sirius_tfa_page_provision_form_submit($form, &$form_state) {
	tfa_basic_setup_form_submit($form, $form_state);

	// Redirect to our own page.
	if ($form_state['redirect']) {
		$form_state['redirect'] = preg_replace("/\/security\/tfa/", "/sirius-tfa", $form_state['redirect']);
	}
}
