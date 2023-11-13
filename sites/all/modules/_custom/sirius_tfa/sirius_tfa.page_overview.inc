<?php

function sirius_tfa_page_overview($account) {
	// Require a recent password in order to continue
	sirius_tfa_reauthenticate();

	// Load tfa_basic functions
	sirius_require('tfa_basic', 'tfa_basic.pages.inc');

	// Taking a shortcut here. "tfa_basic_overview" is technically a form,
	// but it basically just generates a big batch of nested #markup elements.
	// Rather than rewriting the entire UX, we're just going to grab that HTML
	// and rewrite it slightly.
	// $form = drupal_get_form('tfa_basic_overview', $account);
	// Except now, we're copying-and-pasting, to make it look a little tidier.

	$html = '';

  $user_tfa = tfa_basic_get_tfa_data($account);
  $enabled = isset($user_tfa['status']) && $user_tfa['status'] ? TRUE : FALSE;

  $html .= '<h2>' . t('TFA Status') . '</h2>';
  if (!empty($user_tfa)) {
    if ($enabled) {
      $html .= sirius_fakerender_field('Status:', 'TFA Enabled');
    }
  } else {
    $html .= sirius_fakerender_field('Status:', 'TFA Disabled');
  }
  if ($user_tfa['saved']) {
		$html .= sirius_fakerender_field('Date:', format_date($user_tfa['saved']));
	}

  // Start with validate plugin setup.
  if (!$enabled) {
    $validate_plugin = variable_get('tfa_validate_plugin', '');
    $html .= drupal_render(_tfa_basic_plugin_setup_form_overview($validate_plugin, $account, $user_tfa));
  }
  else {
    // TOTP setup.
    $html .= drupal_render(_tfa_basic_plugin_setup_form_overview('tfa_basic_totp', $account, $user_tfa));
    // SMS setup.
    $html .= drupal_render(_tfa_basic_plugin_setup_form_overview('tfa_basic_sms', $account, $user_tfa));
    // Trusted browsers.
    $html .= drupal_render(_tfa_basic_plugin_setup_form_overview('tfa_basic_trusted_browser', $account, $user_tfa));
    // Recovery codes.
    $html .= drupal_render(_tfa_basic_plugin_setup_form_overview('tfa_basic_recovery_code', $account, $user_tfa));
  }

  // I'm doing some lazy formatting here -- literally just editing the generated HTML from the form overview built by tfa_basic.
  // This is ugly as heck. I should instead by copy-pasting the form, then generating my own HTML.

	// We want links to go to our own page
	$html = preg_replace("/\/security\/tfa\//", "/sirius-tfa/", $html);

	// Lazy formatting. Really should just rewrite the _tfa_basic_plugin_setup_form_overview
	$html = preg_replace('/h3/', 'h2', $html);

  // Combine multiple links into one paragraph
  $html = preg_replace(
    '/<p><a href=(.+)<\/p><p><a href=(.+)<\/p>/', 
    '<p><a href=${1}<a href=${2}</p>',
    $html
  );

  // Links become tool buttons
  $html = preg_replace("/a href=/", "a class=\"tool-button\" href=", $html);

	return $html;
}
