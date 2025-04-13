<?php

trait Sirius_Ledger_Service_Stripe_PaymentMethods {

	public function paymentmethod_list_multitype($customer_id, $types) {
		$result_json = array('success' => TRUE, 'paymentmethods' => array('data' => array()));
		foreach ($types as $type) {
			$result = $this->paymentmethod_list($customer_id, $type);
			if (!$result['success']) { return $result; }
			$tmp = $this->object_to_json($result['paymentmethods']);
			if ($tmp['data']) {
				foreach ($tmp['data'] as $paymentmethod) {
					$result_json['paymentmethods']['data'][] = $paymentmethod;
				}
			}
			$result_json['paymentmethods']['data'] += $tmp['data'];
		}
		return $result_json;
	}

	public function paymentmethod_list($customer_id, $type = NULL) {
		$stripe = $this->client();
		if (!$stripe) { return array('success' => FALSE, 'msg' => 'client() failed.'); }
		try {
			$params = array('customer' => $customer_id);
			if ($type) { $params['type'] = $type; }
			$paymentmethods = $stripe->paymentMethods->all($params);
		} catch (\Exception $e) {
			return array('success' => FALSE, 'msg' => $e->getMessage());
		}
		return array('success' => TRUE, 'paymentmethods' => $paymentmethods);
	}

	public function paymentmethod_add($entity_node) {
		$stripe = $this->client();
		if (!$stripe) { return array('success' => FALSE, 'msg' => 'client() failed.'); }

		$customer_id = $entity_node->uuid;
		if (!$customer_id) { return array('success' => FALSE, 'msg' => 'No customer_id (no uuid in the entity node).'); }

		// Get a SetupIntent
		$result = $this->setupintent_create($customer_id);
		if (!$result['success']) { return $result; }
		$setupintent = $result['setupintent'];

		$_SESSION['client_secret'] = $setupintent->client_secret;
		drupal_goto('sirius/ledger/stripe/paymentmethod/add', array('query' => array('destination' => current_path())));

		return array('success' => TRUE);
	}

	public function setupintent_create($customer_id) {
		$stripe = $this->client();
		if (!$stripe) { return array('success' => FALSE, 'msg' => 'client() failed.'); }

		try {
			$setupintent = $stripe->setupIntents->create(array('customer' => $customer_id, 'payment_method_types' => array('card', 'us_bank_account')));
		} catch (\Exception $e) {
			return array('success' => FALSE, 'msg' => $e->getMessage());
		}

		return array('success' => TRUE, 'setupintent' => $setupintent);
	}

