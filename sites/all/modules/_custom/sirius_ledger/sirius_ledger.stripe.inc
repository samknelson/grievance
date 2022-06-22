<?php

function sirius_ledger_stripe_client_get($mode = NULL) {
	if (!$mode) { $mode = sirius_ledger_stripe_mode(); }

	$stripe = &drupal_static(__FUNCTION__);

	if (!isset($stripe[$mode])) {

		if (!sirius_domain_component_enabled('ledger:stripe')) {
			sirius_minilog("The Stripe component is not enabled for this domain.", 'error');
			return NULL; 
		}

		if (!sirius_ledger_stripe_active()) {
			sirius_minilog("Stripe is not active for this domain.", 'error');
			return NULL; 
		}

		try {
			require_once DRUPAL_ROOT . '/sites/all/modules/_custom/sirius_ledger/stripe-php/init.php';

			$stripe[$mode] = new \Stripe\StripeClient(sirius_domain_variable_get('sirius_ledger_stripe_' . $mode . '_account_secret'));
		} catch (\Exception $e) {
			sirius_minilog("Stripe connection error: " . $e->getMessage(), 'error');
			return NULL;
		}

	}

	return $stripe[$mode];
}

function sirius_ledger_stripe_mode() {
	return sirius_domain_variable_get('sirius_ledger_stripe_mode', 'test');
}

function sirius_ledger_stripe_active() {
	return (sirius_domain_variable_get('sirius_ledger_stripe_active') == 'Yes');
}

function sirius_ledger_stripe_object_to_json($object) {
	if (!$object) { return array(); }
	return json_decode($object->toJSON(), TRUE);
}

function sirius_ledger_stripe_object_url($json) {
	$url = 'https://dashboard.stripe.com/';
	if (!$json['livemode']) { $url .= 'test/'; }

	if ($json['object'] == 'payment_intent') {
		$url .= 'payments/';
	} else if ($json['object'] == 'webhook_endpoint') {
		$url .= 'webhooks/';
	} else {
		$url .= $json['object'] . 's/';
	}
	$url .= $json['id'];
	return $url;
}

function sirius_ledger_stripe_ping($mode = NULL) {
	sirius_minilog("Stripe ping test: starting.");
	if ($mode) { sirius_minilog("Mode = $mode"); } else { sirius_minilog("Mode = System Default [" . sirius_ledger_stripe_mode() . ']'); }

	if (!sirius_ledger_stripe_active()) { return array('success' => FALSE, 'msg' => "Stripe is not active for this domain."); }

	if (!sirius_domain_variable_get('sirius_ledger_stripe_account_secret')) { return array('success' => FALSE, 'msg' => 'No secret key has been set.'); }

	$stripe = sirius_ledger_stripe_client_get($mode);
	if (!$stripe) { return array('success' => FALSE, 'msg' => 'sirius_ledger_stripe_client_get() failed.'); }

	try {
		$balance = $stripe->balance->retrieve();
		if (!$balance) { return array('success' => FALSE, 'msg' => 'Failed to retrieve account balance object.'); }
		$data = json_decode($balance->toJSON(), TRUE);
	} catch (\Exception $e) {
		return array('success' => FALSE, 'msg' => $e->getMessage());
	}

	return array('success' => TRUE, 'msg' => 'Request succeeded', 'data' => $data);
}

function sirius_ledger_stripe_payment_node_view($payment_node, $view_mode, $langcode) {
	$payment_json = sirius_json_get($payment_node);
	$paymentintent_id = $payment_json['stripe']['paymentintent']['id'];
	if (!$paymentintent_id) { return; }

  drupal_add_css('sites/all/modules/_custom/sirius_ledger/css/stripe.css');
	$payment_node->content['stripe'] = array(
		'#weight' => 100,
		'#markup' => sirius_ledger_stripe_paymentintent_render($payment_json['stripe']['paymentintent']),
	);
}

