<?php

trait Sirius_Ledger_Service_Stripe_Payments {
	public function payment_node_view($payment_node, $view_mode, $langcode) {
		$payment_json = sirius_json_get($payment_node);
		$paymentintent_id = $payment_json['stripe']['paymentintent']['id'];
		if (!$paymentintent_id) { return; }

	  drupal_add_css('sites/all/modules/_custom/sirius_ledger/css/stripe.css');
		$payment_node->content['stripe'] = array(
			'#weight' => 100,
			'#markup' => $this->paymentintent_render($payment_json['stripe']['paymentintent']),
		);
	}

	public function paymentintent_render($paymentintent_json) {
		$html = '<div class="sirius_ledger_stripe_paymentintent">'; 
		$html .= '<div class="sirius_ledger_stripe_customer_links">';
		$url = $this->object_url($paymentintent_json);
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
}