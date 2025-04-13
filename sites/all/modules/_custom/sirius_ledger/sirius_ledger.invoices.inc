<?php

class Sirius_Ledger_Invoices extends Sirius_Singleton {
	public function get_header($invoice) {
		$account_node = node_load($invoice['account_nid']);
		$account_json = sirius_json_get($account_node);
		return $account_json['settings']['invoices']['header'];
	}

	public function get_footer($invoice) {
		$account_node = node_load($invoice['account_nid']);
		$account_json = sirius_json_get($account_node);
		return $account_json['settings']['invoices']['footer'];
	}

	public function invoice_load($participant_nid, $account_nid, $interval) {
		$invoice['participant_nid'] = $participant_nid;
		$invoice['account_nid'] = $account_nid;
		$invoice['interval'] = $interval;

		$params = array();
		$params['ledger_participant'] = $participant_nid;
		$params['ledger_account'] = $account_nid;
		$params['ledger_ts_min'] = $interval['start_ts'];
		$params['ledger_ts_max'] = $interval['end_ts'];
		$invoice['items'] = sirius_ledger_ar_fetch($params);

		$invoice['balances'] = $this->invoice_load_balances($participant_nid, $account_nid, $interval);
		$invoice['items'] = sirius_ledger_ar_fetch($params);
		return $invoice;
	}

	public function invoice_load_balances($participant_nid, $account_nid, $interval) {
		$params = array();

		$params['ledger_participant'] = $participant_nid;
		$params['ledger_account'] = $account_nid;
		$params['ledger_ts_min'] = $interval['start_ts'];
		$params['ledger_ts_max'] = $interval['end_ts'];
		$params['balance'] = TRUE;
		$total = sirius_ledger_ar_fetch($params);

		$params['ledger_ts_min'] = NULL;
		$params['ledger_ts_max'] = $interval['start_ts'] - 1;
		$balance_in = sirius_ledger_ar_fetch($params);

		$balance_out = $total + $balance_in;

		$balances = array();
		$balances['total'] = $total;
		$balances['balance_in'] = $balance_in;
		$balances['balance_out'] = $balance_out;
		$balances['account_nid'] = $account_nid;

		return $balances;
	}

	public function invoice_id($participant_nid, $interval) {
		return $participant_nid . '-' . $interval['id'];
	}

	public function invoice_render($invoice) {
		drupal_add_css('sites/all/modules/_custom/sirius_ledger/css/invoice.css');

		$currency = sirius_ledger_currency_info_from_account_nid($invoice['account_nid'])['instance'];

		$html = '';

		$html .= '<div class="sirius_ledger_invoice">';

		$header = $this->get_header($invoice);
		if (strlen($header) > 10) {
			$html .= '<div class="sirius_ledger_invoice_header">' . $header . '</div>' . '<hr>';
		}

		$participant_nid = $invoice['participant_nid'];
		$participant_title = sirius_node_title($participant_nid);

		$account_nid = $invoice['account_nid'];
		$account_title = sirius_node_title($account_nid);

		$invoice_id = $this->invoice_id($participant_nid, $invoice['interval']);

		$html .= '<div class="sirius_ledger_invoice_body">';
		$html .= '<div class="sirius_ledger_invoice_title">';
		$html .= '<h1>' . $participant_title . ' :: ' . $account_title . ' :: ' . t('Invoice #') . $invoice_id . '</h1>';
		$html .= sirius_fakerender_field('Invoice period:', $invoice['interval']['month'] . ' / ' . $invoice['interval']['year']);

		$options = array();
		$options['hide_status'] = TRUE;
		$options['hide_tools'] = TRUE;
		$options['hide_participant'] = TRUE;
		$options['hide_total'] = TRUE;
		$options['hide_account_header'] = TRUE;
		$options['empty_txt'] = t('No new entries this month.');
		$html .= '<h2>' . $record['title'] . '</h2>';

		$html .= sirius_ledger_ar_render($invoice['items'], $options);
		$html .= sirius_fakerender_field('Previous Balance:', $currency->render_amt(-1 * $invoice['balances']['balance_in']));
		$html .= sirius_fakerender_field('Invoice Total:', $currency->render_amt(-1 * $invoice['balances']['total']));
		$html .= sirius_fakerender_field('New Balance:', $currency->render_amt(-1 * $invoice['balances']['balance_out']));
		$html .= '<br />';
		$html .= sirius_fakerender_field('Total Due:', '<strong>' . $currency->render_amt($invoice['balances']['balance_out']) . '</strong>');

		// Render pending payments
		$params = array();
		$params['account_nid'] = $account_nid;
		$params['payer_nid'] = $participant_nid;
		$params['payment_status'] = 'Pending';
		$payments = sirius_ledger_payments_search($params);
		if ($payments) {
			$options = array();
			$options['hide_account'] = TRUE;
			$options['hide_tools'] = TRUE;
			$options['hide_allocated'] = TRUE;
			$html .= '<p>' . t('The following payments are pending and have not yet cleared.') . '</p>';
			$html .= sirius_ledger_payments_render($payments, $options);
		}

		$html .= '</div>';

		global $user;

		$html .= '<div class="sirius_printonly sirius_invoice_printby">';
		$html .= "Printed by: " . $user->name . " &lt;" . $user->mail . '&gt;';
		$html .= ' ' . date('r');
		$html .= '</div>';



		$html .= '</div>';

		$footer = $this->get_footer($invoice);
		if (strlen($footer) > 10) {
			$html .= '<hr>' . '<div class="sirius_ledger_invoice_footer">' . $footer . '</div>';
		}

		$html .= '</div>';
		return $html;
	}