function sirius_ledger_stripe_paymentintent_render($paymentintent_json) {
	$html = '<div class="sirius_ledger_stripe_paymentintent">'; 
	$html .= '<div class="sirius_ledger_stripe_customer_links">';
	$url = sirius_ledger_stripe_object_url($paymentintent_json);
	$html .= "<a href=\"$url\" class=\"tool-button\" target=\"_blank\">";
	$html .= '<span class="sirius_ledger_stripe_link_icon">';
	$html .= '<i class="fab fa-cc-stripe"></i>';
	$html .= '</span>';
	$html .= "View in Stripe";
	$html .= '</a>';
	$html .= '</div>';
	$html .= '</div>';

	return $html;
}

/****************************************************************************************
 * Customer
 ****************************************************************************************/

function sirius_ledger_stripe_customer_sync($entity_node) {
	$stripe = sirius_ledger_stripe_client_get();
	if (!$stripe) { return array('success' => FALSE, 'msg' => 'sirius_ledger_stripe_client_get() failed.'); }

	$customer_id = $entity_node->uuid;
	if (!$customer_id) { return array('success' => FALSE, 'msg' => 'No customer_id (no uuid in the entity node).'); }

	$params = array();
	$params['name'] = $entity_node->title;
	if ($entity_node->field_sirius_phone['und'][0]['value']) { $params['phone'] = $entity_node->field_sirius_phone['und'][0]['value']; }
	if ($entity_node->field_sirius_email['und'][0]['value']) { $params['email'] = $entity_node->field_sirius_email['und'][0]['value']; }

	try {
		try { $customer = $stripe->customers->retrieve($customer_id); } catch (\Exception $ignore) {} 

		if ($customer) {
			$customer = $stripe->customers->update($customer_id, $params);
		} else {
			$params['id'] = $customer_id;
			$customer = $stripe->customers->create($params);
		}

		$data = json_decode($customer->toJSON(), TRUE);
	} catch (\Exception $e) {
		return array('success' => FALSE, 'msg' => $e->getMessage());
	}

	return array('success' => TRUE, 'data' => $data);
}

function sirius_ledger_stripe_customer_retrieve($customer_id) {
	$stripe = sirius_ledger_stripe_client_get();
	if (!$stripe) { return array('success' => FALSE, 'msg' => 'sirius_ledger_stripe_client_get() failed.'); }

	try {
		$customer = $stripe->customers->retrieve($customer_id);
	} catch (\Exception $e) {
		return array('success' => FALSE, 'msg' => $e->getMessage());
	}

	return array('success' => TRUE, 'customer' => $customer);
}

function sirius_ledger_stripe_customer_render($customer_json) {
	$classes = array();
	$classes[] = 'sirius_ledger_stripe_customer';
	if ($customer_json) { $classes[] = 'sirius_ledger_stripe_customer_active'; } 
	else  { $classes[] = 'sirius_ledger_stripe_customer_empty'; }

	$html .= '<div class="sirius_ledger_stripe_customers">';
	$html .= "<h2>Customer</h2>";

	$html .= '<div class="' . join(' ', $classes) . '">';
	$html .= 	'<div class="sirius_ledger_stripe_customer_icon">';
	$html .= '<i class="fas fa-user"></i>';
	$html .= '</div>';

	if ($customer_json) {
		$html .= '<div class="sirius_ledger_stripe_customer_name">' . $customer_json['name'] . '</div>';
		$html .= '<div class="sirius_ledger_stripe_customer_id">' . 'ID ' . $customer_json['id'] . '</div>';
		$html .= '<div class="sirius_ledger_stripe_customer_created">' . 'Created ' . date('m/d/Y H:i:s', $customer_json['created']) . '</div>';

		$url = sirius_ledger_stripe_object_url($customer_json);
		$html .= '<div class="sirius_ledger_stripe_customer_links">';
		$html .= "<a href=\"$url\" class=\"tool-button\" target=\"_blank\">";
		$html .= "View in Stripe";
		$html .= '</a>';
		$html .= '</div>';
	} else {
		$html .= '<em>' . t('No customer has been created yet.') . '</em>';
	}
	$html .= '</div>';

	$html .= '</div>';

	return $html;
}

/****************************************************************************************
 * Payment Method
 ****************************************************************************************/