	public function paymentmethod_charge($entity_node, $amount, $account_nid, $paymentmethod_id) {
		$stripe = $this->client();
		if (!$stripe) { return array('success' => FALSE, 'msg' => 'client() failed.'); }

		$customer_id = $entity_node->uuid;
		if (!$customer_id) { return array('success' => FALSE, 'msg' => 'No customer_id (no uuid in the entity node).'); }

		/*
		// Here is how Stripe transaction fees will be calculated.
		$face = $amount + $fee;
		$fee = (0.029 * $face) + 0.3;
		$face = $amount + (0.029*$face) + 0.3;
		(1-0.029)*$face = $amount + 0.3;
		$face = ($amount + 0.3) / (1-0.029);
		drupal_set_message("Here $with $face, $fee");
		return array('success' => FALSE);

		$entry = array();
		$entry['ledger_amount'] = $payment_node->field_sirius_dollar_amt['und'][0]['value'];
		$entry['ledger_account'] = $payment_node->field_sirius_ledger_account['und'][0]['target_id'];
		$entry['ledger_status'] = $payment_node->field_sirius_payment_status['und'][0]['value'];
		$payment_type_term = taxonomy_term_load($payment_node->field_sirius_payment_type['und'][0]['tid']);
		$payer_node = node_load($payment_node->field_sirius_payer['und'][0]['target_id']);
		$entry['ledger_memo'] = sirius_ledger_payment_memo($payment_node);
		$entry['ledger_ts'] = $ts;
		$entry['ledger_reference'] = $payment_node->nid;
		$entry['ledger_participant'] = $payer_node->nid;
		sirius_ledger_ar_insert($entry);
		*/

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

		$result = $this->paymentmethod_list($customer_id);
		if ($result['success']) { $paymentmethods_json = $result['paymentmethods']; }
		$paymentmethods_options = $this->paymentmethods_options($entity_node, $paymentmethods_json);


		$payment_params = array();
		$payment_params['notes'] = $paymentmethods_options[$paymentmethod_id];
		if ($entity_node->type == 'sirius_worker') {
			$payment_params['payer_type'] = 'worker';
			$payment_params['payer_worker'] = $entity_node->field_sirius_id['und'][0]['value'];
			$payment_params['payer_employer'] = $entity_node->field_grievance_shop['und'][0]['target_id'];
			$payment_params['ledger_participant'] = $entity_node->field_sirius_contact['und'][0]['target_id'];
		}
		else if ($entity_node->type == 'grievance_shop') {
			$payment_params['payer_type'] = 'employer';
			$payment_params['payer_employer'] = $entity_node->nid;
			$payment_params['ledger_participant'] = $entity_node->nid;
		}
		else { 
			return array(
				'success' => FALSE, 
				'msg' => 'Don\'t know how to construct a payment node for an object of type ' . $entity_node->type
			);
		}

		$paymentintent_json = $this->object_to_json($paymentintent);

		$payment_params['ledger_account'] = $account_nid;
		$payment_params['dollar_amt'] = $payment_params['ledger_am'] = -1 * $amount;
		$payment_params['payment_type'] = sirius_domain_variable_get('sirius_ledger_stripe_payment_type');
		$payment_params['merchant_name'] = 'Stripe';
		$payment_params['json'] = array(
			'stripe' => array('paymentintent' => $paymentintent_json),
			'external_id' => $paymentintent_json['id'],
		);
		$payment_params['payment_status'] = 'Pending';
		$payment_params['allocate'] = 'Yes';
		$payment_params['datetime_created'] = date('Y-m-d H:i:s');
		$payment_params['datetime'] = date('Y-m-d H:i:s');;
		$payment_params['id'] = $paymentintent_json['id'];
		// $payment_params['notes'] = "Stripe payment to "
		$result = sirius_ledger_payment_create($payment_params);
		$result['paymentintent'] = $paymentintent;

		return $result;
	}

	public function paymentmethod_detach($entity_node, $paymentmethod_id) {
		$stripe = $this->client();
		if (!$stripe) { return array('success' => FALSE, 'msg' => 'client() failed.'); }

		$customer_id = $entity_node->uuid;
		if (!$customer_id) { return array('success' => FALSE, 'msg' => 'No customer_id (no uuid in the entity node).'); }

		try {
			$stripe->paymentMethods->detach($paymentmethod_id);
		} catch (\Exception $e) {
			return array('success' => FALSE, 'msg' => $e->getMessage());
		}

		if ($this->paymentmethod_default_get($entity_node) == $paymentmethod_id) {
			$this->paymentmethod_default_set($entity_node, NULL);
		}

		return array('success' => TRUE);
	}

	public function paymentmethod_default_set($entity_node, $paymentmethod_id) {

		if (!$paymentmethod_id) {
			// We always try to make the default payment method be the first available.
			$customer_id = $entity_node->uuid;
			if (!$customer_id) { return array('success' => FALSE, 'msg' => 'No customer_id (no uuid in the entity node).'); }
			$result = $this->paymentmethod_list($customer_id);
			if ($result['success']) {
				$paymentmethods_json = $result['paymentmethods'];
				foreach ($paymentmethods_json as $idx => $paymentmethod_json) {
					$paymentmethod_id = $paymentmethod_json['id'];
					break;
				}
			}
		}

		$entity_json = sirius_json_get($entity_node);
		$entity_json['stripe']['paymentmethod']['default'] = $paymentmethod_id;
		sirius_json_set($entity_node, $entity_json);
		node_save($entity_node);
		return array('success' => TRUE);
	}

	public function paymentmethod_default_get($entity_node) {
		$entity_json = sirius_json_get($entity_node);
		return $entity_json['stripe']['paymentmethod']['default'];
	}

