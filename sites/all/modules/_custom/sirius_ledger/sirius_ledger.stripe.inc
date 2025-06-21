<?php

require_once DRUPAL_ROOT . '/sites/all/modules/_custom/sirius_ledger/sirius_ledger.stripe.payments.inc';
require_once DRUPAL_ROOT . '/sites/all/modules/_custom/sirius_ledger/sirius_ledger.stripe.customers.inc';
require_once DRUPAL_ROOT . '/sites/all/modules/_custom/sirius_ledger/sirius_ledger.stripe.paymentmethods.inc';
require_once DRUPAL_ROOT . '/sites/all/modules/_custom/sirius_ledger/sirius_ledger.stripe.webhooks.inc';

class Sirius_Ledger_Service_Stripe extends Sirius_Singleton {
	use Sirius_Ledger_Service_Stripe_Payments;
	use Sirius_Ledger_Service_Stripe_Customers;
	use Sirius_Ledger_Service_Stripe_PaymentMethods;
	use Sirius_Ledger_Service_Stripe_Webhooks;

	public function client($mode = NULL) {
		if (!$mode) { $mode = $this->mode(); }

		$stripe = &drupal_static(__FUNCTION__);

		if (!isset($stripe[$mode])) {

			if (!sirius_domain_component_enabled('ledger:stripe')) {
				sirius_minilog("The Stripe component is not enabled for this domain.", 'error');
				return NULL; 
			}

			if (!$this->active()) {
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

	public function mode() {
		return sirius_domain_variable_get('sirius_ledger_stripe_mode', 'test');
	}

	public function active() {
		return (sirius_domain_variable_get('sirius_ledger_stripe_active') == 'Yes');
	}

	public function object_to_json($object) {
		if (!$object) { return array(); }
		return json_decode($object->toJSON(), TRUE);
	}

	public function object_url($json) {
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

	public function ping($mode = NULL) {
		sirius_minilog("Stripe ping test: starting.");
		if ($mode) {
			sirius_minilog("Mode = $mode");
		} else {
			$mode = $this->mode();
			sirius_minilog("Mode = System Default [$mode]");
		}

		if (!$this->active()) { return array('success' => FALSE, 'msg' => "Stripe is not active for this domain."); }

		if (!sirius_domain_variable_get('sirius_ledger_stripe_' . $mode . '_account_secret')) { return array('success' => FALSE, 'msg' => 'No secret key has been set.'); }

		$stripe = $this->client($mode);
		if (!$stripe) { return array('success' => FALSE, 'msg' => 'client() failed.'); }

		try {
			$balance = $stripe->balance->retrieve();
			if (!$balance) { return array('success' => FALSE, 'msg' => 'Failed to retrieve account balance object.'); }
			$data = $this->object_to_json($balance);
		} catch (\Exception $e) {
			return array('success' => FALSE, 'msg' => $e->getMessage());
		}

		return array('success' => TRUE, 'msg' => 'Request succeeded', 'data' => $data);
	}

	public function fetch_by_id($id, $nid_only = FALSE) {
		if (preg_match('/^pi/', $id)) {
			return $this->paymentintent_fetch_by_id($id, $nid_only);
		}
		return NULL;
	}
}