function sirius_ledger_stripe_paymentmethod_list_multitype($customer_id, $types) {
	$result_json = array('success' => TRUE, 'paymentmethods' => array('data' => array()));
	foreach ($types as $type) {
		$result = sirius_ledger_stripe_paymentmethod_list($customer_id, $type);
		if (!$result['success']) { return $result; }
		$tmp = sirius_ledger_stripe_object_to_json($result['paymentmethods']);
		if ($tmp['data']) {
			foreach ($tmp['data'] as $paymentmethod) {
				$result_json['paymentmethods']['data'][] = $paymentmethod;
			}
		}
		$result_json['paymentmethods']['data'] += $tmp['data'];
	}
	return $result_json;
}

function sirius_ledger_stripe_paymentmethod_list($customer_id, $type = 'card') {
	$stripe = sirius_ledger_stripe_client_get();
	if (!$stripe) { return array('success' => FALSE, 'msg' => 'sirius_ledger_stripe_client_get() failed.'); }
	try {
		$paymentmethods = $stripe->paymentMethods->all(array('customer' => $customer_id, 'type' => $type));
	} catch (\Exception $e) {
		return array('success' => FALSE, 'msg' => $e->getMessage());
	}
	return array('success' => TRUE, 'paymentmethods' => $paymentmethods);
}

function sirius_ledger_stripe_paymentmethod_add($entity_node) {
	$stripe = sirius_ledger_stripe_client_get();
	if (!$stripe) { return array('success' => FALSE, 'msg' => 'sirius_ledger_stripe_client_get() failed.'); }

	$customer_id = $entity_node->uuid;
	if (!$customer_id) { return array('success' => FALSE, 'msg' => 'No customer_id (no uuid in the entity node).'); }

	// Get a SetupIntent
	$result = sirius_ledger_stripe_setupintent_create($customer_id);
	if (!$result['success']) { return $result; }
	$setupintent = $result['setupintent'];

	$_SESSION['client_secret'] = $setupintent->client_secret;
	drupal_goto('sirius/ledger/stripe/paymentmethod/add', array('query' => array('destination' => current_path())));

	return array('success' => TRUE);
}

function sirius_ledger_stripe_setupintent_create($customer_id) {
	$stripe = sirius_ledger_stripe_client_get();
	if (!$stripe) { return array('success' => FALSE, 'msg' => 'sirius_ledger_stripe_client_get() failed.'); }

	try {
		$setupintent = $stripe->setupIntents->create(array('customer' => $customer_id, 'payment_method_types' => array('card', 'us_bank_account')));
	} catch (\Exception $e) {
		return array('success' => FALSE, 'msg' => $e->getMessage());
	}

	return array('success' => TRUE, 'setupintent' => $setupintent);
}

function sirius_ledger_stripe_paymentmethod_charge($entity_node, $amount, $account_nid, $paymentmethod_id) {
	$stripe = sirius_ledger_stripe_client_get();
	if (!$stripe) { return array('success' => FALSE, 'msg' => 'sirius_ledger_stripe_client_get() failed.'); }

	$customer_id = $entity_node->uuid;
	if (!$customer_id) { return array('success' => FALSE, 'msg' => 'No customer_id (no uuid in the entity node).'); }

	try {
		$paymentintent = $stripe->paymentIntents->create(array(
			'amount' => 100 * $amount,
			'currency' => 'usd',
			'payment_method' => $paymentmethod_id,
			'customer' => $customer_id,
  		'off_session' => true,
			'confirm' => true,
			'payment_method_types' => array('card', 'us_bank_account'),
    ));
	} catch (\Exception $e) {
		return array('success' => FALSE, 'msg' => $e->getMessage());
	}

	//
	// Now construct the payment object. 
	// @todo this probably should go in client code or a wrapper function
	// 

	$payment_params = array();
	if ($entity_node->type == 'sirius_worker') {
		$payment_params['payer_type'] = 'worker';
		$payment_params['payer_worker'] = $entity_node->field_sirius_id['und'][0]['value'];
		$payment_params['payer_employer'] = $entity_node->field_grievance_shop['und'][0]['target_id'];
	}
	else if ($entity_type->node == 'grievance_shop') {
		$payment_params['payer_type'] = 'employer';
		$payment_params['payer_employer'] = $entity_node->nid;
	}
	else { 
		return array(
			'success' => FALSE, 
			'msg' => 'Don\'t know how to construct a payment node for an object of type ' . $entity_node->type
		);
	}

	$paymentintent_json = sirius_ledger_stripe_object_to_json($paymentintent);

	$payment_params['ledger_account'] = $account_nid;
	$payment_params['dollar_amt'] = $amount;
	$payment_params['payment_type'] = sirius_domain_variable_get('sirius_ledger_stripe_payment_type');
	$payment_params['merchant_name'] = 'Stripe';
	$payment_params['json'] = array('stripe' => array('paymentintent' => sirius_ledger_stripe_object_to_json($paymentintent)));
	$payment_params['payment_status'] = 'Cleared';
	$payment_params['allocate'] = 'Yes';
	$payment_params['datetime_created'] = date('Y-m-d H:i:s');
	$payment_params['datetime'] = $payment_params['datetime_created'];
	$payment_params['id'] = $paymentintent_json['id'];
	$form_state = array('values' => array('payment' => $payment_params));
	$result = sirius_ledger_payment_save($form_state);
	$result['paymentintent'] = $paymentintent;

	return $result;
}