	public function render_cc_icon($brand) {
		$html = '<div class="sirius_ledger_stripe_cc_icon">';
		if ($brand == 'mastercard') 	{ $html .= '<i class="fab fa-cc-mastercard"></i>'; } 
		else if ($brand == 'visa') 		{ $html .= '<i class="fab fa-cc-visa"></i>'; } 
		else if ($brand == 'amex') 		{ $html .= '<i class="fab fa-cc-amex"></i>'; } 
		else if ($brand == 'us_bank_account') { $html .= '<i class="fas fa-money-check"></i>'; } 
		else 													{ $html .= '<i class="fas fa-credit-card"></i>'; }
		$html .= '</div>';

		return $html;
	}

	public function paymentmethods_render($entity_node, $paymentmethods_json) {
		// Render the payment methods
		$html .= '<div class="sirius_ledger_stripe_paymentmethods">';
		$html .= "<h2>Payment Methods</h2>";

		if (!$paymentmethods_json || !$paymentmethods_json['data'][0]) { 
			$html .= '<em>' . t('No payment methods have been created yet.') . '</em>';
		} else {
			$paymentmethod_default_id = $this->paymentmethod_default_get($entity_node);
			foreach ($paymentmethods_json['data'] as $idx => $paymentmethod_json) {
				$paymentmethod_id = $paymentmethod_json['id'];
				$is_default = ($paymentmethod_id == $paymentmethod_default_id);
				$html .= $this->paymentmethod_render($this->object_to_json($paymentmethod_json), $is_default);
			}
		}

		$html .= '</div>';

		return $html;
	}

	public function paymentmethod_render($paymentmethod_json, $is_default = FALSE) {
		$classes = array();
		$classes[] = 'sirius_ledger_stripe_paymentmethod';
		if ($is_default) { $classes[] = 'sirius_ledger_stripe_paymentmethod_default '; } 
		else { $classes[] = 'sirius_ledger_stripe_paymentmethod_alternative '; }
		$html .= '<div class="' . join(' ', $classes) . '">';

		$type = $paymentmethod_json['type'];
		if ($type == 'card') {
			$html .= $this->render_cc_icon($paymentmethod_json['card']['brand']);
			$html .= '<div class="sirius_ledger_stripe_card_namenumber">' . ucfirst($paymentmethod_json['card']['brand']) . ' **** ' . $paymentmethod_json['card']['last4'] . '</div>';
			$html .= '<div class="sirius_ledger_stripe_card_expires">' . t('Expires ') . $paymentmethod_json['card']['exp_month'] . ' / ' . $paymentmethod_json['card']['exp_year'] . '</div>';
		} else if ($type == 'us_bank_account') {
			$html .= $this->render_cc_icon('us_bank_account');
			$html .= '<div class="sirius_ledger_stripe_usbank_namenumber">' . ucfirst($paymentmethod_json['us_bank_account']['bank_name']) . '</div>';
			$html .= '<div class="sirius_ledger_stripe_usbank_type">' . $paymentmethod_json['us_bank_account']['account_type'] . ' / ' . $paymentmethod_json['us_bank_account']['account_holder_type'] . '</div>';
		} else {
			$html .= "[Unknown payment method type: " . $paymentmethod_json['type'] . "]";
		}
		$html .= '</div>';

		return $html;
	}

	public function paymentmethods_options($entity_node, $paymentmethods_json) {
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

	/**
	 * @todo: This function can probably be thrown away. We don't really need to listen to webhooks on pyament methods,
	 *        since they're being rendered in real time.
	 */

	public function paymentmethod_update($event_json) {
		// Get the customer ID
		$customer_id = $event_json['data']['object']['customer'];
		if (!$customer_id) { return array('success' => FALSE, 'msg' => 'Event payload has no customer.'); }

		// Get the "entity node", which is the host for the customer
		// Why is there no function node_load_by_uuid()?
		$entity_nids = entity_get_id_by_uuid('node', array($customer_id));
		if (!$entity_nids) { $entity_nids = array(); }
		$entity_nid = reset($entity_nids);
		$entity_node = node_load($entity_nid);
		if (!$entity_node) { return array('success' => FALSE, 'msg' => 'Customer not found.'); }

		// @todo: Nothing implemented yet.
		return array('success' => TRUE);
	}
}