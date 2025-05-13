<?php

function sirius_ledger_stripe_webhook_handle($domain_nid) {
	return sirius_ledger_service()->webhook_handle($domain_nid); 
}

trait Sirius_Ledger_Service_Stripe_Webhooks {
	public function webhookendpoint_url() {
		global $base_url;
		$domain_nid = sirius_domain_nid();
		$url = $base_url . "/sirius/stripe/webhookendpoint/$domain_nid";
		return $url;
	}

	public function webhookendpoint_events() {
		return array(
			'setup_intent.canceled',
			'setup_intent.created',
			'setup_intent.requires_action',
			'setup_intent.setup_failed',
			'setup_intent.succeeded',
			'payment_intent.amount_capturable_updated',
			'payment_intent.canceled',
			'payment_intent.created',
			'payment_intent.partially_funded',
			'payment_intent.payment_failed',
			'payment_intent.processing',
			'payment_intent.requires_action',
			'payment_intent.succeeded',
		);
	}

	public function webhookendpoint_create() {
		$stripe = $this->client();
		if (!$stripe) { return array('success' => FALSE, 'msg' => 'client() failed.'); }
		try {
			$webhookendpoint = $stripe->webhookEndpoints->create(
				array(
					'url' => $this->webhookendpoint_url(), 
					'enabled_events' => $this->webhookendpoint_events()
				)
			);
		} catch (\Exception $e) {
			return array('success' => FALSE, 'msg' => $e->getMessage());
		}
		return array('success' => TRUE, 'webhookendpoint' => $webhookendpoint);
	}

	public function webhookendpoint_update($webhookendpoint_id) {
		$stripe = $this->client();
		if (!$stripe) { return array('success' => FALSE, 'msg' => 'client() failed.'); }
		try {
			$webhookendpoint = $stripe->webhookEndpoints->update(
				$webhookendpoint_id, 
				array(
					'url' => $this->webhookendpoint_url(), 
					'enabled_events' => $this->webhookendpoint_events()
				)
			);

		} catch (\Exception $e) {
			return array('success' => FALSE, 'msg' => $e->getMessage());
		}
		return array('success' => TRUE, 'webhookendpoint' => $webhookendpoint);
	}

	function webhookendpoint_retrieve($webhookendpoint_id) {
		$stripe = $this->client();
		if (!$stripe) { return array('success' => FALSE, 'msg' => 'client() failed.'); }
		try {
			$webhookendpoint = $stripe->webhookEndpoints->retrieve($webhookendpoint_id, array());
		} catch (\Exception $e) {
			return array('success' => FALSE, 'msg' => $e->getMessage());
		}
		return array('success' => TRUE, 'webhookendpoint' => $webhookendpoint);
	}

	function webhookendpoint_delete($webhookendpoint_id) {
		$stripe = $this->client();
		if (!$stripe) { return array('success' => FALSE, 'msg' => 'client() failed.'); }
		try {
			$webhookendpoint = $stripe->webhookEndpoints->delete($webhookendpoint_id, array());
		} catch (\Exception $e) {
			return array('success' => FALSE, 'msg' => $e->getMessage());
		}
		return array('success' => TRUE);
	}

	function webhook_handle($domain_nid) {
		watchdog('sirius_ledger_stripe_webhook', 'Received Stripe webhook callback...');
		$orig_domain_nid = sirius_domain_nid();

		// Switch into the new domain
		$domain_node = node_load($domain_nid);
		if (!$domain_nid || !$domain_node || ($domain_node->type != 'sirius_domain')) { return $this->webhook_error($orig_domain_nid, "Domain [$domain_nid] not found, or not a valid domain."); }

		sirius_domain($domain_nid);

		// Validate the request and construct an event
		$endpoint_secret = sirius_domain_variable_get('sirius_ledger_stripe_webhookendpoint_secret');
		if (!$endpoint_secret) { return $this->webhook_error($orig_domain_nid, "sirius_ledger_stripe_webhookendpoint_secret not found."); }

		$payload = @file_get_contents('php://input');
		if (!$payload) { return $this->webhook_error($orig_domain_nid, "No payload."); }

		$sig_header = $_SERVER['HTTP_STRIPE_SIGNATURE'];
		if (!$sig_header) { return $this->webhook_error($orig_domain_nid, "No signature (missing STRIPE_SIGNATURE in _REQUEST."); }

		$stripe = $this->client();
		if (!$stripe) { return $this->webhook_error($orig_domain_nid, "Unable to create a Stripe client."); }

		try {
	    $event = \Stripe\Webhook::constructEvent(
				$payload, $sig_header, $endpoint_secret
	    );
		} catch (\Exception $e) {
			return $this->webhook_error($orig_domain_nid, 'Stripe authentication error: ' . $e->getMessage());
		}

		// Switch into the Stripe user
	  $user_name = sirius_domain_variable_get('sirius_ledger_stripe_user_name');
	  if ($user_name) {
	    global $user;
	    $user = user_load_by_name($user_name);
	    if (!$user) { return $this->webhook_error($orig_domain_nid, "Unable to switch to user $user_name.");}
	  }

	  $event_json = $this->object_to_json($event);
	  list ($event_category, $event_type) = explode('.', $event['type']);

	  if ($event_category == 'setup_intent') {
	  	$result = array('success' => FALSE, 'msg' => 'Payment method webhooks not implemented yet.');
	  } else if ($event_category == 'payment_intent') {

	  	return $this->paymentintent_webhook_handle($event_json);
	  } else {
			return $this->webhook_error("Don't know how to handle webhook of type $event_category.$event_type: <pre>" . print_r($event_json, 1) . '</pre>');
		}

		if (!$result['success']) {
			watchdog('sirius_ledger_stripe_webhook', "Stripe webhook failed. Msg = $result[msg]. <pre>" . print_r($event_json, 1) . '</pre>');
		}
		http_response_code(200);
		return drupal_json_output(array('success' => TRUE));
	}

	function webhook_error($orig_domain_nid, $msg) {
		sirius_domain($orig_domain_nid);
		watchdog('sirius_ledger_stripe', "Webhook error: $msg");
		http_response_code(400);
		return drupal_json_output(array('success' => FALSE, 'msg' => $msg));
	}
}