function sirius_ledger_stripe_paymentmethod_detach($entity_node, $paymentmethod_id) {
	$stripe = sirius_ledger_stripe_client_get();
	if (!$stripe) { return array('success' => FALSE, 'msg' => 'sirius_ledger_stripe_client_get() failed.'); }

	$customer_id = $entity_node->uuid;
	if (!$customer_id) { return array('success' => FALSE, 'msg' => 'No customer_id (no uuid in the entity node).'); }

	try {
		$stripe->paymentMethods->detach($paymentmethod_id);
	} catch (\Exception $e) {
		return array('success' => FALSE, 'msg' => $e->getMessage());
	}

	if (sirius_ledger_stripe_paymentmethod_default_get($entity_node) == $paymentmethod_id) {
		sirius_ledger_stripe_paymentmethod_default_set($entity_node, NULL);
	}

	return array('success' => TRUE);
}

function sirius_ledger_stripe_paymentmethod_default_set($entity_node, $paymentmethod_id) {
	$entity_json = sirius_json_get($entity_node);
	$entity_json['stripe']['paymentmethod']['default'] = $paymentmethod_id;
	sirius_json_set($entity_node, $entity_json);
	node_save($entity_node);
	return array('success' => TRUE);
}

function sirius_ledger_stripe_paymentmethod_default_get($entity_node) {
	$entity_json = sirius_json_get($entity_node);
	return $entity_json['stripe']['paymentmethod']['default'];
}

function sirius_ledger_stripe_render_cc_icon($brand) {
	$html = '<div class="sirius_ledger_stripe_cc_icon">';
	if ($brand == 'mastercard') 	{ $html .= '<i class="fab fa-cc-mastercard"></i>'; } 
	else if ($brand == 'visa') 		{ $html .= '<i class="fab fa-cc-visa"></i>'; } 
	else if ($brand == 'amex') 		{ $html .= '<i class="fab fa-cc-amex"></i>'; } 
	else if ($brand == 'us_bank_account') { $html .= '<i class="fas fa-money-check"></i>'; } 
	else 													{ $html .= '<i class="fas fa-credit-card"></i>'; }
	$html .= '</div>';

	return $html;
}

function sirius_ledger_stripe_paymentmethods_render($entity_node, $paymentmethods_json) {
	// Render the payment methods
	$html .= '<div class="sirius_ledger_stripe_paymentmethods">';
	$html .= "<h2>Payment Methods</h2>";

	if (!$paymentmethods_json || !$paymentmethods_json['data'][0]) { 
		$html .= '<em>' . t('No payment methods have been created yet.') . '</em>';
	} else {
		$paymentmethod_default_id = sirius_ledger_stripe_paymentmethod_default_get($entity_node);
		foreach ($paymentmethods_json['data'] as $idx => $paymentmethod_json) {
			$paymentmethod_id = $paymentmethod_json['id'];
			$is_default = ($paymentmethod_id == $paymentmethod_default_id);
			$html .= sirius_ledger_stripe_paymentmethod_render($paymentmethod_json, $is_default);
		}
	}

	$html .= '</div>';

	return $html;
}

