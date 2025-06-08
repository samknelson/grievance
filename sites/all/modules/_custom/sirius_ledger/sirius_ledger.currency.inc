<?php

function sirius_ledger_currency_info_from_account_nid($account_nid) {
	$account_nodes = &drupal_static(__FUNCTION__);
	if (!isset($account_nodes[$account_nid])) {
		$account_nodes[$account_nid] = node_load($account_nid);
	}

	if ($account_nodes[$account_nid]) {
		$type = $account_nodes[$account_nid]->field_sirius_currency['und'][0]['value'];
	}

	if (!$type) { $type = 'USD'; }

	return sirius_plugin_info('ledger_currency', $type);
}

function sirius_ledger_currency_from_account_nid($account_nid) {
	return sirius_ledger_currency_info_from_account_nid($account_nid)['instance'];
}

class Sirius_Ledger_Currency_Plugin extends Sirius_Singleton {
	public $type = 'root';
	public $name = 'Root';
	public $description = 'Generic currency root.';

	public function plugin_class() { return 'ledger_currency'; }

	public function render_amt($amt) {
		$class = "sirius_currency";
		if ($amt >= 0) {$class .= " sirius_currency_positive"; }
		else { $class .= "sirius_currency_negative"; }

		$html = "<span class=\"$class\">";
		$html .= '$';
		$html .= number_format($amt, 2);
		$html .= '</span>';

		return $html;
	}

	public function validate_amt($amt) {
		if (!is_numeric($amt) || !($amt > 0)) { return array('success' => FALSE, 'msg' => 'Please enter a positive number.'); }

		if ($amt != round($amt, 2)) { return array('success' => FALSE, 'msg' => 'Please enter a valid dollar amount.'); }

		$max = sirius_domain_variable_get('sirius_ledger_payment_max', 100000);
		if ($amt > $max) { return array('success' => FALSE, 'msg' => "Please enter a value less than " . $this->render_amt($max)); }

		$min = sirius_domain_variable_get('sirius_ledger_payment_min', 2);
		if ($amt < $min) {  return array('success' => FALSE, 'msg' => "Please enter a value greater than " . $this->render_amt($min)); }

		return array('success' => TRUE, 'amt' => $amt);
	}
}

class Sirius_Ledger_Currency_Plugin_USD extends Sirius_Ledger_Currency_Plugin {
	public $type = 'USD';
	public $name = 'USD';
	public $description = 'U.S. Dollars';

	public function render_amt($amt) {
		if (!$amt) { $amt = 0; }
		$class = "";
		if ($amt >= 0) {
			$html = "<span class=\"sirius_currency_usd sirius_currency_usd_positive\">";
			$html .= '$';
			$html .= number_format($amt, 2);
			$html .= '</span>';
		} else {
			$html = "<span class=\"sirius_currency_usd sirius_currency_usd_negative\">";
			$html .= '(';
			$html .= '$';
			$html .= number_format(-1 * $amt, 2);
			$html .= ')';
			$html .= '</span>';
		}

		return $html;
	}
}

class Sirius_Ledger_Currency_Plugin_Points extends Sirius_Ledger_Currency_Plugin {
	public $type = 'points';
	public $name = 'Points';
	public $description = '"Points" or other non-monetary awards.';

	public function render_amt($amt) {
		if (!$amt) { $amt = 0; }
		$class = "";
		if ($amt >= 0) {
			$html = "<span class=\"sirius_currency_points sirius_currency_points_positive\">";
			$html .= number_format($amt, 0);
			$html .= ' Points';
			$html .= '</span>';
		} else {
			$html = "<span class=\"sirius_currency_points sirius_currency_points_negative\">";
			$html .= '(';
			$html .= number_format(-1 * $amt, 0);
			$html .= ' Points';
			$html .= ')';
			$html .= '</span>';
		}

		return $html;
	}
}
