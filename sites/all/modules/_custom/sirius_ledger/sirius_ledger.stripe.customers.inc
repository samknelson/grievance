<?php

trait Sirius_Ledger_Service_Stripe_Customers {
	public function customer_render($customer_json) {
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

			$url = $this->object_url($customer_json);
			$html .= '<div class="sirius_ledger_stripe_customer_links">';
			$html .= "<a href=\"$url\" class=\"tool-button\" target=\"_blank\">";
			$html .= "View in Stripe";
			$html .= '</a>';
			$html .= '</div>';
		} else {
			$html .= '<em>' . t('No customer has been created yet.') . '</em>';
		}
		// $html .= '<br clear="left" />';
		$html .= '</div>';

		$html .= '</div>';

		return $html;
	}

	public function customer_sync($entity_node) {
		$stripe = $this->client();
		if (!$stripe) { return array('success' => FALSE, 'msg' => 'client() failed.'); }

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

		return array('success' => TRUE, 'data' => $data, 'customer' => $customer);
	}

	public function customer_retrieve($customer_id) {
		$stripe = $this->client();
		if (!$stripe) { return array('success' => FALSE, 'msg' => 'client() failed.'); }

		try {
			$customer = $stripe->customers->retrieve($customer_id);
		} catch (\Exception $e) {
			return array('success' => FALSE, 'msg' => $e->getMessage());
		}

		return array('success' => TRUE, 'customer' => $customer);
	}
}