function sirius_ledger_stripe_paymentmethod_render($paymentmethod_json, $is_default = FALSE) {
	$classes = array();
	$classes[] = 'sirius_ledger_stripe_paymentmethod';
	if ($is_default) { $classes[] = 'sirius_ledger_stripe_paymentmethod_default '; } 
	else { $classes[] = 'sirius_ledger_stripe_paymentmethod_alternative '; }

	$html .= '<div class="' . join(' ', $classes) . '">';

	$type = $paymentmethod_json['type'];
	if ($type == 'card') {
		$html .= sirius_ledger_stripe_render_cc_icon($paymentmethod_json['card']['brand']);
		$html .= '<div class="sirius_ledger_stripe_card_namenumber">' . ucfirst($paymentmethod_json['card']['brand']) . ' **** ' . $paymentmethod_json['card']['last4'] . '</div>';
		$html .= '<div class="sirius_ledger_stripe_card_expires">' . t('Expires ') . $paymentmethod_json['card']['exp_month'] . ' / ' . $paymentmethod_json['card']['exp_year'] . '</div>';
	} else if ($type == 'us_bank_account') {
		$html .= sirius_ledger_stripe_render_cc_icon('us_bank_account');
		$html .= '<div class="sirius_ledger_stripe_usbank_namenumber">' . ucfirst($paymentmethod_json['us_bank_account']['bank_name']) . '</div>';
		$html .= '<div class="sirius_ledger_stripe_usbank_type">' . $paymentmethod_json['us_bank_account']['account_type'] . ' / ' . $paymentmethod_json['us_bank_account']['account_holder_type'] . '</div>';
	} else {
		$html .= "[Unknown payment method type: " . $paymentmethod_json['type'] . "]";
	}
	$html .= '</div>'; 

	return $html;
}

function sirius_ledger_stripe_paymentmethods_options($entity_node, $paymentmethods_json) {
	if (!$paymentmethods_json || !$paymentmethods_json['data'][0]) { return array(); }

	$paymentmethods_options = array();
	foreach ($paymentmethods_json['data'] as $idx => $paymentmethod_json) {
		$type = $paymentmethod_json['type'];
		if ($type == 'card') {
			$label = ucfirst($paymentmethod_json['card']['brand']) . 
			' **** ' . $paymentmethod_json['card']['last4'];
		} else if ($type == 'us_bank_account') {
			$label = ucfirst($paymentmethod_json['us_bank_account']['bank_name']);
		} else {
			$label = "Unknown type: " . $paymentmethod_json['type'];
		}
		$paymentmethods_options[$paymentmethod_json['id']] = $label;
	}
	return $paymentmethods_options;
}

/****************************************************************************************
 * Webhook endpoints
 ****************************************************************************************/

function sirius_ledger_stripe_webhookendpoint_url() {
	global $base_url;
	$domain_nid = sirius_domain_nid();
	$url = $base_url . "/sirius/stripe/webhookendpoint/$domain_nid";
	return $url;
}

function sirius_ledger_stripe_webhookendpoint_events() {
	return array(
		'setup_intent.canceled',
		'setup_intent.created',
		'setup_intent.requires_action',
		'setup_intent.setup_failed',
		'setup_intent.succeeded',
	);
}

function sirius_ledger_stripe_webhookendpoint_create() {
	$stripe = sirius_ledger_stripe_client_get();
	if (!$stripe) { return array('success' => FALSE, 'msg' => 'sirius_ledger_stripe_client_get() failed.'); }
	try {
		$webhookendpoint = $stripe->webhookEndpoints->create(
			array(
				'url' => sirius_ledger_stripe_webhookendpoint_url(), 
				'enabled_events' => sirius_ledger_stripe_webhookendpoint_events()
			)
		);
	} catch (\Exception $e) {
		return array('success' => FALSE, 'msg' => $e->getMessage());
	}
	return array('success' => TRUE, 'webhookendpoint' => $webhookendpoint);
}

