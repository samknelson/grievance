<?php

class Sirius_Trust_Elgibility_Plugin_Arrears extends Sirius_Trust_Elgibility_Plugin {
	public $type = 'arrears';
	public $name = 'Arrears';
	public $description = 'Members lose eligiblity if their account is more than the specified number of months in arrears -- i.e. if the balance as of 3 months ago is greater than their payments in the last three months.';

	public function form($json) {
		if (!sirius_ledger_account_list()) {
			return sirius_form_error("There are no ledger accounts.");
		}

		$form['account'] = array(
			'#type' => 'select',
			'#title' => t('Account'),
			'#options' => array(NULL => t('-- Select --')) + sirius_ledger_account_list(),
		);

		$form['warning_months'] = array(
			'#type' => 'textfield',
			'#size' => 4,
			'#title' => t('Warning Months'),
			'#description' => t('Generate a warning when the the account is this many months in arrears.'),
		);

		$form['max_months'] = array(
			'#type' => 'textfield',
			'#size' => 4,
			'#title' => t('Maximum Months'),
			'#description' => t('Remove eligibility when the the account is this many months in arrears.'),
		);

		sirius_jsonfield_util_set_defaults($form, $json);

		return $form;
	}

	public function check($settings, $params) {
		$account_nid = $settings['account'];
		if (!$account_nid) {
			return array('success' => TRUE, 'msg' => 'No account specified.');
		}
		$currency = sirius_ledger_currency_from_account_nid($account_nid);

		$max_months = $settings['max_months'];
		if (!is_numeric($max_months)) { return array('success' => FALSE, 'msg' => 'Please enter a numeric value for max months.'); }

		$warning_months = $settings['warning_months'];
		if (!is_numeric($warning_months)) { return array('success' => FALSE, 'msg' => 'Please enter a numeric value for warning months.'); }

		$ea = Sirius_Ledger_EA::getInstance();
		$ea_node = $ea->fetch($params['subscriber_worker_node']->nid, $account_nid, FALSE);
		if (!$ea_node) { return array('success' => TRUE, 'msg' => 'No charges or payments found.'); }
		$currency = sirius_ledger_currency_from_account_nid($account_nid);

		$date1 = date('Y-m-t 23:59:59', strtotime("-$max_months months", $params['ts']));
		$date1_render = date('F Y', strtotime($date1));

		$date2 = date('Y-m-t 23:59:59', strtotime("-$warning_months months", $params['ts']));
		$date2_render = date('F Y', strtotime($date2));

		$ts1 = strtotime($date1);
		$ts2 = strtotime($date2);

		$search_params = array();
		$search_params['balance'] = TRUE;
		$search_params['ledger_ts_max'] = $ts1;
		$search_params['ledger_account'] = $account_nid;
		$search_params['ledger_participant']  = $params['subscriber_worker_node']->field_sirius_contact['und'][0]['target_id'];
		$balance1 = sirius_ledger_ar_fetch($search_params);
		$balance1_render = $currency->render_amt($balance1);

		$search_params = array();
		$search_params['balance'] = TRUE;
		$search_params['ledger_ts_max'] = $ts2;
		$search_params['ledger_account'] = $account_nid;
		$search_params['ledger_participant']  = $params['subscriber_worker_node']->field_sirius_contact['und'][0]['target_id'];
		$balance2 = sirius_ledger_ar_fetch($search_params);
		$balance2_render = $currency->render_amt($balance2);

		$search_params = array();
		$search_params['balance'] = TRUE;
		$search_params['ledger_ts_min'] = $ts1;
		$search_params['ledger_account'] = $account_nid;
		$search_params['ledger_participant']  = $params['subscriber_worker_node']->field_sirius_contact['und'][0]['target_id'];
		$search_params['ledger_amount_max'] = 0;
		$payments1 = -1 * sirius_ledger_ar_fetch($search_params);
		$payments1_render = $currency->render_amt($payments1);

		$search_params = array();
		$search_params['balance'] = TRUE;
		$search_params['ledger_ts_min'] = $ts2;
		$search_params['ledger_account'] = $account_nid;
		$search_params['ledger_participant']  = $params['subscriber_worker_node']->field_sirius_contact['und'][0]['target_id'];
		$search_params['ledger_amount_max'] = 0;
		$payments2 = -1 * sirius_ledger_ar_fetch($search_params);
		$payments2_render = $currency->render_amt($payments2);

		if ($balance1 > $payments1) {
			return array('success' => FALSE, 'msg' => "The amount due as of the end of $date1_render was $balance1_render, which is greater than the subsequent payments totaling $payments1_render");
		} if ($balance2 > $payments2) {
		return array('success' => TRUE, 'msg' => "The amount due as of the end of $date2_render was $balance2_render, which is greater than the subsequent payments totaling $payments2_render", 'warning' => TRUE);
		} else {
			return array('success' => TRUE, 'msg' => "The amount due as of the end of $date1_render was less than the subsquent payments totaling $payments1_render.");
		}
	}
}