	public function tz() {
		return sirius_do_not_disturb_tz(); 
	}

	public function first_ts($participant_id, $account_nid) {
		$sql = "select min(ledger_ts) as m from sirius_ledger_ar where ledger_participant = :participant_id and ledger_account = :account_nid";
		$stmt = sirius_sql_query($sql, array(':participant_id' => $participant_id, ':account_nid' => $account_nid));
		$hr = $stmt->fetchAssoc();
		if (!$hr) { return NULL; }
		if ($hr['m'] === NULL) { return NULL; }
		$first_ts = $hr['m'];

		$old_tz = date_default_timezone_get();
		date_default_timezone_set($this->tz());
		$cutoff = strtotime("1980-01-01");
		date_default_timezone_set($old_tz);

		if ($first_ts < $cutoff) { $first_ts = $cutoff; }
		return $first_ts;
	}

	public function interval_from_id($interval_id) {
		list ($year, $month) = explode('-', $interval_id);
		if (!($year>0) || !($month>0)) { return array(); }
		return $this->interval_from_ts(strtotime("$year-$month-15"));
	}

	public function interval_from_ts($ts) {
		if ($ts === FALSE) { return array(); }
		if ($ts === NULL) { return array(); }

		$old_tz = date_default_timezone_get();
		date_default_timezone_set($this->tz());
		$year = date('Y', $ts);
		$month = date('n', $ts);
		$month0 = date('m', $ts);
		$last_day = date('t', $ts);
		$start_ts = strtotime("$year-$month-1 00:00:00");
		$end_ts = strtotime("$year-$month-$last_day 23:59:59");
		date_default_timezone_set($old_tz);

		$now_ts = time();
		if ($end_ts < $now_ts) { $status = 'past'; }
		else if ($start_ts <= $now_ts) { $status = 'current'; }
		else { $status = 'future'; }

		return array(
			'year' => $year, 
			'month' => $month,
			'start_ts' => $start_ts,
			'end_ts' => $end_ts,
			'last_day' => $last_day,
			'id' => "$year-$month0",
			'status' => $status,
		);
	}

	public function first_interval($participant_id, $account_nid) {
		return $this->interval_from_ts($this->first_ts($participant_id, $account_nid));
	}

	public function next_interval($interval) {
		$ts = strtotime("+1 month", strtotime($interval['year'] . '-' . $interval['month'] . '-15'));
		return $this->interval_from_ts($ts);
	}
	public function prev_interval($interval) {
		$ts = strtotime("-1 month", strtotime($interval['year'] . '-' . $interval['month'] . '-15'));
		return $this->interval_from_ts($ts);
	}
	public function now_interval() {
		return $this->interval_from_ts(time());
	}

	public function intervals($participant_id, $account_nid) {
		$intervals = array();

		$interval = $this->first_interval($participant_id, $account_nid);
		if (!$interval) { return array(); }
		while ($interval) {
			$intervals[$interval['id']] = $interval;

			if ($interval['status'] != 'past') { break; }
			$interval = $this->next_interval($interval);

			++$no_infinite_loops; if ($no_infinite_loops > 1000) { break; }
		}

		return $intervals;
	}
}