function sirius_ledger_stripe_webhookendpoint_update($webhookendpoint_id) {
	$stripe = sirius_ledger_stripe_client_get();
	if (!$stripe) { return array('success' => FALSE, 'msg' => 'sirius_ledger_stripe_client_get() failed.'); }
	try {
		$webhookendpoint = $stripe->webhookEndpoints->update(
			$webhookendpoint_id, 
			array(
				'url' => sirius_ledger_stripe_webhookendpoint_url(), 
				'enabled_events' => sirius_ledger_stripe_webhookendpoint_events()
			)
		);

	} catch (\Exception $e) {
		return array('success' => FALSE, 'msg' => $e->getMessage());
	}
	return array('success' => TRUE, 'webhookendpoint' => $webhookendpoint);
}

function sirius_ledger_stripe_webhookendpoint_retrieve($webhookendpoint_id) {
	$stripe = sirius_ledger_stripe_client_get();
	if (!$stripe) { return array('success' => FALSE, 'msg' => 'sirius_ledger_stripe_client_get() failed.'); }
	try {
		$webhookendpoint = $stripe->webhookEndpoints->retrieve($webhookendpoint_id, array());
	} catch (\Exception $e) {
		return array('success' => FALSE, 'msg' => $e->getMessage());
	}
	return array('success' => TRUE, 'webhookendpoint' => $webhookendpoint);
}

function sirius_ledger_stripe_webhookendpoint_delete($webhookendpoint_id) {
	$stripe = sirius_ledger_stripe_client_get();
	if (!$stripe) { return array('success' => FALSE, 'msg' => 'sirius_ledger_stripe_client_get() failed.'); }
	try {
		$webhookendpoint = $stripe->webhookEndpoints->delete($webhookendpoint_id, array());
	} catch (\Exception $e) {
		return array('success' => FALSE, 'msg' => $e->getMessage());
	}
	return array('success' => TRUE);
}

function sirius_ledger_stripe_webhook_handle($domain_nid) {
	$orig_domain_nid = sirius_domain_nid();

	// Switch into the new domain
	$domain_node = node_load($domain_nid);
	if (!$domain_nid || !$domain_node || ($domain_node->type != 'sirius_domain')) { sirius_ledger_stripe_webhook_error($orig_domain_nid, "Domain [$domain_nid] not found, or not a valid domain."); }

	sirius_domain($domain_nid);

	// Validate the request and construct an event
	$endpoint_secret = sirius_domain_variable_get('sirius_ledger_stripe_webhookendpoint_secret');
	if (!$endpoint_secret) { sirius_ledger_stripe_webhook_error($orig_domain_nid, "sirius_ledger_stripe_webhookendpoint_secret not found."); }

	$payload = @file_get_contents('php://input');
	if (!$payload) { sirius_ledger_stripe_webhook_error($orig_domain_nid, "No payload."); }

	$sig_header = $_SERVER['HTTP_STRIPE_SIGNATURE'];
	if (!$sig_header) { sirius_ledger_stripe_webhook_error($orig_domain_nid, "No signature (missing STRIPE_SIGNATURE in _REQUEST."); }

	$stripe = sirius_ledger_stripe_client_get();
	if (!$stripe) { sirius_ledger_stripe_webhook_error($orig_domain_nid, "Unable to create a Stripe client."); }

	try {
    $event = \Stripe\Webhook::constructEvent(
			$payload, $sig_header, $endpoint_secret
    );
	} catch (\Exception $e) {
		sirius_ledger_stripe_webhook_error($orig_domain_nid, 'Stripe authentication error: ' . $e->getMessage());
	}

	// Switch into the Stripe user
  $user_name = sirius_domain_variable_get('sirius_ledger_stripe_user_name');
  if ($user_name) {
    global $user;
    $user = user_load_by_name($user_name);
    if (!$user) { sirius_ledger_stripe_webhook_error($orig_domain_nid, "Unable to switch to user $user_name.");}
  }

	watchdog('sirius_ledger_stripe_webhook', 'Received webhook callback: <pre>' . print_r(sirius_ledger_stripe_object_to_json($event), 1) . '</pre>');
	http_response_code(200);
	print "Ok.";
	sirius_domain($orig_domain_nid);
	drupal_exit();
}

function sirius_ledger_stripe_webhook_error($orig_domain_nid, $msg) {
	sirius_domain($orig_domain_nid);
	watchdog('sirius_ledger_stripe', "Webhook error: $msg");
	http_response_code(400);
	print $msg;
	drupal_exit();
}