<?php

function sirius_ledger_page_stripe_paymentmethod_add() {
	sirius_require('sirius_ledger', 'sirius_ledger.stripe.inc');
	$service = sirius_ledger_service();
	$mode = $service->mode();
	$account_id = sirius_domain_variable_get('sirius_ledger_stripe_' . $mode . '_account_id');

	global $base_url;
	$js_parameters = array(
		'mode' => $mode,
		'account_id' => $account_id,
		'client_secret' => $_SESSION['client_secret'],
		'return_url' => $base_url . '/' . $_REQUEST['destination'],
	);

  drupal_add_css('sites/all/modules/_custom/sirius_ledger/css/stripe.css');
  drupal_add_js('sites/all/modules/_custom/sirius_ledger/js/stripe.js', 'file');
	drupal_add_js(array('sirius_ledger_stripe' => $js_parameters), array('type' => 'setting'));
	drupal_add_js('https://js.stripe.com/v3/', 'file');

	$html = '<div class="sirius_stripe_wrapper">';

	$html .= '<div class="sirius_stripe_loading">';
	$html .= '<div class="ajax-progress"><div class="throbber"></div>Loading payment form ...</div>';
	$html .= '</div>';

	$html .= '<form id="payment-form">';
  $html .= '<div id="payment-element">';
  $html .= '<!-- Elements will create form elements here -->';
  $html .= '</div>';
  $html .= '<button id="submit">Submit</button>';
  $html .= '<div id="error-message">';
  $html .= '<!-- Display error message to your customers here -->';
  $html .= '</div>';
  $html .= '</form>';
  $html .= '</div>';

  return $html;
}
