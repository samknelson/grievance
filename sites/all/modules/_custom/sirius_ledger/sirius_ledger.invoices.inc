<?php

class Sirius_Ledger_Invoices extends Sirius_Singleton {
	public function get_header() {
		return sirius_domain_variable_get('sirius_ledger_invoice_header');
	}

	public function get_footer() {
		return sirius_domain_variable_get('sirius_ledger_invoice_footer');
	}

	public function invoice_load($participant_nid, $interval) {
		$accounts = $this->accounts($participant_nid, $interval);

		$invoice = array();
		$invoice['participant_nid'] = $participant_nid;
		$invoice['interval'] = $interval;

		$items_by_account = array();
		foreach ($accounts as $account_nid => $account_title) {

			$items_by_account[$account_nid] = array('title' => $account_title);

			$params = array();
			$params['ledger_participant'] = $participant_nid;
			$params['ledger_account'] = $account_nid;

			$params['ledger_ts_min'] = $interval['start_ts'];
			$params['ledger_ts_max'] = $interval['end_ts'];
			$items_by_account[$account_nid]['items'] = sirius_ledger_ar_fetch($params);

			$params['ledger_participant'] = $participant_nid;
			$params['ledger_ts_min'] = $interval['start_ts'];
			$params['ledger_ts_max'] = $interval['end_ts'];
			$params['balance'] = TRUE;
			$total = sirius_ledger_ar_fetch($params);
		}

		$invoice['balances'] = $this->invoice_load_balances($participant_nid, $interval);
		$invoice['items_by_account'] = $items_by_account;
		return $invoice;
	}

	public function invoice_load_balances($participant_nid, $interval) {
		$accounts = $this->accounts($participant_nid, $interval);
		$balances = array();
		foreach ($accounts as $account_nid => $account_title) {
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

			$balances[$account_nid]['total'] = $total;
			$balances[$account_nid]['balance_in'] = $balance_in;
			$balances[$account_nid]['balance_out'] = $balance_out;
		}

		return $balances;
	}

	public function accounts($participant_nid, $interval) {
		// Get the list of accounts
		$sql = "select distinct ledger_account from sirius_ledger_ar where ledger_participant = :participant_nid";
		$stmt = sirius_sql_query($sql, array(':participant_nid' => $participant_nid));
		$tmp = array();
		while ($hr = $stmt->fetchAssoc()) { $tmp[] = $hr['ledger_account']; }
		$accounts = sirius_ledger_account_list();
		foreach ($accounts as $account_nid => $account_title) {
			if (!in_array($account_nid, $tmp)) { unset($accounts[$account_nid]); }
		}
		return $accounts;
	}

	public function invoice_id($participant_nid, $interval) {
		return $participant_nid . '-' . $interval['id'];
	}

	public function invoice_render($invoice) {
		drupal_add_css('sites/all/modules/_custom/sirius_ledger/css/invoice.css');

		$html = '';

		$html .= '<div class="sirius_ledger_invoice">';

		$header = $this->get_header();
		if (strlen($header) > 10) {
			$html .= '<div class="sirius_ledger_invoice_header">' . $header . '</div>' . '<hr>';
		}

		$participant_nid = $invoice['participant_nid'];
		$participant_title = sirius_node_title($participant_nid);

		$invoice_id = $this->invoice_id($participant_nid, $invoice['interval']);

		$html .= '<div class="sirius_ledger_invoice_body">';
		$html .= '<div class="sirius_ledger_invoice_title">';
		$html .= '<h1>' . $participant_title . ': ' . t('Invoice #') . $invoice_id . '</h1>';
		$html .= sirius_fakerender_field('Invoice period:', $invoice['interval']['month'] . ' / ' . $invoice['interval']['year']);

		foreach ($invoice['items_by_account'] as $account_nid => $record) {
			$options = array();
			$options['hide_status'] = TRUE;
			$options['hide_tools'] = TRUE;
			$options['hide_participant'] = TRUE;
			$options['hide_total'] = TRUE;
			$options['hide_account_header'] = TRUE;
			$options['empty_txt'] = t('No new entries this month.');
			$html .= '<h2>' . $record['title'] . '</h2>';

			$html .= sirius_ledger_ar_render($record['items'], $options);
			$html .= sirius_fakerender_field('Previous Balance:', sirius_ledger_ar_format_balance($invoice['balances'][$account_nid]['balance_in']));
			$html .= sirius_fakerender_field('Invoice Total:', sirius_ledger_ar_format_balance($invoice['balances'][$account_nid]['total']));
			$html .= sirius_fakerender_field('New Balance:', sirius_ledger_ar_format_balance($invoice['balances'][$account_nid]['balance_out']));
		}

		$html .= '</div>';

		$html .= '</div>';

		$footer = $this->get_footer();
		if (strlen($footer) > 10) {
			$html .= '<hr>' . '<div class="sirius_ledger_invoice_footer">' . $footer . '</div>';
		}

		$html .= '</div>';
		return $html;
	}

	public function tz() {
		return sirius_do_not_disturb_tz(); 
	}

	public function first_ts($participant_id) {
		$sql = "select min(ledger_ts) as m from sirius_ledger_ar where ledger_participant = :participant_id";
		$stmt = sirius_sql_query($sql, array(':participant_id' => $participant_id));
		$hr = $stmt->fetchAssoc();
		if (!$hr) { return NULL; }
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

	public function first_interval($participant_id) {
		return $this->interval_from_ts($this->first_ts($participant_id));
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

	public function intervals($participant_id) {
		$intervals = array();

		$interval = $this->first_interval($participant_id);
		while ($interval) {
			$intervals[$interval['id']] = $interval;

			if ($interval['status'] != 'past') { break; }
			$interval = $this->next_interval($interval);

			++$no_infinite_loops; if ($no_infinite_loops > 1000) { break; }
		}

		return $intervals;
	}
}