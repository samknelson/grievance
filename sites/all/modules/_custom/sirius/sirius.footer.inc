<?php

function sirius_footer_html() {
	$html = variable_get('sirius_footer', '');

	// Legacy
	if (!$html) { $html = variable_get('grievance_messaging_footer', ''); }

	// Default
	if (!$html) { $html = 'The Sirius Software Suite is open source software distributed under the <a href="/grievance/license">GNU General Public License</a>.'; }

	return $html;
}

function sirius_footer_config() {
  $form = array();

  $form['sirius_footer'] = array(
    '#type' => 'textarea',
    '#title' => t('Site Name'),
    '#default_value' => sirius_footer_html(),
    '#description' => t('May contain HTML'),
  );

	return system_settings_form($form);
}
